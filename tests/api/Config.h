#ifndef REALM_CONFIG_TEST_CONFIG_H
#define REALM_CONFIG_TEST_CONFIG_H

#include "Common.h"

#include <string>

// Test-only double for the module configuration manager. The standalone suite
// does not drive ReloadSettings, so options always return their default.
class ConfigMgr
{
public:
    template<class T>
    T GetOption(std::string const& /*key*/, T const& fallback) const { return fallback; }
};

extern ConfigMgr* sConfigMgr;

#endif