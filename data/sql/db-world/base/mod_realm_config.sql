-- Import into the AzerothCore WORLD database. This table contains PUBLIC metadata.
-- One publication per world database; only row id=1 is consumed.
-- Re-importing preserves existing administrator values. Never store secrets here.
CREATE TABLE IF NOT EXISTS `mod_realm_config` (
  `id` TINYINT UNSIGNED NOT NULL,
  `name` VARCHAR(255) NOT NULL,
  `address` VARCHAR(255) NOT NULL,
  `description` VARCHAR(2048) NOT NULL DEFAULT '',
  `website_url` VARCHAR(2048) NOT NULL DEFAULT '',
  `client_version` VARCHAR(32) NOT NULL,
  `client_build` SMALLINT UNSIGNED NOT NULL,
  `auth_port` SMALLINT UNSIGNED NOT NULL DEFAULT 3724,
  `world_port` SMALLINT UNSIGNED NOT NULL DEFAULT 8085,
  `update_url` VARCHAR(2048) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- Safe example metadata lives ONLY in SQL; replace it before serving the file.
INSERT INTO `mod_realm_config`
  (`id`, `name`, `address`, `description`, `website_url`, `client_version`,
   `client_build`, `auth_port`, `world_port`, `update_url`)
SELECT 1, 'Example Realm', 'realm.example.com', '', '', '3.3.5a', 12340, 3724, 8085, ''
WHERE NOT EXISTS (SELECT 1 FROM `mod_realm_config` WHERE `id` = 1);
