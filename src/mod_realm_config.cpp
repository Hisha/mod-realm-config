#include "Config.h"
#include "DatabaseEnv.h"
#include "Field.h"
#include "QueryResult.h"
#include "Log.h"
#include "ScriptMgr.h"

#include <atomic>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <optional>
#include <set>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace
{
namespace fs = std::filesystem;

struct Addon
{
    std::string Key, Name, Requirement, SourceType, SourceURL, Ref, InstallDirectory;
};

struct Patch
{
    std::string Key, Name, Requirement, SourceType, SourceURL, FileName, InstallDirectory, SHA256;
};

struct Settings
{
    std::string OutputDirectory;
    std::string RealmKey;
    std::string Name;
    std::string Address;
    std::string Description;
    std::string WebsiteURL;
    std::string ClientVersion;
    std::string ClientBuild;
    std::string AuthPort;
    std::string WorldPort;
    std::string ConfigURL;
    std::string Executable, ExecutableSHA256, MinimumVersion;
    std::string ManifestURL, NewsURL, StatusURL, CalendarURL, ArmoryURL;
    std::vector<Addon> Addons;
    std::vector<Patch> Patches;
};

// One statement gives a consistent snapshot across all three InnoDB tables.
// Empty catalogs still return the singleton; any missing table/query error fails closed.
// Control columns type/order/key precede 18 explicitly whitelisted text columns.
constexpr char MetadataQuery[] = R"SQL(
SELECT
       CONVERT('0' USING utf8mb4) COLLATE utf8mb4_unicode_ci AS record_type,
       0 AS sort_order,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci AS record_key,
       CONVERT(realm_key USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f0,
       CONVERT(name USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f1,
       CONVERT(address USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f2,
       CONVERT(description USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f3,
       CONVERT(website_url USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f4,
       CONVERT(client_version USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f5,
       CONVERT(CAST(client_build AS CHAR) USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f6,
       CONVERT(CAST(auth_port AS CHAR) USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f7,
       CONVERT(CAST(world_port AS CHAR) USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f8,
       CONVERT(config_url USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f9,
       CONVERT(client_executable USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f10,
       CONVERT(client_executable_sha256 USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f11,
       CONVERT(portalkeeper_minimum_version USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f12,
       CONVERT(manifest_url USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f13,
       CONVERT(news_url USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f14,
       CONVERT(status_url USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f15,
       CONVERT(calendar_url USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f16,
       CONVERT(armory_url USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f17
FROM mod_realm_config
WHERE id = 1
UNION ALL
SELECT
       CONVERT('1' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       sort_order,
       CONVERT(addon_key USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT(name USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT(requirement USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT(source_type USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT(source_url USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT(source_ref USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT(install_directory USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci
FROM mod_realm_config_addon
WHERE enabled = 1
UNION ALL
SELECT
       CONVERT('2' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       sort_order,
       CONVERT(patch_key USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT(name USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT(requirement USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT(source_type USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT(source_url USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT(file_name USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT(install_directory USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT(sha256 USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci
FROM mod_realm_config_patch
WHERE enabled = 1
ORDER BY record_type, sort_order, BINARY record_key
)SQL";

Settings LoadMetadata(QueryResult const& result, std::string const& directory)
{
    if (!result)
        throw std::runtime_error("Schema v1 SQL query failed or singleton id=1 is missing; apply module migrations and check database logs");
    if (result->GetFieldCount() != 21)
        throw std::runtime_error("unexpected Schema v1 SQL result shape");
    Settings settings;
    settings.OutputDirectory = directory;
    bool found = false;
    do
    {
        Field* fields = result->Fetch();
        for (unsigned i = 0; i < 21; ++i)
            if (fields[i].IsNull())
                throw std::runtime_error("mod_realm_config SQL data contains an unexpected NULL");
        auto value = [fields](unsigned i) { return fields[i + 3].Get<std::string>(); };
        auto type = fields[0].Get<std::string>();
        if (type == "0")
        {
            if (found) throw std::runtime_error("duplicate singleton configuration row");
            found = true;
            settings.RealmKey = value(0);
            settings.Name = value(1);
            settings.Address = value(2);
            settings.Description = value(3);
            settings.WebsiteURL = value(4);
            settings.ClientVersion = value(5);
            settings.ClientBuild = value(6);
            settings.AuthPort = value(7);
            settings.WorldPort = value(8);
            settings.ConfigURL = value(9);
            settings.Executable = value(10);
            settings.ExecutableSHA256 = value(11);
            settings.MinimumVersion = value(12);
            settings.ManifestURL = value(13);
            settings.NewsURL = value(14);
            settings.StatusURL = value(15);
            settings.CalendarURL = value(16);
            settings.ArmoryURL = value(17);
        }
        else if (type == "1")
            settings.Addons.push_back({fields[2].Get<std::string>(), value(0), value(1),
                value(2), value(3), value(4), value(5)});
        else if (type == "2")
            settings.Patches.push_back({fields[2].Get<std::string>(), value(0), value(1),
                value(2), value(3), value(4), value(5), value(6)});
        else throw std::runtime_error("unexpected Schema v1 SQL record type");
    } while (result->NextRow());
    if (!found) throw std::runtime_error("mod_realm_config singleton id=1 is missing");
    return settings;
}

void ValidateText(std::string const& value, char const* key, bool required)
{
    auto fail = [key]() { throw std::runtime_error(std::string("Invalid ") + key +
        " must be single-line UTF-8 text without control characters or surrounding spaces"); };
    if (required && value.empty())
        throw std::runtime_error(std::string("Invalid ") + key + " cannot be empty");
    if (!value.empty() && (value.front() == ' ' || value.back() == ' '))
        fail();
    for (std::size_t i = 0; i < value.size();)
    {
        unsigned char c = static_cast<unsigned char>(value[i++]);
        if (c < 0x20 || c == 0x7f)
            fail(); // Includes CR, LF and NUL: values cannot inject INI sections/keys.
        if (c < 0x80)
            continue;
        unsigned count;
        std::uint32_t point;
        std::uint32_t minimum;
        if (c >= 0xc2 && c <= 0xdf) { count = 1; point = c & 0x1f; minimum = 0x80; }
        else if (c >= 0xe0 && c <= 0xef) { count = 2; point = c & 0x0f; minimum = 0x800; }
        else if (c >= 0xf0 && c <= 0xf4) { count = 3; point = c & 0x07; minimum = 0x10000; }
        else { fail(); return; }
        while (count--)
        {
            if (i == value.size()) { fail(); return; }
            c = static_cast<unsigned char>(value[i++]);
            if ((c & 0xc0) != 0x80) fail();
            point = (point << 6) | (c & 0x3f);
        }
        if (point < minimum || point > 0x10ffff || (point >= 0xd800 && point <= 0xdfff) ||
            (point >= 0x80 && point <= 0x9f) || point == 0x2028 || point == 0x2029)
            fail();
    }
}

void ValidateNumber(std::string& value, char const* key, std::uint32_t maximum)
{
    std::uint32_t number = 0;
    bool valid = !value.empty();
    for (unsigned char c : value)
    {
        if (c < '0' || c > '9' || number > maximum / 10 ||
            (number == maximum / 10 && static_cast<unsigned>(c - '0') > maximum % 10))
        { valid = false; break; }
        number = number * 10 + (c - '0');
    }
    if (!valid || !number)
        throw std::runtime_error(std::string("Invalid ") + key +
            " must be an integer between 1 and " + std::to_string(maximum));
    value = std::to_string(number);
}

void ValidateURL(std::string const& value, char const* key)
{
    ValidateText(value, key, false);
    if (value.empty()) return;
    auto start = value.rfind("https://", 0) == 0 ? 8u : value.rfind("http://", 0) == 0 ? 7u : 0u;
    auto end = value.find_first_of("/?#", start);
    auto authority = value.substr(start, end == std::string::npos ? end : end - start);
    if (!start || authority.empty() || authority.find('@') != std::string::npos ||
        value.find(' ') != std::string::npos || value.find('\\') != std::string::npos)
        throw std::runtime_error(std::string("Invalid ") + key +
            " must be a public http(s) URL without embedded credentials or spaces");
    // Basic authority checks only; no network requests or full URI parser.
    std::string host = authority;
    std::string port;
    if (host.front() == '[')
    {
        auto close = host.find(']');
        if (close == std::string::npos || close <= 1 ||
            (close + 1 < host.size() && host[close + 1] != ':'))
            throw std::runtime_error(std::string(key) + " has an invalid bracketed URL host");
        if (close + 1 < host.size()) port = host.substr(close + 2);
        host = host.substr(1, close - 1);
        if (host.find(':') == std::string::npos ||
            host.find_first_not_of("0123456789abcdefABCDEF:.") != std::string::npos)
            throw std::runtime_error(std::string(key) + " has an invalid IPv6 URL host");
    }
    else
    {
        auto colon = host.find(':');
        if (colon != std::string::npos) { port = host.substr(colon + 1); host.resize(colon); }
        if (host.empty() || host.find_first_not_of(
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._") != std::string::npos)
            throw std::runtime_error(std::string(key) + " has an invalid URL hostname (use ASCII/punycode)");
    }
    if (authority.back() == ':')
        throw std::runtime_error(std::string(key) + " has an empty URL port");
    if (!port.empty()) ValidateNumber(port, key, 65535);
}

std::string FoldASCII(std::string value)
{
    for (char& c : value) if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
    return value;
}

void ValidateKey(std::string const& value)
{
    if (value.empty() || value.size() > 64 ||
        !std::all_of(value.begin(), value.end(), [](unsigned char c)
        { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') || c == '_' || c == '-'; }))
        throw std::runtime_error("section key must contain 1..64 ASCII letters, digits, underscores or hyphens");
}

void ValidateRealmKey(std::string const& value)
{
    if (value.empty() || value.size() > 64 ||
        !std::all_of(value.begin(), value.end(), [](unsigned char c)
        {
            return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_' || c == '-';
        }))
        throw std::runtime_error("mod_realm_config.realm_key must contain 1..64 lowercase ASCII letters, digits, underscores or hyphens");
}

void ValidateHash(std::string& value, char const* key)
{
    if (value.empty()) return;
    if (value.size() != 64 || !std::all_of(value.begin(), value.end(), [](unsigned char c)
        { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }))
        throw std::runtime_error(std::string(key) + " must be empty or exactly 64 hexadecimal characters");
    value = FoldASCII(value);
}

void ValidateRelativePath(std::string const& value, char const* key, bool singleComponent)
{
    ValidateText(value, key, true);
    if (value.find_first_of("\\:<>\"|?*") != std::string::npos || value.front() == '/')
        throw std::runtime_error(std::string(key) + " must be a portable relative path using forward slashes");
    std::size_t start = 0;
    do
    {
        auto end = value.find('/', start);
        auto part = value.substr(start, end == std::string::npos ? end : end - start);
        if (part.empty() || part == "." || part == ".." || part.front() == ' ' ||
            part.back() == ' ' || part.back() == '.' || (singleComponent && end != std::string::npos))
            throw std::runtime_error(std::string(key) + " contains an unsafe path component");
        auto stem = FoldASCII(part.substr(0, part.find('.')));
        if (stem == "con" || stem == "prn" || stem == "aux" || stem == "nul" ||
            (stem.size() == 4 && (stem.substr(0, 3) == "com" || stem.substr(0, 3) == "lpt") &&
             stem[3] >= '1' && stem[3] <= '9'))
            throw std::runtime_error(std::string(key) + " contains a reserved device name");
        if (end == std::string::npos) break;
        start = end + 1;
    } while (true);
}

void ValidateSource(std::string const& requirement, std::string const& type, std::string const& url)
{
    if (requirement != "Required" && requirement != "Recommended" && requirement != "Optional")
        throw std::runtime_error("Requirement must be Required, Recommended or Optional (case sensitive)");
    if (type != "GitHub" && type != "HTTP")
        throw std::runtime_error("SourceType must be GitHub or HTTP (case sensitive)");
    ValidateText(url, "SourceURL", true);
    ValidateURL(url, "SourceURL");
}

void ValidateCatalogs(Settings& settings)
{
    std::set<std::string> addonKeys, patchKeys;
    for (auto& addon : settings.Addons)
    {
        try
        {
            ValidateKey(addon.Key);
            if (!addonKeys.insert(FoldASCII(addon.Key)).second) throw std::runtime_error("duplicate addon key");
            ValidateText(addon.Name, "Name", true);
            ValidateSource(addon.Requirement, addon.SourceType, addon.SourceURL);
            ValidateText(addon.Ref, "Ref", false);
            ValidateRelativePath(addon.InstallDirectory, "InstallDirectory", true);
        }
        catch (std::exception const& error)
        {
            // Do not echo unvalidated database content into logs.
            throw std::runtime_error("addon record #" + std::to_string(&addon - settings.Addons.data() + 1) +
                " in SQL sort order: " + error.what());
        }
    }
    for (auto& patch : settings.Patches)
    {
        try
        {
            ValidateKey(patch.Key);
            if (!patchKeys.insert(FoldASCII(patch.Key)).second) throw std::runtime_error("duplicate patch key");
            ValidateText(patch.Name, "Name", true);
            ValidateSource(patch.Requirement, patch.SourceType, patch.SourceURL);
            ValidateRelativePath(patch.FileName, "FileName", true);
            ValidateRelativePath(patch.InstallDirectory, "InstallDirectory", false);
            ValidateHash(patch.SHA256, "SHA256");
        }
        catch (std::exception const& error)
        {
            throw std::runtime_error("patch record #" + std::to_string(&patch - settings.Patches.data() + 1) +
                " in SQL sort order: " + error.what());
        }
    }
}

void ValidateSettings(Settings& settings)
{
    ValidateText(settings.OutputDirectory, "RealmConfig.OutputDirectory", true);
    ValidateRealmKey(settings.RealmKey);
    ValidateText(settings.Name, "mod_realm_config.name", true);
    ValidateText(settings.Address, "mod_realm_config.address", true);
    if (settings.Address.find_first_of(" /\\\"'\t=#;@") != std::string::npos ||
        (settings.Address.find(':') != std::string::npos &&
         settings.Address.find(':') == settings.Address.rfind(':')))
        throw std::runtime_error("mod_realm_config.address must be a hostname or IP address, without a URL scheme or spaces");
    ValidateText(settings.Description, "mod_realm_config.description", false);
    ValidateText(settings.ClientVersion, "mod_realm_config.client_version", true);
    ValidateURL(settings.WebsiteURL, "mod_realm_config.website_url");
    ValidateURL(settings.ConfigURL, "mod_realm_config.config_url");
    ValidateNumber(settings.ClientBuild, "mod_realm_config.client_build", 65535);
    ValidateNumber(settings.AuthPort, "mod_realm_config.auth_port", 65535);
    ValidateNumber(settings.WorldPort, "mod_realm_config.world_port", 65535);
    ValidateRelativePath(settings.Executable, "Client.Executable", true);
    ValidateHash(settings.ExecutableSHA256, "Client.ExecutableSHA256");
    ValidateText(settings.MinimumVersion, "Portalkeeper.MinimumVersion", true);
    ValidateURL(settings.ManifestURL, "Services.ManifestURL");
    ValidateURL(settings.NewsURL, "Services.NewsURL");
    ValidateURL(settings.StatusURL, "Services.StatusURL");
    ValidateURL(settings.CalendarURL, "Services.CalendarURL");
    ValidateURL(settings.ArmoryURL, "Services.ArmoryURL");
    ValidateCatalogs(settings);
}

using Fields = std::vector<std::pair<std::string, std::string>>;

void AppendSection(std::string& output, char const* name, Fields const& fields)
{
    output += std::string("\n[") + name + "]\n";
    for (auto const& field : fields)
        output += field.first + "=" + field.second + "\n";
}

std::string BuildConfiguration(Settings const& settings)
{
    std::string output = "# Generated by mod-realm-config.\n"
        "# Public configuration consumed by Portalkeeper.\n"
        "# Do not edit this file manually.\n";
    AppendSection(output, "Config", {{"SchemaVersion", "1"}});
    AppendSection(output, "Realm", {{"Name", settings.Name}, {"Description", settings.Description},
        {"WebsiteURL", settings.WebsiteURL}});
    AppendSection(output, "Connection", {{"Address", settings.Address},
        {"AuthPort", settings.AuthPort}, {"WorldPort", settings.WorldPort}});
    AppendSection(output, "Client", {{"Version", settings.ClientVersion}, {"Build", settings.ClientBuild},
        {"Executable", settings.Executable}, {"ExecutableSHA256", settings.ExecutableSHA256}});
    AppendSection(output, "Portalkeeper", {{"MinimumVersion", settings.MinimumVersion}});
    AppendSection(output, "Services", {{"ManifestURL", settings.ManifestURL}, {"NewsURL", settings.NewsURL},
        {"StatusURL", settings.StatusURL}, {"CalendarURL", settings.CalendarURL},
        {"ArmoryURL", settings.ArmoryURL}, {"ConfigURL", settings.ConfigURL}});
    for (auto const& addon : settings.Addons)
        AppendSection(output, ("Addon." + addon.Key).c_str(), {{"Name", addon.Name},
            {"Requirement", addon.Requirement}, {"SourceType", addon.SourceType},
            {"SourceURL", addon.SourceURL}, {"Ref", addon.Ref}, {"InstallDirectory", addon.InstallDirectory}});
    for (auto const& patch : settings.Patches)
        AppendSection(output, ("Patch." + patch.Key).c_str(), {{"Name", patch.Name},
            {"Requirement", patch.Requirement}, {"SourceType", patch.SourceType}, {"SourceURL", patch.SourceURL},
            {"FileName", patch.FileName}, {"InstallDirectory", patch.InstallDirectory}, {"SHA256", patch.SHA256}});
    return output;
}

fs::path RealmConfigFileName(Settings const& settings)
{
    return fs::path(settings.RealmKey + ".realm.conf");
}

struct TemporaryDirectory
{
    fs::path Path;
    ~TemporaryDirectory()
    {
        std::error_code ignored;
        fs::remove_all(Path, ignored);
    }
};

fs::path PublishConfiguration(Settings const& settings, std::string const& output)
{
    fs::path directory = fs::path(settings.OutputDirectory);
    fs::create_directories(directory);
    // Atomically reserve a private staging name, avoiding symlinks and concurrent writers.
    // The staged file stays on the destination filesystem for the final rename.
    static std::atomic<unsigned long long> sequence{0};
    fs::path staging;
    for (unsigned attempt = 0; attempt < 100; ++attempt)
    {
        auto candidate = directory / (".realm-config-" +
            std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) + "-" +
            std::to_string(sequence.fetch_add(1)));
        if (fs::create_directory(candidate)) { staging = std::move(candidate); break; }
    }
    if (staging.empty()) throw std::runtime_error("cannot reserve temporary publication directory");
    TemporaryDirectory temporary{staging};
    fs::permissions(staging, fs::perms::owner_all, fs::perm_options::replace);
    auto stagedFile = staging / RealmConfigFileName(settings);
    std::ofstream stream(stagedFile, std::ios::binary | std::ios::trunc);
    stream.exceptions(std::ios::badbit | std::ios::failbit);
    stream.write(output.data(), static_cast<std::streamsize>(output.size()));
    stream.flush();
    stream.close(); // A close failure also prevents publication.
    auto target = directory / RealmConfigFileName(settings);
    fs::rename(stagedFile, target); // Never remove the previous file to work around a rename error.
    return target;
}

class RealmConfigWorldScript : public WorldScript
{
public:
    RealmConfigWorldScript() : WorldScript("RealmConfigWorldScript") { }

    void OnStartup() override
    {
        _started = true;
        ReloadSettings();
    }

    void OnAfterConfigLoad(bool reload) override
    {
        if (reload && _started) ReloadSettings();
    }

    void OnUpdate(uint32 diff) override
    {
        // Query completion and all filesystem/state operations run on the world thread.
        if (_pending && _pending->InvokeIfReady()) _pending.reset();
        if (!_enabled) return;
        if (diff < _remaining) { _remaining -= diff; return; }
        _remaining = 0;
        if (_pending) return; // At most one outstanding query, even across config reloads.
        _remaining = _interval;
        auto epoch = _epoch;
        try
        {
            _pending.emplace(WorldDatabase.AsyncQuery(MetadataQuery).WithCallback(
                [this, epoch](QueryResult result)
                {
                    if (_enabled && epoch == _epoch) AcceptResult(result);
                }));
        }
        catch (std::exception const& error) { ReportFailure(error.what()); }
    }

private:
    void ReportFailure(std::string const& message)
    {
        // Retry every interval, but don't repeat the same module error indefinitely.
        if (message != _lastError)
            LOG_ERROR("module", "mod-realm-config: publication failed; existing realm.conf preserved: {}", message);
        _lastError = message;
    }

    void AcceptResult(QueryResult const& result)
    {
        try
        {
            auto settings = LoadMetadata(result, _directory);
            ValidateSettings(settings);
            auto output = BuildConfiguration(settings);
            auto target = fs::path(_directory) / RealmConfigFileName(settings);
            if (output != _lastOutput || target != _lastTarget || !fs::is_regular_file(target))
            {
                PublishConfiguration(settings, output);
                _lastOutput = std::move(output);
                _lastTarget = std::move(target);
                LOG_INFO("module", "mod-realm-config: loaded SQL configuration; generated {} with {} addons and {} patches",
                    _lastTarget.string(), settings.Addons.size(), settings.Patches.size());
            }
            else if (!_lastError.empty())
                LOG_INFO("module", "mod-realm-config: database refresh recovered; published metadata is current");
            _lastError.clear();
        }
        catch (std::exception const& error) { ReportFailure(error.what()); }
    }

    void ReloadSettings()
    {
        ++_epoch; // Discard any result queued under an older operational configuration.
        _enabled = false;
        try
        {
            if (!sConfigMgr->GetOption<bool>("RealmConfig.Enable", true))
            {
                LOG_INFO("module", "mod-realm-config: disabled");
                return;
            }
            _directory = sConfigMgr->GetOption<std::string>("RealmConfig.OutputDirectory", "realm-config");
            ValidateText(_directory, "RealmConfig.OutputDirectory", true);
            auto seconds = sConfigMgr->GetOption<std::string>("RealmConfig.RefreshIntervalSeconds", "30");
            ValidateNumber(seconds, "RealmConfig.RefreshIntervalSeconds", 86400);
            _interval = static_cast<uint32>(std::stoul(seconds)) * 1000;
            _remaining = _interval;
            _enabled = true;
            LOG_INFO("module", "mod-realm-config: enabled; checking world database every {} seconds", seconds);
            // One synchronous read at startup/manual config reload; periodic reads are asynchronous.
            AcceptResult(WorldDatabase.Query(MetadataQuery));
        }
        catch (std::exception const& error) { ReportFailure(error.what()); }
    }

    bool _started = false;
    bool _enabled = false;
    uint32 _interval = 30000;
    uint32 _remaining = 30000;
    std::uint64_t _epoch = 0;
    std::string _directory;
    std::string _lastOutput;
    fs::path _lastTarget;
    std::string _lastError;
    std::optional<QueryCallback> _pending;
};
}

void Addmod_realm_configScripts()
{
    new RealmConfigWorldScript();
}