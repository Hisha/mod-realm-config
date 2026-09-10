# mod-realm-config

Publishes public Portalkeeper launcher metadata from the AzerothCore **world database**
to `realm.conf`. Administrators edit a SQL row; worldserver picks up committed changes
automatically. The module configuration contains operational controls only, never
realm metadata. Portalkeeper needs neither database access nor worldserver configuration.

## Install or upgrade

1. Place this module at `azerothcore-wotlk/modules/mod-realm-config`, then reconfigure,
   rebuild and install AzerothCore using your normal CMake workflow with modules enabled.
2. Import `data/sql/db-world/base/mod_realm_config.sql` into your **world** database
   using your SQL administration tool. It creates the table and a safe example row;
   re-importing preserves an existing row. The conventional SQL directory also permits
   discovery by module-aware AzerothCore database update tooling. Verify the table
   exists before relying on generation; the C++ module does not execute DDL.
3. Set your public metadata in row `id=1`, replacing the example name/address.
4. Install `conf/mod_realm_config.conf.dist` as `mod_realm_config.conf` in the server's
   module configuration directory (normally `etc/modules/`). Retain your desired
   output path; the portable example uses `realm-config`. Your original local path
   `/mnt/ai_data/linkable/` can still be configured here.
5. Start the rebuilt worldserver and separately configure HTTP hosting for the file.

Upgrading from the earlier config-backed version requires deploying/restarting the
rebuilt binary once. Move metadata into SQL and remove obsolete `RealmConfig.Name`,
`Address`, `Description`, `WebsiteURL`, `ClientVersion`, `ClientBuild`, `AuthPort`,
`WorldPort` and `UpdateURL` keys from the installed config. They are no longer read.
Subsequent **metadata edits need no restart or config reload**.

No AzerothCore core changes or external dependencies. CMake registration is unchanged;
`Addmod_realm_configScripts` follows the module directory's loader convention.

## Operational configuration

| Key | Default | Meaning |
| --- | --- | --- |
| RealmConfig.Enable | 1 | Enable SQL refresh and publication; 0 leaves the existing file untouched |
| RealmConfig.OutputDirectory | realm-config | Destination directory, absolute or relative to worldserver's working directory |
| RealmConfig.RefreshIntervalSeconds | 30 | Poll interval in seconds, integer 1–86400 |

These three values are never published. Change operational controls with the normal
worldserver `reload config` command or a restart. A config reload also triggers an
immediate database read when enabled. No metadata fallback exists in `.conf.dist` or
C++; missing/invalid SQL data retains the last valid file.

## Database metadata and live edits

Table `mod_realm_config` belongs to the **world database**, not the auth database.
Only primary-key row `id=1` is consumed: one publication per world database. Other IDs
are ignored. Instances that need different metadata should use separate world
databases and destinations; this version does not select a row by auth realm ID.

| Column | Published key | Requirements / SQL example |
| --- | --- | --- |
| name | Server.Name | Required UTF-8 display name; Example Realm |
| address | Server.Address | Required public hostname/IP without scheme or port; realm.example.com |
| description | Server.Description | Optional single-line text; empty |
| website_url | Server.WebsiteURL | Optional public HTTP(S) URL; empty omits key |
| client_version | Client.Version | Required version label; 3.3.5a |
| client_build | Client.Build | Integer 1–65535; 12340 |
| auth_port | Server.AuthPort | Integer 1–65535; 3724 |
| world_port | Server.WorldPort | Integer 1–65535; 8085 |
| update_url | Updates.UpdateURL | Optional public URL of this file; empty omits section |

The installer includes safe example values **in SQL only**. Edit through your existing
SQL administration tool, for example:

```sql
UPDATE mod_realm_config
SET name = 'Example Realm',
    address = 'realm.example.com',
    description = 'Welcome to our realm',
    website_url = 'https://example.com/',
    client_version = '3.3.5a',
    client_build = 12340,
    auth_port = 3724,
    world_port = 8085,
    update_url = 'https://example.com/realm.conf'
WHERE id = 1;
```

Use one UPDATE (or a committed transaction) for related edits so one poll sees a
consistent row. Within the configured interval plus query completion/world update
time, changes are validated and published. Portalkeeper's own refresh behavior and
HTTP caches can add delay before a launcher displays new data. No `updated_at`
maintenance or revision bump is required: the module compares serialized content.

All columns are NOT NULL; empty strings represent optional unset values. Text must
be valid single-line UTF-8 without controls or surrounding spaces. Validation rejects
empty required fields, invalid builds/ports and basic malformed URLs. It does not
check DNS, availability or guarantee that an administrator-entered URL is public.

