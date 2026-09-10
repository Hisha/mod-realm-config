# mod-realm-config

`mod-realm-config` is an [AzerothCore](https://www.azerothcore.org/) module that generates and publishes a public `realm.conf` manifest for use by the [Portalkeeper](https://github.com/Hisha/Portalkeeper) launcher.

The module provides a server-side source of truth for the information Portalkeeper needs to connect to and manage a realm, including:

- Realm identity and connection information
- Required WoW client version and build
- Client executable validation
- Portalkeeper compatibility requirements
- Realm service and JSON feed locations
- Required, recommended, and optional addons
- Required, recommended, and optional client patches

The generated `realm.conf` is intended to be hosted somewhere accessible to Portalkeeper, such as a web server or other public download location.

## Architecture

The module follows a simple separation of responsibilities:

```text
AzerothCore / Realm Administration
              |
              v
      mod-realm-config
       Database Tables
              |
              v
      realm.conf Generator
              |
              v
      Published realm.conf
              |
              v
         Portalkeeper
```

AzerothCore controls what the realm requires.

`mod-realm-config` publishes those requirements.

Portalkeeper consumes the resulting configuration.

Portalkeeper does not require access to the AzerothCore database or private worldserver configuration.

## Requirements

- AzerothCore WotLK
- C++17 filesystem support
- Access to the AzerothCore world database
- Write permission to the configured output directory

## Installation

Clone the module into your AzerothCore modules directory:

```bash
cd ~/azerothcore-wotlk/modules
git clone https://github.com/Hisha/mod-realm-config.git
```

Apply the base world database SQL:

```text
data/sql/db-world/base/mod_realm_config.sql
```

Then rebuild AzerothCore normally.

Copy the module configuration:

```bash
cp modules/mod-realm-config/conf/mod_realm_config.conf.dist \
   env/etc/modules/mod_realm_config.conf
```

Adjust the configuration for your environment and restart `worldserver`.

## Module Configuration

Server-side operational settings are stored in:

```text
conf/mod_realm_config.conf.dist
```

Example:

```ini
RealmConfig.Enable = 1
RealmConfig.OutputDirectory = "/mnt/ai_data/linkable/"
RealmConfig.RefreshIntervalSeconds = 60
```

### RealmConfig.Enable

Enables or disables realm configuration generation.

```ini
RealmConfig.Enable = 1
```

When disabled, the module does not generate or modify the published `realm.conf`.

### RealmConfig.OutputDirectory

Directory where the generated configuration is published.

```ini
RealmConfig.OutputDirectory = "/mnt/ai_data/linkable/"
```

The module generates:

```text
<OutputDirectory>/realm.conf
```

The worldserver process must have permission to create and replace files in this directory.

The configured path is server-side only and is never written into the public `realm.conf`.

### RealmConfig.RefreshIntervalSeconds

Controls how often the module checks the database for configuration changes.

When changes are detected, the public `realm.conf` is regenerated.

Use the value documented in `mod_realm_config.conf.dist` for the supported range and default behavior.

## Database

The module uses three world database tables:

```text
mod_realm_config
mod_realm_config_addon
mod_realm_config_patch
```

### mod_realm_config

Contains the singleton realm configuration used to generate the primary sections of `realm.conf`.

This includes information for:

```text
[Realm]
[Connection]
[Client]
[Portalkeeper]
[Services]
```

Typical values include:

- Realm name and description
- Realm website
- Server address and ports
- Client version and build
- Client executable and SHA-256
- Minimum Portalkeeper version
- Realm manifest URL
- News feed URL
- Status feed URL
- Calendar feed URL
- Armory feed URL
- Canonical realm configuration URL

The generated Schema version is controlled by the module and is not administrator data.

### mod_realm_config_addon

Contains zero or more addon definitions.

Each enabled record becomes:

```ini
[Addon.<addon_key>]
```

There is no fixed limit on the number of addons a realm may publish.

Addon records support:

- Stable addon key
- Display name
- Requirement level
- Source type
- Source URL
- Optional source reference
- WoW addon installation directory
- Administrator-controlled sort order
- Enabled/disabled state

Example conceptually:

```text
addon_key:         AutoTalentsUI
name:              AutoTalentsUI
requirement:       Required
source_type:       GitHub
source_url:        https://github.com/Hisha/AutoTalentsUI
source_ref:        main
install_directory: AutoTalentsUI
sort_order:        10
enabled:           1
```

This generates:

```ini
[Addon.AutoTalentsUI]
Name=AutoTalentsUI
Requirement=Required
SourceType=GitHub
SourceURL=https://github.com/Hisha/AutoTalentsUI
Ref=main
InstallDirectory=AutoTalentsUI
```

### mod_realm_config_patch

Contains zero or more client patch definitions.

Each enabled record becomes:

```ini
[Patch.<patch_key>]
```

Patch records support:

- Stable patch key
- Display name
- Requirement level
- Source type
- Source URL
- Target filename
- Installation directory relative to the WoW client
- Optional SHA-256 validation
- Administrator-controlled sort order
- Enabled/disabled state

Example conceptually:

```text
patch_key:         LivingWorldAssets
name:              Living World Assets
requirement:       Required
source_type:       HTTP
source_url:        https://example.com/patch-L.MPQ
file_name:         patch-L.MPQ
install_directory: Data
sha256:            <SHA-256>
sort_order:        10
enabled:           1
```

This generates:

```ini
[Patch.LivingWorldAssets]
Name=Living World Assets
Requirement=Required
SourceType=HTTP
SourceURL=https://example.com/patch-L.MPQ
FileName=patch-L.MPQ
InstallDirectory=Data
SHA256=<SHA-256>
```

## Requirement Levels

Addon and patch records support three requirement levels:

### Required

The component is required by the realm.

Portalkeeper should ensure that the required component is present and valid before normal realm launch.

### Recommended

The realm recommends the component, but the player may choose not to install or use it.

### Optional

The component is available through the realm configuration but is not required or actively recommended.

The canonical values are:

```text
Required
Recommended
Optional
```

## Source Types

Schema v1 supports launcher resources from supported source types such as:

```text
GitHub
HTTP
```

`SourceURL` identifies the source location.

For sources that support references, such as GitHub, `Ref` may identify a branch, tag, or other source reference:

```ini
Ref=main
```

or:

```ini
Ref=v1.0.7
```

This allows a realm to track a branch or pin a particular release when appropriate.

## realm.conf Schema v1

`mod-realm-config` generates the public configuration in a deterministic INI-style format.

The section order is:

```text
[Config]
[Realm]
[Connection]
[Client]
[Portalkeeper]
[Services]
[Addon.*]
[Patch.*]
```

A representative configuration looks like:

```ini
# Generated by mod-realm-config.
# Public configuration consumed by Portalkeeper.
# Do not edit this file manually.

[Config]
SchemaVersion=1

[Realm]
Name=Example Realm
Description=Private Wrath of the Lich King realm
WebsiteURL=https://example.com/

[Connection]
Address=example.com
AuthPort=3724
WorldPort=8085

[Client]
Version=3.3.5a
Build=12340
Executable=Wow.exe
ExecutableSHA256=

[Portalkeeper]
MinimumVersion=0.1.0

[Services]
ManifestURL=
NewsURL=https://example.com/news.json
StatusURL=
CalendarURL=https://example.com/calendar.json
ArmoryURL=https://example.com/armory/index.json
ConfigURL=https://example.com/realm.conf

[Addon.ExampleAddon]
Name=Example Addon
Requirement=Recommended
SourceType=GitHub
SourceURL=https://github.com/example/ExampleAddon
Ref=main
InstallDirectory=ExampleAddon

[Patch.ExamplePatch]
Name=Example Realm Patch
Requirement=Optional
SourceType=HTTP
SourceURL=https://example.com/patch-X.MPQ
FileName=patch-X.MPQ
InstallDirectory=Data
SHA256=
```

## Schema Versioning

Schema v1 is identified by:

```ini
[Config]
SchemaVersion=1
```

Portalkeeper can use this value to determine whether it understands the realm configuration.

Future additions should remain backward-compatible wherever possible.

A new schema version should only be introduced when a change cannot be represented safely within the existing contract.

## Realm Services

The `[Services]` section acts as Portalkeeper's discovery point for other public realm services.

Schema v1 defines:

```ini
[Services]
ManifestURL=
NewsURL=
StatusURL=
CalendarURL=
ArmoryURL=
ConfigURL=
```

### ManifestURL

Optional URL for a broader realm manifest or service index.

### NewsURL

Optional public news feed, such as one generated by `mod-realm-news`.

### StatusURL

Optional realm status feed.

### CalendarURL

Optional realm calendar feed.

### ArmoryURL

Optional public character roster or armory feed, such as one generated by `mod-realm-armory`.

### ConfigURL

Canonical URL where Portalkeeper can retrieve an updated copy of this realm's `realm.conf`.

Empty optional URLs are valid and may be ignored by Portalkeeper.

## Client Validation

The `[Client]` section describes the WoW client expected by the realm:

```ini
[Client]
Version=3.3.5a
Build=12340
Executable=Wow.exe
ExecutableSHA256=
```

The module describes compatibility requirements only.

It does **not** distribute the World of Warcraft client.

Portalkeeper is responsible for locating and validating the user's existing client installation.

## Addon Installation

`InstallDirectory` identifies the addon directory underneath:

```text
Interface/AddOns/
```

For example:

```ini
InstallDirectory=AutoTalentsUI
```

corresponds to:

```text
Interface/AddOns/AutoTalentsUI
```

Absolute player filesystem paths are never stored in `realm.conf`.

## Patch Installation

Patch `InstallDirectory` values are relative to the WoW client root.

For example:

```ini
InstallDirectory=Data
FileName=patch-L.MPQ
```

describes:

```text
<WoW Client>/Data/patch-L.MPQ
```

Portalkeeper is responsible for resolving the actual local WoW installation directory.

## Deterministic Generation

The generated file uses stable section and field ordering.

Addon and patch records are ordered using their configured sort order with a stable secondary key.

Unchanged database configuration should therefore produce unchanged `realm.conf` content.

This makes the file suitable for caching, comparison, synchronization, and launcher-side update detection.

## Safe Publishing

The module does not directly overwrite the live configuration while constructing it.

The general publishing process is:

1. Load configuration from the world database.
2. Validate the complete configuration.
3. Generate the complete `realm.conf`.
4. Write to a temporary file.
5. Verify the write completed successfully.
6. Replace the published file.

If validation or publishing fails, the existing known-good `realm.conf` is preserved whenever possible.

## Validation

The module validates configuration before publishing.

Validation includes appropriate checks for:

- Required realm information
- Network ports
- Client build information
- Addon and patch keys
- Requirement values
- Source types
- Source URLs
- Installation paths
- SHA-256 values
- Duplicate section keys

Invalid enabled addon or patch records are not silently published as broken launcher configuration.

Validation failures are reported through AzerothCore logging and prevent an invalid configuration from replacing the last known-good file.

## Security

**`realm.conf` must be treated as public data.**

The module intentionally publishes only explicitly supported launcher-facing fields.

It must never be used to expose:

- Database connection strings
- Database usernames or passwords
- SOAP credentials
- API keys or tokens
- Private server filesystem information
- Internal worldserver settings
- Administrator credentials
- Player credentials

Server-side operational settings such as `RealmConfig.OutputDirectory` remain in the module configuration and are not included in the generated public manifest.

## Relationship to Portalkeeper

Portalkeeper is a separate launcher project:

https://github.com/Hisha/Portalkeeper

The intended relationship is:

```text
Realm Administrator
       |
       v
mod-realm-config
       |
       v
   realm.conf
       |
       v
  Portalkeeper
       |
       +--> Validate client
       +--> Configure realmlist
       +--> Manage realm addons
       +--> Manage realm patches
       +--> Discover realm services
       |
       v
      WoW
```

This keeps Portalkeeper independent from the AzerothCore database while allowing each realm to publish its own connection and client requirements.

## License

This module follows the licensing requirements of AzerothCore and any license included with this repository.

## Related Projects

- AzerothCore: https://www.azerothcore.org/
- Portalkeeper: https://github.com/Hisha/Portalkeeper