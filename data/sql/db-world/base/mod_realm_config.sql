-- Import into the AzerothCore WORLD database. This table contains PUBLIC metadata.
-- One publication per world database; only row id=1 is consumed.
-- Re-importing preserves existing administrator values. Never store secrets here.
CREATE TABLE IF NOT EXISTS `mod_realm_config` (
  `id` TINYINT UNSIGNED NOT NULL,
  `realm_key` VARCHAR(64) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL,
  `name` VARCHAR(255) NOT NULL,
  `address` VARCHAR(255) NOT NULL,
  `description` VARCHAR(2048) NOT NULL DEFAULT '',
  `website_url` VARCHAR(1024) NOT NULL DEFAULT '',
  `client_version` VARCHAR(32) NOT NULL,
  `client_build` SMALLINT UNSIGNED NOT NULL,
  `auth_port` SMALLINT UNSIGNED NOT NULL DEFAULT 3724,
  `world_port` SMALLINT UNSIGNED NOT NULL DEFAULT 8085,
  `config_url` VARCHAR(1024) NOT NULL DEFAULT '',
  `client_executable` VARCHAR(255) NOT NULL DEFAULT 'Wow.exe',
  `client_executable_sha256` VARCHAR(64) NOT NULL DEFAULT '',
  `portalkeeper_minimum_version` VARCHAR(32) NOT NULL DEFAULT '0.1.0',
  `manifest_url` VARCHAR(1024) NOT NULL DEFAULT '',
  `news_url` VARCHAR(1024) NOT NULL DEFAULT '',
  `status_url` VARCHAR(1024) NOT NULL DEFAULT '',
  `calendar_url` VARCHAR(1024) NOT NULL DEFAULT '',
  `armory_url` VARCHAR(1024) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB
  DEFAULT CHARSET=utf8mb4
  COLLATE=utf8mb4_unicode_ci;

CREATE TABLE IF NOT EXISTS `mod_realm_config_addon` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `addon_key` VARCHAR(64) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL,
  `name` VARCHAR(255) NOT NULL,
  `requirement` ENUM('Required','Recommended','Optional') CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
  `source_type` ENUM('GitHub','HTTP') CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
  `source_url` TEXT NOT NULL,
  `source_ref` VARCHAR(255) NOT NULL DEFAULT '',
  `install_directory` VARCHAR(255) NOT NULL,
  `sort_order` INT NOT NULL DEFAULT 0,
  `enabled` TINYINT UNSIGNED NOT NULL DEFAULT 1,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uq_mod_realm_config_addon_key` (`addon_key`),
  KEY `idx_mod_realm_config_addon_publish` (`enabled`, `sort_order`)
) ENGINE=InnoDB
  DEFAULT CHARSET=utf8mb4
  COLLATE=utf8mb4_unicode_ci;

CREATE TABLE IF NOT EXISTS `mod_realm_config_patch` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `patch_key` VARCHAR(64) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL,
  `name` VARCHAR(255) NOT NULL,
  `requirement` ENUM('Required','Recommended','Optional') CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
  `source_type` ENUM('HTTP','GitHub') CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
  `source_url` TEXT NOT NULL,
  `file_name` VARCHAR(255) NOT NULL,
  `install_directory` VARCHAR(1024) NOT NULL,
  `sha256` VARCHAR(64) NOT NULL DEFAULT '',
  `sort_order` INT NOT NULL DEFAULT 0,
  `enabled` TINYINT UNSIGNED NOT NULL DEFAULT 1,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uq_mod_realm_config_patch_key` (`patch_key`),
  KEY `idx_mod_realm_config_patch_publish` (`enabled`, `sort_order`)
) ENGINE=InnoDB
  DEFAULT CHARSET=utf8mb4
  COLLATE=utf8mb4_unicode_ci;

-- Safe example metadata lives ONLY in SQL; replace it before serving the file.
INSERT INTO `mod_realm_config`
  (`id`, `realm_key`, `name`, `address`, `description`, `website_url`, `client_version`,
   `client_build`, `auth_port`, `world_port`, `config_url`)
SELECT 1, 'example', 'Example Realm', 'realm.example.com', '', '', '3.3.5a', 12340, 3724, 8085, ''
WHERE NOT EXISTS (SELECT 1 FROM `mod_realm_config` WHERE `id` = 1);