## Refresh and publication behavior

Startup reads the row once synchronously after configuration and databases are ready.
Manual config reload also reads once synchronously. Periodic reads use
`WorldDatabase.AsyncQuery`; `WorldScript::OnUpdate` processes completed callbacks on
the world thread. At most one asynchronous query is outstanding. Results issued
before a disable or operational reload are discarded, preventing stale publication.
File operations execute on the world thread, so use a responsive local destination.

Unchanged metadata does not rewrite the file or emit routine success logs. A deleted
publication is recreated on the next successful refresh. Manual edits to an existing
file are unsupported; update SQL instead. SQL/query/validation/write failures preserve
the old publication and retry next interval. The module suppresses repeated identical
errors and logs recovery; AzerothCore's database layer may independently log SQL errors.
A missing table or row is an error, never a reason to publish empty/default metadata.

Disabling performs no new queries or publication. An already queued query may complete,
but its result cannot publish while disabled. The old file remains publicly available
until removed by the administrator. Invalid operational settings suspend polling until
a valid config reload. Database metadata problems recover automatically after correction.

## Published format and Portalkeeper

UTF-8 without BOM, LF newlines, fixed ordering, no timestamps, newline at EOF. Values
are unquoted literals after the first `=`; there are no escapes, interpolation or
inline comments. Both `/some/path` and `/some/path/` produce `/some/path/realm.conf`.
With the SQL example row:

```ini
# Generated by mod-realm-config. Public launcher metadata.
# Edit world database table mod_realm_config (id=1), not this file.

[Server]
Name=Example Realm
Address=realm.example.com
AuthPort=3724
WorldPort=8085
Description=

[Client]
Version=3.3.5a
Build=12340
```

If configured, `WebsiteURL` follows `Description`, and `[Updates]` with `UpdateURL`
follows `[Client]`. The inspected Portalkeeper parser already reads the name, address,
ports and UpdateURL. Its separate update must consume description, website and client
policy fields, which it currently ignores. Current local discovery expects a
non-example `*.realm.conf` bootstrap file (for example `my-realm.realm.conf`); the
server publication remains `realm.conf`.

Addon/patch schemas remain deferred. Add dedicated SQL metadata, validation and section
serializers at `BuildConfiguration` once the launcher schema is finalized. This module
does not download or validate addons/client files.

## Filesystem permissions and security

Worldserver requires write/search permission on the output directory and permission to
create it if missing. The web server needs read access to `realm.conf` and search access
to ancestor directories. Files inherit the process umask; replacing an old file does
not preserve that file's custom permissions/ACLs. Set an appropriate umask/group policy.
Only trusted accounts should be able to write this directory.

The complete document is built/validated before writing. A unique staging directory
inside the destination is restricted to its owner; the file is written, flushed and
closed before rename over the publication. Normal failures clean up staging files.
A process crash can leave `.realm-config-*` directories; clean these manually with the
server stopped. Serve only the final file and disable directory listing.

Same-filesystem rename gives atomic replacement on normal POSIX filesystems. If the
platform refuses replacement, the module logs failure and keeps the old file; it never
deletes it first. No fsync is used, so this is not a power-loss durability guarantee.
Multiple writers should not share an output destination.

Everything in the table's metadata columns is PUBLIC. An explicit fixed SELECT and
serializer whitelist prevent unrelated columns/settings from being exported. The
module uses the existing `WorldDatabase` connection; it never reads or publishes
connection strings, SOAP/console credentials or other server configuration. Runtime
access only needs SELECT on this table; your administrator performs updates. Do not
store secrets or tokens in metadata, including URL query strings. Basic URL validation
rejects embedded user information but cannot detect all sensitive content.

## Compatibility and verification

Requires C++17 and standard AzerothCore configuration, logging, WorldScript, database
and callback APIs. Assumptions checked against upstream headers:
[WorldScript hooks](https://github.com/azerothcore/azerothcore-wotlk/blob/master/src/server/game/Scripting/ScriptDefines/WorldScript.h),
[database queries](https://github.com/azerothcore/azerothcore-wotlk/blob/master/src/server/database/Database/DatabaseWorkerPool.h),
[QueryCallback](https://github.com/azerothcore/azerothcore-wotlk/blob/master/src/server/database/Database/QueryCallback.h).
Raw fixed SQL avoids requiring core prepared-statement registration. Numeric columns
are selected with CAST AS CHAR so Field::Get<std::string> matches their result type.

See `VERIFICATION.md`. A full AzerothCore build and live MySQL integration were not
available; the supplied standalone harness compiles and tests the real module using
API doubles. Keep that separate verification archive outside the module source tree.
