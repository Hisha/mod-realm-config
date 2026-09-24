#ifndef REALM_CONFIG_TEST_SCRIPTMGR_H
#define REALM_CONFIG_TEST_SCRIPTMGR_H

#include "Common.h"

// Test-only double for AzerothCore's WorldScript base class. Registration and
// ScriptMgr routing are not exercised by the standalone suite.
class WorldScript
{
public:
    explicit WorldScript(char const* /*name*/) {}
    virtual ~WorldScript() = default;

    virtual void OnStartup() {}
    virtual void OnAfterConfigLoad(bool /*reload*/) {}
    virtual void OnUpdate(uint32 /*diff*/) {}
};

#endif