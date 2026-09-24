// Standalone regression/feature suite for mod-realm-config.
//
// Compiles the production translation unit (src/mod_realm_config.cpp) against
// small AzerothCore API doubles (tests/api/*.h) and exercises the parser,
// contract detection, content synthesis, validator, serializer, and publisher.
// Run with `bash tests/run.sh`.

#include "../src/mod_realm_config.cpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
int g_checks = 0;
int g_failures = 0;

#define REQUIRE(cond)                                                        \
    do {                                                                     \
        ++g_checks;                                                          \
        if (!(cond)) {                                                       \
            ++g_failures;                                                    \
            std::cerr << "FAIL(" << __FILE__ << ":" << __LINE__ << "): "     \
                      << #cond << "\n";                                      \
        }                                                                    \
    } while (0)

#define REQUIRE_THROW(expr)                                                  \
    do {                                                                     \
        ++g_checks;                                                          \
        bool threw_ = false;                                                 \
        try { (void)(expr); } catch (std::exception const&) { threw_ = true; } \
        if (!threw_) {                                                       \
            ++g_failures;                                                    \
            std::cerr << "FAIL(" << __FILE__ << ":" << __LINE__              \
                      << "): expected throw: " #expr "\n";                   \
        }                                                                    \
    } while (0)

std::string const SHA64 = std::string(64, 'a');

Settings MakeSettings()
{
    Settings s;
    s.OutputDirectory = "/tmp/realm-config-out";
    s.RealmKey = "test";
    s.Name = "Test Realm";
    s.Address = "realm.example.com";
    s.Description = "";
    s.WebsiteURL = "https://example.com/";
    s.ClientVersion = "3.3.5a";
    s.ClientBuild = "12340";
    s.AuthPort = "3724";
    s.WorldPort = "8085";
    s.ConfigURL = "https://example.com/realm.conf";
    s.Executable = "Wow.exe";
    s.ExecutableSHA256 = "";
    s.MinimumVersion = "0.1.0";
    s.ManifestURL = "";
    s.NewsURL = "";
    s.StatusURL = "";
    s.CalendarURL = "";
    s.ArmoryURL = "";
    s.ContentBaseURL = "";
    return s;
}

void AddExampleCatalog(Settings& s)
{
    s.Addons.push_back({"ExampleAddon", "Example Addon", "Required", "GitHub",
        "https://github.com/example/ExampleAddon", "main", "ExampleAddon"});
    s.Patches.push_back({"ExamplePatch", "Example Realm Patch", "Required", "HTTP",
        "https://example.com/patch-X.MPQ", "patch-X.MPQ", "Data", "",
        PatchInstallMode::File});
}

QueryResult::Row MetadataRow(std::string const& type, std::string const& key,
    std::vector<std::string> const& values)
{
    QueryResult::Row row;
    row.emplace_back(type);
    row.emplace_back(std::uint64_t(0));
    row.emplace_back(key);
    for (std::size_t i = 0; i < 19; ++i)
        row.emplace_back(i < values.size() ? values[i] : "");
    return row;
}

QueryResult::Row SingletonRow()
{
    return MetadataRow("0", "", {
        "test", "Test Realm", "realm.example.com", "", "https://example.com/",
        "3.3.5a", "12340", "3724", "8085", "https://example.com/realm.conf",
        "Wow.exe", "", "0.1.0", "", "", "", "", "",
        ""});
}

QueryResult::Row BuildRow(std::string const& number, std::string const& realmName,
    std::string const& filename, std::string const& sha256)
{
    return MetadataRow("3", "", {number, realmName, filename, sha256});
}

QueryResult::Row RequirementRow(std::string const& requirement, std::string const& number)
{
    return MetadataRow("4", requirement, {number});
}

QueryResult::Row AddonRow()
{
    return MetadataRow("1", "ExampleAddon", {"Example Addon", "Required", "GitHub",
        "https://github.com/example/ExampleAddon", "main", "ExampleAddon"});
}

QueryResult::Row PatchRow()
{
    return MetadataRow("2", "ExamplePatch", {"Example Realm Patch", "Required", "HTTP",
        "https://example.com/patch-X.MPQ", "patch-X.MPQ", "Data", ""});
}

Settings Load(std::vector<QueryResult::Row> rows)
{
    return LoadMetadata(QueryResult(std::move(rows)), "/tmp/realm-config-out");
}

