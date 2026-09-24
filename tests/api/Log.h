#ifndef REALM_CONFIG_TEST_LOG_H
#define REALM_CONFIG_TEST_LOG_H

// Test-only doubles for AzerothCore's logging macros. The standalone suite
// asserts on return values/exceptions, not on emitted log lines.
#define LOG_ERROR(category, ...) ((void)0)
#define LOG_WARN(category, ...) ((void)0)
#define LOG_INFO(category, ...) ((void)0)
#define LOG_DEBUG(category, ...) ((void)0)

#endif