# Final Schema v1 SQL tables

Reference definitions after applying the incremental migration. Do not run this
snapshot over an existing installation; use the migration instructions in README.
All text columns are NOT NULL; empty strings represent optional unset values.

```sql
CREATE TABLE `mod_realm_config` (
  `id` TINYINT UNSIGNED NOT NULL,
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
