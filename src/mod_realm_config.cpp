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
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace
{
namespace fs = std::filesystem;

struct Settings
{
    std::string OutputDirectory;
    std::string Name;
    std::string Address;
    std::string Description;
    std::string WebsiteURL;
    std::string ClientVersion;
    std::string ClientBuild;
    std::string AuthPort;
    std::string WorldPort;
    std::string UpdateURL;
};

// Single explicit row and column whitelist; no server configuration is exported.
constexpr char MetadataQuery[] =
    "SELECT name, address, description, website_url, client_version, "
    "CAST(client_build AS CHAR), CAST(auth_port AS CHAR), CAST(world_port AS CHAR), update_url "
    "FROM mod_realm_config WHERE id = 1";

Settings LoadMetadata(QueryResult const& result, std::string const& directory)
{
    if (!result)
        throw std::runtime_error("world.mod_realm_config row id=1 is missing or the query failed; "
            "import the module SQL and check database logs");
    Field* fields = result->Fetch();
    for (unsigned i = 0; i < 9; ++i)
        if (fields[i].IsNull())
            throw std::runtime_error("world.mod_realm_config metadata columns must not be NULL");
    return {directory, fields[0].Get<std::string>(), fields[1].Get<std::string>(),
        fields[2].Get<std::string>(), fields[3].Get<std::string>(),
        fields[4].Get<std::string>(), fields[5].Get<std::string>(),
        fields[6].Get<std::string>(), fields[7].Get<std::string>(), fields[8].Get<std::string>()};
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
}

void ValidateSettings(Settings& settings)
{
    ValidateText(settings.OutputDirectory, "RealmConfig.OutputDirectory", true);
    ValidateText(settings.Name, "mod_realm_config.name", true);
    ValidateText(settings.Address, "mod_realm_config.address", true);
    if (settings.Address.find_first_of(" /\\\"'\t=#;@") != std::string::npos ||
        (settings.Address.find(':') != std::string::npos &&
         settings.Address.find(':') == settings.Address.rfind(':')))
        throw std::runtime_error("mod_realm_config.address must be a hostname or IP address, without a URL scheme or spaces");
    ValidateText(settings.Description, "mod_realm_config.description", false);
    ValidateText(settings.ClientVersion, "mod_realm_config.client_version", true);
    ValidateURL(settings.WebsiteURL, "mod_realm_config.website_url");
    ValidateURL(settings.UpdateURL, "mod_realm_config.update_url");
    ValidateNumber(settings.ClientBuild, "mod_realm_config.client_build", 65535);
    ValidateNumber(settings.AuthPort, "mod_realm_config.auth_port", 65535);
    ValidateNumber(settings.WorldPort, "mod_realm_config.world_port", 65535);
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
    std::string output = "# Generated by mod-realm-config. Public launcher metadata.\n"
                         "# Edit world database table mod_realm_config (id=1), not this file.\n";
    Fields server = {{"Name", settings.Name}, {"Address", settings.Address},
        {"AuthPort", settings.AuthPort}, {"WorldPort", settings.WorldPort},
        {"Description", settings.Description}};
    if (!settings.WebsiteURL.empty()) server.emplace_back("WebsiteURL", settings.WebsiteURL);
    AppendSection(output, "Server", server);
    AppendSection(output, "Client", {{"Version", settings.ClientVersion}, {"Build", settings.ClientBuild}});
    if (!settings.UpdateURL.empty())
        AppendSection(output, "Updates", {{"UpdateURL", settings.UpdateURL}});
    // Future Addons/Patches serializers belong here, once Portalkeeper's schema is agreed.
    return output;
}

struct TemporaryDirectory
{
    fs::path Path;
    ~TemporaryDirectory()
    {
        std::error_code ignored;
        fs::remove(Path / "realm.conf", ignored);
        fs::remove(Path, ignored);
    }
};

fs::path PublishConfiguration(Settings const& settings, std::string const& output)
{
    fs::path directory = fs::u8path(settings.OutputDirectory);
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
    auto stagedFile = staging / "realm.conf";
    std::ofstream stream(stagedFile, std::ios::binary | std::ios::trunc);
    stream.exceptions(std::ios::badbit | std::ios::failbit);
    stream.write(output.data(), static_cast<std::streamsize>(output.size()));
    stream.flush();
    stream.close(); // A close failure also prevents publication.
    auto target = directory / "realm.conf";
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
            auto target = fs::u8path(_directory) / "realm.conf";
            if (output != _lastOutput || target != _lastTarget || !fs::is_regular_file(target))
            {
                PublishConfiguration(settings, output);
                _lastOutput = std::move(output);
                _lastTarget = std::move(target);
                LOG_INFO("module", "mod-realm-config: generated {} from world database", _lastTarget.string());
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
            if (!sConfigMgr->GetOption<bool>("RealmConfig.Enable", true)) return;
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
