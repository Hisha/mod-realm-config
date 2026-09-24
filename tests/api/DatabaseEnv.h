#ifndef REALM_CONFIG_TEST_DATABASEENV_H
#define REALM_CONFIG_TEST_DATABASEENV_H

#include "Common.h"
#include "QueryResult.h"

#include <functional>
#include <string>
#include <utility>

// Test-only double for the world/auth database worker pools. Queries return a
// null QueryResult by default; tests exercise the parser/serializer directly
// rather than through the pool.
class QueryCallback
{
public:
    template<class Callback>
    QueryCallback& WithCallback(Callback&& callback)
    {
        _callback = std::forward<Callback>(callback);
        return *this;
    }

    bool InvokeIfReady()
    {
        if (_callback)
        {
            _callback(QueryResult());
            return true;
        }
        return false;
    }

private:
    std::function<void(QueryResult)> _callback;
};

class DatabaseWorkerPool
{
public:
    QueryResult Query(std::string const&) { return QueryResult(); }

    template<class... Args>
    QueryResult Query(std::string const&, Args&&...) { return QueryResult(); }

    QueryCallback AsyncQuery(std::string const&) { return QueryCallback(); }
};

extern DatabaseWorkerPool WorldDatabase;
extern DatabaseWorkerPool LoginDatabase;

#endif