std::vector<std::string> SectionLines(std::string const& output, std::string const& name)
{
    std::vector<std::string> lines;
    std::istringstream ss(output);
    std::string line;
    bool inside = false;
    while (std::getline(ss, line))
    {
        if (!line.empty() && line.front() == '[')
        {
            inside = (line == "[" + name + "]");
            continue;
        }
        if (inside && !line.empty()) lines.push_back(line);
    }
    return lines;
}

bool Contains(std::string const& haystack, std::string const& needle)
{
    return haystack.find(needle) != std::string::npos;
}

std::string RunPublication(Settings& settings, bool content, std::string const& realmName)
{
    SynthesizeRealmContent(settings, content, realmName);
    ValidateSettings(settings);
    return BuildConfiguration(settings);
}
}
// namespace

// Definitions for the stub globals declared extern by tests/api headers.
ConfigMgr* sConfigMgr = nullptr;
DatabaseWorkerPool WorldDatabase;
DatabaseWorkerPool LoginDatabase;
Realm realm;

int main()
{
    sConfigMgr = new ConfigMgr();
    realm.Name = "Test Realm";
    realm.Id.Realm = 1;

    {
        // 1/13. Golden output: existing generation stays valid and unchanged
        // apart from the new Requirements= line.
        auto settings = MakeSettings();
        AddExampleCatalog(settings);
        auto output = BuildConfiguration(settings);
        std::string const expected =
            "# Generated by mod-realm-config.\n"
            "# Public configuration consumed by Portalkeeper.\n"
            "# Do not edit this file manually.\n"
            "\n[Config]\nSchemaVersion=1\n"
            "\n[Realm]\nName=Test Realm\nDescription=\nWebsiteURL=https://example.com/\n"
            "\n[Connection]\nAddress=realm.example.com\nAuthPort=3724\nWorldPort=8085\n"
            "\n[Client]\nVersion=3.3.5a\nBuild=12340\nExecutable=Wow.exe\n"
            "ExecutableSHA256=\nRuntimeMode=Legacy\nRequirements=\n"
            "\n[Portalkeeper]\nMinimumVersion=0.1.0\n"
            "\n[Services]\nManifestURL=\nNewsURL=\nStatusURL=\nCalendarURL=\n"
            "ArmoryURL=\nConfigURL=https://example.com/realm.conf\n"
            "\n[Addon.ExampleAddon]\nName=Example Addon\nRequirement=Required\n"
            "SourceType=GitHub\nSourceURL=https://github.com/example/ExampleAddon\n"
            "Ref=main\nInstallDirectory=ExampleAddon\n"
            "\n[Patch.ExamplePatch]\nName=Example Realm Patch\nRequirement=Required\n"
            "SourceType=HTTP\nSourceURL=https://example.com/patch-X.MPQ\n"
            "FileName=patch-X.MPQ\nInstallDirectory=Data\nSHA256=\n";
        REQUIRE(output == expected);

        // 2. Existing [Client] keys are unchanged; Requirements is additive.
        auto client = SectionLines(output, "Client");
        REQUIRE(client.size() == 6);
        REQUIRE(client[0] == "Version=3.3.5a");
        REQUIRE(client[1] == "Build=12340");
        REQUIRE(client[2] == "Executable=Wow.exe");
        REQUIRE(client[3] == "ExecutableSHA256=");
        REQUIRE(client[4] == "RuntimeMode=Legacy");
        REQUIRE(client[5] == "Requirements=");
    }

    {
        // 11/12. RuntimeMode Legacy and Isolated behavior is unchanged.
        auto legacy = MakeSettings();
        legacy.ClientRuntimeMode = "Legacy";
        auto outLegacy = BuildConfiguration(legacy);
        REQUIRE(Contains(outLegacy, "RuntimeMode=Legacy\nRequirements="));

        auto isolated = MakeSettings();
        isolated.ClientRuntimeMode = "Isolated";
        auto outIsolated = BuildConfiguration(isolated);
        REQUIRE(Contains(outIsolated, "RuntimeMode=Isolated\nRequirements="));
    }

    {
        // JoinClientRequirements: empty, single, deduplicated+deterministic.
        REQUIRE(JoinClientRequirements({}) == "");
        REQUIRE(JoinClientRequirements({"protected-framexml"}) == "protected-framexml");
        REQUIRE(JoinClientRequirements({"b", "a", "b", "c"}) == "a,b,c");
        REQUIRE(JoinClientRequirements({"zesty", "apple", "banana"}) == "apple,banana,zesty");
    }

    {
        // 3. No Content Manager integration: generation still succeeds and
        // requirements are empty.
        auto settings = MakeSettings();
        AddExampleCatalog(settings);
        auto output = RunPublication(settings, false, "Test Realm");
        REQUIRE(Contains(output, "Requirements=\n"));
        REQUIRE(!Contains(output, "Patch.realm-content"));
        REQUIRE(settings.ClientRequirements.empty());
    }

    {
        // 4. ACTIVE legacy/pre-Schema-3 build with zero requirement rows.
        auto settings = Load({SingletonRow(),
            BuildRow("12", "Test Realm", "Test-Realm-Content-000012.mpq", SHA64)});
        settings.ContentBaseURL = "https://example.com/download/";
        auto output = RunPublication(settings, true, "Test Realm");
        REQUIRE(Contains(output, "Requirements=\n"));
        REQUIRE(Contains(output, "[Patch.realm-content]"));
        REQUIRE(settings.ClientRequirements.empty());
    }

    {
        // 5. ACTIVE Schema-3 build requiring protected-framexml.
        auto settings = Load({SingletonRow(),
            BuildRow("12", "Test Realm", "Test-Realm-Content-000012.mpq", SHA64),
            RequirementRow("protected-framexml", "12")});
        settings.ContentBaseURL = "https://example.com/download/";
        REQUIRE(settings.ActiveBuilds.size() == 1);
        REQUIRE(settings.ActiveBuilds[0].ClientRequirements ==
            std::vector<std::string>{"protected-framexml"});
        auto output = RunPublication(settings, true, "Test Realm");
        REQUIRE(Contains(output, "Requirements=protected-framexml\n"));
        REQUIRE(settings.ClientRequirements ==
            std::vector<std::string>{"protected-framexml"});
    }

    {
        // 6/7. Multiple returned and duplicate requirements: deterministic,
        // comma-separated, deduplicated output in byte-wise order.
        auto settings = Load({SingletonRow(),
            BuildRow("12", "Test Realm", "Test-Realm-Content-000012.mpq", SHA64),
            RequirementRow("zesty", "12"),
            RequirementRow("apple", "12"),
            RequirementRow("protected-framexml", "12"),
            RequirementRow("banana", "12"),
            RequirementRow("apple", "12")});
        settings.ContentBaseURL = "https://example.com/download/";
        REQUIRE(settings.ActiveBuilds[0].ClientRequirements ==
            (std::vector<std::string>{"apple", "banana", "protected-framexml", "zesty"}));
        auto output = RunPublication(settings, true, "Test Realm");
        REQUIRE(Contains(output,
            "Requirements=apple,banana,protected-framexml,zesty\n"));
        REQUIRE(!Contains(output, "Requirements=,"));
    }

    {
        // 8. Requirements source: the requirement SELECT reads the exact ACTIVE
        // build rows only, never package/EPF/patchhold state.
        auto sqlBoth = MetadataSQL({true, true});
        REQUIRE(Contains(sqlBoth, "content_manager_build_client_requirement"));
        REQUIRE(Contains(sqlBoth, "WHERE build_number IN (SELECT build_number "
            "FROM content_manager_build WHERE BINARY state = 'ACTIVE')"));
        auto sqlContentOnly = MetadataSQL({true, false});
        REQUIRE(!Contains(sqlContentOnly, "content_manager_build_client_requirement"));
        REQUIRE(Contains(sqlContentOnly, "content_manager_build"));
        auto sqlNone = MetadataSQL({false, false});
        REQUIRE(!Contains(sqlNone, "content_manager_build"));
        // Requirements stay attached to the build object independently of any
        // catalog (package) rows loaded in the same snapshot.
        auto settings = Load({SingletonRow(), AddonRow(), PatchRow(),
            BuildRow("12", "Test Realm", "Test-Realm-Content-000012.mpq", SHA64),
            RequirementRow("protected-framexml", "12")});
        REQUIRE(settings.Addons.size() == 1);
        REQUIRE(settings.Patches.size() == 1);
        REQUIRE(settings.ActiveBuilds[0].ClientRequirements ==
            std::vector<std::string>{"protected-framexml"});
    }

    {
        // 9. ACTIVE build belonging to another realm: neither its patch nor its
        // requirements are advertised.
        auto settings = Load({SingletonRow(),
            BuildRow("20", "Other Realm", "Other-Realm-Content-000020.mpq", SHA64),
            RequirementRow("protected-framexml", "20")});
        settings.ContentBaseURL = "https://example.com/download/";
        auto status = SynthesizeRealmContent(settings, true, "Test Realm");
        REQUIRE(Contains(status, "not advertised"));
        REQUIRE(settings.Patches.empty());
        REQUIRE(settings.ClientRequirements.empty());
        auto output = BuildConfiguration(settings);
        REQUIRE(!Contains(output, "Patch.realm-content"));
        REQUIRE(Contains(output, "Requirements=\n"));
    }

    {
        // ACTIVE build owned by the realm is advertised with its requirements.
        auto settings = MakeSettings();
        settings.ContentBaseURL = "https://example.com/download/";
        settings.ActiveBuilds.push_back({"30", "Test Realm",
            "Test-Realm-Content-000030.mpq", SHA64, {"protected-framexml"}});
        auto status = SynthesizeRealmContent(settings, true, "Test Realm");
        REQUIRE(Contains(status, "available; ACTIVE realm content build 30"));
        REQUIRE(settings.Patches.size() == 1);
        REQUIRE(settings.Patches[0].Key == "realm-content");
        REQUIRE(settings.Patches[0].InstallMode == PatchInstallMode::WowPatch);
        REQUIRE(settings.ClientRequirements ==
            std::vector<std::string>{"protected-framexml"});
    }

    {
        // content_base_url unset: no build is advertised, so no requirements.
        auto settings = MakeSettings();
        settings.ActiveBuilds.push_back({"12", "Test Realm",
            "Test-Realm-Content-000012.mpq", SHA64, {"protected-framexml"}});
        auto status = SynthesizeRealmContent(settings, true, "Test Realm");
        REQUIRE(Contains(status, "content_base_url is unset"));
        REQUIRE(settings.Patches.empty());
        REQUIRE(settings.ClientRequirements.empty());
    }

    {
        // Multiple ACTIVE builds remain ambiguous and are rejected.
        auto settings = Load({SingletonRow(),
            BuildRow("12", "Test Realm", "Test-Realm-Content-000012.mpq", SHA64),
            BuildRow("13", "Test Realm", "Test-Realm-Content-000013.mpq", SHA64)});
        settings.ContentBaseURL = "https://example.com/download/";
        REQUIRE_THROW(SynthesizeRealmContent(settings, true, "Test Realm"));
    }

    {
        // Contract detection: happy paths and incompatible installs.
        auto contract = ReadContentContract(QueryResult(std::vector<QueryResult::Row>{
            {Field(std::uint64_t(1)), Field(std::uint64_t(5)),
             Field(std::uint64_t(1)), Field(std::uint64_t(2))}}));
        REQUIRE(contract.content && contract.requirements);

        auto legacy = ReadContentContract(QueryResult(std::vector<QueryResult::Row>{
            {Field(std::uint64_t(1)), Field(std::uint64_t(5)),
             Field(std::uint64_t(0)), Field(std::uint64_t(0))}}));
        REQUIRE(legacy.content && !legacy.requirements);

        auto absent = ReadContentContract(QueryResult(std::vector<QueryResult::Row>{
            {Field(std::uint64_t(0)), Field(std::uint64_t(0)),
             Field(std::uint64_t(0)), Field(std::uint64_t(0))}}));
        REQUIRE(!absent.content && !absent.requirements);

        // Incompatible requirement-table columns must not be treated as legacy.
        REQUIRE_THROW(ReadContentContract(QueryResult(std::vector<QueryResult::Row>{
            {Field(std::uint64_t(1)), Field(std::uint64_t(5)),
             Field(std::uint64_t(1)), Field(std::uint64_t(3))}})));
        REQUIRE_THROW(ReadContentContract(QueryResult(std::vector<QueryResult::Row>{
            {Field(std::uint64_t(1)), Field(std::uint64_t(4)),
             Field(std::uint64_t(1)), Field(std::uint64_t(2))}})));
        REQUIRE_THROW(ReadContentContract(QueryResult()));

        // 10. A failed/absent snapshot cannot masquerade as Requirements=.
        REQUIRE_THROW(LoadMetadata(QueryResult(), "/tmp/realm-config-out"));
        QueryResult::Row shortRow{Field("0"), Field(std::uint64_t(0)), Field("")};
        REQUIRE_THROW(LoadMetadata(QueryResult(std::vector<QueryResult::Row>{shortRow}),
            "/tmp/realm-config-out"));
        // Requirement rows must reference an ACTIVE build from the same snapshot.
        REQUIRE_THROW(Load(std::vector<QueryResult::Row>{SingletonRow(),
            BuildRow("12", "Test Realm", "Test-Realm-Content-000012.mpq", SHA64),
            RequirementRow("protected-framexml", "99")}));
    }

    {
        // 10 (behavioral). Requirement lookup failure preserves the last
        // known-good file instead of silently publishing Requirements=.
        auto directory = fs::temp_directory_path() /
            ("realm-config-failsafe-" + std::to_string(std::chrono::steady_clock::now()
                .time_since_epoch().count()));
        auto knownGood = MakeSettings();
        knownGood.OutputDirectory = directory.string();
        AddExampleCatalog(knownGood);
        PublishConfiguration(knownGood, BuildConfiguration(knownGood));

        auto bad = MakeSettings();
        bad.OutputDirectory = directory.string();
        bad.ContentBaseURL = "https://example.com/download/";
        bad.ActiveBuilds.push_back({"12", "Test Realm",
            "Test-Realm-Content-000012.mpq", SHA64,
            {"protected-framexml,extra"}}); // invalid published token
        bool published = false;
        try
        {
            auto output = RunPublication(bad, true, "Test Realm");
            PublishConfiguration(bad, output);
            published = true;
        }
        catch (std::exception const&) { }

        auto saved = fs::path(directory) / "test.realm.conf";
        std::ifstream in(saved, std::ios::in | std::ios::binary);
        std::ostringstream contents; contents << in.rdbuf();
        REQUIRE(!published);
        REQUIRE(contents.str() == BuildConfiguration(knownGood));
        std::error_code ignored;
        fs::remove_all(directory, ignored);
    }

    {
        // Validation: semantically invalid requirement tokens are rejected.
        auto settings = MakeSettings();
        settings.ClientRequirements = {"protected framexml"};
        REQUIRE_THROW(ValidateSettings(settings));
        settings.ClientRequirements = {"protected-framexml,"};
        REQUIRE_THROW(ValidateSettings(settings));
        settings.ClientRequirements = {""};
        REQUIRE_THROW(ValidateSettings(settings));
        settings.ClientRequirements = {std::string(65, 'a')};
        REQUIRE_THROW(ValidateSettings(settings));
        settings.ClientRequirements = {"protected-framexml"};
        REQUIRE(SynthesizeRealmContent(settings, false, "Test Realm") == "unavailable");
        ValidateSettings(settings); // valid token round-trips through validation
        REQUIRE(true);
    }

    {
        // Publisher: atomic publication writes the expected filename/content.
        auto directory = fs::temp_directory_path() /
            ("realm-config-publish-" + std::to_string(std::chrono::steady_clock::now()
                .time_since_epoch().count()));
        auto settings = MakeSettings();
        settings.OutputDirectory = directory.string();
        auto output = BuildConfiguration(settings);
        fs::path target = PublishConfiguration(settings, output);
        REQUIRE(target == directory / "test.realm.conf");
        REQUIRE(fs::is_regular_file(target));
        std::ifstream in(target, std::ios::in | std::ios::binary);
        std::ostringstream contents; contents << in.rdbuf();
        REQUIRE(contents.str() == output);
        std::error_code ignored;
        fs::remove_all(directory, ignored);
    }

    {
        // Determinism: identical settings produce byte-identical files.
        auto a = MakeSettings();
        AddExampleCatalog(a);
        auto b = MakeSettings();
        AddExampleCatalog(b);
        REQUIRE(BuildConfiguration(a) == BuildConfiguration(b));
        REQUIRE(BuildConfiguration(a) == BuildConfiguration(a));
    }

    delete sConfigMgr;
    if (g_failures)
    {
        std::cerr << g_failures << " of " << g_checks << " checks FAILED\n";
        return 1;
    }
    std::cout << "All " << g_checks << " checks passed\n";
    return 0;
}