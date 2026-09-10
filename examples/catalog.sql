-- Illustrative public data only. NOT an automatic migration.
-- Apply to a Schema v1 world database only if you want this example catalog.
-- Example download URLs are placeholders, not endorsed or verified resources.
UPDATE `mod_realm_config`
SET `name` = 'Example Realm', `address` = 'realm.example.com',
    `description` = '', `website_url` = '',
    `client_version` = '3.3.5a', `client_build` = 12340,
    `client_executable` = 'Wow.exe', `client_executable_sha256` = '',
    `auth_port` = 3724, `world_port` = 8085,
    `portalkeeper_minimum_version` = '0.1.0',
    `manifest_url` = '', `news_url` = '', `status_url` = '',
    `calendar_url` = '', `armory_url` = '',
    `config_url` = 'https://example.com/realm.conf'
WHERE `id` = 1;

INSERT INTO `mod_realm_config_addon`
  (`addon_key`, `name`, `requirement`, `source_type`, `source_url`,
   `source_ref`, `install_directory`, `sort_order`, `enabled`)
VALUES
  ('ExampleAddon', 'Example Addon', 'Required', 'GitHub',
   'https://github.com/example/ExampleAddon', 'main', 'ExampleAddon', 10, 1);

INSERT INTO `mod_realm_config_patch`
  (`patch_key`, `name`, `requirement`, `source_type`, `source_url`,
   `file_name`, `install_directory`, `sha256`, `sort_order`, `enabled`)
VALUES
  ('ExamplePatch', 'Example Realm Patch', 'Required', 'HTTP',
   'https://example.com/patch-X.MPQ', 'patch-X.MPQ', 'Data', '', 10, 1);
