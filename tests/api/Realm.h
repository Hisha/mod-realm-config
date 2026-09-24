#ifndef REALM_CONFIG_TEST_REALM_H
#define REALM_CONFIG_TEST_REALM_H

#include "Common.h"

#include <string>

// Test-only double for the current realm identity advertised by worldserver.
struct RealmId
{
    uint32 Realm = 0;
};

struct Realm
{
    std::string Name;
    RealmId Id;
};

extern Realm realm;

#endif