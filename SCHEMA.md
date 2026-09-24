# Final Schema v1 SQL tables

Reference definitions after applying the content_base_url incremental migration. Do not run this
snapshot over an existing installation; use the migration instructions in README.
All text columns are NOT NULL; empty strings represent optional unset values.

```sql
CREATE TABLE `mod_realm_config` (
  `id` TINYINT UNSIGNED NOT NULL,
  `realm_key` VARCHAR(64) NOT NULL,
  `name` VARCHAR(255) NOT NULL,
  `address` VARCHAR(255) NOT NULL,
  `description` VARCHAR(2048) NOT NULL DEFAULT '',
  `website_url` VARCHAR(2048) NOT NULL DEFAULT '',
  `client_version` VARCHAR(32) NOT NULL,
  `client_build` SMALLINT UNSIGNED NOT NULL,
  `auth_port` SMALLINT UNSIGNED NOT NULL DEFAULT 3724,
  `world_port` SMALLINT UNSIGNED NOT NULL DEFAULT 8085,
  `config_url` VARCHAR(2048) NOT NULL DEFAULT '',
  `client_executable` VARCHAR(255) NOT NULL DEFAULT 'Wow.exe',
  `client_executable_sha256` VARCHAR(64) NOT NULL DEFAULT '',
  `portalkeeper_minimum_version` VARCHAR(32) NOT NULL DEFAULT '0.1.0',
  `manifest_url` VARCHAR(2048) NOT NULL DEFAULT '',
  `news_url` VARCHAR(2048) NOT NULL DEFAULT '',
  `status_url` VARCHAR(2048) NOT NULL DEFAULT '',
  `calendar_url` VARCHAR(2048) NOT NULL DEFAULT '',
  `armory_url` VARCHAR(2048) NOT NULL DEFAULT '',
  `content_base_url` VARCHAR(1024) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE `mod_realm_config_addon` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `addon_key` VARCHAR(64) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL,
  `name` VARCHAR(255) NOT NULL,
  `requirement` ENUM('Required','Recommended','Optional') CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
  `source_type` ENUM('GitHub','HTTP') CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
  `source_url` VARCHAR(2048) NOT NULL,
  `source_ref` VARCHAR(255) NOT NULL DEFAULT '',
  `install_directory` VARCHAR(255) NOT NULL,
  `sort_order` INT NOT NULL DEFAULT 0,
  `enabled` TINYINT UNSIGNED NOT NULL DEFAULT 1,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uq_mod_realm_config_addon_key` (`addon_key`),
  KEY `idx_mod_realm_config_addon_publish` (`enabled`, `sort_order`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE `mod_realm_config_patch` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `patch_key` VARCHAR(64) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL,
  `name` VARCHAR(255) NOT NULL,
  `requirement` ENUM('Required','Recommended','Optional') CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
  `source_type` ENUM('HTTP','GitHub') CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
  `source_url` VARCHAR(2048) NOT NULL,
  `file_name` VARCHAR(255) NOT NULL,
  `install_directory` VARCHAR(1024) NOT NULL,
  `sha256` VARCHAR(64) NOT NULL DEFAULT '',
  `sort_order` INT NOT NULL DEFAULT 0,
  `enabled` TINYINT UNSIGNED NOT NULL DEFAULT 1,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uq_mod_realm_config_patch_key` (`patch_key`),
  KEY `idx_mod_realm_config_patch_publish` (`enabled`, `sort_order`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
```

Section keys are case-insensitively unique. Runtime validation additionally enforces
ASCII section identifiers, enums, hashes and safe paths. Only enabled=1 children and
singleton id=1 are published. No child catalog rows are automatically seeded.

## Optional Content Manager advertisement

`mod_realm_config.content_base_url` is the only added column. It is the public
HTTP(S) directory for Content Manager artifacts and defaults to empty (disabled).
It is independent of `config_url`. Apply
`data/sql/db-world/updates/2026_09_15_00_content_base_url.sql` to existing databases.
The migration adds no columns to `mod_realm_config_patch` and changes no Content
Manager tables or administrator rows.

The optional external world tables supply the existing Content Manager
integration. `content_manager_build` provides `build_number` (unsigned integer),
`realm_name`, `filename`, `sha256`, and `state`. Only exact ACTIVE state is
consumed. There must be at most one ACTIVE row globally; it must match canonical
AzerothCore `realm.Name` exactly to be advertised. The public display name in this
module is not the matching identity.

The Schema 3 tables added by Content Manager are:

```sql
CREATE TABLE `content_manager_build` (
  `build_number` INT UNSIGNED NOT NULL,
  -- realm_name, filename, sha256, state added by Content Manager
  ...
);

CREATE TABLE `content_manager_build_client_requirement` (
  `build_number` INT UNSIGNED NOT NULL,
  `requirement` VARCHAR(64) COLLATE utf8mb4_bin NOT NULL,
  PRIMARY KEY (`build_number`, `requirement`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;
```

The client requirement table is immutable build metadata recording the semantic
client capabilities an ACTIVE build requires (currently `protected-framexml`).
It is published only for the ACTIVE build this realm owns and fully advertises,
as `Client.Requirements` (comma-separated, sorted, deduplicated), always emitted
after `Client.RuntimeMode`. Entries must be 1..64 safe ASCII characters with no
commas or whitespace. Installs without this table (pre-Schema 3) publish an empty
`Requirements=`. Divergent requirement columns, or requirement rows referencing no
ACTIVE build in the same snapshot, fail the refresh and preserve the last
known-good file.

The in-memory `realm-content` patch is `Required`, `HTTP`, and `InstallMode=WowPatch`.
It uses the recorded SHA256 (nonempty, 64 hexadecimal characters) and the unchanged
artifact filename joined to the configured base URL. No `FileName` or
`InstallDirectory` is emitted. Portalkeeper owns the persistent client destination;
server build filenames can change without changing that allocation.

`realm-content` is case-insensitively reserved while a matching build and configured
URL enable synthesis. Enabled manual collisions fail publication. Other manual
patches retain their existing File semantics and output without an InstallMode
column or emitted InstallMode field. Synthetic entries are never persisted.
