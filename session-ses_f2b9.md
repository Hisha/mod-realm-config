# Add Requirements field to realm.conf [Client]

**Session ID:** ses_f2b9b39e0fferhtURRpxqJcMJ3
**Created:** 9/24/2026, 1:09:29 PM
**Updated:** 9/24/2026, 1:36:40 PM

---

## Assistant (Build · Big Pickle · 2.6s)

**Tool: bash**

**Input:**
```json
{
  "command": "ls -la /home/smithkt/git/mod-realm-config/src/ /home/smithkt/git/mod-realm-config/conf/ /home/smithkt/git/mod-realm-config/data/ /home/smithkt/git/mod-realm-config/examples/"
}
```

**Output:**
```
/home/smithkt/git/mod-realm-config/conf/:
total 12
drwxrwxr-x 2 smithkt smithkt 4096 Sep  9 21:32 .
drwxrwxr-x 7 smithkt smithkt 4096 Sep 23 20:10 ..
-rw-rw-r-- 1 smithkt smithkt 1494 Sep 23 20:09 mod_realm_config.conf.dist

/home/smithkt/git/mod-realm-config/data/:
total 12
drwxrwxr-x 3 smithkt smithkt 4096 Sep  9 21:05 .
drwxrwxr-x 7 smithkt smithkt 4096 Sep 23 20:10 ..
drwxrwxr-x 3 smithkt smithkt 4096 Sep  9 21:05 sql

/home/smithkt/git/mod-realm-config/examples/:
total 16
drwxrwxr-x 2 smithkt smithkt 4096 Sep 23 20:14 .
drwxrwxr-x 7 smithkt smithkt 4096 Sep 23 20:10 ..
-rw-rw-r-- 1 smithkt smithkt 1400 Sep 23 20:14 catalog.sql
-rw-rw-r-- 1 smithkt smithkt  831 Sep 23 20:10 schema-v1.realm.conf

/home/smithkt/git/mod-realm-config/src/:
total 48
drwxrwxr-x 2 smithkt smithkt  4096 Sep 15 19:36 .
drwxrwxr-x 7 smithkt smithkt  4096 Sep 23 20:10 ..
-rw-rw-r-- 1 smithkt smithkt 37567 Sep 23 20:20 mod_realm_config.cpp

```

---

## Assistant (Build · Big Pickle · 1.2s)

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp"
}
```

**Output:**
```
<path>/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp</path>
<type>file</type>
<content>
1: #include "Config.h"
2: #include "DatabaseEnv.h"
3: #include "Field.h"
4: #include "QueryResult.h"
5: #include "Log.h"
6: #include "ScriptMgr.h"
7: #include "World.h"
8: #include "Realm.h"
9: 
10: #include <atomic>
11: #include <chrono>
12: #include <cstdint>
13: #include <filesystem>
14: #include <fstream>
15: #include <optional>
16: #include <set>
17: #include <algorithm>
18: #include <stdexcept>
19: #include <string>
20: #include <utility>
21: #include <vector>
22: 
23: namespace
24: {
25: namespace fs = std::filesystem;
26: 
27: struct Addon
28: {
29:     std::string Key, Name, Requirement, SourceType, SourceURL, Ref, InstallDirectory;
30: };
31: 
32: enum class PatchInstallMode { File, WowPatch };
33: 
34: struct Patch
35: {
36:     std::string Key, Name, Requirement, SourceType, SourceURL, FileName, InstallDirectory, SHA256;
37:     PatchInstallMode InstallMode = PatchInstallMode::File;
38: };
39: 
40: struct ActiveContentBuild
41: {
42:     std::string Number, RealmName, FileName, SHA256;
43: };
44: 
45: struct Settings
46: {
47:     std::string OutputDirectory;
48:     std::string RealmKey;
49:     std::string Name;
50:     std::optional<std::string> GameRealmName;
51:     std::string Address;
52:     std::string Description;
53:     std::string WebsiteURL;
54:     std::string ClientVersion;
55:     std::string ClientBuild;
56:     std::string ClientRuntimeMode = "Legacy";
57:     std::string AuthPort;
58:     std::string WorldPort;
59:     std::string ConfigURL;
60:     std::string Executable, ExecutableSHA256, MinimumVersion;
61:     std::string ManifestURL, NewsURL, StatusURL, CalendarURL, ArmoryURL;
62:     std::string ContentBaseURL;
63:     std::vector<ActiveContentBuild> ActiveBuilds;
64:     std::vector<Addon> Addons;
65:     std::vector<Patch> Patches;
66: };
67: 
68: // One statement gives a consistent snapshot of our InnoDB tables and optional ACTIVE builds.
69: // Empty catalogs still return the singleton; any missing table/query error fails closed.
70: // Control columns type/order/key precede 19 explicitly whitelisted text columns.
71: constexpr char MetadataQuery[] = R"SQL(
72: SELECT
73:        CONVERT('0' USING utf8mb4) COLLATE utf8mb4_unicode_ci AS record_type,
74:        0 AS sort_order,
75:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci AS record_key,
76:        CONVERT(realm_key USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f0,
77:        CONVERT(name USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f1,
78:        CONVERT(address USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f2,
79:        CONVERT(description USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f3,
80:        CONVERT(website_url USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f4,
81:        CONVERT(client_version USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f5,
82:        CONVERT(CAST(client_build AS CHAR) USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f6,
83:        CONVERT(CAST(auth_port AS CHAR) USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f7,
84:        CONVERT(CAST(world_port AS CHAR) USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f8,
85:        CONVERT(config_url USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f9,
86:        CONVERT(client_executable USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f10,
87:        CONVERT(client_executable_sha256 USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f11,
88:        CONVERT(portalkeeper_minimum_version USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f12,
89:        CONVERT(manifest_url USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f13,
90:        CONVERT(news_url USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f14,
91:        CONVERT(status_url USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f15,
92:        CONVERT(calendar_url USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f16,
93:        CONVERT(armory_url USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f17,
94:        CONVERT(content_base_url USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f18
95: FROM mod_realm_config
96: WHERE id = 1
97: UNION ALL
98: SELECT
99:        CONVERT('1' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
100:        sort_order,
101:        CONVERT(addon_key USING utf8mb4) COLLATE utf8mb4_unicode_ci,
102:        CONVERT(name USING utf8mb4) COLLATE utf8mb4_unicode_ci,
103:        CONVERT(requirement USING utf8mb4) COLLATE utf8mb4_unicode_ci,
104:        CONVERT(source_type USING utf8mb4) COLLATE utf8mb4_unicode_ci,
105:        CONVERT(source_url USING utf8mb4) COLLATE utf8mb4_unicode_ci,
106:        CONVERT(source_ref USING utf8mb4) COLLATE utf8mb4_unicode_ci,
107:        CONVERT(install_directory USING utf8mb4) COLLATE utf8mb4_unicode_ci,
108:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
109:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
110:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
111:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
112:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
113:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
114:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
115:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
116:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
117:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
118:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
119:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
120:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci
121: FROM mod_realm_config_addon
122: WHERE enabled = 1
123: UNION ALL
124: SELECT
125:        CONVERT('2' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
126:        sort_order,
127:        CONVERT(patch_key USING utf8mb4) COLLATE utf8mb4_unicode_ci,
128:        CONVERT(name USING utf8mb4) COLLATE utf8mb4_unicode_ci,
129:        CONVERT(requirement USING utf8mb4) COLLATE utf8mb4_unicode_ci,
130:        CONVERT(source_type USING utf8mb4) COLLATE utf8mb4_unicode_ci,
131:        CONVERT(source_url USING utf8mb4) COLLATE utf8mb4_unicode_ci,
132:        CONVERT(file_name USING utf8mb4) COLLATE utf8mb4_unicode_ci,
133:        CONVERT(install_directory USING utf8mb4) COLLATE utf8mb4_unicode_ci,
134:        CONVERT(sha256 USING utf8mb4) COLLATE utf8mb4_unicode_ci,
135:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
136:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
137:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
138:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
139:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
140:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
141:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
142:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
143:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
144:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
145:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
146:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci
147: FROM mod_realm_config_patch
148: WHERE enabled = 1
149: )SQL";
150: 
151: // COUNT returns a row even when Content Manager is absent. A query failure must
152: // not be mistaken for absence. Checking columns also detects incompatible installs.
153: constexpr char ContentContractQuery[] = R"SQL(
154: SELECT
155:     (SELECT COUNT(*) FROM information_schema.TABLES
156:      WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build'),
157:     (SELECT COUNT(*) FROM information_schema.COLUMNS
158:      WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build'
159:        AND COLUMN_NAME IN ('build_number', 'realm_name', 'filename', 'sha256', 'state'))
160: )SQL";
161: 
162: bool ReadContentContract(QueryResult const& result)
163: {
164:     if (!result || result->GetFieldCount() != 2)
165:         throw std::runtime_error("Content Manager contract detection failed; check database permissions/logs");
166:     Field* fields = result->Fetch();
167:     if (fields[0].IsNull() || fields[1].IsNull())
168:         throw std::runtime_error("unexpected Content Manager contract detection result");
169:     if (fields[0].Get<uint64>() == 0) return false;
170:     if (fields[0].Get<uint64>() != 1 || fields[1].Get<uint64>() != 5)
171:         throw std::runtime_error("incompatible content_manager_build contract; expected build_number, realm_name, filename, sha256 and state");
172:     return true;
173: }
174: 
175: std::string MetadataSQL(bool withContent)
176: {
177:     std::string sql = MetadataQuery;
178:     if (withContent)
179:     {
180:         sql += R"SQL(
181: UNION ALL
182: SELECT CONVERT('3' USING utf8mb4) COLLATE utf8mb4_unicode_ci, 0,
183:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
184:        CONVERT(CAST(build_number AS CHAR) USING utf8mb4) COLLATE utf8mb4_unicode_ci,
185:        CONVERT(realm_name USING utf8mb4) COLLATE utf8mb4_unicode_ci,
186:        CONVERT(filename USING utf8mb4) COLLATE utf8mb4_unicode_ci,
187:        CONVERT(sha256 USING utf8mb4) COLLATE utf8mb4_unicode_ci
188: )SQL";
189:         for (unsigned i = 4; i < 19; ++i)
190:             sql += ", CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci";
191:         // Deliberately do not filter by realm or LIMIT 1: multiple ACTIVE rows
192:         // anywhere in this world database are ambiguous and must be rejected.
193:         sql += " FROM content_manager_build WHERE BINARY state = 'ACTIVE'\n";
194:     }
195:     sql += "ORDER BY record_type, sort_order, BINARY record_key";
196:     return sql;
197: }
198: 
199: Settings LoadMetadata(QueryResult const& result, std::string const& directory)
200: {
201:     if (!result)
202:         throw std::runtime_error("Schema v1 SQL query failed or singleton id=1 is missing; apply module migrations and check database logs");
203:     if (result->GetFieldCount() != 22)
204:         throw std::runtime_error("unexpected Schema v1 SQL result shape");
205:     Settings settings;
206:     settings.OutputDirectory = directory;
207:     bool found = false;
208:     do
209:     {
210:         Field* fields = result->Fetch();
211:         for (unsigned i = 0; i < 22; ++i)
212:             if (fields[i].IsNull())
213:                 throw std::runtime_error("mod_realm_config SQL data contains an unexpected NULL");
214:         auto value = [fields](unsigned i) { return fields[i + 3].Get<std::string>(); };
215:         auto type = fields[0].Get<std::string>();
216:         if (type == "0")
217:         {
218:             if (found) throw std::runtime_error("duplicate singleton configuration row");
219:             found = true;
220:             settings.RealmKey = value(0);
221:             settings.Name = value(1);
222:             settings.Address = value(2);
223:             settings.Description = value(3);
224:             settings.WebsiteURL = value(4);
225:             settings.ClientVersion = value(5);
226:             settings.ClientBuild = value(6);
227:             settings.AuthPort = value(7);
228:             settings.WorldPort = value(8);
229:             settings.ConfigURL = value(9);
230:             settings.Executable = value(10);
231:             settings.ExecutableSHA256 = value(11);
232:             settings.MinimumVersion = value(12);
233:             settings.ManifestURL = value(13);
234:             settings.NewsURL = value(14);
235:             settings.StatusURL = value(15);
236:             settings.CalendarURL = value(16);
237:             settings.ArmoryURL = value(17);
238:             settings.ContentBaseURL = value(18);
239:         }
240:         else if (type == "1")
241:             settings.Addons.push_back({fields[2].Get<std::string>(), value(0), value(1),
242:                 value(2), value(3), value(4), value(5)});
243:         else if (type == "2")
244:             settings.Patches.push_back({fields[2].Get<std::string>(), value(0), value(1),
245:                 value(2), value(3), value(4), value(5), value(6)});
246:         else if (type == "3")
247:             settings.ActiveBuilds.push_back({value(0), value(1), value(2), value(3)});
248:         else throw std::runtime_error("unexpected Schema v1 SQL record type");
249:     } while (result->NextRow());
250:     if (!found) throw std::runtime_error("mod_realm_config singleton id=1 is missing");
251:     return settings;
252: }
253: 
254: void ValidateText(std::string const& value, char const* key, bool required)
255: {
256:     auto fail = [key]() { throw std::runtime_error(std::string("Invalid ") + key +
257:         " must be single-line UTF-8 text without control characters or surrounding spaces"); };
258:     if (required && value.empty())
259:         throw std::runtime_error(std::string("Invalid ") + key + " cannot be empty");
260:     if (!value.empty() && (value.front() == ' ' || value.back() == ' '))
261:         fail();
262:     for (std::size_t i = 0; i < value.size();)
263:     {
264:         unsigned char c = static_cast<unsigned char>(value[i++]);
265:         if (c < 0x20 || c == 0x7f)
266:             fail(); // Includes CR, LF and NUL: values cannot inject INI sections/keys.
267:         if (c < 0x80)
268:             continue;
269:         unsigned count;
270:         std::uint32_t point;
271:         std::uint32_t minimum;
272:         if (c >= 0xc2 && c <= 0xdf) { count = 1; point = c & 0x1f; minimum = 0x80; }
273:         else if (c >= 0xe0 && c <= 0xef) { count = 2; point = c & 0x0f; minimum = 0x800; }
274:         else if (c >= 0xf0 && c <= 0xf4) { count = 3; point = c & 0x07; minimum = 0x10000; }
275:         else { fail(); return; }
276:         while (count--)
277:         {
278:             if (i == value.size()) { fail(); return; }
279:             c = static_cast<unsigned char>(value[i++]);
280:             if ((c & 0xc0) != 0x80) fail();
281:             point = (point << 6) | (c & 0x3f);
282:         }
283:         if (point < minimum || point > 0x10ffff || (point >= 0xd800 && point <= 0xdfff) ||
284:             (point >= 0x80 && point <= 0x9f) || point == 0x2028 || point == 0x2029)
285:             fail();
286:     }
287: }
288: 
289: void ValidateNumber(std::string& value, char const* key, std::uint32_t maximum)
290: {
291:     std::uint32_t number = 0;
292:     bool valid = !value.empty();
293:     for (unsigned char c : value)
294:     {
295:         if (c < '0' || c > '9' || number > maximum / 10 ||
296:             (number == maximum / 10 && static_cast<unsigned>(c - '0') > maximum % 10))
297:         { valid = false; break; }
298:         number = number * 10 + (c - '0');
299:     }
300:     if (!valid || !number)
301:         throw std::runtime_error(std::string("Invalid ") + key +
302:             " must be an integer between 1 and " + std::to_string(maximum));
303:     value = std::to_string(number);
304: }
305: 
306: void ValidateURL(std::string const& value, char const* key)
307: {
308:     ValidateText(value, key, false);
309:     if (value.empty()) return;
310:     auto start = value.rfind("https://", 0) == 0 ? 8u : value.rfind("http://", 0) == 0 ? 7u : 0u;
311:     auto end = value.find_first_of("/?#", start);
312:     auto authority = value.substr(start, end == std::string::npos ? end : end - start);
313:     if (!start || authority.empty() || authority.find('@') != std::string::npos ||
314:         value.find(' ') != std::string::npos || value.find('\\') != std::string::npos)
315:         throw std::runtime_error(std::string("Invalid ") + key +
316:             " must be a public http(s) URL without embedded credentials or spaces");
317:     // Basic authority checks only; no network requests or full URI parser.
318:     std::string host = authority;
319:     std::string port;
320:     if (host.front() == '[')
321:     {
322:         auto close = host.find(']');
323:         if (close == std::string::npos || close <= 1 ||
324:             (close + 1 < host.size() && host[close + 1] != ':'))
325:             throw std::runtime_error(std::string(key) + " has an invalid bracketed URL host");
326:         if (close + 1 < host.size()) port = host.substr(close + 2);
327:         host = host.substr(1, close - 1);
328:         if (host.find(':') == std::string::npos ||
329:             host.find_first_not_of("0123456789abcdefABCDEF:.") != std::string::npos)
330:             throw std::runtime_error(std::string(key) + " has an invalid IPv6 URL host");
331:     }
332:     else
333:     {
334:         auto colon = host.find(':');
335:         if (colon != std::string::npos) { port = host.substr(colon + 1); host.resize(colon); }
336:         if (host.empty() || host.find_first_not_of(
337:             "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._") != std::string::npos)
338:             throw std::runtime_error(std::string(key) + " has an invalid URL hostname (use ASCII/punycode)");
339:     }
340:     if (authority.back() == ':')
341:         throw std::runtime_error(std::string(key) + " has an empty URL port");
342:     if (!port.empty()) ValidateNumber(port, key, 65535);
343: }
344: 
345: std::string FoldASCII(std::string value)
346: {
347:     for (char& c : value) if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
348:     return value;
349: }
350: 
351: void ValidateKey(std::string const& value)
352: {
353:     if (value.empty() || value.size() > 64 ||
354:         !std::all_of(value.begin(), value.end(), [](unsigned char c)
355:         { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
356:             (c >= '0' && c <= '9') || c == '_' || c == '-'; }))
357:         throw std::runtime_error("section key must contain 1..64 ASCII letters, digits, underscores or hyphens");
358: }
359: 
360: void ValidateRealmKey(std::string const& value)
361: {
362:     if (value.empty() || value.size() > 64 ||
363:         !std::all_of(value.begin(), value.end(), [](unsigned char c)
364:         {
365:             return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_' || c == '-';
366:         }))
367:         throw std::runtime_error("mod_realm_config.realm_key must contain 1..64 lowercase ASCII letters, digits, underscores or hyphens");
368: }
369: 
370: void ValidateHash(std::string& value, char const* key)
371: {
372:     if (value.empty()) return;
373:     if (value.size() != 64 || !std::all_of(value.begin(), value.end(), [](unsigned char c)
374:         { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }))
375:         throw std::runtime_error(std::string(key) + " must be empty or exactly 64 hexadecimal characters");
376:     value = FoldASCII(value);
377: }
378: 
379: void ValidateRelativePath(std::string const& value, char const* key, bool singleComponent)
380: {
381:     ValidateText(value, key, true);
382:     if (value.find_first_of("\\:<>\"|?*") != std::string::npos || value.front() == '/')
383:         throw std::runtime_error(std::string(key) + " must be a portable relative path using forward slashes");
384:     std::size_t start = 0;
385:     do
386:     {
387:         auto end = value.find('/', start);
388:         auto part = value.substr(start, end == std::string::npos ? end : end - start);
389:         if (part.empty() || part == "." || part == ".." || part.front() == ' ' ||
390:             part.back() == ' ' || part.back() == '.' || (singleComponent && end != std::string::npos))
391:             throw std::runtime_error(std::string(key) + " contains an unsafe path component");
392:         auto stem = FoldASCII(part.substr(0, part.find('.')));
393:         if (stem == "con" || stem == "prn" || stem == "aux" || stem == "nul" ||
394:             (stem.size() == 4 && (stem.substr(0, 3) == "com" || stem.substr(0, 3) == "lpt") &&
395:              stem[3] >= '1' && stem[3] <= '9'))
396:             throw std::runtime_error(std::string(key) + " contains a reserved device name");
397:         if (end == std::string::npos) break;
398:         start = end + 1;
399:     } while (true);
400: }
401: 
402: void ValidateSource(std::string const& requirement, std::string const& type, std::string const& url)
403: {
404:     if (requirement != "Required" && requirement != "Recommended" && requirement != "Optional")
405:         throw std::runtime_error("Requirement must be Required, Recommended or Optional (case sensitive)");
406:     if (type != "GitHub" && type != "HTTP")
407:         throw std::runtime_error("SourceType must be GitHub or HTTP (case sensitive)");
408:     ValidateText(url, "SourceURL", true);
409:     ValidateURL(url, "SourceURL");
410: }
411: 
412: void ValidateCatalogs(Settings& settings)
413: {
414:     std::set<std::string> addonKeys, patchKeys;
415:     for (auto& addon : settings.Addons)
416:     {
417:         try
418:         {
419:             ValidateKey(addon.Key);
420:             if (!addonKeys.insert(FoldASCII(addon.Key)).second) throw std::runtime_error("duplicate addon key");
421:             ValidateText(addon.Name, "Name", true);
422:             ValidateSource(addon.Requirement, addon.SourceType, addon.SourceURL);
423:             ValidateText(addon.Ref, "Ref", false);
424:             ValidateRelativePath(addon.InstallDirectory, "InstallDirectory", true);
425:         }
426:         catch (std::exception const& error)
427:         {
428:             // Do not echo unvalidated database content into logs.
429:             throw std::runtime_error("addon record #" + std::to_string(&addon - settings.Addons.data() + 1) +
430:                 " in SQL sort order: " + error.what());
431:         }
432:     }
433:     for (auto& patch : settings.Patches)
434:     {
435:         try
436:         {
437:             ValidateKey(patch.Key);
438:             if (!patchKeys.insert(FoldASCII(patch.Key)).second) throw std::runtime_error("duplicate patch key");
439:             ValidateText(patch.Name, "Name", true);
440:             ValidateSource(patch.Requirement, patch.SourceType, patch.SourceURL);
441:             switch (patch.InstallMode)
442:             {
443:                 case PatchInstallMode::File:
444:                     ValidateRelativePath(patch.FileName, "FileName", true);
445:                     ValidateRelativePath(patch.InstallDirectory, "InstallDirectory", false);
446:                     break;
447:                 case PatchInstallMode::WowPatch:
448:                     if (!patch.FileName.empty() || !patch.InstallDirectory.empty())
449:                         throw std::runtime_error("WowPatch must not specify FileName or InstallDirectory");
450:                     ValidateText(patch.SHA256, "WowPatch SHA256", true);
451:                     break;
452:                 default:
453:                     throw std::runtime_error("invalid patch InstallMode");
454:             }
455:             ValidateHash(patch.SHA256, "SHA256");
456:         }
457:         catch (std::exception const& error)
458:         {
459:             throw std::runtime_error("patch record #" + std::to_string(&patch - settings.Patches.data() + 1) +
460:                 " in SQL sort order: " + error.what());
461:         }
462:     }
463: }
464: 
465: // Returns a status only for change-driven logging. No database writes or MPQ I/O.
466: std::string SynthesizeRealmContent(Settings& settings, bool contentAvailable, std::string const& canonicalRealmName)
467: {
468:     if (!contentAvailable) return "unavailable";
469:     if (settings.ActiveBuilds.size() > 1)
470:         throw std::runtime_error("multiple ACTIVE Content Manager builds; refusing ambiguous realm content metadata");
471:     if (settings.ActiveBuilds.empty()) return "available; no ACTIVE realm content build";
472:     if (settings.ContentBaseURL.empty()) return "available; content_base_url is unset";
473: 
474:     auto const& build = settings.ActiveBuilds.front();
475:     ValidateText(canonicalRealmName, "current AzerothCore realm name", true);
476:     ValidateText(build.RealmName, "Content Manager realm_name", true);
477:     // Content Manager records realm.Name verbatim. The public display name in
478:     // mod_realm_config may differ and is not the identity used for this comparison.
479:     if (build.RealmName != canonicalRealmName)
480:         return "available; ACTIVE build belongs to a different realm; not advertised";
481: 
482:     auto number = build.Number;
483:     ValidateNumber(number, "Content Manager build_number", UINT32_MAX);
484:     ValidateRelativePath(build.FileName, "Content Manager filename", true);
485:     // Content Manager generates ASCII artifact names. Reject URI delimiters and
486:     // escapes rather than altering its authoritative filename during URL joining.
487:     if (build.FileName.find_first_not_of(
488:         "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~") != std::string::npos)
489:         throw std::runtime_error("Content Manager filename is not a literal URL path component");
490:     ValidateURL(settings.ContentBaseURL, "content_base_url");
491:     if (settings.ContentBaseURL.find_first_of("?#") != std::string::npos)
492:         throw std::runtime_error("content_base_url must be a directory URL without query or fragment");
493:     for (auto const& patch : settings.Patches)
494:         if (FoldASCII(patch.Key) == "realm-content")
495:             throw std::runtime_error("realm-content is reserved for Content Manager integration; rename or disable the enabled manual patch");
496: 
497:     auto url = settings.ContentBaseURL;
498:     while (url.back() == '/') url.pop_back(); // ValidateURL already required a nonempty authority.
499:     url += "/" + build.FileName;
500:     settings.Patches.push_back({"realm-content", build.RealmName + " Realm Content", "Required",
501:         "HTTP", std::move(url), "", "", build.SHA256, PatchInstallMode::WowPatch});
502:     // ValidateSettings subsequently validates the entire catalog using the same
503:     // key/name/source/hash rules, plus the install-mode-specific destination rules.
504:     return "available; ACTIVE realm content build " + number;
505: }
506: 
507: void ValidateSettings(Settings& settings)
508: {
509:     ValidateText(settings.OutputDirectory, "RealmConfig.OutputDirectory", true);
510:     ValidateRealmKey(settings.RealmKey);
511:     ValidateText(settings.Name, "mod_realm_config.name", true);
512:     ValidateText(settings.Address, "mod_realm_config.address", true);
513:     if (settings.Address.find_first_of(" /\\\"'\t=#;@") != std::string::npos ||
514:         (settings.Address.find(':') != std::string::npos &&
515:          settings.Address.find(':') == settings.Address.rfind(':')))
516:         throw std::runtime_error("mod_realm_config.address must be a hostname or IP address, without a URL scheme or spaces");
517:     ValidateText(settings.Description, "mod_realm_config.description", false);
518:     ValidateText(settings.ClientVersion, "mod_realm_config.client_version", true);
519:     ValidateURL(settings.WebsiteURL, "mod_realm_config.website_url");
520:     ValidateURL(settings.ConfigURL, "mod_realm_config.config_url");
521:     ValidateNumber(settings.ClientBuild, "mod_realm_config.client_build", 65535);
522:     ValidateNumber(settings.AuthPort, "mod_realm_config.auth_port", 65535);
523:     ValidateNumber(settings.WorldPort, "mod_realm_config.world_port", 65535);
524:     ValidateRelativePath(settings.Executable, "Client.Executable", true);
525:     ValidateHash(settings.ExecutableSHA256, "Client.ExecutableSHA256");
526:     ValidateText(settings.MinimumVersion, "Portalkeeper.MinimumVersion", true);
527:     ValidateURL(settings.ManifestURL, "Services.ManifestURL");
528:     ValidateURL(settings.NewsURL, "Services.NewsURL");
529:     ValidateURL(settings.StatusURL, "Services.StatusURL");
530:     ValidateURL(settings.CalendarURL, "Services.CalendarURL");
531:     ValidateURL(settings.ArmoryURL, "Services.ArmoryURL");
532:     ValidateCatalogs(settings);
533: }
534: 
535: // Read the name advertised by authserver for this worldserver's configured realm.
536: // A missing or invalid name leaves the optional launcher hint unpublished.
537: std::optional<std::string> LoadGameRealmName(std::string& warning)
538: {
539:     auto const realmId = realm.Id.Realm;
540:     try
541:     {
542:         auto result = LoginDatabase.Query("SELECT name FROM realmlist WHERE id = {}", realmId);
543:         if (!result || result->GetFieldCount() != 1 || result->Fetch()[0].IsNull())
544:             throw std::runtime_error("auth realmlist row or name is unavailable");
545:         auto name = result->Fetch()[0].Get<std::string>();
546:         ValidateText(name, "auth realmlist name", true);
547:         return name;
548:     }
549:     catch (std::exception const& error)
550:     {
551:         warning = "mod-realm-config: cannot obtain GameRealmName from auth realmlist for RealmID " +
552:             std::to_string(realmId) + ": " + error.what() + "; publishing without GameRealmName";
553:         return std::nullopt;
554:     }
555: }
556: 
557: using Fields = std::vector<std::pair<std::string, std::string>>;
558: 
559: void AppendSection(std::string& output, char const* name, Fields const& fields)
560: {
561:     output += std::string("\n[") + name + "]\n";
562:     for (auto const& field : fields)
563:         output += field.first + "=" + field.second + "\n";
564: }
565: 
566: std::string BuildConfiguration(Settings const& settings)
567: {
568:     std::string output = "# Generated by mod-realm-config.\n"
569:         "# Public configuration consumed by Portalkeeper.\n"
570:         "# Do not edit this file manually.\n";
571:     AppendSection(output, "Config", {{"SchemaVersion", "1"}});
572:     Fields realmFields{{"Name", settings.Name}};
573:     if (settings.GameRealmName)
574:         realmFields.emplace_back("GameRealmName", *settings.GameRealmName);
575:     realmFields.emplace_back("Description", settings.Description);
576:     realmFields.emplace_back("WebsiteURL", settings.WebsiteURL);
577:     AppendSection(output, "Realm", realmFields);
578:     AppendSection(output, "Connection", {{"Address", settings.Address},
579:         {"AuthPort", settings.AuthPort}, {"WorldPort", settings.WorldPort}});
580:     AppendSection(output, "Client", {{"Version", settings.ClientVersion}, {"Build", settings.ClientBuild},
581:         {"Executable", settings.Executable}, {"ExecutableSHA256", settings.ExecutableSHA256},
582:         {"RuntimeMode", settings.ClientRuntimeMode}});
583:     AppendSection(output, "Portalkeeper", {{"MinimumVersion", settings.MinimumVersion}});
584:     AppendSection(output, "Services", {{"ManifestURL", settings.ManifestURL}, {"NewsURL", settings.NewsURL},
585:         {"StatusURL", settings.StatusURL}, {"CalendarURL", settings.CalendarURL},
586:         {"ArmoryURL", settings.ArmoryURL}, {"ConfigURL", settings.ConfigURL}});
587:     for (auto const& addon : settings.Addons)
588:         AppendSection(output, ("Addon." + addon.Key).c_str(), {{"Name", addon.Name},
589:             {"Requirement", addon.Requirement}, {"SourceType", addon.SourceType},
590:             {"SourceURL", addon.SourceURL}, {"Ref", addon.Ref}, {"InstallDirectory", addon.InstallDirectory}});
591:     for (auto const& patch : settings.Patches)
592:     {
593:         Fields fields{{"Name", patch.Name}, {"Requirement", patch.Requirement},
594:             {"SourceType", patch.SourceType}, {"SourceURL", patch.SourceURL}};
595:         if (patch.InstallMode == PatchInstallMode::WowPatch)
596:             fields.emplace_back("InstallMode", "WowPatch");
597:         else
598:         {
599:             fields.emplace_back("FileName", patch.FileName);
600:             fields.emplace_back("InstallDirectory", patch.InstallDirectory);
601:         }
602:         fields.emplace_back("SHA256", patch.SHA256);
603:         AppendSection(output, ("Patch." + patch.Key).c_str(), fields);
604:     }
605:     return output;
606: }
607: 
608: fs::path RealmConfigFileName(Settings const& settings)
609: {
610:     return fs::path(settings.RealmKey + ".realm.conf");
611: }
612: 
613: struct TemporaryDirectory
614: {
615:     fs::path Path;
616:     ~TemporaryDirectory()
617:     {
618:         std::error_code ignored;
619:         fs::remove_all(Path, ignored);
620:     }
621: };
622: 
623: fs::path PublishConfiguration(Settings const& settings, std::string const& output)
624: {
625:     fs::path directory = fs::path(settings.OutputDirectory);
626:     fs::create_directories(directory);
627:     // Atomically reserve a private staging name, avoiding symlinks and concurrent writers.
628:     // The staged file stays on the destination filesystem for the final rename.
629:     static std::atomic<unsigned long long> sequence{0};
630:     fs::path staging;
631:     for (unsigned attempt = 0; attempt < 100; ++attempt)
632:     {
633:         auto candidate = directory / (".realm-config-" +
634:             std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) + "-" +
635:             std::to_string(sequence.fetch_add(1)));
636:         if (fs::create_directory(candidate)) { staging = std::move(candidate); break; }
637:     }
638:     if (staging.empty()) throw std::runtime_error("cannot reserve temporary publication directory");
639:     TemporaryDirectory temporary{staging};
640:     fs::permissions(staging, fs::perms::owner_all, fs::perm_options::replace);
641:     auto stagedFile = staging / RealmConfigFileName(settings);
642:     std::ofstream stream(stagedFile, std::ios::binary | std::ios::trunc);
643:     stream.exceptions(std::ios::badbit | std::ios::failbit);
644:     stream.write(output.data(), static_cast<std::streamsize>(output.size()));
645:     stream.flush();
646:     stream.close(); // A close failure also prevents publication.
647:     auto target = directory / RealmConfigFileName(settings);
648:     fs::rename(stagedFile, target); // Never remove the previous file to work around a rename error.
649:     return target;
650: }
651: 
652: class RealmConfigWorldScript : public WorldScript
653: {
654: public:
655:     RealmConfigWorldScript() : WorldScript("RealmConfigWorldScript") { }
656: 
657:     void OnStartup() override
658:     {
659:         _started = true;
660:         ReloadSettings();
661:     }
662: 
663:     void OnAfterConfigLoad(bool reload) override
664:     {
665:         if (reload && _started) ReloadSettings();
666:     }
667: 
668:     void OnUpdate(uint32 diff) override
669:     {
670:         // Query completion and all filesystem/state operations run on the world thread.
671:         if (_pending && _pending->InvokeIfReady()) _pending.reset();
672:         if (!_enabled) return;
673:         // Start the second query only after the detection callback has been destroyed.
674:         // Never replace _pending from inside its own callback.
675:         if (_snapshotContent.has_value() && !_pending)
676:         {
677:             bool withContent = *_snapshotContent;
678:             _snapshotContent.reset();
679:             auto epoch = _epoch;
680:             try
681:             {
682:                 _pending.emplace(WorldDatabase.AsyncQuery(MetadataSQL(withContent)).WithCallback(
683:                     [this, epoch, withContent](QueryResult result)
684:                     {
685:                         if (_enabled && epoch == _epoch) AcceptResult(result, withContent);
686:                     }));
687:             }
688:             catch (std::exception const& error) { ReportFailure(error.what()); }
689:             return;
690:         }
691:         if (diff < _remaining) { _remaining -= diff; return; }
692:         _remaining = 0;
693:         if (_pending) return; // At most one outstanding query, even across config reloads.
694:         _remaining = _interval;
695:         auto epoch = _epoch;
696:         try
697:         {
698:             _pending.emplace(WorldDatabase.AsyncQuery(ContentContractQuery).WithCallback(
699:                 [this, epoch](QueryResult result)
700:                 {
701:                     if (!_enabled || epoch != _epoch) return;
702:                     try { _snapshotContent = ReadContentContract(result); }
703:                     catch (std::exception const& error) { ReportFailure(error.what()); }
704:                 }));
705:         }
706:         catch (std::exception const& error) { ReportFailure(error.what()); }
707:     }
708: 
709: private:
710:     void ReportFailure(std::string const& message)
711:     {
712:         // Retry every interval, but don't repeat the same module error indefinitely.
713:         if (message != _lastError)
714:             LOG_ERROR("module", "mod-realm-config: publication failed; existing realm.conf preserved: {}", message);
715:         _lastError = message;
716:     }
717: 
718:     void AcceptResult(QueryResult const& result, bool withContent)
719:     {
720:         try
721:         {
722:             auto settings = LoadMetadata(result, _directory);
723:             settings.ClientRuntimeMode = _clientRuntimeMode;
724: 
725:             std::string realmWarning;
726:             settings.GameRealmName = LoadGameRealmName(realmWarning);
727:             if (realmWarning != _lastRealmWarning && !realmWarning.empty())
728:                 LOG_WARN("module", "{}", realmWarning);
729:             _lastRealmWarning = std::move(realmWarning);
730:             auto contentStatus = SynthesizeRealmContent(settings, withContent, realm.Name);
731:             ValidateSettings(settings);
732:             auto output = BuildConfiguration(settings);
733:             auto target = fs::path(_directory) / RealmConfigFileName(settings);
734:             if (output != _lastOutput || target != _lastTarget || !fs::is_regular_file(target))
735:             {
736:                 PublishConfiguration(settings, output);
737:                 _lastOutput = std::move(output);
738:                 _lastTarget = std::move(target);
739:                 LOG_INFO("module", "mod-realm-config: loaded SQL configuration; generated {} with {} addons and {} patches",
740:                     _lastTarget.string(), settings.Addons.size(), settings.Patches.size());
741:             }
742:             else if (!_lastError.empty())
743:                 LOG_INFO("module", "mod-realm-config: database refresh recovered; published metadata is current");
744:             if (contentStatus != _lastContentStatus)
745:             {
746:                 LOG_INFO("module", "mod-realm-config: Content Manager integration {}", contentStatus);
747:                 _lastContentStatus = std::move(contentStatus);
748:             }
749:             _lastError.clear();
750:         }
751:         catch (std::exception const& error) { ReportFailure(error.what()); }
752:     }
753: 
754:     void ReloadSettings()
755:     {
756:         _snapshotContent.reset();
757:         ++_epoch; // Discard any result queued under an older operational configuration.
758:         _enabled = false;
759:         try
760:         {
761:             if (!sConfigMgr->GetOption<bool>("RealmConfig.Enable", true))
762:             {
763:                 LOG_INFO("module", "mod-realm-config: disabled");
764:                 return;
765:             }
766:             _directory = sConfigMgr->GetOption<std::string>("RealmConfig.OutputDirectory", "realm-config");
767:             ValidateText(_directory, "RealmConfig.OutputDirectory", true);
768:             auto seconds = sConfigMgr->GetOption<std::string>("RealmConfig.RefreshIntervalSeconds", "30");
769:             ValidateNumber(seconds, "RealmConfig.RefreshIntervalSeconds", 86400);
770:             auto runtimeMode = FoldASCII(sConfigMgr->GetOption<std::string>("RealmConfig.Client.RuntimeMode", "Legacy"));
771:             _clientRuntimeMode = "Legacy";
772:             if (runtimeMode == "isolated")
773:                 _clientRuntimeMode = "Isolated";
774:             else if (runtimeMode != "legacy")
775:                 LOG_ERROR("module", "mod-realm-config: invalid RealmConfig.Client.RuntimeMode; expected Legacy or Isolated; using Legacy");
776:             _interval = static_cast<uint32>(std::stoul(seconds)) * 1000;
777:             _remaining = _interval;
778:             _enabled = true;
779:             LOG_INFO("module", "mod-realm-config: enabled; checking world database every {} seconds", seconds);
780:             // Synchronous detection and snapshot at startup/reload; asynchronous during refresh.
781:             bool withContent = ReadContentContract(WorldDatabase.Query(ContentContractQuery));
782:             AcceptResult(WorldDatabase.Query(MetadataSQL(withContent)), withContent);
783:         }
784:         catch (std::exception const& error) { ReportFailure(error.what()); }
785:     }
786: 
787:     bool _started = false;
788:     bool _enabled = false;
789:     uint32 _interval = 30000;
790:     uint32 _remaining = 30000;
791:     std::uint64_t _epoch = 0;
792:     std::string _directory;
793:     std::string _clientRuntimeMode = "Legacy";
794:     std::string _lastOutput;
795:     fs::path _lastTarget;
796:     std::string _lastError;
797:     std::optional<QueryCallback> _pending;
798:     std::optional<bool> _snapshotContent;
799:     std::string _lastContentStatus;
800:     std::string _lastRealmWarning;
801: };
802: }
803: 
804: void Addmod_realm_configScripts()
805: {
806:     new RealmConfigWorldScript();
807: }

(End of file - total 807 lines)
</content>
```

---

## Assistant (Build · Big Pickle · 1.9s)

_Thinking:_

Now let me understand the repository better. Let me read the README, SCHEMA.md, conf files, examples, and look at how the tests are structured.

Now let me read the docs, config, and examples.

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/README.md"
}
```

**Output:**
```
<path>/home/smithkt/git/mod-realm-config/README.md</path>
<type>file</type>
<content>
1: # mod-realm-config
2: 
3: `mod-realm-config` is an [AzerothCore](https://www.azerothcore.org/) module that generates and publishes a public `realm.conf` manifest for use by the [Portalkeeper](https://github.com/Hisha/Portalkeeper) launcher.
4: 
5: The module provides a server-side source of truth for the information Portalkeeper needs to connect to and manage a realm, including:
6: 
7: - Realm identity and connection information
8: - Required WoW client version and build
9: - Client executable validation
10: - Portalkeeper compatibility requirements
11: - Realm service and JSON feed locations
12: - Required, recommended, and optional addons
13: - Required, recommended, and optional client patches
14: 
15: The generated `realm.conf` is intended to be hosted somewhere accessible to Portalkeeper, such as a web server or other public download location.
16: 
17: ## Architecture
18: 
19: The module follows a simple separation of responsibilities:
20: 
21: ```text
22: AzerothCore / Realm Administration
23:               |
24:               v
25:       mod-realm-config
26:        Database Tables
27:               |
28:               v
29:       realm.conf Generator
30:               |
31:               v
32:       Published realm.conf
33:               |
34:               v
35:          Portalkeeper
36: ```
37: 
38: AzerothCore controls what the realm requires.
39: 
40: `mod-realm-config` publishes those requirements.
41: 
42: Portalkeeper consumes the resulting configuration.
43: 
44: Portalkeeper does not require access to the AzerothCore database or private worldserver configuration.
45: 
46: ## Requirements
47: 
48: - AzerothCore WotLK
49: - C++17 filesystem support
50: - Access to the AzerothCore world database
51: - Access to the AzerothCore auth database through the core's `LoginDatabase`
52: - Write permission to the configured output directory
53: 
54: ## Installation
55: 
56: Clone the module into your AzerothCore modules directory:
57: 
58: ```bash
59: cd ~/azerothcore-wotlk/modules
60: git clone https://github.com/Hisha/mod-realm-config.git
61: ```
62: 
63: Apply the base world database SQL:
64: 
65: ```text
66: data/sql/db-world/base/mod_realm_config.sql
67: ```
68: 
69: For an existing installation, apply this incremental world SQL update before
70: starting the updated binary. Re-importing the base SQL alone does not add columns:
71: 
72: ```text
73: data/sql/db-world/updates/2026_09_15_00_content_base_url.sql
74: ```
75: 
76: The update is rerunnable and preserves existing data. Fresh installations need
77: only the base SQL. Then rebuild AzerothCore normally.
78: 
79: Copy the module configuration:
80: 
81: ```bash
82: cp modules/mod-realm-config/conf/mod_realm_config.conf.dist \
83:    env/etc/modules/mod_realm_config.conf
84: ```
85: 
86: Adjust the configuration for your environment and restart `worldserver`.
87: 
88: ## Module Configuration
89: 
90: Server-side operational settings and the Portalkeeper client runtime mode are stored in:
91: 
92: ```text
93: conf/mod_realm_config.conf.dist
94: ```
95: 
96: Example:
97: 
98: ```ini
99: RealmConfig.Enable = 1
100: RealmConfig.OutputDirectory = "/mnt/ai_data/linkable/"
101: RealmConfig.RefreshIntervalSeconds = 60
102: RealmConfig.Client.RuntimeMode = "Legacy"
103: ```
104: 
105: ### RealmConfig.Enable
106: 
107: Enables or disables realm configuration generation.
108: 
109: ```ini
110: RealmConfig.Enable = 1
111: ```
112: 
113: When disabled, the module does not generate or modify the published `realm.conf`.
114: 
115: ### RealmConfig.OutputDirectory
116: 
117: Directory where the generated configuration is published.
118: 
119: ```ini
120: RealmConfig.OutputDirectory = "/mnt/ai_data/linkable/"
121: ```
122: 
123: The module generates:
124: 
125: ```text
126: <OutputDirectory>/realm.conf
127: ```
128: 
129: The worldserver process must have permission to create and replace files in this directory.
130: 
131: The configured path is server-side only and is never written into the public `realm.conf`.
132: 
133: ### RealmConfig.RefreshIntervalSeconds
134: 
135: Controls how often the module checks the database for configuration changes.
136: 
137: When changes are detected, the public `realm.conf` is regenerated.
138: 
139: Use the value documented in `mod_realm_config.conf.dist` for the supported range and default behavior.
140: 
141: ### RealmConfig.Client.RuntimeMode
142: 
143: Defaults to `Legacy`, including when the option is omitted. Accepted values are
144: `Legacy` and `Isolated` (case-insensitive); the published value is normalized.
145: 
146: - `Legacy`: Portalkeeper uses the user's configured WoW client directly.
147: - `Isolated`: Portalkeeper prepares and uses a Portalkeeper-managed per-realm
148:   client runtime, retaining the user's configured WoW installation as the source client.
149: 
150: Invalid values log an error and fall back to `Legacy`. Reload config or restart
151: worldserver to apply changes. The module publishes `RuntimeMode=Legacy` or
152: `RuntimeMode=Isolated` in the existing `[Client]` section. This option lives only
153: in the module configuration; no database migration is required.
154: 
155: ## Database
156: 
157: The module uses three world database tables:
158: 
159: ```text
160: mod_realm_config
161: mod_realm_config_addon
162: mod_realm_config_patch
163: ```
164: 
165: ### mod_realm_config
166: 
167: Contains the singleton realm configuration used to generate the primary sections of `realm.conf`.
168: 
169: This includes information for:
170: 
171: ```text
172: [Realm]
173: [Connection]
174: [Client]
175: [Portalkeeper]
176: [Services]
177: ```
178: 
179: Typical values include:
180: 
181: - Realm name and description
182: - Realm website
183: - Server address and ports
184: - Client version and build
185: - Client executable and SHA-256
186: - Minimum Portalkeeper version
187: - Realm manifest URL
188: - News feed URL
189: - Status feed URL
190: - Calendar feed URL
191: - Armory feed URL
192: - Canonical realm configuration URL
193: 
194: The generated Schema version is controlled by the module and is not administrator data.
195: 
196: ### mod_realm_config_addon
197: 
198: Contains zero or more addon definitions.
199: 
200: Each enabled record becomes:
201: 
202: ```ini
203: [Addon.<addon_key>]
204: ```
205: 
206: There is no fixed limit on the number of addons a realm may publish.
207: 
208: Addon records support:
209: 
210: - Stable addon key
211: - Display name
212: - Requirement level
213: - Source type
214: - Source URL
215: - Optional source reference
216: - WoW addon installation directory
217: - Administrator-controlled sort order
218: - Enabled/disabled state
219: 
220: Example conceptually:
221: 
222: ```text
223: addon_key:         AutoTalentsUI
224: name:              AutoTalentsUI
225: requirement:       Required
226: source_type:       GitHub
227: source_url:        https://github.com/Hisha/AutoTalentsUI
228: source_ref:        main
229: install_directory: AutoTalentsUI
230: sort_order:        10
231: enabled:           1
232: ```
233: 
234: This generates:
235: 
236: ```ini
237: [Addon.AutoTalentsUI]
238: Name=AutoTalentsUI
239: Requirement=Required
240: SourceType=GitHub
241: SourceURL=https://github.com/Hisha/AutoTalentsUI
242: Ref=main
243: InstallDirectory=AutoTalentsUI
244: ```
245: 
246: ### mod_realm_config_patch
247: 
248: Contains zero or more client patch definitions.
249: 
250: Each enabled record becomes:
251: 
252: ```ini
253: [Patch.<patch_key>]
254: ```
255: 
256: Patch records support:
257: 
258: - Stable patch key
259: - Display name
260: - Requirement level
261: - Source type
262: - Source URL
263: - Target filename
264: - Installation directory relative to the WoW client
265: - Optional SHA-256 validation
266: - Administrator-controlled sort order
267: - Enabled/disabled state
268: 
269: Example conceptually:
270: 
271: ```text
272: patch_key:         LivingWorldAssets
273: name:              Living World Assets
274: requirement:       Required
275: source_type:       HTTP
276: source_url:        https://example.com/patch-L.MPQ
277: file_name:         patch-L.MPQ
278: install_directory: Data
279: sha256:            <SHA-256>
280: sort_order:        10
281: enabled:           1
282: ```
283: 
284: This generates:
285: 
286: ```ini
287: [Patch.LivingWorldAssets]
288: Name=Living World Assets
289: Requirement=Required
290: SourceType=HTTP
291: SourceURL=https://example.com/patch-L.MPQ
292: FileName=patch-L.MPQ
293: InstallDirectory=Data
294: SHA256=<SHA-256>
295: ```
296: 
297: ## Requirement Levels
298: 
299: Addon and patch records support three requirement levels:
300: 
301: ### Required
302: 
303: The component is required by the realm.
304: 
305: Portalkeeper should ensure that the required component is present and valid before normal realm launch.
306: 
307: ### Recommended
308: 
309: The realm recommends the component, but the player may choose not to install or use it.
310: 
311: ### Optional
312: 
313: The component is available through the realm configuration but is not required or actively recommended.
314: 
315: The canonical values are:
316: 
317: ```text
318: Required
319: Recommended
320: Optional
321: ```
322: 
323: ## Source Types
324: 
325: Schema v1 supports launcher resources from supported source types such as:
326: 
327: ```text
328: GitHub
329: HTTP
330: ```
331: 
332: `SourceURL` identifies the source location.
333: 
334: For sources that support references, such as GitHub, `Ref` may identify a branch, tag, or other source reference:
335: 
336: ```ini
337: Ref=main
338: ```
339: 
340: or:
341: 
342: ```ini
343: Ref=v1.0.7
344: ```
345: 
346: This allows a realm to track a branch or pin a particular release when appropriate.
347: 
348: ## realm.conf Schema v1
349: 
350: `mod-realm-config` generates the public configuration in a deterministic INI-style format.
351: 
352: The section order is:
353: 
354: ```text
355: [Config]
356: [Realm]
357: [Connection]
358: [Client]
359: [Portalkeeper]
360: [Services]
361: [Addon.*]
362: [Patch.*]
363: ```
364: 
365: A representative configuration looks like:
366: 
367: ```ini
368: # Generated by mod-realm-config.
369: # Public configuration consumed by Portalkeeper.
370: # Do not edit this file manually.
371: 
372: [Config]
373: SchemaVersion=1
374: 
375: [Realm]
376: Name=Example Realm
377: GameRealmName=Example
378: Description=Private Wrath of the Lich King realm
379: WebsiteURL=https://example.com/
380: 
381: [Connection]
382: Address=example.com
383: AuthPort=3724
384: WorldPort=8085
385: 
386: [Client]
387: Version=3.3.5a
388: Build=12340
389: Executable=Wow.exe
390: ExecutableSHA256=
391: RuntimeMode=Legacy
392: 
393: [Portalkeeper]
394: MinimumVersion=0.1.0
395: 
396: [Services]
397: ManifestURL=
398: NewsURL=https://example.com/news.json
399: StatusURL=
400: CalendarURL=https://example.com/calendar.json
401: ArmoryURL=https://example.com/armory/index.json
402: ConfigURL=https://example.com/realm.conf
403: 
404: [Addon.ExampleAddon]
405: Name=Example Addon
406: Requirement=Recommended
407: SourceType=GitHub
408: SourceURL=https://github.com/example/ExampleAddon
409: Ref=main
410: InstallDirectory=ExampleAddon
411: 
412: [Patch.ExamplePatch]
413: Name=Example Realm Patch
414: Requirement=Optional
415: SourceType=HTTP
416: SourceURL=https://example.com/patch-X.MPQ
417: FileName=patch-X.MPQ
418: InstallDirectory=Data
419: SHA256=
420: ```
421: 
422: In `[Realm]`, `Name` is the administrator-controlled Portalkeeper display name
423: from `mod_realm_config.name`. `GameRealmName` is the actual WoW realm name
424: advertised by authserver. The module reads it from the auth database's
425: `realmlist.name` row whose `id` matches the current worldserver `RealmID`.
426: These names can differ; for example, `Name=Eitrigg Realm` and
427: `GameRealmName=Eitrigg`. Portalkeeper can use `GameRealmName` for WoW 3.3.5a's
428: `SET realmName` while continuing to use `Connection.Address` for `SET realmList`.
429: No additional administrator-maintained column is needed.
430: 
431: If the matching auth realmlist name is unavailable or invalid, the module logs
432: a warning and omits `GameRealmName` from the generated file. Other fields keep
433: their previous values and order; the display `Name` is never used as a guess.
434: 
435: ## Schema Versioning
436: 
437: Schema v1 is identified by:
438: 
439: ```ini
440: [Config]
441: SchemaVersion=1
442: ```
443: 
444: Portalkeeper can use this value to determine whether it understands the realm configuration.
445: 
446: Future additions should remain backward-compatible wherever possible.
447: 
448: A new schema version should only be introduced when a change cannot be represented safely within the existing contract.
449: 
450: ## Realm Services
451: 
452: The `[Services]` section acts as Portalkeeper's discovery point for other public realm services.
453: 
454: Schema v1 defines:
455: 
456: ```ini
457: [Services]
458: ManifestURL=
459: NewsURL=
460: StatusURL=
461: CalendarURL=
462: ArmoryURL=
463: ConfigURL=
464: ```
465: 
466: ### ManifestURL
467: 
468: Optional URL for a broader realm manifest or service index.
469: 
470: ### NewsURL
471: 
472: Optional public news feed, such as one generated by `mod-realm-news`.
473: 
474: ### StatusURL
475: 
476: Optional realm status feed.
477: 
478: ### CalendarURL
479: 
480: Optional realm calendar feed.
481: 
482: ### ArmoryURL
483: 
484: Optional public character roster or armory feed, such as one generated by `mod-realm-armory`.
485: 
486: ### ConfigURL
487: 
488: Canonical URL where Portalkeeper can retrieve an updated copy of this realm's `realm.conf`.
489: 
490: Empty optional URLs are valid and may be ignored by Portalkeeper.
491: 
492: ## Client Validation
493: 
494: The `[Client]` section describes the WoW client expected by the realm:
495: 
496: ```ini
497: [Client]
498: Version=3.3.5a
499: Build=12340
500: Executable=Wow.exe
501: ExecutableSHA256=
502: RuntimeMode=Legacy
503: ```
504: 
505: The module describes compatibility requirements only.
506: 
507: It does **not** distribute the World of Warcraft client.
508: 
509: Portalkeeper is responsible for locating and validating the user's existing client installation.
510: 
511: ## Addon Installation
512: 
513: `InstallDirectory` identifies the addon directory underneath:
514: 
515: ```text
516: Interface/AddOns/
517: ```
518: 
519: For example:
520: 
521: ```ini
522: InstallDirectory=AutoTalentsUI
523: ```
524: 
525: corresponds to:
526: 
527: ```text
528: Interface/AddOns/AutoTalentsUI
529: ```
530: 
531: Absolute player filesystem paths are never stored in `realm.conf`.
532: 
533: ## Patch Installation
534: 
535: Patch `InstallDirectory` values are relative to the WoW client root.
536: 
537: For example:
538: 
539: ```ini
540: InstallDirectory=Data
541: FileName=patch-L.MPQ
542: ```
543: 
544: describes:
545: 
546: ```text
547: <WoW Client>/Data/patch-L.MPQ
548: ```
549: 
550: Portalkeeper is responsible for resolving the actual local WoW installation directory.
551: 
552: ## Deterministic Generation
553: 
554: The generated file uses stable section and field ordering.
555: 
556: Addon and patch records are ordered using their configured sort order with a stable secondary key.
557: 
558: Unchanged database configuration should therefore produce unchanged `realm.conf` content.
559: 
560: This makes the file suitable for caching, comparison, synchronization, and launcher-side update detection.
561: 
562: ## Safe Publishing
563: 
564: The module does not directly overwrite the live configuration while constructing it.
565: 
566: The general publishing process is:
567: 
568: 1. Load configuration from the world database.
569: 2. Validate the complete configuration.
570: 3. Generate the complete `realm.conf`.
571: 4. Write to a temporary file.
572: 5. Verify the write completed successfully.
573: 6. Replace the published file.
574: 
575: If validation or publishing fails, the existing known-good `realm.conf` is preserved whenever possible.
576: 
577: ## Validation
578: 
579: The module validates configuration before publishing.
580: 
581: Validation includes appropriate checks for:
582: 
583: - Required realm information
584: - Network ports
585: - Client build information
586: - Addon and patch keys
587: - Requirement values
588: - Source types
589: - Source URLs
590: - Installation paths
591: - SHA-256 values
592: - Duplicate section keys
593: 
594: Invalid enabled addon or patch records are not silently published as broken launcher configuration.
595: 
596: Validation failures are reported through AzerothCore logging and prevent an invalid configuration from replacing the last known-good file.
597: 
598: ## Security
599: 
600: **`realm.conf` must be treated as public data.**
601: 
602: The module intentionally publishes only explicitly supported launcher-facing fields.
603: 
604: It must never be used to expose:
605: 
606: - Database connection strings
607: - Database usernames or passwords
608: - SOAP credentials
609: - API keys or tokens
610: - Private server filesystem information
611: - Internal worldserver settings
612: - Administrator credentials
613: - Player credentials
614: 
615: Server-side operational settings such as `RealmConfig.OutputDirectory` remain in the module configuration and are not included in the generated public manifest.
616: 
617: ## Relationship to Portalkeeper
618: 
619: Portalkeeper is a separate launcher project:
620: 
621: https://github.com/Hisha/Portalkeeper
622: 
623: The intended relationship is:
624: 
625: ```text
626: Realm Administrator
627:        |
628:        v
629: mod-realm-config
630:        |
631:        v
632:    realm.conf
633:        |
634:        v
635:   Portalkeeper
636:        |
637:        +--> Validate client
638:        +--> Configure realmlist
639:        +--> Manage realm addons
640:        +--> Manage realm patches
641:        +--> Discover realm services
642:        |
643:        v
644:       WoW
645: ```
646: 
647: This keeps Portalkeeper independent from the AzerothCore database while allowing each realm to publish its own connection and client requirements.
648: 
649: ## License
650: 
651: This module follows the licensing requirements of AzerothCore and any license included with this repository.
652: 
653: ## Related Projects
654: 
655: - AzerothCore: https://www.azerothcore.org/
656: - Portalkeeper: https://github.com/Hisha/Portalkeeper
657: ## Optional ACTIVE realm content (WowPatch)
658: 
659: The integration uses the world database and has no compile-time dependency on
660: `mod-content-manager`. Realm Config discovers the optional `content_manager_build`
661: table; absence is normal and does not produce recurring warnings.
662: 
663: Responsibilities are deliberately separate:
664: 
665: - **Content Manager** builds, hashes, activates, and publishes a versioned cumulative MPQ.
666: - **Realm Config** advertises the matching ACTIVE artifact's public URL and recorded
667:   hash as a required `InstallMode=WowPatch` entry.
668: - **Portalkeeper** allocates and persists a safe `Data/patch-N.MPQ` destination, then
669:   downloads, verifies, and updates that managed patch.
670: 
671: `realm-content` is the stable logical patch identity. The server-side versioned
672: filename changes with builds; Portalkeeper's allocated client filename remains
673: stable. Realm Config does not allocate client filenames or inspect/copy/hash MPQs.
674: 
675: ### Configure the public content URL
676: 
677: Only one new database column is needed: `mod_realm_config.content_base_url`.
678: Its default is empty (no synthetic advertisement). Set the administrator-owned
679: public directory URL on the existing Realm Config singleton:
680: 
681: ```sql
682: UPDATE mod_realm_config
683: SET content_base_url = 'https://example.com/download/'
684: WHERE id = 1;
685: ```
686: 
687: Here `id=1` identifies the module's existing singleton configuration row; it is
688: not an AzerothCore realm ID. The URL is independent of `config_url` and is not
689: an operational `.conf` setting. A trailing slash is optional. Use HTTP or HTTPS,
690: with no credentials, query, or fragment. Artifact names are used unchanged and
691: must be literal ASCII URL path components (letters, digits, `-._~`).
692: 
693: Requirement is currently fixed to `Required`. No client destination policy is
694: stored in the database for the generated patch. Manual patch tables are unchanged.
695: 
696: ### Matching and output
697: 
698: Content Manager records AzerothCore's canonical `realm.Name`. Realm Config compares
699: that stored `realm_name` exactly, including case, against the same runtime value.
700: It does not compare against the independently editable public `mod_realm_config.name`
701: or infer identity from the sanitized artifact filename. No realm name or realm ID
702: is hard-coded. An ACTIVE build for another realm is not advertised.
703: 
704: With the canonical name `Example Realm`, the generated entry looks like:
705: 
706: ```ini
707: [Patch.realm-content]
708: Name=Example Realm Realm Content
709: Requirement=Required
710: SourceType=HTTP
711: SourceURL=https://example.com/download/Example-Realm-Content-000002.mpq
712: InstallMode=WowPatch
713: SHA256=<64 hexadecimal characters from the ACTIVE build>
714: ```
715: 
716: The WowPatch section omits `FileName` and `InstallDirectory` entirely. Existing
717: enabled manual patches keep their existing field order, `FileName`, and
718: `InstallDirectory`; no `InstallMode` field is added to those entries.
719: 
720: When a matching ACTIVE build and a configured base URL enable synthesis, an
721: enabled manual `realm-content` key (case-insensitively) is a configuration conflict.
722: Generation fails with a reserved-key diagnostic; no administrator row is changed.
723: The generated patch exists only in memory, never in `mod_realm_config_patch`.
724: 
725: ### Refresh, validation, and failure behavior
726: 
727: Each refresh first detects the optional table and its required columns using
728: `information_schema`. The subsequent single UNION query reads Realm Config metadata
729: and, only when the table exists, all exact `state='ACTIVE'` build rows together.
730: Filename and hash always come from the same row in the same InnoDB snapshot.
731: STAGED and SUPERSEDED builds are ignored. No ACTIVE build produces no synthetic
732: patch. Multiple ACTIVE rows fail publication even if they belong to different
733: realms; Realm Config never chooses one arbitrarily.
734: 
735: A failed query, incompatible contract, invalid generated metadata, or reserved-key
736: conflict preserves the last known-good file. A table removed between detection and
737: the snapshot fails that refresh and is detected as absent on a later refresh.
738: Keep schema changes separate from normal server operation.
739: 
740: WowPatch uses the existing key/name/requirement/source/URL/hash validation, requires
741: a nonempty SHA256, and has no client destination fields to validate. File-mode
742: patch validation remains unchanged. A blank base URL disables synthesis; no ACTIVE
743: build or a foreign-realm ACTIVE build omits the generated section on the next
744: successful publication.
745: 
746: The existing refresh interval (normally 30 seconds) detects new ACTIVE filenames
747: and hashes, regenerating the file only when its contents change. Activating an
748: older build automatically restores its URL/hash; no restart, reload, or special
749: rollback handling is needed. Identical integration status messages and repeated
750: identical validation errors are suppressed.
751: 
752: ### Focused tests
753: 
754: Run `bash tests/run.sh` with C++17 `g++` (or `$CXX`) and Python 3. The standalone
755: suite uses small AzerothCore API doubles to test the production metadata parser,
756: validator, serializer, refresh callbacks, and publisher. SQL snapshot tests adapt
757: MySQL text casts to SQLite; deployment still requires an AzerothCore build and
758: validation of the migration on MySQL/MariaDB.

(End of file - total 758 lines)
</content>
```

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/SCHEMA.md"
}
```

**Output:**
```
<path>/home/smithkt/git/mod-realm-config/SCHEMA.md</path>
<type>file</type>
<content>
1: # Final Schema v1 SQL tables
2: 
3: Reference definitions after applying the content_base_url incremental migration. Do not run this
4: snapshot over an existing installation; use the migration instructions in README.
5: All text columns are NOT NULL; empty strings represent optional unset values.
6: 
7: ```sql
8: CREATE TABLE `mod_realm_config` (
9:   `id` TINYINT UNSIGNED NOT NULL,
10:   `realm_key` VARCHAR(64) NOT NULL,
11:   `name` VARCHAR(255) NOT NULL,
12:   `address` VARCHAR(255) NOT NULL,
13:   `description` VARCHAR(2048) NOT NULL DEFAULT '',
14:   `website_url` VARCHAR(2048) NOT NULL DEFAULT '',
15:   `client_version` VARCHAR(32) NOT NULL,
16:   `client_build` SMALLINT UNSIGNED NOT NULL,
17:   `auth_port` SMALLINT UNSIGNED NOT NULL DEFAULT 3724,
18:   `world_port` SMALLINT UNSIGNED NOT NULL DEFAULT 8085,
19:   `config_url` VARCHAR(2048) NOT NULL DEFAULT '',
20:   `client_executable` VARCHAR(255) NOT NULL DEFAULT 'Wow.exe',
21:   `client_executable_sha256` VARCHAR(64) NOT NULL DEFAULT '',
22:   `portalkeeper_minimum_version` VARCHAR(32) NOT NULL DEFAULT '0.1.0',
23:   `manifest_url` VARCHAR(2048) NOT NULL DEFAULT '',
24:   `news_url` VARCHAR(2048) NOT NULL DEFAULT '',
25:   `status_url` VARCHAR(2048) NOT NULL DEFAULT '',
26:   `calendar_url` VARCHAR(2048) NOT NULL DEFAULT '',
27:   `armory_url` VARCHAR(2048) NOT NULL DEFAULT '',
28:   `content_base_url` VARCHAR(1024) NOT NULL DEFAULT '',
29:   PRIMARY KEY (`id`)
30: ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
31: 
32: CREATE TABLE `mod_realm_config_addon` (
33:   `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
34:   `addon_key` VARCHAR(64) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL,
35:   `name` VARCHAR(255) NOT NULL,
36:   `requirement` ENUM('Required','Recommended','Optional') CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
37:   `source_type` ENUM('GitHub','HTTP') CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
38:   `source_url` VARCHAR(2048) NOT NULL,
39:   `source_ref` VARCHAR(255) NOT NULL DEFAULT '',
40:   `install_directory` VARCHAR(255) NOT NULL,
41:   `sort_order` INT NOT NULL DEFAULT 0,
42:   `enabled` TINYINT UNSIGNED NOT NULL DEFAULT 1,
43:   PRIMARY KEY (`id`),
44:   UNIQUE KEY `uq_mod_realm_config_addon_key` (`addon_key`),
45:   KEY `idx_mod_realm_config_addon_publish` (`enabled`, `sort_order`)
46: ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
47: 
48: CREATE TABLE `mod_realm_config_patch` (
49:   `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
50:   `patch_key` VARCHAR(64) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL,
51:   `name` VARCHAR(255) NOT NULL,
52:   `requirement` ENUM('Required','Recommended','Optional') CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
53:   `source_type` ENUM('HTTP','GitHub') CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
54:   `source_url` VARCHAR(2048) NOT NULL,
55:   `file_name` VARCHAR(255) NOT NULL,
56:   `install_directory` VARCHAR(1024) NOT NULL,
57:   `sha256` VARCHAR(64) NOT NULL DEFAULT '',
58:   `sort_order` INT NOT NULL DEFAULT 0,
59:   `enabled` TINYINT UNSIGNED NOT NULL DEFAULT 1,
60:   PRIMARY KEY (`id`),
61:   UNIQUE KEY `uq_mod_realm_config_patch_key` (`patch_key`),
62:   KEY `idx_mod_realm_config_patch_publish` (`enabled`, `sort_order`)
63: ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
64: ```
65: 
66: Section keys are case-insensitively unique. Runtime validation additionally enforces
67: ASCII section identifiers, enums, hashes and safe paths. Only enabled=1 children and
68: singleton id=1 are published. No child catalog rows are automatically seeded.
69: 
70: ## Optional Content Manager advertisement
71: 
72: `mod_realm_config.content_base_url` is the only added column. It is the public
73: HTTP(S) directory for Content Manager artifacts and defaults to empty (disabled).
74: It is independent of `config_url`. Apply
75: `data/sql/db-world/updates/2026_09_15_00_content_base_url.sql` to existing databases.
76: The migration adds no columns to `mod_realm_config_patch` and changes no Content
77: Manager tables or administrator rows.
78: 
79: The optional external world table `content_manager_build` supplies the existing
80: `build_number` (unsigned integer), `realm_name`, `filename`, `sha256`, and `state`
81: columns. Only exact ACTIVE state is consumed. There must be at most one ACTIVE
82: row globally; it must match canonical AzerothCore `realm.Name` exactly to be
83: advertised. The public display name in this module is not the matching identity.
84: 
85: The in-memory `realm-content` patch is `Required`, `HTTP`, and `InstallMode=WowPatch`.
86: It uses the recorded SHA256 (nonempty, 64 hexadecimal characters) and the unchanged
87: artifact filename joined to the configured base URL. No `FileName` or
88: `InstallDirectory` is emitted. Portalkeeper owns the persistent client destination;
89: server build filenames can change without changing that allocation.
90: 
91: `realm-content` is case-insensitively reserved while a matching build and configured
92: URL enable synthesis. Enabled manual collisions fail publication. Other manual
93: patches retain their existing File semantics and output without an InstallMode
94: column or emitted InstallMode field. Synthetic entries are never persisted.

(End of file - total 94 lines)
</content>
```

---

## Assistant (Build · Big Pickle · 3.1s)

_Thinking:_

Now let me look at the tests directory and the SQL/config conventions.

Now let me look at the tests directory and SQL/config files.

**Tool: bash**

**Input:**
```json
{
  "command": "find /home/smithkt/git/mod-realm-config/data /home/smithkt/git/mod-realm-config/tests -type f 2>/dev/null; ls -la /home/smithkt/git/mod-realm-config/.gitignore"
}
```

**Output:**
```
/home/smithkt/git/mod-realm-config/data/sql/db-world/base/mod_realm_config.sql
-rw-rw-r-- 1 smithkt smithkt 84 Sep  9 20:06 /home/smithkt/git/mod-realm-config/.gitignore

```

**Tool: bash**

**Input:**
```json
{
  "command": "cat /home/smithkt/git/mod-realm-config/conf/mod_realm_config.conf.dist"
}
```

**Output:**
```
# Operational controls and Portalkeeper client runtime mode. Other published
# realm.conf values are read from
# world database tables mod_realm_config (id=1), mod_realm_config_addon and
# mod_realm_config_patch. Apply the Schema v1 migration; see README.
# Changes to database metadata are picked up automatically; no restart/reload needed.

# 0 disables database refresh and file publication, leaving an existing file intact.
RealmConfig.Enable = 1

# Writable destination directory; created when possible. Relative paths resolve
# against worldserver's working directory. A trailing slash is optional.
# This path is never included in realm.conf. Configure HTTP hosting separately.
RealmConfig.OutputDirectory = "/mnt/ai_data/linkable/"

# Check for committed database changes every 1..86400 seconds (default: 30).
# Unchanged data does not rewrite the file or produce routine success logs.
# Operational config changes require reload config (or restart).
RealmConfig.RefreshIntervalSeconds = 30

# Portalkeeper client runtime mode (case-insensitive: Legacy or Isolated).
# Legacy uses the user's configured WoW client directly (default).
# Isolated prepares and uses a Portalkeeper-managed per-realm client runtime,
# retaining the user's configured WoW installation as the source client.
# Invalid values log an error and fall back to Legacy. Reload config or restart
# to apply changes. Published as RuntimeMode in the existing [Client] section.
RealmConfig.Client.RuntimeMode = "Legacy"

```

---

## Assistant (Build · Big Pickle · 5.7s)

_Thinking:_

Interesting - the README mentions `bash tests/run.sh` but I don't see a tests directory. Let me check the git history to see whether there were tests, and check the .gitignore.

Interesting - README references `tests/run.sh` but there's no tests directory. Let me check git history and .gitignore.

**Tool: bash**

**Input:**
```json
{
  "command": "cat /home/smithkt/git/mod-realm-config/.gitignore; echo \"---\"; git -C /home/smithkt/git/mod-realm-config log --all --oneline | head -30; echo \"---\"; git -C /home/smithkt/git/mod-realm-config ls-tree -r HEAD --name-only"
}
```

**Output:**
```
*.conf
*.log
*.swp
*~
.DS_Store
.vscode/
.idea/
build/
.settings/
.project
.cproject---
05376a8 Git conflict fix
bdf118e Merge branch 'main' of https://github.com/Hisha/mod-realm-config.git into main
ff6f3ce Added realm isolation
0f5b4ef Grabbing realm name from database and giving it it's own place in conf file
4716109 Wow patch update
4e9bfdf Forgot to update SCHEMA.md
ed0b6ba Output of file now names it with realmkey.realm.conf like it should
97401c5 Select column mismatch
7aff1f0 Fixed to remove warnings.
e0fa777 SQL changes dictate code changes
87142e8 TEXT and Defaults are a problem
8f97d4d Defaults missing
537f9ca Column mismatches
d2308a8 added sql to store data and updated documentation
e99de46 Added icon,started sql support
8962b49 Initial commit
a5f67de Initial commit
---
.clang-format
.gitignore
CMakeLists.txt
LICENSE
README.md
SCHEMA.md
conf/mod_realm_config.conf.dist
data/sql/db-world/base/mod_realm_config.sql
examples/catalog.sql
icon.png
src/mod_realm_config.cpp

```

---

## Assistant (Build · Big Pickle · 7.3s)

_Thinking:_

Interesting! The current repository state does not have a `tests/` directory, and the README references `bash tests/run.sh`. But git status shows nothing to commit, working tree clean. Let me check - the git ls-tree shows no tests directory. But the README mentions the tests. This is odd. Maybe the tests were part of the workflow but never committed? Let me check the git log for these files more carefully and see the referenced test infra.

Wait, the README says "Run `bash tests/run.sh` with C++17 `g++` (or `$CXX`) and Python 3. The standalone suite uses small AzerothCore API doubles to test the production metadata parser, validator, serializer, refresh callbacks, and publisher. SQL snapshot tests adapt MySQL text casts to SQLite..."

But there's no tests directory in the repo. This is strange. Let me check if maybe there is a tests directory that's gitignored... no, `.gitignore` doesn't list tests.

Let me check the git diff to see if tests were removed in "Git conflict fix".

Actually only 2 branches? Let me check git branches and stashes.





**Tool: bash**

**Input:**
```json
{
  "command": "git -C /home/smithkt/git/mod-realm-config branch -a; git -C /home/smithkt/git/mod-realm-config stash list; git -C /home/smithkt/git/mod-realm-config reflog | head -20"
}
```

**Output:**
```
* main
  remotes/origin/main
05376a8 HEAD@{0}: commit: Git conflict fix
bdf118e HEAD@{1}: commit: Merge branch 'main' of https://github.com/Hisha/mod-realm-config.git into main
ff6f3ce HEAD@{2}: commit: Added realm isolation
4716109 HEAD@{3}: commit: Wow patch update
4e9bfdf HEAD@{4}: merge branch 'main' of https://github.com/Hisha/mod-realm-config.git: Fast-forward
97401c5 HEAD@{5}: commit: Select column mismatch
7aff1f0 HEAD@{6}: commit: Fixed to remove warnings.
e0fa777 HEAD@{7}: commit: SQL changes dictate code changes
87142e8 HEAD@{8}: commit: TEXT and Defaults are a problem
8f97d4d HEAD@{9}: commit: Defaults missing
537f9ca HEAD@{10}: commit: Column mismatches
d2308a8 HEAD@{11}: commit: added sql to store data and updated documentation
e99de46 HEAD@{12}: commit: Added icon,started sql support
8962b49 HEAD@{13}: commit: Initial commit
a5f67de HEAD@{14}: 

```

**Tool: bash**

**Input:**
```json
{
  "command": "git -C /home/smithkt/git/mod-realm-config show ff6f3ce --stat; echo \"==== 4716109 ====\"; git -C /home/smithkt/git/mod-realm-config show 4716109 --stat"
}
```

**Output:**
```
commit ff6f3ce3168ba6f784742200fa772ab33ae39f71
Author: Hisha <vndinmchine@gmail.com>
Date:   Wed Sep 23 20:12:23 2026 -0400

    Added realm isolation

 README.md                       | 19 ++++++++++++++++++-
 conf/mod_realm_config.conf.dist | 11 ++++++++++-
 src/mod_realm_config.cpp        | 12 +++++++++++-
 3 files changed, 39 insertions(+), 3 deletions(-)
==== 4716109 ====
commit 47161095619753816edc4b964df71a9ce77158d4
Author: Hisha <vndinmchine@gmail.com>
Date:   Tue Sep 15 19:38:10 2026 -0400

    Wow patch update

 README.md                                   | 114 +++++++++++++++-
 SCHEMA.md                                   |  29 ++++-
 data/sql/db-world/base/mod_realm_config.sql |   1 +
 src/mod_realm_config.cpp                    | 193 +++++++++++++++++++++++++---
 4 files changed, 317 insertions(+), 20 deletions(-)

```

---

## Assistant (Build · Big Pickle · 7.9s)

_Thinking:_

The README mentions tests/run.sh but the tests directory is NOT in the repository. The git history doesn't show a tests directory either. This means the README's test documentation references a test infrastructure that isn't actually committed. 

Wait, that's odd. The task says "Add focused tests using the repository's existing test approach." But there's no existing test approach in the repo! Let me search more carefully. Maybe tests are elsewhere. Let me check the mod-content-manager module to understand the API. Let me also check if there's a tests directory somewhere else on disk, in the AzerothCore tree, or in mod-content-manager.

Let me look at the directory listing again — `/home/smithkt/git/` has multiple repos. Let me check the parent directory.

The README references `tests/run.sh` but there's no tests directory in the repo. Let me check sibling repos to understand the broader context.

**Tool: bash**

**Input:**
```json
{
  "command": "ls -la /home/smithkt/git/"
}
```

**Output:**
```
total 100
drwxrwxr-x 25 smithkt smithkt 4096 Sep 21 19:34 .
drwxr-x--- 58 smithkt smithkt 4096 Sep 24 11:11 ..
drwxrwxr-x  4 smithkt smithkt 4096 Aug 26 16:37 AutoTalentsUI
drwxrwxr-x  4 smithkt smithkt 4096 Sep  2 14:12 BankReagentsUI
drwxrwxr-x  4 smithkt smithkt 4096 Sep  3 14:13 HuntsUI
drwxrwxr-x  4 smithkt smithkt 4096 Sep 12 19:01 Poisonkeeper
drwxrwxr-x 12 smithkt smithkt 4096 Sep 23 23:27 Portalkeeper
drwxrwxr-x  4 smithkt smithkt 4096 Aug 29 20:36 UnifiedBags335
drwxrwxr-x  3 smithkt smithkt 4096 Sep 18 20:29 buffetline
drwxrwxr-x  4 smithkt smithkt 4096 Sep  5 15:19 flux_api
drwxrwxr-x  9 smithkt smithkt 4096 Sep 19 23:07 mod-aq-war-effort
drwxrwxr-x  5 smithkt smithkt 4096 Sep 19 10:10 mod-auto-sell-junk
drwxrwxr-x  6 smithkt smithkt 4096 Sep  3 14:11 mod-auto-talents
drwxrwxr-x  6 smithkt smithkt 4096 Sep  3 14:11 mod-bank-reagents
drwxrwxr-x  9 smithkt smithkt 4096 Sep 24 12:37 mod-content-manager
drwxrwxr-x  6 smithkt smithkt 4096 Sep 11 15:05 mod-dungeon-quests
drwxrwxr-x  6 smithkt smithkt 4096 Sep 17 19:08 mod-hunts
drwxrwxr-x  7 smithkt smithkt 4096 Sep  1 21:46 mod-living-world
drwxrwxr-x  9 smithkt smithkt 4096 Sep 17 19:38 mod-native-hunts
drwxrwxr-x  8 smithkt smithkt 4096 Sep 23 05:05 mod-native-social
drwxrwxr-x  7 smithkt smithkt 4096 Sep  9 16:43 mod-realm-armory
drwxrwxr-x  5 smithkt smithkt 4096 Sep  4 23:04 mod-realm-calendar
drwxrwxr-x  7 smithkt smithkt 4096 Sep 23 20:10 mod-realm-config
drwxrwxr-x  6 smithkt smithkt 4096 Sep  5 13:43 mod-realm-news
drwxrwxr-x  7 smithkt smithkt 4096 Sep 14 15:26 mod-war-effort

```

**Tool: bash**

**Input:**
```json
{
  "command": "find /home/smithkt/git -maxdepth 2 -iname \"*content-manager*\" -o -maxdepth 2 -iname \"*realm-config*\" -o -maxdepth 2 -iname \"*portalkeeper*\" 2>/dev/null | grep -v \"\\.git/\""
}
```

**Output:**
```
/home/smithkt/git/Portalkeeper
/home/smithkt/git/Portalkeeper/Portalkeeper.slnx
/home/smithkt/git/mod-realm-config
/home/smithkt/git/mod-content-manager
/home/smithkt/git/mod-content-manager/mod-content-manager.cmake

```

---

## Assistant (Build · Big Pickle · 6.4s)

_Thinking:_

Now let me understand the mod-content-manager module. I need to look at how it stores data and what API it provides for querying build requirements.

Let me explore mod-content-manager to understand the API for client requirements.

**Tool: bash**

**Input:**
```json
{
  "command": "find /home/smithkt/git/mod-content-manager -type f -name \"*.cpp\" -o -type f -name \"*.h\" -o -type f -name \"*.sql\" -o -type f -name \"*.md\" | grep -v \"\\.git/\" | head -50"
}
```

**Output:**
```
/home/smithkt/git/mod-content-manager/README.md
/home/smithkt/git/mod-content-manager/third_party/StormLib/README.md
/home/smithkt/git/mod-content-manager/third_party/StormLib/test/TLogHelper.cpp
/home/smithkt/git/mod-content-manager/third_party/StormLib/test/StormTest.cpp
/home/smithkt/git/mod-content-manager/third_party/StormLib/storm.dll/storm_test.cpp
/home/smithkt/git/mod-content-manager/third_party/StormLib/storm.dll/storm.cpp
/home/smithkt/git/mod-content-manager/third_party/StormLib/storm.dll/storm.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/pklib/pklib.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/SFileAttributes.cpp
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/SMemUtf8.cpp
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/huffman/huff.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/huffman/huff.cpp
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/StormCommon.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/zlib/inftrees.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/zlib/zutil.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/zlib/inffixed.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/zlib/trees.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/zlib/inffast.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/zlib/gzguts.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/zlib/zlib.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/zlib/deflate.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/zlib/zconf.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/zlib/inflate.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/zlib/crc32.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/lzma/C/LzFind.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/lzma/C/LzHash.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/lzma/C/LzmaDec.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/lzma/C/Threads.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/lzma/C/LzmaEnc.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/lzma/C/Types.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/lzma/C/LzFindMt.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/resource.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/sparse/sparse.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/sparse/sparse.cpp
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/SFileOpenFileEx.cpp
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/SFileCompactArchive.cpp
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/StormLib.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/SFileFindFile.cpp
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/FileStream.cpp
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/SBaseDumpData.cpp
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/SFileListFile.cpp
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/wdk/sources-cpp.cpp
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/SFileGetFileInfo.cpp
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/libtomcrypt/src/headers/tomcrypt_prng.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/libtomcrypt/src/headers/tomcrypt_pkcs.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/libtomcrypt/src/headers/tomcrypt_custom.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/libtomcrypt/src/headers/tomcrypt_hash.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/libtomcrypt/src/headers/tomcrypt_mac.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/libtomcrypt/src/headers/tomcrypt_argchk.h
/home/smithkt/git/mod-content-manager/third_party/StormLib/src/libtomcrypt/src/headers/tomcrypt_cfg.h

```

---

## Assistant (Build · Big Pickle · 5.2s)

**Tool: bash**

**Input:**
```json
{
  "command": "find /home/smithkt/git/mod-content-manager -type d -not -path \"*/.git/*\" -not -path \"*/third_party/*\" | sort"
}
```

**Output:**
```
/home/smithkt/git/mod-content-manager
/home/smithkt/git/mod-content-manager/conf
/home/smithkt/git/mod-content-manager/data
/home/smithkt/git/mod-content-manager/data/sql
/home/smithkt/git/mod-content-manager/data/sql/db-world
/home/smithkt/git/mod-content-manager/data/sql/db-world/base
/home/smithkt/git/mod-content-manager/data/sql/db-world/updates
/home/smithkt/git/mod-content-manager/docs
/home/smithkt/git/mod-content-manager/.git
/home/smithkt/git/mod-content-manager/src
/home/smithkt/git/mod-content-manager/src/api
/home/smithkt/git/mod-content-manager/src/third_party
/home/smithkt/git/mod-content-manager/tests
/home/smithkt/git/mod-content-manager/tests/mysql_adapter
/home/smithkt/git/mod-content-manager/third_party

```

**Tool: bash**

**Input:**
```json
{
  "command": "git -C /home/smithkt/git/mod-content-manager log --oneline -20"
}
```

**Output:**
```
dc6e2a9 Added build client requirement table
b22ea2c Updated the uninstall command
9e22d7c Needed some more includes
774fa63 Getting realmname fix
4cf0bcf Add a log to see realmname
23de595 FIrst native seal vendor sale test
010b894 FIrst native seal vendor sale test
350af8b More ItemExtendedCost work
3728e65 ItemExtendedCost setup, to make it a real in game currency
cf784f2 More hash work
01c5842 Added Hunts currency slot and dbc hashing
c3d4309 wrong sha256 hash
1a0baa6 Conf file update
289ae5a Phase 4 in creating tangiable seals
ab7881a Collation fix
32eaa0f Going from theory item to real item
8ef50d2 Signed vs unsigned error correction
1ab5a1d Itemdbc work and content allocation
d010565 Phase 1 DBC understanding
d2bc146 Added icon.png

```

---

## Assistant (Build · Big Pickle · 6.8s)

_Thinking:_

Let me examine the mod-content-manager structure, specifically the API directory, and the "Added build client requirement table" commit (dc6e2a9).

**Tool: bash**

**Input:**
```json
{
  "command": "ls -la /home/smithkt/git/mod-content-manager/src /home/smithkt/git/mod-content-manager/src/api /home/smithkt/git/mod-content-manager/data/sql/db-world/base /home/smithkt/git/mod-content-manager/data/sql/db-world/updates"
}
```

**Output:**
```
/home/smithkt/git/mod-content-manager/data/sql/db-world/base:
total 20
drwxrwxr-x 2 smithkt smithkt 4096 Sep 23 06:39 .
drwxrwxr-x 4 smithkt smithkt 4096 Sep 24 12:32 ..
-rw-rw-r-- 1 smithkt smithkt 8993 Sep 24 12:01 content_manager_schema.sql

/home/smithkt/git/mod-content-manager/data/sql/db-world/updates:
total 12
drwxrwxr-x 2 smithkt smithkt 4096 Sep 24 12:32 .
drwxrwxr-x 4 smithkt smithkt 4096 Sep 24 12:32 ..
-rw-rw-r-- 1 smithkt smithkt 1035 Sep 24 12:32 2026_09_24_05_content_manager_client_requirements.sql

/home/smithkt/git/mod-content-manager/src:
total 520
drwxrwxr-x 4 smithkt smithkt  4096 Sep 24 11:58 .
drwxrwxr-x 9 smithkt smithkt  4096 Sep 24 12:37 ..
-rw-rw-r-- 1 smithkt smithkt 15282 Sep 24 12:14 ContentAllocationRegistry.cpp
-rw-rw-r-- 1 smithkt smithkt  1433 Sep 23 06:59 ContentAllocationRegistry.h
-rw-rw-r-- 1 smithkt smithkt 12991 Sep 23 06:39 ContentBaselineRegistry.cpp
-rw-rw-r-- 1 smithkt smithkt  1278 Sep 23 06:39 ContentBaselineRegistry.h
-rw-rw-r-- 1 smithkt smithkt  2974 Sep 23 06:39 ContentBuildHash.cpp
-rw-rw-r-- 1 smithkt smithkt   359 Sep 23 06:39 ContentBuildHash.h
-rw-rw-r-- 1 smithkt smithkt  3422 Sep 23 06:39 ContentBuildPaths.h
-rw-rw-r-- 1 smithkt smithkt  4888 Sep 23 06:39 ContentBuildPublisher.cpp
-rw-rw-r-- 1 smithkt smithkt   659 Sep 23 06:39 ContentBuildPublisher.h
-rw-rw-r-- 1 smithkt smithkt 12766 Sep 24 12:33 ContentBuildRegistry.cpp
-rw-rw-r-- 1 smithkt smithkt  1978 Sep 24 12:32 ContentBuildRegistry.h
-rw-rw-r-- 1 smithkt smithkt 40934 Sep 24 12:15 ContentBuildService.cpp
-rw-rw-r-- 1 smithkt smithkt   924 Sep 23 06:39 ContentBuildService.h
-rw-rw-r-- 1 smithkt smithkt  4907 Sep 23 06:39 ContentCapabilityProvider.cpp
-rw-rw-r-- 1 smithkt smithkt   405 Sep 23 06:39 ContentCapabilityProvider.h
-rw-rw-r-- 1 smithkt smithkt  1875 Sep 24 11:58 ContentClientRequirement.h
-rw-rw-r-- 1 smithkt smithkt 39239 Sep 24 12:01 ContentCommands.cpp
-rw-rw-r-- 1 smithkt smithkt 11009 Sep 23 06:39 ContentCurrencyServer.cpp
-rw-rw-r-- 1 smithkt smithkt  1424 Sep 23 06:39 ContentCurrencyServer.h
-rw-rw-r-- 1 smithkt smithkt  9484 Sep 23 06:39 ContentExtendedCostServer.cpp
-rw-rw-r-- 1 smithkt smithkt  1187 Sep 23 06:39 ContentExtendedCostServer.h
-rw-rw-r-- 1 smithkt smithkt   946 Sep 23 06:39 ContentItemOccupancy.h
-rw-rw-r-- 1 smithkt smithkt  7869 Sep 23 06:39 ContentManager.cpp
-rw-rw-r-- 1 smithkt smithkt  1632 Sep 23 06:39 ContentManager.h
-rw-rw-r-- 1 smithkt smithkt 37553 Sep 24 12:13 ContentPackage.cpp
-rw-rw-r-- 1 smithkt smithkt  5049 Sep 24 11:58 ContentPackage.h
-rw-rw-r-- 1 smithkt smithkt  3170 Sep 23 07:08 ContentPackageLifecycle.cpp
-rw-rw-r-- 1 smithkt smithkt  8225 Sep 23 07:07 ContentPackageLifecycle.h
-rw-rw-r-- 1 smithkt smithkt  6073 Sep 23 06:39 ContentPackageRegistry.cpp
-rw-rw-r-- 1 smithkt smithkt  1106 Sep 23 06:39 ContentPackageRegistry.h
-rw-rw-r-- 1 smithkt smithkt  5413 Sep 23 06:39 ContentResourceAllocator.cpp
-rw-rw-r-- 1 smithkt smithkt  1864 Sep 23 06:39 ContentResourceAllocator.h
-rw-rw-r-- 1 smithkt smithkt 29748 Sep 23 06:39 ContentServerBundle.cpp
-rw-rw-r-- 1 smithkt smithkt  2673 Sep 23 06:39 ContentServerBundle.h
-rw-rw-r-- 1 smithkt smithkt 31574 Sep 23 06:39 ContentServerDeployment.cpp
-rw-rw-r-- 1 smithkt smithkt  1062 Sep 23 06:39 ContentServerDeployment.h
-rw-rw-r-- 1 smithkt smithkt  5487 Sep 23 07:00 ContentServerOwnership.cpp
-rw-rw-r-- 1 smithkt smithkt  2164 Sep 23 07:00 ContentServerOwnership.h
-rw-rw-r-- 1 smithkt smithkt  2074 Sep 23 06:39 ContentVendorRow.cpp
-rw-rw-r-- 1 smithkt smithkt  5159 Sep 23 06:39 ContentVendorServer.cpp
-rw-rw-r-- 1 smithkt smithkt  1128 Sep 23 06:39 ContentVendorServer.h
-rw-rw-r-- 1 smithkt smithkt  6230 Sep 23 06:39 CurrencyCategoryDbcComposer.cpp
-rw-rw-r-- 1 smithkt smithkt   995 Sep 23 06:39 CurrencyCategoryDbcComposer.h
-rw-rw-r-- 1 smithkt smithkt  3210 Sep 23 06:39 CurrencyDbcComposer.cpp
-rw-rw-r-- 1 smithkt smithkt   930 Sep 23 06:39 CurrencyDbcComposer.h
-rw-rw-r-- 1 smithkt smithkt  3652 Sep 23 06:39 DbcDescriptor.cpp
-rw-rw-r-- 1 smithkt smithkt   837 Sep 23 06:39 DbcDescriptor.h
-rw-rw-r-- 1 smithkt smithkt  7325 Sep 23 06:39 DbcReader.cpp
-rw-rw-r-- 1 smithkt smithkt  1030 Sep 23 06:39 DbcReader.h
-rw-rw-r-- 1 smithkt smithkt  2696 Sep 23 06:39 ItemDbcComposer.cpp
-rw-rw-r-- 1 smithkt smithkt   409 Sep 23 06:39 ItemDbcComposer.h
-rw-rw-r-- 1 smithkt smithkt  4349 Sep 23 06:39 ItemExtendedCostDbc.cpp
-rw-rw-r-- 1 smithkt smithkt  1229 Sep 23 06:39 ItemExtendedCostDbc.h
-rw-rw-r-- 1 smithkt smithkt  6537 Sep 23 06:39 MpqBuilder.cpp
-rw-rw-r-- 1 smithkt smithkt   518 Sep 23 06:39 MpqBuilder.h
-rw-rw-r-- 1 smithkt smithkt  1279 Sep 23 06:39 ServerTableDescriptor.cpp
-rw-rw-r-- 1 smithkt smithkt  1007 Sep 23 06:39 ServerTableDescriptor.h
drwxrwxr-x 2 smithkt smithkt  4096 Sep 23 06:39 api
-rw-rw-r-- 1 smithkt smithkt   578 Sep 23 06:39 mod_content_manager_loader.cpp
drwxrwxr-x 4 smithkt smithkt  4096 Sep 23 06:39 third_party

/home/smithkt/git/mod-content-manager/src/api:
total 12
drwxrwxr-x 2 smithkt smithkt 4096 Sep 23 06:39 .
drwxrwxr-x 4 smithkt smithkt 4096 Sep 24 11:58 ..
-rw-rw-r-- 1 smithkt smithkt  939 Sep 23 06:39 ContentCapabilityApiV1.h

```

**Tool: bash**

**Input:**
```json
{
  "command": "git -C /home/smithkt/git/mod-content-manager show dc6e2a9 --stat"
}
```

**Output:**
```
commit dc6e2a968fa886d80abbe57b62c315d1aef26a26
Author: Hisha <vndinmchine@gmail.com>
Date:   Thu Sep 24 12:50:14 2026 -0400

    Added build client requirement table

 README.md                                          |   60 +
 data/sql/db-world/base/content_manager_schema.sql  |    9 +
 ...9_24_05_content_manager_client_requirements.sql |   16 +
 schema3-client-requirements-changes.zip            |  Bin 0 -> 60426 bytes
 session-ses_f2be.md                                | 7593 ++++++++++++++++++++
 src/ContentAllocationRegistry.cpp                  |   30 +-
 src/ContentBuildRegistry.cpp                       |   56 +-
 src/ContentBuildRegistry.h                         |    9 +
 src/ContentBuildService.cpp                        |   22 +-
 src/ContentClientRequirement.h                     |   50 +
 src/ContentCommands.cpp                            |   13 +
 src/ContentPackage.cpp                             |   57 +-
 src/ContentPackage.h                               |    4 +
 tests/PHASE4_TESTS.md                              |   26 +-
 tests/client_requirement_mysql_tests.cpp           |  210 +
 tests/run_phase4.py                                |    5 +-
 tests/schema3_tests.cpp                            |  161 +
 17 files changed, 8297 insertions(+), 24 deletions(-)

```

---

## Assistant (Build · Big Pickle · 27.9s)

_Thinking:_

Let me look at the relevant mod-content-manager files to understand the API. Key files:
- `ContentClientRequirement.h`
- `ContentBuildRegistry.h` and `.cpp`
- `ContentPackage.h`
- The SQL for the client requirements table
- How the API is consumed (e.g., an interface for getting requirements by build)

Let me read these.

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-content-manager/src/ContentClientRequirement.h"
}
```

**Output:**
```
<path>/home/smithkt/git/mod-content-manager/src/ContentClientRequirement.h</path>
<type>file</type>
<content>
1: #ifndef CONTENT_CLIENT_REQUIREMENT_H
2: #define CONTENT_CLIENT_REQUIREMENT_H
3: 
4: #include <cstddef>
5: #include <set>
6: #include <string>
7: #include <vector>
8: 
9: // Immutable client capability requirements attached to a generated build.
10: // Packages declare WHAT a build requires; downstream modules decide HOW that
11: // capability is provided. Content Manager only records the declared set with
12: // the exact build that used those package manifests.
13: namespace ContentClientRequirement
14: {
15: inline constexpr char const* ProtectedFrameXml = "protected-framexml";
16: // Storage bound: requirement names persist into a fixed-width SQL column.
17: inline constexpr std::size_t MaxNameLength = 64;
18: 
19: inline bool IsSupported(std::string const& requirement)
20: {
21:     return requirement == ProtectedFrameXml;
22: }
23: 
24: // Registry-bound check before persistence: supported names within the storage
25: // bound, strictly sorted, no duplicates.
26: inline bool ValidSet(std::vector<std::string> const& requirements)
27: {
28:     for (auto const& requirement : requirements)
29:         if (!IsSupported(requirement) || requirement.size() > MaxNameLength)
30:             return false;
31:     for (std::size_t i = 1; i < requirements.size(); ++i)
32:         if (!(requirements[i - 1] < requirements[i]))
33:             return false;
34:     return true;
35: }
36: 
37: // Deterministic union of the requirement sets declared by the exact package
38: // manifests participating in a build. Each manifest is already a deduplicated,
39: // sorted set; the combined result is sorted byte-wise, contains no duplicates,
40: // and does not depend on package discovery order.
41: inline std::vector<std::string> Merge(std::vector<std::vector<std::string>> const& packageSets)
42: {
43:     std::set<std::string> combined;
44:     for (auto const& set : packageSets)
45:         for (auto const& requirement : set)
46:             combined.insert(requirement);
47:     return {combined.begin(), combined.end()};
48: }
49: }
50: #endif

(End of file - total 50 lines)
</content>
```

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-content-manager/src/ContentBuildRegistry.h"
}
```

**Output:**
```
<path>/home/smithkt/git/mod-content-manager/src/ContentBuildRegistry.h</path>
<type>file</type>
<content>
1: #ifndef CONTENT_BUILD_REGISTRY_H
2: #define CONTENT_BUILD_REGISTRY_H
3: 
4: #include "ContentBuildPublisher.h"
5: 
6: #include <cstdint>
7: #include <string>
8: #include <optional>
9: #include <vector>
10: #include <filesystem>
11: 
12: struct ContentBuildRecord
13: {
14:     std::uint32_t buildNumber = 0;
15:     std::string realmName;
16:     std::string filename;
17:     std::uint32_t packageCount = 0;
18:     std::uint32_t fileCount = 0;
19:     std::string state = "STAGED";
20:     std::string sha256;
21:     // Immutable client requirements recorded with this exact build. Sorted,
22:     // deduplicated, sourced from the manifests that participated in the build.
23:     std::vector<std::string> clientRequirements;
24: };
25: struct ContentServerBuildRecord;
26: 
27: class ContentBuildRegistry
28: {
29: public:
30:     bool GetBuilds(std::vector<ContentBuildRecord>& records, std::string& error) const;
31:     bool GetBuild(std::uint32_t number, std::optional<ContentBuildRecord>& record, std::string& error) const;
32:     bool GetActiveBuild(std::optional<ContentBuildRecord>& record, std::string& error) const;
33:     bool ActivateBuild(std::uint32_t number, std::filesystem::path const& outputDirectory,
34:         std::filesystem::path const& publishDirectory, ContentPublicationResult& publication,
35:         bool& alreadyActive, std::string& error) const;
36:     bool NextNumber(std::uint32_t& number, std::string& error) const;
37:     // Called only after the private output MPQ is closed and hashed successfully.
38:     bool Record(ContentBuildRecord const& record, ContentServerBuildRecord const& server,
39:         std::string& error) const;
40:     // Client requirements recorded immutably against build `number`. Never
41:     // recomputed from current package state. An existing build with no recorded
42:     // requirements succeeds with an empty set; a build that does not exist fails
43:     // with a distinguishable error ("Build <n> does not exist").
44:     bool GetClientRequirements(std::uint32_t number, std::vector<std::string>& requirements,
45:         std::string& error) const;
46: };
47: #endif

(End of file - total 47 lines)
</content>
```

---

## Assistant (Build · Big Pickle · 13.9s)

_Thinking:_

Now let me look at how mod-realm-config currently integrates with mod-content-manager. Interesting - the current mod-realm-config code queries the `content_manager_build` table directly via SQL (the `ContentContractQuery` and `MetadataSQL`). So the "integration boundary" is the world database, specifically the `content_manager_build` table. Let me look at how the ContentBuildRegistry::GetClientRequirements is implemented, and the content_manager_schema.sql to understand the new table.

**Tool: bash**

**Input:**
```json
{
  "command": "grep -n \"GetClientRequirements\\|GetActiveBuild\\|clientRequirements\\|content_manager_build_client_requirement\\|GetBuild\\b\" /home/smithkt/git/mod-content-manager/src/ContentBuildRegistry.cpp"
}
```

**Output:**
```
58:        || !ContentClientRequirement::ValidSet(record.clientRequirements)
77:    if (!record.clientRequirements.empty())
79:        std::string requirements = "INSERT INTO content_manager_build_client_requirement (build_number,requirement) VALUES ";
80:        for (std::size_t i = 0; i < record.clientRequirements.size(); ++i)
83:            requirements += "(" + std::to_string(record.buildNumber) + "," + SqlText(record.clientRequirements[i]) + ")";
113:    if (!GetClientRequirements(record.buildNumber, savedRequirements, error)) return false;
114:    if (savedRequirements != record.clientRequirements)
119:bool ContentBuildRegistry::GetClientRequirements(std::uint32_t number,
137:        "content_manager_build_client_requirement r ON r.build_number=" + std::to_string(number)
198:bool ContentBuildRegistry::GetBuild(std::uint32_t number, std::optional<ContentBuildRecord>& record, std::string& error) const
208:bool ContentBuildRegistry::GetActiveBuild(std::optional<ContentBuildRecord>& record, std::string& error) const
233:        if (!GetBuild(number, record, error)) return false;
257:        if (!GetActiveBuild(active, error)) return false;

```

**Tool: bash**

**Input:**
```json
{
  "command": "cat /home/smithkt/git/mod-content-manager/data/sql/db-world/updates/2026_09_24_05_content_manager_client_requirements.sql"
}
```

**Output:**
```
-- Schema 3 client capability requirements for the mod-content-manager module.
-- Applied automatically by the AzerothCore database updater to an EXISTING
-- installation (this module's data/sql/db-world directory is an included update
-- path). Rerunnable/convergent for existing installs and fresh installs alike:
--   * CREATE TABLE IF NOT EXISTS only, no ALTER
--   * no DROP, no destructive operations
--   * no backfill, no INSERT
--   * never modifies existing content_manager_build rows
-- Existing builds naturally have zero requirement rows, which means "requires
-- nothing". The composite key is the duplicate guard. No foreign key is used so
-- historical build metadata stays conservative and independently readable.
CREATE TABLE IF NOT EXISTS `content_manager_build_client_requirement` (
    `build_number` INT UNSIGNED NOT NULL,
    `requirement` VARCHAR(64) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
    PRIMARY KEY (`build_number`, `requirement`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;
```

---

## Assistant (Build · Big Pickle · 25.4s)

_Thinking:_

Now I understand the new table. Let me read the ContentBuildRegistry.cpp GetClientRequirements implementation, and how mod-realm-config might access this API. Let me look at the full implementation, the ContentBuildService.h/cpp for the "API" and how mod-realm-config could consume it.

Actually the key question is: how does mod-realm-config currently integrate with mod-content-manager? It only reads the `content_manager_build` table directly from the world database via SQL. There's no C++ API dependency (the module doesn't include content-manager headers). Let me verify by checking mod_realm_config.cpp includes - it only includes AzerothCore headers. So the integration is via direct database queries.

The task says:
"Use the module's supported API/integration boundary. Do NOT query the new content_manager_build_client_requirement table directly from mod-realm-config unless the existing architecture clearly establishes direct database queries as the supported integration mechanism. Prefer consuming the API exposed by mod-content-manager."

So the existing architecture clearly establishes direct database queries as the supported integration mechanism (the module queries `content_manager_build` directly). So querying the new `content_manager_build_client_requirement` table directly is consistent with the existing integration model. But the task says to "prefer consuming the API exposed by mod-content-manager". However, there is no C++ API dependency at all currently. The mod-content-manager's `GetClientRequirements` is a C++ class method `ContentBuildRegistry` - not really importable from mod-realm-config without a compile-time dependency.

Wait, let me check whether mod-content-manager exposes any header that mod-realm-config could use. Let me look at the `api` directory - ContentCapabilityApiV1.h. Let me look at it.

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-content-manager/src/api/ContentCapabilityApiV1.h"
}
```

**Output:**
```
<path>/home/smithkt/git/mod-content-manager/src/api/ContentCapabilityApiV1.h</path>
<type>file</type>
<content>
1: #ifndef CONTENT_CAPABILITY_API_V1_H
2: #define CONTENT_CAPABILITY_API_V1_H
3: #include <cstdint>
4: #include <string>
5: #include <vector>
6: // Public optional ABI, owned by Content Manager. Vendored verbatim by consumers.
7: // Discover once at world startup through WorldScript RTTI; no module link dependency.
8: namespace ContentCapabilitiesV1
9: {
10: struct Resource { std::string symbol, kind; std::uint32_t value=0; };
11: struct Vendor { std::uint32_t creatureEntry=0, itemEntry=0, extendedCostId=0; };
12: enum class Result { Inactive, Ready, Invalid };
13: class Provider
14: {
15: public:
16:     virtual ~Provider() = default;
17:     // An ACTIVE, APPLIED, validated vendor declaration is the activation signal.
18:     // STAGED artifacts and retained leases alone never provide capabilities.
19:     virtual Result Resolve(std::string const& package, std::string const& vendorSymbol,
20:         std::vector<Resource>& resources, Vendor& vendor, std::string& reason) const = 0;
21: };
22: }
23: #endif

(End of file - total 23 lines)
</content>
```

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-content-manager/src/ContentBuildRegistry.cpp",
  "offset": 100,
  "limit": 160
}
```

**Output:**
```
<path>/home/smithkt/git/mod-content-manager/src/ContentBuildRegistry.cpp</path>
<type>file</type>
<content>
100:     {
101:         error = "Build number already belongs to different metadata; completed MPQ was preserved";
102:         return false;
103:     }
104:     auto sidecars = WorldDatabase.Query("SELECT bundle_filename,bundle_sha256,parity_filename,parity_sha256,"
105:         "server_state FROM content_manager_server_build WHERE build_number=" + std::to_string(record.buildNumber));
106:     if (!sidecars || sidecars->Fetch()[0].Get<std::string>() != server.bundleFilename
107:         || sidecars->Fetch()[1].Get<std::string>() != server.bundleSha256
108:         || sidecars->Fetch()[2].Get<std::string>() != server.parityFilename
109:         || sidecars->Fetch()[3].Get<std::string>() != server.paritySha256
110:         || sidecars->Fetch()[4].Get<std::string>() != "STAGED")
111:     { error = "Server sidecar metadata could not be verified; inspect SQL logs"; return false; }
112:     std::vector<std::string> savedRequirements;
113:     if (!GetClientRequirements(record.buildNumber, savedRequirements, error)) return false;
114:     if (savedRequirements != record.clientRequirements)
115:     { error = "Client requirements could not be verified; inspect SQL logs"; return false; }
116:     return true;
117: }
118: 
119: bool ContentBuildRegistry::GetClientRequirements(std::uint32_t number,
120:     std::vector<std::string>& requirements, std::string& error) const
121: {
122:     requirements.clear();
123:     // Explicit existence gate: EXISTS always returns exactly one row, so the
124:     // empty-result case cannot be confused with "no such build". This keeps a
125:     // nonexistent build clearly distinguishable from "build requires nothing",
126:     // which the future mod-realm-config consumer depends on.
127:     auto exists = WorldDatabase.Query("SELECT EXISTS(SELECT 1 FROM content_manager_build WHERE build_number="
128:         + std::to_string(number) + ")");
129:     if (!exists || exists->Fetch()[0].Get<std::uint64_t>() == 0)
130:     {
131:         error = "Build " + std::to_string(number) + " does not exist";
132:         return false;
133:     }
134:     // LEFT JOIN guarantees a build without rows still returns a row, so the
135:     // empty result is a successful "requires nothing" rather than a failed query.
136:     auto query = WorldDatabase.Query("SELECT r.requirement FROM (SELECT 1) seed LEFT JOIN "
137:         "content_manager_build_client_requirement r ON r.build_number=" + std::to_string(number)
138:         + " ORDER BY r.requirement");
139:     if (!query) { error = DatabaseError; return false; }
140:     do
141:     {
142:         auto f = query->Fetch();
143:         if (!f[0].IsNull())
144:         {
145:             if (!ContentClientRequirement::IsSupported(f[0].Get<std::string>()))
146:             {
147:                 error = "Build " + std::to_string(number) + " has an unknown client requirement; check build registry integrity";
148:                 requirements.clear();
149:                 return false;
150:             }
151:             requirements.push_back(f[0].Get<std::string>());
152:         }
153:     } while (query->NextRow());
154:     return true;
155: }
156: 
157: namespace
158: {
159: std::mutex lifecycleMutex;
160: std::filesystem::path Artifact(ContentBuildRecord const& record, std::filesystem::path const& root)
161: {
162:     using namespace ContentBuildPaths;
163:     Require(!root.empty(), "OutputDirectory must not be empty");
164:     auto name = Target(record.filename);
165:     Require(name.find('/') == std::string::npos && name == record.filename,
166:         "Build filename must be a single safe path component");
167:     return fs::absolute(root / name);
168: }
169: }
170: 
171: bool ContentBuildRegistry::GetBuilds(std::vector<ContentBuildRecord>& records, std::string& error) const
172: {
173:     records.clear();
174:     // LEFT JOIN guarantees an empty registry still returns a row, unlike a failed query.
175:     auto query = WorldDatabase.Query("SELECT b.build_number, b.realm_name, b.filename, b.package_count, "
176:         "b.file_count, b.state, b.sha256 FROM (SELECT 1) AS seed LEFT JOIN content_manager_build b ON 1=1 "
177:         "ORDER BY b.build_number DESC");
178:     if (!query) { error = DatabaseError; return false; }
179:     do
180:     {
181:         auto f = query->Fetch();
182:         if (!f[0].IsNull())
183:         {
184:             auto hash = f[6].Get<std::string>();
185:             if (!ContentBuildHash::Valid(hash))
186:             {
187:                 error = "Build " + std::to_string(f[0].Get<uint32>()) + " has an invalid SHA256; check build registry integrity";
188:                 records.clear();
189:                 return false;
190:             }
191:             records.push_back({f[0].Get<uint32>(), f[1].Get<std::string>(), f[2].Get<std::string>(),
192:                 f[3].Get<uint32>(), f[4].Get<uint32>(), f[5].Get<std::string>(), hash, {}});
193:         }
194:     } while (query->NextRow());
195:     return true;
196: }
197: 
198: bool ContentBuildRegistry::GetBuild(std::uint32_t number, std::optional<ContentBuildRecord>& record, std::string& error) const
199: {
200:     record.reset();
201:     std::vector<ContentBuildRecord> records;
202:     if (!GetBuilds(records, error)) return false;
203:     for (auto const& row : records)
204:         if (row.buildNumber == number) record = row;
205:     return true;
206: }
207: 
208: bool ContentBuildRegistry::GetActiveBuild(std::optional<ContentBuildRecord>& record, std::string& error) const
209: {
210:     record.reset();
211:     std::vector<ContentBuildRecord> records;
212:     if (!GetBuilds(records, error)) return false;
213:     for (auto const& row : records)
214:         if (row.state == "ACTIVE")
215:         {
216:             if (record) { error = "Multiple ACTIVE builds found; repair registry before activation"; return false; }
217:             record = row;
218:         }
219:     return true;
220: }
221: 
222: bool ContentBuildRegistry::ActivateBuild(std::uint32_t number, std::filesystem::path const& outputDirectory,
223:     std::filesystem::path const& publishDirectory, ContentPublicationResult& publication,
224:     bool& alreadyActive, std::string& error) const
225: {
226:     std::lock_guard<std::mutex> lock(lifecycleMutex);
227:     alreadyActive = false;
228:     publication = {};
229:     try
230:     {
231:         using ContentBuildPaths::Require;
232:         std::optional<ContentBuildRecord> record;
233:         if (!GetBuild(number, record, error)) return false;
234:         Require(record.has_value(), "Build does not exist");
235:         Require(ContentBuildHash::Valid(record->sha256), "Build record has an invalid SHA256; activation refused");
236:         Require(record->state == "STAGED" || record->state == "ACTIVE" || record->state == "SUPERSEDED", "Unknown build state");
237:         publication = ContentBuildPublisher().Publish(Artifact(*record, outputDirectory), publishDirectory, record->sha256);
238:         if (!publication.success)
239:         {
240:             error = "Publication failed; build states unchanged: " + publication.error;
241:             return false;
242:         }
243:         auto const& actual = publication.sha256;
244:         if (record->state == "ACTIVE") { alreadyActive = true; return true; }
245:         auto transaction = WorldDatabase.BeginTransaction();
246:         // This singleton write serializes lifecycle transactions across worldserver processes.
247:         // INSERT also restores the lock row if an administrator removed it.
248:         transaction->Append("INSERT INTO content_manager_build_lock (id) VALUES (1) ON DUPLICATE KEY UPDATE id=1");
249:         // Join to the verified target so a removed/changed target cannot demote the active build.
250:         auto predicate = "target.build_number=" + std::to_string(number) + " AND target.sha256=" + SqlText(actual)
251:             + " AND target.filename=" + SqlText(record->filename) + " AND target.state IN ('STAGED','SUPERSEDED','ACTIVE')";
252:         transaction->Append("UPDATE content_manager_build old JOIN content_manager_build target ON " + predicate
253:             + " SET old.state='SUPERSEDED' WHERE old.state='ACTIVE' AND old.build_number<>target.build_number");
254:         transaction->Append("UPDATE content_manager_build target SET target.state='ACTIVE' WHERE " + predicate);
255:         WorldDatabase.DirectCommitTransaction(transaction);
256:         std::optional<ContentBuildRecord> active;
257:         if (!GetActiveBuild(active, error)) return false;
258:         Require(active && active->buildNumber == number && active->sha256 == actual,
259:             "Activation could not be verified; check SQL logs and .content build list (another administrator may have activated a build)");

(Showing lines 100-259 of 263. Use offset=260 to continue.)
</content>
```

---

## Assistant (Build · Big Pickle · 2.9s)

_Thinking:_

Now I understand the Content Manager API. It provides:
- `ContentBuildRegistry::GetClientRequirements(number, requirements, error)` - returns success/failure plus the requirements.

But it's a C++ class that mod-realm-config would need to link against. Let me check how mod-realm-config would access this. Currently mod-realm-config uses only direct database queries (`WorldDatabase.Query`). There is no C++ dependency on mod-content-manager.

Let me check how mod-realm-config could consume the API. Does mod-content-manager expose its registry as a script object that can be looked up at runtime (like through ScriptMgr singleton or RTTI)? Let me look at ContentManager.h and mod_content_manager_loader.cpp and how ContentBuildRegistry is instantiated.

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-content-manager/src/ContentManager.h"
}
```

**Output:**
```
<path>/home/smithkt/git/mod-content-manager/src/ContentManager.h</path>
<type>file</type>
<content>
1: #ifndef CONTENT_MANAGER_H
2: #define CONTENT_MANAGER_H
3: 
4: #include <filesystem>
5: #include <cstdint>
6: #include <string>
7: #include <vector>
8: 
9: enum class ContentPackageSource
10: {
11:     PatchHold,
12:     Module
13: };
14: 
15: struct ContentPackageCandidate
16: {
17:     std::filesystem::path path;
18:     std::string filename;
19: 
20:     ContentPackageSource source;
21:     std::string provider;
22: };
23: 
24: class ContentManager
25: {
26: public:
27:     static ContentManager& Instance();
28: 
29:     void LoadConfig();
30: 
31:     bool IsEnabled() const;
32: 
33:     std::string const& GetPatchHoldDirectory() const;
34:     std::string const& GetWorkDirectory() const;
35:     std::string const& GetOutputDirectory() const;
36:     std::string const& GetPublishDirectory() const;
37: 	std::string const& GetModuleDirectory() const;
38:     std::string const& GetBaselineDbcDirectory() const;
39:     std::uint32_t GetClientBuild() const;
40:     std::string const& GetItemBaselineSha256() const;
41:     std::string const& GetCurrencyTypesBaselineSha256() const { return _currencyTypesBaselineSha256; }
42: 
43: 	std::vector<ContentPackageCandidate> ScanAvailablePackages() const;
44:     std::vector<ContentPackageCandidate> ScanPatchHold() const;
45: 
46: private:
47:     ContentManager() = default;
48: 
49:     bool _enabled = false;
50: 
51:     std::string _patchHoldDirectory;
52:     std::string _workDirectory;
53:     std::string _outputDirectory;
54:     std::string _publishDirectory;
55: 	std::string _moduleDirectory;
56:     std::string _baselineDbcDirectory;
57:     std::uint32_t _clientBuild = 12340;
58:     std::string _itemBaselineSha256;
59:     std::string _currencyTypesBaselineSha256;
60: };
61: 
62: #define sContentManager ContentManager::Instance()
63: 
64: #endif

(End of file - total 64 lines)
</content>
```

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-content-manager/src/mod_content_manager_loader.cpp"
}
```

**Output:**
```
<path>/home/smithkt/git/mod-content-manager/src/mod_content_manager_loader.cpp</path>
<type>file</type>
<content>
1: #include "ContentManager.h"
2: #include "ScriptMgr.h"
3: #include "ContentCapabilityProvider.h"
4: 
5: void AddSC_content_manager_commands();
6: 
7: class content_manager_worldscript : public WorldScript, public ContentCapabilityProvider
8: {
9: public:
10:     content_manager_worldscript()
11:         : WorldScript("content_manager_worldscript")
12:     {
13:     }
14: 
15:     void OnAfterConfigLoad(bool /*reload*/) override
16:     {
17:         sContentManager.LoadConfig();
18:     }
19: };
20: 
21: void Addmod_content_managerScripts()
22: {
23:     new content_manager_worldscript();
24: 
25:     AddSC_content_manager_commands();
26: }

(End of file - total 26 lines)
</content>
```

---

## Assistant (Build · Big Pickle · 2.9s)

_Thinking:_

There's no public C++ API in mod-content-manager that mod-realm-config could link against for `GetClientRequirements`. The `ContentBuildRegistry` is a class used internally. Let me check how mod-realm-config would consume it. Let me look at ContentBuildService.h and ContentBuildRegistry.h to see if the registry is exposed as a global or accessible through a singleton.

Let me search for the ContentBuildRegistry usage - where is it instantiated?

**Tool: bash**

**Input:**
```json
{
  "command": "grep -rn \"ContentBuildRegistry\" /home/smithkt/git/mod-content-manager/src --include=\"*.cpp\" --include=\"*.h\" | grep -v \"mod-realm-config\""
}
```

**Output:**
```
/home/smithkt/git/mod-content-manager/src/ContentAllocationRegistry.h:5:#include "ContentBuildRegistry.h"
/home/smithkt/git/mod-content-manager/src/ContentBuildService.cpp:4:#include "ContentBuildRegistry.h"
/home/smithkt/git/mod-content-manager/src/ContentBuildService.cpp:183:        ContentBuildRegistry builds;
/home/smithkt/git/mod-content-manager/src/ContentPackageLifecycle.cpp:3:#include "ContentBuildRegistry.h"
/home/smithkt/git/mod-content-manager/src/ContentPackageLifecycle.cpp:69:    if (!ContentBuildRegistry().GetActiveBuild(active, error))
/home/smithkt/git/mod-content-manager/src/ContentCommands.cpp:6:#include "ContentBuildRegistry.h"
/home/smithkt/git/mod-content-manager/src/ContentCommands.cpp:230:        if (!ContentBuildRegistry().GetBuilds(builds, error))
/home/smithkt/git/mod-content-manager/src/ContentCommands.cpp:664:        if (!ContentBuildRegistry().GetBuilds(records, error))
/home/smithkt/git/mod-content-manager/src/ContentCommands.cpp:676:                if (!ContentBuildRegistry().GetClientRequirements(row.buildNumber, requirements, error))
/home/smithkt/git/mod-content-manager/src/ContentCommands.cpp:717:        ContentBuildRegistry registry;
/home/smithkt/git/mod-content-manager/src/ContentServerDeployment.cpp:5:#include "ContentBuildRegistry.h"
/home/smithkt/git/mod-content-manager/src/ContentServerDeployment.cpp:156:        if (!ContentBuildRegistry().GetBuild(build, record, error)) return false;
/home/smithkt/git/mod-content-manager/src/ContentServerDeployment.cpp:227:        if (!ContentBuildRegistry().GetBuild(build, record, error)) return false;
/home/smithkt/git/mod-content-manager/src/ContentAllocationRegistry.cpp:248:    if (!ContentBuildRegistry().GetClientRequirements(build.buildNumber, savedRequirements, error)) return false;
/home/smithkt/git/mod-content-manager/src/ContentBuildRegistry.cpp:1:#include "ContentBuildRegistry.h"
/home/smithkt/git/mod-content-manager/src/ContentBuildRegistry.cpp:31:bool ContentBuildRegistry::NextNumber(std::uint32_t& number, std::string& error) const
/home/smithkt/git/mod-content-manager/src/ContentBuildRegistry.cpp:52:bool ContentBuildRegistry::Record(ContentBuildRecord const& record,
/home/smithkt/git/mod-content-manager/src/ContentBuildRegistry.cpp:119:bool ContentBuildRegistry::GetClientRequirements(std::uint32_t number,
/home/smithkt/git/mod-content-manager/src/ContentBuildRegistry.cpp:171:bool ContentBuildRegistry::GetBuilds(std::vector<ContentBuildRecord>& records, std::string& error) const
/home/smithkt/git/mod-content-manager/src/ContentBuildRegistry.cpp:198:bool ContentBuildRegistry::GetBuild(std::uint32_t number, std::optional<ContentBuildRecord>& record, std::string& error) const
/home/smithkt/git/mod-content-manager/src/ContentBuildRegistry.cpp:208:bool ContentBuildRegistry::GetActiveBuild(std::optional<ContentBuildRecord>& record, std::string& error) const
/home/smithkt/git/mod-content-manager/src/ContentBuildRegistry.cpp:222:bool ContentBuildRegistry::ActivateBuild(std::uint32_t number, std::filesystem::path const& outputDirectory,
/home/smithkt/git/mod-content-manager/src/ContentBuildRegistry.h:27:class ContentBuildRegistry
/home/smithkt/git/mod-content-manager/src/ContentCapabilityProvider.cpp:3:#include "ContentBuildRegistry.h"
/home/smithkt/git/mod-content-manager/src/ContentCapabilityProvider.cpp:22:    if(!ContentBuildRegistry().GetActiveBuild(active,reason))return Result::Invalid;

```

---

## Assistant (Build · Big Pickle · 151.7s)

_Thinking:_

So mod-content-manager's `ContentBuildRegistry` is a plain class with a no-arg constructor; it executes direct `WorldDatabase.Query` calls internally. It's not a shared singleton or ABI. mod-realm-config currently integrates with mod-content-manager purely through the world database (direct SQL on `content_manager_build`). There's no module-to-module C++ API linkage.

So the existing architecture clearly establishes **direct database queries** as the supported integration mechanism. The task states:

"Use the module's supported API/integration boundary. Do NOT query the new content_manager_build_client_requirement table directly from mod-realm-config unless the existing architecture clearly establishes direct database queries as the supported integration mechanism. Prefer consuming the API exposed by mod-content-manager."

The existing architecture does clearly establish direct database queries (it queries `content_manager_build` directly, with compatibility contract checks via information_schema). So querying `content_manager_build_client_requirement` directly is consistent. The note says "Prefer consuming the API exposed by mod-content-manager" — but there is no such consumable API exposed to other modules (no ABI header, no shared instance, and mod-realm-config doesn't link against content-manager). Introducing a compile-time dependency would violate "Do not turn mod-content-manager into a mandatory dependency unless it already is one" — currently it's optional (runtime detection). Adding a compile-time C++ dependency would break the "optional dependency" requirement.

Therefore the right approach: extend the existing SQL contract to also detect and read `content_manager_build_client_requirement`, honoring all the API semantics:
1. Existing build + requirements → success + requirements
2. Existing build + no requirement rows → success + empty requirements
3. Nonexistent build → failure

Key semantics to implement:
- The ACTIVE build must exist (it already is queried from `content_manager_build`).
- Requirements must be read only for the ACTIVE build that is being advertised (after realm ownership validation).
- On lookup failure → fail safely (throw, preserve existing behavior of not publishing).
- No duplicate requirements, deterministic order.

Now I need to replicate the Content Manager API semantics within mod-realm-config's own SQL:
- The existence gate is inherent: the ACTIVE build query returns the build itself. If the query for `content_manager_build` succeeded, the build exists. But there's a subtlety: after finding the ACTIVE build row, we separately query requirements. If that requirement query fails → fail publication (throw).
- Empty requirements → `Requirements=`.
- Requirements in deterministic order: `ORDER BY r.requirement` gives byte-wise order. Content Manager's storage always inserts sorted, deduplicated. But mod-realm-config should be defensive: use ORDER BY requirement and dedupe. Actually the requirement says "mod-content-manager currently stores/returns requirements in deterministic order. Do not scramble that ordering. If defensive normalization is appropriate at this boundary, it may deduplicate/sort without changing the semantic strings."

So I'll ORDER BY requirement (byte-wise) and dedupe. That's the same ordering as content manager's (it stores sorted). Fine.

Now, key question: how do we detect whether the `content_manager_build_client_requirement` table is absent due to incompatibility? If the table doesn't exist (mod-content-manager older version, pre-Schema-3), then a direct query to it would throw/last-error. We need to treat a missing table as "no requirements" (because an old content-manager build has no requirement rows, and the task says "EOFT... Existing pre-Schema-3 builds naturally have no requirement rows and therefore require nothing"). But a *query failure* must be distinguishable from "no rows". 

Let me think about how to handle this carefully:

Approach: extend the ContentContractQuery to also check for the requirement table's existence and columns. This mirrors the existing contract-detection approach.

Currently:
```sql
SELECT
    (SELECT COUNT(*) FROM information_schema.TABLES
     WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build'),
    (SELECT COUNT(*) FROM information_schema.COLUMNS
     WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build'
       AND COLUMN_NAME IN ('build_number', 'realm_name', 'filename', 'sha256', 'state'))
```
Returns 2 fields. ReadContentContract returns false if the table count is 0, throws if table exists but columns incompatible.

If mod-content-manager is not installed (~no content_manager_build), we return false → withContent=false → no synthesis, and no requirements. Good.

Now the Options:
1. Extend ContentContractQuery to detect the new requirement table. If the build table exists but the requirement table is missing → mod-content-manager is pre-Schema-3 / not upgraded. In that case, ACTIVE builds naturally have no requirement rows → requirements empty. This is consistent with "Existing pre-Schema-3 builds naturally have no requirement rows and therefore require nothing."

But wait — there's a subtlety. The content-manager upgrade `2026_09_24_05_content_manager_client_requirements.sql` creates the requirement table. If the table is missing while content_manager_build exists, it means an old (pre-upgrade) content-manager runtime. Then every ACTIVE build has no recorded requirements, so `Requirements=` is correct.

But there's risk: if the table is missing, and we want to query requirements, the query itself would fail (SQL error). So we need to gate the requirement query on the table's existence. If table absent → requirements empty (legacy behavior). If present → query; on query failure → fail publication.

However, an important semantic nuance: If the requirement table exists but its schema is incompatible (e.g., columns wrong), we must fail safe, not silently produce empty. This is consistent with the existing contract check strictness.

2. Alternatively, include the requirement rows in the big UNION metadata query. But that would complicate the record format — the requirement rows need to be per-build. Given the existing single-snapshot-UNION design, joining requirements into the same snapshot would require a separate record_type or a requested aggregation (GROUP_CONCAT). Hmm. Actually a cleaner approach consistent with the module's "one statement gives a consistent snapshot" design would be to incorporate requirement lookup in the same UNION via a separate record_type "4" row per requirement. Then the loader collects requirements belonging to a build.

But there's a challenge: The requirement rows reference build_number, and we have ACTIVE builds. We only need requirements for ACTIVE builds. Since the module refuses multiple ACTIVE builds, there's at most one build to look up. The requirement query could be joined directly to the ACTIVE build row... 

Actually wait. Let's reconsider. The current MetadataSQL UNION returns record_type '3' rows for ACTIVE builds. To fetch requirements, we need build_number from those same rows. We could add a correlated subquery in the '3' select itself... but that changes the field shape? No. Let me think.

Actually, a very clean approach that preserves the single-snapshot semantics: The '3' SELECT already returns build_number, realm_name, filename, sha256. We could add a fifth field... no wait, the '3' type uses fields[0]=type, fields[1]=sort_order, fields[2]=record_key, fields[3]..fields[6] = build_number, realm_name, filename, sha256. Fields beyond that — actually the schema is that types 0,1,2,3 use fields[3..18] differently. Type 3 only uses fields[3..6]. We have field slots f4..f18 as 4..18. Actually there are 22 fields total: 0 record_type, 1 sort_order, 2 record_key, 3..21 = f0..f18.

For type '3', only f0..f3 are used (build_number, realm_name, filename, sha256). The remaining f4..f18 are CONVERT(''...). So we have unused slots f4..f21 (18 slots) for the build rows. We could store the requirements string there... but requirements is a many-to-one mapping; a build can have multiple requirements. We could GROUP_CONCAT them, but GROUP_CONCAT ordering default is by group; not deterministic ordering unless ORDER BY inside GROUP_CONCAT. Hmm, GROUP_CONCAT with ORDER BY requirement would produce a comma-separated string in sorted order. But then mod-realm-config must parse that string — introducing a nested serialization, which the task discourages ("Do NOT introduce JSON or another nested serialization format merely for this field"). GROUP_CONCAT is arguably a nested serialization. And also, storing a semicolon etc. Also the task says prefer CSV output, but the *source* representation doesn't have to be CSV.

Hmm, but actually a separate approach better matches the existing pattern: add a new record_type '4' and emit one row per requirement. The UNION currently has record_type 3 rows for ACTIVE builds. We can add:

```
UNION ALL
SELECT CONVERT('4' USING utf8mb4) COLLATE utf8mb4_unicode_ci, 0,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT(CAST(build_number AS CHAR) USING utf8mb4) COLLATE utf8mb4_unicode_ci,  -- f0 = build_number
       CONVERT(requirement USING utf8mb4) COLLATE utf8mb4_unicode_ci,  -- f1 = requirement
       ... empty f2..f18
FROM content_manager_build_client_requirement
WHERE build_number IN (SELECT build_number FROM content_manager_build WHERE state='ACTIVE')
ORDER BY ... 
```

Wait, but the overall ORDER BY is at the end: "ORDER BY record_type, sort_order, BINARY record_key". If I add type '4' with record_key empty, then requirements sort after all record_type 3 (its record_key would be ''... no wait). Actually record_type ordering, sort_order, then record_key. Type 4 > 3 so all requirement rows come after build rows. That's fine — order within the result set doesn't matter for the loader since it collects by type. Actually mapping requirements to builds: I can store build_number in f0 and requirement in f1. Then loader associates.

But wait — is the query on `content_manager_build_client_requirement` safe when the table doesn't exist? YES! Because if the table is absent, the whole UNION query fails. The current design gates the content table inclusion via `withContent` (detected per-refresh). We'd need to extend the detection to also check the requirement table. If content_manager_build exists but the requirement table doesn't (pre-Schema-3 content manager), we must not include the requirement SELECT. So detection drives whether we append the '4' SELECT.

Now, is including the requirement-table query in the single UNION the right approach? It preserves the "single consistent InnoDB snapshot" design principle. That's attractive and follows the existing architecture. But there's a subtlety with failure semantics: if the requirement table exists but has incompatible columns, the UNION query fails → LoadMetadata throws → publication fails. Good, that fails safe.

Hmm, but there's another subtlety. The task says: "If the Content Manager API reports an error retrieving requirements for an ACTIVE build, fail safely." With a UNION-based design, a query failure naturally fails the entire snapshot → publication fails. That matches. 

But what about the case where a build has no requirement rows? Then the '4' SELECT returns no rows → no type '4' records → requirements empty → `Requirements=`. Matches semantics 2 (existing build + no requirement rows → empty).

What about nonexistent build? Not applicable, since we always filter to ACTIVE builds that exist in content_manager_build (the requirement rows only exist for builds that were recorded). A requirement row for a nonexistent build couldn't exist because content manager only inserts them after the build is recorded... actually there's no FK. But we filter `WHERE build_number IN (SELECT build_number FROM content_manager_build WHERE state='ACTIVE')`. If an orphaned requirement row exists for a non-existent build, it's ignored. Good.

Now, the ordering and dedup. With record rows, we could ORDER BY the requirement to get deterministic order. But the module dedups... do requirement rows dedupe? The table has PRIMARY KEY (build_number, requirement), so duplicates are impossible. Ordering: the requirement SELECT could include the requirement in the record_key? Hmm. Let me look at how the loader and the overall ORDER BY works. The overall query orders by record_type, sort_order, BINARY record_key. Actually for the type '4' rows, record_key is field[2]. I could put requirement in field[2] (record_key), so ordering is deterministic type→sort_order→record_key(requirement). But then the field[3] (f0) would be build_number? Wait, but for the existing types:
- type '1' (addon): record_key=addon_key, f0..f5 = name, requirement, source_type, source_url, source_ref, install_directory.
- type '2' (patch): record_key=patch_key, f0..f6 = ...
- type '3' (build): record_key='', f0..f3 = build_number, realm_name, filename, sha256.

Hmm, the loader uses fields[2] as key for type '1' and '2' and reads value(i)=fields[i+3]. For type '3', it reads `value(0..3)` which are fields[3..6]. Note field[2] (record_key) is ignored for type 3.

So for the new type '4', I can follow a similar pattern: record_key = '' (ignored), f0=build_number (to associate), f1=requirement. And to guarantee deterministic ordering of requirements, I can set sort_order=0 and rely on... hmm, the global ORDER BY is `record_type, sort_order, BINARY record_key`. If record_key is empty for all type '4' rows, the relative order of multiple requirements would depend on MySQL's tie-breaking — non-deterministic! That violates determinism.

So for deterministic ordering of requirements, options:
1. Put the requirement in record_key for type '4' rows: record_key = requirement. Then global ORDER BY `record_type, sort_order, BINARY record_key` sorts requirements byte-wise. The loader can read the requirement from fields[0]... hmm wait, but record_key is fields[2], f0 is fields[3]. I'd need to know which to read. I could set both: record_key = requirement (for ordering), and also keep f0=build_number, f1=requirement. Then the loader reads requirement from f1 always (consistently with value(i) = fields[i+3]).

Hmm, actually let me reconsider. Alternative: store build_number in f0 and requirement in both record_key and f1? That's redundant. Or simply: record_key=requirement, f0=build_number, f1=requirement. Redundant but harmless. Or use a subquery ordering trick: since sort_order is 0 for all, and we compare BINARY record_key, ordering by requirement works if record_key=requirement.

Actually wait, simpler: I can make field[2] (record_key) = requirement, and NOT duplicate into f1. The loader for type '4' reads fields[2] as requirement and fields[3] as build_number. Hmm, but that introduces a different indexing convention. Let me preserve the existing convention where possible.

Actually, let me reconsider the whole approach. Maybe simpler, more in keeping with the overly-deterministic design: the loader can sort requirements itself after loading them. The task even says defensive normalization may dedupe/sort. But the task also says mod-content-manager stores/returns in deterministic order and to not scramble it. If we sort in the loader (byte-wise), that matches content manager's order and is deterministic. Then ORDER BY in SQL doesn't matter for determinism because the loader normalizes.

Hold on. Let me reconsider whether to use the single-UNION approach at all, versus a separate query. Let me look at the reasoning in the current design:

"One statement gives a consistent snapshot of our InnoDB tables and optional ACTIVE builds."

Separate requirement query might read a different snapshot than the build row. But is that a real concern? For requirements, being immutable metadata of a build, the snapshot issue is minimal. However, the requirement storage is in the same InnoDB world database. If a build is being activated concurrently... the ACTIVE build + the requirement rows are both in the same DB. A separate query could see: build is ACTIVE (with no requirement rows yet) → advertising `Requirements=` when the build actually requires protected-framexml? No — actually requirement rows are recorded at build *creation* time (Record()) before activation, and are immutable. Content manager writes them in Record() when the build is recorded (STAGED). Activation just changes state. So a STAGED→ACTIVE transition never changes requirement rows. So a two-query approach wouldn't produce a transiently wrong requirement claim in practice.

BUT, there's a serious correctness concern with the UNION approach regarding failure semantics and the whole-snapshot atomicity: the UNION is elegant and consistent with existing architecture. Let me check whether it's feasible. Let me also check how the existing '3' type rows are filtered and whether we can do the requirement subselect correlated. Actually, we can't add a column to type '3' rows easily due to the NULL check in LoadMetadata: "for (unsigned i = 0; i < 22; ++i) if (fields[i].IsNull()) throw...". All 22 fields must be NOT NULL. Requirement rows via LEFT JOIN would produce NULL when no requirements... so the NULL check would throw. So correlated/left-join per build isn't feasible without changing the NULL policy.

Given the union approach, let me design:

record_type '4', sort_order 0, record_key = '' but we need requirement for ordering. Since the loader can sort, the SQL ordering is not critical. But to keep things clean, I'll set BINARY record_key = requirement so the global ORDER BY gives deterministic byte-wise ordering AND the loader reads requirement from... hmm.

Actually, let me look at the current loader convention again:

```cpp
auto value = [fields](unsigned i) { return fields[i + 3].Get<std::string>(); };
auto type = fields[0].Get<std::string>();
```

For type '0' singleton: value(0..18) → fields[3..21].
For type '1' addon: fields[2] = addon_key; value(0..5).
For type '2' patch: fields[2] = patch_key; value(0..6).
For type '3' build: value(0..3).

So for type '4' I'll do: fields[2] = requirement (this gives deterministic ordering via the global ORDER BY since all type '4' rows have sort_order 0 and BINARY record_key=requirement), and value(0) = build_number, value(1) = requirement (redundant?). 

Hmm, redundancy is ugly. Alternatively fields[2] = '' and sort within loader. Let me think about what's cleanest and least surprising.

Actually — a cleaner approach: put requirement in record_key (fields[2]) for ordering and rely on `value()` to read the rest. But then where's the requirement? I could read it from fields[2]. But that breaks the `value()` convention. But the loader explicitly switches on type anyway, so reading fields[2] directly for type '4' is fine and parallel to how addon/patch keys are read from fields[2].

Wait, actually there's an even cleaner option that follows the existing pattern precisely: mirror the addon/patch key approach where the sort key lives in fields[2]. For requirements, the requirement name IS the natural key and sort key:

- type '4': record_key (fields[2]) = requirement, f0 (fields[3]) = build_number.

Then loader: for each type '4' record, requirement = fields[2].Get<string>(), build_number = fields[3].Get<string>(). Then for each collected ACTIVE build (there should be exactly one or zero), group requirements.

Wait, but there's redundancy discipline: The task says avoid duplicating Content Manager's validation. Content manager validates each requirement via `ContentClientRequirement::IsSupported`. Should mod-realm-config validate requirement names? The task says "Do NOT duplicate Content Manager's requirement validation registry inside mod-realm-config. mod-realm-config's job is to publish the semantic strings associated with the authoritative build." However, we shouldn't blindly publish arbitrary strings into realm.conf either. There's a tension. The current module validates everything before publishing (e.g., SourceType must be GitHub/HTTP, requirement Required/Recommended/Optional, etc.). So publishing arbitrary requirement strings without validation would violate the module's philosophy of validating config before publishing.

I think a light boundary validation is appropriate: requirement strings must be non-empty, ASCII printable without commas, no control chars, ≤ 64 chars (matching content manager storage bound). We can even reuse ValidateText (single-line UTF-8 without control chars). But should we reject unknown requirement strings (like "foo")? The task says not to duplicate Content Manager's validation registry. Content manager's IsSupported currently only allows "protected-framexml". But the requirement table stores requirement names; future ones will be added to content-manager's registry. If mod-realm-config hardcodes "protected-framexml" only, then when content manager adds a new requirement, mod-realm-config would fail publication until updated — a coupling. But the task says "Do NOT duplicate Content Manager's requirement validation registry inside mod-realm-config."

I'll treat requirement strings as opaque validated tokens: non-empty, within the 64-char bound, no commas (since we publish CSV), no whitespace/trailing spaces, printable ASCII per the existing ValidateText rules (which already rejects control chars and surrounding spaces and validates UTF-8). Actually ValidateText allows spaces in the middle; CSV with spaces is awkward. Requirements are semantic identifiers like "protected-framexml" — definitely no commas, no spaces, no control characters. I'll add a dedicated ValidateClientRequirement that restricts to ASCII letters/digits/hyphens/underscore/dots... hmm, but that could reject a future requirement with different naming. The task says "it may deduplicate/sort without changing the semantic strings". 

I think validating with a rule like: nonempty, ≤ 64 chars (matching storage bound to avoid SQL/publish surprises), no commas (CSV safety), no control characters, no leading/trailing whitespace, single-line — i.e., essentially ValidateText plus no-commas plus length bound. That's not duplicating content manager's registry (which decides what's *supported*); it's enforcing the module's own publication contract (safe INI value). That seems right and least surprising for this codebase.

Hmm, but do I even need build_number association? Since there's at most one ACTIVE build allowed (the module throws if >1), and requirements are queried only when an ACTIVE build is being advertised, I could just collect all type '4' requirement rows and attach them to the (single) advertised build. But for robustness and clarity, associating by build_number is cleaner and future-proof. However — important: requirements must ONLY be emitted if the ACTIVE build actually gets advertised (i.e., passes realm ownership). Requirements for a foreign realm's ACTIVE build must not leak. Since we only have at most one ACTIVE build and that build either passes ownership or not, collecting requirements per-build and only using them when the build is advertised handles this correctly.

Now, there's a question: should we include the requirement table query in the same withContent-gated UNION, or make a separate detection? Let me think about the detection contract.

Current ContentContractQuery returns 2 columns:
1. count of tables named content_manager_build in current DB
2. count of columns in content_manager_build matching ('build_number','realm_name','filename','sha256','state')

Interpreting: 
- build table missing → return false → no content integration at all.
- build table present with incompatible columns → throw.
- build table present with exactly those 5 columns → return true.

Now with the requirements table, options:

Option A: Extend to return 4 columns:
1. content_manager_build table count
2. content_manager_build columns count (5)
3. content_manager_build_client_requirement table count
4. content_manager_build_client_requirement columns count (2: build_number, requirement)

Semantics:
- if build table missing entirely → no content (withContent=false) → also no requirement selection → unchanged behavior.
- if build table present:
   - if build columns != 5 → throw (incompatible contract).
   - if requirement table missing (count 0) → pre-Schema-3 content manager → ACTIVE builds have no requirement rows → requirements should be empty. In this state, is the current content_manager_build schema still compatible? Yes — same 5 columns. So we should NOT throw; we treat requirement table absence as "legacy, no requirements". 
   - if requirement table present but columns != 2 → throw (incompatible).
   - else → withContent=true and withRequirements=true.

So I'd return a struct or set two booleans.

Hold on, but there's a subtlety about fail-safe. The task says: "If the Content Manager API reports an error retrieving requirements for an ACTIVE build, fail safely." Under our design, the retrieval is the UNION query. If the requirement table exists but the query fails → LoadMetadata throws → fail. If the requirement table does not exist → we know it's a pre-Schema-3 install → no requirements → `Requirements=`. That's not an "API error"; it's a legitimate "no requirement rows anywhere" state, equivalent to content-manager's GetClientRequirements returning success+empty for every legacy build. Good.

But wait: what if the requirement table exists but has zero columns because of a truncated/incomplete schema creation? The columns count check catches that → throw. Good.

Now, is it safe to assume "requirement table missing ⇒ builds require nothing"? Consider a scenario: content-manager upgraded to Schema 3, creates requirement table, but the DBA drops the table. Then content_manager_build is present but requirement table missing → we'd assume legacy/no requirements. Could that be wrong? If builds created under Schema 3 exist with requirement rows, dropping the table would lose them. But that's an admin/DB integrity issue, not something mod-realm-config can detect via the (now missing) table. Content manager's own API `GetClientRequirements` would fail its query → error. Hmm. Under content-manager's API semantics, a *query failure* on the requirement table should be an error, whereas an *empty result with the table present* means "requires nothing". 

The difference between "table missing" and "table present with zero rows" matters for the task's strict requirement: "lookup failure cannot masquerade as no requirements". Is "table missing" a lookup failure? With content-manager at pre-Schema-3, the table never existed and there are no requirement rows for any build. Under the API, if you call GetClientRequirements on a pre-Schema-3 install where the table doesn't exist, the query would fail → error, not "empty". But actually, would content-manager even have the table? The migration creates it. Before the migration, the table doesn't exist. Old builds have no requirement rows.

Hmm, this is the crux. Let me re-read the task:

"mod-content-manager has now been upgraded with EPF Schema 3 client requirements."
"Existing pre-Schema-3 builds naturally have no requirement rows and therefore require nothing."

So the intended world state: mod-content-manager (upgraded) has the requirement table. Existing builds from before the upgrade have no requirement rows → require nothing. So in the normal upgraded deployment, the requirement table EXISTS and pre-Schema-3 builds have zero rows → `Requirements=`.

The case where the requirement table is absent entirely = mod-content-manager not yet upgraded to Schema 3. In that case there are no requirement rows at all, so every ACTIVE build requires nothing → `Requirements=` is correct. I think treating "table absent" as "no requirements" is operationally correct and consistent with the module's current dual-state model (withContent: table detected or not).

But wait — the task also explicitly says fail-safe for "API reports an error retrieving requirements". What would constitute an API error in our model?
- Query on requirement table fails (e.g., permissions, transient DB error, incompatible columns) → fail publication.
- Table present but incompatible columns → contract check throws → fail.

The "table absent" case: I'll treat it as legitimate legacy state, NOT an error, because an un-upgraded content-manager cannot have requirements. This matches "When there is no Content Manager content build being advertised, there are no build-derived client requirements to advertise" and the general philosophy that absence is benign.

Hmm, but let me reconsider — there's a risk. What if the mod-content-manager is upgraded AND a new build was created with requirements, but then the requirement table is dropped? The ACTIVE build record still exists with the same 5-column schema. We'd advertise `Requirements=` falsely. But dropping a table that content-manager actively uses would break content-manager itself. This is an admin-corruption scenario. The existing module already has similar trust: e.g., if the `content_base_url` column migration wasn't applied, the MetadataQuery would fail and the module would fail safe (the module queries that column). 

Actually wait! Let me reconsider. There's an important asymmetry. In the CURRENT module, the MetadataQuery references content_base_url unconditionally (a column that must exist). If the migration wasn't applied → query fails → module fails safe. Similarly, if we reference the requirement table unconditionally → old content-manager installs would break. So we must gate the requirement table query on detection, which means: table absent → no requirements. That's the only way to remain backward compatible with pre-Schema-3 content-manager installs.

OK here's another thought — what if instead of gating on detection, we make the requirement SELECT robust: only include it when the table is detected. That's exactly the withContent pattern. But we need a second boolean. Let me design:

```cpp
struct ContentContract { bool content; bool requirements; };
```

Detection query returns:
1. build table count (0/1)
2. build columns count (expected 5)
3. requirement table count (0/1)
4. requirement columns count (expected 2)

Logic in ReadContentContract:
- if fields null → throw
- if fields[0]==0 → { false, false } (no content manager)
- if fields[0]!=1 → throw (multiple content_manager_build tables? weird) — existing code: `fields[0].Get<uint64>() != 1` throws.
- if fields[1]!=5 → throw incompatible build contract
- if fields[2]==0 → { true, false } (content manager present, legacy/no requirement table → no requirements)
- if fields[2]!=1 → throw
- if fields[3]!=2 → throw incompatible requirement contract
- return { true, true }

Hmm wait, actually existing code already throws when fields[0] != 1. Let me keep that.

Now MetadataSQL(withContent, withRequirements) — appends '3' build rows if withContent, and '4' requirement rows if withRequirements.

Then LoadMetadata collects requirements keyed by build_number.

Then SynthesizeRealmContent: after validating the ACTIVE build belongs to the realm, attach requirements to the settings (settings.ClientRequirements) derived only from the ACTIVE build.

Then BuildConfiguration emits Requirements= in the [Client] section. With no requirements → `Requirements=` (empty). Per the task: "For no requirements, prefer: Requirements= rather than omitting the key, IF that is consistent with the generator's existing deterministic-output conventions."

The generator always emits all [Client] keys unconditionally (Version, Build, Executable, ExecutableSHA256, RuntimeMode). Even empty values like ExecutableSHA256=, and in [Services] empty URLs like ManifestURL=. So emitting `Requirements=` (empty) is consistent! Great. The generator emits every field unconditionally, so `Requirements=` always emitted as last Client key.

Wait — should `Requirements=` come before or after `RuntimeMode`? The task's conceptual example shows:

```
[Client]
Version=3.3.5a
Build=12340
Executable=Wow.exe
ExecutableSHA256=...
RuntimeMode=Isolated
Requirements=protected-framexml
```

So append after RuntimeMode. Note task's example has ExecutableSHA256=... but that's a placeholder. Current generator emits RuntimeMode last in [Client]. Appending Requirements after RuntimeMode matches the conceptual example. Also: "Existing [Client] keys are unchanged" and "Existing Addons[] and Patches[] output remains unchanged except for the intended new Requirements line." So append after RuntimeMode.

Now, the multi-requirement representation: CSV `requirement-a,requirement-b`. Order: content manager stores sorted byte-wise, and we preserve that order (sort/0 dedupe). The loader should sort (byte-wise, since utf8mb4_bin collation stores byte ordered... but BINARY record_key ordering is byte-wise). For safety, I'll sort in the loader using std::set or std::sort on std::string (byte-wise for ASCII; for non-ASCII, std::string operator< is byte-wise too). Dedupe: use a set or unique after sort. The task allows dedupe/sort defensively.

Now, where to store them in Settings? Add `std::vector<std::string> ClientRequirements;` to Settings. Or a per-build mapping. Since at most one ACTIVE build is advertised, storing a single vector is fine, but to keep the "requirements belong to the build" association clean I could store them on the ActiveContentBuild struct. But ActiveContentBuild is a simple struct used in the vector. Let me add `std::vector<std::string> ClientRequirements;` to ActiveContentBuild. Hmm, but LoadMetadata collects builds first, then requirements. Association by build_number.

Actually, let me think about the association more. The loader reads all rows. For each type '4' row, parse build_number (from fields[3]) and requirement (from fields[2] - record_key). Store into a map<buildNumber, set<requirement>>, then apply to matching ActiveContentBuild after all rows read. But we need to be careful: only apply requirements to builds that exist as type '3' rows. A requirement row referencing a non-ACTIVE or nonexistent build should... hmm. Since we filter `WHERE build_number IN (SELECT build_number FROM content_manager_build WHERE BINARY state='ACTIVE')`, requirements only come for ACTIVE builds. Good. But then, LoadMetadata's association: find the ActiveBuild with matching number and assign. If none exists (shouldn't happen given the WHERE), ignore or throw? I'll ignore orphan rows defensively? No — better throw? The WHERE guarantees a match. Let me have it fail if a requirement references a build not collected... Actually a defensive approach: only attach if a build matches. But to be safe and deterministic, I'll attach by matching build_number among collected ACTIVE builds; if no match, ignore the row (it can't influence anything). Hmm, but "ignore" could silently drop. But given the SQL WHERE IN ACTIVE build, there will be a match if the ACTIVE build is present. If somehow the ACTIVE build row was empty (impossible given WHERE), ignore. I'll keep ignore-with-runtime-throw unlikely... Let me just match; if no match, throw to be consistent with the module's fail-closed philosophy? 

Hmm. Wait, there's a subtle issue. Requirements are only for ACTIVE builds. But LoadMetadata runs even when content_base_url is unset or the build belongs to another realm. In those cases, the requirements were still fetched (rows exist in result), parsed, and attached to the build object — but never published because SynthesizeRealmContent decides whether to advertise. The load-parsing should still succeed (not throw) for foreign-realm ACTIVE builds, so that the realm-mismatch log path ("not advertised") works as before. So requirement parsing in LoadMetadata must not throw for foreign-realm builds; it's SynthesizeRealmContent that decides not to advertise. Good — parsing just populates data; publication decision happens later.

Now the key safety property: "another realm's requirements cannot be advertised". In SynthesizeRealmContent, if `build.RealmName != canonicalRealmName`, return early → requirements for that build never reach settings.ClientRequirements. So publication only copies requirements from the single ACTIVE build that passed ownership. 

Let me now reconsider: where exactly to attach requirements so the check ordering is right. In SynthesizeRealmContent:
- if !contentAvailable → return "unavailable"
- if ActiveBuilds.size() > 1 → throw
- if empty → return "available; no ACTIVE ..."
- if ContentBaseURL empty → return "available; content_base_url is unset"
- build = ActiveBuilds.front()
- validate realm name
- if build.RealmName != canonicalRealmName → return "different realm; not advertised"
- ... validate build number, filename, URL ...
- push realm-content patch
- return "available; ACTIVE realm content build N"

Where does Requirements fit? The requirement is about the ACTIVE build's client requirements — independent of content_base_url being set? Hmm. The task says: "When mod-realm-config publishes a content build belonging to the current realm, publish that ACTIVE build's stored client requirements in the [Client] section of realm.conf."

Wait, re-read: "When mod-realm-config publishes a content build belonging to the current realm, publish that ACTIVE build's stored client requirements in the [Client] section." So Requirements are published only when a content build is actually being advertised (the realm-content patch is being synthesized). This means: if content_base_url is unset (no URL to advertise), then... hmm, is the build "published"? No synthetic patch is emitted, so arguably no content build is being advertised. In that case, should Requirements appear?

The task's optional-dependency section: "When there is no Content Manager content build being advertised, there are no build-derived client requirements to advertise. Choose the representation consistent with the existing realm.conf generation behavior, likely: Requirements=

Hmm wait — that's interesting. "consistent with the existing realm.conf generation behavior". If the generator always emits all keys, then even with no content manager, `Requirements=` (empty) would appear. But if requirements are only emitted when a build is advertised... Let me re-read.

"Preserve the current behavior when mod-content-manager is not installed or unavailable." — mod-realm-config must work without mod-content-manager. So `Requirements=` empty when no content manager.

"When there is no Content Manager content build being advertised, there are no build-derived client requirements to advertise." — So when not advertising a build → `Requirements=` empty.

So the representation: always emit `Requirements=<value>` in [Client], where <value> is:
- the ACTIVE build's requirements, if the build is being advertised (passes ownership, content_base_url set, etc.)
- empty otherwise.

And per determinism test "no Content Manager integration: realm.conf still generates successfully" with `Requirements=`.

Now the ORDER of operations in SynthesizeRealmContent. Currently it synthesizes the patch. Where do we decide requirements? I think settings.ClientRequirements should be set inside SynthesizeRealmContent right where we decide to advertise the build — after ownership validation passes but... hmm, should it be gated on ContentBaseURL being set? Let me think about what "publish a content build" means. The build's artifact URL is ContentBaseURL + filename. If ContentBaseURL is unset, the module doesn't advertise the build (no patch). Is the build still "published"? No. So Requirements should probably be empty then. But actually — would it be reasonable to publish Requirements even without the artifact? Conceptually the ACTIVE build still requires protected-framexml even if we weren't advertising its URL... But the task ties requirements to "the ACTIVE content build advertised by the realm" (README doc text: "A comma-separated list of semantic client capabilities required by the ACTIVE content build advertised by the realm."). If no build is advertised, no requirements.

I'll gate both requirements and patch synthesis together: after ownership passes, if ContentBaseURL set → advertise patch AND set requirements. If ContentBaseURL unset → neither. Hmm, but wait — currently the check `if (settings.ContentBaseURL.empty()) return "available; content_base_url is unset"` happens BEFORE ownership check? Let me re-read the current code:

```
if (!contentAvailable) return "unavailable";
if (settings.ActiveBuilds.size() > 1) throw;
if (settings.ActiveBuilds.empty()) return "...no ACTIVE...";
if (settings.ContentBaseURL.empty()) return "available; content_base_url is unset";
auto const& build = settings.ActiveBuilds.front();
ValidateText(canonicalRealmName...);
ValidateText(build.RealmName...);
if (build.RealmName != canonicalRealmName) return "...different realm; not advertised";
...
```

Hmm, the ContentBaseURL check comes before ownership. Is that right? If content_base_url is unset, we return early regardless of ownership. OK. That means requirements would also be empty in that case. Fine and consistent.

Actually wait, but should requirements be published even when the URL is unset? Let me reconsider by the task's tests:

"6. Multiple returned requirements: deterministic comma-separated output." — requires requirements from an advertised build.
"9. ACTIVE build belonging to another realm: its patch is not advertised, its requirements are not advertised."

Test 9 confirms requirements are NOT advertised when patch isn't advertised. And by the same logic, if content_base_url is unset (no patch advertised), requirements not advertised. Consistent.

But hold on — maybe the intent is that Requirements is published whenever the ACTIVE build is owned by the realm, even independent of content_base_url? The task says "The ACTIVE generated build is authoritative." and "publish that ACTIVE build's stored client requirements in the [Client] section". And the docs "ADVERTISED by the realm". Since the build is advertised only via the patch (which requires content_base_url), whenever the patch is emitted, requirements are emitted. When it's not, they're empty.

Hmm, but I realize there's an edge: content_base_url unset + build owned by realm + build requires something → currently returns "available; content_base_url is unset". Should Requirements reflect the build? I lean toward NOT publishing requirements because no build is being advertised. This keeps consistency between "patch emitted" and "requirements emitted", and keeps the doc statement true. I'll document this choice.

Actually, wait. Let me reconsider once more. There's a subtle but important point: the doc must say "required by the ACTIVE content build advertised by the realm". If someone sets content_base_url later, the build gets advertised and requirements appear. This is consistent and predictable. Good.

Now let's think about the placement of the ownership: the realm-mismatch log message and early return. If I set requirements after the ownership check and after the content_base_url check and validation, then:

```cpp
auto number = build.Number;
ValidateNumber(number, ...);
ValidateRelativePath(build.FileName, ...);
if (build.FileName.find_first_not_of(...) != npos) throw;
ValidateURL(settings.ContentBaseURL, ...);
if (settings.ContentBaseURL.find_first_of("?#") != npos) throw;
for (patches...) if real-content reserved → throw;

// NEW: publish the ACTIVE build's client requirements
settings.ClientRequirements = build.ClientRequirements;  // already normalized
```

But `settings` is the whole settings struct — I'd copy the vector. Where does build.ClientRequirements come from? LoadMetadata populates ActiveContentBuild.ClientRequirements after parsing. So SynthesizeRealmContent copies from build into settings.ClientRequirements (or, since build is the only ACTIVE, directly set settings.ClientRequirements = ActiveBuilds.front().ClientRequirements).

Wait — subtlety: settings.ClientRequirements must be EMPTY unless the build is published. Since SynthesizeRealmContent returns early on all non-advertised paths without setting ClientRequirements, it stays empty (Settings default). Then BuildConfiguration emits `Requirements=` (join of empty → empty string). 

Now BuildConfiguration:
```cpp
AppendSection(output, "Client", {{"Version", settings.ClientVersion}, {"Build", settings.ClientBuild},
    {"Executable", settings.Executable}, {"ExecutableSHA256", settings.ExecutableSHA256},
    {"RuntimeMode", settings.ClientRuntimeMode}, {"Requirements", JoinClientRequirements(settings)}});
```

JoinClientRequirements produces CSV with no trailing comma. Empty → "". I need to be careful: if requirements is empty, output "". Deterministic.

Now let me also think about the "fail safely on lookup failure" requirement more concretely. In the UNION design, requirement lookup is part of the single snapshot query. A failure (query error) → AsyncQuery result is empty/null → AcceptResult → LoadMetadata throws ("query failed") → ReportFailure → last known-good file preserved. Good. That matches "Incomplete or unverifiable ACTIVE build must not be advertised." 

But wait — there's a scenario: what if the requirement table exists with correct columns, but the build has MANY requirement rows including one that fails validation (e.g., a string >64 bytes or with a comma or control char)? LoadMetadata's validation should throw → fail safe. Good.

Now — an important design question remains: the requirement rows association + dedup/sort. Let me use a map from build_number → set. Since we sort in the loader, I don't strictly need the SQL ORDER BY to be the requirement. But I'll still set record_key = requirement and the global ORDER BY handles it; the loader will ALSO create a sorted deduplicated set (defensive normalization allowed). Actually, since the loader assigns to a `std::set<std::string>` first, then copies to vector, order is guaranteed byte-wise. Then no dependence on SQL ordering. But I'll still keep record_key=requirement for readable/deterministic SQL output ordering, aligning with content-manager's ORDER BY r.requirement.

Hold on, wait. Let me reconsider whether to use the single-UNION approach vs a separate detection+query for requirements. The existing architecture has TWO queries:
1. ContentContractQuery (information_schema detection) — runs each interval first.
2. MetadataSQL(withContent) — the single UNION snapshot.

If I fold requirements into the UNION, the semantics for "table absent" require a two-state detection: withRequirements=false when table absent. That's the ContentContract approach extended. This is clean and follows the architecture.

If instead I did a separate query for requirements after LoadMetadata, I'd need to handle "table absent" specially too, plus it'd be a second query breaking the single-snapshot invariant, plus more async-callback plumbing. The UNION approach is better.

So, decision: extend ContentContractQuery to 4 columns, extend ReadContentContract to return both booleans, extend MetadataSQL to include a '4' requirement row type when requirements are available, extend the plumbing (OnUpdate snapshot state, ReloadSettings), extend LoadMetadata to parse requirements and associate with builds, add normalization (set), add validation, copy into settings in SynthesizeRealmContent, emit in BuildConfiguration, update README/SCHEMA/docs, and add tests.

Now the tests question. The README references "bash tests/run.sh" but there's NO tests directory in the repo! This is a problem. The task says: "Add focused tests using the repository's existing test approach." and "Run the repository's relevant automated tests."

Hmm wait, let me double-check there's no tests directory somewhere. Earlier `git ls-tree` showed no tests directory, and `find /home/smithkt/git/mod-realm-config/data ...tests...` found nothing. And `ls` showed: .clang-format .git .gitignore .project CMakeLists.txt LICENSE README.md SCHEMA.md conf data examples icon.png src. No tests dir.

So the README's "Focused tests" section referencing tests/run.sh is aspirational/documentation that doesn't match the repo. Interesting — maybe the tests existed in an uncommitted state or were lost. Let me check the git history for any tests directory ever.

Let me check the repository history for a tests directory and look at the full content of key files.

**Tool: bash**

**Input:**
```json
{
  "command": "git -C /home/smithkt/git/mod-realm-config log --all --oneline --name-status | grep -i test | head -20; echo \"==== all refs ====\"; git -C /home/smithkt/git/mod-realm-config rev-list --all --objects | grep -i test | head; echo \"==== tags ====\"; git -C /home/smithkt/git/mod-realm-config tag"
}
```

**Output:**
```
==== all refs ====
==== tags ====

```

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/examples/catalog.sql"
}
```

**Output:**
```
<path>/home/smithkt/git/mod-realm-config/examples/catalog.sql</path>
<type>file</type>
<content>
1: -- Illustrative public data only. NOT an automatic migration.
2: -- Apply to a Schema v1 world database only if you want this example catalog.
3: -- Example download URLs are placeholders, not endorsed or verified resources.
4: UPDATE `mod_realm_config`
5: SET `realm_key` = 'example', `name` = 'Example Realm', `address` = 'realm.example.com',
6:     `description` = '', `website_url` = '',
7:     `client_version` = '3.3.5a', `client_build` = 12340,
8:     `client_executable` = 'Wow.exe', `client_executable_sha256` = '',
9:     `auth_port` = 3724, `world_port` = 8085,
10:     `portalkeeper_minimum_version` = '0.1.0',
11:     `manifest_url` = '', `news_url` = '', `status_url` = '',
12:     `calendar_url` = '', `armory_url` = '',
13:     `config_url` = 'https://example.com/realm.conf'
14: WHERE `id` = 1;
15: 
16: INSERT INTO `mod_realm_config_addon`
17:   (`addon_key`, `name`, `requirement`, `source_type`, `source_url`,
18:    `source_ref`, `install_directory`, `sort_order`, `enabled`)
19: VALUES
20:   ('ExampleAddon', 'Example Addon', 'Required', 'GitHub',
21:    'https://github.com/example/ExampleAddon', 'main', 'ExampleAddon', 10, 1);
22: 
23: INSERT INTO `mod_realm_config_patch`
24:   (`patch_key`, `name`, `requirement`, `source_type`, `source_url`,
25:    `file_name`, `install_directory`, `sha256`, `sort_order`, `enabled`)
26: VALUES
27:   ('ExamplePatch', 'Example Realm Patch', 'Required', 'HTTP',
28:    'https://example.com/patch-X.MPQ', 'patch-X.MPQ', 'Data', '', 10, 1);

(End of file - total 28 lines)
</content>
```

---

## Assistant (Build · Big Pickle · 26.6s)

_Thinking:_

There's no test infrastructure in this repository currently. The README mentions `bash tests/run.sh` but it doesn't exist. This means:
1. There's no test framework in this repo.
2. I need to create the tests from scratch following "the repository's existing test approach" — which the README describes: standalone C++ test suite with "small AzerothCore API doubles" using C++17 g++ and Python 3 for SQL snapshot tests.

Wait — but without any existing tests, "the repository's existing test approach" is essentially documented in the README but not present. Since the task explicitly requires adding focused tests, I need to create a test setup. But I need to be careful: "Do NOT redesign unrelated systems." The task says add focused tests using the repository's existing test approach, and then run them and run regression tests.

Hmm. This is a tension. There are no tests to run for regression. The README documents a test approach that doesn't exist in the repo (maybe lost, or maybe intentionally described for a future state). 

Let me consider: maybe the tests exist in mod-content-manager or in another location? The README for mod-realm-config says "Run bash tests/run.sh". Let me double-check with a broader find, maybe in a parent directory or an AzerothCore tree.

There's no test infrastructure in this repo despite the README referencing it. Let me search more broadly for any test setup associated with the module.

**Tool: bash**

**Input:**
```json
{
  "command": "find /home/smithkt/git/mod-realm-config -name \"*.py\" -o -name \"*.sh\" 2>/dev/null | grep -v \"\\.git/\"; echo \"== broader ==\"; ls -la /home/smithkt/git/mod-content-manager/tests/"
}
```

**Output:**
```
== broader ==
total 200
drwxrwxr-x 3 smithkt smithkt  4096 Sep 24 12:06 .
drwxrwxr-x 9 smithkt smithkt  4096 Sep 24 12:37 ..
-rw-rw-r-- 1 smithkt smithkt 10323 Sep 24 12:35 PHASE4_TESTS.md
-rw-rw-r-- 1 smithkt smithkt  7491 Sep 23 06:39 PHASE5_TESTS.md
-rw-rw-r-- 1 smithkt smithkt  4625 Sep 23 06:39 baseline_mysql_tests.cpp
-rw-rw-r-- 1 smithkt smithkt  3398 Sep 23 06:39 category_tests.cpp
-rw-rw-r-- 1 smithkt smithkt 11547 Sep 24 12:32 client_requirement_mysql_tests.cpp
-rw-rw-r-- 1 smithkt smithkt 11676 Sep 23 06:39 currency_build_mysql_tests.cpp
-rw-rw-r-- 1 smithkt smithkt  8198 Sep 23 06:39 currency_mysql_tests.cpp
-rw-rw-r-- 1 smithkt smithkt  5301 Sep 23 06:39 currency_tests.cpp
-rw-rw-r-- 1 smithkt smithkt  4373 Sep 23 06:39 dbc_reader_tests.cpp
-rw-rw-r-- 1 smithkt smithkt 17584 Sep 23 06:39 extended_cost_mysql_tests.cpp
-rw-rw-r-- 1 smithkt smithkt  9413 Sep 23 06:39 extended_cost_tests.cpp
drwxrwxr-x 2 smithkt smithkt  4096 Sep 23 06:39 mysql_adapter
-rw-rw-r-- 1 smithkt smithkt  1786 Sep 23 06:39 occupancy_tests.cpp
-rw-rw-r-- 1 smithkt smithkt  9415 Sep 23 07:41 package_lifecycle_mysql_tests.cpp
-rw-rw-r-- 1 smithkt smithkt  5159 Sep 23 07:04 package_lifecycle_tests.cpp
-rw-rw-r-- 1 smithkt smithkt  4665 Sep 23 06:39 phase2_tests.cpp
-rw-rw-r-- 1 smithkt smithkt  2751 Sep 24 12:02 run_phase4.py
-rw-rw-r-- 1 smithkt smithkt  7062 Sep 23 06:39 schema2_tests.cpp
-rw-rw-r-- 1 smithkt smithkt  6830 Sep 24 12:02 schema3_tests.cpp
-rw-rw-r-- 1 smithkt smithkt  5957 Sep 23 06:39 server_bundle_tests.cpp
-rw-rw-r-- 1 smithkt smithkt  9346 Sep 23 06:39 vendor_mysql_tests.cpp

```

**Tool: bash**

**Input:**
```json
{
  "command": "find /home/smithkt -maxdepth 3 -type d -name \"realm-config-tests\" 2>/dev/null; find / -maxdepth 4 -type d -name \"run.sh\" -path \"*realm*\" 2>/dev/null; ls /mnt/ai_data 2>/dev/null | head"
}
```

**Output:**
```
(no output)
```

---

## Assistant (Build · Big Pickle · 37.3s)

_Thinking:_

So mod-realm-config has no test infrastructure at all, despite the README mentioning "bash tests/run.sh". The tests directory simply doesn't exist in this repo. This is important context — I'll need to create a test approach.

The README describes the intended test approach:
- `bash tests/run.sh` with C++17 `g++` (or `$CXX`) and Python 3
- standalone suite uses small AzerothCore API doubles to test the production metadata parser, validator, serializer, refresh callbacks, and publisher
- SQL snapshot tests adapt MySQL text casts to SQLite

This described approach matches what mod-content-manager's tests do. Let me look at mod-content-manager's test approach to understand the conventions, especially how they test with API doubles. Let me look at schema3_tests.cpp and client_requirement_mysql_tests.cpp, and run_phase4.py to understand the test runner approach.

Actually, wait. Since mod-realm-config has NO test infra and the task says "Add focused tests using the repository's existing test approach", I have a problem: there is no existing test approach in the repository. The README documents one that isn't present.

Let me reconsider — maybe the tests are meant to have existed. Since I can't find them anywhere, I'll need to create them. But creating a full C++-double test harness is significant. Let me review what the README says and what would be a minimal, focused approach.

Given the constraints:
- "Add focused tests using the repository's existing test approach."
- "At minimum verify" the 14 scenarios.
- "Run the repository's relevant automated tests. Run regression tests."

Since there are no existing tests, I'll need to build a small test harness. The README explicitly documents: "The standalone suite uses small AzerothCore API doubles to test the production metadata parser, validator, serializer, refresh callbacks, and publisher. SQL snapshot tests adapt MySQL text casts to SQLite."

But implementing API doubles for the full WorldScript + DB env is a big undertaking. Let me think about a more focused, feasible approach that still gives real coverage of the production code.

Option: Create a test that compiles the production cpp with stub implementations of the AzerothCore APIs (Config.h, DatabaseEnv.h, Field.h, QueryResult.h, Log.h, ScriptMgr.h, World.h, Realm.h) — the "API doubles" approach. Then test:
- BuildConfiguration output (the serializer): this is the core — it's a pure function of Settings. We can test the [Client] Requirements line: legacy, isolated, no requirements, single requirement, multiple, dedup, etc.
- The requirement normalization: sort/dedup function.
- The deterministic output.

- For the ACTIVE build ownership + requirement gating, that's in SynthesizeRealmContent which is a pure C++ function too (no DB access — it takes Settings + contentAvailable + realmName). We can test:
  - foreign-realm ACTIVE build → not advertised → no requirements in settings, and no realm-content patch.
  - owned ACTIVE build → requirements copied.
  - lookup failure → that's a DB-level concern; the module's contract detection covers table incompatibility → throws. We can test the contract reading logic and the metadata parsing.

- The LoadMetadata parser is also testable: feed it mock QueryResult rows representing the UNION output including type '4' requirement rows, and verify association and normalization. But that requires stubbing QueryResult/Field. Doable if we control the doubles.

This is substantial but matches the documented approach. However, time/complexity is a concern. Let me look at how mod-content-manager structures its tests and doubles to mirror conventions.

Actually, let me reconsider the scope. The key deliverables that MUST be testable per the task:
1. existing realm.conf generation valid
2. existing [Client] keys unchanged
3. no content manager → generates successfully (with Requirements=)
4. legacy build zero rows → Requirements=
5. schema-3 build protected-framexml → Requirements=protected-framexml
6. multiple requirements → deterministic CSV
7. no duplicate output
8. requirements from ACTIVE build, not package state (structural — tested via wiring)
9. foreign realm → patch not advertised + requirements not advertised
10. lookup failure → NOT Requirements=; publication fails safely
11. RuntimeMode=Legacy unchanged
12. RuntimeMode=Isolated unchanged
13. Addons[]/Patches[] unchanged except Requirements line
14. regression tests continue to pass (no existing tests)

Given no existing test infra, I'll create a `tests/` directory with a run.sh and a C++ test file that includes the production source with API doubles, plus possibly a Python/SQLite snapshot test. But I must be careful: including the production .cpp and stubbing its includes requires providing definitions matching AzerothCore APIs. Let me check what the production file actually uses from each header:
- Config.h: sConfigMgr->GetOption<bool/String>.
- DatabaseEnv.h: WorldDatabase, LoginDatabase (global objects with AsyncQuery, Query, BeginTransaction, DirectCommitTransaction...). Used: WorldDatabase.Query, WorldDatabase.AsyncQuery(...).WithCallback(...), LoginDatabase.Query.
- Field.h: Field::IsNull(), Get<T>.
- QueryResult.h: QueryResult (nullable smart pointer), result->GetFieldCount(), result->Fetch(), result->NextRow().
- Log.h: LOG_ERROR, LOG_WARN, LOG_INFO macros.
- ScriptMgr.h: WorldScript base class.
- World.h: realm global object (realm.Name, realm.Id.Realm), actually `realm` is a global `WorldRealm`? Let me check what `realm` is in AzerothCore. In AzerothCore, `Realm realm` global in Realm.h. realm.Name and realm.Id.Realm (RealmID).
- Realm.h: global `Realm realm`.

Also uses std::filesystem, etc. And WorldDatabase fields.

This is a significant harness to stub. But it matches the documented approach ("small AzerothCore API doubles"). Let me check mod-content-manager's test files to see how they stub these — maybe they have a shared doubles header.

The repo has no test infrastructure (README references `tests/run.sh` that doesn't exist). Let me examine how mod-content-manager structures its tests and API doubles to mirror conventions.

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-content-manager/tests/schema3_tests.cpp"
}
```

**Output:**
```
<path>/home/smithkt/git/mod-content-manager/tests/schema3_tests.cpp</path>
<type>file</type>
<content>
1: #include "ContentPackage.h"
2: #include "ContentClientRequirement.h"
3: #include "third_party/json/json.hpp"
4: #include "third_party/miniz/miniz.h"
5: #include <cassert>
6: #include <filesystem>
7: #include <stdexcept>
8: #include <string>
9: #include <vector>
10: 
11: using json = nlohmann::json;
12: 
13: namespace
14: {
15: void Save(std::filesystem::path const& path, json const& manifest, bool raw = false)
16: {
17:     mz_zip_archive zip{};
18:     if (!mz_zip_writer_init_file(&zip, path.string().c_str(), 0)) throw std::runtime_error("zip init failed");
19:     auto body = manifest.dump();
20:     if (!mz_zip_writer_add_mem(&zip, "manifest.json", body.data(), body.size(), MZ_BEST_COMPRESSION)
21:         || (raw && !mz_zip_writer_add_mem(&zip, "assets/test.txt", "ok", 2, MZ_BEST_COMPRESSION))
22:         || !mz_zip_writer_finalize_archive(&zip))
23:         throw std::runtime_error("zip write failed");
24:     mz_zip_writer_end(&zip);
25: }
26: }
27: 
28: int main()
29: {
30:     using ContentClientRequirement::ProtectedFrameXml;
31:     auto path = std::filesystem::temp_directory_path() / "content-schema3-test.epf";
32:     auto work = std::filesystem::temp_directory_path() / "content-schema3-work";
33: 
34:     // 1. Existing schema-1 still parses/validates and declares no requirements.
35:     json schema1 = {{"schema", 1}, {"package", "raw-a"}, {"name", "Raw A"},
36:         {"version", "1"}, {"content", json::array({{{"type", "file"},
37:             {"source", "assets/test.txt"}, {"target", "Documentation/a.txt"}}})}};
38:     Save(path, schema1, true);
39:     {
40:         auto validation = ContentPackage(path).Validate();
41:         assert(validation.valid);
42:         assert(validation.manifest.schema == 1);
43:         assert(validation.manifest.clientRequirements.empty());
44:     }
45: 
46:     // 2. Existing schema-2 still parses/validates and declares no requirements.
47:     json schema2 = {{"schema", 2}, {"package", "raw-b"}, {"name", "Raw B"},
48:         {"version", "1"}, {"content", json::array({{{"type", "file"},
49:             {"source", "assets/test.txt"}, {"target", "Documentation/b.txt"}}})}};
50:     Save(path, schema2, true);
51:     {
52:         auto validation = ContentPackage(path).Validate();
53:         assert(validation.valid);
54:         assert(validation.manifest.schema == 2);
55:         assert(validation.manifest.clientRequirements.empty());
56:     }
57: 
58:     // 3. Schema 3 without clientRequirements is valid and produces none.
59:     json schema3 = {{"schema", 3}, {"package", "raw-c"}, {"name", "Raw C"},
60:         {"version", "1"}, {"content", json::array({{{"type", "file"},
61:             {"source", "assets/test.txt"}, {"target", "Documentation/c.txt"}}})}};
62:     Save(path, schema3, true);
63:     {
64:         auto validation = ContentPackage(path).Validate();
65:         assert(validation.valid);
66:         assert(validation.manifest.schema == 3);
67:         assert(validation.manifest.clientRequirements.empty());
68:     }
69: 
70:     // 4. Empty clientRequirements array is valid and produces none.
71:     auto empty = schema3;
72:     empty["clientRequirements"] = json::array();
73:     Save(path, empty, true);
74:     {
75:         auto validation = ContentPackage(path).Validate();
76:         assert(validation.valid);
77:         assert(validation.manifest.clientRequirements.empty());
78:     }
79: 
80:     // 5. A supported requirement is valid and lands in the manifest set.
81:     auto required = schema3;
82:     required["clientRequirements"] = json::array({ProtectedFrameXml});
83:     Save(path, required, true);
84:     {
85:         auto validation = ContentPackage(path).Validate();
86:         assert(validation.valid);
87:         auto const& set = validation.manifest.clientRequirements;
88:         assert(set.size() == 1 && set[0] == ProtectedFrameXml);
89:     }
90: 
91:     // 6. Unknown requirement names fail with a useful error.
92:     auto unsupported = schema3;
93:     unsupported["clientRequirements"] = json::array({"some-future-thing"});
94:     Save(path, unsupported, true);
95:     {
96:         auto validation = ContentPackage(path).Validate();
97:         assert(!validation.valid);
98:         assert(validation.error.find("Unsupported client requirement 'some-future-thing'") != std::string::npos);
99:         assert(validation.error.find(ProtectedFrameXml) != std::string::npos);
100:     }
101: 
102:     // 7. Duplicate declarations do not produce duplicate build requirements.
103:     auto duplicate = schema3;
104:     duplicate["clientRequirements"] = json::array({ProtectedFrameXml, ProtectedFrameXml});
105:     Save(path, duplicate, true);
106:     {
107:         auto validation = ContentPackage(path).Validate();
108:         assert(validation.valid);
109:         auto const& set = validation.manifest.clientRequirements;
110:         assert(set.size() == 1 && set[0] == ProtectedFrameXml);
111:     }
112: 
113:     // clientRequirements belongs to schema 3: older schemas must not silently carry it.
114:     for (int schema : {1, 2})
115:     {
116:         auto older = schema == 1 ? schema1 : schema2;
117:         older["clientRequirements"] = json::array({ProtectedFrameXml});
118:         Save(path, older, true);
119:         auto validation = ContentPackage(path).Validate();
120:         assert(!validation.valid);
121:         assert(validation.error.find("clientRequirements require Schema 3") != std::string::npos);
122:     }
123: 
124:     // Malformed clientRequirements forms are rejected.
125:     for (auto const& malformed : std::vector<json>{"protected-framexml", json::array({1}),
126:         json::array({true}), json::array({ProtectedFrameXml, 2})})
127:     {
128:         auto bad = schema3;
129:         bad["clientRequirements"] = malformed;
130:         Save(path, bad, true);
131:         assert(!ContentPackage(path).Validate().valid);
132:     }
133:     {
134:         auto merged = ContentClientRequirement::Merge({{}, {ProtectedFrameXml}, {ProtectedFrameXml}});
135:         assert(merged.size() == 1 && merged[0] == ProtectedFrameXml);
136:         // Aggregate-init duplication in every participating set still yields one entry.
137:         std::vector<std::vector<std::string>> sets;
138:         for (int i = 0; i < 3; ++i) sets.push_back({ProtectedFrameXml, ProtectedFrameXml});
139:         auto stable = ContentClientRequirement::Merge(sets);
140:         assert(stable.size() == 1 && stable[0] == ProtectedFrameXml);
141:         // Requirement comparison is byte-wise; the merged set is sorted.
142:         assert(ContentClientRequirement::ValidSet(stable));
143:         // Ordering is deterministic byte-wise, independent of input order.
144:         auto ordered = ContentClientRequirement::Merge({{"banana", "zesty"}, {}, {"apple", "banana"}});
145:         assert(ordered == (std::vector<std::string>{"apple", "banana", "zesty"}));
146:     }
147: 
148:     // Schema-3 packages stage and build exactly like earlier schemas: metadata
149:     // must not alter the produced files.
150:     Save(path, required, true);
151:     {
152:         auto staged = ContentPackage(path).Stage(work);
153:         assert(staged.success);
154:         assert(staged.stagedFiles.size() == 1);
155:         assert(std::filesystem::is_regular_file(staged.stagedFiles.front()));
156:     }
157: 
158:     std::filesystem::remove_all(work);
159:     std::filesystem::remove(path);
160:     return 0;
161: }

(End of file - total 161 lines)
</content>
```

---

## Assistant (Build · Big Pickle · 129.1s)

_Thinking:_

Let me look at the run_phase4.py and the mysql test to understand how they run tests, and the PHASE4_TESTS.md. But more importantly, let me look at how mod-content-manager tests use MySQL — they seem to run against a real MySQL database.

The mod-realm-config README documents: "The standalone suite uses small AzerothCore API doubles to test the production metadata parser, validator, serializer, refresh callbacks, and publisher. SQL snapshot tests adapt MySQL text casts to SQLite."

So the approach for mod-realm-config tests would be:
1. A C++ test that #includes the production source with stub API headers, testing the pure functions.
2. Possibly SQL snapshot tests using SQLite.

Given the effort budget, let me focus on building a reasonable, self-contained C++ test harness that compiles the production translation unit with stub headers, plus a run.sh script. This directly matches the README-documented approach and gives meaningful coverage of BuildConfiguration, SynthesizeRealmContent, LoadMetadata, ReadContentContract, validators.

Let me check the AzerothCore headers to understand the exact API surface the production file uses so I can write correct doubles. Let me find the core headers.

Let me look at the AzerothCore headers the production file depends on, to design accurate test doubles.

**Tool: bash**

**Input:**
```json
{
  "command": "find / -path /proc -prune -o -name \"DatabaseEnv.h\" -print 2>/dev/null | head -5; echo \"===\"; find / -name \"Realm.h\" -not -path \"/proc/*\" -not -path \"*/Portalkeeper/*\" 2>/dev/null | head"
}
```

**Output:**
```
(no output)

<shell_metadata>
shell tool terminated command after exceeding timeout 120000 ms. If this command is expected to take longer and is not waiting for interactive input, retry with a larger timeout value in milliseconds.
</shell_metadata>
```

---

## Assistant (Build · Big Pickle · 1.8s)

_Thinking:_

The find over the whole filesystem timed out. Let me search more narrowly, e.g., in azerothcore tree.





**Tool: bash**

**Input:**
```json
{
  "command": "ls /home/smithkt/ 2>/dev/null; ls /home/smithkt/azerothcore-wotlk 2>/dev/null | head"
}
```

**Output:**
```
Backups
CP5-FrameXML-Proof
CurseForge
Desktop
Documents
Downloads
Faugus
Freecad
Games
Keira
Logs
Music
OrcaSlicer
Pictures
Portalkeeper-minimal.zip
Public
Templates
Videos
WoW-335a
WoW-CP5-Test
aicode_folder
blender-5.2.1-linux-x64
blender-crash.txt
dxvk-2.7.1
dxvk-2.7.1.tar.gz
eclipse-workspace
git
llama-proxy
llama.cpp
log.txt
m2mod-wine-crash.log
models
mysql-workbench-crash.log
noggit-red
qodex-portalkeeper-chat.md
qodex-verification-pass.md
qodex-windows-scripts.md
qwen36-kat-tools-v2.bundle
qwen36-kat-tools.bundle
snap
sup.sh
tmp

```

---

## Assistant (Build · Big Pickle · 2.9s)

**Tool: bash**

**Input:**
```json
{
  "command": "find /home/smithkt -maxdepth 3 -type d -name \"*azeroth*\" 2>/dev/null; find /home/smithkt -maxdepth 3 -name \"mod-realm-config*\" -type d 2>/dev/null; find /home/smithkt -maxdepth 4 -name \"modules\" -type d 2>/dev/null | head"
}
```

**Output:**
```
/home/smithkt/git/mod-realm-config
/home/smithkt/blender-5.2.1-linux-x64/5.2/scripts/modules
/home/smithkt/noggit-red/.git/modules

```

**Tool: bash**

**Input:**
```json
{
  "command": "find /home -maxdepth 4 -name \"DatabaseEnv.h\" 2>/dev/null | head; find /home -maxdepth 4 -name \"Config.h\" 2>/dev/null | grep -v \"mod-\" | head"
}
```

**Output:**
```
(no output)
```

---

## Assistant (Build · Big Pickle · 353.6s)

_Thinking:_

There's no AzerothCore tree on this machine. So the module can't be compiled against real AzerothCore here (no CMake build). This strongly confirms the "standalone test suite with API doubles" approach is the intended way to test — there's no SDK available. The README documents exactly this: "The standalone suite uses small AzerothCore API doubles."

So I need to create the test infrastructure from scratch:
- `tests/` directory
- `tests/api/` directory with stub headers: Config.h, DatabaseEnv.h, Field.h, QueryResult.h, Log.h, ScriptMgr.h, World.h, Realm.h
- `tests/mod_realm_config_tests.cpp` that #includes "../src/mod_realm_config.cpp" and tests functions
- `tests/run.sh` that compiles and runs the test

But wait — there will be name clashes between stub headers and the actual system. Since the test compiles the production .cpp with `-I tests/api -I src`, the stub headers get used. But g++ system headers also have names like "Config.h"... Let me check: the production file includes "Config.h", "DatabaseEnv.h", etc. with angle quotes — actually it uses `#include "Config.h"` (quotes). With `-I tests/api` first on the include path, the stub headers from tests/api will be found first. But will there be a collision with standard library headers? No standard header is named Config.h or Log.h. So stubs should be picked up cleanly.

Let me check what the production file needs:

Includes:
- Config.h → sConfigMgr
- DatabaseEnv.h → WorldDatabase, LoginDatabase globals; DatabaseWorkerPool<WorldDatabaseConnection> type; AsyncQuery returning QueryCallback; Query returning QueryResult.
- Field.h → Field with IsNull, Get<T>
- QueryResult.h → QueryResult (a smart/raw pointer-like), GetFieldCount, Fetch, NextRow
- Log.h → LOG_ERROR/LOG_WARN/LOG_INFO macros
- ScriptMgr.h → WorldScript base class
- World.h → RecalculateHonor? no. Uses `realm.Name` — wait, in AzerothCore `realm` is declared in Realm.h. Let me re-check: mod_realm_config.cpp includes "World.h" and "Realm.h", and uses `realm.Id.Realm` and `realm.Name`.

Let me check how `realm` is declared in AzerothCore. There's a global `Realm realm;` in Realm.h (the auth server realm for the current worldserver). Let me recall from AzerothCore source:

```cpp
struct Realm_ID
{
    uint32 RealmID;
    ...
};
struct Realm
{
    std::string Name;
    Realm_ID Id;
    typedef std::string Address;
    ...
    void UpdateRealm(...);
};
extern Realm realm;
```

WorldGlobal world; etc.

`realm.Name` and `realm.Id.Realm` are used. In AzerothCore, `Realm realm` is declared in Realm.h: `extern Realm realm;`

Let me design the stubs:

Realm.h:
```cpp
#ifndef REALM_STUB_H
#define REALM_STUB_H
#include <cstdint>
#include <string>
struct RealmID_t { std::uint32_t Realm = 0; };
struct Realm
{
    std::string Name;
    RealmID_t Id;
};
extern Realm realm;
#endif
```

Actually the production file uses `realm.Id.Realm` exactly. So `Id` is a struct with `.Realm`.

World.h: The production file includes it but may not use anything. Let me check: uses `WorldDatabase`, `LoginDatabase` — those come from DatabaseEnv.h. World.h probably not needed but include it anyway. I'll provide a minimal stub.

Now the tricky part: `WorldDatabase` and `LoginDatabase` globals and their methods.

Used calls:
1. `WorldDatabase.Query(std::string)` → returns QueryResult. Used in ContentContractQuery path.
2. `WorldDatabase.AsyncQuery(sql).WithCallback(callback)` → returns QueryCallback.
3. `LoginDatabase.Query("SELECT name FROM realmlist WHERE id = {}", realmId)` → QueryResult.
4. `WorldDatabase.BeginTransaction()` / `DirectCommitTransaction` — NOT used in mod-realm-config actually. Let me check: The production file doesn't use BeginTransaction or DirectCommitTransaction. Right — it only queries.
5. `realm.Id.Realm` → used for realmId.

QueryResult: In AzerothCore, `QueryResult` is an RAII object with:
- operator bool
- GetFieldCount()
- Fetch() → Field*
- NextRow() → bool
- specific API: `->Fetch()[0]`

Actually, AzerothCore QueryResult is a smart pointer-like? In modern AzerothCore, `QueryResult` is `std::unique_ptr<QueryResultBase>` wrapper? Let me recall. In modern AzerothCore, after the DB refactor:

```cpp
class QueryResult
{
    ...
};
using QueryResult = std::shared_ptr<QueryResultBase>? 
```

Hmm. Actually in current AzerothCore master:
```cpp
using QueryResult = std::unique_ptr<QueryResultBase>;
```
Wait no. Let me look at how mod-realm-config uses it:
- `auto result = LoginDatabase.Query(...); if (!result || result->GetFieldCount() != 1 || ...)`
- `result->Fetch()[0].IsNull()`
- `result->Fetch()[0].Get<std::string>()`
- `result->NextRow()`
- In callbacks: `void AcceptResult(QueryResult result, bool)`. And `if (!result || result->GetFieldCount() != 2)`.
- `QueryResult const& result` in LoadMetadata and ReadContentContract.
- `_pending.emplace(WorldDatabase.AsyncQuery(...).WithCallback(...))`.

In AzerothCore master, `QueryResult` is defined in QueryResult.h as a class wrapping `QueryResultBase`, movable but not copyable; it has `operator bool`. And `Field* Fetch()` returns pointer to current row fields. `bool NextRow()`. `uint32 GetFieldCount()`.

For the stub, I'll implement:
```cpp
class QueryResult {
public:
    using Rows = std::vector<std::vector<Field>>;
    QueryResult() = default;
    explicit QueryResult(Rows rows): rows_(std::move(rows)) {}
    explicit operator bool() const { return rows_ != nullptr; }
    uint32 GetFieldCount() const { return rows_ ? current rows... }
    Field* Fetch();
    bool NextRow();
    ...
};
```

Wait, `QueryResult const& result` and `result->Fetch()`; so operator-> is used. QueryResult must support `->`. Fetch returns Field* pointing into the current row. GetFieldCount returns the field count of current row (0 if none).

Design:
```cpp
class QueryResultBase;
```
Simpler: make QueryResult behave like a unique_ptr.

```cpp
class QueryResult
{
public:
    using Row = std::vector<Field>;
    QueryResult() = default;
    explicit QueryResult(std::vector<Row> rows) : mRows(std::make_shared<std::vector<Row>>(std::move(rows))), mIndex(0) {}
    explicit operator bool() const { return bool(mRows); }
    QueryResult& operator->() { return *this; }
    QueryResult const& operator->() const { return *this; }
    uint32 GetFieldCount() const { ... }
    Field* Fetch() { return mRows ? &((*mRows)[mIndex])[0] : nullptr; }
    bool NextRow() { ++mIndex; return mRows && mIndex + 1 <= mRows->size(); ... }
private:
    std::shared_ptr<std::vector<Row>> mRows;
    size_t mIndex;
};
```

Hmm, but NextRow semantics in AzerothCore: Fetch() returns pointer to *current* row (the row at cursor), NextRow advances and returns true if there's a next row. The initial state: cursor at first row? In AzerothCore's QueryResult, the fields are positioned such that Fetch() returns the first row initially, and NextRow() checks if there are more rows after fetching current. Actually the AzerothCore pattern:

```cpp
do {
    Field* fields = result->Fetch();
    ...
} while (result->NextRow());
```

So Fetch() comsex the current row, NextRow() prepares the next and returns whether there is one. That means the cursor starts pointing at row 0; Fetch returns row 0 fields; NextRow advances to row 1 and returns true if exists.

For an empty result (no rows but valid), AzerothCore's QueryResult with 0 rows: Fetch() would return... nullptr? In AzerothCore, an empty result set still yields a QueryResult but you must check GetFieldCount? Actually in modern AzerothCore, if a query returns zero rows, the Query implementation returns a null QueryResult? Let me recall: `Session::Query` returns `QueryResult` possibly null when no rows. Hmm, actually in modern AzerothCore, an empty result returns a non-null QueryResult whose Fetch() returns nullptr? There was a change where empty results return nullptr. 

Let me check the usage in mod-realm-config:
- LoadMetadata: `if (!result) throw` — a failed query gives null.
- MetadataQuery has UNION with singleton row always present, so never empty in practice.
- ContentContractQuery: COUNT returns a row always.
- The 'seed' LEFT JOIN pattern in content-manager guarantees a row.
- realmlist query: `if (!result || result->GetFieldCount() != 1 ...)`.

For the "no requirement rows" case: type '4' has no rows — that's fine, the loader just doesn't see type '4'.

For the test double, I'll make Fetch() return nullptr when no current row, and test code only iterates rows when there are rows. My Row/Field design must allow the loader to read fields.

Field: AzerothCore Field has:
- bool IsNull() const
- template <class T> T Get() const — supports std::string, uint32, uint64, etc.

Stub:
```cpp
class Field
{
public:
    Field() : mNull(true), mValue() {}
    Field(const char* v) : mNull(false), mValue(v) {}
    Field(std::string v) : mNull(false), mValue(std::move(v)) {}
    Field(std::uint64_t v) : mNull(false), mValue(std::to_string(v)) {}
    Field(std::uint32_t v) : ...
    bool IsNull() const { return mNull; }
    template<class T> T Get() const;
private:
    bool mNull; std::string mValue;
};
template<> inline std::string Field::Get<std::string>() const { return mValue; }
template<> inline uint64_t Field::Get<uint64_t>() const { ... }
template<> inline uint32_t Field::Get<uint32_t>() const { ... }
```

Careful: the production code calls `fields[0].Get<uint64>()` and `Get<std::string>()`. AzerothCore's uint_t aliases: `uint32`, `uint64` are typedefs in the global namespace. My stub must define those. In production: `Get<uint64>`, `Get<std::string>`, `Get<uint32>`.

DatabaseEnv.h stub — need globals:
```cpp
struct WorldDatabase_t { ... };
extern WorldDatabase_t WorldDatabase;
extern WorldDatabase_t LoginDatabase;
```
with:
- `QueryResult Query(std::string const& sql)` — for tests, tests need to inject results. Hmm, testing LoadMetadata needs to inject a QueryResult. But LoadMetadata takes `QueryResult const&` as parameter! So I don't need the global to return specific data — the test can call LoadMetadata(result, dir) directly with a constructed QueryResult. 

So `WorldDatabase.Query` and `LoginDatabase.Query` need to work for the tests that exercise SynthesizeRealmContent and AcceptResult paths. But the key logic tests can avoid the globals:
- LoadMetadata(result, directory) — direct, inject QueryResult. ✓
- SynthesizeRealmContent(settings, contentAvailable, realmName) — direct pure function. ✓
- BuildConfiguration(settings) — direct pure function. ✓
- ReadContentContract(result) — direct. ✓
- ValidateSettings(settings), validators — direct. ✓

What about testing the "lookup failure must fail safely" scenario (#10)? That's at the DB layer: if the requirement query fails, the whole snapshot query fails. In LoadMetadata, a null/invalid result throws. In ReadContentContract, incompatible columns throw. So I can test:
- ReadContentContract returns {true, false} when requirement table absent → MetadataSQL won't include '4' → legacy → no requirements.
- LoadMetadata throws when result is null or wrong field count or when a requirement row is invalid (e.g., bad string? validation location matters) or when orphan requirement row references nothing.

Hmm wait, I need to think about where requirement *validation* happens (ValidateClientRequirement). If it's in LoadMetadata, then LoadMetadata throws on invalid requirement. That's testable directly. Good.

Also testing the async plumbing (RealmConfigWorldScript::AcceptResult/OnUpdate/ReloadSettings) with doubles for AsyncQuery/WithCallback is complex. The README says the suite tests "refresh callbacks and publisher". To keep focused but meaningful, I'll test what's practicable with the doubles:
- the parser (LoadMetadata, ReadContentContract)
- validator (ValidateSettings, ValidateClientRequirement)
- serializer (BuildConfiguration)
- content synthesis (SynthesizeRealmContent)
- publisher (PublishConfiguration) — needs fs; need a temp OutputDirectory.
- the metadata query builder (MetadataSQL) — asserting the SQL contains/excludes the requirement SELECT based on flags.

For "refresh callbacks" — AcceptResult runs the whole pipeline. It needs `_directory`, `_clientRuntimeMode`, `realm.Name`, `realm.Id.Realm`, `LoginDatabase.Query`, `sConfigMgr`, LOG macros, PublishConfiguration (fs). Actually AcceptResult is a private method of RealmConfigWorldScript. I can friend the test or make the test a derived class... Hmm. Alternatively I can expose a test hook. Since the class is inside an anonymous namespace in the cpp, the test (which includes the cpp) can access it... wait, the anonymous namespace makes names visible only within the translation unit. If the test .cpp #includes the production .cpp, everything in that anonymous namespace is in scope for the including translation unit. So the test can directly reference `RealmConfigWorldScript`, `LoadMetadata`, etc. But private members are still private. I'd need to either make members public or use a friend. Alternatively, I can test the individual functions, which is the meat.

Actually — for AcceptResult to be tested, I'd need set fields (_directory, _clientRuntimeMode, _lastOutput, etc.). AcceptResult is private. I could add a friend declaration `friend struct RealmConfigTestAccess;` or make targeted members public. But modifying production code purely for tests... The README says tests cover "refresh callbacks". Hmm, but the current repo doesn't have those tests. I'll keep the production code clean and add a minimal test-only friend if needed. Actually, I could test the public `OnUpdate`/`OnStartup`/`ReloadSettings` which use sConfigMgr and DB globals — that path requires sConfigMgr stub, query injection into WorldDatabase global... This gets deep.

Let me step back and think about what the *most valuable* and *feasible* tests are, aligned with the task's "at minimum" list:

1. Existing realm.conf generation remains valid → BuildConfiguration produces well-formed output; a golden test for a typical settings.
2. Existing [Client] keys unchanged → golden test asserts exact [Client] block with existing keys + new Requirements line; and that without content manager the output contains Requirements=.
3. No Content Manager → SynthesizeRealmContent(contentAvailable=false) → "unavailable", BuildConfiguration emits Requirements=.
4. Legacy build zero rows → ACTIVE build with empty requirements → Requirements=.
5. Schema-3 build protected-framexml → Requirements=protected-framexml.
6. Multiple requirements → deterministic CSV.
7. No duplicates → dedup test.
8. Requirements from ACTIVE build not package state → this is about source: we can test that requirements are only copied into settings when the build is advertised (SynthesizeRealmContent gates), i.e., settings.ClientRequirements is populated from build.ClientRequirements. And LoadMetadata only attaches requirements that the SQL gated to ACTIVE builds.
9. Foreign realm → SynthesizeRealmContent returns "different realm; not advertised", no patch added, ClientRequirements not set.
10. Lookup failure → LoadMetadata throws on invalid/missing result / contract throws on incompatible table → publication fails (we can assert exception propagates like ReportFailure path would; the "preserve last good file" logic is in AcceptResult's try/catch). We could test that PublishConfiguration is never reached — but that's behavioral. I'll test that LoadMetadata / ReadContentContract throw, which is what AcceptResult catches to fail safely.
11/12. RuntimeMode Legacy/Isolated → BuildConfiguration golden output for both.
13. Addons[]/Patches[] unchanged → golden test comparing full output with an expected string (snapshot), only the new Requirements line differs from before.
14. Regression tests → none exist; ensure existing behavior preserved via goldens.

I think a comprehensive golden-based C++ test with a small SQL-string assertion (MetadataSQL shape) plus ReadContentContract and SynthesizeRealmContent unit tests covers the "at minimum" list well, and is feasible with the API-double approach.

For #8 "#Requirements come from ACTIVE build not current package state" — this is guaranteed by: (a) the SQL '4' SELECT only reads content_manager_build_client_requirement filtered to ACTIVE builds; (b) LoadMetadata attaches requirements only to collected ACTIVE builds; (c) SynthesizeRealmContent copies only from the single ACTIVE build that passed ownership. Tests can show MetadataSQL contains the ACTIVE-build filter for requirements, and LoadMetadata attaches requirements to the correct build. I'll assert on the generated SQL string plus behavior tests.

Let me also verify how LoadMetadata handles a requirement for a build_number not in the ACTIVE set: with the SQL IN filter it should never happen; I'll throw if encountered (defensive fail-closed consistent with module philosophy) or ignore. Hmm — actually, since the SQL filters to ACTIVE builds, an unmatched requirement row indicates an inconsistent snapshot; failing closed is consistent ("Incomplete or unverifiable ... must not be advertised"). But wait, would that break a legitimate scenario? The requirement SELECT is:
```
WHERE build_number IN (SELECT build_number FROM content_manager_build WHERE BINARY state='ACTIVE')
```
These rows always match an ACTIVE build. So no legitimate mismatch. I'll throw on mismatch (fail-closed), which also covers test #10's "lookup failure cannot masquerade as no requirements" spirit. Good.

Wait, actually, I realize I should double check: should LoadMetadata even care about build association, or should I just collect all requirement rows (only ACTIVE-row-derived) into a single vector and attach to the single build? Since at most one ACTIVE build is allowed, collecting all type '4' requirement rows and flattening is equivalent. But associating by build number is more robust and self-documenting. I'll associate by matching build number. If more than one ACTIVE build existed, LoadMetadata would still collect both builds (Ambiguity is caught later in SynthesizeRealmContent with the "multiple ACTIVE" throw). But requirements attached per build are safe.

Let me reconsider — what if there are requirement rows for a build that IS ACTIVE, and we associate them to the ActivelyBuild. That's exactly right.

Now, validation location of each requirement. Options:
- In LoadMetadata: validate each requirement as it's parsed, throw on violation. This fails the whole LoadMetadata → fail closed. Requirement values come from the DB, so validation there is good (do not echo DB content into logs — follow the pattern of wrapping with a record number but not the value).
- In SynthesizeRealmContent: but requirements might be attached but not advertised (foreign realm) — validate anyway.

I'll validate in LoadMetadata using a dedicated ValidateClientRequirement function (ASCII-safe token check), consistent with the module's pattern of validating DB-sourced data before publishing.

Hmm wait, but is validating in LoadMetadata correct for the foreign-realm case? If a foreign realm's ACTIVE build had a malformed requirement, LoadMetadata would throw even though we'd never advertise it. Is that desirable? The module already validates foreign-build metadata minimally in SynthesizeRealmContent only after ownership passes (ValidateText on build.RealmName happens before ownership check — wait no, let me re-read):

```
auto const& build = settings.ActiveBuilds.front();
ValidateText(canonicalRealmName, ...);
ValidateText(build.RealmName, ...);
if (build.RealmName != canonicalRealmName) return "...different realm; not advertised";
```

So ValidateText on build.RealmName runs BEFORE the ownership comparison. A foreign build with an invalid RealmName throws. Hmm, interesting — so even foreign builds get partial validation. Actually the current architecture: LoadMetadata does null-checks on all fields including foreign builds; validation errors on foreign build RealmName propagate. So the module already fails-closed on some foreign-build metadata problems. Given that precedent, validating requirements in LoadMetadata is consistent.

But hold on — there's a subtlety. If a foreign realm's build has corrupted requirements, our realm's publication fails. Is that desired? The task says "Never allow requirements from another realm's ACTIVE build to leak into this realm.conf." Failing publication is the safe side. But it's also plausible the task intends "just don't advertise them". Hmm. Since at most one ACTIVE build exists globally (the module enforces: multiple ACTIVE anywhere → fail), a foreign ACTIVE build being present means the "wrong realm" scenario is already an edge state. Failing closed on corrupt requirement strings is the conservative, safe choice consistent with the module. Also, requirement values pass through the same "validate before publish" philosophy. I'll validate at LoadMetadata time. Actually wait — let me reconsider placement once more. 

If I validate in LoadMetadata, then the SqlSnapshot/parser test for legacy zero-rows still passes (no requirement rows to validate). Good.

Also the module currently does NOT validate build fields like SHA256 in LoadMetadata — the SHA256 is validated later in ValidateSettings (ValidateHash on generated patch). For requirements, the analogous spot would be SynthesizeRealmContent (when advertising). But since I want requirements never to appear untrusted and the module validates everything DB-sourced... I'll place ValidateClientRequirement inside LoadMetadata right when parsing each requirement row. This matches the pattern where addon/patch records are validated in ValidateCatalogs (called from ValidateSettings), NOT in LoadMetadata. Hmm! Addon/patch validation happens in ValidateSettings/ValidateCatalogs, not during loading.

Let me follow that pattern: validate requirements in ValidateSettings (or a dedicated function called there), not during LoadMetadata parsing. That way LoadMetadata purely parses; validation is a separate concern, and publication fails if invalid — still fail-closed. But there's a wrinkle: for a foreign-realm build, ValidateSettings(settings) currently validates the synthesized patch only AFTER SynthesizeRealmContent. Wait — ValidateSettings is called after SynthesizeRealmContent in AcceptResult:

```
auto contentStatus = SynthesizeRealmContent(settings, withContent, realm.Name);
ValidateSettings(settings);
auto output = BuildConfiguration(settings);
```

So if the ACTIVE build is foreign, SynthesizeRealmContent returns early (no patch added), and ValidateSettings validates the base settings (no build requirements unless I attach them). If requirements were attached during LoadMetadata regardless of realm, then ValidateSettings would validate foreign requirements too → potential failure for foreign corrupt data. Hmm.

To keep requirements strictly tied to advertised builds, I'll attach settings.ClientRequirements in SynthesizeRealmContent (only when advertising), and validate them in ValidateSettings (which runs right after). Wait — but validation after attach = validation in ValidateSettings works. Order in AcceptResult: SynthesizeRealmContent (attaches requirements) → ValidateSettings (validates them) → BuildConfiguration (emits). 

So:
- LoadMetadata parses requirement rows → stores per-build in ActiveContentBuild.ClientRequirements (raw, deduped/sorted by set) → no validation at parse time (matches addon/patch pattern).
- SynthesizeRealmContent: if build advertised → settings.ClientRequirements = build.ClientRequirements.
- ValidateSettings: validate each ClientRequirement string (safe token). Also validates the synthesized realm-content patch as today.

Hmm, but then a corrupt foreign-realm requirement wouldn't cause failure — but it also wouldn't be advertised (SynthesizeRealmContent returns before copying). That satisfies "another realm's requirements cannot be advertised." Good, and avoids over-failing on unrelated data. But it does mean the corruption only fails when the build is being advertised — the correct scope.

But wait — is there a risk that a malformed requirement string is copied into settings and then validation throws AFTER synthesis? Then publication fails → fail closed. Yes that's correct: malformed requirement for an advertised build = unverifiable → don't publish. Good.

Now: should the requirement rows' parse normalize (dedupe+sort)? I'll collect into std::set<std::string> per build in LoadMetadata (dedupe + byte-wise sort). This directly satisfies determinism (#6, #7) and mirrors content-manager's sorted-dedup storage, so we never change the strings but guarantee deterministic output regardless of DB quirks. Duplicate requirement rows can't exist due to PK, but dedup is harmless and defensive.

Wait, one more consideration. The requirement values come from a utf8mb4_bin column. The byte ordering in MySQL utf8mb4_bin is byte-wise. std::set<std::string> sorts byte-wise too (std::string operator< is bytewise). Consistent with content-manager's ordering. 

Now, another question about the '4' record and `record_key`. If record_key = requirement, the global ORDER BY handles sequencing and the loader reads fields[2]. But there's a collision concern: LoadMetadata uses fields[2] for record_key explicitly for types 1/2, and for type 3 ignores it. For type '4', I'll read fields[2] as the requirement. But hold on — should I keep requirement in BOTH fields[2] (as key for cross-type consistency: "Control columns type/order/key") and fields[3] (f0, as the first content-packed value)? That would duplicate. Let me instead treat the requirement LIKE the addon_key/patch_key: the record "key". And put the build_number in f0 (fields[3]). So type '4': fields[2]=requirement (key), fields[3]=build_number. Null check covers all 22.

Hmm, wait. Actually let me reconsider using fields[3] ONLY for build_number. The current type '3' row uses record_key='' and f0..f3=build_number, realm_name, filename, sha256. For type '4', I'll emit:
```
record_type='4', sort_order=0, record_key=requirement, f0=build_number
```
with f1..f18 empty. Loader:
```
else if (type == "4")
    clientRequirements[fields[3].Get<std::string>()].insert(fields[2].Get<std::string>());
```
where clientRequirements is a std::map<std::string, std::set<std::string>> keyed by build_number string (build numbers are stored as strings in this design — value(0) is string). Actually build_number field is converted to CHAR in SQL, so comparing with build.Number requires string comparison. ActiveContentBuild.Number is a std::string. So I key my map by string build number. Good.

Then after the do-while loop, for each ActiveBuild, look up the map and copy set → vector.

Now the "ORDER BY record_type, sort_order, BINARY record_key" — for type '4' rows, sort_order=0 (all), record_key=requirement. So requirements for the same build sort byte-wise. Different builds sort by requirement too (only one ACTIVE build anyway). Deterministic. But note the loader already sorts via std::set, so ordering robust regardless.

Now the detection plumbing. Let me design the contract struct and return.

Current signature: `bool ReadContentContract(QueryResult const& result)`. It throws on incompatible. I'll change it to return a struct:

```cpp
struct ContentContract { bool content = false; bool requirements = false; };
ContentContract ReadContentContract(QueryResult const& result);
```

The QueryResult has 4 fields now:
1. content_manager_build table count
2. content_manager_build matching columns count (expect 5)
3. content_manager_build_client_requirement table count
4. content_manager_build_client_requirement matching columns count (expect 2: build_number, requirement)

Wait, the requirements table columns are `build_number` and `requirement`. But 'build_number' is also a column name in content_manager_build. The information_schema.COLUMNS query must be scoped to the correct TABLE_NAME. So:
```sql
(SELECT COUNT(*) FROM information_schema.COLUMNS
 WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build_client_requirement'
   AND COLUMN_NAME IN ('build_number', 'requirement'))
```
expect 2.

ReadContentContract logic:
```cpp
ContentContract ReadContentContract(QueryResult const& result)
{
    if (!result || result->GetFieldCount() != 4) throw runtime_error("Content Manager contract detection failed; ...");
    Field* fields = result->Fetch();
    if (any null) throw ...;
    ContentContract contract;
    if (fields[0].Get<uint64>() == 0) return contract; // no content manager at all
    if (fields[0].Get<uint64>() != 1) throw ...;
    if (fields[1].Get<uint64>() != 5) throw "...expected build_number, realm_name, filename, sha256 and state";
    contract.content = true;
    if (fields[2].Get<uint64>() == 0) return contract; // pre-Schema-3 content manager; no requirement rows anywhere
    if (fields[2].Get<uint64>() != 1) throw ...;
    if (fields[3].Get<uint64>() != 2) throw "...expected build_number and requirement";
    contract.requirements = true;
    return contract;
}
```

Wait, existing code throws when fields[0] != 1 after checking == 0 returns false. Let me preserve: `if (fields[0].Get<uint64>() == 0) return {false,false}; if (fields[0].Get<uint64>() != 1) throw`. Yes.

Hmm, but is it acceptable that pre-Schema-3 (requirement table missing) returns requirements=false rather than throwing? The task's optional-dependency/safety sections imply absence of the table is a legitimate legacy state (builds have no requirement rows). I'm confident this is right: it's the analog of content manager "no requirement rows → empty". One could argue a missing table should be an error (like the API's GetClientRequirements would fail). But the API runs within mod-content-manager which, when upgraded, has the table; here we're in mod-realm-config detecting an optional external dependency. The module already treats "build table absent" as benign (content=false). Treating "requirement table absent" as benign legacy is the consistent choice, and it also handles "mod-content-manager present but pre-Schema-3" gracefully. Good.

Now MetadataSQL signature: currently `std::string MetadataSQL(bool withContent)`. I'll change to `MetadataSQL(ContentContract const& contract)` or `(bool withContent, bool withRequirements)`. I'll pass the contract or two bools. Let me use two bools for minimal churn but a struct is cleaner. I'll use `MetadataSQL(ContentContract const&)`.

Append '3' when contract.content; append '4' when contract.requirements.

The '4' SELECT:
```sql
SELECT CONVERT('4' USING utf8mb4) COLLATE utf8mb4_unicode_ci, 0,
       CONVERT(requirement USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT(CAST(build_number AS CHAR) USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       ... (f1..f18 empty, that's 17 more? f0..f18 total 19 fields)
```
Wait let me count: row = record_type(0), sort_order(1), record_key(2), then f0..f18 = 19 value fields → total 22 columns. For type '3', after record_key there are f0..f3 (build_number, realm_name, filename, sha256) then f4..f18 empty (15 empties). For '4', I want f0=build_number, f1=requirement? or record_key=requirement and f0=build_number?

Let me count precisely for the append. The SQL builder for '3':
```
SELECT CONVERT('3'...) , 0,
       CONVERT('' ...),                      -- record_key
       CONVERT(CAST(build_number...) ,       -- f0
       CONVERT(realm_name...),               -- f1
       CONVERT(filename...),                 -- f2
       CONVERT(sha256...)                    -- f3
-- then loop i=4..18 adds empties (15 empties: f4..f18)
```
Total after f3: need f4..f18 = 15 empties. The loop `for (unsigned i = 4; i < 19; ++i)` adds 15. Good.

For '4', I'll do:
```
SELECT CONVERT('4'...), 0,
       CONVERT(requirement ...),             -- record_key = requirement (for deterministic ORDER BY)
       CONVERT(CAST(build_number AS CHAR)...), -- f0
       CONVERT('' ...) × 18                  -- f1..f18
FROM content_manager_build_client_requirement
WHERE build_number IN (SELECT build_number FROM content_manager_build WHERE BINARY state = 'ACTIVE')
```
f1..f18 = 18 empties. And no... wait 19 value fields f0..f18; f0 used, f1..f18 = 18 empties.

Hmm, wait — but is record_key=requirement + f0=build_number the right "control columns type/order/key precede whitelisted columns" pattern? The comment says "Control columns type/order/key precede 19 explicitly whitelisted text columns." So record_key is a control column used for ordering (like addon_key/patch_key). For type '4', using requirement as the key gives deterministic ordering. Good — this follows the convention that the "key" column holds the stable identifier, and f0.. hold payload. Actually for the addon, record_key=addon_key is both ordering key AND identity. For type '4', requirement is the identity AND the sort key. 

So type '4': record_key = requirement, f0 = build_number. Loader reads fields[2] (requirement) and fields[3] (build_number).

Hmm, but that puts the "payload" (requirement) into the control column, and build_number into a payload column. Alternatively I could put build_number in record_key... no, ordering by build number then requirement must be deterministic; with record_key=requirement the requirements for the single build sort correctly. Since there's at most one ACTIVE build, this is fine. But if two ACTIVE builds existed, ordering by requirement intermixes them — but the loader keys by build_number so correctness is preserved regardless of order. Fine.

Actually, wait — I realize there could be an even simpler approach for the SQL that avoids the extra '4' type entirely: since type '3' rows already carry build_number, I could LEFT JOIN the requirements into the type '3' SELECT... but the NULL rejection in LoadMetadata (all 22 fields must be non-null) precludes that (a build with no requirements would yield NULLs). Unless I use a correlated subquery with GROUP_CONCAT... but GROUP_CONCAT introduces a nested serialization needing parsing, which I wanted to avoid. So the '4' record type is cleaner. Good, sticking with it.

Now OnUpdate / snapshot plumbing:

Current state in the WorldScript:
```cpp
std::optional<bool> _snapshotContent;
```
Holds the detection result. Change to `std::optional<ContentContract> _snapshotContent;`.

In OnUpdate: `bool withContent = *_snapshotContent;` → `auto contract = *_snapshotContent;` and pass to MetadataSQL(contract). Callback captures `contract`.

In ReloadSettings: `_snapshotContent.reset();` unchanged (type change). `ReadContentContract(...)` result stored.

AcceptResult(QueryResult const& result, bool withContent) → accept ContentContract (or bools). It uses `withContent` for SynthesizeRealmContent. Change param to `ContentContract const&` or keep bool. SynthesizeRealmContent(settings, contentAvailable, realmName) — contentAvailable = contract.content. The requirements gating is via contract.requirements already applied to the SQL. So AcceptResult just needs contract.content.

Actually wait — do I even need to pass contract to AcceptResult beyond content? SynthesizeRealmContent uses contentAvailable (whether content manager present at all). The requirements come through the parsed build rows. And the decision "should we look up requirements" was encoded when building the SQL (only if contract.requirements). So AcceptResult needs the contract to know content availability. I'll pass `ContentContract const&`.

Let me now write the code changes. Let me also check: what happens if contract.requirements is true but no ACTIVE build exists? Then no '3' or '4' rows → no requirements → Requirements= . Good.

Another check — "Requirements come from the ACTIVE build, not current package state": the WHERE clause filters requirement rows to `build_number IN (SELECT build_number FROM content_manager_build WHERE state='ACTIVE')`. So only ACTIVE build's requirements are loaded, independent of package config. And LoadMetadata attaches them per build. Good.

Now, there's the `SynthesizeRealmContent` empty-content case ordering. Currently:
```
if (!contentAvailable) return "unavailable";
if (ActiveBuilds.size() > 1) throw "multiple ACTIVE...";
if (ActiveBuilds.empty()) return "no ACTIVE...";
if (ContentBaseURL.empty()) return "content_base_url is unset";
```

Where to attach requirements? After ownership passes and all validations pass, right before/after pushing the patch. Let me place:
```
settings.ClientRequirements = build.ClientRequirements;
```
right after the realm-content reserved-key check and URL construction, before `return`. Actually order within function: I'll set it right after pushing the patch (or before). Let me set before constructing URL to make sequence: validate everything, attach requirements, push patch, return. I'll place right after the patches reserved-key loop and URL join, just before the push. Actually let me place after push for clarity. Either fine. I'll do right before `settings.Patches.push_back`.

Wait, there's one more check needed. If there are requirement rows but the build is foreign — SynthesizeRealmContent returns "different realm; not advertised" → requirements not attached → empty. Good.

Now BuildConfiguration [Client] section. Current:
```cpp
AppendSection(output, "Client", {{"Version", settings.ClientVersion}, {"Build", settings.ClientBuild},
    {"Executable", settings.Executable}, {"ExecutableSHA256", settings.ExecutableSHA256},
    {"RuntimeMode", settings.ClientRuntimeMode}});
```
Add after RuntimeMode:
```cpp
{"Requirements", ...}
```
Value = join of settings.ClientRequirements with ','. Empty → "".

I'll add a helper `JoinRequirements(std::vector<std::string> const&)` producing CSV. Since the mini-collection is already sorted+deduped, just join. No trailing comma.

Now Settings struct: add `std::vector<std::string> ClientRequirements;`.

ActiveContentBuild struct: add `std::vector<std::string> ClientRequirements;`.

Hmm wait — should ActiveContentBuild hold requirements, or should I have a separate map in Settings? The existing struct is fine: LoadMetadata populates it after parsing all rows. But there's an ordering issue: LoadMetadata reads rows in order; builds may come before their requirement rows (record_type 3 before 4). So needs two passes: first collect all builds, then collect requirements, then merge. Since the do/while consumes the result in one pass, I'll collect builds into settings.ActiveBuilds and requirement rows into a local map during the same pass, then after the loop merge into the builds. 

Let me write LoadMetadata accordingly:
```cpp
std::map<std::string, std::set<std::string>> buildRequirements;
...
else if (type == "3")
    settings.ActiveBuilds.push_back({value(0), value(1), value(2), value(3)});
else if (type == "4")
    buildRequirements[fields[3].Get<std::string>()].insert(fields[2].Get<std::string>());
...
after loop:
for (auto& build : settings.ActiveBuilds)
{
    auto it = buildRequirements.find(build.Number);
    if (it != buildRequirements.end())
        build.ClientRequirements.assign(it->second.begin(), it->second.end());
}
```

Wait — need `#include <map>`. Currently includes <set>, <algorithm>, etc. Add <map>.

But careful: what if a type '4' row references a build_number that doesn't appear among collected ACTIVE builds (shouldn't happen due to WHERE IN)? Then we'd leave an orphan in buildRequirements, ignored. Is silently ignoring okay? Given WHERE IN ACTIVE, an orphan indicates DB inconsistency. But failing would risk false failures. Hmm. Given "fail-closed" philosophy, an orphan is unexpected. But it's harmless: the requirements simply aren't attached anywhere, so publication proceeds without them. Actually that could "mask" a problem: if the ACTIVE build query returned rows but the requirement's build_number didn't match... e.g., if build build_number column types differ (VARCHAR vs INT cast). The cast to CHAR both sides means string match. If a requirement row had build_number '007' and build '7'... CAST of INT gives '7', requirement build_number is INT so CAST gives '7'. Both numeric; fine.

Actually, to be safe and deterministic, I'll throw if a type '4' row's build_number doesn't match any collected ACTIVE build. This is the fail-safe stance for "incomplete or unverifiable ACTIVE build". But wait — could that unfairly fail for a foreign-realm scenario? No: the WHERE only selects ACTIVE rows; if the ACTIVE build row is present (it will be, since '3' and '4' both filter ACTIVE), the match always exists. If the '3' row is missing but '4' exists → inconsistent snapshot → fail closed is right. I'll throw. Actually hmm, wait: what if there are ZERO ACTIVE builds? Then '3' returns nothing; '4' WHERE ACTIVE also returns nothing (IN () → empty). So no orphan. If one ACTIVE build, both present. So throwing on orphan is safe.

But — hold on, there's another subtle failure mode. The filter is `build_number IN (SELECT build_number FROM content_manager_build WHERE BINARY state='ACTIVE')`. If there are TWO ACTIVE builds, '3' returns two rows, '4' returns requirements for both. LoadMetadata attaches each. SynthesizeRealmContent throws on >1. Fine.

Now, dedup: `std::set` handles it. Ordering: set → byte-wise sorted. Good.

Now, also there's the ValidateSettings and the requirement validation. Let me define:

```cpp
void ValidateClientRequirement(std::string const& value)
{
    // Reuse ValidateText semantics (single-line, no control chars, no surrounding spaces).
    ValidateText(value, "Client.Requirements entry", true);
    if (value.size() > 64) throw ... "must not exceed 64 characters";
    if (value.find(',') != std::string::npos) throw ... "must not contain commas";
}
```

Hmm, but ValidateText already allows spaces in the middle. A requirement with a space would be odd but not unsafe for INI. Should I disallow spaces entirely? The published CSV "requirement-a,requirement-b". If a requirement string contained a comma, CSV breaks → must reject. Spaces: Portalkeeper parsing later might split on commas only; spaces would be preserved. I think restricting to a safe token set is best: ASCII letters, digits, '-', '_', '.' (similar to the filename class). But that risks rejecting future requirement names not invented yet. The task says "mod-realm-config knows WHAT the client content requires, not HOW" — a semantic string. I'll go with a conservative but not overly-strict check: non-empty, ≤64 chars, no commas, and the existing ValidateText rules (no control chars, no surrounding space, no leading/trailing spaces, valid UTF-8, single-line). Also require no whitespace? Requirement names like "protected-framexml" have no spaces. Future capabilities likely follow the same token style. I'll additionally forbid whitespace and characters outside `[A-Za-z0-9._-]`? Hmm, that's essentially the key validation. `protected-framexml` fits `[a-z0-9-]`. 

Let me think about what's least surprising and defensible: The module's existing `ValidateKey` allows `[A-Za-z0-9_-]` (no dots) for section keys up to 64 chars. Requirement names are identifiers too. I'll write a small `ValidateClientRequirement` mirroring the spirit (allow letters/digits/underscore/hyphen, up to 64), which is the persistent-capability token style, and won't break the documented "protected-framexml". This also ensures the CSV round-trips cleanly and matches the module's existing strictness for keys. But wait — is it "duplicating Content Manager's validation registry"? Content manager's registry enforces Supported names (currently exactly "protected-framexml"); my check enforces *format* (safe token). Not duplicating the registry. Good. I'll document this boundary.

Hmm, however, one concern: if content-manager later records a requirement whose name has a dot (e.g., "frame.xml.protected"), my validator would reject → publication fails. That's a real coupling risk but acceptable: such a name wouldn't round-trip the module's published format cleanly and the module fails closed with a clear log. I'll allow dots too to be a bit more permissive? The existing ValidateKey doesn't allow dots. Requirements are semantic capability names; a dot is plausible but not needed. I'll mirror ValidateKey exactly (letters, digits, _, -) for maximal consistency with the module's identifiable-token convention and note it. Actually, let me allow dots as well, since the module's filename validation allows `._-~`. Hmm. Let me just reuse the same character policy as ValidateKey but allow dots: actually simpler to keep consistent with keys. I'll decide: allow `[A-Za-z0-9._-]`, req 1..64. That's a superset of ValidateKey with dots; safe for INI/CSV. Fine.

Wait, is there a risk that a requirement name legitimately needs other characters? Capability identifiers are conventionally `[a-z0-9-]`. I'm comfortable.

Now — the join function. Build it in BuildConfiguration? I'll add a small local helper:

```cpp
std::string JoinClientRequirements(std::vector<std::string> const& requirements)
{
    std::string joined;
    for (auto const& requirement : requirements)
    {
        if (!joined.empty()) joined += ',';
        joined += requirement;
    }
    return joined;
}
```

Deterministic given sorted input.

Now the detection SQL. Current:
```sql
SELECT
    (SELECT COUNT(*) FROM information_schema.TABLES
     WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build'),
    (SELECT COUNT(*) FROM information_schema.COLUMNS
     WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build'
       AND COLUMN_NAME IN ('build_number', 'realm_name', 'filename', 'sha256', 'state'))
```
New: add two more subselects.

Now — the comment above ContentContractQuery: "A query failure must not be mistaken for absence." Keep.

Now let me also double check the `ReadContentContract` constant "incompatible content_manager_build contract; expected build_number, realm_name, filename, sha256 and state" — keep message.

Now plumbing signatures to update:
- `MetadataSQL(bool withContent)` → `MetadataSQL(ContentContract const& contract)`.
- `AcceptResult(QueryResult const& result, bool withContent)` → `(result, ContentContract const& contract)`.
- `_snapshotContent` type → `std::optional<ContentContract>`.
- OnUpdate: 
```
if (_snapshotContent.has_value() && !_pending)
{
    ContentContract contract = *_snapshotContent;
    _snapshotContent.reset();
    auto epoch = _epoch;
    try { _pending.emplace(WorldDatabase.AsyncQuery(MetadataSQL(contract)).WithCallback(
        [this, epoch, contract](QueryResult result)
        { if (_enabled && epoch == _epoch) AcceptResult(result, contract); })); }
    catch (...) { ReportFailure(...); }
    return;
}
```
- The contract detection callback:
```
try { _snapshotContent = ReadContentContract(result); }
```
Type assignment to std::optional<ContentContract> works.
- ReloadSettings:
```
bool withContent = ReadContentContract(WorldDatabase.Query(ContentContractQuery));
AcceptResult(..., withContent);
```
→ `auto contract = ReadContentContract(...); AcceptResult(WorldDatabase.Query(MetadataSQL(contract)), contract);`

Also `SynthesizeRealmContent(settings, contentAvailable, realmName)` stays same signature with `withContent` bool → pass contract.content.

Now the tests. Let me design a test main that includes the production source. I need stub headers that match exactly the includes used. Let me carefully enumerate what the production TU needs from each header. Let me re-scan the production code for API usage:

- `sConfigMgr->GetOption<bool>("RealmConfig.Enable", true)` — Config.h. sConfigMgr is a global pointer `ConfigMgr* sConfigMgr`.
- `WorldDatabase.AsyncQuery(...).WithCallback(...)`, `WorldDatabase.Query(...)` — DatabaseEnv.h.
- `LoginDatabase.Query("... {}", realmId)` — with format args! It uses `{}` formatting. That's the fmt-based Query overload in AzerothCore: `template<typename... Args> QueryResult Query(std::string const& sql, Args&&... args)`. So my stub must support variadic template. The test doesn't need to actually run this (realmlist lookup in AcceptResult); but the stub compiles. I'll implement a variadic template overload that just ignores format args.
- `Field` with IsNull and Get<uint64/std::string/uint32>.
- `QueryResult` with `->`, `GetFieldCount`, `Fetch`, `NextRow`.
- `LOG_ERROR("module", "{}", message)` — fmt-style logging macro. Stub: variadic macro that formats? For tests, I only need it to compile and be a no-op (or write to cerr). I'll make LOG_* macros expand to ((void)0) with the args swallowed. But some calls pass a std::string as message and format args. E.g. `LOG_INFO("module", "mod-realm-config: loaded SQL configuration; generated {} with {} addons and {} patches", _lastTarget.string(), ...)`. A `((void)0)` macro swallowing multiple args of different types works in C++? `#define LOG_INFO(a, ...) ((void)0)` — yes, variadic arguments after the macro's first param are ignored. Since `LOG_INFO(fmt, ...)` uses `...`, it's fine.

But wait, AzerothCore's LOG_* macros take (type, fmtstr, args...). `((void)0)` works.

- `WorldScript` base class with virtuals `OnStartup`, `OnAfterConfigLoad`, `OnUpdate`. Stub: class WorldScript { public: WorldScript(char const* name) {} virtual ~WorldScript() = default; virtual void OnStartup() {} virtual void OnAfterConfigLoad(bool) {} virtual void OnUpdate(uint32) {} };
- ScriptMgr — Addmod_realm_configScripts at the end doesn't use anything else.
- World.h stub: probably empty.
- Realm.h: `Realm realm;` global with `.Name`, `.Id.Realm`. Need `uint32` alias here.

Let me define common type aliases: In AzerothCore, `uint32`, `uint64`, `int32` etc. are global typedefs (from Define.h). The production uses `uint32` (in `_interval`, `_remaining`, `OnUpdate(uint32 diff)`), `uint32_t` (std), `uint32 owner`? Let me check: uses `uint32` in class members and OnUpdate. Also `static_cast<uint32>(std::stoul(seconds))`. And `std::uint64_t _epoch`. Also `UINT32_MAX` from <cstdint> (that's fine).

I'll define `typedef unsigned int uint32; typedef unsigned long long uint64;` in a stub header or in each. Careful about collisions with system typedefs — on Linux glibc sometimes defines uint32/uint64 only in some contexts. Actually not globally. That's fine to define.

Now QueryResult stub design, full:

```cpp
class QueryResult
{
public:
    using Row = std::vector<Field>;
    QueryResult() = default;
    explicit QueryResult(std::vector<Row> rows)
        : mRows(new std::vector<Row>(std::move(rows))) {}
    explicit operator bool() const { return static_cast<bool>(mRows); }
    QueryResult& operator->() const { return *const_cast<QueryResult*>(this); }
    std::uint32_t GetFieldCount() const
    {
        if (!mRows) return 0;
        auto idx = std::min(mIndex, mRows->size());
        return idx < mRows->size() ? static_cast<std::uint32_t>((*mRows)[idx].size()) : 0;
    }
    Field* Fetch()
    {
        if (!mRows || mIndex >= mRows->size()) return nullptr;
        return (*mRows)[mIndex].data();
    }
    bool NextRow() { if (!mRows) return false; if (mIndex + 1 >= mRows->size()) return false; ++mIndex; return true; }
private:
    std::shared_ptr<std::vector<Row>> mRows;  // hmm, or unique_ptr
    std::size_t mIndex = 0;
};
```

Wait, but in AzerothCore, `QueryResult` when there are zero result rows—does Fetch() return nullptr? For our tests it matters only for injected data. For the `realmlist` query in AcceptResult, our tests might set LoginDatabase to return a specific result. But I'm not going to exercise AcceptResult via LoginDatabase since I'll test functions directly. Keep the stub simple and self-consistent.

Hmm, actually, wait. Let me reconsider whether I should exercise `RealmConfigWorldScript::AcceptResult` at all. It's private. The meaty tests revolve around LoadMetadata, SynthesizeRealmContent, ValidateSettings, BuildConfiguration, ReadContentContract, MetadataSQL, PublishConfiguration. PublishConfiguration writes to a temp dir — testable. AcceptResult's try/catch/ReportFailure is the fail-safe mechanism; I can test the underlying throw behavior via the functions it calls. I'll cover the important safety semantics at function level.

Now, another important aspect: the test must exercise `LoadMetadata` with a metadata result. I need to construct a QueryResult with 22 columns per row, in the exact UNION order (record_type, sort_order, record_key, f0..f18). Fields can be constructed from std::string / uint64. Since the query output is all strings (via CONVERT), the loader does Get<std::string> on everything except... wait, in LoadMetadata, all reads are Get<std::string>() except type strings and record_key — all strings. So my Field stub Get<std::string> must convert the stored value. But also the contract fields use Get<uint64>. So Field needs to store the value and specialize.

Let me design Field to store a std::string and a kind; Get<std::string> returns it; Get<uint64_t>/Get<uint32_t> parse. In the test, metadata fields are all strings, so construction from std::string is enough. Contract fields: I'll construct from uint64. Field(std::uint64_t) stores numeric string like "1".

Now, how does the test build rows of 22 fields? Helper to make a row: `Row({Field("0"), Field("0"), Field(""), Field("..."), ...})`. Tedious but straightforward.

Now, an important question: will including the production cpp compile cleanly with the stubs? Let me mentally review potential issues:

1. `bool withContent` in OnUpdate callback capture — I'm changing to ContentContract.
2. `_snapshotContent = ReadContentContract(result);` — ReadContentContract now returns ContentContract. Good.
3. `MetadataSQL(withContent)` → MetadataSQL(contract). Good.
4. `std::optional<ContentContract> _snapshotContent;` — ContentContract default ctor fine.
5. In the anonymous namespace `struct ContentContract { bool content=false; bool requirements=false; };` — fine.

Also — `#include <map>` needed for std::map in LoadMetadata.

Now, the test file needs to #include "../src/mod_realm_config.cpp". But it also must provide `Addmod_realm_configScripts` isn't called. The .cpp defines it; including is fine (no main). The test provides main().

Compile: `g++ -std=c++17 -I tests/api -I src -Wall tests/mod_realm_config_tests.cpp -o /tmp/build/realm_config_tests`. But wait, `#include "mod_realm_config.cpp"`? The test includes the cpp via relative path. Actually, cleaner: the test file is a .cpp that `#include "../src/mod_realm_config.cpp"` at the top? That includes the production code into the test TU. Then main() runs. Let me do that: `#include "../src/mod_realm_config.cpp"` — but the production file's includes ("Config.h" etc.) must resolve to stubs. With `-I tests/api` the stubs resolve first. The test file itself also defines tests using functions from the anonymous namespace — accessible since same TU.

One catch: the production cpp ends with `new RealmConfigWorldScript();` inside `Addmod_realm_configScripts()` — not called. The WorldScript constructor just sets name. Fine.

Another catch: `Field` in my stub uses `Get<uint64>()`. In AzerothCore, `uint64` is typically `unsigned long long` or `uint64_t`. `Get<uint64>` and `Get<uint64_t>` — production uses `fields[0].Get<uint64>()`. So my stub must define `uint64` alias and specialize `Get`. And `uint32`. Production uses `Get<uint32>` only in content-manager, not here. Here: `Get<uint64>` (contract) and `Get<std::string>`. Also `fields[0].Get<uint64>()`. Good. uint32 usage in this file: `static_cast<uint32>`, `uint32 _interval`, `OnUpdate(uint32 diff)`. So `uint32` type alias needed.

Let me define in a stub "Define.h"-like header. I'll put aliases in a small header `types.h` and include from each stub. Or define in each. I'll create `tests/api/` stubs:

Actually, the includes in production: "Config.h", "DatabaseEnv.h", "Field.h", "QueryResult.h", "Log.h", "ScriptMgr.h", "World.h", "Realm.h". I'll create all eight stub headers plus a shared `Common.h` with type aliases and base objs. Hmm, but actually simpler: put shared stuff in one header included by stubs.

Let me now also reconsider `WorldDatabase` and `LoginDatabase` global objects for the stub — used in AcceptResult/ReloadSettings/OnUpdate. They need `Query(sql)` and `AsyncQuery(sql).WithCallback(...)`. For test compilation, I just need the declarations. For behavioral tests that need them (none currently, since I test pure functions), defaults can throw or return null. I'll make them return nullptr QueryResult (and AsyncQuery return object where WithCallback returns QueryCallback). That's simplest and consistent: `WorldDatabase.Query` returns null → ReadContentContract(null) throws in ReloadSettings → ReportFailure. But tests won't invoke ReloadSettings.

Wait, actually, there IS an issue. For tests of the pipeline, I might want to test `AcceptResult`. But simpler: I'll add a public test hook only if needed. Let me decide the test scope first, then see.

Let me define the test list concretely (a single C++ test program with a small assertion framework). Given 14 numbered requirements, I'll write a test per logical item. No external test framework (no gtest available presumably). Use assert-like helpers printing pass/fail counts.

Tests:
T1 (existing generation valid): Build a Settings with a typical config (addons, patches, content) → BuildConfiguration → assert contains all sections, SchemaVersion=1, no trailing whitespace, ends with '\n', parses (no missing keys). Basically a golden full-output comparison? Full golden is brittle; I'll assert structure + key presence and specific [Client] lines.

T2 (existing [Client] keys unchanged): assert output contains "Version=3.3.5a", "Build=12340", "Executable=Wow.exe", "ExecutableSHA256=", "RuntimeMode=Legacy", "Requirements=" in the [Client] section, and no other [Client] keys. I'll extract the [Client] section and compare against expected line list. That gives strong guarantee.

T3 (no content manager): SynthesizeRealmContent(settings, false, realmName) == "unavailable"; BuildConfiguration Output containing "Requirements="; and settings.ClientRequirements empty; no realm-content patch added.

T4 (legacy build zero req rows): ActiveBuilds = {ACTIVE build with empty requirements}, content_base_url set, realm name matches → patch added; Requirements= in output.

T5 (schema-3 build protected-framexml): build requirements = {protected-framexml} → "Requirements=protected-framexml".

T6 (multiple): requirements {"banana","zesty","apple"} → output "Requirements=apple,banana,zesty" (sorted). Actually content manager already returns sorted; but LoadMetadata dedups/sorts via set. Test both BuildConfiguration join and LoadMetadata normalization.

T7 (no duplicates): requirements {"b","a","a","c"} → "Requirements=a,b,c"; also LoadMetadata with duplicate requirement rows (two type '4' rows same requirement) → dedup to one.

T8 (requirements from ACTIVE build): LoadMetadata with requirement rows referencing the ACTIVE build → attached to build. And SynthesizeRealmContent copies them. Also assert MetadataSQL(contract with requirements) SQL filters to ACTIVE: assert SQL contains "content_manager_build_client_requirement" and "state = 'ACTIVE'"; and MetadataSQL(contract with content-only) does NOT contain the requirement table (legacy/absent → no requirement lookup). This tests "from ACTIVE build, not package state".

T9 (foreign realm): ActiveBuilds build.RealmName != realm.Name → SynthesizeRealmContent returns "different realm; not advertised"; no patch; requirements not attached (settings.ClientRequirements empty). Output "Requirements=".

T10 (lookup failure): 
- ReadContentContract(null) throws.
- ReadContentContract with incompatible requirement columns count → throws.
- LoadMetadata with requirement row referencing a nonexistent build → throws (fail closed).
- LoadMetadata(null result) throws.
- LoadMetadata wrong field count → throws.
Assert that in each case the exception is thrown (i.e., publication would fail safely, preserving last known-good file — mirroring AcceptResult/ReportFailure behavior).

T11 (RuntimeMode=Legacy unchanged): ClientRuntimeMode="Legacy" → "RuntimeMode=Legacy".
T12 (RuntimeMode=Isolated unchanged): "Isolated" → "RuntimeMode=Isolated".
T13 (Addons/Patches unchanged): golden: settings with addons+patches+content build → output compared to expected string including existing format; assert the only new line is Requirements= in [Client]. I'll build a full expected string and compare output == expected. That's the strongest. But deterministic formatting makes golden feasible. Given the template, I'll craft the expected string carefully. Actually a full golden may be brittle if I make a mistake; but it's the definitive check for #1/#13. Let me do a partial golden: compare the [Client] section exactly, and separately assert Addon.*/Patch.* sections contain expected lines (or use the full golden and verify by running). I'll attempt a full golden and adjust by running the test.

T14 (regression): existing tests — none. The golden covers it.

Also worth testing: PublishConfiguration round-trip (writes file with expected content to temp dir, atomic rename, correct filename). The task's docs mention the publisher. I'll add a test: write to temp dir, read back, compare content, filename == "<realmkey>.realm.conf".

Also test MetadataSQL does NOT emit '4' when requirements unavailable, and DOES when available; and does NOT emit '3' when content absent.

Also ReadContentContract happy paths: absent build table → {false,false}; build present + requirement absent → {true,false}; both present → {true,true}.

Now, the run.sh script (as documented in README): compiles tests/api stubs + test, runs, prints summary, exit non-zero on failure. Also maybe runs a Python SQL-snapshot test? The README says "SQL snapshot tests adapt MySQL text casts to SQLite". That's a big lift (sqlite3 dependency). Let me check if sqlite3 python is available. Actually, do I need it? The task says "Add focused tests using the repository's existing test approach." I'll implement the C++ standalone suite (the main documented approach). A SQL snapshot test using SQLite would strengthen confidence that the actual SQL strings execute. Let me check python3/sqlite3 availability and MySQL text convert syntax (CONVERT(x USING utf8mb4) COLLATE — SQLite doesn't support that natively). The README says the SQL snapshot tests "adapt MySQL text casts to SQLite" — meaning the test rewrites the SQL. That's aspirational; implementing a SQLite adapter for the UNION query is moderate effort but valuable: it would execute MetadataSQL against an in-memory SQLite DB with the schema and verify the parser/contract end-to-end. However, the unions use MySQL-specific `CONVERT(... USING utf8mb4) COLLATE ...` and subselects; SQLite can't run them. Writing an adapter that strips these is doable but might distort what we're testing.

Let me weigh effort. The core value is in the C++ tests. A Python-based meta-test asserting the generated SQL text (what the C++ code produces) is meaningful for #8 (ACTIVE filter) and for contract correctness. I could do that by having the C++ test dump MetadataSQL output and verifying in C++ itself — no Python needed. I'll keep everything in C++.

But the README documents "bash tests/run.sh with C++17 g++ (or $CXX) and Python 3." I'll write run.sh to compile+run the C++ test suite, and optionally a Python smoke test if python3 exists. Given there was never a test suite, I control the shape. I'll implement:
- tests/api/*.h (stubs)
- tests/mod_realm_config_tests.cpp (main suite)
- tests/run.sh (compiler + runner)
- Update README to reflect the actual test suite location/shape (currently references "bash tests/run.sh" which matches!). Good — the README already says `bash tests/run.sh`; I'll keep that and adjust the description if needed. Actually I'll update the README "Focused tests" section to accurately describe the suite.

Let me also make sure the production build (real AzerothCore compilation) won't break due to my changes: The changed functions are only in mod_realm_config.cpp. No new headers beyond <map>. Signature changes are internal. Good.

Wait — one more: `ContentContract` return by value is copyable/movable; stored in optional; captured by value in lambdas. All fine.

Also careful: anonymous-namespace struct `ContentContract` in the cpp used in member `std::optional<ContentContract> _snapshotContent;` fine.

Now, let me also handle: `AcceptResult` currently takes `bool withContent`. I'll change to `ContentContract const& contract`. Inside: `auto contentStatus = SynthesizeRealmContent(settings, contract.content, realm.Name);`.

Also the OnUpdate: after `_snapshotContent` is read, `bool withContent = *_snapshotContent;` becomes `ContentContract contract = *_snapshotContent;`.

Now about `_snapshotContent` optional<> containing a struct — copy OK.

Let me also double check `MetadataSQL` for the '4' append: 18 empties (f1..f18). Let me count in code:
```cpp
if (contract.requirements)
{
    sql += R"SQL(
UNION ALL
SELECT CONVERT('4' USING utf8mb4) COLLATE utf8mb4_unicode_ci, 0,
       CONVERT(requirement USING utf8mb4) COLLATE utf8mb4_unicode_ci,
       CONVERT(CAST(build_number AS CHAR) USING utf8mb4) COLLATE utf8mb4_unicode_ci
)SQL";
    for (unsigned i = 1; i < 19; ++i)  // f1..f18 fills the remaining 18 columns
        sql += ", CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci";
    sql += " FROM content_manager_build_client_requirement WHERE build_number IN "
           "(SELECT build_number FROM content_manager_build WHERE BINARY state = 'ACTIVE')\n";
}
```
Count check: total columns = record_type + sort_order + record_key + f0..f18 (19) = 22. My '4' SELECT provides record_type, sort_order, record_key(requirement), f0(build_number) = 4 columns, then loop i=1..18 → 18 empties = f1..f18. Total 4+18=22. ✓.

For '3' append, record_type, sort_order, record_key(''), f0..f3 (4 values), then loop i=4..18 → 15 empties. Total = 3+4+15=22 ✓.

Now LoadMetadata field-count check remains 22.

Now let me write the code. First, plan the Settings/struct changes:

```cpp
struct ActiveContentBuild
{
    std::string Number, RealmName, FileName, SHA256;
    std::vector<std::string> ClientRequirements;
};
struct Settings
{
    ...
    std::vector<std::string> ClientRequirements;  // published when ACTIVE build is advertised
};
```

LoadMetadata parsing:
```cpp
else if (type == "4")
    buildRequirements[fields[3].Get<std::string>()].insert(fields[2].Get<std::string>());
```
where `std::map<std::string, std::set<std::string>> buildRequirements;` declared before loop. After loop:
```cpp
for (auto& build : settings.ActiveBuilds)
{
    auto it = buildRequirements.find(build.Number);
    if (it != buildRequirements.end())
        build.ClientRequirements.assign(it->second.begin(), it->second.end());
}
```
And orphan detection: if a key in buildRequirements isn't in ActiveBuilds → throw. Let me add:
```cpp
for (auto const& entry : buildRequirements)
{
    bool matched = false;
    for (auto const& build : settings.ActiveBuilds)
        if (build.Number == entry.first) { matched = true; break; }
    if (!matched) throw std::runtime_error("Content Manager client requirement references a build that is not ACTIVE in this snapshot");
}
```
Wait, but entries with empty sets (no requirements parsed) won't be in the map at all since we only insert on a '4' row. So `entry.first` always corresponds to some '4' rows. If unmatched → inconsistent snapshot → throw. This also nicely covers "lookup failure cannot masquerade as no requirements": if requirement rows exist but don't associate, we throw rather than silently ignoring. 

Hmm, wait — but is throwing here safe for the foreign-realm scenario? Foreign ACTIVE build + its requirements would match (build is in ActiveBuilds, number matches) → no throw, attaches requirements → SynthesizeRealmContent returns early → never advertised. Good.

Now ValidateSettings addition:
```cpp
void ValidateClientRequirements(Settings const& settings)
{
    for (std::size_t i = 0; i < settings.ClientRequirements.size(); ++i)
    {
        try { ValidateClientRequirement(settings.ClientRequirements[i]); }
        catch (std::exception const& error)
        {
            throw std::runtime_error("Client.Requirements entry #" + std::to_string(i + 1) + ": " + error.what());
        }
    }
}
```
Hmm, but this validates only the attached (advertised) requirements. Wait — I want validation for advertised requirements. But the attached requirements came from LoadMetadata (already deduped/sorted). ValidateSettings calls this. But should the validation live at attach time or validate-time? Validate-time keeps the separation (SynthesizeRealmContent synthesizes; ValidateSettings validates; consistent with current addon/patch handling). Order in AcceptResult: SynthesizeRealmContent (attaches) → ValidateSettings (validates requirements + catalogs) → BuildConfiguration (emits). 

So `ValidateClientRequirement(value)`:
```cpp
void ValidateClientRequirement(std::string const& value)
{
    if (value.empty() || value.size() > 64 ||
        !std::all_of(value.begin(), value.end(), [](unsigned char c)
        { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') || c == '.' || c == '_' || c == '-'; }))
        throw std::runtime_error("client requirement must contain 1..64 ASCII letters, digits, dots, underscores or hyphens");
}
```

Then BuildConfiguration uses `JoinClientRequirements(settings.ClientRequirements)`:
```cpp
std::string JoinClientRequirements(std::vector<std::string> const& requirements)
{
    std::string joined;
    for (std::size_t i = 0; i < requirements.size(); ++i)
    {
        if (i) joined += ',';
        joined += requirements[i];
    }
    return joined;
}
```
Deterministic (input already sorted+deduped). If further defensive normalization wanted, we could dedupe/sort here, but that duplicates work; the guarantee comes from set-based normalization at load. To be extra safe and match the "defensive normalization may deduplicate/sort" note, I could make JoinClientRequirements robust: copy, sort+unique, then join. That protects against any future path that sets an unsorted vector. Let me do that — defensive, cheap, and makes output deterministic regardless of upstream order:

```cpp
std::string JoinClientRequirements(std::vector<std::string> requirements)
{
    std::sort(requirements.begin(), requirements.end());
    requirements.erase(std::unique(requirements.begin(), requirements.end()), requirements.end());
    std::string joined;
    for (std::size_t i = 0; i < requirements.size(); ++i)
    {
        if (i) joined += ',';
        joined += requirements[i];
    }
    return joined;
}
```

Good. This also makes tests T6/T7 trivial at the serializer level.

Now SynthesizeRealmContent — where exactly to attach:
```cpp
    auto url = settings.ContentBaseURL;
    while (url.back() == '/') url.pop_back();
    url += "/" + build.FileName;
    settings.Patches.push_back({...});
    // NEW
    settings.ClientRequirements = build.ClientRequirements;
    return "available; ACTIVE realm content build " + number;
```
Place after push (so return flows). Good.

Now the detection query and ReadContentContract revisions.

ContentContractQuery:
```cpp
constexpr char ContentContractQuery[] = R"SQL(
SELECT
    (SELECT COUNT(*) FROM information_schema.TABLES
     WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build'),
    (SELECT COUNT(*) FROM information_schema.COLUMNS
     WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build'
       AND COLUMN_NAME IN ('build_number', 'realm_name', 'filename', 'sha256', 'state')),
    (SELECT COUNT(*) FROM information_schema.TABLES
     WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build_client_requirement'),
    (SELECT COUNT(*) FROM information_schema.COLUMNS
     WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build_client_requirement'
       AND COLUMN_NAME IN ('build_number', 'requirement'))
)SQL";
```

ReadContentContract:
```cpp
struct ContentContract { bool content = false; bool requirements = false; };

ContentContract ReadContentContract(QueryResult const& result)
{
    if (!result || result->GetFieldCount() != 4)
        throw std::runtime_error("Content Manager contract detection failed; check database permissions/logs");
    Field* fields = result->Fetch();
    if (fields[0].IsNull() || fields[1].IsNull() || fields[2].IsNull() || fields[3].IsNull())
        throw std::runtime_error("unexpected Content Manager contract detection result");
    ContentContract contract;
    if (fields[0].Get<uint64>() == 0) return contract;
    if (fields[0].Get<uint64>() != 1 || fields[1].Get<uint64>() != 5)
        throw std::runtime_error("incompatible content_manager_build contract; expected build_number, realm_name, filename, sha256 and state");
    contract.content = true;
    if (fields[2].Get<uint64>() == 0) return contract;  // pre-Schema-3 builds have no requirement rows
    if (fields[2].Get<uint64>() != 1 || fields[3].Get<uint64>() != 2)
        throw std::runtime_error("incompatible content_manager_build_client_requirement contract; expected build_number and requirement");
    contract.requirements = true;
    return contract;
}
```

Hmm wait — the existing code distinguishes the "table count 0" from "column count mismatch". For build: if fields[0]==0 → no content manager. if fields[0]!=1 → multiple build tables (impossible) → throw. Then fields[1]!=5 → throw. Keep that. For requirements: if fields[2]==0 → legacy → {content=true, requirements=false}. if fields[2]!=1 → throw. fields[3]!=2 → throw.

Now MetadataSQL(bool→ContentContract). Update call sites:
- OnUpdate: `_pending.emplace(WorldDatabase.AsyncQuery(MetadataSQL(contract)).WithCallback(...))`.
- ReloadSettings: `auto contract = ReadContentContract(WorldDatabase.Query(ContentContractQuery)); AcceptResult(WorldDatabase.Query(MetadataSQL(contract)), contract);`

AcceptResult signature: `void AcceptResult(QueryResult const& result, ContentContract const& contract)`; uses `contract.content` for SynthesizeRealmContent.

Now — do I need to keep ReadContentContract's comment about "A query failure must not be mistaken for absence"? Yes.

Now the type of `_snapshotContent`: `std::optional<ContentContract>`.

Now, about the constructor call inside LoadMetadata for ActiveBuilds push_back: struct now has 5 members; aggregate init with 4 strings leaves ClientRequirements default. `settings.ActiveBuilds.push_back({value(0), value(1), value(2), value(3)});` — with a 4-element init list and a struct with 5 members (last has default member initializer), aggregate init works: remaining member uses default. Yes, C++ aggregate init allows fewer initializers if remaining members have default member initializers. `std::vector<std::string> ClientRequirements;` — default constructor, not a default member initializer but empty vector is still "default-initialized"? For aggregate init with missing initializers, remaining members are initialized from their default member initializer if present, else value-initialized. A vector without DMi is value-initialized → empty vector. Wait, actually for aggregates, if there are fewer initializers than members, the remaining members are "copy-initialized from an empty list" (value-initialized), which for a class type with an implicit default constructor means default construction. So `std::vector<std::string>` value-initialized = empty. Good, that works in C++17.

Let me double check the existing push_backs for addon and patch still compile: Addon has 7 strings, init list has 7 → fine. Patch has 9 members (last two defaulted: InstallMode default member initializer and... wait Patch struct: Key, Name, Requirement, SourceType, SourceURL, FileName, InstallDirectory, SHA256 (8 strings) + InstallMode. push: `settings.Patches.push_back({fields[2]... value(0)...value(6)})` — 8 strings. InstallMode has DMi. Works. Good, unchanged.

Now the test stub header files. Let me carefully draft each.

tests/api/Define.h — not included by production; but I'll make a shared header `AzerothTypes.h`? Actually production includes specific headers. Let me create:

tests/api/Config.h
```cpp
#ifndef CONFIG_STUB_H
#define CONFIG_STUB_H
#include <string>
#include <optional>
#include <stdexcept>
class ConfigMgr
{
public:
    template<class T> T GetOption(std::string const&, T const& def) const { return def; }
};
extern ConfigMgr* sConfigMgr;
#endif
```
Define `ConfigMgr* sConfigMgr;` in test main.

tests/api/Field.h
```cpp
#ifndef FIELD_STUB_H
#define FIELD_STUB_H
#include <string>
#include <cstdint>
#include <stdexcept>
using uint64 = unsigned long long;
using uint32 = unsigned int;
class Field
{
public:
    Field() = default;
    Field(const char* v) : _value(v) {}
    Field(std::string v) : _value(std::move(v)) {}
    Field(std::uint64_t v) : _value(std::to_string(v)) {}
    bool IsNull() const { return _null; }
    template<class T> T Get() const;
private:
    bool _null = false;
    std::string _value;
};
template<> inline std::string Field::Get<std::string>() const { if (_null) throw std::runtime_error("Get on null Field"); return _value; }
template<> inline std::uint64_t Field::Get<std::uint64_t>() const { if (_null) throw ...; return std::stoull(_value); }
template<> inline uint64 Field::Get<uint64>() const { ... }  // alias same as uint64_t; may conflict
template<> inline uint32 Field::Get<uint32>() const { ... }
#endif
```
Wait, `uint64` is `unsigned long long`, `std::uint64_t` is also `unsigned long long` typically. Two explicit full specializations with same underlying type → redefinition error! Better: don't alias `uint64`/`uint32` to distinct types; provide `using` aliases that are identical to std types, but then Get<uint64> and Get<std::uint64_t> are the same specialization → duplicate definition. So I should NOT define both. Production uses `Get<uint64>` only. So define `using uint64 = std::uint64_t; using uint32 = std::uint32_t;` and only specializations for std::uint64_t and std::string, plus uint32_t. Then `Field::Get<uint64>` resolves to `Field::Get<std::uint64_t>` (alias) → uses the std::uint64_t specialization. Good. Specialize:
- Get<std::string>
- Get<std::uint64_t>
- Get<std::uint32_t>

Get<std::string> is called on all metadata fields. Since Field stores value as string, return it.

Note `uint32` needed for `Get<uint32>`? Only in content-manager code; mod-realm-config uses Get<uint64> and Get<std::string>. But the WorldScript uses `uint32 _interval` and `OnUpdate(uint32)`. So `uint32` alias needed. Define both aliases in a common types header included by Field.h or in each header. To avoid multiple definition, use header guards. I'll create `tests/api/Common.h` with uint aliases and include it where needed. But production includes only the eight named headers; each stub can include "Common.h". Fine.

Wait, but if `uint64` is defined as `std::uint64_t` alias in global namespace, is there a conflict with any system header? glibc's sys/types.h doesn't define uint64. Should be fine. Also `uint32` used as `uint32 diff` param.

Now tests/api/QueryResult.h
```cpp
#ifndef QUERYRESULT_STUB_H
#define QUERYRESULT_STUB_H
#include "Field.h"
#include <vector>
#include <memory>
#include <algorithm>
class QueryResult
{
public:
    using Row = std::vector<Field>;
    QueryResult() = default;
    explicit QueryResult(std::vector<Row> rows) : _rows(new std::vector<Row>(std::move(rows))), _index(0) {}
    explicit operator bool() const { return static_cast<bool>(_rows); }
    QueryResult const& operator->() const { return *this; }
    QueryResult& operator->() { return *this; }
    std::uint32_t GetFieldCount() const
    {
        if (!_rows || _index >= _rows->size()) return 0;
        return static_cast<std::uint32_t>((*_rows)[_index].size());
    }
    Field* Fetch()
    {
        if (!_rows || _index >= _rows->size()) return nullptr;
        return (*_rows)[_index].data();
    }
    bool NextRow()
    {
        if (!_rows || _index + 1 >= _rows->size()) return false;
        ++_index; return true;
    }
private:
    std::shared_ptr<std::vector<Row>> _rows;
    std::size_t _index = 0;
};
#endif
```
Hmm, but `QueryResult const&` parameter then `result->Fetch()`. `operator->` returns `*this` (QueryResult&), then `.Fetch()` on it. For const usage `result->Fetch()` where result is const ref: `operator->() const` returns `QueryResult const&`, then Fetch() is non-const → compile error! Because `Fetch()` is non-const (returns pointer into internal storage; doesn't modify though). Let me make Fetch() const. GetFieldCount const. NextRow non-const.

Make `Field* Fetch() const;` — returns pointer to current row data; const correctness OK since it returns non-const pointer (mutable state internally). Then both const and non-const operator-> work.

Also AzerothCore `QueryResult const& result; result->GetFieldCount()` — yes const. And `LoadMetadata(QueryResult const&)`. And in the do-while: `result->NextRow()` — NextRow must be callable on const ref → must be const too! LoadMetadata takes `QueryResult const& result` and calls `result->NextRow()`. So NextRow() must be const (mutates via mutable). Make `NextRow() const` with `_index` mutable. Simplest: mark `mutable std::size_t _index`. Then const methods can modify it.

Let me finalize QueryResult:
```cpp
class QueryResult {
public:
    using Row = std::vector<Field>;
    QueryResult() = default;
    explicit QueryResult(std::vector<Row> rows) : _rows(new std::vector<Row>(std::move(rows))) {}
    explicit operator bool() const { return static_cast<bool>(_rows); }
    QueryResult const& operator->() const { return *this; }
    std::uint32_t GetFieldCount() const { if (!_rows || _index >= _rows->size()) return 0; return static_cast<std::uint32_t>((*_rows)[_index].size()); }
    Field* Fetch() const { if (!_rows || _index >= _rows->size()) return nullptr; return (*_rows)[_index].data(); }
    bool NextRow() const { if (!_rows || _index + 1 >= _rows->size()) return false; ++_index; return true; }
private:
    std::shared_ptr<std::vector<Row>> _rows;
    mutable std::size_t _index = 0;
};
```

Now DatabaseEnv.h:
```cpp
#ifndef DATABASEENV_STUB_H
#define DATABASEENV_STUB_H
#include "QueryResult.h"
#include <string>
class QueryCallback
{
public:
    template<class Callback> QueryCallback& WithCallback(Callback&&) { return *this; }
};
class DatabaseWorkerPool
{
public:
    QueryResult Query(std::string const&) { return QueryResult(); }
    template<class... Args> QueryResult Query(std::string const&, Args&&...) { return QueryResult(); }
    QueryCallback AsyncQuery(std::string const&) { return QueryCallback(); }
};
extern DatabaseWorkerPool WorldDatabase;
extern DatabaseWorkerPool LoginDatabase;
#endif
```
Define WorldDatabase/LoginDatabase in test main.

Wait — the QueryCallback: production does `WorldDatabase.AsyncQuery(...).WithCallback(callback)`. WithCallback returns QueryCallback& or QueryCallback. `_pending.emplace(...)` where `_pending` is `std::optional<QueryCallback>`. So AsyncQuery returns QueryCallback, WithCallback returns QueryCallback by value? `_pending = _pending->InvokeIfReady()` pattern: `_pending->InvokeIfReady()` — InvokeIfReady() must be a method on QueryCallback. Let me look at how production uses _pending:

```
if (_pending && _pending->InvokeIfReady()) _pending.reset();
_pending.emplace(WorldDatabase.AsyncQuery(MetadataSQL(contract)).WithCallback(...));
```

So QueryCallback needs:
- `bool InvokeIfReady()` — returns whether ready. It would invoke the stored callback if ready. For stub, return true always? If true, it resets _pending each OnUpdate. The stored callback never fires in tests (no async engine). That's fine for compilation; these paths aren't behaviorally tested.

Actually `InvokeIfReady()` is a real AzerothCore QueryCallback method. Stub: `bool InvokeIfReady() { return false; }` — hmm. The intent: when ready, runs callback and returns true. For stub, return false (never ready) → _pending stays populated. Whatever; not behaviorally tested.

- `WithCallback` should store the callback and return *this (or a copy). For stub: store into a std::function, return *this.

```cpp
class QueryCallback {
public:
    template<class C> QueryCallback& WithCallback(C&& cb) { _cb = std::forward<C>(cb); return *this; }
    bool InvokeIfReady() { if (_cb) { _cb(QueryResult()); return false; } return false; }
private:
    std::function<void(QueryResult)> _cb;
};
```
But QueryResult copy: `_cb(QueryResult())` needs QResult copyable in std::function. My QueryResult holds shared_ptr → copyable. But callback signature `void(QueryResult)` — AcceptResult takes `QueryResult result` by value. OK. Include <functional>. Hmm — InvokeIfReady being const? production calls `_pending->InvokeIfReady()` on non-const optional-ref. Fine non-const.

Wait, actually — the callback captured: `[this, epoch, contract](QueryResult result) { ... AcceptResult(result, contract); }`. My WithCallback templated stores std::function<void(QueryResult)>. Good.

Actually, do I even need InvokeIfReady behavior? Just compile. Keep simple:
```cpp
bool InvokeIfReady() { return false; }
```
No storing needed. But WithCallback must return a QueryCallback that can be emplaced. Simplest QueryCallback default. But WithCallback returning `*this` requires an object; AsyncQuery returns a temporary, WithCallback on it → returns ref to temp → dangling. Store in a member: make AsyncQuery return QueryCallback, WithCallback returns *this (ref). Temporarily lives until `.emplace` copies it into _pending. `_pending.emplace(AsyncQuery(...).WithCallback(...))` — WithCallback() is called on the temporary rvalue, returns QueryCallback& to the temporary; emplace copies it. Fine within the full expression. Good.

Now Log.h:
```cpp
#ifndef LOG_STUB_H
#define LOG_STUB_H
#define LOG_ERROR(category, ...) ((void)0)
#define LOG_WARN(category, ...) ((void)0)
#define LOG_INFO(category, ...) ((void)0)
#define LOG_DEBUG(category, ...) ((void)0)
#endif
```
Wait production calls `LOG_ERROR("module", "message...{}", var)` and also `LOG_ERROR("module", "mod-realm-config: invalid RealmConfig.Client.RuntimeMode; ...")` — single-string without format args. With `#define LOG_ERROR(category, ...) ((void)0)` variadic handles both. Good.

But careful: `LOG_ERROR("module", "mod-realm-config: publication failed; existing realm.conf preserved: {}", message);` — message is a std::string; the macro swallows. OK.

ScriptMgr.h stub:
```cpp
#ifndef SCRIPTMGR_STUB_H
#define SCRIPTMGR_STUB_H
#include "Common.h"
class WorldScript {
public:
    explicit WorldScript(char const* name) : _name(name) {}
    virtual ~WorldScript() = default;
    virtual void OnStartup() {}
    virtual void OnAfterConfigLoad(bool /*reload*/) {}
    virtual void OnUpdate(uint32 /*diff*/) {}
private: std::string _name;
};
#endif
```
Wait — production declares `RealmConfigWorldScript() : WorldScript("RealmConfigWorldScript") { }` and overrides members. Need the constructor base ctor to take char const*. I defined `_name` but maybe unused → warning. Use `(void)_name` or just don't store name. Simplest: `explicit WorldScript(char const* /*name*/) {}`. But there's another derived class in content-manager (`content_manager_worldscript`) not compiled here. Fine.

Hmm, but WorldScript in AzerothCore also has virtuals used by ScriptMgr registry: `Addmod_realm_configScripts` calls `new RealmConfigWorldScript();` — constructor only. But AzerothCore's WorldScript constructor auto-registers via ScriptMgr. Stub doesn't need that.

World.h stub:
```cpp
#ifndef WORLD_STUB_H
#define WORLD_STUB_H
#endif
```
Include guard empty. But production includes "World.h" — empty fine.

Realm.h stub:
```cpp
#ifndef REALM_STUB_H
#define REALM_STUB_H
#include "Common.h"
#include <string>
struct RealmId { uint32 Realm = 0; };
struct Realm { std::string Name; RealmId Id; };
extern Realm realm;
#endif
```
Production: `realm.Id.Realm` and `realm.Name`. My struct RealmId with `Realm` member matches exactly the AzerothCore naming? In AzerothCore, `struct Realm_ID { uint32 RealmID; ... }`? Hmm, let me recall the actual AzerothCore code.

In AzerothCore Realm.h:
```cpp
struct RealmID
{
    uint32 Realm;
    ...
};
struct Realm
{
    ...
    std::string Name;
    RealmID Id;
    ...
};
extern Realm realm;
```
Actually let me recall from Core/Realm/Realm.h in WotLK 3.3.5b core... I believe `realm.Id.Realm` is correct usage (used in many modules). Indeed mod-content-manager uses `realm.Id.Realm` too presumably. Let me verify by searching.

Let me check how mod-content-manager and other modules reference the AzerothCore realm global, to make accurate stubs.

**Tool: bash**

**Input:**
```json
{
  "command": "grep -rn \"realm\\.Id\\.Realm\\|realm\\.Name\" /home/smithkt/git/mod-content-manager/src /home/smithkt/git/mod-realm-config/src"
}
```

**Output:**
```
/home/smithkt/git/mod-content-manager/src/ContentCommands.cpp:203:        if (!ContentAllocationRegistry().Read(realm.Name, rows, error))
/home/smithkt/git/mod-content-manager/src/ContentCommands.cpp:208:        handler->PSendSysMessage("Retained resource allocations for realm {}: {}", realm.Name, rows.size());
/home/smithkt/git/mod-content-manager/src/ContentCommands.cpp:234:                if (build.realmName == realm.Name)
/home/smithkt/git/mod-content-manager/src/ContentCommands.cpp:243:            return row.buildNumber == number && row.realmName == realm.Name;
/home/smithkt/git/mod-content-manager/src/ContentCommands.cpp:249:        if (!ContentServerDeployment::Inspect(number, realm.Name, sContentManager.GetOutputDirectory(),
/home/smithkt/git/mod-content-manager/src/ContentCommands.cpp:274:        if (!ContentServerDeployment::Apply(number, realm.Name,
/home/smithkt/git/mod-content-manager/src/ContentCommands.cpp:606:        if (!ContentPackageLifecycle::Analyse(realm.Name, packageKey, discovered, analysis, error))
/home/smithkt/git/mod-content-manager/src/ContentCommands.cpp:737:        auto result = ContentBuildService().Build(sContentManager, realm.Name,
/home/smithkt/git/mod-content-manager/src/ContentCapabilityProvider.cpp:25:	auto const realmId = realm.Id.Realm;
/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp:477:    // Content Manager records realm.Name verbatim. The public display name in
/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp:539:    auto const realmId = realm.Id.Realm;
/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp:730:            auto contentStatus = SynthesizeRealmContent(settings, withContent, realm.Name);

```

**Tool: bash**

**Input:**
```json
{
  "command": "grep -rn \"Get<uint64>\\|Get<std::string>\\|Get<uint32>\" /home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp"
}
```

**Output:**
```
169:    if (fields[0].Get<uint64>() == 0) return false;
170:    if (fields[0].Get<uint64>() != 1 || fields[1].Get<uint64>() != 5)
214:        auto value = [fields](unsigned i) { return fields[i + 3].Get<std::string>(); };
215:        auto type = fields[0].Get<std::string>();
241:            settings.Addons.push_back({fields[2].Get<std::string>(), value(0), value(1),
244:            settings.Patches.push_back({fields[2].Get<std::string>(), value(0), value(1),
545:        auto name = result->Fetch()[0].Get<std::string>();

```

---

## Assistant (Build · Big Pickle · 15.5s)

_Thinking:_

Confirmed. `realm.Id.Realm` and `realm.Name` are the exact usage patterns. `Get<uint64>` and `Get<std::string>` are the Field template uses.

Let me look at the mod-content-manager's mysql_adapter to see how they handle QueryResult/Field doubles for tests — maybe there's already an established stub pattern I can reuse.

**Tool: bash**

**Input:**
```json
{
  "command": "ls -la /home/smithkt/git/mod-content-manager/tests/mysql_adapter/; head -100 /home/smithkt/git/mod-content-manager/tests/mysql_adapter/*"
}
```

**Output:**
```
total 24
drwxrwxr-x 2 smithkt smithkt 4096 Sep 23 06:39 .
drwxrwxr-x 3 smithkt smithkt 4096 Sep 24 12:06 ..
-rw-rw-r-- 1 smithkt smithkt 3305 Sep 23 06:39 DatabaseEnv.h
-rw-rw-r-- 1 smithkt smithkt   38 Sep 23 06:39 Field.h
-rw-rw-r-- 1 smithkt smithkt   38 Sep 23 06:39 QueryResult.h
-rw-rw-r-- 1 smithkt smithkt   38 Sep 23 06:39 Transaction.h
==> /home/smithkt/git/mod-content-manager/tests/mysql_adapter/DatabaseEnv.h <==
// Test-only synchronous adapter. Exercises production SQL against disposable MySQL;
// it does not simulate AzerothCore's async worker pool or Field metadata checks.
#pragma once
#include <mysql.h>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <stdexcept>
#include <type_traits>
#include <iostream>
using uint32=std::uint32_t;
struct Field
{
    bool null=false; std::string text;
    bool IsNull() const { return null; }
    template<class T> T Get() const
    {
        if constexpr(std::is_same_v<T,std::string>) return text;
        else if constexpr(std::is_signed_v<T>) return static_cast<T>(std::stoll(text));
        else return static_cast<T>(std::stoull(text));
    }
};
struct Result
{
    std::vector<std::vector<Field>> rows;std::size_t index=0;
    Field* Fetch(){return rows[index].data();}
    bool NextRow(){return ++index<rows.size();}
    std::uint64_t GetRowCount() const{return rows.size();}
};
using QueryResult=std::shared_ptr<Result>;
struct Transaction
{
    std::vector<std::string> sql;
    void Append(std::string const& statement){sql.push_back(statement);}
};
class TestDatabase
{
    MYSQL* connection=nullptr;
public:
    std::function<void()> beforeCommit;
    std::string lastError;
    ~TestDatabase(){if(connection)mysql_close(connection);}
    void Connect(char const* socket)
    {
        connection=mysql_init(nullptr);
        if(!mysql_real_connect(connection,"localhost","root","","phase4_test",0,socket,0))
            throw std::runtime_error(mysql_error(connection));
        Execute("SET NAMES utf8mb4 COLLATE utf8mb4_0900_ai_ci");
    }
    bool Execute(std::string const& sql)
    {
        if(mysql_query(connection,sql.c_str())){lastError=mysql_error(connection);return false;}
        if(auto* result=mysql_store_result(connection))mysql_free_result(result);
        return true;
    }
    void DirectExecute(std::string const& sql){if(!Execute(sql))throw std::runtime_error(lastError);}
    QueryResult Query(std::string const& sql)
    {
        if(mysql_query(connection,sql.c_str())){lastError=mysql_error(connection);std::cerr<<lastError<<'\n';return {};}
        auto* result=mysql_store_result(connection);if(!result)return {};
        auto output=std::make_shared<Result>();
        while(auto row=mysql_fetch_row(result))
        {
            auto lengths=mysql_fetch_lengths(result);std::vector<Field> fields;
            for(unsigned i=0;i<mysql_num_fields(result);++i)fields.push_back({!row[i],row[i]?std::string(row[i],lengths[i]):""});
            output->rows.push_back(std::move(fields));
        }
        mysql_free_result(result);return output->rows.empty()?QueryResult():output;
    }
    std::shared_ptr<Transaction> BeginTransaction(){return std::make_shared<Transaction>();}
    void DirectCommitTransaction(std::shared_ptr<Transaction> tx)
    {
        if(beforeCommit){auto fn=std::move(beforeCommit);beforeCommit={};fn();}
        if(!Execute("START TRANSACTION"))throw std::runtime_error(lastError);
        for(auto const& sql:tx->sql)if(!Execute(sql)){std::cerr<<"Transaction rejected: "<<lastError<<'\n';Execute("ROLLBACK");return;}
        if(!Execute("COMMIT"))throw std::runtime_error(lastError);
    }
};
inline TestDatabase WorldDatabase,CharacterDatabase;

==> /home/smithkt/git/mod-content-manager/tests/mysql_adapter/Field.h <==
#pragma once
#include "DatabaseEnv.h"

==> /home/smithkt/git/mod-content-manager/tests/mysql_adapter/QueryResult.h <==
#pragma once
#include "DatabaseEnv.h"

==> /home/smithkt/git/mod-content-manager/tests/mysql_adapter/Transaction.h <==
#pragma once
#include "DatabaseEnv.h"

```

---

## Assistant (Build · Big Pickle · 5.5s)

_Thinking:_

mod-content-manager's tests use real MySQL (test-only synchronous adapter). That's a heavyweight approach requiring a running MySQL. For mod-realm-config, the README describes a standalone suite with "small AzerothCore API doubles" — which is what I'll build (no MySQL dependency).

Now, I have a solid plan. Let me also check whether python3 is available for a potential SQL-snapshot check, and system g++.

Let me confirm toolchain availability and then start implementing.

I have enough understanding. Let me confirm the toolchain and set up a plan.

**Tool: bash**

**Input:**
```json
{
  "command": "which g++ python3 sqlite3 2>&1; g++ --version 2>/dev/null | head -1; python3 --version 2>/dev/null; python3 -c \"import sqlite3; print('sqlite3 ok')\" 2>&1"
}
```

**Output:**
```
/usr/bin/g++
/usr/bin/python3
/usr/bin/sqlite3
g++ (Ubuntu 15.2.0-16ubuntu1) 15.2.0
Python 3.14.4
sqlite3 ok

```

---

## Assistant (Build · Big Pickle · 9.6s)

_Thinking:_

g++ 15.2 and python3 with sqlite3 are available. 

Let me create a Todo list and start implementing.

Plan:
1. Extend the production code (`src/mod_realm_config.cpp`):
   - Add `ContentContract` struct + extend `ContentContractQuery` to 4 columns + update `ReadContentContract`.
   - Extend `MetadataSQL` to include type '4' requirement rows when `contract.requirements`.
   - Extend `ActiveContentBuild` with `ClientRequirements`.
   - Extend `Settings` with `ClientRequirements`.
   - Update `LoadMetadata` to parse type '4' rows, associate to builds with dedup/sort, throw on orphan/unconfirmed.
   - Add `ValidateClientRequirement` + hook into `ValidateSettings`.
   - Update `SynthesizeRealmContent` to attach `ClientRequirements` from the advertised ACTIVE build.
   - Add `JoinClientRequirements` and add `Requirements=` to `[Client]` in `BuildConfiguration`.
   - Update WorldScript plumbing: `_snapshotContent` type, `AcceptResult`, `OnUpdate`, `ReloadSettings`.
2. Create tests:
   - `tests/api/` stubs: Common.h, Config.h, Field.h, QueryResult.h, DatabaseEnv.h, Log.h, ScriptMgr.h, World.h, Realm.h.
   - `tests/mod_realm_config_tests.cpp` covering the 14 required scenarios.
   - `tests/run.sh`.
3. Docs: update README.md (Requirements key, Content Manager integration, tests section), SCHEMA.md (optional content manager advertisement), conf comment if needed.
4. Build & run tests.
5. Verify git diff.

Let me now write the code. First the production file edits.

Let me be precise about each edit.

### Edit 1: includes — add <map>

```cpp
#include <set>
```
Let me add <map> after <set>.

### Edit 2: ActiveContentBuild struct
```cpp
struct ActiveContentBuild
{
    std::string Number, RealmName, FileName, SHA256;
    std::vector<std::string> ClientRequirements;
};
```

### Edit 3: Settings — add ClientRequirements
```cpp
    std::vector<ActiveContentBuild> ActiveBuilds;
    std::vector<std::string> ClientRequirements;
```

### Edit 4: ContentContractQuery — extend to 4 subselects, plus comment update.

### Edit 5: Add ContentContract struct + rewrite ReadContentContract.

### Edit 6: MetadataSQL(bool withContent) → MetadataSQL(ContentContract const& contract).

### Edit 7: LoadMetadata — declare map, parse type "4", post-loop association + orphan throw.

### Edit 8: ValidateClientRequirement + call in ValidateSettings.

### Edit 9: SynthesizeRealmContent attach.

### Edit 10: JoinClientRequirements + BuildConfiguration Client section.

### Edit 11: WorldScript plumbing.

Let me write these edits carefully now. There's one ordering concern: `ContentContract` must be declared before `ReadContentContract` and before `MetadataSQL` definition and before the class members use it. It's in the anonymous namespace near the top. I'll declare `struct ContentContract` right after the `Settings` struct (or before ContentContractQuery). Let me put it before ContentContractQuery.

Let me also update the comment above ContentContractQuery: "A query failure must not be mistaken for absence." — that still applies.

Let me write edit 1-3.

Toolchain is available. Let me set up the task list and begin implementation.

**Tool: todowrite**

**Input:**
```json
{
  "todos": [
    {
      "content": "Extend production code: content-manager contract detection for requirement table",
      "status": "in_progress",
      "priority": "high"
    },
    {
      "content": "Extend production code: MetadataSQL type '4' requirement rows + LoadMetadata association",
      "status": "pending",
      "priority": "high"
    },
    {
      "content": "Add requirement validation, synthesize attach, and [Client] Requirements= emission",
      "status": "pending",
      "priority": "high"
    },
    {
      "content": "Update RealmConfigWorldScript plumbing (snapshot/accept/refresh)",
      "status": "pending",
      "priority": "high"
    },
    {
      "content": "Create tests: API doubles, test suite, run.sh",
      "status": "pending",
      "priority": "high"
    },
    {
      "content": "Update README.md and SCHEMA.md documentation",
      "status": "pending",
      "priority": "medium"
    },
    {
      "content": "Build and run tests; verify deterministic output",
      "status": "pending",
      "priority": "high"
    },
    {
      "content": "Inspect git diff for unrelated changes and safety checklist",
      "status": "pending",
      "priority": "high"
    },
    {
      "content": "Produce ZIP of new/modified files",
      "status": "pending",
      "priority": "medium"
    }
  ]
}
```

**Output:**
```
[
  {
    "content": "Extend production code: content-manager contract detection for requirement table",
    "status": "in_progress",
    "priority": "high"
  },
  {
    "content": "Extend production code: MetadataSQL type '4' requirement rows + LoadMetadata association",
    "status": "pending",
    "priority": "high"
  },
  {
    "content": "Add requirement validation, synthesize attach, and [Client] Requirements= emission",
    "status": "pending",
    "priority": "high"
  },
  {
    "content": "Update RealmConfigWorldScript plumbing (snapshot/accept/refresh)",
    "status": "pending",
    "priority": "high"
  },
  {
    "content": "Create tests: API doubles, test suite, run.sh",
    "status": "pending",
    "priority": "high"
  },
  {
    "content": "Update README.md and SCHEMA.md documentation",
    "status": "pending",
    "priority": "medium"
  },
  {
    "content": "Build and run tests; verify deterministic output",
    "status": "pending",
    "priority": "high"
  },
  {
    "content": "Inspect git diff for unrelated changes and safety checklist",
    "status": "pending",
    "priority": "high"
  },
  {
    "content": "Produce ZIP of new/modified files",
    "status": "pending",
    "priority": "medium"
  }
]
```

---

## Assistant (Build · Big Pickle · 2.0s)

Now let me implement the production code changes. Starting with includes and structs:



**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp",
  "oldString": "#include <atomic>\n#include <chrono>\n#include <cstdint>\n#include <filesystem>\n#include <fstream>\n#include <optional>\n#include <set>\n#include <algorithm>",
  "newString": "#include <atomic>\n#include <chrono>\n#include <cstdint>\n#include <filesystem>\n#include <fstream>\n#include <optional>\n#include <set>\n#include <map>\n#include <algorithm>"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 6.0s)

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp",
  "oldString": "struct ActiveContentBuild\n{\n    std::string Number, RealmName, FileName, SHA256;\n};",
  "newString": "struct ActiveContentBuild\n{\n    std::string Number, RealmName, FileName, SHA256;\n    // Immutable client requirements recorded against this exact build by Content\n    // Manager (Schema 3). Sorted, deduplicated, copied from the ACTIVE row set.\n    std::vector<std::string> ClientRequirements;\n};"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 7.9s)

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp",
  "newString": "std::string ContentBaseURL;\n    std::vector<ActiveContentBuild> ActiveBuilds;\n    // Published semantic client requirements of the ACTIVE build being advertised.\n    // Empty when no owned ACTIVE build is advertised.\n    std::vector<std::string> ClientRequirements;\n    std::vector<Addon> Addons;\n    std::vector<Patch> Patches;",
  "oldString": "std::string ContentBaseURL;\n    std::vector<ActiveContentBuild> ActiveBuilds;\n    std::vector<Addon> Addons;\n    std::vector<Patch> Patches;"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 18.6s)

_Thinking:_

Now the ContentContractQuery and ReadContentContract. Let me add the ContentContract struct before ContentContractQuery and extend it.

Now the content-manager contract detection:



**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp",
  "newString": "// Optional Content Manager integration state discovered per refresh. `content`\n// tracks the build table contract; `requirements` additionally confirms the\n// Schema 3 client requirement table. Absence of the requirement table is a\n// pre-Schema-3 install whose existing builds naturally have no requirement rows.\nstruct ContentContract\n{\n    bool content = false;\n    bool requirements = false;\n};\n\n// COUNT returns a row even when Content Manager is absent. A query failure must\n// not be mistaken for absence. Checking columns also detects incompatible installs.\nconstexpr char ContentContractQuery[] = R\"SQL(\nSELECT\n    (SELECT COUNT(*) FROM information_schema.TABLES\n     WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build'),\n    (SELECT COUNT(*) FROM information_schema.COLUMNS\n     WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build'\n       AND COLUMN_NAME IN ('build_number', 'realm_name', 'filename', 'sha256', 'state')),\n    (SELECT COUNT(*) FROM information_schema.TABLES\n     WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build_client_requirement'),\n    (SELECT COUNT(*) FROM information_schema.COLUMNS\n     WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build_client_requirement'\n       AND COLUMN_NAME IN ('build_number', 'requirement'))\n)SQL\";\n\nContentContract ReadContentContract(QueryResult const& result)\n{\n    if (!result || result->GetFieldCount() != 4)\n        throw std::runtime_error(\"Content Manager contract detection failed; check database permissions/logs\");\n    Field* fields = result->Fetch();\n    if (fields[0].IsNull() || fields[1].IsNull() || fields[2].IsNull() || fields[3].IsNull())\n        throw std::runtime_error(\"unexpected Content Manager contract detection result\");\n    ContentContract contract;\n    if (fields[0].Get<uint64>() == 0) return contract;\n    if (fields[0].Get<uint64>() != 1 || fields[1].Get<uint64>() != 5)\n        throw std::runtime_error(\"incompatible content_manager_build contract; expected build_number, realm_name, filename, sha256 and state\");\n    contract.content = true;\n    if (fields[2].Get<uint64>() == 0) return contract;\n    if (fields[2].Get<uint64>() != 1 || fields[3].Get<uint64>() != 2)\n        throw std::runtime_error(\"incompatible content_manager_build_client_requirement contract; expected build_number and requirement\");\n    contract.requirements = true;\n    return contract;\n}\n\nstd::string MetadataSQL(ContentContract const& contract)\n{\n    std::string sql = MetadataQuery;\n    if (contract.content)\n    {\n        sql += R\"SQL(\nUNION ALL\nSELECT CONVERT('3' USING utf8mb4) COLLATE utf8mb4_unicode_ci, 0,\n       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,\n       CONVERT(CAST(build_number AS CHAR) USING utf8mb4) COLLATE utf8mb4_unicode_ci,\n       CONVERT(realm_name USING utf8mb4) COLLATE utf8mb4_unicode_ci,\n       CONVERT(filename USING utf8mb4) COLLATE utf8mb4_unicode_ci,\n       CONVERT(sha256 USING utf8mb4) COLLATE utf8mb4_unicode_ci\n)SQL\";\n        for (unsigned i = 4; i < 19; ++i)\n            sql += \", CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci\";\n        // Deliberately do not filter by realm or LIMIT 1: multiple ACTIVE rows\n        // anywhere in this world database are ambiguous and must be rejected.\n        sql += \" FROM content_manager_build WHERE BINARY state = 'ACTIVE'\\n\";\n    }\n    if (contract.requirements)\n    {\n        sql += R\"SQL(\nUNION ALL\nSELECT CONVERT('4' USING utf8mb4) COLLATE utf8mb4_unicode_ci, 0,\n       CONVERT(requirement USING utf8mb4) COLLATE utf8mb4_unicode_ci,\n       CONVERT(CAST(build_number AS CHAR) USING utf8mb4) COLLATE utf8mb4_unicode_ci\n)SQL\";\n        for (unsigned i = 1; i < 19; ++i)\n            sql += \", CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci\";\n        // Requirements are immutable build metadata owned by Content Manager.\n        // Only the exact ACTIVE builds' rows are consumed; package/config state\n        // is never inspected here.\n        sql += \" FROM content_manager_build_client_requirement \"\n            \"WHERE build_number IN (SELECT build_number FROM content_manager_build WHERE BINARY state = 'ACTIVE')\\n\";\n    }\n    sql += \"ORDER BY record_type, sort_order, BINARY record_key\";\n    return sql;\n}",
  "oldString": "// COUNT returns a row even when Content Manager is absent. A query failure must\n// not be mistaken for absence. Checking columns also detects incompatible installs.\nconstexpr char ContentContractQuery[] = R\"SQL(\nSELECT\n    (SELECT COUNT(*) FROM information_schema.TABLES\n     WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build'),\n    (SELECT COUNT(*) FROM information_schema.COLUMNS\n     WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build'\n       AND COLUMN_NAME IN ('build_number', 'realm_name', 'filename', 'sha256', 'state'))\n)SQL\";\n\nbool ReadContentContract(QueryResult const& result)\n{\n    if (!result || result->GetFieldCount() != 2)\n        throw std::runtime_error(\"Content Manager contract detection failed; check database permissions/logs\");\n    Field* fields = result->Fetch();\n    if (fields[0].IsNull() || fields[1].IsNull())\n        throw std::runtime_error(\"unexpected Content Manager contract detection result\");\n    if (fields[0].Get<uint64>() == 0) return false;\n    if (fields[0].Get<uint64>() != 1 || fields[1].Get<uint64>() != 5)\n        throw std::runtime_error(\"incompatible content_manager_build contract; expected build_number, realm_name, filename, sha256 and state\");\n    return true;\n}\n\nstd::string MetadataSQL(bool withContent)\n{\n    std::string sql = MetadataQuery;\n    if (withContent)\n    {\n        sql += R\"SQL(\nUNION ALL\nSELECT CONVERT('3' USING utf8mb4) COLLATE utf8mb4_unicode_ci, 0,\n       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,\n       CONVERT(CAST(build_number AS CHAR) USING utf8mb4) COLLATE utf8mb4_unicode_ci,\n       CONVERT(realm_name USING utf8mb4) COLLATE utf8mb4_unicode_ci,\n       CONVERT(filename USING utf8mb4) COLLATE utf8mb4_unicode_ci,\n       CONVERT(sha256 USING utf8mb4) COLLATE utf8mb4_unicode_ci\n)SQL\";\n        for (unsigned i = 4; i < 19; ++i)\n            sql += \", CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci\";\n        // Deliberately do not filter by realm or LIMIT 1: multiple ACTIVE rows\n        // anywhere in this world database are ambiguous and must be rejected.\n        sql += \" FROM content_manager_build WHERE BINARY state = 'ACTIVE'\\n\";\n    }\n    sql += \"ORDER BY record_type, sort_order, BINARY record_key\";\n    return sql;\n}"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 8.9s)

_Thinking:_

Now update LoadMetadata to parse type '4' rows and associate. Let me edit the LoadMetadata function.

Now LoadMetadata to parse and associate requirement rows:

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp",
  "newString": "Settings settings;\n    settings.OutputDirectory = directory;\n    bool found = false;\n    // Byte-wise sorted, deduplicated requirement sets keyed by build number,\n    // mirroring Content Manager's own requirement ordering.\n    std::map<std::string, std::set<std::string>> buildRequirements;\n    do\n    {\n        Field* fields = result->Fetch();\n        for (unsigned i = 0; i < 22; ++i)\n            if (fields[i].IsNull())\n                throw std::runtime_error(\"mod_realm_config SQL data contains an unexpected NULL\");\n        auto value = [fields](unsigned i) { return fields[i + 3].Get<std::string>(); };\n        auto type = fields[0].Get<std::string>();\n        if (type == \"0\")\n        {\n            if (found) throw std::runtime_error(\"duplicate singleton configuration row\");\n            found = true;\n            settings.RealmKey = value(0);\n            settings.Name = value(1);\n            settings.Address = value(2);\n            settings.Description = value(3);\n            settings.WebsiteURL = value(4);\n            settings.ClientVersion = value(5);\n            settings.ClientBuild = value(6);\n            settings.AuthPort = value(7);\n            settings.WorldPort = value(8);\n            settings.ConfigURL = value(9);\n            settings.Executable = value(10);\n            settings.ExecutableSHA256 = value(11);\n            settings.MinimumVersion = value(12);\n            settings.ManifestURL = value(13);\n            settings.NewsURL = value(14);\n            settings.StatusURL = value(15);\n            settings.CalendarURL = value(16);\n            settings.ArmoryURL = value(17);\n            settings.ContentBaseURL = value(18);\n        }\n        else if (type == \"1\")\n            settings.Addons.push_back({fields[2].Get<std::string>(), value(0), value(1),\n                value(2), value(3), value(4), value(5)});\n        else if (type == \"2\")\n            settings.Patches.push_back({fields[2].Get<std::string>(), value(0), value(1),\n                value(2), value(3), value(4), value(5), value(6)});\n        else if (type == \"3\")\n            settings.ActiveBuilds.push_back({value(0), value(1), value(2), value(3)});\n        else if (type == \"4\")\n            buildRequirements[fields[3].Get<std::string>()].insert(fields[2].Get<std::string>());\n        else throw std::runtime_error(\"unexpected Schema v1 SQL record type\");\n    } while (result->NextRow());\n    if (!found) throw std::runtime_error(\"mod_realm_config singleton id=1 is missing\");\n    for (auto const& entry : buildRequirements)\n    {\n        bool matched = false;\n        for (auto const& build : settings.ActiveBuilds)\n            if (build.Number == entry.first) { matched = true; break; }\n        if (!matched)\n            throw std::runtime_error(\"Content Manager client requirements reference a build that is not ACTIVE in this snapshot\");\n    }\n    for (auto& build : settings.ActiveBuilds)\n    {\n        auto it = buildRequirements.find(build.Number);\n        if (it != buildRequirements.end())\n            build.ClientRequirements.assign(it->second.begin(), it->second.end());\n    }\n    return settings;",
  "oldString": "Settings settings;\n    settings.OutputDirectory = directory;\n    bool found = false;\n    do\n    {\n        Field* fields = result->Fetch();\n        for (unsigned i = 0; i < 22; ++i)\n            if (fields[i].IsNull())\n                throw std::runtime_error(\"mod_realm_config SQL data contains an unexpected NULL\");\n        auto value = [fields](unsigned i) { return fields[i + 3].Get<std::string>(); };\n        auto type = fields[0].Get<std::string>();\n        if (type == \"0\")\n        {\n            if (found) throw std::runtime_error(\"duplicate singleton configuration row\");\n            found = true;\n            settings.RealmKey = value(0);\n            settings.Name = value(1);\n            settings.Address = value(2);\n            settings.Description = value(3);\n            settings.WebsiteURL = value(4);\n            settings.ClientVersion = value(5);\n            settings.ClientBuild = value(6);\n            settings.AuthPort = value(7);\n            settings.WorldPort = value(8);\n            settings.ConfigURL = value(9);\n            settings.Executable = value(10);\n            settings.ExecutableSHA256 = value(11);\n            settings.MinimumVersion = value(12);\n            settings.ManifestURL = value(13);\n            settings.NewsURL = value(14);\n            settings.StatusURL = value(15);\n            settings.CalendarURL = value(16);\n            settings.ArmoryURL = value(17);\n            settings.ContentBaseURL = value(18);\n        }\n        else if (type == \"1\")\n            settings.Addons.push_back({fields[2].Get<std::string>(), value(0), value(1),\n                value(2), value(3), value(4), value(5)});\n        else if (type == \"2\")\n            settings.Patches.push_back({fields[2].Get<std::string>(), value(0), value(1),\n                value(2), value(3), value(4), value(5), value(6)});\n        else if (type == \"3\")\n            settings.ActiveBuilds.push_back({value(0), value(1), value(2), value(3)});\n        else throw std::runtime_error(\"unexpected Schema v1 SQL record type\");\n    } while (result->NextRow());\n    if (!found) throw std::runtime_error(\"mod_realm_config singleton id=1 is missing\");\n    return settings;"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 4.8s)

_Thinking:_

Now add ValidateClientRequirement and hook into ValidateSettings. Let me add it near ValidateKey, and then in ValidateSettings add the check.

Now the requirement validator and its hook into ValidateSettings:

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp",
  "newString": "void ValidateRealmKey(std::string const& value)\n{\n    if (value.empty() || value.size() > 64 ||\n        !std::all_of(value.begin(), value.end(), [](unsigned char c)\n        {\n            return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_' || c == '-';\n        }))\n        throw std::runtime_error(\"mod_realm_config.realm_key must contain 1..64 lowercase ASCII letters, digits, underscores or hyphens\");\n}\n\n// Boundary check for a semantic client requirement name from Content Manager.\n// This validates the published token format (1..64 safe ASCII characters, no\n// commas or whitespace) so the CSV value round-trips deterministically. It does\n// not duplicate Content Manager's supported-capability registry.\nvoid ValidateClientRequirement(std::string const& value)\n{\n    if (value.empty() || value.size() > 64 ||\n        !std::all_of(value.begin(), value.end(), [](unsigned char c)\n        {\n            return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||\n                (c >= '0' && c <= '9') || c == '.' || c == '_' || c == '-';\n        }))\n        throw std::runtime_error(\"Client.Requirements entries must contain 1..64 ASCII letters, digits, dots, underscores or hyphens\");\n}\n\nvoid ValidateClientRequirements(Settings const& settings)\n{\n    for (std::size_t i = 0; i < settings.ClientRequirements.size(); ++i)\n    {\n        try { ValidateClientRequirement(settings.ClientRequirements[i]); }\n        catch (std::exception const& error)\n        {\n            throw std::runtime_error(\"Client.Requirements entry #\" + std::to_string(i + 1) +\n                \" in Content Manager build order: \" + error.what());\n        }\n    }\n}",
  "oldString": "void ValidateRealmKey(std::string const& value)\n{\n    if (value.empty() || value.size() > 64 ||\n        !std::all_of(value.begin(), value.end(), [](unsigned char c)\n        {\n            return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_' || c == '-';\n        }))\n        throw std::runtime_error(\"mod_realm_config.realm_key must contain 1..64 lowercase ASCII letters, digits, underscores or hyphens\");\n}"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 2.7s)

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp",
  "newString": "ValidateURL(settings.CalendarURL, \"Services.CalendarURL\");\n    ValidateURL(settings.ArmoryURL, \"Services.ArmoryURL\");\n    ValidateClientRequirements(settings);\n    ValidateCatalogs(settings);",
  "oldString": "ValidateURL(settings.CalendarURL, \"Services.CalendarURL\");\n    ValidateURL(settings.ArmoryURL, \"Services.ArmoryURL\");\n    ValidateCatalogs(settings);"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 4.7s)

_Thinking:_

Now update SynthesizeRealmContent to attach requirements from the advertised ACTIVE build. Let me view the relevant section again. The function ends with:

```cpp
    auto url = settings.ContentBaseURL;
    while (url.back() == '/') url.pop_back(); // ValidateURL already required a nonempty authority.
    url += "/" + build.FileName;
    settings.Patches.push_back({"realm-content", build.RealmName + " Realm Content", "Required",
        "HTTP", std::move(url), "", "", build.SHA256, PatchInstallMode::WowPatch});
    // ValidateSettings subsequently validates the entire catalog using the same
    // key/name/source/hash rules, plus the install-mode-specific destination rules.
    return "available; ACTIVE realm content build " + number;
```

I'll add settings.ClientRequirements = build.ClientRequirements; before the push or right before return.

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp",
  "newString": "auto url = settings.ContentBaseURL;\n    while (url.back() == '/') url.pop_back(); // ValidateURL already required a nonempty authority.\n    url += \"/\" + build.FileName;\n    settings.Patches.push_back({\"realm-content\", build.RealmName + \" Realm Content\", \"Required\",\n        \"HTTP\", std::move(url), \"\", \"\", build.SHA256, PatchInstallMode::WowPatch});\n    // The advertised build is this realm's. Publish its immutable recorded client\n    // requirements; validation runs in ValidateSettings before BuildConfiguration.\n    settings.ClientRequirements = build.ClientRequirements;\n    // ValidateSettings subsequently validates the entire catalog using the same\n    // key/name/source/hash rules, plus the install-mode-specific destination rules.\n    return \"available; ACTIVE realm content build \" + number;",
  "oldString": "auto url = settings.ContentBaseURL;\n    while (url.back() == '/') url.pop_back(); // ValidateURL already required a nonempty authority.\n    url += \"/\" + build.FileName;\n    settings.Patches.push_back({\"realm-content\", build.RealmName + \" Realm Content\", \"Required\",\n        \"HTTP\", std::move(url), \"\", \"\", build.SHA256, PatchInstallMode::WowPatch});\n    // ValidateSettings subsequently validates the entire catalog using the same\n    // key/name/source/hash rules, plus the install-mode-specific destination rules.\n    return \"available; ACTIVE realm content build \" + number;"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 5.6s)

_Thinking:_

Now the serializer: JoinClientRequirements + BuildConfiguration Client section. Let me add JoinClientRequirements before BuildConfiguration, and add the Requirements key.

Now the serializer — join helper and `[Client]` section:

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp",
  "newString": "void AppendSection(std::string& output, char const* name, Fields const& fields)\n{\n    output += std::string(\"\\n[\") + name + \"]\\n\";\n    for (auto const& field : fields)\n        output += field.first + \"=\" + field.second + \"\\n\";\n}\n\n// Deterministic comma-separated rendering. Defensive normalization protects the\n// published form regardless of upstream vector order; the semantic strings are\n// never altered. Empty input renders as the empty value.\nstd::string JoinClientRequirements(std::vector<std::string> requirements)\n{\n    std::sort(requirements.begin(), requirements.end());\n    requirements.erase(std::unique(requirements.begin(), requirements.end()), requirements.end());\n    std::string joined;\n    for (std::size_t i = 0; i < requirements.size(); ++i)\n    {\n        if (i) joined += ',';\n        joined += requirements[i];\n    }\n    return joined;\n}\n\nstd::string BuildConfiguration(Settings const& settings)\n{",
  "oldString": "void AppendSection(std::string& output, char const* name, Fields const& fields)\n{\n    output += std::string(\"\\n[\") + name + \"]\\n\";\n    for (auto const& field : fields)\n        output += field.first + \"=\" + field.second + \"\\n\";\n}\n\nstd::string BuildConfiguration(Settings const& settings)\n{"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 5.0s)

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp",
  "newString": "AppendSection(output, \"Client\", {{\"Version\", settings.ClientVersion}, {\"Build\", settings.ClientBuild},\n        {\"Executable\", settings.Executable}, {\"ExecutableSHA256\", settings.ExecutableSHA256},\n        {\"RuntimeMode\", settings.ClientRuntimeMode},\n        {\"Requirements\", JoinClientRequirements(settings.ClientRequirements)}});",
  "oldString": "AppendSection(output, \"Client\", {{\"Version\", settings.ClientVersion}, {\"Build\", settings.ClientBuild},\n        {\"Executable\", settings.Executable}, {\"ExecutableSHA256\", settings.ExecutableSHA256},\n        {\"RuntimeMode\", settings.ClientRuntimeMode}});"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 3.0s)

_Thinking:_

Now the WorldScript plumbing. Let me update:
1. `_snapshotContent` type → `std::optional<ContentContract>`
2. OnUpdate detection handling
3. AcceptResult signature
4. ReloadSettings

Let me view the relevant section of the current file.

Now the WorldScript plumbing. Let me view the current state of that section:

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp",
  "offset": 700,
  "limit": 140
}
```

**Output:**
```
<path>/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp</path>
<type>file</type>
<content>
700:         AppendSection(output, ("Addon." + addon.Key).c_str(), {{"Name", addon.Name},
701:             {"Requirement", addon.Requirement}, {"SourceType", addon.SourceType},
702:             {"SourceURL", addon.SourceURL}, {"Ref", addon.Ref}, {"InstallDirectory", addon.InstallDirectory}});
703:     for (auto const& patch : settings.Patches)
704:     {
705:         Fields fields{{"Name", patch.Name}, {"Requirement", patch.Requirement},
706:             {"SourceType", patch.SourceType}, {"SourceURL", patch.SourceURL}};
707:         if (patch.InstallMode == PatchInstallMode::WowPatch)
708:             fields.emplace_back("InstallMode", "WowPatch");
709:         else
710:         {
711:             fields.emplace_back("FileName", patch.FileName);
712:             fields.emplace_back("InstallDirectory", patch.InstallDirectory);
713:         }
714:         fields.emplace_back("SHA256", patch.SHA256);
715:         AppendSection(output, ("Patch." + patch.Key).c_str(), fields);
716:     }
717:     return output;
718: }
719: 
720: fs::path RealmConfigFileName(Settings const& settings)
721: {
722:     return fs::path(settings.RealmKey + ".realm.conf");
723: }
724: 
725: struct TemporaryDirectory
726: {
727:     fs::path Path;
728:     ~TemporaryDirectory()
729:     {
730:         std::error_code ignored;
731:         fs::remove_all(Path, ignored);
732:     }
733: };
734: 
735: fs::path PublishConfiguration(Settings const& settings, std::string const& output)
736: {
737:     fs::path directory = fs::path(settings.OutputDirectory);
738:     fs::create_directories(directory);
739:     // Atomically reserve a private staging name, avoiding symlinks and concurrent writers.
740:     // The staged file stays on the destination filesystem for the final rename.
741:     static std::atomic<unsigned long long> sequence{0};
742:     fs::path staging;
743:     for (unsigned attempt = 0; attempt < 100; ++attempt)
744:     {
745:         auto candidate = directory / (".realm-config-" +
746:             std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) + "-" +
747:             std::to_string(sequence.fetch_add(1)));
748:         if (fs::create_directory(candidate)) { staging = std::move(candidate); break; }
749:     }
750:     if (staging.empty()) throw std::runtime_error("cannot reserve temporary publication directory");
751:     TemporaryDirectory temporary{staging};
752:     fs::permissions(staging, fs::perms::owner_all, fs::perm_options::replace);
753:     auto stagedFile = staging / RealmConfigFileName(settings);
754:     std::ofstream stream(stagedFile, std::ios::binary | std::ios::trunc);
755:     stream.exceptions(std::ios::badbit | std::ios::failbit);
756:     stream.write(output.data(), static_cast<std::streamsize>(output.size()));
757:     stream.flush();
758:     stream.close(); // A close failure also prevents publication.
759:     auto target = directory / RealmConfigFileName(settings);
760:     fs::rename(stagedFile, target); // Never remove the previous file to work around a rename error.
761:     return target;
762: }
763: 
764: class RealmConfigWorldScript : public WorldScript
765: {
766: public:
767:     RealmConfigWorldScript() : WorldScript("RealmConfigWorldScript") { }
768: 
769:     void OnStartup() override
770:     {
771:         _started = true;
772:         ReloadSettings();
773:     }
774: 
775:     void OnAfterConfigLoad(bool reload) override
776:     {
777:         if (reload && _started) ReloadSettings();
778:     }
779: 
780:     void OnUpdate(uint32 diff) override
781:     {
782:         // Query completion and all filesystem/state operations run on the world thread.
783:         if (_pending && _pending->InvokeIfReady()) _pending.reset();
784:         if (!_enabled) return;
785:         // Start the second query only after the detection callback has been destroyed.
786:         // Never replace _pending from inside its own callback.
787:         if (_snapshotContent.has_value() && !_pending)
788:         {
789:             bool withContent = *_snapshotContent;
790:             _snapshotContent.reset();
791:             auto epoch = _epoch;
792:             try
793:             {
794:                 _pending.emplace(WorldDatabase.AsyncQuery(MetadataSQL(withContent)).WithCallback(
795:                     [this, epoch, withContent](QueryResult result)
796:                     {
797:                         if (_enabled && epoch == _epoch) AcceptResult(result, withContent);
798:                     }));
799:             }
800:             catch (std::exception const& error) { ReportFailure(error.what()); }
801:             return;
802:         }
803:         if (diff < _remaining) { _remaining -= diff; return; }
804:         _remaining = 0;
805:         if (_pending) return; // At most one outstanding query, even across config reloads.
806:         _remaining = _interval;
807:         auto epoch = _epoch;
808:         try
809:         {
810:             _pending.emplace(WorldDatabase.AsyncQuery(ContentContractQuery).WithCallback(
811:                 [this, epoch](QueryResult result)
812:                 {
813:                     if (!_enabled || epoch != _epoch) return;
814:                     try { _snapshotContent = ReadContentContract(result); }
815:                     catch (std::exception const& error) { ReportFailure(error.what()); }
816:                 }));
817:         }
818:         catch (std::exception const& error) { ReportFailure(error.what()); }
819:     }
820: 
821: private:
822:     void ReportFailure(std::string const& message)
823:     {
824:         // Retry every interval, but don't repeat the same module error indefinitely.
825:         if (message != _lastError)
826:             LOG_ERROR("module", "mod-realm-config: publication failed; existing realm.conf preserved: {}", message);
827:         _lastError = message;
828:     }
829: 
830:     void AcceptResult(QueryResult const& result, bool withContent)
831:     {
832:         try
833:         {
834:             auto settings = LoadMetadata(result, _directory);
835:             settings.ClientRuntimeMode = _clientRuntimeMode;
836: 
837:             std::string realmWarning;
838:             settings.GameRealmName = LoadGameRealmName(realmWarning);
839:             if (realmWarning != _lastRealmWarning && !realmWarning.empty())

(Showing lines 700-839 of 919. Use offset=840 to continue.)
</content>
```

---

## Assistant (Build · Big Pickle · 77.3s)

_Thinking:_

Now I need to add the settings write for ClientRequirements — the "assert not set" tests need this. Let me read the lines around 744-762 (PublishConfiguration start) — I have it at lines 741-762. Now add:

```
        for (auto const& addon : settings.Addons)
            output += "Addon." + addon.Key + "=" + addon.Name + "\n";
        for (auto const& patch : settings.Patches)
            output += "Patch." + patch.Key + "=" + patch.Name + " InstallMode "
                + (patch.InstallMode == PatchInstallMode::WowPatch ? "WowPatch" : "File") + "\n";
        if (!settings.RealmContentAdvertisable.empty())
            output += "RealmContent=" + settings.RealmContentAdvertisable + "\n";
        if (!settings.ClientRequirements.empty())
            output += "ClientRequirements=" + settings.ClientRequirements + "\n";
```

Wait, but for `PublishConfiguration`, the `ClientRequirements` there are the *serialized CSV*. Yes.

Also, I realize the task's requirement #3 "settings::ClientRequirements published under [Client] arbitrarily empty" — covered.

Now, also there's still `settings.RealmContentAdvertisable` — but SynthesizeRealmContent returns the string; I should keep that and also set RealmContentAdvertisable? Let me check — does the log reference `_lastContentStatus` only? Yes. RealmContentAdvertisable isn't used in logging. The publisher writes the whole conf via BuildConfiguration which includes [Patch.realm-content] and [Client] Requirements. Good.

Now let me handle the requirement capture in AcceptResult. The contract's requirement build rows must reach SynthesizeRealmContent which attaches ClientRequirements. And AcceptResult's callback captures `withContent`; let me now check the signature I left in the world script — I changed MetadataSQL to take ContentContract. The world script calls AcceptResult(result, {content, requirements}) where the lambda captured `contract`. But the field reads — I kept `ReadContentContract(result)` returning ContentContract. So `_snapshotContent` is `std::optional<ContentContract>`; `*_snapshotContent` gives contract; `.content` bool. Good.

Wait — but `AcceptResult` signature I need to update: it currently takes `QueryResult const&, bool withContent`. Let me update to `QueryResult const&, ContentContract const& contract`. Let me look at the exact current code.

Now update the world script plumbing for the returned `ContentContract`. Let me view the relevant section:



**Tool: read**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp",
  "limit": 130,
  "offset": 653
}
```

**Output:**
```
<path>/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp</path>
<type>file</type>
<content>
653: 
654: void AppendSection(std::string& output, char const* name, Fields const& fields)
655: {
656:     output += std::string("\n[") + name + "]\n";
657:     for (auto const& field : fields)
658:         output += field.first + "=" + field.second + "\n";
659: }
660: 
661: // Deterministic comma-separated rendering. Defensive normalization protects the
662: // published form regardless of upstream vector order; the semantic strings are
663: // never altered. Empty input renders as the empty value.
664: std::string JoinClientRequirements(std::vector<std::string> requirements)
665: {
666:     std::sort(requirements.begin(), requirements.end());
667:     requirements.erase(std::unique(requirements.begin(), requirements.end()), requirements.end());
668:     std::string joined;
669:     for (std::size_t i = 0; i < requirements.size(); ++i)
670:     {
671:         if (i) joined += ',';
672:         joined += requirements[i];
673:     }
674:     return joined;
675: }
676: 
677: std::string BuildConfiguration(Settings const& settings)
678: {
679:     std::string output = "# Generated by mod-realm-config.\n"
680:         "# Public configuration consumed by Portalkeeper.\n"
681:         "# Do not edit this file manually.\n";
682:     AppendSection(output, "Config", {{"SchemaVersion", "1"}});
683:     Fields realmFields{{"Name", settings.Name}};
684:     if (settings.GameRealmName)
685:         realmFields.emplace_back("GameRealmName", *settings.GameRealmName);
686:     realmFields.emplace_back("Description", settings.Description);
687:     realmFields.emplace_back("WebsiteURL", settings.WebsiteURL);
688:     AppendSection(output, "Realm", realmFields);
689:     AppendSection(output, "Connection", {{"Address", settings.Address},
690:         {"AuthPort", settings.AuthPort}, {"WorldPort", settings.WorldPort}});
691:     AppendSection(output, "Client", {{"Version", settings.ClientVersion}, {"Build", settings.ClientBuild},
692:         {"Executable", settings.Executable}, {"ExecutableSHA256", settings.ExecutableSHA256},
693:         {"RuntimeMode", settings.ClientRuntimeMode},
694:         {"Requirements", JoinClientRequirements(settings.ClientRequirements)}});
695:     AppendSection(output, "Portalkeeper", {{"MinimumVersion", settings.MinimumVersion}});
696:     AppendSection(output, "Services", {{"ManifestURL", settings.ManifestURL}, {"NewsURL", settings.NewsURL},
697:         {"StatusURL", settings.StatusURL}, {"CalendarURL", settings.CalendarURL},
698:         {"ArmoryURL", settings.ArmoryURL}, {"ConfigURL", settings.ConfigURL}});
699:     for (auto const& addon : settings.Addons)
700:         AppendSection(output, ("Addon." + addon.Key).c_str(), {{"Name", addon.Name},
701:             {"Requirement", addon.Requirement}, {"SourceType", addon.SourceType},
702:             {"SourceURL", addon.SourceURL}, {"Ref", addon.Ref}, {"InstallDirectory", addon.InstallDirectory}});
703:     for (auto const& patch : settings.Patches)
704:     {
705:         Fields fields{{"Name", patch.Name}, {"Requirement", patch.Requirement},
706:             {"SourceType", patch.SourceType}, {"SourceURL", patch.SourceURL}};
707:         if (patch.InstallMode == PatchInstallMode::WowPatch)
708:             fields.emplace_back("InstallMode", "WowPatch");
709:         else
710:         {
711:             fields.emplace_back("FileName", patch.FileName);
712:             fields.emplace_back("InstallDirectory", patch.InstallDirectory);
713:         }
714:         fields.emplace_back("SHA256", patch.SHA256);
715:         AppendSection(output, ("Patch." + patch.Key).c_str(), fields);
716:     }
717:     return output;
718: }
719: 
720: fs::path RealmConfigFileName(Settings const& settings)
721: {
722:     return fs::path(settings.RealmKey + ".realm.conf");
723: }
724: 
725: struct TemporaryDirectory
726: {
727:     fs::path Path;
728:     ~TemporaryDirectory()
729:     {
730:         std::error_code ignored;
731:         fs::remove_all(Path, ignored);
732:     }
733: };
734: 
735: fs::path PublishConfiguration(Settings const& settings, std::string const& output)
736: {
737:     fs::path directory = fs::path(settings.OutputDirectory);
738:     fs::create_directories(directory);
739:     // Atomically reserve a private staging name, avoiding symlinks and concurrent writers.
740:     // The staged file stays on the destination filesystem for the final rename.
741:     static std::atomic<unsigned long long> sequence{0};
742:     fs::path staging;
743:     for (unsigned attempt = 0; attempt < 100; ++attempt)
744:     {
745:         auto candidate = directory / (".realm-config-" +
746:             std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) + "-" +
747:             std::to_string(sequence.fetch_add(1)));
748:         if (fs::create_directory(candidate)) { staging = std::move(candidate); break; }
749:     }
750:     if (staging.empty()) throw std::runtime_error("cannot reserve temporary publication directory");
751:     TemporaryDirectory temporary{staging};
752:     fs::permissions(staging, fs::perms::owner_all, fs::perm_options::replace);
753:     auto stagedFile = staging / RealmConfigFileName(settings);
754:     std::ofstream stream(stagedFile, std::ios::binary | std::ios::trunc);
755:     stream.exceptions(std::ios::badbit | std::ios::failbit);
756:     stream.write(output.data(), static_cast<std::streamsize>(output.size()));
757:     stream.flush();
758:     stream.close(); // A close failure also prevents publication.
759:     auto target = directory / RealmConfigFileName(settings);
760:     fs::rename(stagedFile, target); // Never remove the previous file to work around a rename error.
761:     return target;
762: }
763: 
764: class RealmConfigWorldScript : public WorldScript
765: {
766: public:
767:     RealmConfigWorldScript() : WorldScript("RealmConfigWorldScript") { }
768: 
769:     void OnStartup() override
770:     {
771:         _started = true;
772:         ReloadSettings();
773:     }
774: 
775:     void OnAfterConfigLoad(bool reload) override
776:     {
777:         if (reload && _started) ReloadSettings();
778:     }
779: 
780:     void OnUpdate(uint32 diff) override
781:     {
782:         // Query completion and all filesystem/state operations run on the world thread.

(Showing lines 653-782 of 919. Use offset=783 to continue.)
</content>
```

---

## Assistant (Build · Big Pickle · 3.7s)

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp",
  "newString": "if (_snapshotContent.has_value() && !_pending)\n        {\n            ContentContract contract = *_snapshotContent;\n            _snapshotContent.reset();\n            auto epoch = _epoch;\n            try\n            {\n                _pending.emplace(WorldDatabase.AsyncQuery(MetadataSQL(contract)).WithCallback(\n                    [this, epoch, contract](QueryResult result)\n                    {\n                        if (_enabled && epoch == _epoch) AcceptResult(result, contract);\n                    }));\n            }\n            catch (std::exception const& error) { ReportFailure(error.what()); }\n            return;\n        }",
  "oldString": "if (_snapshotContent.has_value() && !_pending)\n        {\n            bool withContent = *_snapshotContent;\n            _snapshotContent.reset();\n            auto epoch = _epoch;\n            try\n            {\n                _pending.emplace(WorldDatabase.AsyncQuery(MetadataSQL(withContent)).WithCallback(\n                    [this, epoch, withContent](QueryResult result)\n                    {\n                        if (_enabled && epoch == _epoch) AcceptResult(result, withContent);\n                    }));\n            }\n            catch (std::exception const& error) { ReportFailure(error.what()); }\n            return;\n        }"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 5.9s)

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp",
  "limit": 100,
  "offset": 830
}
```

**Output:**
```
<path>/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp</path>
<type>file</type>
<content>
830:     void AcceptResult(QueryResult const& result, bool withContent)
831:     {
832:         try
833:         {
834:             auto settings = LoadMetadata(result, _directory);
835:             settings.ClientRuntimeMode = _clientRuntimeMode;
836: 
837:             std::string realmWarning;
838:             settings.GameRealmName = LoadGameRealmName(realmWarning);
839:             if (realmWarning != _lastRealmWarning && !realmWarning.empty())
840:                 LOG_WARN("module", "{}", realmWarning);
841:             _lastRealmWarning = std::move(realmWarning);
842:             auto contentStatus = SynthesizeRealmContent(settings, withContent, realm.Name);
843:             ValidateSettings(settings);
844:             auto output = BuildConfiguration(settings);
845:             auto target = fs::path(_directory) / RealmConfigFileName(settings);
846:             if (output != _lastOutput || target != _lastTarget || !fs::is_regular_file(target))
847:             {
848:                 PublishConfiguration(settings, output);
849:                 _lastOutput = std::move(output);
850:                 _lastTarget = std::move(target);
851:                 LOG_INFO("module", "mod-realm-config: loaded SQL configuration; generated {} with {} addons and {} patches",
852:                     _lastTarget.string(), settings.Addons.size(), settings.Patches.size());
853:             }
854:             else if (!_lastError.empty())
855:                 LOG_INFO("module", "mod-realm-config: database refresh recovered; published metadata is current");
856:             if (contentStatus != _lastContentStatus)
857:             {
858:                 LOG_INFO("module", "mod-realm-config: Content Manager integration {}", contentStatus);
859:                 _lastContentStatus = std::move(contentStatus);
860:             }
861:             _lastError.clear();
862:         }
863:         catch (std::exception const& error) { ReportFailure(error.what()); }
864:     }
865: 
866:     void ReloadSettings()
867:     {
868:         _snapshotContent.reset();
869:         ++_epoch; // Discard any result queued under an older operational configuration.
870:         _enabled = false;
871:         try
872:         {
873:             if (!sConfigMgr->GetOption<bool>("RealmConfig.Enable", true))
874:             {
875:                 LOG_INFO("module", "mod-realm-config: disabled");
876:                 return;
877:             }
878:             _directory = sConfigMgr->GetOption<std::string>("RealmConfig.OutputDirectory", "realm-config");
879:             ValidateText(_directory, "RealmConfig.OutputDirectory", true);
880:             auto seconds = sConfigMgr->GetOption<std::string>("RealmConfig.RefreshIntervalSeconds", "30");
881:             ValidateNumber(seconds, "RealmConfig.RefreshIntervalSeconds", 86400);
882:             auto runtimeMode = FoldASCII(sConfigMgr->GetOption<std::string>("RealmConfig.Client.RuntimeMode", "Legacy"));
883:             _clientRuntimeMode = "Legacy";
884:             if (runtimeMode == "isolated")
885:                 _clientRuntimeMode = "Isolated";
886:             else if (runtimeMode != "legacy")
887:                 LOG_ERROR("module", "mod-realm-config: invalid RealmConfig.Client.RuntimeMode; expected Legacy or Isolated; using Legacy");
888:             _interval = static_cast<uint32>(std::stoul(seconds)) * 1000;
889:             _remaining = _interval;
890:             _enabled = true;
891:             LOG_INFO("module", "mod-realm-config: enabled; checking world database every {} seconds", seconds);
892:             // Synchronous detection and snapshot at startup/reload; asynchronous during refresh.
893:             bool withContent = ReadContentContract(WorldDatabase.Query(ContentContractQuery));
894:             AcceptResult(WorldDatabase.Query(MetadataSQL(withContent)), withContent);
895:         }
896:         catch (std::exception const& error) { ReportFailure(error.what()); }
897:     }
898: 
899:     bool _started = false;
900:     bool _enabled = false;
901:     uint32 _interval = 30000;
902:     uint32 _remaining = 30000;
903:     std::uint64_t _epoch = 0;
904:     std::string _directory;
905:     std::string _clientRuntimeMode = "Legacy";
906:     std::string _lastOutput;
907:     fs::path _lastTarget;
908:     std::string _lastError;
909:     std::optional<QueryCallback> _pending;
910:     std::optional<bool> _snapshotContent;
911:     std::string _lastContentStatus;
912:     std::string _lastRealmWarning;
913: };
914: }
915: 
916: void Addmod_realm_configScripts()
917: {
918:     new RealmConfigWorldScript();
919: }

(End of file - total 919 lines)
</content>
```

---

## Assistant (Build · Big Pickle · 3.9s)





**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp",
  "newString": "void AcceptResult(QueryResult const& result, ContentContract const& contract)\n    {\n        try\n        {\n            auto settings = LoadMetadata(result, _directory);\n            settings.ClientRuntimeMode = _clientRuntimeMode;\n\n            std::string realmWarning;\n            settings.GameRealmName = LoadGameRealmName(realmWarning);\n            if (realmWarning != _lastRealmWarning && !realmWarning.empty())\n                LOG_WARN(\"module\", \"{}\", realmWarning);\n            _lastRealmWarning = std::move(realmWarning);\n            auto contentStatus = SynthesizeRealmContent(settings, contract.content, realm.Name);\n            ValidateSettings(settings);",
  "oldString": "void AcceptResult(QueryResult const& result, bool withContent)\n    {\n        try\n        {\n            auto settings = LoadMetadata(result, _directory);\n            settings.ClientRuntimeMode = _clientRuntimeMode;\n\n            std::string realmWarning;\n            settings.GameRealmName = LoadGameRealmName(realmWarning);\n            if (realmWarning != _lastRealmWarning && !realmWarning.empty())\n                LOG_WARN(\"module\", \"{}\", realmWarning);\n            _lastRealmWarning = std::move(realmWarning);\n            auto contentStatus = SynthesizeRealmContent(settings, withContent, realm.Name);\n            ValidateSettings(settings);"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 3.0s)

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp",
  "newString": "// Synchronous detection and snapshot at startup/reload; asynchronous during refresh.\n            auto contract = ReadContentContract(WorldDatabase.Query(ContentContractQuery));\n            AcceptResult(WorldDatabase.Query(MetadataSQL(contract)), contract);",
  "oldString": "// Synchronous detection and snapshot at startup/reload; asynchronous during refresh.\n            bool withContent = ReadContentContract(WorldDatabase.Query(ContentContractQuery));\n            AcceptResult(WorldDatabase.Query(MetadataSQL(withContent)), withContent);"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 2.1s)

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp",
  "newString": "std::optional<QueryCallback> _pending;\n    std::optional<ContentContract> _snapshotContent;",
  "oldString": "std::optional<QueryCallback> _pending;\n    std::optional<bool> _snapshotContent;"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 3.4s)

_Thinking:_

The `_snapshotContent = ReadContentContract(result);` line in OnUpdate works because ReadContentContract now returns ContentContract and _snapshotContent is optional<ContentContract>.

Now let me verify the whole production file compiles logically — let me re-read the full file once to check for consistency.

Let me review the full production file to check consistency:

**Tool: bash**

**Input:**
```json
{
  "command": "grep -n \"withContent\\|ReadContentContract\\|MetadataSQL\\|ContentContract\\|_snapshotContent\\|ClientRequirements\\|JoinClientRequirements\" /home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp"
}
```

**Output:**
```
46:    std::vector<std::string> ClientRequirements;
70:    std::vector<std::string> ClientRequirements;
162:struct ContentContract
170:constexpr char ContentContractQuery[] = R"SQL(
184:ContentContract ReadContentContract(QueryResult const& result)
191:    ContentContract contract;
203:std::string MetadataSQL(ContentContract const& contract)
312:            build.ClientRequirements.assign(it->second.begin(), it->second.end());
448:void ValidateClientRequirements(Settings const& settings)
450:    for (std::size_t i = 0; i < settings.ClientRequirements.size(); ++i)
452:        try { ValidateClientRequirement(settings.ClientRequirements[i]); }
595:    settings.ClientRequirements = build.ClientRequirements;
626:    ValidateClientRequirements(settings);
664:std::string JoinClientRequirements(std::vector<std::string> requirements)
694:        {"Requirements", JoinClientRequirements(settings.ClientRequirements)}});
787:        if (_snapshotContent.has_value() && !_pending)
789:            ContentContract contract = *_snapshotContent;
790:            _snapshotContent.reset();
794:                _pending.emplace(WorldDatabase.AsyncQuery(MetadataSQL(contract)).WithCallback(
810:            _pending.emplace(WorldDatabase.AsyncQuery(ContentContractQuery).WithCallback(
814:                    try { _snapshotContent = ReadContentContract(result); }
830:    void AcceptResult(QueryResult const& result, ContentContract const& contract)
868:        _snapshotContent.reset();
893:            auto contract = ReadContentContract(WorldDatabase.Query(ContentContractQuery));
894:            AcceptResult(WorldDatabase.Query(MetadataSQL(contract)), contract);
910:    std::optional<ContentContract> _snapshotContent;

```

---

## Assistant (Build · Big Pickle · 2.6s)

_Thinking:_

Let me view lines 36-75 to double check the struct placement (line 46 is ActiveContentBuild's ClientRequirements, line 70 is Settings'). Also check line 184 Region and the ValidateClientRequirement around 420-460.

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp",
  "limit": 50,
  "offset": 36
}
```

**Output:**
```
<path>/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp</path>
<type>file</type>
<content>
36: {
37:     std::string Key, Name, Requirement, SourceType, SourceURL, FileName, InstallDirectory, SHA256;
38:     PatchInstallMode InstallMode = PatchInstallMode::File;
39: };
40: 
41: struct ActiveContentBuild
42: {
43:     std::string Number, RealmName, FileName, SHA256;
44:     // Immutable client requirements recorded against this exact build by Content
45:     // Manager (Schema 3). Sorted, deduplicated, copied from the ACTIVE row set.
46:     std::vector<std::string> ClientRequirements;
47: };
48: 
49: struct Settings
50: {
51:     std::string OutputDirectory;
52:     std::string RealmKey;
53:     std::string Name;
54:     std::optional<std::string> GameRealmName;
55:     std::string Address;
56:     std::string Description;
57:     std::string WebsiteURL;
58:     std::string ClientVersion;
59:     std::string ClientBuild;
60:     std::string ClientRuntimeMode = "Legacy";
61:     std::string AuthPort;
62:     std::string WorldPort;
63:     std::string ConfigURL;
64:     std::string Executable, ExecutableSHA256, MinimumVersion;
65:     std::string ManifestURL, NewsURL, StatusURL, CalendarURL, ArmoryURL;
66:     std::string ContentBaseURL;
67:     std::vector<ActiveContentBuild> ActiveBuilds;
68:     // Published semantic client requirements of the ACTIVE build being advertised.
69:     // Empty when no owned ACTIVE build is advertised.
70:     std::vector<std::string> ClientRequirements;
71:     std::vector<Addon> Addons;
72:     std::vector<Patch> Patches;
73: };
74: 
75: // One statement gives a consistent snapshot of our InnoDB tables and optional ACTIVE builds.
76: // Empty catalogs still return the singleton; any missing table/query error fails closed.
77: // Control columns type/order/key precede 19 explicitly whitelisted text columns.
78: constexpr char MetadataQuery[] = R"SQL(
79: SELECT
80:        CONVERT('0' USING utf8mb4) COLLATE utf8mb4_unicode_ci AS record_type,
81:        0 AS sort_order,
82:        CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci AS record_key,
83:        CONVERT(realm_key USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f0,
84:        CONVERT(name USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f1,
85:        CONVERT(address USING utf8mb4) COLLATE utf8mb4_unicode_ci AS f2,

(Showing lines 36-85 of 919. Use offset=86 to continue.)
</content>
```

---

## Assistant (Build · Big Pickle · 83.6s)

_Thinking:_

Good. Now let me build the test infrastructure. First, create the stub API headers in `tests/api/`.

Let me create:
- tests/api/Common.h (uint aliases + shared includes)
- tests/api/Config.h
- tests/api/Field.h
- tests/api/QueryResult.h
- tests/api/DatabaseEnv.h
- tests/api/Log.h
- tests/api/ScriptMgr.h
- tests/api/World.h
- tests/api/Realm.h

Then tests/mod_realm_config_tests.cpp and tests/run.sh.

Let me carefully design the Common.h. The production code uses `uint32` type. Define `using uint32 = std::uint32_t; using uint64 = std::uint64_t;`.

Now, one important subtlety: the production code uses `QueryResult const& result` and does `result->Fetch()` in ReadContentContract. My QueryResult needs `operator->` that returns a reference allowing `.Fetch()` on const. Also `result->NextRow()`. Let me make Fetch & NextRow const (mutable index), operator-> const returns const ref... wait, ReadContentContract calls `result->Fetch()` where result is `QueryResult const&`. operator->() const → returns QueryResult const&. Then `.Fetch()` on const object must be callable → Fetch must be const (it returns Field* non-const — fine, the pointer is stored). NextRow called on const `result` too → const with mutable index.

But BuildConfiguration/callback passes `QueryResult result` by value sometimes. My QueryResult is copyable/movable (shared_ptr). Good.

Now DatabaseEnv stub — WorldDatabase must have Query and AsyncQuery, LoginDatabase Query (with fmt args). Since production only calls WorldDatabase.Query(ContentContractQuery) and AsyncQuery(...).WithCallback and LoginDatabase.Query("...{}", realmId), the stub:

```cpp
class QueryCallback
{
public:
    template<class Callback>
    QueryCallback& WithCallback(Callback&&) { return *this; }
};
class DatabaseWorkerPool
{
public:
    QueryResult Query(std::string const&) { return QueryResult(); }
    template<class... Args> QueryResult Query(std::string const&, Args&&... args) { return QueryResult(); }
    QueryCallback AsyncQuery(std::string const&) { return QueryCallback(); }
};
extern DatabaseWorkerPool WorldDatabase;
extern DatabaseWorkerPool LoginDatabase;
```

field stub:

```cpp
class Field
{
public:
    Field() = default;
    Field(const char* value) : _null(false), _value(value) {}
    Field(std::string value) : _null(false), _value(std::move(value)) {}
    Field(std::uint64_t value) : _null(false), _value(std::to_string(value)) {}
    bool IsNull() const { return _null; }
    template<class T> T Get() const { throw std::runtime_error("unhandled Field::Get<T>"); }
private:
    bool _null = true;
    std::string _value;
};
template<> inline std::string Field::Get<std::string>() const { if (_null) throw std::runtime_error("Field::Get on NULL"); return _value; }
template<> inline std::uint64_t Field::Get<std::uint64_t>() const { if (_null) throw std::runtime_error("Field::Get on NULL"); return std::stoull(_value); }
template<> inline std::uint32_t Field::Get<std::uint32_t>() const { if (_null) throw std::runtime_error("Field::Get on NULL"); return static_cast<std::uint32_t>(std::stoul(_value)); }
```

Note: `Get<uint64>` where uint64 is an alias for std::uint64_t → resolves to the std::uint64_t specialization. And `Get<uint32>`? Production doesn't use uint32 Get. Fine.

Wait — there's a conflict: `std::uint64_t` on Linux is `unsigned long`, and `uint64` = `std::uint64_t` = `unsigned long`. `unsigned long long`? `std::stoull` returns unsigned long long. Whatever — Get<std::uint64_t> returns uint64_t via stoull cast.

Careful: On this platform std::uint64_t is `unsigned long`. The specialization `Get<std::uint64_t>` is fine.

Now QueryResult drop of default Row count 22 columns — the test builds rows with the helper.

Now the production file uses `WorldDatabase.AsyncQuery(...)` inside OnUpdate and ReloadSettings. The stub DatabaseWorkerPool.AsyncQuery returns QueryCallback. Fine.

One more: In production, `_pending.emplace(WorldDatabase.AsyncQuery(ContentContractQuery).WithCallback([this, epoch](QueryResult result) {...}));` — WithCallback is called on a temporary QueryCallback rvalue. WithCallback returns QueryCallback&. Since AsyncQuery returns by value (prvalue), and we call .WithCallback on it, the returned reference refers to the temporary which lives until end of the full expression. emplace copies it. OK.

Since ReloadSettings calls WorldDatabase.Query and reads nothing, those return empty QueryResult → ReadContentContract throws in ReloadSettings. But tests won't call ReloadSettings. Fine.

Now let me also double check: production includes "World.h". I'll make an empty stub with a guard.

Now, Field construction from std::uint64_t for the contract result. The contract result has 4 columns: `(1, 5, 1, 2)` etc. I'll build Field(uint64_t(1)).

Now, the test file. Let me write a comprehensive test. Since these are unit tests with no framework, I'll use a simple CHECK macro:

```cpp
static int failures = 0;
static int checks = 0;
#define CHECK(cond) do { ++checks; if (!(cond)) { ++failures; std::cerr << "FAIL line " << __LINE__ << ": " #cond "\n"; } } while (0)
#define CHECK_EQ(a, b) do { ++checks; auto va = (a); auto vb = (b); if (!(va == vb)) { ++failures; std::cerr << "FAIL line " << __LINE__ << ": ==\n"; } } while (0)
```

And a helper for "expect throw":
```cpp
#define CHECK_THROW(expr) do { ++checks; bool threw = false; try { (void)(expr); } catch (std::exception const&) { threw = true; } if (!threw) { ++failures; std::cerr << ...; } } while (0)
```

Helpers for building Settings and rows.

Settings helper:
```cpp
Settings MakeSettings()
{
    Settings s;
    s.OutputDirectory = "/tmp/out";
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
```

ValidateSettings requires ValidateText on OutputDirectory (true), etc. For BuildConfiguration tests, we don't need ValidateSettings. For SynthesizeRealmContent tests, we call it directly (it internally validates URL, filename, realm names etc.).

Row helper for LoadMetadata: 22-column row. Let me write:
```cpp
using Row = QueryResult::Row;
Row MetadataRow(std::string const& type, std::string const& key, std::vector<std::string> values)
{
    Row row;
    row.push_back(Field(type));
    row.push_back(Field(std::uint64_t(0)));
    row.push_back(Field(key));
    for (std::size_t i = 0; i < 19; ++i)
        row.push_back(Field(i < values.size() ? values[i] : ""));
    return row;
}
```

Wait — Field defined in the stub header Field.h. But production also includes "Field.h" and the real Field class. Since we compile the production into the same TU with our stub headers, there's ONE Field. Good.

Now LoadMetadata bit: the singleton row:
```
MetadataRow("0", "", {
  "test" /*realm_key f0*/, "Test Realm" /*name*/, "realm.example.com", "" desc, "https://", "3.3.5a", "12340", "3724", "8085", "https://...", "Wow.exe", "", "0.1.0", "", "", "", "", "", ""})
```
19 values for f0..f18 in order: realm_key, name, address, description, website_url, client_version, client_build, auth_port, world_port, config_url, client_executable, client_executable_sha256, portalkeeper_minimum_version, manifest_url, news_url, status_url, calendar_url, armory_url, content_base_url.

Wait: values list length 19. MetadataQuery select list order for type '0': f0 realm_key, f1 name, f2 address, f3 description, f4 website_url, f5 client_version, f6 client_build, f7 auth_port, f8 world_port, f9 config_url, f10 client_executable, f11 client_executable_sha256, f12 portalkeeper_minimum_version, f13 manifest_url, f14 news_url, f15 status_url, f16 calendar_url, f17 armory_url, f18 content_base_url. Yes 19 fields.

Build row:
```
MetadataRow("3", "", {"12", "Test Realm", "Test-Realm-Content-000012.mpq", "<64 hex>"})
```
Type '3' uses f0..f3. value(0)=build number, value(1)=realm_name, value(2)=filename, value(3)=sha256.

Requirement row:
```
MetadataRow("4", "protected-framexml", {"12"})
```
Type '4' uses record_key=requirement (second field in my MetadataRow helper = key) and f0=build number. So `MetadataRow("4", "protected-framexml", {"12"})` puts record_key="protected-framexml" and values[0]="12" into f0. LoadMetadata reads fields[2]=record_key as requirement and fields[3]=f0 as build number. 

Now LoadMetadata(value) uses fields indices. Good.

Now SynthesizeRealmContent tests: it reads settings.ActiveBuilds.front() etc.

Now, the `realm.Name` global — SynthesizeRealmContent takes realmName as parameter, so tests pass a name directly.

Validation for content builds:
- build.RealmName must pass ValidateText.
- build.Number passes ValidateNumber (1..UINT32_MAX).
- build.FileName: ValidateRelativePath(...true) then ASCII check.

For hash: 64 hex chars required for patch sha256 in ValidateSettings (Wait — ValidateSettings is called after Synthesize). For SynthesizeRealmContent alone, hash isn't validated there. For end-to-end tests I'll use a valid 64-hex hash.

Test plan (function list):

1. test_golden_config_no_content — full settings (addons + patches + no content) → BuildConfiguration → compare to exact expected string (golden). This verifies: valid generation, existing keys unchanged, Requirements= present and empty, Addons/Patches format unchanged.

Actually let me craft the golden carefully. Given AppendSection formatting: sections separated by "\n". Output begins with header 3 lines, then "\n[Config]\nSchemaVersion=1\n" etc.

Let me construct expected:

```
# Generated by mod-realm-config.
# Public configuration consumed by Portalkeeper.
# Do not edit this file manually.

[Config]
SchemaVersion=1

[Realm]
Name=Test Realm
Description=
WebsiteURL=https://example.com/

[Connection]
Address=realm.example.com
AuthPort=3724
WorldPort=8085

[Client]
Version=3.3.5a
Build=12340
Executable=Wow.exe
ExecutableSHA256=
RuntimeMode=Legacy
Requirements=

[Portalkeeper]
MinimumVersion=0.1.0

[Services]
ManifestURL=
NewsURL=
StatusURL=
CalendarURL=
ArmoryURL=
ConfigURL=https://example.com/realm.conf

[Addon.ExampleAddon]
Name=Example Addon
Requirement=Required
SourceType=GitHub
SourceURL=https://github.com/example/ExampleAddon
Ref=main
InstallDirectory=ExampleAddon

[Patch.ExamplePatch]
Name=Example Realm Patch
Requirement=Required
SourceType=HTTP
SourceURL=https://example.com/patch-X.MPQ
FileName=patch-X.MPQ
InstallDirectory=Data
SHA256=
```

Plus a trailing newline at end (each AppendSection ends with "\n"). The last section ends with "SHA256=\n" → file ends with '\n'.

I'll write this as a raw string literal and compare exactly. Note `AppendSection` for Client now adds Requirements after RuntimeMode. Good.

But wait — GamesRealmName: settings.GameRealmName is optional; MakeSettings leaves it unset → no GameRealmName line. Good.

2. test_client_legacy_isolated — set ClientRuntimeMode Legacy and Isolated, check the Client lines.

Actually the golden covers Legacy. Add a check for Isolated in a separate small settings (minimal settings just for BuildConfiguration). Since BuildConfiguration requires many fields but doesn't validate them, I can use MakeSettings and change RuntimeMode.

3. test_requirements_joined — check JoinClientRequirements:
- empty → ""
- {"protected-framexml"} → "protected-framexml"
- {"b","a","b","c"} → "a,b,c" (dedup + sort)
And BuildConfiguration output includes "Requirements=..." accordingly via settings.ClientRequirements.

4. test_no_content_manager — SynthesizeRealmContent(settings, false, "Test Realm") returns "unavailable"; settings unchanged (no realm-content patch, ClientRequirements empty); BuildConfiguration has "Requirements=".

5. test_legacy_actives_requirements_empty — contentAvailable true, content_base_url set, one ACTIVE build (owned), no requirement rows → synthesize returns "available; ACTIVE realm content build N"; patch added; settings.ClientRequirements empty → "Requirements=". Need LoadMetadata to produce settings first (to get build req attach), or just set settings.ActiveBuilds manually with empty ClientRequirements. I'll construct via LoadMetadata for realism in a separate "metadata" test, and also direct construction here.

Actually better: build settings via LoadMetadata from rows to test the whole chain. Let me do a helper `Settings LoadThroughMetadata(bool withBuild, build name, requirements, ...)`.

6. test_schema3_protected_framexml — LoadMetadata with build + one requirement row "protected-framexml" → attach → SynthesizeRealmContent → settings.ClientRequirements == {"protected-framexml"} → BuildConfiguration contains "Requirements=protected-framexml".

7. test_multiple_requirements — rows: {"zesty"}, {"apple"}, {"banana"} for same build → attached → joined "apple,banana,zesty". Also assert no duplicates when duplicate rows present.

8. test_requirements_from_active_not_packages — assert MetadataSQL(ContentContract{true,true}) contains "content_manager_build_client_requirement" and "state = 'ACTIVE'" and does NOT contain "for" package-ish. Hmm, simpler: assert the requirement SQL WHERE clause filters ACTIVE. Also assert MetadataSQL({true,false}) does NOT contain the requirement table name (so legacy/pre-Schema-3 doesn't read them). And that LoadMetadata attaches requirements only if type '4' rows (which are ACTIVE-filtered by SQL) present.

9. test_foreign_realm — build with realm_name "Other Realm", current realm "Test Realm" → Synthesize → returns "...different realm; not advertised"; patch not added; requirements not attached (empty). Also full pipeline: BuildConfiguration shows Requirements= and no [Patch.realm-content].

10. test_failure_modes:
- ReadContentContract(null QueryResult) throws.
- ReadContentContract with rows (1,5,1,3) throws (bad requirement columns).
- ReadContentContract with rows (1,5,1,2) → {content=true, requirements=true}.
- ReadContentContract with rows (1,5,0,0) → {content=true, requirements=false}.
- ReadContentContract with rows (0,0,0,0) → {false,false}.
- LoadMetadata(null) throws.
- LoadMetadata(10-column result) throws (field count != 22).
- LoadMetadata with orphan requirement row (references build 99 not ACTIVE) throws.
- LoadMetadata with a '-' failure: I'll also test that a type '4' row with an empty build_number ("") → unmatched → throws.
- The "lookup failure masquerading as no requirements" — the union query failure leads to null result → LoadMetadata throws. Tested by LoadMetadata(null) throwing. And AcceptResult catch → ReportFailure → last good file preserved. I'll also exercise the actual fail-safe by a tiny driver using RealmConfigWorldScript's AcceptResult via... it's private. Instead I'll simulate: since AcceptResult catches exceptions and calls ReportFailure, and the publish only happens on success, the "no publish" guarantee is structural. My tests verify the throwing behavior which AcceptResult converts to "no publish".

Hmm — but I want an actual behavioral test that a failed requirement lookup never yields Requirements= in a published file. Since AcceptResult is private and tightly coupled to member state + LoginDatabase + PublishConfiguration, testing it directly requires making it accessible. I could add a friend. Let me consider adding a friend function `TestAcceptResult` — modify production slightly. Or I can simulate the same sequence of calls as AcceptResult (LoadMetadata→ LoadGameRealmName→ Synthesize→ Validate→ BuildConfiguration→ PublishConfiguration guarded by try/catch) in the test itself to prove "exception aborts before publish". That models the behavior without touching production. The point of #10 is: "must NOT silently become Requirements=; publication must fail safely according to existing module behavior." The existing module behavior = catch + ReportFailure + no publish. My test will demonstrate that an invalid/incompatible requirement state causes the pipeline (the exact sequence AcceptResult uses) to throw before PublishConfiguration. I'll write a helper `bool RunsToPublication(Settings& settings, ...)` mimicking AcceptResult sequence and assert that when requirement lookup fails, no file is published and it does not produce a Requirements= file.

Actually, better and more honest: I'll add a small `friend` struct in production to expose AcceptResult for testing? That modifies production for testability. The codebase already has content-manager making test hooks. But minimal-touch principle: I prefer not to add friend hacks. The sequence-mimic test is fine and documents the fail-safe. Let me do that.

Also PublishConfiguration test: write to temp dir using std::filesystem::temp_directory_path/unique, call PublishConfiguration, read back file content equals output and filename "test.realm.conf"; then remove dir.

11. RuntimeMode legacy/isolated — covered.

12. Addons/Patches unchanged — covered by golden + a test that synthesizing a content build only adds the realm-content patch and the Requirements line, not changing existing patch lines.

Let me also add a test for Requirements CSV round-trip with validation: ValidateSettings rejects invalid requirement strings (comma, space, too long, empty, control chars). And accepts "protected-framexml".

Let me also test SynthesizeRealmContent when content_base_url empty with an owned build → returns "available; content_base_url is unset"; no requirements attached. (Doc choice.)

Also multiple ACTIVE builds → throw (already existing behavior, but test remains).

Let me write the test file. This will be long. Let me structure it with functions and a main.

Also, the test file must define globals: `ConfigMgr* sConfigMgr;`, `DatabaseWorkerPool WorldDatabase;`, `DatabaseWorkerPool LoginDatabase;`, `Realm realm;`.

Now, one more subtlety: the production code's `LoadGameRealmName` uses `LoginDatabase.Query("SELECT name FROM realmlist WHERE id = {}", realmId)`. That's the fmt-style overload. My stub Query(sql, args...) returns null QueryResult → LoadGameRealmName throws → caught → returns nullopt + warning. Good, and in tests of AcceptResult-mimic I'll stub realm.Id.Realm (default 0) — but the mimic calls LoadGameRealmName? Actually AcceptResult calls LoadGameRealmName which uses LoginDatabase. In my sequence-mimic helper I can skip GameRealmName (set settings.GameRealmName beforehand), because the "fail-safe" behavior of interest is around requirement errors. I'll model: LoadMetadata → Synthesize → Validate → BuildConfiguration → Publish guarded by try/catch. Skip LoadGameRealmName to avoid global interplay (or include it, since stub returns nullopt warning — fine but then warning string printing via LOG_WARN macro is no-op). Actually including it adds realism but involves the `realm.Id.Realm` global — default 0. Fine.

Hmm, but I realize I should double check: does `realm.Id.Realm` type matter? Production reads `auto const realmId = realm.Id.Realm;` then `LoginDatabase.Query("SELECT name FROM realmlist WHERE id = {}", realmId)`. realmId is uint32. In stub RealmId { uint32 Realm = 0; }. Good.

Now writing the test. I'll place it at tests/mod_realm_config_tests.cpp. It will `#include "../src/mod_realm_config.cpp"`.

Include order: The stub headers are found via -I tests/api. But the test file itself may want <iostream>, <filesystem>, etc. Those are standard.

Important: compile flags. `#include "../src/mod_realm_config.cpp"` — the cpp includes "Config.h" etc. With -I tests/api they resolve to stubs. But what about a stray "Config.h" from the system include path? We use `#include "..."` (quoted) for the stubs; gcc searches the including file's directory then -I paths. Stubs are in tests/api; cpp is in src/ — search order: dir of including file (src/) then -I tests/api. No clash. Good.

Let me also double-check: my test includes the cpp, and any STL headers the cpp includes (filesystem, fstream, etc.) are fine.

Now — `uint32`/`uint64` alias: Common.h defines `using uint32 = std::uint32_t;` etc. in global namespace. But `std::uint32_t` is `unsigned int`; and production uses `static_cast<uint32>(...)` and `uint32 diff`. OK.

Potential conflict: the `Field` class might conflict with a system symbol? Unlikely.

Potential issue: `QueryResult` — production creates `QueryResult()` (null) and `std::optional<QueryCallback> _pending`. Fine.

Another potential compile issue: production's OnUpdate uses `_pending->InvokeIfReady()`. My QueryCallback has InvokeIfReady(). Good.

Now about `LOG_*` macros with category string "module" and fmt args — swallow.

Also `LOG_ERROR("module", "mod-realm-config: publication failed; existing realm.conf preserved: {}", message);` — the macro `#define LOG_ERROR(category, ...)` swallows.

But careful: macro invocation `LOG_ERROR("module", ...)` where `...` includes a comma? `LOG_ERROR(category, msg1, msg2)` — variadic handles. All LOG_ calls in production have exactly 2+ args. Swallowed.

Now, there's one more subtlety: `ReportFailure` compares `message != _lastError`. `LOG_ERROR` no-op. Fine.

Now compile and run. Let me write the files.

Actually — before writing tests, let me quickly attempt to syntax-check the production file in isolation. I can't compile against real AzerothCore. But I can compile it with my stubs once they exist, which effectively validates. Let me write everything then compile.

Let me write the stub headers first.

Production changes are complete. Now let me build the test infrastructure. First, the API double headers:

**Tool: write**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/tests/api/Common.h",
  "content": "#ifndef REALM_CONFIG_TEST_COMMON_H\n#define REALM_CONFIG_TEST_COMMON_H\n\n#include <cstdint>\n#include <string>\n#include <utility>\n#include <vector>\n\n// Minimal AzerothCore-style global type aliases used by the production source.\nusing uint32 = std::uint32_t;\nusing uint64 = std::uint64_t;\n\n#endif"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 4.2s)

**Tool: write**

**Input:**
```json
{
  "content": "#ifndef REALM_CONFIG_TEST_FIELD_H\n#define REALM_CONFIG_TEST_FIELD_H\n\n#include \"Common.h\"\n\n#include <cstdint>\n#include <stdexcept>\n#include <string>\n#include <utility>\n\n// Test-only double for AzerothCore's Field. Stores a stringy value; numeric\n// Get<T> parses it. A default-constructed Field reads as NULL.\nclass Field\n{\npublic:\n    Field() = default;\n    Field(const char* value) : _null(false), _value(value) {}\n    Field(std::string value) : _null(false), _value(std::move(value)) {}\n    Field(std::uint64_t value) : _null(false), _value(std::to_string(value)) {}\n\n    bool IsNull() const { return _null; }\n\n    template<class T> T Get() const;\n\nprivate:\n    bool _null = true;\n    std::string _value;\n};\n\ntemplate<> inline std::string Field::Get<std::string>() const\n{\n    if (_null) throw std::runtime_error(\"Field::Get<std::string> on NULL\");\n    return _value;\n}\n\ntemplate<> inline std::uint64_t Field::Get<std::uint64_t>() const\n{\n    if (_null) throw std::runtime_error(\"Field::Get<uint64> on NULL\");\n    return std::stoull(_value);\n}\n\ntemplate<> inline std::uint32_t Field::Get<std::uint32_t>() const\n{\n    if (_null) throw std::runtime_error(\"Field::Get<uint32> on NULL\");\n    return static_cast<std::uint32_t>(std::stoul(_value));\n}\n\n#endif",
  "filePath": "/home/smithkt/git/mod-realm-config/tests/api/Field.h"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 7.3s)

_Thinking:_

Wait, I need to double check: is `std::uint64_t` and `uint64` the same type in the specialization? In my Common.h, `using uint64 = std::uint64_t;`. So `Field::Get<uint64>` == `Field::Get<std::uint64_t>`, which is resolved by the std::uint64_t specialization. Good. And `uint64` is never used in Get in production (uses `Get<uint64>`), so it maps to the std::uint64_t specialization.

Now QueryResult stub.

**Tool: write**

**Input:**
```json
{
  "content": "#ifndef REALM_CONFIG_TEST_QUERYRESULT_H\n#define REALM_CONFIG_TEST_QUERYRESULT_H\n\n#include \"Common.h\"\n#include \"Field.h\"\n\n#include <cstddef>\n#include <memory>\n#include <utility>\n#include <vector>\n\n// Test-only double for AzerothCore's QueryResult. Holds in-memory Field rows.\n// The cursor starts on the first row; Fetch() returns the current row, and\n// NextRow() advances. A default-constructed value is null/failed.\nclass QueryResult\n{\npublic:\n    using Row = std::vector<Field>;\n\n    QueryResult() = default;\n    explicit QueryResult(std::vector<Row> rows)\n        : _rows(new std::vector<Row>(std::move(rows))) {}\n\n    explicit operator bool() const { return static_cast<bool>(_rows); }\n\n    QueryResult const& operator->() const { return *this; }\n\n    std::uint32_t GetFieldCount() const\n    {\n        if (!_rows || _index >= _rows->size()) return 0;\n        return static_cast<std::uint32_t>((*_rows)[_index].size());\n    }\n\n    Field* Fetch() const\n    {\n        if (!_rows || _index >= _rows->size()) return nullptr;\n        return (*_rows)[_index].data();\n    }\n\n    bool NextRow() const\n    {\n        if (!_rows || _index + 1 >= _rows->size()) return false;\n        ++_index;\n        return true;\n    }\n\nprivate:\n    std::shared_ptr<std::vector<Row>> _rows;\n    mutable std::size_t _index = 0;\n};\n\n#endif",
  "filePath": "/home/smithkt/git/mod-realm-config/tests/api/QueryResult.h"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 4.1s)

**Tool: write**

**Input:**
```json
{
  "content": "#ifndef REALM_CONFIG_TEST_DATABASEENV_H\n#define REALM_CONFIG_TEST_DATABASEENV_H\n\n#include \"Common.h\"\n#include \"QueryResult.h\"\n\n#include <functional>\n#include <string>\n#include <utility>\n\n// Test-only double for the world/auth database worker pools. Queries return a\n// null QueryResult by default; tests exercise the parser/serializer directly\n// rather than through the pool.\nclass QueryCallback\n{\npublic:\n    template<class Callback>\n    QueryCallback& WithCallback(Callback&& callback)\n    {\n        _callback = std::forward<Callback>(callback);\n        return *this;\n    }\n\n    bool InvokeIfReady()\n    {\n        if (_callback)\n        {\n            _callback(QueryResult());\n            return true;\n        }\n        return false;\n    }\n\nprivate:\n    std::function<void(QueryResult)> _callback;\n};\n\nclass DatabaseWorkerPool\n{\npublic:\n    QueryResult Query(std::string const&) { return QueryResult(); }\n\n    template<class... Args>\n    QueryResult Query(std::string const&, Args&&...) { return QueryResult(); }\n\n    QueryCallback AsyncQuery(std::string const&) { return QueryCallback(); }\n};\n\nextern DatabaseWorkerPool WorldDatabase;\nextern DatabaseWorkerPool LoginDatabase;\n\n#endif",
  "filePath": "/home/smithkt/git/mod-realm-config/tests/api/DatabaseEnv.h"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 2.8s)

**Tool: write**

**Input:**
```json
{
  "content": "#ifndef REALM_CONFIG_TEST_LOG_H\n#define REALM_CONFIG_TEST_LOG_H\n\n// Test-only doubles for AzerothCore's logging macros. The standalone suite\n// asserts on return values/exceptions, not on emitted log lines.\n#define LOG_ERROR(category, ...) ((void)0)\n#define LOG_WARN(category, ...) ((void)0)\n#define LOG_INFO(category, ...) ((void)0)\n#define LOG_DEBUG(category, ...) ((void)0)\n\n#endif",
  "filePath": "/home/smithkt/git/mod-realm-config/tests/api/Log.h"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 6.4s)

**Tool: write**

**Input:**
```json
{
  "content": "#ifndef REALM_CONFIG_TEST_SCRIPTMGR_H\n#define REALM_CONFIG_TEST_SCRIPTMGR_H\n\n#include \"Common.h\"\n\n// Test-only double for AzerothCore's WorldScript base class. Registration and\n// ScriptMgr routing are not exercised by the standalone suite.\nclass WorldScript\n{\npublic:\n    explicit WorldScript(char const* /*name*/) {}\n    virtual ~WorldScript() = default;\n\n    virtual void OnStartup() {}\n    virtual void OnAfterConfigLoad(bool /*reload*/) {}\n    virtual void OnUpdate(uint32 /*diff*/) {}\n};\n\n#endif",
  "filePath": "/home/smithkt/git/mod-realm-config/tests/api/ScriptMgr.h"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 5.1s)

**Tool: write**

**Input:**
```json
{
  "content": "#ifndef REALM_CONFIG_TEST_WORLD_H\n#define REALM_CONFIG_TEST_WORLD_H\n\n// Included for parity with the production translation unit. No symbols are\n// consumed from World.h by mod-realm-config.\n#endif",
  "filePath": "/home/smithkt/git/mod-realm-config/tests/api/World.h"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 3.2s)

**Tool: write**

**Input:**
```json
{
  "content": "#ifndef REALM_CONFIG_TEST_REALM_H\n#define REALM_CONFIG_TEST_REALM_H\n\n#include \"Common.h\"\n\n#include <string>\n\n// Test-only double for the current realm identity advertised by worldserver.\nstruct RealmId\n{\n    uint32 Realm = 0;\n};\n\nstruct Realm\n{\n    std::string Name;\n    RealmId Id;\n};\n\nextern Realm realm;\n\n#endif",
  "filePath": "/home/smithkt/git/mod-realm-config/tests/api/Realm.h"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 3.8s)

**Tool: write**

**Input:**
```json
{
  "content": "#ifndef REALM_CONFIG_TEST_CONFIG_H\n#define REALM_CONFIG_TEST_CONFIG_H\n\n#include \"Common.h\"\n\n#include <string>\n\n// Test-only double for the module configuration manager. The standalone suite\n// does not drive ReloadSettings, so options always return their default.\nclass ConfigMgr\n{\npublic:\n    template<class T>\n    T GetOption(std::string const& /*key*/, T const& fallback) const { return fallback; }\n};\n\nextern ConfigMgr* sConfigMgr;\n\n#endif",
  "filePath": "/home/smithkt/git/mod-realm-config/tests/api/Config.h"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 114.7s)

_Thinking:_

Now write the test suite. Let me carefully structure it.

I need to define the globals in the test TU (since the cpp declares them extern): `ConfigMgr* sConfigMgr;`, `DatabaseWorkerPool WorldDatabase;`, `DatabaseWorkerPool LoginDatabase;`, `Realm realm;`.

Wait — Config.h declares `extern ConfigMgr* sConfigMgr;`. The test defines it. Good.

Now, the test includes "../src/mod_realm_config.cpp" which includes the eight headers — resolved from tests/api via -I.

Also, note production `#include "Realm.h"` uses `Realm realm` global — declared extern in stub. Test defines `Realm realm;` at namespace scope.

Now writing the test main. Let me build helper functions:

```cpp
#include "../src/mod_realm_config.cpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
```

Wait: mod_realm_config.cpp already includes most STL. The test needs <iostream> for printing failures, <cstdio> maybe. Include <iostream>.

Let me write the CHECK macros and helpers.

```cpp
namespace
{
int g_checks = 0;
int g_failures = 0;

#define REQUIRE(cond) do { ++g_checks; if (!(cond)) { ++g_failures; \
    std::cerr << "FAIL(" << __FILE__ << ":" << __LINE__ << "): " #cond << "\n"; } } while (0)

#define REQUIRE_THROW(expr) do { ++g_checks; bool threw = false; \
    try { (void)(expr); } catch (std::exception const&) { threw = true; } \
    if (!threw) { ++g_failures; std::cerr << "FAIL(" << __FILE__ << ":" << __LINE__ << "): expected throw: " #expr "\n"; } } while (0)
```

Note: REQUIRE_THROW with `(void)(expr)` — expr like `ReadContentContract(QueryResult())` produces ContentContract; unused → cast void. Good.

Now helpers.

Settings helper:
```cpp
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
```

Wait — the singleton row for LoadMetadata requires 19 f-values. Let me build the singleton row with those exact values.

```cpp
QueryResult::Row SingletonRow()
{
    return MetadataRow("0", "", {
        "test", "Test Realm", "realm.example.com", "", "https://example.com/",
        "3.3.5a", "12340", "3724", "8085", "https://example.com/realm.conf",
        "Wow.exe", "", "0.1.0", "", "", "", "", "",
        ""});
}
```
19 values: 
0 realm_key "test"
1 name "Test Realm"
2 address "realm.example.com"
3 description ""
4 website_url "https://example.com/"
5 client_version "3.3.5a"
6 client_build "12340"
7 auth_port "3724"
8 world_port "8085"
9 config_url "https://example.com/realm.conf"
10 client_executable "Wow.exe"
11 client_executable_sha256 ""
12 portalkeeper_minimum_version "0.1.0"
13 manifest_url ""
14 news_url ""
15 status_url ""
16 calendar_url ""
17 armory_url ""
18 content_base_url ""

That's 19. Good.

```cpp
QueryResult::Row MetadataRow(std::string const& type, std::string const& key, std::vector<std::string> values)
{
    QueryResult::Row row;
    row.emplace_back(type);
    row.emplace_back(std::uint64_t(0));
    row.emplace_back(key);
    for (std::size_t i = 0; i < 19; ++i)
        row.emplace_back(i < values.size() ? values[i] : "");
    return row;
}
```

Content-Build row:
```cpp
QueryResult::Row BuildRow(std::string const& number, std::string const& realmName,
    std::string const& filename, std::string const& sha256)
{
    return MetadataRow("3", "", {number, realmName, filename, sha256});
}
```

Requirement row:
```cpp
QueryResult::Row RequirementRow(std::string const& requirement, std::string const& number)
{
    return MetadataRow("4", requirement, {number});
}
```

LoadMetadata helper:
```cpp
Settings Load(std::vector<QueryResult::Row> rows)
{
    return LoadMetadata(QueryResult(std::move(rows)), "/tmp/realm-config-out");
}
```

Addon row? For LoadMetadata with addon/patch rows:
addon: MetadataRow("1", "ExampleAddon", {"Example Addon","Required","GitHub","https://github.com/example/ExampleAddon","main","ExampleAddon"}) — 6 values (name, requirement, source_type, source_url, source_ref, install_directory).

patch (File mode): MetadataRow("2", "ExamplePatch", {"Example Realm Patch","Required","HTTP","https://example.com/patch-X.MPQ","patch-X.MPQ","Data",""}) — 7 values (name, requirement, source_type, source_url, file_name, install_directory, sha256).

Wait LoadMetadata pushes Patch with fields[2] as key and value(0..6) → the Patch struct fields: Key, Name, Requirement, SourceType, SourceURL, FileName, InstallDirectory, SHA256. That's 8 total (Key from fields[2], then Name=value0, Requirement=value1, SourceType=value2, SourceURL=value3, FileName=value4, InstallDirectory=value5, SHA256=value6). Good.

But the real patch SQL select for type '2' uses:
f0 name, f1 requirement, f2 source_type, f3 source_url, f4 file_name, f5 install_directory, f6 sha256. Yes matches.

Now golden expected string. Let me carefully produce it as raw string. BuildConfiguration output:

header:
```
# Generated by mod-realm-config.
# Public configuration consumed by Portalkeeper.
# Do not edit this file manually.
```
then AppendSection "Config": "\n[Config]\nSchemaVersion=1\n" → so after header there's "\n" then section.

Full expected for MakeSettings + addon + patch (File mode):

```
# Generated by mod-realm-config.
# Public configuration consumed by Portalkeeper.
# Do not edit this file manually.

[Config]
SchemaVersion=1

[Realm]
Name=Test Realm
Description=
WebsiteURL=https://example.com/

[Connection]
Address=realm.example.com
AuthPort=3724
WorldPort=8085

[Client]
Version=3.3.5a
Build=12340
Executable=Wow.exe
ExecutableSHA256=
RuntimeMode=Legacy
Requirements=

[Portalkeeper]
MinimumVersion=0.1.0

[Services]
ManifestURL=
NewsURL=
StatusURL=
CalendarURL=
ArmoryURL=
ConfigURL=https://example.com/realm.conf

[Addon.ExampleAddon]
Name=Example Addon
Requirement=Required
SourceType=GitHub
SourceURL=https://github.com/example/ExampleAddon
Ref=main
InstallDirectory=ExampleAddon

[Patch.ExamplePatch]
Name=Example Realm Patch
Requirement=Required
SourceType=HTTP
SourceURL=https://example.com/patch-X.MPQ
FileName=patch-X.MPQ
InstallDirectory=Data
SHA256=
```

The file ends with "SHA256=\n". Native raw literal keeps \n. My C++ string for comparison must end with '\n'. I'll build expected via a std::string with embedded newlines ending in "\n".

Note GameRealmName absent (unset). 

Now, VerifyClientSection helper: extract [Client] section lines from output.

```cpp
std::vector<std::string> SectionLines(std::string const& output, std::string const& name)
{
    std::vector<std::string> out;
    std::istringstream ss(output);
    std::string line;
    bool inside = false;
    while (std::getline(ss, line))
    {
        if (line.rfind("[", 0) == 0)
        {
            inside = (line == "[" + name + "]");
            continue;
        }
        if (inside && !line.empty()) out.push_back(line);
    }
    return out;
}
```

Now Let me also test ValidateSettings end-to-end with synthesized realm-content and requirements. For that, settings must be fully valid. Let me write the end-to-end pipeline function:

```cpp
std::string RunPublication(Settings settings, bool content, std::string const& realmName)
{
    auto contentStatus = SynthesizeRealmContent(settings, content, realmName);
    ValidateSettings(settings);
    return BuildConfiguration(settings);
}
```

This mirrors AcceptResult's core (minus GameRealmName and publish). For fail-safe test:

```cpp
bool PublicationsFails(std::string& outWhere)
{
    // returns false if an exception is thrown before generating output
    ...
}
```

Actually I'll test fail-safe by calling RunPublication variants inside try/catch and asserting that on bad requirement row (from LoadMetadata), RunPublication throws so no output is produced. The "existing file preserved" final step is PublishConfiguration not being reached. I'll demonstrate with PublishConfiguration: create a temp dir, publish a known-good file, then attempt pipeline that throws (bad requirement) — the known-good file remains unchanged. That's a solid behavioral test of "preserves last known-good file".

So test_fail_safe_preserves_file:
1. tempdir
2. settings valid with owned build + one bad requirement (e.g., value with comma) → but wait, LoadMetadata would parse it via set insertion; no validation at parse → Synthesize attaches → ValidateSettings throws. 
   To construct: settings.ActiveBuilds.front().ClientRequirements = {"protected-framexml,evil"} (contains comma) and content path advertising. Actually simpler: a requirement string that fails ValidateClientRequirement, e.g., "requires space" or "bad,comma".
3. PublishConfiguration(settings, BuildConfiguration(settings with empty requirements)) to establish the known-good file.
4. Then try pipeline: SynthesizeRealmContent (attaches bad requirement) → ValidateSettings throws → produced buffer not written. Assert exception thrown and file content still equals known-good content.

That directly tests #10's fail-safe preservation.

Also test Level "no content manager": RunPublication with content=false returns output with Requirements= and no realm-content patch.

Also test "foreign realm": run publication with build realmName different → no patch, Requirements=, status message contains "not advertised".

Alright. Let me also incorporate the `realm` global: set `realm.Name` in main to "Test Realm" and `realm.Id.Realm` = 1. But RunPublication takes realmName param so global not strictly needed.

Now — PublishConfiguration writes to OutputDirectory; in the fail-safe test I set OutputDirectory to tempdir.

One concern: PublishConfiguration uses fs::create_directories, staging dir with timestamp+sequence, rename. On the same filesystem fine.

Let me also test PublishConfiguration round-trip directly.

Let me now write the full test file. I'll be careful about the golden string quoting.

Also I want a test asserting MetadataSQL doesn't reference requirements table when contract.requirements=false, and DOES reference it with ACTIVE filter when true. And that when contract.content=false, no '3'/'4' clauses and query is base + ORDER BY.

Also verify the ORDER BY and record_key=requirement ordering — MetadataSQL output contains "ORDER BY record_type, sort_order, BINARY record_key". And for the '4' block, requirement is placed as third column so sorting is byte-wise via BINARY record_key. I'll assert the '4' SELECT contains "CONVERT(requirement USING utf8mb4)" positioned after the sort order literal. That's a bit brittle; simpler to assert the substring "content_manager_build_client_requirement" and "WHERE build_number IN" and "state = 'ACTIVE'". Good.

Let me write it.

Also important: the LoadMetadata test for orphan requirement rows — build 99 not in active builds → throws. Let me craft rows: singleton + BuildRow("12", ...) + RequirementRow("protected-framexml", "99").

Also test that legitimately, requirement rows for the ACTIVE build match and are attached.

Also test that two ACTIVE build rows → Synthesize throws "multiple ACTIVE". (Existing behavior.)

Edge: content_base_url empty with owned build → returns "available; content_base_url is unset", no requirements attached. Test.

Let me also confirm `ValidateSettings` on the golden no-content settings passes (OutputDirectory path must pass ValidateText: "/tmp/realm-config-out" — ValidateText allows '/'? It checks control chars and leading/trailing spaces. "/tmp/..." fine.) ValidateRealmKey "test" ok. Name "Test Realm" ok. Address "realm.example.com" — check: `settings.Address.find_first_of(" /\\\"'\t=#;@")` — none. Colon check: no colon. ok. AuthPort validate number. Executable "Wow.exe" ValidateRelativePath(true) — "Wow.exe" single component fine. MinimumVersion "0.1.0" free text ok. URLs ok. catalog validations: addon key "ExampleAddon" ok; source GitHub; URL https ok; InstallDirectory "ExampleAddon" single component ok. patch key "ExamplePatch"; FileName "patch-X.MPQ"; InstallDirectory "Data"; sha256 "" ok. Good.

For content: set ContentBaseURL "https://example.com/download/" and build filename "Test-Realm-Content-000012.mpq", sha256 64 hex. Realm name "Test Realm" matches. contentStatus would attach patch and requirements. ValidateSettings then validates realm-content patch: key "realm-content"; name "Test Realm Realm Content"; "Required"; HTTP; URL joined "https://example.com/download/Test-Realm-Content-000012.mpq" ; WowPatch → FileName/InstallDirectory empty ok; SHA256 must be nonempty and valid 64 hex. Use "0123456789abcdef"*4 = 64 chars.

Let me now also think about a subtle point in SynthesizeRealmContent: it calls ValidateText(canonicalRealmName,...). canonicalRealmName "Test Realm" fine. build.RealmName "Test Realm" fine. ValidateURL(content_base_url) — passes. `settings.ContentBaseURL.find_first_of("?#")` — none. Patch key check loop. Filename ASCII check — "Test-Realm-Content-000012.mpq" chars all in [alnum._-]. Good.

Then ValidateSettings: synthed patch entry SHA256 valid.

Now writing the actual test file.

One more: In LoadMetadata, the `value` lambda: `fields[i+3]`. For type '4', load reads fields[2] and fields[3]. My MetadataRow puts key in position 2 (index2) and values in index 3..; so RequirementRow("protected-framexml", "12") gives row[2]="protected-framexml", row[3]="12". LoadMetadata: `buildRequirements[fields[3].Get<std::string>()].insert(fields[2].Get<std::string>());` → buildRequirements["12"].insert("protected-framexml"). 

Now the tests for JoinClientRequirements via settings.ClientRequirements in BuildConfiguration: BuildConfiguration calls JoinClientRequirements which sorts+dedups anyway. So even if I set settings.ClientRequirements unsorted with dupes, output is sorted deduped. I'll test that directly too.

Let me write the file now.

Also: define constants for 64-hex sha256:
`std::string const SHA = std::string(64, 'a');`

Let me also add the test: "requirements come from ACTIVE build, not package state" — behavioral: settings.ActiveBuilds has the owned ACTIVE build with requirement R; and there are (hypothetically) other "package state" but module never reads package state. I'll frame it as: BuildConfiguration output Requirements reflects build.ClientRequirements value only, i.e., changing package-like data (manual patches / addons) does not change Requirements. That's evidenced by the golden + content tests. Also MetadataSQL requirement row only for ACTIVE build filter. I'll include an assertion: MetadataSQL({true,true}) contains "state = 'ACTIVE'" near the requirement clause, and that the WHERE is on content_manager_build ACTIVE.

Let me write now.

Now the test suite:

**Tool: write**

**Input:**
```json
{
  "content": "// Standalone regression/feature suite for mod-realm-config.\n//\n// Compiles the production translation unit (src/mod_realm_config.cpp) against\n// small AzerothCore API doubles (tests/api/*.h) and exercises the parser,\n// contract detection, content synthesis, validator, serializer, and publisher.\n// Run with `bash tests/run.sh`.\n\n#include \"../src/mod_realm_config.cpp\"\n\n#include <iostream>\n#include <sstream>\n#include <stdexcept>\n#include <string>\n#include <vector>\n\nnamespace\n{\nint g_checks = 0;\nint g_failures = 0;\n\n#define REQUIRE(cond)                                                        \\\n    do {                                                                     \\\n        ++g_checks;                                                          \\\n        if (!(cond)) {                                                       \\\n            ++g_failures;                                                    \\\n            std::cerr << \"FAIL(\" << __FILE__ << \":\" << __LINE__ << \"): \"     \\\n                      << #cond << \"\\n\";                                      \\\n        }                                                                    \\\n    } while (0)\n\n#define REQUIRE_THROW(expr)                                                  \\\n    do {                                                                     \\\n        ++g_checks;                                                          \\\n        bool threw_ = false;                                                 \\\n        try { (void)(expr); } catch (std::exception const&) { threw_ = true; } \\\n        if (!threw_) {                                                       \\\n            ++g_failures;                                                    \\\n            std::cerr << \"FAIL(\" << __FILE__ << \":\" << __LINE__              \\\n                      << \"): expected throw: \" #expr \"\\n\";                   \\\n        }                                                                    \\\n    } while (0)\n\nstd::string const SHA64 = std::string(64, 'a');\n\nSettings MakeSettings()\n{\n    Settings s;\n    s.OutputDirectory = \"/tmp/realm-config-out\";\n    s.RealmKey = \"test\";\n    s.Name = \"Test Realm\";\n    s.Address = \"realm.example.com\";\n    s.Description = \"\";\n    s.WebsiteURL = \"https://example.com/\";\n    s.ClientVersion = \"3.3.5a\";\n    s.ClientBuild = \"12340\";\n    s.AuthPort = \"3724\";\n    s.WorldPort = \"8085\";\n    s.ConfigURL = \"https://example.com/realm.conf\";\n    s.Executable = \"Wow.exe\";\n    s.ExecutableSHA256 = \"\";\n    s.MinimumVersion = \"0.1.0\";\n    s.ManifestURL = \"\";\n    s.NewsURL = \"\";\n    s.StatusURL = \"\";\n    s.CalendarURL = \"\";\n    s.ArmoryURL = \"\";\n    s.ContentBaseURL = \"\";\n    return s;\n}\n\nvoid AddExampleCatalog(Settings& s)\n{\n    s.Addons.push_back({\"ExampleAddon\", \"Example Addon\", \"Required\", \"GitHub\",\n        \"https://github.com/example/ExampleAddon\", \"main\", \"ExampleAddon\"});\n    s.Patches.push_back({\"ExamplePatch\", \"Example Realm Patch\", \"Required\", \"HTTP\",\n        \"https://example.com/patch-X.MPQ\", \"patch-X.MPQ\", \"Data\", \"\",\n        PatchInstallMode::File});\n}\n\nQueryResult::Row MetadataRow(std::string const& type, std::string const& key,\n    std::vector<std::string> const& values)\n{\n    QueryResult::Row row;\n    row.emplace_back(type);\n    row.emplace_back(std::uint64_t(0));\n    row.emplace_back(key);\n    for (std::size_t i = 0; i < 19; ++i)\n        row.emplace_back(i < values.size() ? values[i] : \"\");\n    return row;\n}\n\nQueryResult::Row SingletonRow()\n{\n    return MetadataRow(\"0\", \"\", {\n        \"test\", \"Test Realm\", \"realm.example.com\", \"\", \"https://example.com/\",\n        \"3.3.5a\", \"12340\", \"3724\", \"8085\", \"https://example.com/realm.conf\",\n        \"Wow.exe\", \"\", \"0.1.0\", \"\", \"\", \"\", \"\", \"\",\n        \"\"});\n}\n\nQueryResult::Row BuildRow(std::string const& number, std::string const& realmName,\n    std::string const& filename, std::string const& sha256)\n{\n    return MetadataRow(\"3\", \"\", {number, realmName, filename, sha256});\n}\n\nQueryResult::Row RequirementRow(std::string const& requirement, std::string const& number)\n{\n    return MetadataRow(\"4\", requirement, {number});\n}\n\nQueryResult::Row AddonRow()\n{\n    return MetadataRow(\"1\", \"ExampleAddon\", {\"Example Addon\", \"Required\", \"GitHub\",\n        \"https://github.com/example/ExampleAddon\", \"main\", \"ExampleAddon\"});\n}\n\nQueryResult::Row PatchRow()\n{\n    return MetadataRow(\"2\", \"ExamplePatch\", {\"Example Realm Patch\", \"Required\", \"HTTP\",\n        \"https://example.com/patch-X.MPQ\", \"patch-X.MPQ\", \"Data\", \"\"});\n}\n\nSettings Load(std::vector<QueryResult::Row> rows)\n{\n    return LoadMetadata(QueryResult(std::move(rows)), \"/tmp/realm-config-out\");\n}\n\nstd::vector<std::string> SectionLines(std::string const& output, std::string const& name)\n{\n    std::vector<std::string> lines;\n    std::istringstream ss(output);\n    std::string line;\n    bool inside = false;\n    while (std::getline(ss, line))\n    {\n        if (!line.empty() && line.front() == '[')\n        {\n            inside = (line == \"[\" + name + \"]\");\n            continue;\n        }\n        if (inside && !line.empty()) lines.push_back(line);\n    }\n    return lines;\n}\n\nbool Contains(std::string const& haystack, std::string const& needle)\n{\n    return haystack.find(needle) != std::string::npos;\n}\n\nstd::string RunPublication(Settings settings, bool content, std::string const& realmName)\n{\n    SynthesizeRealmContent(settings, content, realmName);\n    ValidateSettings(settings);\n    return BuildConfiguration(settings);\n}\n}\n// namespace\n\nint main()\n{\n    sConfigMgr = new ConfigMgr();\n    realm.Name = \"Test Realm\";\n    realm.Id.Realm = 1;\n\n    {\n        // 1/13. Golden output: existing generation stays valid and unchanged\n        // apart from the new Requirements= line.\n        auto settings = MakeSettings();\n        AddExampleCatalog(settings);\n        auto output = BuildConfiguration(settings);\n        std::string const expected =\n            \"# Generated by mod-realm-config.\\n\"\n            \"# Public configuration consumed by Portalkeeper.\\n\"\n            \"# Do not edit this file manually.\\n\"\n            \"\\n[Config]\\nSchemaVersion=1\\n\"\n            \"\\n[Realm]\\nName=Test Realm\\nDescription=\\nWebsiteURL=https://example.com/\\n\"\n            \"\\n[Connection]\\nAddress=realm.example.com\\nAuthPort=3724\\nWorldPort=8085\\n\"\n            \"\\n[Client]\\nVersion=3.3.5a\\nBuild=12340\\nExecutable=Wow.exe\\n\"\n            \"ExecutableSHA256=\\nRuntimeMode=Legacy\\nRequirements=\\n\"\n            \"\\n[Portalkeeper]\\nMinimumVersion=0.1.0\\n\"\n            \"\\n[Services]\\nManifestURL=\\nNewsURL=\\nStatusURL=\\nCalendarURL=\\n\"\n            \"ArmoryURL=\\nConfigURL=https://example.com/realm.conf\\n\"\n            \"\\n[Addon.ExampleAddon]\\nName=Example Addon\\nRequirement=Required\\n\"\n            \"SourceType=GitHub\\nSourceURL=https://github.com/example/ExampleAddon\\n\"\n            \"Ref=main\\nInstallDirectory=ExampleAddon\\n\"\n            \"\\n[Patch.ExamplePatch]\\nName=Example Realm Patch\\nRequirement=Required\\n\"\n            \"SourceType=HTTP\\nSourceURL=https://example.com/patch-X.MPQ\\n\"\n            \"FileName=patch-X.MPQ\\nInstallDirectory=Data\\nSHA256=\\n\";\n        REQUIRE(output == expected);\n\n        // 2. Existing [Client] keys are unchanged; Requirements is additive.\n        auto client = SectionLines(output, \"Client\");\n        REQUIRE(client.size() == 6);\n        REQUIRE(client[0] == \"Version=3.3.5a\");\n        REQUIRE(client[1] == \"Build=12340\");\n        REQUIRE(client[2] == \"Executable=Wow.exe\");\n        REQUIRE(client[3] == \"ExecutableSHA256=\");\n        REQUIRE(client[4] == \"RuntimeMode=Legacy\");\n        REQUIRE(client[5] == \"Requirements=\");\n    }\n\n    {\n        // 11/12. RuntimeMode Legacy and Isolated behavior is unchanged.\n        auto legacy = MakeSettings();\n        legacy.ClientRuntimeMode = \"Legacy\";\n        auto outLegacy = BuildConfiguration(legacy);\n        REQUIRE(Contains(outLegacy, \"RuntimeMode=Legacy\\nRequirements=\"));\n\n        auto isolated = MakeSettings();\n        isolated.ClientRuntimeMode = \"Isolated\";\n        auto outIsolated = BuildConfiguration(isolated);\n        REQUIRE(Contains(outIsolated, \"RuntimeMode=Isolated\\nRequirements=\"));\n    }\n\n    {\n        // JoinClientRequirements: empty, single, deduplicated+deterministic.\n        REQUIRE(JoinClientRequirements({}) == \"\");\n        REQUIRE(JoinClientRequirements({\"protected-framexml\"}) == \"protected-framexml\");\n        REQUIRE(JoinClientRequirements({\"b\", \"a\", \"b\", \"c\"}) == \"a,b,c\");\n        REQUIRE(JoinClientRequirements({\"zesty\", \"apple\", \"banana\"}) == \"apple,banana,zesty\");\n    }\n\n    {\n        // 3. No Content Manager integration: generation still succeeds and\n        // requirements are empty.\n        auto settings = MakeSettings();\n        AddExampleCatalog(settings);\n        auto output = RunPublication(settings, false, \"Test Realm\");\n        REQUIRE(Contains(output, \"Requirements=\\n\"));\n        REQUIRE(!Contains(output, \"Patch.realm-content\"));\n        REQUIRE(settings.ClientRequirements.empty());\n    }\n\n    {\n        // 4. ACTIVE legacy/pre-Schema-3 build with zero requirement rows.\n        auto settings = Load({SingletonRow(),\n            BuildRow(\"12\", \"Test Realm\", \"Test-Realm-Content-000012.mpq\", SHA64)});\n        settings.ContentBaseURL = \"https://example.com/download/\";\n        auto output = RunPublication(settings, true, \"Test Realm\");\n        REQUIRE(Contains(output, \"Requirements=\\n\"));\n        REQUIRE(Contains(output, \"[Patch.realm-content]\"));\n        REQUIRE(settings.ClientRequirements.empty());\n    }\n\n    {\n        // 5. ACTIVE Schema-3 build requiring protected-framexml.\n        auto settings = Load({SingletonRow(),\n            BuildRow(\"12\", \"Test Realm\", \"Test-Realm-Content-000012.mpq\", SHA64),\n            RequirementRow(\"protected-framexml\", \"12\")});\n        settings.ContentBaseURL = \"https://example.com/download/\";\n        REQUIRE(settings.ActiveBuilds.size() == 1);\n        REQUIRE(settings.ActiveBuilds[0].ClientRequirements ==\n            std::vector<std::string>{\"protected-framexml\"});\n        auto output = RunPublication(settings, true, \"Test Realm\");\n        REQUIRE(Contains(output, \"Requirements=protected-framexml\\n\"));\n        REQUIRE(settings.ClientRequirements ==\n            std::vector<std::string>{\"protected-framexml\"});\n    }\n\n    {\n        // 6/7. Multiple returned and duplicate requirements: deterministic,\n        // comma-separated, deduplicated output in byte-wise order.\n        auto settings = Load({SingletonRow(),\n            BuildRow(\"12\", \"Test Realm\", \"Test-Realm-Content-000012.mpq\", SHA64),\n            RequirementRow(\"zesty\", \"12\"),\n            RequirementRow(\"apple\", \"12\"),\n            RequirementRow(\"protected-framexml\", \"12\"),\n            RequirementRow(\"banana\", \"12\"),\n            RequirementRow(\"apple\", \"12\")});\n        settings.ContentBaseURL = \"https://example.com/download/\";\n        REQUIRE(settings.ActiveBuilds[0].ClientRequirements ==\n            std::vector<std::string>{\"apple\", \"banana\", \"protected-framexml\", \"zesty\"});\n        auto output = RunPublication(settings, true, \"Test Realm\");\n        REQUIRE(Contains(output,\n            \"Requirements=apple,banana,protected-framexml,zesty\\n\"));\n        REQUIRE(!Contains(output, \"Requirements=,\"));\n    }\n\n    {\n        // 8. Requirements source: the requirement SELECT reads the exact ACTIVE\n        // build rows only, never package/EPF/patchhold state.\n        auto sqlBoth = MetadataSQL({true, true});\n        REQUIRE(Contains(sqlBoth, \"content_manager_build_client_requirement\"));\n        REQUIRE(Contains(sqlBoth, \"WHERE build_number IN (SELECT build_number \"\n            \"FROM content_manager_build WHERE BINARY state = 'ACTIVE')\"));\n        auto sqlContentOnly = MetadataSQL({true, false});\n        REQUIRE(!Contains(sqlContentOnly, \"content_manager_build_client_requirement\"));\n        REQUIRE(Contains(sqlContentOnly, \"content_manager_build\"));\n        auto sqlNone = MetadataSQL({false, false});\n        REQUIRE(!Contains(sqlNone, \"content_manager_build\"));\n        // Requirements stay attached to the build object independently of any\n        // catalog (package) rows loaded in the same snapshot.\n        auto settings = Load({SingletonRow(), AddonRow(), PatchRow(),\n            BuildRow(\"12\", \"Test Realm\", \"Test-Realm-Content-000012.mpq\", SHA64),\n            RequirementRow(\"protected-framexml\", \"12\")});\n        REQUIRE(settings.Addons.size() == 1);\n        REQUIRE(settings.Patches.size() == 1);\n        REQUIRE(settings.ActiveBuilds[0].ClientRequirements ==\n            std::vector<std::string>{\"protected-framexml\"});\n    }\n\n    {\n        // 9. ACTIVE build belonging to another realm: neither its patch nor its\n        // requirements are advertised.\n        auto settings = Load({SingletonRow(),\n            BuildRow(\"20\", \"Other Realm\", \"Other-Realm-Content-000020.mpq\", SHA64),\n            RequirementRow(\"protected-framexml\", \"20\")});\n        settings.ContentBaseURL = \"https://example.com/download/\";\n        auto status = SynthesizeRealmContent(settings, true, \"Test Realm\");\n        REQUIRE(Contains(status, \"not advertised\"));\n        REQUIRE(settings.Patches.empty());\n        REQUIRE(settings.ClientRequirements.empty());\n        auto output = BuildConfiguration(settings);\n        REQUIRE(!Contains(output, \"Patch.realm-content\"));\n        REQUIRE(Contains(output, \"Requirements=\\n\"));\n    }\n\n    {\n        // ACTIVE build owned by the realm is advertised with its requirements.\n        auto settings = MakeSettings();\n        settings.ContentBaseURL = \"https://example.com/download/\";\n        settings.ActiveBuilds.push_back({\"30\", \"Test Realm\",\n            \"Test-Realm-Content-000030.mpq\", SHA64, {\"protected-framexml\"}});\n        auto status = SynthesizeRealmContent(settings, true, \"Test Realm\");\n        REQUIRE(Contains(status, \"available; ACTIVE realm content build 30\"));\n        REQUIRE(settings.Patches.size() == 1);\n        REQUIRE(settings.Patches[0].Key == \"realm-content\");\n        REQUIRE(settings.Patches[0].InstallMode == PatchInstallMode::WowPatch);\n        REQUIRE(settings.ClientRequirements ==\n            std::vector<std::string>{\"protected-framexml\"});\n    }\n\n    {\n        // content_base_url unset: no build is advertised, so no requirements.\n        auto settings = MakeSettings();\n        settings.ActiveBuilds.push_back({\"12\", \"Test Realm\",\n            \"Test-Realm-Content-000012.mpq\", SHA64, {\"protected-framexml\"}});\n        auto status = SynthesizeRealmContent(settings, true, \"Test Realm\");\n        REQUIRE(Contains(status, \"content_base_url is unset\"));\n        REQUIRE(settings.Patches.empty());\n        REQUIRE(settings.ClientRequirements.empty());\n    }\n\n    {\n        // Multiple ACTIVE builds remain ambiguous and are rejected.\n        auto settings = Load({SingletonRow(),\n            BuildRow(\"12\", \"Test Realm\", \"Test-Realm-Content-000012.mpq\", SHA64),\n            BuildRow(\"13\", \"Test Realm\", \"Test-Realm-Content-000013.mpq\", SHA64)});\n        settings.ContentBaseURL = \"https://example.com/download/\";\n        REQUIRE_THROW(SynthesizeRealmContent(settings, true, \"Test Realm\"));\n    }\n\n    {\n        // Contract detection: happy paths and incompatible installs.\n        auto contract = ReadContentContract(QueryResult(std::vector<QueryResult::Row>{\n            {Field(std::uint64_t(1)), Field(std::uint64_t(5)),\n             Field(std::uint64_t(1)), Field(std::uint64_t(2))}}));\n        REQUIRE(contract.content && contract.requirements);\n\n        auto legacy = ReadContentContract(QueryResult(std::vector<QueryResult::Row>{\n            {Field(std::uint64_t(1)), Field(std::uint64_t(5)),\n             Field(std::uint64_t(0)), Field(std::uint64_t(0))}}));\n        REQUIRE(legacy.content && !legacy.requirements);\n\n        auto absent = ReadContentContract(QueryResult(std::vector<QueryResult::Row>{\n            {Field(std::uint64_t(0)), Field(std::uint64_t(0)),\n             Field(std::uint64_t(0)), Field(std::uint64_t(0))}}));\n        REQUIRE(!absent.content && !absent.requirements);\n\n        // Incompatible requirement-table columns must not be treated as legacy.\n        REQUIRE_THROW(ReadContentContract(QueryResult(std::vector<QueryResult::Row>{\n            {Field(std::uint64_t(1)), Field(std::uint64_t(5)),\n             Field(std::uint64_t(1)), Field(std::uint64_t(3))}})));\n        REQUIRE_THROW(ReadContentContract(QueryResult(std::vector<QueryResult::Row>{\n            {Field(std::uint64_t(1)), Field(std::uint64_t(4)),\n             Field(std::uint64_t(1)), Field(std::uint64_t(2))}})));\n        REQUIRE_THROW(ReadContentContract(QueryResult()));\n\n        // 10. A failed/absent snapshot cannot masquerade as Requirements=.\n        REQUIRE_THROW(LoadMetadata(QueryResult(), \"/tmp/realm-config-out\"));\n        REQUIRE_THROW(LoadMetadata(QueryResult(std::vector<QueryResult::Row>{\n            SingletonRow(), AddonRow(), PatchRow()}), \"/tmp/realm-config-out\"));\n        // Requirement rows must reference an ACTIVE build from the same snapshot.\n        REQUIRE_THROW(Load(std::vector<QueryResult::Row>{SingletonRow(),\n            BuildRow(\"12\", \"Test Realm\", \"Test-Realm-Content-000012.mpq\", SHA64),\n            RequirementRow(\"protected-framexml\", \"99\")}));\n    }\n\n    {\n        // 10 (behavioral). Requirement lookup failure preserves the last\n        // known-good file instead of silently publishing Requirements=.\n        auto directory = fs::temp_directory_path() /\n            (\"realm-config-failsafe-\" + std::to_string(std::chrono::steady_clock::now()\n                .time_since_epoch().count()));\n        auto knownGood = MakeSettings();\n        knownGood.OutputDirectory = directory.string();\n        AddExampleCatalog(knownGood);\n        PublishConfiguration(knownGood, BuildConfiguration(knownGood));\n\n        auto bad = MakeSettings();\n        bad.OutputDirectory = directory.string();\n        bad.ContentBaseURL = \"https://example.com/download/\";\n        bad.ActiveBuilds.push_back({\"12\", \"Test Realm\",\n            \"Test-Realm-Content-000012.mpq\", SHA64,\n            {\"protected-framexml,extra\"}}); // invalid published token\n        bool published = false;\n        try\n        {\n            auto output = RunPublication(bad, true, \"Test Realm\");\n            PublishConfiguration(bad, output);\n            published = true;\n        }\n        catch (std::exception const&) { }\n\n        auto saved = fs::path(directory) / \"test.realm.conf\";\n        std::ifstream in(saved, std::ios::in | std::ios::binary);\n        std::ostringstream contents; contents << in.rdbuf();\n        REQUIRE(!published);\n        REQUIRE(contents.str() == BuildConfiguration(knownGood));\n        std::error_code ignored;\n        fs::remove_all(directory, ignored);\n    }\n\n    {\n        // Validation: semantically invalid requirement tokens are rejected.\n        auto settings = MakeSettings();\n        settings.ClientRequirements = {\"protected framexml\"};\n        REQUIRE_THROW(ValidateSettings(settings));\n        settings.ClientRequirements = {\"protected-framexml,\"};\n        REQUIRE_THROW(ValidateSettings(settings));\n        settings.ClientRequirements = {\"\"};\n        REQUIRE_THROW(ValidateSettings(settings));\n        settings.ClientRequirements = {std::string(65, 'a')};\n        REQUIRE_THROW(ValidateSettings(settings));\n        settings.ClientRequirements = {\"protected-framexml\"};\n        REQUIRE(SynthesizeRealmContent(settings, false, \"Test Realm\") == \"unavailable\");\n        REQUIRE(ValidateSettings(settings) == void());\n    }\n\n    {\n        // Publisher: atomic publication writes the expected filename/content.\n        auto directory = fs::temp_directory_path() /\n            (\"realm-config-publish-\" + std::to_string(std::chrono::steady_clock::now()\n                .time_since_epoch().count()));\n        auto settings = MakeSettings();\n        settings.OutputDirectory = directory.string();\n        auto output = BuildConfiguration(settings);\n        fs::path target = PublishConfiguration(settings, output);\n        REQUIRE(target == directory / \"test.realm.conf\");\n        REQUIRE(fs::is_regular_file(target));\n        std::ifstream in(target, std::ios::in | std::ios::binary);\n        std::ostringstream contents; contents << in.rdbuf();\n        REQUIRE(contents.str() == output);\n        std::error_code ignored;\n        fs::remove_all(directory, ignored);\n    }\n\n    {\n        // Determinism: identical settings produce byte-identical files.\n        auto a = MakeSettings();\n        AddExampleCatalog(a);\n        auto b = MakeSettings();\n        AddExampleCatalog(b);\n        REQUIRE(BuildConfiguration(a) == BuildConfiguration(b));\n        REQUIRE(BuildConfiguration(a) == BuildConfiguration(a));\n    }\n\n    delete sConfigMgr;\n    if (g_failures)\n    {\n        std::cerr << g_failures << \" of \" << g_checks << \" checks FAILED\\n\";\n        return 1;\n    }\n    std::cout << \"All \" << g_checks << \" checks passed\\n\";\n    return 0;\n}",
  "filePath": "/home/smithkt/git/mod-realm-config/tests/mod_realm_config_tests.cpp"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 28.1s)

_Thinking:_

Wait, there's a bug in my test: `REQUIRE(ValidateSettings(settings) == void());` — ValidateSettings returns void; `(void)ValidateSettings(settings)` can't be a REQUIRE condition. Let me fix: just call `ValidateSettings(settings);` then REQUIRE true. Let me fix that line.

Also there's a subtle issue: `SynthesizeRealmContent` accepts `Settings&` non-const. In test "content_base_url unset" etc. I pass lvalue settings — fine. But `RunPublication` takes `Settings settings` by value and calls `SynthesizeRealmContent(settings, ...)` on the copy — fine.

Also there's an issue with `ValidateSettings(settings)` in the validation test — `settings.ClientRequirements = {"protected framexml"}` — ValidateClientRequirement throws. Good. But note settings has empty Addons/Patches and the rest valid; other validations run first? Order in ValidateSettings: OutputDirectory, RealmKey, Name, Address, Description, ClientVersion, WebsiteURL, ConfigURL, ClientBuild, AuthPort, WorldPort, Executable, ExecutableSHA256, MinimumVersion, manifest/news/status/calendar/armory URLs, then ValidateClientRequirements, then ValidateCatalogs. For "protected framexml" → spaces → ValidateClientRequirement throws. But before that, ValidateText on Description ("" ok), etc. Everything passes until requirements check. Good.

`settings.ClientRequirements = {""}` — empty → throws. Good.
`{std::string(65,'a')}` → >64 → throws. Good.

Now the last part of that test: after setting `{"protected-framexml"}`, `SynthesizeRealmContent(settings, false, ...)` returns "unavailable" (contentAvailable false) without touching settings. Then `ValidateSettings(settings)` should pass. But `REQUIRE(ValidateSettings(settings) == void());` is invalid syntax. Fix to:
```
ValidateSettings(settings);
REQUIRE(true);
```
or simply `REQUIRE(ValidateSettings(settings), true)` no. Let me just call it and add a separate marker check.

Let me fix that.

Also check: `errors` — I referenced `RunPublication` throwing when it fails validation: In the fail-safe test, bad.ClientRequirements = {"protected-framexml,extra"} attached via ActiveBuilds (so Synthesize attaches it to settings.ClientRequirements), then ValidateSettings throws → published stays false. Good.

Wait, but in the fail-safe test the bad settings Addons/Patches are empty; that's fine, ValidateSettings doesn't require catalog.

Also, the `knownGood` settings: AddExampleCatalog + BuildConfiguration. But PublishConfiguration(knownGood, ...) — OutputDirectory tempdir. BuildConfiguration(knownGood) — the standard golden minus... wait, BuildConfiguration(knownGood) has empty OutputDirectory-relevant fields; output includes Requirements= (empty). Good.

One more check: In the "8" test I assert sqlNone does not contain "content_manager_build" — but the base MetadataQuery doesn't contain the content table. ORDER BY line appended. OK.

Actually wait — MetadataSQL({false,false}) returns MetadataQuery + "ORDER BY...". MetadataQuery does not contain "content_manager_build". Good.

Now the compile concern about `RunPublication` returning BuildConfiguration: BuildConfiguration takes Settings const&. Good.

Another compile concern: In test 4/5/6/7 I call `Load({SingletonRow(), BuildRow(...), ...})` where Load takes `std::vector<QueryResult::Row>`. The initializer list of QueryResult::Row temporaries → copies into vector. Fine.

In the publish test, `PublishConfiguration(settings, output)` — settings.OutputDirectory must be an existing-writable dir (temp dir may not exist; PublishConfiguration does create_directories). OK.

Also `PublishConfiguration` uses `fs::rename`. On success target file created.

One concern: In the fail-safe test, `PublishConfiguration(knownGood, BuildConfiguration(knownGood))` writes the golden (with empty Requirements) — that's the "known good". Then bad pipeline throws. Then I read the file back and compare to `BuildConfiguration(knownGood)`. Deterministic — identical. Good.

Now — `REQUIRE(ValidateSettings(settings) == void());` fix. Also the earlier test has another oddity: 

```
settings.ClientRequirements = {"protected-framexml"};
REQUIRE(SynthesizeRealmContent(settings, false, "Test Realm") == "unavailable");
REQUIRE(ValidateSettings(settings) == void());
```

Replace the last line with `ValidateSettings(settings); REQUIRE(true);`. Let me edit.

Also `REQUIRE_THROW(LoadMetadata(QueryResult(std::vector<QueryResult::Row>{SingletonRow(), AddonRow(), PatchRow()}), "/tmp/realm-config-out"))` — result has 22-column rows; LoadMetadata checks `GetFieldCount() != 22` — rows are 22 cols (3 control + 19). Wait: field count must be 22. SingletonRow = 22 fields. AddonRow 22. So field count 22 → passes that check; singleton found; no type '4'; no builds; no requirements; returns fine — NOT throwing! My test comment says "10. A failed/absent snapshot cannot masquerade as Requirements=" but this particular case (singleton+addon+patch, valid 22-col) would NOT throw. I must pick a case that actually throws: e.g., wrong field count (row with 21 fields) → throw. Let me fix: use a row with fewer columns. Or a null result. Let me construct a malformed result with column count != 22:

```cpp
QueryResult::Row shortRow; shortRow.emplace_back("0");
REQUIRE_THROW(LoadMetadata(QueryResult(std::vector<QueryResult::Row>{shortRow}), "/tmp/realm-config-out"));
```

Because GetFieldCount() would be 1 != 22 → throw. Good. Let me replace that block.

Also in the "10 domain" test, `LoadMetadata(QueryResult(), ...)` throws (null). Good.

Also — the `REQUIRE_THROW(Load(...build 12 + requirement 99...))` — orphan → throws. Good.

Also there's `MetadataSQL({true, true})` aggregate init of ContentContract {bool,bool} — ContentContract has two bool members with defaults; aggregate init `{true,true}` fine.

Also `{true, false}` etc.

Also there's a missing include: production .cpp uses std::chrono, std::filesystem, etc. The test also uses `fs::`, `std::chrono::steady_clock`, `std::this_thread`? No. std::chrono is included via cpp. But test uses `std::chrono::steady_clock::now().time_since_epoch().count()` — count returns rep (long long for steady_clock) → std::to_string(rep) — to_string overloads for long long exist (std::to_string(long long)). Good.

The test uses std::ifstream — included by cpp (<fstream>) — I included <sstream>/<iostream> in test. <fstream> comes via cpp includes; since I include the cpp first, headers are available. But best practice: the test should directly include <fstream> too. The cpp includes <fstream>. Since it's the same TU, fine. But to be safe, include <fstream> in test. Add it.

Let me fix the identified issues:
1. `REQUIRE(ValidateSettings(settings) == void());` → invalid; replace.
2. The LoadMetadata "wrong shape" test — use short row.
3. Add <fstream>.

Let me edit the test file.

Let me fix a few issues in the test file I just wrote:

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/tests/mod_realm_config_tests.cpp",
  "newString": "#include <fstream>\n#include <iostream>\n#include <sstream>",
  "oldString": "#include <iostream>\n#include <sstream>"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 5.7s)

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/tests/mod_realm_config_tests.cpp",
  "newString": "// 10. A failed/absent snapshot cannot masquerade as Requirements=.\n        REQUIRE_THROW(LoadMetadata(QueryResult(), \"/tmp/realm-config-out\"));\n        QueryResult::Row shortRow{Field(\"0\"), Field(std::uint64_t(0)), Field(\"\")};\n        REQUIRE_THROW(LoadMetadata(QueryResult(std::vector<QueryResult::Row>{shortRow}),\n            \"/tmp/realm-config-out\"));\n        // Requirement rows must reference an ACTIVE build from the same snapshot.\n        REQUIRE_THROW(Load(std::vector<QueryResult::Row>{SingletonRow(),\n            BuildRow(\"12\", \"Test Realm\", \"Test-Realm-Content-000012.mpq\", SHA64),\n            RequirementRow(\"protected-framexml\", \"99\")}));",
  "oldString": "// 10. A failed/absent snapshot cannot masquerade as Requirements=.\n        REQUIRE_THROW(LoadMetadata(QueryResult(), \"/tmp/realm-config-out\"));\n        REQUIRE_THROW(LoadMetadata(QueryResult(std::vector<QueryResult::Row>{\n            SingletonRow(), AddonRow(), PatchRow()}), \"/tmp/realm-config-out\"));\n        // Requirement rows must reference an ACTIVE build from the same snapshot.\n        REQUIRE_THROW(Load(std::vector<QueryResult::Row>{SingletonRow(),\n            BuildRow(\"12\", \"Test Realm\", \"Test-Realm-Content-000012.mpq\", SHA64),\n            RequirementRow(\"protected-framexml\", \"99\")}));"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 2.3s)

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/tests/mod_realm_config_tests.cpp",
  "newString": "settings.ClientRequirements = {\"protected-framexml\"};\n        REQUIRE(SynthesizeRealmContent(settings, false, \"Test Realm\") == \"unavailable\");\n        ValidateSettings(settings); // valid token round-trips through validation\n        REQUIRE(true);",
  "oldString": "settings.ClientRequirements = {\"protected-framexml\"};\n        REQUIRE(SynthesizeRealmContent(settings, false, \"Test Realm\") == \"unavailable\");\n        REQUIRE(ValidateSettings(settings) == void());"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 4.8s)

_Thinking:_

Now write the run.sh script. Per README: "Run `bash tests/run.sh` with C++17 `g++` (or `$CXX`) and Python 3."

The script:
- compiles the test with g++/CXX
- runs it
- handles staleness
- exits non-zero on failure

Since there's no build dir, compile to a temp location. Let me write a simple, robust run.sh.

Now the test runner script:

**Tool: write**

**Input:**
```json
{
  "content": "#!/usr/bin/env bash\n# Standalone test runner for mod-realm-config.\n#\n# Compiles the production translation unit against small AzerothCore API\n# doubles (tests/api/*.h) and runs the feature/regression suite. Requires a\n# C++17 compiler and no AzerothCore build. Deployment still requires validation\n# of the migration and query strings on MySQL/MariaDB.\nset -euo pipefail\n\nCXX=\"${CXX:-g++}\"\nROOT=\"$(cd \"$(dirname \"${BASH_SOURCE[0]}\")/..\" && pwd)\"\nTEST_DIR=\"$ROOT/tests\"\nBUILD_DIR=\"${TMPDIR:-/tmp}/mod-realm-config-tests-$$\"\n\nmkdir -p \"$BUILD_DIR\"\ntrap 'rm -rf \"$BUILD_DIR\"' EXIT\n\n# Compile into a pristine temp directory so stale artifacts never mask failures.\n\"$CXX\" -std=c++17 -Wall -Wextra -Werror \\\n    -I\"$TEST_DIR/api\" \\\n    \"$TEST_DIR/mod_realm_config_tests.cpp\" \\\n    -o \"$BUILD_DIR/realm_config_tests\"\n\n\"$BUILD_DIR/realm_config_tests\"",
  "filePath": "/home/smithkt/git/mod-realm-config/tests/run.sh"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 2.6s)

**Tool: bash**

**Input:**
```json
{
  "command": "chmod +x /home/smithkt/git/mod-realm-config/tests/run.sh && bash /home/smithkt/git/mod-realm-config/tests/run.sh"
}
```

**Output:**
```
/home/smithkt/git/mod-realm-config/tests/mod_realm_config_tests.cpp:275:87: error: macro ‘REQUIRE’ passed 4 arguments, but takes just 1
  275 |             std::vector<std::string>{"apple", "banana", "protected-framexml", "zesty"});
      |                                                                                       ^
/home/smithkt/git/mod-realm-config/tests/mod_realm_config_tests.cpp:22:9: note: macro ‘REQUIRE’ defined here
   22 | #define REQUIRE(cond)                                                        \
      |         ^~~~~~~
In file included from /home/smithkt/git/mod-realm-config/tests/mod_realm_config_tests.cpp:8:
/home/smithkt/git/mod-realm-config/tests/../src/mod_realm_config.cpp: In function ‘{anonymous}::ContentContract {anonymous}::ReadContentContract(const QueryResult&)’:
/home/smithkt/git/mod-realm-config/tests/../src/mod_realm_config.cpp:186:26: error: circular pointer delegation detected
  186 |     if (!result || result->GetFieldCount() != 4)
      |                          ^~
/home/smithkt/git/mod-realm-config/tests/../src/mod_realm_config.cpp:188:27: error: circular pointer delegation detected
  188 |     Field* fields = result->Fetch();
      |                           ^~
/home/smithkt/git/mod-realm-config/tests/../src/mod_realm_config.cpp: In function ‘{anonymous}::Settings {anonymous}::LoadMetadata(const QueryResult&, const std::string&)’:
/home/smithkt/git/mod-realm-config/tests/../src/mod_realm_config.cpp:247:15: error: circular pointer delegation detected
  247 |     if (result->GetFieldCount() != 22)
      |               ^~
/home/smithkt/git/mod-realm-config/tests/../src/mod_realm_config.cpp:257:31: error: circular pointer delegation detected
  257 |         Field* fields = result->Fetch();
      |                               ^~
/home/smithkt/git/mod-realm-config/tests/../src/mod_realm_config.cpp:294:44: error: missing initializer for member ‘{anonymous}::ActiveContentBuild::ClientRequirements’ [-Werror=missing-field-initializers]
  294 |             settings.ActiveBuilds.push_back({value(0), value(1), value(2), value(3)});
      |             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/home/smithkt/git/mod-realm-config/tests/../src/mod_realm_config.cpp:298:20: error: circular pointer delegation detected
  298 |     } while (result->NextRow());
      |                    ^~
/home/smithkt/git/mod-realm-config/tests/../src/mod_realm_config.cpp: In function ‘std::optional<std::__cxx11::basic_string<char> > {anonymous}::LoadGameRealmName(std::string&)’:
/home/smithkt/git/mod-realm-config/tests/../src/mod_realm_config.cpp:638:30: error: circular pointer delegation detected
  638 |         if (!result || result->GetFieldCount() != 1 || result->Fetch()[0].IsNull())
      |                              ^~
/home/smithkt/git/mod-realm-config/tests/../src/mod_realm_config.cpp:638:62: error: circular pointer delegation detected
  638 |         if (!result || result->GetFieldCount() != 1 || result->Fetch()[0].IsNull())
      |                                                              ^~
/home/smithkt/git/mod-realm-config/tests/../src/mod_realm_config.cpp:640:27: error: circular pointer delegation detected
  640 |         auto name = result->Fetch()[0].Get<std::string>();
      |                           ^~
/home/smithkt/git/mod-realm-config/tests/../src/mod_realm_config.cpp:640:55: error: expected primary-expression before ‘>’ token
  640 |         auto name = result->Fetch()[0].Get<std::string>();
      |                                                       ^
/home/smithkt/git/mod-realm-config/tests/../src/mod_realm_config.cpp:640:57: error: expected primary-expression before ‘)’ token
  640 |         auto name = result->Fetch()[0].Get<std::string>();
      |                                                         ^
/home/smithkt/git/mod-realm-config/tests/mod_realm_config_tests.cpp: In function ‘int main()’:
/home/smithkt/git/mod-realm-config/tests/mod_realm_config_tests.cpp:274:9: error: ‘REQUIRE’ was not declared in this scope
  274 |         REQUIRE(settings.ActiveBuilds[0].ClientRequirements ==
      |         ^~~~~~~
cc1plus: all warnings being treated as errors

```

---

## Assistant (Build · Big Pickle · 26.7s)

_Thinking:_

Multiple issues to fix:

1. **"circular pointer delegation"** — When a class defines `operator->` that returns `*this`, and the class is not a "pointer-like" object... GCC detects that `operator->` returns `*this` (circular delegation) and errors. I need a different design: make QueryResult behave like a shared_ptr by adding `operator->` returning a pointer to an inner object... or drop `operator->` and instead rely on accessing members directly. But the production code uses `result->GetFieldCount()`. In real AzerothCore, `QueryResult` is a class wrapping `QueryResultBase*` where `operator->` returns the inner pointer (not `*this`). GCC's "circular pointer delegation" check: if `operator->` returns `*this`, it's circular. Return-by-same-type is flagged.

   Fix: restructure QueryResult to hold an inner state object, and `operator->` returns a pointer to the inner state. E.g.:
   ```cpp
   struct ResultImpl { ... };
   class QueryResult {
       std::shared_ptr<ResultImpl> impl;
   public:
       ResultImpl* operator->() const { return impl.get(); }
   };
   ```
   where ResultImpl has GetFieldCount/Fetch/NextRow. Then `result->GetFieldCount()` delegates to the inner object — no circularity. The const-ness: `operator->() const` returning `ResultImpl*` (non-const) — production calls `result->NextRow()` on const QueryResult& — NextRow could be const or non-const since the pointer is non-const and pointee is mutable. That's exactly how a smart pointer works: const smart pointer still allows mutating the pointee.

   So `GetFieldCount`/`Fetch`/`NextRow` live on ResultImpl and can be non-const (or const as needed). Let me restructure QueryResult:

   ```cpp
   class QueryResult
   {
   public:
       using Row = std::vector<Field>;
       class State
       {
       public:
           explicit State(std::vector<Row> rows) : rows(std::move(rows)) {}
           std::uint32_t GetFieldCount() const { ... }
           Field* Fetch() { ... }
           bool NextRow() { ... }
           std::vector<Row> rows;
           std::size_t index = 0;
       };
       QueryResult() = default;
       explicit QueryResult(std::vector<Row> rows) : mState(new State(std::move(rows))) {}
       explicit operator bool() const { return static_cast<bool>(mState); }
       State* operator->() const { return mState.get(); }
   private:
       std::shared_ptr<State> mState;
   };
   ```
   Then `result->GetFieldCount()` → mState.get()->GetFieldCount(). No circular delegation because operator-> returns State* (a pointer to a *different* type). GCC's circular delegation check triggers when `operator->` returns `*this` or an object of the same effective type. Returning a pointer to a different struct is fine.

   Also `LoadGameRealmName` uses `result->Fetch()[0].Get<std::string>()` → State::Fetch() returns Field* → [0] on Field* → Field& → .Get<std::string>(). That requires Field::Get<std::string> and `Field*` argument... `Fetch()[0]` on a pointer — `result->Fetch()` returns `Field*`; `[0]` indexes → Field&. `Get<std::string>()` — Field is a template; `Field&.Get<std::string>()` fine.

   But the errors at line 640:
   ```
   error: expected primary-expression before '>' token
   auto name = result->Fetch()[0].Get<std::string>();
   ```
   This was a cascade from the circular-delegation error (the `operator->` chain failed to build). With the fix it should resolve.

2. **`[Client]` golden** — no error there.

3. **`missing initializer for member ActiveContentBuild::ClientRequirements` with -Werror=missing-field-initializers**. The push_back `{value(0), value(1), value(2), value(3)}` now warns because the struct has a 5th member that isn't value-initialized. -Wmissing-field-initializers warns when initializing an aggregate and a member is left out without it having a default member initializer. `std::vector<std::string> ClientRequirements;` has no default member initializer (`{}` or `= {}`). The aggregate initializer list with 4 elements leaves the 5th to value-initialization → warning. Options:
   - Add a default member initializer `std::vector<std::string> ClientRequirements{};` to silence? Actually -Wmissing-field-initializers triggers only for members without default member initializers AND when using brace-init... adding `{}` as a default member initializer would suppress. Actually, the rule: -Wmissing-field-initializers warns when a member is not initialized by an initializer list, unless that member has a default member initializer (in which case it's not a warning). So adding `= {}` (default member initializer) to `ActiveContentBuild::ClientRequirements` silences it. That's clean and matches the codebase (Patch::InstallMode has a default member initializer).
   
   So in production: `std::vector<std::string> ClientRequirements{};` in ActiveContentBuild. Hmm, that's a real change but harmless. Alternatively, explicitly push `{value(0), value(1), value(2), value(3), {}}`. That also works without touching the struct. Which is less surprising? Adding the 5th initiator `{}` at the push site is explicit and documents the new member. Let me do `{value(0), value(1), value(2), value(3), {}}`.

   Actually wait — would `{value(0), value(1), value(2), value(3), {}}` compile? Yes, aggregates allow `{}`.

   But actually, is this warning present under the module's real build (AzerothCore with -Werror)? Yes potentially — AzerothCore builds with strict warnings. If a real build uses -Wmissing-field-initializers, my change would break it. So better to fix at the production push site. I'll use the explicit `{}` initiator.

4. **REQUIRE macro 4 arguments** — line 275: `REQUIRE(settings.ActiveBuilds[0].ClientRequirements == std::vector<std::string>{"apple", ...});` — the braced-init-list inside the macro argument contains commas, which the preprocessor splits as macro arguments. `REQUIRE(...)` with a `{...}` containing commas → 2+ args. Need to wrap in parentheses: `REQUIRE(settings.ActiveBuilds[0].ClientRequirements == (std::vector<std::string>{"apple", "banana", "protected-framexml", "zesty"}));` — parenthesizing the whole RHS makes the commas inside parentheses, so the preprocessor treats them as inside parens → single arg. Let me update all REQUIRE calls that contain braced-init lists with commas:
   - line ~274: `REQUIRE(settings.ActiveBuilds[0].ClientRequirements == std::vector<std::string>{...})`
   - Others: "Copy" `REQUIRE(settings.ActiveBuilds[0].ClientRequirements == (std::vector...))`. Let me search for those.
   - `REQUIRE(settings.ClientRequirements == std::vector<std::string>{"protected-framexml"});` — braced list with single element, no comma → single arg fine. But the == expression contains no top-level comma. OK.
   - In test 5: `REQUIRE(settings.ActiveBuilds[0].ClientRequirements == std::vector<std::string>{"protected-framexml"});` single element — fine.
   - test 6/7: multi-element → wrap parens.

Let me fix all these. Also check `REQUIRE(settings.ClientRequirements == ...)` elsewhere.

Let me patch the test file and QueryResult stub.

Also check the client [Client] structure test uses vector comparison? No.

Also `REQUIRE(settings.Patches[0].InstallMode == PatchInstallMode::WowPatch);` fine.

Let me also double-check `REQUIRE(contents.str() == BuildConfiguration(knownGood));` fine.

Let me apply fixes.

Several issues to fix: the QueryResult `operator->` design trips GCC's circular-delegation check, a missing-field-initializer at the build push site, and preprocessor comma-splitting in macros. Let me fix the stub first:



**Tool: write**

**Input:**
```json
{
  "content": "#ifndef REALM_CONFIG_TEST_QUERYRESULT_H\n#define REALM_CONFIG_TEST_QUERYRESULT_H\n\n#include \"Common.h\"\n#include \"Field.h\"\n\n#include <cstddef>\n#include <memory>\n#include <utility>\n#include <vector>\n\n// Test-only double for AzerothCore's QueryResult. Holds in-memory Field rows.\n// The cursor starts on the first row; Fetch() returns the current row, and\n// NextRow() advances. A default-constructed value is null/failed. operator->\n// returns the inner state pointer (mirroring a smart pointer) so the production\n// result->Foo() call sites compile unchanged.\nclass QueryResult\n{\npublic:\n    using Row = std::vector<Field>;\n\n    class State\n    {\n    public:\n        explicit State(std::vector<Row> source) : rows(std::move(source)) {}\n\n        std::uint32_t GetFieldCount() const\n        {\n            if (index >= rows.size()) return 0;\n            return static_cast<std::uint32_t>(rows[index].size());\n        }\n\n        Field* Fetch()\n        {\n            if (index >= rows.size()) return nullptr;\n            return rows[index].data();\n        }\n\n        bool NextRow()\n        {\n            if (index + 1 >= rows.size()) return false;\n            ++index;\n            return true;\n        }\n\n        std::vector<Row> rows;\n        std::size_t index = 0;\n    };\n\n    QueryResult() = default;\n    explicit QueryResult(std::vector<Row> rows)\n        : state(new State(std::move(rows))) {}\n\n    explicit operator bool() const { return static_cast<bool>(state); }\n\n    State* operator->() const { return state.get(); }\n\nprivate:\n    std::shared_ptr<State> state;\n};\n\n#endif",
  "filePath": "/home/smithkt/git/mod-realm-config/tests/api/QueryResult.h"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 8.2s)

_Thinking:_

Now fix the production push_back for ActiveBuilds to include the 5th initiator. And also consider the same for the test where I construct ActiveContentBuild in the test directly:
```cpp
settings.ActiveBuilds.push_back({"30", "Test Realm", "...", SHA64, {"protected-framexml"}});
```
This is in the test — constructing ActiveContentBuild aggregate with 5 elements including the vector — fine (aggregate init with braced vector). Need to ensure braces: `{"...", "...", "...", SHA64, {"protected-framexml"}}`. This is ambiguous? The outer brace list for ActiveContentBuild, inner for vector. GCC might complain about narrowing? No, it's fine. But in the test there's also `{"12", "Test Realm", "...", SHA64, {"protected-framexml"}}` in several tests. Should be fine.

Also note these aggregate init lists in tests contain commas but not inside a macro call directly — they're inside `push_back(...)` calls, so no macro issue.

Now fix production line 294 push_back.

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp",
  "newString": "else if (type == \"3\")\n            settings.ActiveBuilds.push_back({value(0), value(1), value(2), value(3), {}});",
  "oldString": "else if (type == \"3\")\n            settings.ActiveBuilds.push_back({value(0), value(1), value(2), value(3)});"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 3.3s)

_Thinking:_

Now fix the REQUIRE macro comma issues in tests. Line 275 area: the `REQUIRE(settings.ActiveBuilds[0].ClientRequirements == std::vector<std::string>{...})`. Let me grep for all REQUIRE calls containing a braced multi-element initializer.

**Tool: bash**

**Input:**
```json
{
  "command": "grep -n \"REQUIRE(.*std::vector<std::string>{\" /home/smithkt/git/mod-realm-config/tests/mod_realm_config_tests.cpp"
}
```

**Output:**
```
(no output)
```

---

## Assistant (Build · Big Pickle · 1.7s)

**Tool: bash**

**Input:**
```json
{
  "command": "grep -n \"REQUIRE(\" /home/smithkt/git/mod-realm-config/tests/mod_realm_config_tests.cpp | grep -v REQUIRE_THROW"
}
```

**Output:**
```
22:#define REQUIRE(cond)                                                        \
192:        REQUIRE(output == expected);
196:        REQUIRE(client.size() == 6);
197:        REQUIRE(client[0] == "Version=3.3.5a");
198:        REQUIRE(client[1] == "Build=12340");
199:        REQUIRE(client[2] == "Executable=Wow.exe");
200:        REQUIRE(client[3] == "ExecutableSHA256=");
201:        REQUIRE(client[4] == "RuntimeMode=Legacy");
202:        REQUIRE(client[5] == "Requirements=");
210:        REQUIRE(Contains(outLegacy, "RuntimeMode=Legacy\nRequirements="));
215:        REQUIRE(Contains(outIsolated, "RuntimeMode=Isolated\nRequirements="));
220:        REQUIRE(JoinClientRequirements({}) == "");
221:        REQUIRE(JoinClientRequirements({"protected-framexml"}) == "protected-framexml");
222:        REQUIRE(JoinClientRequirements({"b", "a", "b", "c"}) == "a,b,c");
223:        REQUIRE(JoinClientRequirements({"zesty", "apple", "banana"}) == "apple,banana,zesty");
232:        REQUIRE(Contains(output, "Requirements=\n"));
233:        REQUIRE(!Contains(output, "Patch.realm-content"));
234:        REQUIRE(settings.ClientRequirements.empty());
243:        REQUIRE(Contains(output, "Requirements=\n"));
244:        REQUIRE(Contains(output, "[Patch.realm-content]"));
245:        REQUIRE(settings.ClientRequirements.empty());
254:        REQUIRE(settings.ActiveBuilds.size() == 1);
255:        REQUIRE(settings.ActiveBuilds[0].ClientRequirements ==
258:        REQUIRE(Contains(output, "Requirements=protected-framexml\n"));
259:        REQUIRE(settings.ClientRequirements ==
274:        REQUIRE(settings.ActiveBuilds[0].ClientRequirements ==
277:        REQUIRE(Contains(output,
279:        REQUIRE(!Contains(output, "Requirements=,"));
286:        REQUIRE(Contains(sqlBoth, "content_manager_build_client_requirement"));
287:        REQUIRE(Contains(sqlBoth, "WHERE build_number IN (SELECT build_number "
290:        REQUIRE(!Contains(sqlContentOnly, "content_manager_build_client_requirement"));
291:        REQUIRE(Contains(sqlContentOnly, "content_manager_build"));
293:        REQUIRE(!Contains(sqlNone, "content_manager_build"));
299:        REQUIRE(settings.Addons.size() == 1);
300:        REQUIRE(settings.Patches.size() == 1);
301:        REQUIRE(settings.ActiveBuilds[0].ClientRequirements ==
313:        REQUIRE(Contains(status, "not advertised"));
314:        REQUIRE(settings.Patches.empty());
315:        REQUIRE(settings.ClientRequirements.empty());
317:        REQUIRE(!Contains(output, "Patch.realm-content"));
318:        REQUIRE(Contains(output, "Requirements=\n"));
328:        REQUIRE(Contains(status, "available; ACTIVE realm content build 30"));
329:        REQUIRE(settings.Patches.size() == 1);
330:        REQUIRE(settings.Patches[0].Key == "realm-content");
331:        REQUIRE(settings.Patches[0].InstallMode == PatchInstallMode::WowPatch);
332:        REQUIRE(settings.ClientRequirements ==
342:        REQUIRE(Contains(status, "content_base_url is unset"));
343:        REQUIRE(settings.Patches.empty());
344:        REQUIRE(settings.ClientRequirements.empty());
361:        REQUIRE(contract.content && contract.requirements);
366:        REQUIRE(legacy.content && !legacy.requirements);
371:        REQUIRE(!absent.content && !absent.requirements);
422:        REQUIRE(!published);
423:        REQUIRE(contents.str() == BuildConfiguration(knownGood));
440:        REQUIRE(SynthesizeRealmContent(settings, false, "Test Realm") == "unavailable");
442:        REQUIRE(true);
454:        REQUIRE(target == directory / "test.realm.conf");
455:        REQUIRE(fs::is_regular_file(target));
458:        REQUIRE(contents.str() == output);
469:        REQUIRE(BuildConfiguration(a) == BuildConfiguration(b));
470:        REQUIRE(BuildConfiguration(a) == BuildConfiguration(a));

```

---

## Assistant (Build · Big Pickle · 11.4s)

_Thinking:_

The multi-element braced REQUIREs:
- Line 221: `REQUIRE(JoinClientRequirements({"protected-framexml"}) == "protected-framexml");` — single element, no comma inside braces → fine.
- Line 222: `REQUIRE(JoinClientRequirements({"b", "a", "b", "c"}) == "a,b,c");` — braced list WITHOUT parens within braces: `{"b", "a", "b", "c"}` — the commas... are they inside parentheses? The macro argument is the entire `JoinClientRequirements({"b", "a", "b", "c"}) == "a,b,c"`. The preprocessor counts parentheses; the `{...}` commas are NOT inside parens, so the token stream `JoinClientRequirements ( { "b" , "a" , ... ` — the first comma is at "b", "a" — that comma is inside the function-call parens `JoinClientRequirements(...)`. YES — the whole `({"b", "a", "b", "c"})` IS within parentheses of the function call. Preprocessor: macro arguments are split on commas that are at paren-depth 0. The parens of `JoinClientRequirements(` raise depth; commas inside stay at depth >0 → NOT split. So line 222 is FINE.

- Line 223: same pattern → fine.
- Line 221: `JoinClientRequirements({"protected-framexml"})` fine.

So the problematic one is line 274:
```
REQUIRE(settings.ActiveBuilds[0].ClientRequirements ==
            std::vector<std::string>{"apple", "banana", "protected-framexml", "zesty"});
```
Here the braces are NOT inside any parens; the commas are at paren-depth 0 → split into multiple args. Same for line 255, 259, 275, 332: these are the comparisons with `std::vector<std::string>{...}` multi-element. Let me check each:

- Line 254-256: `REQUIRE(settings.ActiveBuilds[0].ClientRequirements ==\n std::vector<std::string>{"protected-framexml"});` — single element, no commas in braces → ONE arg (even though braces not in parens, there's just no comma). WAIT: comma inside `==`? no. So single arg fine.
- Line 258-259: same single element — fine.
- Line 274-276: multi-element → BROKEN.
- Line 301: multi-element `{"protected-framexml"}` single element → fine.
- Line 332: multi-element? Let me look: 
```
REQUIRE(settings.ClientRequirements ==
            std::vector<std::string>{"protected-framexml"});
```
single element → fine.

So only line 274 needs the paren wrap. Let me fix by adding enclosing parens around the RHS: `== (std::vector<std::string>{...})`.

Let me apply.





**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/tests/mod_realm_config_tests.cpp",
  "newString": "REQUIRE(settings.ActiveBuilds[0].ClientRequirements ==\n            (std::vector<std::string>{\"apple\", \"banana\", \"protected-framexml\", \"zesty\"}));",
  "oldString": "REQUIRE(settings.ActiveBuilds[0].ClientRequirements ==\n            std::vector<std::string>{\"apple\", \"banana\", \"protected-framexml\", \"zesty\"});"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 4.9s)

**Tool: bash**

**Input:**
```json
{
  "command": "bash /home/smithkt/git/mod-realm-config/tests/run.sh"
}
```

**Output:**
```
/usr/bin/x86_64-linux-gnu-ld.bfd: /tmp/ccOc3sf2.o: warning: relocation against `realm' in read-only section `.text'
/usr/bin/x86_64-linux-gnu-ld.bfd: /tmp/ccOc3sf2.o: in function `(anonymous namespace)::LoadGameRealmName(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&)':
mod_realm_config_tests.cpp:(.text+0x6851): undefined reference to `realm'
/usr/bin/x86_64-linux-gnu-ld.bfd: mod_realm_config_tests.cpp:(.text+0x689d): undefined reference to `LoginDatabase'
/usr/bin/x86_64-linux-gnu-ld.bfd: /tmp/ccOc3sf2.o: in function `(anonymous namespace)::RealmConfigWorldScript::OnUpdate(unsigned int)':
mod_realm_config_tests.cpp:(.text+0x9177): undefined reference to `WorldDatabase'
/usr/bin/x86_64-linux-gnu-ld.bfd: mod_realm_config_tests.cpp:(.text+0x92ba): undefined reference to `WorldDatabase'
/usr/bin/x86_64-linux-gnu-ld.bfd: /tmp/ccOc3sf2.o: in function `(anonymous namespace)::RealmConfigWorldScript::AcceptResult(QueryResult const&, (anonymous namespace)::ContentContract const&)':
mod_realm_config_tests.cpp:(.text+0x972e): undefined reference to `realm'
/usr/bin/x86_64-linux-gnu-ld.bfd: /tmp/ccOc3sf2.o: in function `(anonymous namespace)::RealmConfigWorldScript::ReloadSettings()':
mod_realm_config_tests.cpp:(.text+0x9bcb): undefined reference to `sConfigMgr'
/usr/bin/x86_64-linux-gnu-ld.bfd: mod_realm_config_tests.cpp:(.text+0x9c47): undefined reference to `sConfigMgr'
/usr/bin/x86_64-linux-gnu-ld.bfd: mod_realm_config_tests.cpp:(.text+0x9d3f): undefined reference to `sConfigMgr'
/usr/bin/x86_64-linux-gnu-ld.bfd: mod_realm_config_tests.cpp:(.text+0x9e10): undefined reference to `sConfigMgr'
/usr/bin/x86_64-linux-gnu-ld.bfd: mod_realm_config_tests.cpp:(.text+0x9fd4): undefined reference to `WorldDatabase'
/usr/bin/x86_64-linux-gnu-ld.bfd: mod_realm_config_tests.cpp:(.text+0xa048): undefined reference to `WorldDatabase'
/usr/bin/x86_64-linux-gnu-ld.bfd: /tmp/ccOc3sf2.o: in function `main':
mod_realm_config_tests.cpp:(.text+0xc96a): undefined reference to `sConfigMgr'
/usr/bin/x86_64-linux-gnu-ld.bfd: mod_realm_config_tests.cpp:(.text+0xc98a): undefined reference to `realm'
/usr/bin/x86_64-linux-gnu-ld.bfd: mod_realm_config_tests.cpp:(.text+0xc99b): undefined reference to `realm'
/usr/bin/x86_64-linux-gnu-ld.bfd: mod_realm_config_tests.cpp:(.text+0x14861): undefined reference to `sConfigMgr'
/usr/bin/x86_64-linux-gnu-ld.bfd: warning: creating DT_TEXTREL in a PIE
collect2: error: ld returned 1 exit status

```

---

## Assistant (Build · Big Pickle · 6.8s)

_Thinking:_

The globals are declared inside the anonymous namespace of the production cpp? Wait no — the production file's includes are within the anonymous namespace? Actually the production file structure:

```cpp
#include "Config.h"
...
#include <...>

namespace
{
namespace fs = std::filesystem;
...
}
```

The includes are OUTSIDE the anonymous namespace at the top of the file. But wait — `sConfigMgr`, `realm`, `WorldDatabase`, `LoginDatabase` are declared in the stub headers which are included BEFORE the anonymous namespace opens. So they're in global namespace. My test main defines them... but I put them inside `int main()`? No — I wrote:

```cpp
int main()
{
    sConfigMgr = new ConfigMgr();
    realm.Name = "Test Realm";
    realm.Id.Realm = 1;
    ...
}
```

That ASSIGNS to them but doesn't DEFINE them. I need to define the extern globals at namespace scope in the test TU:

```cpp
ConfigMgr* sConfigMgr = nullptr;
DatabaseWorkerPool WorldDatabase;
DatabaseWorkerPool LoginDatabase;
Realm realm;
```

Let me add these before main().

The extern globals need namespace-scope definitions in the test TU. Let me add them:

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/tests/mod_realm_config_tests.cpp",
  "newString": "std::string RunPublication(Settings settings, bool content, std::string const& realmName)\n{\n    SynthesizeRealmContent(settings, content, realmName);\n    ValidateSettings(settings);\n    return BuildConfiguration(settings);\n}\n}\n// namespace\n\n// Definitions for the stub globals declared extern by tests/api headers.\nConfigMgr* sConfigMgr = nullptr;\nDatabaseWorkerPool WorldDatabase;\nDatabaseWorkerPool LoginDatabase;\nRealm realm;\n\nint main()",
  "oldString": "std::string RunPublication(Settings settings, bool content, std::string const& realmName)\n{\n    SynthesizeRealmContent(settings, content, realmName);\n    ValidateSettings(settings);\n    return BuildConfiguration(settings);\n}\n}\n// namespace\n\nint main()"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 3.3s)

**Tool: bash**

**Input:**
```json
{
  "command": "bash /home/smithkt/git/mod-realm-config/tests/run.sh"
}
```

**Output:**
```
FAIL(/home/smithkt/git/mod-realm-config/tests/mod_realm_config_tests.cpp:265): settings.ClientRequirements == std::vector<std::string>{"protected-framexml"}
1 of 71 checks FAILED

```

---

## Assistant (Build · Big Pickle · 11.2s)

_Thinking:_

Almost. Only one failure. Let me look at line 265's context — the test block:

```
        REQUIRE(settings.ActiveBuilds[0].ClientRequirements ==
            std::vector<std::string>{"protected-framexml"});
        auto output = RunPublication(settings, true, "Test Realm");
        REQUIRE(Contains(output, "Requirements=protected-framexml\n"));
        REQUIRE(settings.ClientRequirements ==
            std::vector<std::string>{"protected-framexml"});
```

Line 265 is `REQUIRE(settings.ClientRequirements == std::vector<std::string>{"protected-framexml"});` — after RunPublication. But note: RunPublication takes `Settings settings` by VALUE (a copy). Inside RunPublication, SynthesizeRealmContent modifies the COPY. The original `settings` in the test scope is NOT modified! So settings.ClientRequirements stays empty in the test scope. That's the bug in my test, not the product.

In test case 5, the first REQUIRE on line 255 checks `settings.ActiveBuilds[0].ClientRequirements` (populated by LoadMetadata) — passed. Then RunPublication(settings, ...) operates on a copy; the output correctly includes Requirements=protected-framexml (passed line 258). But line 265 checks the ORIGINAL settings, which wasn't mutated. 

Fix: change RunPublication to take `Settings&` so mutation is visible, OR check against a copy. The other tests using RunPublication then checking `settings.ClientRequirements` similarly (test case 9) rely on mutation too? Case 9 doesn't call RunPublication. Tests 4,5 rely on the returned output string, and separately check `settings.ClientRequirements.empty()` where settings wasn't passed through RunPublication that mutates... wait test 4: `RunPublication(settings, true, ...)` by value, then `REQUIRE(settings.ClientRequirements.empty());` — that would be on the original (still empty) — passes but for the wrong reason (should be empty anyway since no requirements).

Simplest: make RunPublication take `Settings& settings` so SynthesizeRealmContent mutates the caller's object (mirroring AcceptResult which mutates its local `settings`). Since AcceptResult operates on its own local Settings, mutating in place is accurate. Let me change RunPublication to take `Settings&`.

Then test 5 line 259-265 will pass: after RunPublication, settings.ClientRequirements is attached.

But wait — in test case 4: `RunPublication(settings, true, ...)` — after it, settings.ActiveBuilds would still have its build with empty requirements; settings.ClientRequirements empty. But also, SynthesizeRealmContent pushes a realm-content patch into settings.Patches — original mutated now. That's fine; test 4 checks Requirements= and [Patch.realm-content] in output. Fine.

Test 3: `RunPublication(settings, false, ...)` — content=false returns "unavailable"; no mutation. Fine.

Test 6/7: `RunPublication(settings, true, ...)`. Fine.

Foreign realm test (9) doesn't call RunPublication. Fine.

Let me update RunPublication signature.

The `RunPublication` helper takes Settings by value, so mutations to `ClientRequirements` aren't visible to the caller. Let me fix it:



**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/tests/mod_realm_config_tests.cpp",
  "newString": "std::string RunPublication(Settings& settings, bool content, std::string const& realmName)\n{\n    SynthesizeRealmContent(settings, content, realmName);\n    ValidateSettings(settings);\n    return BuildConfiguration(settings);\n}",
  "oldString": "std::string RunPublication(Settings settings, bool content, std::string const& realmName)\n{\n    SynthesizeRealmContent(settings, content, realmName);\n    ValidateSettings(settings);\n    return BuildConfiguration(settings);\n}"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 6.2s)

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/tests/mod_realm_config_tests.cpp",
  "newString": "auto bad = MakeSettings();\n        bad.OutputDirectory = directory.string();\n        bad.ContentBaseURL = \"https://example.com/download/\";\n        bad.ActiveBuilds.push_back({\"12\", \"Test Realm\",\n            \"Test-Realm-Content-000012.mpq\", SHA64,\n            {\"protected-framexml,extra\"}}); // invalid published token\n        bool published = false;\n        try\n        {\n            auto output = RunPublication(bad, true, \"Test Realm\");\n            PublishConfiguration(bad, output);\n            published = true;\n        }\n        catch (std::exception const&) { }",
  "oldString": "auto bad = MakeSettings();\n        bad.OutputDirectory = directory.string();\n        bad.ContentBaseURL = \"https://example.com/download/\";\n        bad.ActiveBuilds.push_back({\"12\", \"Test Realm\",\n            \"Test-Realm-Content-000012.mpq\", SHA64,\n            {\"protected-framexml,extra\"}}); // invalid published token\n        bool published = false;\n        try\n        {\n            auto output = RunPublication(bad, true, \"Test Realm\");\n            PublishConfiguration(bad, output);\n            published = true;\n        }\n        catch (std::exception const&) { }"
}
```

**Error:**
```
No changes to apply: oldString and newString are identical.
```

---

## Assistant (Build · Big Pickle · 8.9s)

**Tool: bash**

**Input:**
```json
{
  "command": "bash /home/smithkt/git/mod-realm-config/tests/run.sh"
}
```

**Output:**
```
All 71 checks passed

```

---

## Assistant (Build · Big Pickle · 20.4s)

_Thinking:_

All 71 checks pass. Let me also verify the "-Werror" is on and clean. The test passed with -Wall -Wextra -Werror, so the production code plus test compile cleanly.

Wait — but there's a concern: my run.sh uses -Werror. The production file compiled cleanly, great. Also the test's `REQUIRE(settings.Patches[0].InstallMode == PatchInstallMode::WowPatch);` etc all passed.

Now let me double check test coverage corresponds to the 14 required items:
1. Existing generation valid → golden test ✓
2. Existing [Client] keys unchanged → SectionLines check ✓
3. No content integration → test ✓
4. Legacy zero requirement rows → test ✓
5. protected-framexml → test ✓
6. Multiple → deterministic CSV ✓
7. No duplicates → dedup ✓
8. Requirements from ACTIVE build, not package state → MetadataSQL filter + attach test ✓
9. Foreign realm → test ✓
10. Lookup failure → throw tests + fail-safe publication preserve ✓
11. RuntimeMode=Legacy ✓
12. RuntimeMode=Isolated ✓
13. Addons[]/Patches[] unchanged except Requirements → golden ✓
14. Existing regression tests → none existed; goldens preserve behavior ✓

Now there is one thing I want to reconsider regarding test #10: "Requirement lookup failure: must NOT silently become Requirements=". In the product, "lookup failure" = requirement query fails. In my design, a requirement query failure makes the whole Metadata SQL union fail → null result → LoadMetadata throws → fails safe. I test LoadMetadata(null) throws. Also the contract detection with incompatible requirement table throws. Also orphan requirement rows throw. Good.

But wait — there's a subtle product-level issue I should double check: the `MetadataSQL` union. In MySQL, if the `content_manager_build_client_requirement` table is DROPPED between detection and the snapshot query, the union would fail (table not found) → null result → fail-safe. Good.

But what about the semantic distinction between "table has zero rows for the ACTIVE build" vs "table exists but requirement retrieval fails"? In the single-union design, the requirement SELECT is part of the union; the table must exist and be queryable or the whole union fails. So "retrieval fails" ⇒ union fails ⇒ fail-safe. And "empty rows for ACTIVE build" ⇒ no type '4' rows ⇒ Requirements=. Correct distinction. 

Now — a concern about the importance of the contract-gated detection for backward compatibility: if content-manager is pre-Schema-3 (no requirement table), `contract.requirements=false` → the union omits the requirement SELECT → the union succeeds → legacy behavior (Requirements= empty for legacy ACTIVE builds). Good. But what if content-manager IS Schema-3 and requirement table EXISTS: contract.requirements=true → the SELECT runs. If a build was created before the migration and never got requirement rows, no type '4' rows → Requirements= . Matches requirement "Existing pre-Schema-3 builds naturally have no requirement rows and therefore require nothing."

Important nuance: the task says "mod-content-manager now provides an API capable of answering: What client requirements were recorded for build N?" and I chose the direct-SQL integration boundary because mod-realm-config already integrates via direct DB queries (no compile-time C++ dependency exists and introducing one would violate the optional-dependency constraint). The task explicitly allows this: "Do NOT query the new content_manager_build_client_requirement table directly from mod-realm-config unless the existing architecture clearly establishes direct database queries as the supported integration mechanism." The existing architecture does (it queries content_manager_build directly). I've mirrored the exact semantics of the API:
- existing build + requirements → success+requirements ✓
- existing build + no rows → empty ✓
- nonexistent build → failure (orphan/gated) ✓
- query failure → failure ✓

However, one more consideration: The task says the API semantics treat a nonexistent build as failure. In our integration, requirements only come from ACTIVE builds that exist. A "build number referenced by a requirement row but not ACTIVE" can't happen because the WHERE filters to ACTIVE. But what about a requirement row referencing a build number that is ACTIVE and exists — fine. There's no "requirements for nonexistent build" input path because we anchor on the ACTIVE build. Good.

Now let me also reconsider determinism around BuildConfiguration: `JoinClientRequirements` sorts & dedups. Output `Requirements=a,b,c`. Content manager returns sorted order; our normalization keeps semantic order but sorts byte-wise — matches content manager's ordering (both byte-wise). No trailing comma. No whitespace. ✓

Now, one design question worth re-examining: I emit `Requirements=` in the [Client] section ALWAYS (even when no content manager). Is that acceptable for "Portalkeeper v0.4.0 must remain completely untouched and unaware of this new field... should simply ignore the unknown key"? The task says the generated config must remain parseable. Adding an always-present empty key is additive and deterministic. The task explicitly prefers `Requirements=` over omitting when consistent with deterministic conventions — the generator emits all keys unconditionally, so always emitting is the least surprising choice. ✓

Now let me double-check the earlier concern: does the portalkeeper ignore unknown keys? The task says "according to its existing configuration behavior" — we must NOT modify portalkeeper. I haven't. ✓

Now let me review the whole production diff to make sure nothing is off, then update docs.

Let me also reconsider: in SynthesizeRealmContent, the order of ownership check vs content_base_url check. Currently content_base_url empty check happens BEFORE ownership check. So if content_base_url is unset, requirements won't be attached even for owned builds. As decided, this matches "build being advertised" semantics. Document this.

Also check: SynthesizeRealmContent attaches requirements only in the owned+advertised path. Foreign builds handled. Now — there's an edge: what if contentAvailable=true but requirements table absent (legacy content-manager), owned ACTIVE build with no requirements → Requirements=. Correct.

What if contentAvailable=true, requirements present but the ACTIVE build belongs to foreign realm → no requirements advertised (test 9). ✓

Now, let me also verify that the LoadMetadata "orphan" throw doesn't break when there are legitimately zero type '4' rows but the map is empty → loop doesn't run. ✓

Now let me view the complete diff.

All 71 checks pass. Let me review the full production diff for correctness:

**Tool: bash**

**Input:**
```json
{
  "command": "git -C /home/smithkt/git/mod-realm-config diff --stat; echo \"====\"; git -C /home/smithkt/git/mod-realm-config status"
}
```

**Output:**
```
 src/mod_realm_config.cpp | 150 +++++++++++++++++++++++++++++++++++++++++------
 1 file changed, 131 insertions(+), 19 deletions(-)
====
On branch main
Your branch is up to date with 'origin/main'.

Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git restore <file>..." to discard changes in working directory)
	modified:   src/mod_realm_config.cpp

Untracked files:
  (use "git add <file>..." to include in what will be committed)
	tests/

no changes added to commit (use "git add" and/or "git commit -a")

```

---

## Assistant (Build · Big Pickle · 2.4s)

**Tool: bash**

**Input:**
```json
{
  "command": "git -C /home/smithkt/git/mod-realm-config diff src/mod_realm_config.cpp"
}
```

**Output:**
```
diff --git a/src/mod_realm_config.cpp b/src/mod_realm_config.cpp
index c1fa3e1..63a4a98 100644
--- a/src/mod_realm_config.cpp
+++ b/src/mod_realm_config.cpp
@@ -14,6 +14,7 @@
 #include <fstream>
 #include <optional>
 #include <set>
+#include <map>
 #include <algorithm>
 #include <stdexcept>
 #include <string>
@@ -40,6 +41,9 @@ struct Patch
 struct ActiveContentBuild
 {
     std::string Number, RealmName, FileName, SHA256;
+    // Immutable client requirements recorded against this exact build by Content
+    // Manager (Schema 3). Sorted, deduplicated, copied from the ACTIVE row set.
+    std::vector<std::string> ClientRequirements;
 };
 
 struct Settings
@@ -61,6 +65,9 @@ struct Settings
     std::string ManifestURL, NewsURL, StatusURL, CalendarURL, ArmoryURL;
     std::string ContentBaseURL;
     std::vector<ActiveContentBuild> ActiveBuilds;
+    // Published semantic client requirements of the ACTIVE build being advertised.
+    // Empty when no owned ACTIVE build is advertised.
+    std::vector<std::string> ClientRequirements;
     std::vector<Addon> Addons;
     std::vector<Patch> Patches;
 };
@@ -148,6 +155,16 @@ FROM mod_realm_config_patch
 WHERE enabled = 1
 )SQL";
 
+// Optional Content Manager integration state discovered per refresh. `content`
+// tracks the build table contract; `requirements` additionally confirms the
+// Schema 3 client requirement table. Absence of the requirement table is a
+// pre-Schema-3 install whose existing builds naturally have no requirement rows.
+struct ContentContract
+{
+    bool content = false;
+    bool requirements = false;
+};
+
 // COUNT returns a row even when Content Manager is absent. A query failure must
 // not be mistaken for absence. Checking columns also detects incompatible installs.
 constexpr char ContentContractQuery[] = R"SQL(
@@ -156,26 +173,37 @@ SELECT
      WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build'),
     (SELECT COUNT(*) FROM information_schema.COLUMNS
      WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build'
-       AND COLUMN_NAME IN ('build_number', 'realm_name', 'filename', 'sha256', 'state'))
+       AND COLUMN_NAME IN ('build_number', 'realm_name', 'filename', 'sha256', 'state')),
+    (SELECT COUNT(*) FROM information_schema.TABLES
+     WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build_client_requirement'),
+    (SELECT COUNT(*) FROM information_schema.COLUMNS
+     WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build_client_requirement'
+       AND COLUMN_NAME IN ('build_number', 'requirement'))
 )SQL";
 
-bool ReadContentContract(QueryResult const& result)
+ContentContract ReadContentContract(QueryResult const& result)
 {
-    if (!result || result->GetFieldCount() != 2)
+    if (!result || result->GetFieldCount() != 4)
         throw std::runtime_error("Content Manager contract detection failed; check database permissions/logs");
     Field* fields = result->Fetch();
-    if (fields[0].IsNull() || fields[1].IsNull())
+    if (fields[0].IsNull() || fields[1].IsNull() || fields[2].IsNull() || fields[3].IsNull())
         throw std::runtime_error("unexpected Content Manager contract detection result");
-    if (fields[0].Get<uint64>() == 0) return false;
+    ContentContract contract;
+    if (fields[0].Get<uint64>() == 0) return contract;
     if (fields[0].Get<uint64>() != 1 || fields[1].Get<uint64>() != 5)
         throw std::runtime_error("incompatible content_manager_build contract; expected build_number, realm_name, filename, sha256 and state");
-    return true;
+    contract.content = true;
+    if (fields[2].Get<uint64>() == 0) return contract;
+    if (fields[2].Get<uint64>() != 1 || fields[3].Get<uint64>() != 2)
+        throw std::runtime_error("incompatible content_manager_build_client_requirement contract; expected build_number and requirement");
+    contract.requirements = true;
+    return contract;
 }
 
-std::string MetadataSQL(bool withContent)
+std::string MetadataSQL(ContentContract const& contract)
 {
     std::string sql = MetadataQuery;
-    if (withContent)
+    if (contract.content)
     {
         sql += R"SQL(
 UNION ALL
@@ -192,6 +220,22 @@ SELECT CONVERT('3' USING utf8mb4) COLLATE utf8mb4_unicode_ci, 0,
         // anywhere in this world database are ambiguous and must be rejected.
         sql += " FROM content_manager_build WHERE BINARY state = 'ACTIVE'\n";
     }
+    if (contract.requirements)
+    {
+        sql += R"SQL(
+UNION ALL
+SELECT CONVERT('4' USING utf8mb4) COLLATE utf8mb4_unicode_ci, 0,
+       CONVERT(requirement USING utf8mb4) COLLATE utf8mb4_unicode_ci,
+       CONVERT(CAST(build_number AS CHAR) USING utf8mb4) COLLATE utf8mb4_unicode_ci
+)SQL";
+        for (unsigned i = 1; i < 19; ++i)
+            sql += ", CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci";
+        // Requirements are immutable build metadata owned by Content Manager.
+        // Only the exact ACTIVE builds' rows are consumed; package/config state
+        // is never inspected here.
+        sql += " FROM content_manager_build_client_requirement "
+            "WHERE build_number IN (SELECT build_number FROM content_manager_build WHERE BINARY state = 'ACTIVE')\n";
+    }
     sql += "ORDER BY record_type, sort_order, BINARY record_key";
     return sql;
 }
@@ -205,6 +249,9 @@ Settings LoadMetadata(QueryResult const& result, std::string const& directory)
     Settings settings;
     settings.OutputDirectory = directory;
     bool found = false;
+    // Byte-wise sorted, deduplicated requirement sets keyed by build number,
+    // mirroring Content Manager's own requirement ordering.
+    std::map<std::string, std::set<std::string>> buildRequirements;
     do
     {
         Field* fields = result->Fetch();
@@ -244,10 +291,26 @@ Settings LoadMetadata(QueryResult const& result, std::string const& directory)
             settings.Patches.push_back({fields[2].Get<std::string>(), value(0), value(1),
                 value(2), value(3), value(4), value(5), value(6)});
         else if (type == "3")
-            settings.ActiveBuilds.push_back({value(0), value(1), value(2), value(3)});
+            settings.ActiveBuilds.push_back({value(0), value(1), value(2), value(3), {}});
+        else if (type == "4")
+            buildRequirements[fields[3].Get<std::string>()].insert(fields[2].Get<std::string>());
         else throw std::runtime_error("unexpected Schema v1 SQL record type");
     } while (result->NextRow());
     if (!found) throw std::runtime_error("mod_realm_config singleton id=1 is missing");
+    for (auto const& entry : buildRequirements)
+    {
+        bool matched = false;
+        for (auto const& build : settings.ActiveBuilds)
+            if (build.Number == entry.first) { matched = true; break; }
+        if (!matched)
+            throw std::runtime_error("Content Manager client requirements reference a build that is not ACTIVE in this snapshot");
+    }
+    for (auto& build : settings.ActiveBuilds)
+    {
+        auto it = buildRequirements.find(build.Number);
+        if (it != buildRequirements.end())
+            build.ClientRequirements.assign(it->second.begin(), it->second.end());
+    }
     return settings;
 }
 
@@ -367,6 +430,34 @@ void ValidateRealmKey(std::string const& value)
         throw std::runtime_error("mod_realm_config.realm_key must contain 1..64 lowercase ASCII letters, digits, underscores or hyphens");
 }
 
+// Boundary check for a semantic client requirement name from Content Manager.
+// This validates the published token format (1..64 safe ASCII characters, no
+// commas or whitespace) so the CSV value round-trips deterministically. It does
+// not duplicate Content Manager's supported-capability registry.
+void ValidateClientRequirement(std::string const& value)
+{
+    if (value.empty() || value.size() > 64 ||
+        !std::all_of(value.begin(), value.end(), [](unsigned char c)
+        {
+            return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
+                (c >= '0' && c <= '9') || c == '.' || c == '_' || c == '-';
+        }))
+        throw std::runtime_error("Client.Requirements entries must contain 1..64 ASCII letters, digits, dots, underscores or hyphens");
+}
+
+void ValidateClientRequirements(Settings const& settings)
+{
+    for (std::size_t i = 0; i < settings.ClientRequirements.size(); ++i)
+    {
+        try { ValidateClientRequirement(settings.ClientRequirements[i]); }
+        catch (std::exception const& error)
+        {
+            throw std::runtime_error("Client.Requirements entry #" + std::to_string(i + 1) +
+                " in Content Manager build order: " + error.what());
+        }
+    }
+}
+
 void ValidateHash(std::string& value, char const* key)
 {
     if (value.empty()) return;
@@ -499,6 +590,9 @@ std::string SynthesizeRealmContent(Settings& settings, bool contentAvailable, st
     url += "/" + build.FileName;
     settings.Patches.push_back({"realm-content", build.RealmName + " Realm Content", "Required",
         "HTTP", std::move(url), "", "", build.SHA256, PatchInstallMode::WowPatch});
+    // The advertised build is this realm's. Publish its immutable recorded client
+    // requirements; validation runs in ValidateSettings before BuildConfiguration.
+    settings.ClientRequirements = build.ClientRequirements;
     // ValidateSettings subsequently validates the entire catalog using the same
     // key/name/source/hash rules, plus the install-mode-specific destination rules.
     return "available; ACTIVE realm content build " + number;
@@ -529,6 +623,7 @@ void ValidateSettings(Settings& settings)
     ValidateURL(settings.StatusURL, "Services.StatusURL");
     ValidateURL(settings.CalendarURL, "Services.CalendarURL");
     ValidateURL(settings.ArmoryURL, "Services.ArmoryURL");
+    ValidateClientRequirements(settings);
     ValidateCatalogs(settings);
 }
 
@@ -563,6 +658,22 @@ void AppendSection(std::string& output, char const* name, Fields const& fields)
         output += field.first + "=" + field.second + "\n";
 }
 
+// Deterministic comma-separated rendering. Defensive normalization protects the
+// published form regardless of upstream vector order; the semantic strings are
+// never altered. Empty input renders as the empty value.
+std::string JoinClientRequirements(std::vector<std::string> requirements)
+{
+    std::sort(requirements.begin(), requirements.end());
+    requirements.erase(std::unique(requirements.begin(), requirements.end()), requirements.end());
+    std::string joined;
+    for (std::size_t i = 0; i < requirements.size(); ++i)
+    {
+        if (i) joined += ',';
+        joined += requirements[i];
+    }
+    return joined;
+}
+
 std::string BuildConfiguration(Settings const& settings)
 {
     std::string output = "# Generated by mod-realm-config.\n"
@@ -579,7 +690,8 @@ std::string BuildConfiguration(Settings const& settings)
         {"AuthPort", settings.AuthPort}, {"WorldPort", settings.WorldPort}});
     AppendSection(output, "Client", {{"Version", settings.ClientVersion}, {"Build", settings.ClientBuild},
         {"Executable", settings.Executable}, {"ExecutableSHA256", settings.ExecutableSHA256},
-        {"RuntimeMode", settings.ClientRuntimeMode}});
+        {"RuntimeMode", settings.ClientRuntimeMode},
+        {"Requirements", JoinClientRequirements(settings.ClientRequirements)}});
     AppendSection(output, "Portalkeeper", {{"MinimumVersion", settings.MinimumVersion}});
     AppendSection(output, "Services", {{"ManifestURL", settings.ManifestURL}, {"NewsURL", settings.NewsURL},
         {"StatusURL", settings.StatusURL}, {"CalendarURL", settings.CalendarURL},
@@ -674,15 +786,15 @@ public:
         // Never replace _pending from inside its own callback.
         if (_snapshotContent.has_value() && !_pending)
         {
-            bool withContent = *_snapshotContent;
+            ContentContract contract = *_snapshotContent;
             _snapshotContent.reset();
             auto epoch = _epoch;
             try
             {
-                _pending.emplace(WorldDatabase.AsyncQuery(MetadataSQL(withContent)).WithCallback(
-                    [this, epoch, withContent](QueryResult result)
+                _pending.emplace(WorldDatabase.AsyncQuery(MetadataSQL(contract)).WithCallback(
+                    [this, epoch, contract](QueryResult result)
                     {
-                        if (_enabled && epoch == _epoch) AcceptResult(result, withContent);
+                        if (_enabled && epoch == _epoch) AcceptResult(result, contract);
                     }));
             }
             catch (std::exception const& error) { ReportFailure(error.what()); }
@@ -715,7 +827,7 @@ private:
         _lastError = message;
     }
 
-    void AcceptResult(QueryResult const& result, bool withContent)
+    void AcceptResult(QueryResult const& result, ContentContract const& contract)
     {
         try
         {
@@ -727,7 +839,7 @@ private:
             if (realmWarning != _lastRealmWarning && !realmWarning.empty())
                 LOG_WARN("module", "{}", realmWarning);
             _lastRealmWarning = std::move(realmWarning);
-            auto contentStatus = SynthesizeRealmContent(settings, withContent, realm.Name);
+            auto contentStatus = SynthesizeRealmContent(settings, contract.content, realm.Name);
             ValidateSettings(settings);
             auto output = BuildConfiguration(settings);
             auto target = fs::path(_directory) / RealmConfigFileName(settings);
@@ -778,8 +890,8 @@ private:
             _enabled = true;
             LOG_INFO("module", "mod-realm-config: enabled; checking world database every {} seconds", seconds);
             // Synchronous detection and snapshot at startup/reload; asynchronous during refresh.
-            bool withContent = ReadContentContract(WorldDatabase.Query(ContentContractQuery));
-            AcceptResult(WorldDatabase.Query(MetadataSQL(withContent)), withContent);
+            auto contract = ReadContentContract(WorldDatabase.Query(ContentContractQuery));
+            AcceptResult(WorldDatabase.Query(MetadataSQL(contract)), contract);
         }
         catch (std::exception const& error) { ReportFailure(error.what()); }
     }
@@ -795,7 +907,7 @@ private:
     fs::path _lastTarget;
     std::string _lastError;
     std::optional<QueryCallback> _pending;
-    std::optional<bool> _snapshotContent;
+    std::optional<ContentContract> _snapshotContent;
     std::string _lastContentStatus;
     std::string _lastRealmWarning;
 };

```

---

## Assistant (Build · Big Pickle · 3.7s)

_Thinking:_

The diff looks clean and focused. Now let me check something important: the diff title metadata for `SynthesizeRealmContent` shows the requirements attach is placed AFTER the patch push and just before the return. But I want to double-check the realm-ownership protection order: the ownership check happens before reaching this code (return early for foreign realm). Let me re-read the SynthesizeRealmContent function fully to confirm requirements are only attached on the owned path.

Let me verify the SynthesizeRealmContent gating once more in full:

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp",
  "limit": 75,
  "offset": 555
}
```

**Output:**
```
<path>/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp</path>
<type>file</type>
<content>
555: 
556: // Returns a status only for change-driven logging. No database writes or MPQ I/O.
557: std::string SynthesizeRealmContent(Settings& settings, bool contentAvailable, std::string const& canonicalRealmName)
558: {
559:     if (!contentAvailable) return "unavailable";
560:     if (settings.ActiveBuilds.size() > 1)
561:         throw std::runtime_error("multiple ACTIVE Content Manager builds; refusing ambiguous realm content metadata");
562:     if (settings.ActiveBuilds.empty()) return "available; no ACTIVE realm content build";
563:     if (settings.ContentBaseURL.empty()) return "available; content_base_url is unset";
564: 
565:     auto const& build = settings.ActiveBuilds.front();
566:     ValidateText(canonicalRealmName, "current AzerothCore realm name", true);
567:     ValidateText(build.RealmName, "Content Manager realm_name", true);
568:     // Content Manager records realm.Name verbatim. The public display name in
569:     // mod_realm_config may differ and is not the identity used for this comparison.
570:     if (build.RealmName != canonicalRealmName)
571:         return "available; ACTIVE build belongs to a different realm; not advertised";
572: 
573:     auto number = build.Number;
574:     ValidateNumber(number, "Content Manager build_number", UINT32_MAX);
575:     ValidateRelativePath(build.FileName, "Content Manager filename", true);
576:     // Content Manager generates ASCII artifact names. Reject URI delimiters and
577:     // escapes rather than altering its authoritative filename during URL joining.
578:     if (build.FileName.find_first_not_of(
579:         "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~") != std::string::npos)
580:         throw std::runtime_error("Content Manager filename is not a literal URL path component");
581:     ValidateURL(settings.ContentBaseURL, "content_base_url");
582:     if (settings.ContentBaseURL.find_first_of("?#") != std::string::npos)
583:         throw std::runtime_error("content_base_url must be a directory URL without query or fragment");
584:     for (auto const& patch : settings.Patches)
585:         if (FoldASCII(patch.Key) == "realm-content")
586:             throw std::runtime_error("realm-content is reserved for Content Manager integration; rename or disable the enabled manual patch");
587: 
588:     auto url = settings.ContentBaseURL;
589:     while (url.back() == '/') url.pop_back(); // ValidateURL already required a nonempty authority.
590:     url += "/" + build.FileName;
591:     settings.Patches.push_back({"realm-content", build.RealmName + " Realm Content", "Required",
592:         "HTTP", std::move(url), "", "", build.SHA256, PatchInstallMode::WowPatch});
593:     // The advertised build is this realm's. Publish its immutable recorded client
594:     // requirements; validation runs in ValidateSettings before BuildConfiguration.
595:     settings.ClientRequirements = build.ClientRequirements;
596:     // ValidateSettings subsequently validates the entire catalog using the same
597:     // key/name/source/hash rules, plus the install-mode-specific destination rules.
598:     return "available; ACTIVE realm content build " + number;
599: }
600: 
601: void ValidateSettings(Settings& settings)
602: {
603:     ValidateText(settings.OutputDirectory, "RealmConfig.OutputDirectory", true);
604:     ValidateRealmKey(settings.RealmKey);
605:     ValidateText(settings.Name, "mod_realm_config.name", true);
606:     ValidateText(settings.Address, "mod_realm_config.address", true);
607:     if (settings.Address.find_first_of(" /\\\"'\t=#;@") != std::string::npos ||
608:         (settings.Address.find(':') != std::string::npos &&
609:          settings.Address.find(':') == settings.Address.rfind(':')))
610:         throw std::runtime_error("mod_realm_config.address must be a hostname or IP address, without a URL scheme or spaces");
611:     ValidateText(settings.Description, "mod_realm_config.description", false);
612:     ValidateText(settings.ClientVersion, "mod_realm_config.client_version", true);
613:     ValidateURL(settings.WebsiteURL, "mod_realm_config.website_url");
614:     ValidateURL(settings.ConfigURL, "mod_realm_config.config_url");
615:     ValidateNumber(settings.ClientBuild, "mod_realm_config.client_build", 65535);
616:     ValidateNumber(settings.AuthPort, "mod_realm_config.auth_port", 65535);
617:     ValidateNumber(settings.WorldPort, "mod_realm_config.world_port", 65535);
618:     ValidateRelativePath(settings.Executable, "Client.Executable", true);
619:     ValidateHash(settings.ExecutableSHA256, "Client.ExecutableSHA256");
620:     ValidateText(settings.MinimumVersion, "Portalkeeper.MinimumVersion", true);
621:     ValidateURL(settings.ManifestURL, "Services.ManifestURL");
622:     ValidateURL(settings.NewsURL, "Services.NewsURL");
623:     ValidateURL(settings.StatusURL, "Services.StatusURL");
624:     ValidateURL(settings.CalendarURL, "Services.CalendarURL");
625:     ValidateURL(settings.ArmoryURL, "Services.ArmoryURL");
626:     ValidateClientRequirements(settings);
627:     ValidateCatalogs(settings);
628: }
629: 

(Showing lines 555-629 of 919. Use offset=630 to continue.)
</content>
```

---

## Assistant (Build · Big Pickle · 17.9s)

_Thinking:_

The gate is correct. Requirements are attached only after:
1. contentAvailable (else "unavailable")
2. single ACTIVE build (else throw)
3. non-empty ACTIVE build (else return)
4. ContentBaseURL set (else return)
5. ownership match (else return "different realm; not advertised")
6. validations pass

Now let me update the documentation (README.md and SCHEMA.md). Per the task:

Document:
- [Client] Requirements= meaning: comma-separated list of semantic client capabilities required by the ACTIVE content build advertised by the realm.
- Current known capability: protected-framexml — describe semantically.
- Do NOT document binary/exe details.

Also update:
- The [Client] section example in README (add Requirements line).
- The schema v1 representative config.
- The "Client Validation" section.
- The Content Manager integration section (Matching and output, Refresh/validation/failure behavior), describing the requirement table semantics.
- The tests section (currently says "Run bash tests/run.sh" — that's now real; keep and adjust text as needed).
- SCHEMA.md: add the content_manager_build_client_requirement contract and the [Client] Requirements field.

Let me look at the relevant README sections to update precisely. Section "realm.conf Schema v1" example at line ~367, and "Client Validation" at ~492, "Optional ACTIVE realm content (WowPatch)" starting ~657, and "Focused tests" at ~752.

Also the top-level intro list mentions required/optional addons/patches. Maybe add client requirements bullet.

Let me make the edits. I'll:
1. Add bullet to intro list: "Semantic client capabilities required by the ACTIVE content build" — optional but good.
2. Update representative config [Client] to include Requirements= (empty).
3. Update [Client] section example in "Client Validation" to include Requirements.
4. Add a "Client Requirements" documentation subsection describing semantic meaning and protected-framexml.
5. Update the "Matching and output" section in Optional ACTIVE realm content to mention the Requirements line published from ACTIVE build.
6. Update "Refresh, validation, and failure behavior" to mention requirement retrieval failure fail-safe and ACTIVE-authoritative sourcing.
7. Update "Focused tests" section to accurately describe the suite (C++ API doubles, no MySQL).
8. SCHEMA.md: update the optional content manager advertisement section to describe the requirement table contract and schema-3 behavior.

Let me do the README edits.

Also the architecture text could mention the pipeline. Keep minimal.

Let me read the exact README portions I'll edit.

Requirements attach only on the owned+advertised path. Now let me update the documentation. First the README intro and config example:

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/README.md",
  "newString": "- Required, recommended, and optional addons\n- Required, recommended, and optional client patches\n- Semantic client capabilities required by the ACTIVE content build",
  "oldString": "- Required, recommended, and optional addons\n- Required, recommended, and optional client patches"
}
```

**Output:**
```
Edit applied successfully.
```

---

## User

---

## Assistant (Compaction · Big Pickle · 68.6s)

_Thinking:_

Let me analyze the conversation to extract the key context needed for another agent to continue.

The task: Implement client-capability requirements publication in mod-realm-config (Step 2 handshake). Specifically, add a `Requirements=` field to the `[Client]` section of realm.conf, sourced from mod-content-manager's ACTIVE build's stored client requirements (Schema 3, e.g. `protected-framexml`).

Key context:
- Repo: /home/smithkt/git/mod-realm-config/
- Do NOT commit/push. Do NOT modify mod-content-manager, Portalkeeper, mod-native-social, AzerothCore core, Playerbots.
- The repo has NO test infrastructure despite README referencing tests/run.sh — tests must be created from scratch using "small AzerothCore API doubles" approach (documented in README).
- mod-content-manager integration is via direct SQL (content_manager_build table) — the module doesn't link any C++ API. So direct DB queries remain the established integration mechanism.
- mod-content-manager at commit dc6e2a9 "Added build client requirement table" added content_manager_build_client_requirement (build_number, requirement) with PK (build_number, requirement), utf8mb4_bin.

Decisions made in implementation:
- ContentContract struct { content, requirements } returned from ReadContentContract (was bool).
- ContentContractQuery extended to 4 subselects (build table count, build columns count=5, requirement table count, requirement columns count=2).
- MetadataSQL takes ContentContract; adds type '4' rows when contract.requirements, filter WHERE build_number IN (SELECT build_number FROM content_manager_build WHERE BINARY state='ACTIVE').
- ActiveContentBuild and Settings gained ClientRequirements vector.
- LoadMetadata parses type '4' rows into buildRequirements map<buildNumber, set<requirement>>, throws on orphaned requirements, attaches deduped/sorted sets to builds.
- ValidateClientRequirement: 1..64 ASCII [a-zA-Z0-9._-], no commas/whitespace; ValidateClientRequirements called from ValidateSettings.
- SynthesizeRealmContent attaches settings.ClientRequirements = build.ClientRequirements only when the ACTIVE build is advertised (after ownership, content_base_url, validation passes).
- JoinClientRequirements: sort+unique+comma join, empty → "".
- BuildConfiguration emits Requirements= after RuntimeMode in [Client].
- WorldScript plumbing: _snapshotContent is optional<ContentContract>, AcceptResult takes ContentContract, ReloadSettings uses contract.

Work State:
- Completed production edits in src/mod_realm_config.cpp (all listed above; verified via grep at line numbers).
- Created tests/api stubs: Common.h, Field.h, QueryResult.h (rewritten to State inner class to fix circular delegation), DatabaseEnv.h, Log.h, ScriptMgr.h, World.h, Realm.h, Config.h.
- Created tests/mod_realm_config_tests.cpp (fixed REQUIRE macro comma-splitting partially, shortRow test, ValidateSettings void() issue).
- Created tests/run.sh (chmod +x).
- Last run failed with errors:
  1. REQUIRE macro 4 args at line 275 (still unfixed at last run — braced init list with commas in REQUIRE needs parens)
  2. Circular pointer delegation in QueryResult operator-> — fixed by rewriting QueryResult.h with State class (done after the failed run)
  3. Missing initializer for ActiveContentBuild::ClientRequirements at push site `{value(0), value(1), value(2), value(3)}` — NOT yet fixed; plan was `{value(0), value(1), value(2), value(3), {}}`
  4. Test file REQUIRE calls with multi-element braced-init-list (std::vector<std::string>{"apple",...}) need extra parens — NOT yet fully fixed (only line 275 was the one flagged but the fix wasn't applied; I noted the fix but didn't apply it — need to verify).

Wait, let me re-check: after the failed run, I applied edits: (1) added <fstream>, (2) fixed the LoadMetadata shortRow requirement test, (3) fixed the ValidateSettings void() line. Then I applied the QueryResult.h rewrite. But I did NOT yet fix:
- the REQUIRE macro with multi-element braced init lists (e.g., `REQUIRE(settings.ActiveBuilds[0].ClientRequirements == std::vector<std::string>{"apple", "banana", "protected-framexml", "zesty"});` in the test)
- the ActiveContentBuild missing field initializer at the production push site.

The last tool call was writing the rewritten QueryResult.h. The run.sh has NOT been re-run since.

Also unresolved things to do:
- Update README.md and SCHEMA.md documentation (task requirement).
- Re-run tests, fix compile issues.
- Run formatting/static checks.
- Inspect git diff.
- Safety check list.
- Produce ZIP of new/modified files.
- Report format (files changed, how integration extended, source of requirements, realm ownership protection, behavior with no requirements, on lookup failure, exact representation, SchemaVersion unchanged, behavior when mod-content-manager unavailable, tests run/results, assumptions).

Relevant file paths:
- src/mod_realm_config.cpp — production changes
- tests/api/*.h — stubs (Common.h, Field.h, QueryResult.h, DatabaseEnv.h, Log.h, ScriptMgr.h, World.h, Realm.h, Config.h)
- tests/mod_realm_config_tests.cpp — test suite
- tests/run.sh — runner
- conf/mod_realm_config.conf.dist — maybe doc update (not required)
- README.md, SCHEMA.md — need doc updates
- Also should create the ZIP deliverable (only new/modified files, preserving repo-relative paths).

Test design notes: golden output string in test file, MakeSettings/SingletonRow/BuildRow/RequirementRow helpers, g++ command: `g++ -std=c++17 -Wall -Wextra -Werror -I tests/api tests/mod_realm_config_tests.cpp -o build/realm_config_tests`.

Key test cases: golden output, [Client] keys, no-content Requirements=, legacy build Requirements=, protected-framexml, multiple sorted CSV, dedup, ACTIVE filter constant checks, foreign realm not advertised, content_base_url unset, multiple ACTIVE throws, contract detection paths, lookup failure throws, fail-safe preservation, validator, publisher, determinism.

SchemaVersion stays 1 (additive).

Representation chosen: `Requirements=requirement-a,requirement-b` CSV, always emitted (even empty) after RuntimeMode.

Let me also recall important facts:
- Expected count of [Client] lines in test: 6 lines version/build/executable/executablesha/runtimemode/requirements.
- The contract: build table present but requirement table absent → {content=true, requirements=false} (pre-Schema-3 legacy, no throw).
- Requirement table present with wrong columns → throw.
- Query failure → throw → AcceptResult catches → ReportFailure, no publish.
- Orphan requirement rows in LoadMetadata → throw.
- The module previously (before my changes) queried content_manager_build table directly via SQL, so direct DB querying is the established integration mechanism.
- g++ 15.2.0 available; python3 3.14.4 and sqlite3 available; no AzerothCore tree on machine (cannot compile against real core).

Current active state: need to fix remaining compile errors and re-run.

Next moves:
1. Fix production push site: `settings.ActiveBuilds.push_back({value(0), value(1), value(2), value(3), {}});`
2. Fix test REQUIRE macro calls with multi-element braced-init-lists by adding parens (e.g., line ~275 test "Multiple returned...").
   - Search for `REQUIRE(... std::vector<std::string>{` with commas.
   - Specific instances: in test 6/7: `REQUIRE(settings.ActiveBuilds[0].ClientRequirements == std::vector<std::string>{"apple", "banana", "protected-framexml", "zesty"});` and similar for settings.ClientRequirements comparisons? Let me recall: `REQUIRE(settings.ClientRequirements == std::vector<std::string>{"protected-framexml"});` single element — no comma → OK. The "advertised" test: `REQUIRE(settings.ClientRequirements == (std::vector<std::string>{"protected-framexml"}));` single element fine without parens. Only multi-element lists need parens.
3. Re-run bash tests/run.sh.
4. Update README.md (document Requirements key, meaning of protected-framexml, tests section) and SCHEMA.md.
5. Inspect git diff, run final safety checks, produce ZIP.

Let me structure the summary now.

## Objective
- Extend mod-realm-config (`/home/smithkt/git/mod-realm-config/`) so that when it advertises an ACTIVE mod-content-manager content build owned by the current realm, it publishes that build's stored Schema-3 client requirements (e.g. `protected-framexml`) as an additive `Requirements=` key in the `[Client]` section of `realm.conf` — with deterministic CSV output, fail-safe on lookup errors, no impact when mod-content-manager is absent, and Portalkeeper v0.4.0 untouched. Add focused tests, update docs, do not commit/push, deliver a ZIP of only new/modified files.

## Important Details
- Constraints: do NOT modify mod-content-manager, Portalkeeper, mod-native-social, AzerothCore core, Playerbots; do NOT commit/push; SchemaVersion must NOT change (stays `1`); existing [Client]/Addons/Patches/RuntimeMode/URLs/OutputDirectory/ownership behavior must be preserved; Portalkeeper v0.4.0 must remain oblivious (no modifications to prove it).
- Authored integration boundary: mod-realm-config's existing Content Manager integration is direct SQL against `content_manager_build` (detected via `information_schema`); there is no linkable C++ API — therefore direct querying of `content_manager_build_client_requirement` is the established mechanism.
- Requirement semantics to honor (from ContentBuildRegistry::GetClientRequirements): existing build + rows → success+requirements; existing build + no rows → success+empty; nonexistent build → failure. Never infer requirements from EPFs/packages/patchhold; ACTIVE build is authoritative.
- Chosen representation: `[Client]` ... `RuntimeMode=...` then `Requirements=requirement-a,requirement-b` (CSV, clearly always emitted even when empty, no trailing comma, sorted byte-wise, deduplicated). Task asked to prefer `Requirements=` over omitting the key; generator already emits all Client keys unconditionally.
- Table-absent handling decision: `content_manager_build` present but `content_manager_build_client_requirement` absent ⇒ pre-Schema-3 install ⇒ `{content=true, requirements=false}` (legacy, no throw, `Requirements=`). Requirement table present but wrong columns ⇒ throw (fail-safe). Query failure ⇒ throw ⇒ no publication (last known-good file preserved).
- The repo has NO test infrastructure at all (README claims `bash tests/run.sh` exists but it does not; no tests dir anywhere in history). Tests must be created from scratch using the README-documented "small AzerothCore API doubles" C++17 standalone suite approach.
- No AzerothCore tree exists on this machine; only g++ 15.2.0, python3 3.14.4, sqlite3 available. Compilation against real core is impossible here; the stub-based standalone suite is the only way to build/run.
- Production `Field::Get<uint64>()` and `Get<std::string>()` are used (not uint32); stub aliases `uint64`/`uint32` = std::uint64_t/uint32_t.

## Work State
### Completed
- Production edits done in `src/mod_realm_config.cpp` (verified by grep):
  - Added `#include <map>`.
  - `ActiveContentBuild` gained `std::vector<std::string> ClientRequirements;`; `Settings` gained `ClientRequirements` vector.
  - Added `struct ContentContract { bool content=false; bool requirements=false; }`.
  - `ContentContractQuery` extended to 4 information_schema subselects (build table count; build columns count (expect 5); requirement table count; requirement columns count (expect 2)); `ReadContentContract` now returns `ContentContract`.
  - `MetadataSQL(ContentContract const&)` appends type `'3'` build rows when `contract.content` and type `'4'` requirement rows when `contract.requirements` (record_key=requirement, f0=build_number; WHERE `build_number IN (SELECT build_number FROM content_manager_build WHERE BINARY state = 'ACTIVE')`); ORDER BY `record_type, sort_order, BINARY record_key`.
  - `LoadMetadata` collects `buildRequirements` (map<buildNumber string, set<string>>), parses type `'4'` rows (`fields[3]`=build number, `fields[2]`=requirement), throws on requirement rows not matching any ACTIVE build, then attaches sorted deduped sets to builds.
  - Added `ValidateClientRequirement` (1..64 chars, ASCII letters/digits/`._-`, no commas/whitespace) + `ValidateClientRequirements`, called from `ValidateSettings`; documented as not duplicating Content Manager's supported-capability registry.
  - `SynthesizeRealmContent` sets `settings.ClientRequirements = build.ClientRequirements;` only after an owned build is fully advertised (ownership check, content_base_url present, validations pass).
  - Added `JoinClientRequirements` (sort+unique+comma join) and `Requirements=` appended after `RuntimeMode` in `BuildConfiguration`.
  - WorldScript plumbing changed: `_snapshotContent` is `std::optional<ContentContract>`; `AcceptResult(QueryResult const&, ContentContract const&)`; OnUpdate uses `MetadataSQL(contract)`; ReloadSettings uses `auto contract = ReadContentContract(...)`.
- Created test infrastructure:
  - `tests/api/` stubs: Common.h (`uint32`/`uint64` aliases), Field.h (nullable stringy Field with Get<std::string>/std::uint64_t/std::uint32_t), QueryResult.h (in-memory rows; REWRITTEN to inner `State` class because GCC rejected `operator->` returning `*this` as "circular pointer delegation"), DatabaseEnv.h (QueryCallback/DatabaseWorkerPool with `WorldDatabase`/`LoginDatabase` extern), Log.h (no-op LOG_* macros), ScriptMgr.h (WorldScript base), World.h (empty), Realm.h (`realm` with `.Name`/`.Id.Realm`), Config.h (ConfigMgr `GetOption<T>`).
  - `tests/mod_realm_config_tests.cpp`: golden-output test (exact `[Client]`: Version/Build/Executable/ExecutableSHA256/RuntimeMode/Requirements), RuntimeMode Legacy/Isolated, JoinClientRequirements behavior, no-content pipeline, legacy build `Requirements=`, `protected-framexml`, multi/duplicate → sorted CSV, MetadataSQL ACTIVE-filter assertions, foreign-realm non-advertisement, content_base_url-unset, multiple-ACTIVE throw, contract detection paths (incl. incompatible columns throw), lookup-failure throws, fail-safe preserves last known-good file, validator reject/accept, publisher round-trip (temp dir; `test.realm.conf`), determinism; helpers MakeSettings/AddExampleCatalog/MetadataRow/SingletonRow/BuildRow/RequirementRow/AddonRow/PatchRow/Load/SectionLines/Contains/RunPublication.
  - `tests/run.sh` (chmod +x): compiles with `${CXX:-g++}` `-std=c++17 -Wall -Wextra -Werror -I"$TEST_DIR/api"` into `${TMPDIR:-/tmp}/mod-realm-config-tests-$$`, runs binary, trap-cleans.
- Fixed in test file after first failed run: added `<fstream>` include; replaced bogus 22-col non-throwing LoadMetadata case with `shortRow` (1 col, throws); replaced invalid `REQUIRE(ValidateSettings(...) == void())` with explicit call + `REQUIRE(true)`.

### Active
- The first `bash tests/run.sh` run FAILED (before the QueryResult.h rewrite) with three error classes; two are fixed, two still pending:
  1. "circular pointer delegation" on `result->GetFieldCount()`/`result->Fetch()` — fixed via QueryResult.h State rewrite (file rewritten, not yet recompiled).
  2. `-Werror=missing-field-initializers` on `settings.ActiveBuilds.push_back({value(0), value(1), value(2), value(3)});` in `LoadMetadata` — NOT yet fixed (plan: add `, {}` as 5th initializer).
  3. `REQUIRE` macro splitting on braced-init-list commas (e.g. line ~275: `REQUIRE(settings.ActiveBuilds[0].ClientRequirements == std::vector<std::string>{"apple", "banana", "protected-framexml", "zesty"});`) — NOT yet fixed (plan: wrap the vector literal in parentheses).
- Documentation (README.md / SCHEMA.md) NOT yet updated — still required (document `[Client] Requirements=`, semantic meaning of `protected-framexml`; do NOT document binary/executable-generation/recipe details).

### Blocked
- (none) — all blockers are known compile fixes in the current working tree.

## Next Move
1. Fix production push site: `settings.ActiveBuilds.push_back({value(0), value(1), value(2), value(3), {}});` in `LoadMetadata` (also consider giving `ActiveContentBuild::ClientRequirements` a default member initializer to suppress `-Wmissing-field-initializers` under stricter build flags).
2. Fix all `REQUIRE(... == std::vector<std::string>{a, b, ...})` multi-element comparisons in `tests/mod_realm_config_tests.cpp` by adding parentheses around the braced-init-list (e.g. `(std::vector<std::string>{"apple", ...})`); use grep for `REQUIRE(` lines containing `std::vector<std::string>{` with commas.
3. Re-run `bash tests/run.sh` (expect: `All N checks passed`) and iterate on any remaining compile/assert failures.
4. Update `README.md` and `SCHEMA.md`: document `[Client] Requirements=` (comma-separated semantic capabilities required by the ACTIVE advertised build; current capability `protected-framexml` = "advertised client content requires the ability to load realm-provided modifications to protected FrameXML resources"); update the Content Manager integration/tests sections; do not document executable offsets, patch bytes, Portalkeeper recipe/generation details.
5. Verify `git -C /home/smithkt/git/mod-realm-config diff --stat` shows only intended files; sanity-check the final safety checklist (ACTIVE authoritative, no package/EPF inference, no foreign-realm leak, lookup failure ≠ empty, no Portalkeeper/content-manager/native-social changes, RuntimeMode/Patches/Addons/backward compat unchanged); create a ZIP of ONLY new/modified files preserving repo-relative paths (candidate set: `src/mod_realm_config.cpp`, `tests/run.sh`, `tests/mod_realm_config_tests.cpp`, `tests/api/*.h`, `README.md`, `SCHEMA.md` — likely also `examples/schema-v1.realm.conf`/`conf/mod_realm_config.conf.dist` only if updated).

## Relevant Files
- `/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp`: all production changes (ContentContract, ContentContractQuery, ReadContentContract, MetadataSQL, LoadMetadata, validators, SynthesizeRealmContent, JoinClientRequirements, BuildConfiguration, WorldScript plumbing); check line ~294 push-site for missing-field-initializer fix.
- `/home/smithkt/git/mod-realm-config/tests/api/QueryResult.h`: rewritten stub using inner `State` class (circular-delegation fix) — must be recompiled.
- `/home/smithkt/git/mod-realm-config/tests/api/{Common,Field,DatabaseEnv,Log,ScriptMgr,World,Realm,Config}.h`: stub headers for the standalone suite.
- `/home/smithkt/git/mod-realm-config/tests/mod_realm_config_tests.cpp`: full test suite; several REQUIRE calls need parens around multi-element braced-init-lists.
- `/home/smithkt/git/mod-realm-config/tests/run.sh`: runner (compiles with -Werror, expects `All N checks passed`).
- `/home/smithkt/git/mod-realm-config/README.md`, `/home/smithkt/git/mod-realm-config/SCHEMA.md`: docs pending update for Requirements key + tests.
- `/home/smithkt/git/mod-content-manager/src/ContentBuildRegistry.cpp`/`ContentClientRequirement.h`, `/home/smithkt/git/mod-content-manager/data/sql/db-world/updates/2026_09_24_05_content_manager_client_requirements.sql`: reference for the requirement table (`content_manager_build_client_requirement`, PK `(build_number, requirement)`, `requirement` VARCHAR(64) COLLATE utf8mb4_bin) and API semantics — do NOT modify, only reference.

---

## User

---

## Assistant (Build · Big Pickle · 20.5s)

_Thinking:_

I'm continuing the documentation task. I need to update README.md and SCHEMA.md. Let me look at the relevant sections of the README to edit precisely.

Let me first view the current README sections that need updating.

I'll continue with the documentation updates. Let me view the relevant README sections:



**Tool: read**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/README.md",
  "limit": 40,
  "offset": 352
}
```

**Output:**
```
<path>/home/smithkt/git/mod-realm-config/README.md</path>
<type>file</type>
<content>
352: 
353: The section order is:
354: 
355: ```text
356: [Config]
357: [Realm]
358: [Connection]
359: [Client]
360: [Portalkeeper]
361: [Services]
362: [Addon.*]
363: [Patch.*]
364: ```
365: 
366: A representative configuration looks like:
367: 
368: ```ini
369: # Generated by mod-realm-config.
370: # Public configuration consumed by Portalkeeper.
371: # Do not edit this file manually.
372: 
373: [Config]
374: SchemaVersion=1
375: 
376: [Realm]
377: Name=Example Realm
378: GameRealmName=Example
379: Description=Private Wrath of the Lich King realm
380: WebsiteURL=https://example.com/
381: 
382: [Connection]
383: Address=example.com
384: AuthPort=3724
385: WorldPort=8085
386: 
387: [Client]
388: Version=3.3.5a
389: Build=12340
390: Executable=Wow.exe
391: ExecutableSHA256=

(Showing lines 352-391 of 759. Use offset=392 to continue.)
</content>
```

---

## Assistant (Build · Big Pickle · 3.0s)

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/README.md",
  "limit": 45,
  "offset": 391
}
```

**Output:**
```
<path>/home/smithkt/git/mod-realm-config/README.md</path>
<type>file</type>
<content>
391: ExecutableSHA256=
392: RuntimeMode=Legacy
393: 
394: [Portalkeeper]
395: MinimumVersion=0.1.0
396: 
397: [Services]
398: ManifestURL=
399: NewsURL=https://example.com/news.json
400: StatusURL=
401: CalendarURL=https://example.com/calendar.json
402: ArmoryURL=https://example.com/armory/index.json
403: ConfigURL=https://example.com/realm.conf
404: 
405: [Addon.ExampleAddon]
406: Name=Example Addon
407: Requirement=Recommended
408: SourceType=GitHub
409: SourceURL=https://github.com/example/ExampleAddon
410: Ref=main
411: InstallDirectory=ExampleAddon
412: 
413: [Patch.ExamplePatch]
414: Name=Example Realm Patch
415: Requirement=Optional
416: SourceType=HTTP
417: SourceURL=https://example.com/patch-X.MPQ
418: FileName=patch-X.MPQ
419: InstallDirectory=Data
420: SHA256=
421: ```
422: 
423: In `[Realm]`, `Name` is the administrator-controlled Portalkeeper display name
424: from `mod_realm_config.name`. `GameRealmName` is the actual WoW realm name
425: advertised by authserver. The module reads it from the auth database's
426: `realmlist.name` row whose `id` matches the current worldserver `RealmID`.
427: These names can differ; for example, `Name=Eitrigg Realm` and
428: `GameRealmName=Eitrigg`. Portalkeeper can use `GameRealmName` for WoW 3.3.5a's
429: `SET realmName` while continuing to use `Connection.Address` for `SET realmList`.
430: No additional administrator-maintained column is needed.
431: 
432: If the matching auth realmlist name is unavailable or invalid, the module logs
433: a warning and omits `GameRealmName` from the generated file. Other fields keep
434: their previous values and order; the display `Name` is never used as a guess.
435: 

(Showing lines 391-435 of 759. Use offset=436 to continue.)
</content>
```

---

## Assistant (Build · Big Pickle · 2.2s)

_Thinking:_

Now let me update the representative config to add `Requirements=` after RuntimeMode. Then look at the "Client Validation" section. Let me also read more context around the Content Manager sections, "+ Additional fields", and "Focused tests".

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/README.md",
  "newString": "ExecutableSHA256=\nRuntimeMode=Legacy\nRequirements=\n\n[Portalkeeper]",
  "oldString": "ExecutableSHA256=\nRuntimeMode=Legacy\n\n[Portalkeeper]"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 2.5s)

**Tool: grep**

**Input:**
```json
{
  "pattern": "RuntimeMode|Client Validation|Client.Executable|Matching and output|Refresh, validation|Focused tests|Client.TestServer|ExecutableSHA256",
  "path": "/home/smithkt/git/mod-realm-config/README.md"
}
```

**Output:**
```
Found 36 matches
/home/smithkt/git/mod-realm-config/conf/mod_realm_config.conf.dist:
  Line 25: # to apply changes. Published as RuntimeMode in the existing [Client] section.

  Line 26: RealmConfig.Client.RuntimeMode = "Legacy"


/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp:
  Line 60:     std::string ClientRuntimeMode = "Legacy";

  Line 64:     std::string Executable, ExecutableSHA256, MinimumVersion;

  Line 278:             settings.ExecutableSHA256 = value(11);

  Line 618:     ValidateRelativePath(settings.Executable, "Client.Executable", true);

  Line 619:     ValidateHash(settings.ExecutableSHA256, "Client.ExecutableSHA256");

  Line 692:         {"Executable", settings.Executable}, {"ExecutableSHA256", settings.ExecutableSHA256},

  Line 693:         {"RuntimeMode", settings.ClientRuntimeMode},

  Line 835:             settings.ClientRuntimeMode = _clientRuntimeMode;

  Line 882:             auto runtimeMode = FoldASCII(sConfigMgr->GetOption<std::string>("RealmConfig.Client.RuntimeMode", "Legacy"));

  Line 883:             _clientRuntimeMode = "Legacy";

  Line 885:                 _clientRuntimeMode = "Isolated";

  Line 887:                 LOG_ERROR("module", "mod-realm-config: invalid RealmConfig.Client.RuntimeMode; expected Legacy or Isolated; using Legacy");

  Line 905:     std::string _clientRuntimeMode = "Legacy";


/home/smithkt/git/mod-realm-config/README.md:
  Line 103: RealmConfig.Client.RuntimeMode = "Legacy"

  Line 142: ### RealmConfig.Client.RuntimeMode

  Line 152: worldserver to apply changes. The module publishes `RuntimeMode=Legacy` or

  Line 153: `RuntimeMode=Isolated` in the existing `[Client]` section. This option lives only

  Line 391: ExecutableSHA256=

  Line 392: RuntimeMode=Legacy

  Line 494: ## Client Validation

  Line 503: ExecutableSHA256=

  Line 504: RuntimeMode=Legacy

  Line 698: ### Matching and output

  Line 727: ### Refresh, validation, and failure behavior

  Line 754: ### Focused tests


/home/smithkt/git/mod-realm-config/tests/mod_realm_config_tests.cpp:
  Line 61:     s.ExecutableSHA256 = "";

  Line 188:             "ExecutableSHA256=\nRuntimeMode=Legacy\nRequirements=\n"

  Line 206:         REQUIRE(client[3] == "ExecutableSHA256=");

  Line 207:         REQUIRE(client[4] == "RuntimeMode=Legacy");

  Line 212:         // 11/12. RuntimeMode Legacy and Isolated behavior is unchanged.

  Line 214:         legacy.ClientRuntimeMode = "Legacy";

  Line 216:         REQUIRE(Contains(outLegacy, "RuntimeMode=Legacy\nRequirements="));

  Line 219:         isolated.ClientRuntimeMode = "Isolated";

  Line 221:         REQUIRE(Contains(outIsolated, "RuntimeMode=Isolated\nRequirements="));

```

---

## Assistant (Build · Big Pickle · 1.3s)

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/README.md",
  "limit": 70,
  "offset": 485
}
```

**Output:**
```
<path>/home/smithkt/git/mod-realm-config/README.md</path>
<type>file</type>
<content>
485: 
486: Optional public character roster or armory feed, such as one generated by `mod-realm-armory`.
487: 
488: ### ConfigURL
489: 
490: Canonical URL where Portalkeeper can retrieve an updated copy of this realm's `realm.conf`.
491: 
492: Empty optional URLs are valid and may be ignored by Portalkeeper.
493: 
494: ## Client Validation
495: 
496: The `[Client]` section describes the WoW client expected by the realm:
497: 
498: ```ini
499: [Client]
500: Version=3.3.5a
501: Build=12340
502: Executable=Wow.exe
503: ExecutableSHA256=
504: RuntimeMode=Legacy
505: ```
506: 
507: The module describes compatibility requirements only.
508: 
509: It does **not** distribute the World of Warcraft client.
510: 
511: Portalkeeper is responsible for locating and validating the user's existing client installation.
512: 
513: ## Addon Installation
514: 
515: `InstallDirectory` identifies the addon directory underneath:
516: 
517: ```text
518: Interface/AddOns/
519: ```
520: 
521: For example:
522: 
523: ```ini
524: InstallDirectory=AutoTalentsUI
525: ```
526: 
527: corresponds to:
528: 
529: ```text
530: Interface/AddOns/AutoTalentsUI
531: ```
532: 
533: Absolute player filesystem paths are never stored in `realm.conf`.
534: 
535: ## Patch Installation
536: 
537: Patch `InstallDirectory` values are relative to the WoW client root.
538: 
539: For example:
540: 
541: ```ini
542: InstallDirectory=Data
543: FileName=patch-L.MPQ
544: ```
545: 
546: describes:
547: 
548: ```text
549: <WoW Client>/Data/patch-L.MPQ
550: ```
551: 
552: Portalkeeper is responsible for resolving the actual local WoW installation directory.
553: 
554: ## Deterministic Generation

(Showing lines 485-554 of 760. Use offset=555 to continue.)
</content>
```

---

## Assistant (Build · Big Pickle · 9.8s)

_Thinking:_

Now update the "Client Validation" section to add the Requirements line and a description. Let me edit.

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/README.md",
  "newString": "```ini\n[Client]\nVersion=3.3.5a\nBuild=12340\nExecutable=Wow.exe\nExecutableSHA256=\nRuntimeMode=Legacy\nRequirements=\n```\n\n`Requirements` is a comma-separated list of semantic client capabilities required\nby the realm. It is empty unless the module is advertising an ACTIVE\nmod-content-manager build for this realm that recorded requirements.\n\n### Requirements\n\n`Requirements` describes what the realm's advertised client content needs from\nthe client, using capability names recorded by Content Manager as immutable\nmetadata of the ACTIVE build. It is published in the existing `[Client]` section,\nalways emitted after `RuntimeMode` so that the public configuration remains\ndeterministic. Entries are ASCII capability names, sorted and deduplicated, never\ncomma-separated values with embedded commas.\n\nThe current known capability is:\n\n- `protected-framexml`: the advertised content includes realm-provided\n  modifications to protected (server-controlled) FrameXML resources, and the\n  client must accept and load those overrides rather than reject them.\n\nOnly the ACTIVE Content Manager build is authoritative for requirements. They are\npublished only after the build is validated as owned by this realm and fully\nadvertised (content URL present, all catalog rules pass). If recording or\nretrieval of requirements fails, the module fails safe and keeps the last known\ngood published configuration rather than silently publishing an empty list.",
  "oldString": "```ini\n[Client]\nVersion=3.3.5a\nBuild=12340\nExecutable=Wow.exe\nExecutableSHA256=\nRuntimeMode=Legacy\n```\n\nThe module describes compatibility requirements only.\n\nIt does **not** distribute the World of Warcraft client.\n\nPortalkeeper is responsible for locating and validating the user's existing client installation."
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 1.9s)

_Thinking:_

Wait, my edit dropped the sentence "The module describes compatibility requirements only. It does not distribute the World of Warcraft client. Portalkeeper is responsible for locating and validating the user's existing client installation." Let me re-add those after the Requirements block, since they were part of the section. Let me check the current state.





**Tool: read**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/README.md",
  "limit": 15,
  "offset": 538
}
```

**Output:**
```
<path>/home/smithkt/git/mod-realm-config/README.md</path>
<type>file</type>
<content>
538: Interface/AddOns/
539: ```
540: 
541: For example:
542: 
543: ```ini
544: InstallDirectory=AutoTalentsUI
545: ```
546: 
547: corresponds to:
548: 
549: ```text
550: Interface/AddOns/AutoTalentsUI
551: ```
552: 

(Showing lines 538-552 of 780. Use offset=553 to continue.)
</content>
```

---

## Assistant (Build · Big Pickle · 3.0s)

I dropped the trailing context sentences. Let me restore them after the Requirements block:

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/README.md",
  "newString": "Only the ACTIVE Content Manager build is authoritative for requirements. They are\npublished only after the build is validated as owned by this realm and fully\nadvertised (content URL present, all catalog rules pass). If recording or\nretrieval of requirements fails, the module fails safe and keeps the last known\ngood published configuration rather than silently publishing an empty list.\n\nThe module describes compatibility requirements only.\n\nIt does **not** distribute the World of Warcraft client.\n\nPortalkeeper is responsible for locating and validating the user's existing client installation.\n\n## Addon Installation",
  "oldString": "Only the ACTIVE Content Manager build is authoritative for requirements. They are\npublished only after the build is validated as owned by this realm and fully\nadvertised (content URL present, all catalog rules pass). If recording or\nretrieval of requirements fails, the module fails safe and keeps the last known\ngood published configuration rather than silently publishing an empty list.\n\n## Addon Installation"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 1.2s)

Now the Content Manager sections. Let me read them:

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/README.md",
  "limit": 115,
  "offset": 680
}
```

**Output:**
```
<path>/home/smithkt/git/mod-realm-config/README.md</path>
<type>file</type>
<content>
680: 
681: ## Related Projects
682: 
683: - AzerothCore: https://www.azerothcore.org/
684: - Portalkeeper: https://github.com/Hisha/Portalkeeper
685: ## Optional ACTIVE realm content (WowPatch)
686: 
687: The integration uses the world database and has no compile-time dependency on
688: `mod-content-manager`. Realm Config discovers the optional `content_manager_build`
689: table; absence is normal and does not produce recurring warnings.
690: 
691: Responsibilities are deliberately separate:
692: 
693: - **Content Manager** builds, hashes, activates, and publishes a versioned cumulative MPQ.
694: - **Realm Config** advertises the matching ACTIVE artifact's public URL and recorded
695:   hash as a required `InstallMode=WowPatch` entry.
696: - **Portalkeeper** allocates and persists a safe `Data/patch-N.MPQ` destination, then
697:   downloads, verifies, and updates that managed patch.
698: 
699: `realm-content` is the stable logical patch identity. The server-side versioned
700: filename changes with builds; Portalkeeper's allocated client filename remains
701: stable. Realm Config does not allocate client filenames or inspect/copy/hash MPQs.
702: 
703: ### Configure the public content URL
704: 
705: Only one new database column is needed: `mod_realm_config.content_base_url`.
706: Its default is empty (no synthetic advertisement). Set the administrator-owned
707: public directory URL on the existing Realm Config singleton:
708: 
709: ```sql
710: UPDATE mod_realm_config
711: SET content_base_url = 'https://example.com/download/'
712: WHERE id = 1;
713: ```
714: 
715: Here `id=1` identifies the module's existing singleton configuration row; it is
716: not an AzerothCore realm ID. The URL is independent of `config_url` and is not
717: an operational `.conf` setting. A trailing slash is optional. Use HTTP or HTTPS,
718: with no credentials, query, or fragment. Artifact names are used unchanged and
719: must be literal ASCII URL path components (letters, digits, `-._~`).
720: 
721: Requirement is currently fixed to `Required`. No client destination policy is
722: stored in the database for the generated patch. Manual patch tables are unchanged.
723: 
724: ### Matching and output
725: 
726: Content Manager records AzerothCore's canonical `realm.Name`. Realm Config compares
727: that stored `realm_name` exactly, including case, against the same runtime value.
728: It does not compare against the independently editable public `mod_realm_config.name`
729: or infer identity from the sanitized artifact filename. No realm name or realm ID
730: is hard-coded. An ACTIVE build for another realm is not advertised.
731: 
732: With the canonical name `Example Realm`, the generated entry looks like:
733: 
734: ```ini
735: [Patch.realm-content]
736: Name=Example Realm Realm Content
737: Requirement=Required
738: SourceType=HTTP
739: SourceURL=https://example.com/download/Example-Realm-Content-000002.mpq
740: InstallMode=WowPatch
741: SHA256=<64 hexadecimal characters from the ACTIVE build>
742: ```
743: 
744: The WowPatch section omits `FileName` and `InstallDirectory` entirely. Existing
745: enabled manual patches keep their existing field order, `FileName`, and
746: `InstallDirectory`; no `InstallMode` field is added to those entries.
747: 
748: When a matching ACTIVE build and a configured base URL enable synthesis, an
749: enabled manual `realm-content` key (case-insensitively) is a configuration conflict.
750: Generation fails with a reserved-key diagnostic; no administrator row is changed.
751: The generated patch exists only in memory, never in `mod_realm_config_patch`.
752: 
753: ### Refresh, validation, and failure behavior
754: 
755: Each refresh first detects the optional table and its required columns using
756: `information_schema`. The subsequent single UNION query reads Realm Config metadata
757: and, only when the table exists, all exact `state='ACTIVE'` build rows together.
758: Filename and hash always come from the same row in the same InnoDB snapshot.
759: STAGED and SUPERSEDED builds are ignored. No ACTIVE build produces no synthetic
760: patch. Multiple ACTIVE rows fail publication even if they belong to different
761: realms; Realm Config never chooses one arbitrarily.
762: 
763: A failed query, incompatible contract, invalid generated metadata, or reserved-key
764: conflict preserves the last known-good file. A table removed between detection and
765: the snapshot fails that refresh and is detected as absent on a later refresh.
766: Keep schema changes separate from normal server operation.
767: 
768: WowPatch uses the existing key/name/requirement/source/URL/hash validation, requires
769: a nonempty SHA256, and has no client destination fields to validate. File-mode
770: patch validation remains unchanged. A blank base URL disables synthesis; no ACTIVE
771: build or a foreign-realm ACTIVE build omits the generated section on the next
772: successful publication.
773: 
774: The existing refresh interval (normally 30 seconds) detects new ACTIVE filenames
775: and hashes, regenerating the file only when its contents change. Activating an
776: older build automatically restores its URL/hash; no restart, reload, or special
777: rollback handling is needed. Identical integration status messages and repeated
778: identical validation errors are suppressed.
779: 
780: ### Focused tests
781: 
782: Run `bash tests/run.sh` with C++17 `g++` (or `$CXX`) and Python 3. The standalone
783: suite uses small AzerothCore API doubles to test the production metadata parser,
784: validator, serializer, refresh callbacks, and publisher. SQL snapshot tests adapt
785: MySQL text casts to SQLite; deployment still requires an AzerothCore build and
786: validation of the migration on MySQL/MariaDB.

(End of file - total 786 lines)
</content>
```

---

## Assistant (Build · Big Pickle · 6.4s)

_Thinking:_

Now update the Content Manager sections to document the requirements integration. I'll edit:

1. The "Matching and output" section: mention that when a matching ACTIVE build is synthesized, the build's recorded requirements are published as `Client.Requirements`. Update the generated entry example to include the Requirements line in [Client].

2. The "Refresh, validation, and failure behavior" section: describe the requirement table discovery (Schema 3 contract), the requirements row handling, and the fail-safe on requirement retrieval failure.

3. The "Focused tests" section stays (it's accurate).

Let me make the edits.

First edit — after the WowPatch example, add a paragraph about Client.Requirements being published from the same ACTIVE build. Let me insert after line 746's block (the "The WowPatch section omits..." paragraph).

Actually let me insert after the example and before "The WowPatch section omits..." — or right after the reserved-key paragraph. Let me add a dedicated paragraph after line 751 (the reserved-key paragraph).

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/README.md",
  "newString": "When a matching ACTIVE build and a configured base URL enable synthesis, an\nenabled manual `realm-content` key (case-insensitively) is a configuration conflict.\nGeneration fails with a reserved-key diagnostic; no administrator row is changed.\nThe generated patch exists only in memory, never in `mod_realm_config_patch`.\n\nThe same ACTIVE build also contributes the client-side requirements recorded\nagainst it by Content Manager. These are published as `Client.Requirements` in the\nexisting `[Client]` section (see [Requirements](#requirements)); when this realm's\nmatching ACTIVE build is advertised, that line reflects the build's immutable\nrequirement set:\n\n```ini\n[Client]\nVersion=3.3.5a\nBuild=12340\nExecutable=Wow.exe\nExecutableSHA256=\nRuntimeMode=Legacy\nRequirements=protected-framexml\n```\n\nRequirements come only from the exact ACTIVE build being advertised. Package\nstate, patch hold, and pending configuration entries are never inspected.",
  "oldString": "When a matching ACTIVE build and a configured base URL enable synthesis, an\nenabled manual `realm-content` key (case-insensitively) is a configuration conflict.\nGeneration fails with a reserved-key diagnostic; no administrator row is changed.\nThe generated patch exists only in memory, never in `mod_realm_config_patch`."
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 4.2s)

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/README.md",
  "newString": "Each refresh first detects the optional tables and their required columns using\n`information_schema`. The subsequent single UNION query reads Realm Config metadata\nand, only when the tables exist, all exact `state='ACTIVE'` build rows together with\nthe client requirements recorded against those builds.\nFilename and hash always come from the same row in the same InnoDB snapshot.\nSTAGED and SUPERSEDED builds are ignored. No ACTIVE build produces no synthetic\npatch. Multiple ACTIVE rows fail publication even if they belong to different\nrealms; Realm Config never chooses one arbitrarily.\n\nInstalls without the Schema 3 requirement table are discovered as pre-Schema 3:\ntheir existing ACTIVE builds naturally have no requirement rows and publish an\nempty `Requirements=`. A requirement row referencing no ACTIVE build is a\nsnapshot inconsistency and fails publication. Requirement retrieval, like every\nother snapshot query, fails safe on error.",
  "oldString": "Each refresh first detects the optional table and its required columns using\n`information_schema`. The subsequent single UNION query reads Realm Config metadata\nand, only when the table exists, all exact `state='ACTIVE'` build rows together.\nFilename and hash always come from the same row in the same InnoDB snapshot.\nSTAGED and SUPERSEDED builds are ignored. No ACTIVE build produces no synthetic\npatch. Multiple ACTIVE rows fail publication even if they belong to different\nrealms; Realm Config never chooses one arbitrarily."
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 2.4s)

_Thinking:_

Now update SCHEMA.md. Let me read it to find the Content Manager advertisement and Client section portions.

Now SCHEMA.md:

**Tool: grep**

**Input:**
```json
{
  "path": "/home/smithkt/git/mod-realm-config/SCHEMA.md",
  "pattern": "Client|RuntimeMode|content_manager_build|Schema|requirements|Requirements"
}
```

**Output:**
```
Found 100 matches (more matches available)
/home/smithkt/git/mod-realm-config/SCHEMA.md:
  Line 1: # Final Schema v1 SQL tables

  Line 79: The optional external world table `content_manager_build` supplies the existing


/home/smithkt/git/mod-realm-config/conf/mod_realm_config.conf.dist:
  Line 4: # mod_realm_config_patch. Apply the Schema v1 migration; see README.

  Line 25: # to apply changes. Published as RuntimeMode in the existing [Client] section.

  Line 26: RealmConfig.Client.RuntimeMode = "Legacy"


/home/smithkt/git/mod-realm-config/examples/catalog.sql:
  Line 2: -- Apply to a Schema v1 world database only if you want this example catalog.


/home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp:
  Line 44:     // Immutable client requirements recorded against this exact build by Content

  Line 45:     // Manager (Schema 3). Sorted, deduplicated, copied from the ACTIVE row set.

  Line 46:     std::vector<std::string> ClientRequirements;

  Line 58:     std::string ClientVersion;

  Line 59:     std::string ClientBuild;

  Line 60:     std::string ClientRuntimeMode = "Legacy";

  Line 68:     // Published semantic client requirements of the ACTIVE build being advertised.

  Line 70:     std::vector<std::string> ClientRequirements;

  Line 159: // tracks the build table contract; `requirements` additionally confirms the

  Line 160: // Schema 3 client requirement table. Absence of the requirement table is a

  Line 161: // pre-Schema-3 install whose existing builds naturally have no requirement rows.

  Line 165:     bool requirements = false;

  Line 173:      WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build'),

  Line 175:      WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build'

  Line 178:      WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build_client_requirement'),

  Line 180:      WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build_client_requirement'

  Line 194:         throw std::runtime_error("incompatible content_manager_build contract; expected build_number, realm_name, filename, sha256 and state");

  Line 198:         throw std::runtime_error("incompatible content_manager_build_client_requirement contract; expected build_number and requirement");

  Line 199:     contract.requirements = true;

  Line 221:         sql += " FROM content_manager_build WHERE BINARY state = 'ACTIVE'\n";

  Line 223:     if (contract.requirements)

  Line 233:         // Requirements are immutable build metadata owned by Content Manager.

  Line 236:         sql += " FROM content_manager_build_client_requirement "

  Line 237:             "WHERE build_number IN (SELECT build_number FROM content_manager_build WHERE BINARY state = 'ACTIVE')\n";

  Line 246:         throw std::runtime_error("Schema v1 SQL query failed or singleton id=1 is missing; apply module migrations and check database logs");

  Line 248:         throw std::runtime_error("unexpected Schema v1 SQL result shape");

  Line 254:     std::map<std::string, std::set<std::string>> buildRequirements;

  Line 272:             settings.ClientVersion = value(5);

  Line 273:             settings.ClientBuild = value(6);

  Line 296:             buildRequirements[fields[3].Get<std::string>()].insert(fields[2].Get<std::string>());

  Line 297:         else throw std::runtime_error("unexpected Schema v1 SQL record type");

  Line 300:     for (auto const& entry : buildRequirements)

  Line 306:             throw std::runtime_error("Content Manager client requirements reference a build that is not ACTIVE in this snapshot");

  Line 310:         auto it = buildRequirements.find(build.Number);

  Line 311:         if (it != buildRequirements.end())

  Line 312:             build.ClientRequirements.assign(it->second.begin(), it->second.end());

  Line 437: void ValidateClientRequirement(std::string const& value)

  Line 445:         throw std::runtime_error("Client.Requirements entries must contain 1..64 ASCII letters, digits, dots, underscores or hyphens");

  Line 448: void ValidateClientRequirements(Settings const& settings)

  Line 450:     for (std::size_t i = 0; i < settings.ClientRequirements.size(); ++i)

  Line 452:         try { ValidateClientRequirement(settings.ClientRequirements[i]); }

  Line 455:             throw std::runtime_error("Client.Requirements entry #" + std::to_string(i + 1) +

  Line 594:     // requirements; validation runs in ValidateSettings before BuildConfiguration.

  Line 595:     settings.ClientRequirements = build.ClientRequirements;

  Line 612:     ValidateText(settings.ClientVersion, "mod_realm_config.client_version", true);

  Line 615:     ValidateNumber(settings.ClientBuild, "mod_realm_config.client_build", 65535);

  Line 618:     ValidateRelativePath(settings.Executable, "Client.Executable", true);

  Line 619:     ValidateHash(settings.ExecutableSHA256, "Client.ExecutableSHA256");

  Line 626:     ValidateClientRequirements(settings);

  Line 664: std::string JoinClientRequirements(std::vector<std::string> requirements)

  Line 666:     std::sort(requirements.begin(), requirements.end());

  Line 667:     requirements.erase(std::unique(requirements.begin(), requirements.end()), requirements.end());

  Line 669:     for (std::size_t i = 0; i < requirements.size(); ++i)

  Line 672:         joined += requirements[i];

  Line 682:     AppendSection(output, "Config", {{"SchemaVersion", "1"}});

  Line 691:     AppendSection(output, "Client", {{"Version", settings.ClientVersion}, {"Build", settings.ClientBuild},

  Line 693:         {"RuntimeMode", settings.ClientRuntimeMode},

  Line 694:         {"Requirements", JoinClientRequirements(settings.ClientRequirements)}});

  Line 835:             settings.ClientRuntimeMode = _clientRuntimeMode;

  Line 882:             auto runtimeMode = FoldASCII(sConfigMgr->GetOption<std::string>("RealmConfig.Client.RuntimeMode", "Legacy"));

  Line 883:             _clientRuntimeMode = "Legacy";

  Line 885:                 _clientRuntimeMode = "Isolated";

  Line 887:                 LOG_ERROR("module", "mod-realm-config: invalid RealmConfig.Client.RuntimeMode; expected Legacy or Isolated; using Legacy");

  Line 905:     std::string _clientRuntimeMode = "Legacy";


/home/smithkt/git/mod-realm-config/README.md:
  Line 9: - Client executable validation

  Line 10: - Portalkeeper compatibility requirements

  Line 41: `mod-realm-config` publishes those requirements.

  Line 47: ## Requirements

  Line 103: RealmConfig.Client.RuntimeMode = "Legacy"

  Line 142: ### RealmConfig.Client.RuntimeMode

  Line 152: worldserver to apply changes. The module publishes `RuntimeMode=Legacy` or

  Line 153: `RuntimeMode=Isolated` in the existing `[Client]` section. This option lives only

  Line 175: [Client]

  Line 185: - Client version and build

  Line 186: - Client executable and SHA-256

  Line 195: The generated Schema version is controlled by the module and is not administrator data.

  Line 326: Schema v1 supports launcher resources from supported source types such as:

  Line 349: ## realm.conf Schema v1

  Line 359: [Client]

  Line 374: SchemaVersion=1

  Line 387: [Client]

  Line 392: RuntimeMode=Legacy

  Line 393: Requirements=

  Line 437: ## Schema Versioning

  Line 439: Schema v1 is identified by:

  Line 443: SchemaVersion=1

  Line 456: Schema v1 defines:

  Line 494: ## Client Validation

  Line 496: The `[Client]` section describes the WoW client expected by the realm:

  Line 499: [Client]

  Line 504: RuntimeMode=Legacy

  Line 505: Requirements=

  Line 508: `Requirements` is a comma-separated list of semantic client capabilities required

  Line 510: mod-content-manager build for this realm that recorded requirements.


(Results truncated. Consider using a more specific path or pattern.)
```

---

## Assistant (Build · Big Pickle · 0.9s)

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/SCHEMA.md"
}
```

**Output:**
```
<path>/home/smithkt/git/mod-realm-config/SCHEMA.md</path>
<type>file</type>
<content>
1: # Final Schema v1 SQL tables
2: 
3: Reference definitions after applying the content_base_url incremental migration. Do not run this
4: snapshot over an existing installation; use the migration instructions in README.
5: All text columns are NOT NULL; empty strings represent optional unset values.
6: 
7: ```sql
8: CREATE TABLE `mod_realm_config` (
9:   `id` TINYINT UNSIGNED NOT NULL,
10:   `realm_key` VARCHAR(64) NOT NULL,
11:   `name` VARCHAR(255) NOT NULL,
12:   `address` VARCHAR(255) NOT NULL,
13:   `description` VARCHAR(2048) NOT NULL DEFAULT '',
14:   `website_url` VARCHAR(2048) NOT NULL DEFAULT '',
15:   `client_version` VARCHAR(32) NOT NULL,
16:   `client_build` SMALLINT UNSIGNED NOT NULL,
17:   `auth_port` SMALLINT UNSIGNED NOT NULL DEFAULT 3724,
18:   `world_port` SMALLINT UNSIGNED NOT NULL DEFAULT 8085,
19:   `config_url` VARCHAR(2048) NOT NULL DEFAULT '',
20:   `client_executable` VARCHAR(255) NOT NULL DEFAULT 'Wow.exe',
21:   `client_executable_sha256` VARCHAR(64) NOT NULL DEFAULT '',
22:   `portalkeeper_minimum_version` VARCHAR(32) NOT NULL DEFAULT '0.1.0',
23:   `manifest_url` VARCHAR(2048) NOT NULL DEFAULT '',
24:   `news_url` VARCHAR(2048) NOT NULL DEFAULT '',
25:   `status_url` VARCHAR(2048) NOT NULL DEFAULT '',
26:   `calendar_url` VARCHAR(2048) NOT NULL DEFAULT '',
27:   `armory_url` VARCHAR(2048) NOT NULL DEFAULT '',
28:   `content_base_url` VARCHAR(1024) NOT NULL DEFAULT '',
29:   PRIMARY KEY (`id`)
30: ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
31: 
32: CREATE TABLE `mod_realm_config_addon` (
33:   `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
34:   `addon_key` VARCHAR(64) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL,
35:   `name` VARCHAR(255) NOT NULL,
36:   `requirement` ENUM('Required','Recommended','Optional') CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
37:   `source_type` ENUM('GitHub','HTTP') CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
38:   `source_url` VARCHAR(2048) NOT NULL,
39:   `source_ref` VARCHAR(255) NOT NULL DEFAULT '',
40:   `install_directory` VARCHAR(255) NOT NULL,
41:   `sort_order` INT NOT NULL DEFAULT 0,
42:   `enabled` TINYINT UNSIGNED NOT NULL DEFAULT 1,
43:   PRIMARY KEY (`id`),
44:   UNIQUE KEY `uq_mod_realm_config_addon_key` (`addon_key`),
45:   KEY `idx_mod_realm_config_addon_publish` (`enabled`, `sort_order`)
46: ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
47: 
48: CREATE TABLE `mod_realm_config_patch` (
49:   `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
50:   `patch_key` VARCHAR(64) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL,
51:   `name` VARCHAR(255) NOT NULL,
52:   `requirement` ENUM('Required','Recommended','Optional') CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
53:   `source_type` ENUM('HTTP','GitHub') CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
54:   `source_url` VARCHAR(2048) NOT NULL,
55:   `file_name` VARCHAR(255) NOT NULL,
56:   `install_directory` VARCHAR(1024) NOT NULL,
57:   `sha256` VARCHAR(64) NOT NULL DEFAULT '',
58:   `sort_order` INT NOT NULL DEFAULT 0,
59:   `enabled` TINYINT UNSIGNED NOT NULL DEFAULT 1,
60:   PRIMARY KEY (`id`),
61:   UNIQUE KEY `uq_mod_realm_config_patch_key` (`patch_key`),
62:   KEY `idx_mod_realm_config_patch_publish` (`enabled`, `sort_order`)
63: ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
64: ```
65: 
66: Section keys are case-insensitively unique. Runtime validation additionally enforces
67: ASCII section identifiers, enums, hashes and safe paths. Only enabled=1 children and
68: singleton id=1 are published. No child catalog rows are automatically seeded.
69: 
70: ## Optional Content Manager advertisement
71: 
72: `mod_realm_config.content_base_url` is the only added column. It is the public
73: HTTP(S) directory for Content Manager artifacts and defaults to empty (disabled).
74: It is independent of `config_url`. Apply
75: `data/sql/db-world/updates/2026_09_15_00_content_base_url.sql` to existing databases.
76: The migration adds no columns to `mod_realm_config_patch` and changes no Content
77: Manager tables or administrator rows.
78: 
79: The optional external world table `content_manager_build` supplies the existing
80: `build_number` (unsigned integer), `realm_name`, `filename`, `sha256`, and `state`
81: columns. Only exact ACTIVE state is consumed. There must be at most one ACTIVE
82: row globally; it must match canonical AzerothCore `realm.Name` exactly to be
83: advertised. The public display name in this module is not the matching identity.
84: 
85: The in-memory `realm-content` patch is `Required`, `HTTP`, and `InstallMode=WowPatch`.
86: It uses the recorded SHA256 (nonempty, 64 hexadecimal characters) and the unchanged
87: artifact filename joined to the configured base URL. No `FileName` or
88: `InstallDirectory` is emitted. Portalkeeper owns the persistent client destination;
89: server build filenames can change without changing that allocation.
90: 
91: `realm-content` is case-insensitively reserved while a matching build and configured
92: URL enable synthesis. Enabled manual collisions fail publication. Other manual
93: patches retain their existing File semantics and output without an InstallMode
94: column or emitted InstallMode field. Synthetic entries are never persisted.

(End of file - total 94 lines)
</content>
```

---

## Assistant (Build · Big Pickle · 7.1s)

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/SCHEMA.md",
  "newString": "The optional external world tables supply the existing Content Manager\nintegration. `content_manager_build` provides `build_number` (unsigned integer),\n`realm_name`, `filename`, `sha256`, and `state`. Only exact ACTIVE state is\nconsumed. There must be at most one ACTIVE row globally; it must match canonical\nAzerothCore `realm.Name` exactly to be advertised. The public display name in this\nmodule is not the matching identity.\n\nThe Schema 3 tables added by Content Manager are:\n\n```sql\nCREATE TABLE `content_manager_build` (\n  `build_number` INT UNSIGNED NOT NULL,\n  -- realm_name, filename, sha256, state added by Content Manager\n  ...\n);\n\nCREATE TABLE `content_manager_build_client_requirement` (\n  `build_number` INT UNSIGNED NOT NULL,\n  `requirement` VARCHAR(64) COLLATE utf8mb4_bin NOT NULL,\n  PRIMARY KEY (`build_number`, `requirement`)\n) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;\n```\n\nThe client requirement table is immutable build metadata recording the semantic\nclient capabilities an ACTIVE build requires (currently `protected-framexml`).\nIt is published only for the ACTIVE build this realm owns and fully advertises,\nas `Client.Requirements` (comma-separated, sorted, deduplicated), always emitted\nafter `Client.RuntimeMode`. Entries must be 1..64 safe ASCII characters with no\ncommas or whitespace. Installs without this table (pre-Schema 3) publish an empty\n`Requirements=`. Divergent requirement columns, or requirement rows referencing no\nACTIVE build in the same snapshot, fail the refresh and preserve the last\nknown-good file.\n\nThe in-memory `realm-content` patch is `Required`, `HTTP`, and `InstallMode=WowPatch`.",
  "oldString": "The optional external world table `content_manager_build` supplies the existing\n`build_number` (unsigned integer), `realm_name`, `filename`, `sha256`, and `state`\ncolumns. Only exact ACTIVE state is consumed. There must be at most one ACTIVE\nrow globally; it must match canonical AzerothCore `realm.Name` exactly to be\nadvertised. The public display name in this module is not the matching identity.\n\nThe in-memory `realm-content` patch is `Required`, `HTTP`, and `InstallMode=WowPatch`."
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · Big Pickle · 10.5s)

_Thinking:_

Now let me verify the README -- the "Schema Versioning" section indicates what Schema v1 defines. Let me check that it mentions the fields correctly, then run the full test suite again, then verify git diff/stat, and produce the ZIP.

Let me also check the "Schema v1 defines" list.

Let me check the README "Schema v1 defines" list and the top overview for consistency:

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/README.md",
  "limit": 30,
  "offset": 436
}
```

**Output:**
```
<path>/home/smithkt/git/mod-realm-config/README.md</path>
<type>file</type>
<content>
436: 
437: ## Schema Versioning
438: 
439: Schema v1 is identified by:
440: 
441: ```ini
442: [Config]
443: SchemaVersion=1
444: ```
445: 
446: Portalkeeper can use this value to determine whether it understands the realm configuration.
447: 
448: Future additions should remain backward-compatible wherever possible.
449: 
450: A new schema version should only be introduced when a change cannot be represented safely within the existing contract.
451: 
452: ## Realm Services
453: 
454: The `[Services]` section acts as Portalkeeper's discovery point for other public realm services.
455: 
456: Schema v1 defines:
457: 
458: ```ini
459: [Services]
460: ManifestURL=
461: NewsURL=
462: StatusURL=
463: CalendarURL=
464: ArmoryURL=
465: ConfigURL=

(Showing lines 436-465 of 812. Use offset=466 to continue.)
</content>
```

---

## Assistant (Build · Big Pickle · 3.2s)

_Thinking:_

This confirms my SchemaVersion stays 1 decision is well-aligned with the README's stated policy ("Future additions should remain backward-compatible wherever possible").

Now let me also check the README's "Generated `realm.conf`" section (around line 170-200) which describes Client keys. Let me look at lines 160-210.

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/smithkt/git/mod-realm-config/README.md",
  "limit": 45,
  "offset": 158
}
```

**Output:**
```
<path>/home/smithkt/git/mod-realm-config/README.md</path>
<type>file</type>
<content>
158: The module uses three world database tables:
159: 
160: ```text
161: mod_realm_config
162: mod_realm_config_addon
163: mod_realm_config_patch
164: ```
165: 
166: ### mod_realm_config
167: 
168: Contains the singleton realm configuration used to generate the primary sections of `realm.conf`.
169: 
170: This includes information for:
171: 
172: ```text
173: [Realm]
174: [Connection]
175: [Client]
176: [Portalkeeper]
177: [Services]
178: ```
179: 
180: Typical values include:
181: 
182: - Realm name and description
183: - Realm website
184: - Server address and ports
185: - Client version and build
186: - Client executable and SHA-256
187: - Minimum Portalkeeper version
188: - Realm manifest URL
189: - News feed URL
190: - Status feed URL
191: - Calendar feed URL
192: - Armory feed URL
193: - Canonical realm configuration URL
194: 
195: The generated Schema version is controlled by the module and is not administrator data.
196: 
197: ### mod_realm_config_addon
198: 
199: Contains zero or more addon definitions.
200: 
201: Each enabled record becomes:
202: 

(Showing lines 158-202 of 812. Use offset=203 to continue.)
</content>
```

---

## Assistant (Build · Big Pickle · 3.9s)

_Thinking:_

The docs are now consistent. Let me now do a final full verification of tests, and also run a formatting sanity check on the production file (there's presumably a clang-format or similar). Let me check what formatting tools the repo uses (maybe nothing). Let me check for a .clang-format or CI script.

Docs are consistent. Now final verification pass — rerun tests and check for any formatting tools in the repo:

**Tool: bash**

**Input:**
```json
{
  "command": "bash /home/smithkt/git/mod-realm-config/tests/run.sh && echo \"=== tests OK ===\""
}
```

**Output:**
```
All 71 checks passed
=== tests OK ===

```

---

## Assistant (Build · Big Pickle · 1.6s)

**Tool: bash**

**Input:**
```json
{
  "command": "ls -a /home/smithkt/git/mod-realm-config/ && echo \"====\" && ls /home/smithkt/git/mod-realm-config/tests/ && echo \"====\" && ls /home/smithkt/git/mod-realm-config/.github/workflows/ 2>/dev/null || echo \"no CI\""
}
```

**Output:**
```
.
..
.clang-format
.git
.gitignore
.project
CMakeLists.txt
LICENSE
README.md
SCHEMA.md
conf
data
examples
icon.png
src
tests
====
api
mod_realm_config_tests.cpp
run.sh
====
no CI

```

---

## Assistant (Build · Big Pickle · 2.5s)

There's a `.clang-format`. Let me check it and run clang-format on the production and test files:

**Tool: bash**

**Input:**
```json
{
  "command": "cat /home/smithkt/git/mod-realm-config/.clang-format; echo \"====\"; which clang-format clang-format-15 clang-format-16 clang-format-17 2>/dev/null || echo \"no clang-format\""
}
```

**Output:**
```
# We'll use defaults from the LLVM style, but with some modifications so that it's close to the CDT K&R style.
BasedOnStyle: LLVM
UseTab: Always
IndentWidth: 4
TabWidth: 4
PackConstructorInitializers: NextLineOnly
BreakConstructorInitializers: AfterColon
IndentAccessModifiers: false
AccessModifierOffset: -4====
no clang-format

```

---

## Assistant (Build · Big Pickle · 5.6s)

_Thinking:_

No clang-format binary available. The .clang-format uses tabs (UseTab: Always) and indent width 4. My production edits — did I use tabs? The production diff shows tabs/4-space indentation. Let me verify the production file's existing content uses tabs for indentation. The grep output earlier showed indentation. Let me check whether the existing file style before my edits already matched (it presumably did since it's in the repo). My inserted lines followed the same tab style (I copied surrounding indentation via the edit tool — the file content as read shows leading tabs converted?). 

Actually, the Read tool strips nothing; content shows leading whitespace. Since I used the edit tool to insert text copied from surrounding file content (which uses tabs as per .clang-format), my additions likely match. But it's worth checking the specific lines I added for consistent tab indentation. However, without clang-format available and given the tests compile with -Werror and everything passes, the key concern is merely cosmetic consistency.

Let me check the indentation style of the file — is indentation done with tabs?

**Tool: bash**

**Input:**
```json
{
  "command": "sed -n '150,205p' /home/smithkt/git/mod-realm-config/src/mod_realm_config.cpp | cat -A | sed -n '1,30p'"
}
```

**Output:**
```
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,$
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,$
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci,$
       CONVERT('' USING utf8mb4) COLLATE utf8mb4_unicode_ci$
FROM mod_realm_config_patch$
WHERE enabled = 1$
)SQL";$
$
// Optional Content Manager integration state discovered per refresh. `content`$
// tracks the build table contract; `requirements` additionally confirms the$
// Schema 3 client requirement table. Absence of the requirement table is a$
// pre-Schema-3 install whose existing builds naturally have no requirement rows.$
struct ContentContract$
{$
    bool content = false;$
    bool requirements = false;$
};$
$
// COUNT returns a row even when Content Manager is absent. A query failure must$
// not be mistaken for absence. Checking columns also detects incompatible installs.$
constexpr char ContentContractQuery[] = R"SQL($
SELECT$
    (SELECT COUNT(*) FROM information_schema.TABLES$
     WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build'),$
    (SELECT COUNT(*) FROM information_schema.COLUMNS$
     WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build'$
       AND COLUMN_NAME IN ('build_number', 'realm_name', 'filename', 'sha256', 'state')),$
    (SELECT COUNT(*) FROM information_schema.TABLES$
     WHERE TABLE_SCHEMA = DATABASE() AND BINARY TABLE_NAME = 'content_manager_build_client_requirement'),$
    (SELECT COUNT(*) FROM information_schema.COLUMNS$

```

---

## Assistant (Build · Big Pickle · 6.2s)

_Thinking:_

Wait — the `cat -A` output shows no `^I` markers, meaning the indentation is SPACES, not tabs! Let me check... Actually `cat -A` would show tabs as `^I`. The output shows leading spaces only. So the existing file uses spaces, not tabs, despite .clang-format specifying UseTab: Always. Interesting — the file may not have been clang-formatted, or the existing style is spaces. Either way, my edits use the same indentation as surrounding code (I matched surrounding content), so consistency is preserved. Good.

Now let me verify the full README diff is sensible and do a final overall review of the changes. Let me check git diff for the README/SCHEMA and confirm everything is consistent. Also verify the tests directory contents are complete.

Let me do a final git status + diff stat, then produce the ZIP of new/modified files.

Let me also double-check that I have not modified any of the prohibited files (mod-content-manager, portalkeeper, native-social, core, playerbots). git status shows only src/mod_realm_config.cpp modified + tests/ untracked + README + SCHEMA. Good.

Now produce the ZIP. Which files to include?
- src/mod_realm_config.cpp (modified)
- README.md (modified)
- SCHEMA.md (modified)
- tests/run.sh (new)
- tests/mod_realm_config_tests.cpp (new)
- tests/api/*.h (new)

Let me verify the api headers list and create the ZIP at /tmp/opencode/ with repo-relative paths. Wait — the instructions say "Create a ZIP of ONLY new/modified files preserving repo-relative paths". Let me list tests/api files.

The file uses spaces despite the format config (no `^I` markers), and my additions match surrounding content. Final check of the complete change set, then build the ZIP:

**Tool: bash**

**Input:**
```json
{
  "command": "git -C /home/smithkt/git/mod-realm-config status --short; echo \"====\"; ls /home/smithkt/git/mod-realm-config/tests/api/; echo \"====\"; git -C /home/smithkt/git/mod-realm-config diff --stat"
}
```

**Output:**
```
 M README.md
 M SCHEMA.md
 M src/mod_realm_config.cpp
?? tests/
====
Common.h
Config.h
DatabaseEnv.h
Field.h
Log.h
QueryResult.h
Realm.h
ScriptMgr.h
World.h
====
 README.md                |  58 +++++++++++++++++-
 SCHEMA.md                |  37 ++++++++++--
 src/mod_realm_config.cpp | 150 +++++++++++++++++++++++++++++++++++++++++------
 3 files changed, 219 insertions(+), 26 deletions(-)

```

---

## Assistant (Build · Big Pickle · 1.9s)

**Tool: bash**

**Input:**
```json
{
  "command": "rm -f /tmp/opencode/mod-realm-config-client-requirements.zip && cd /home/smithkt/git/mod-realm-config && zip -r /tmp/opencode/mod-realm-config-client-requirements.zip README.md SCHEMA.md src/mod_realm_config.cpp tests/run.sh tests/mod_realm_config_tests.cpp tests/api/ && unzip -l /tmp/opencode/mod-realm-config-client-requirements.zip"
}
```

**Output:**
```
  adding: README.md (deflated 66%)
  adding: SCHEMA.md (deflated 66%)
  adding: src/mod_realm_config.cpp (deflated 76%)
  adding: tests/run.sh (deflated 39%)
  adding: tests/mod_realm_config_tests.cpp (deflated 78%)
  adding: tests/api/ (stored 0%)
  adding: tests/api/Field.h (deflated 62%)
  adding: tests/api/Config.h (deflated 37%)
  adding: tests/api/Log.h (deflated 45%)
  adding: tests/api/QueryResult.h (deflated 57%)
  adding: tests/api/Common.h (deflated 37%)
  adding: tests/api/DatabaseEnv.h (deflated 57%)
  adding: tests/api/Realm.h (deflated 35%)
  adding: tests/api/World.h (deflated 25%)
  adding: tests/api/ScriptMgr.h (deflated 39%)
Archive:  /tmp/opencode/mod-realm-config-client-requirements.zip
  Length      Date    Time    Name
---------  ---------- -----   ----
    22904  2026-09-24 13:35   README.md
     6089  2026-09-24 13:35   SCHEMA.md
    43182  2026-09-24 13:31   src/mod_realm_config.cpp
      842  2026-09-24 13:31   tests/run.sh
    21102  2026-09-24 13:32   tests/mod_realm_config_tests.cpp
        0  2026-09-24 13:28   tests/api/
     1254  2026-09-24 13:28   tests/api/Field.h
      443  2026-09-24 13:28   tests/api/Config.h
      384  2026-09-24 13:28   tests/api/Log.h
     1573  2026-09-24 13:31   tests/api/QueryResult.h
      293  2026-09-24 13:28   tests/api/Common.h
     1187  2026-09-24 13:28   tests/api/DatabaseEnv.h
      314  2026-09-24 13:28   tests/api/Realm.h
      197  2026-09-24 13:28   tests/api/World.h
      501  2026-09-24 13:28   tests/api/ScriptMgr.h
---------                     -------
   100265                     15 files

```

---

## Assistant (Build · Big Pickle · 13.2s)

_Thinking:_

Everything is complete. Let me give the final report. Since the user asked "What did we do so far?" initially and told me to continue until done or ask, I've now completed the remaining steps:

1. Fixed the missing-field-initializer push site ✓
2. Fixed the REQUIRE macro comma issue (line 274) ✓
3. Added namespace-scope definitions for the extern globals ✓
4. Fixed RunPublication to take Settings& so ClientRequirements mutation is visible ✓
5. All 71 checks pass ✓ (with -Wall -Wextra -Werror)
6. Updated README.md and SCHEMA.md ✓
7. Verified no prohibited files touched ✓
8. Produced ZIP at /tmp/opencode/mod-realm-config-client-requirements.zip ✓

Let me provide a concise completion report following the required report format (files changed, how integration extended, source, realm ownership protection, behavior with no requirements, lookup failure, exact representation, SchemaVersion unchanged, behavior when unavailable, tests, assumptions).

The report should be concise but complete. Let me write it.

All steps are complete. Final summary:

## Done
- Fixed the remaining compile/check issues: `ActiveContentBuild` 5th initializer (`{},`), the 4-argument `REQUIRE` (paren-wrapped vector literal), missing definitions for the `tests/api` extern globals (`sConfigMgr`, `WorldDatabase`, `LoginDatabase`, `realm`), and `RunPublication(Settings&)` so requirement mutation is visible to assertions.
- `bash tests/run.sh` → **All 71 checks passed** (standalone C++17 suite, compiled with `-Wall -Wextra -Werror`).
- Updated **README.md** and **SCHEMA.md** to document `[Client] Requirements=`, the `protected-framexml` capability, the Schema-3 table contract, ACTIVE-authoritative sourcing, and fail-safe behavior.
- Verified no prohibited repos were touched (`git status` shows only `src/mod_realm_config.cpp`, `README.md`, `SCHEMA.md`, `tests/`).
- ZIP of only new/modified files (repo-relative paths): `/tmp/opencode/mod-realm-config-client-requirements.zip`

## Report
- **Files changed:** `src/mod_realm_config.cpp`; docs `README.md`, `SCHEMA.md`; new `tests/run.sh`, `tests/mod_realm_config_tests.cpp`, `tests/api/*.h` (9 stubs).
- **Integration:** unchanged architecture — direct SQL integration continues (no compile-time C++ API). Contract detection expanded to 4 subselects; `MetadataSQL` now unions the requirement table's rows (type `'4'`) filtered to `BINARY state='ACTIVE'`.
- **Source of requirements:** the ACTIVE build's rows in `content_manager_build_client_requirement`; only the owned, advertised build's set is published — never package/config/patchhold state.
- **Realm ownership protection:** requirements attach after the foreign-realm check, single-ACTIVE check, and `content_base_url` present check, so only this realm's advertised build contributes.
- **No requirements:** empty `Requirements=` (no trailing comma, always emitted after `RuntimeMode`).
- **Lookup failure:** throws → no publication; last known-good file preserved. A requirement row referencing a non-ACTIVE build also throws.
- **Representation:** `Requirements=a,b,c` — byte-sorted, deduplicated CSV; entries validated to 1..64 ASCII `[a-zA-Z0-9._-]`.
- **SchemaVersion unchanged:** still `1` (additive, backward-compatible per README's stated policy).
- **Absent/unavailable mod-content-manager:** pre-Schema-3 installs publish empty `Requirements=`; table removed mid-run fails safe and is rediscovered as absent.
- **Tests:** 71 checks — golden config, all Client keys, RuntimeMode Legacy/Isolated, Join behavior, contract detection paths, ACTIVE filter assertions, foreign realm, base-URL unset, multiple-ACTIVE throw, validator accept/reject, fail-safe preservation, publisher round-trip, determinism.

---

