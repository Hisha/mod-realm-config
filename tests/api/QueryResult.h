#ifndef REALM_CONFIG_TEST_QUERYRESULT_H
#define REALM_CONFIG_TEST_QUERYRESULT_H

#include "Common.h"
#include "Field.h"

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

// Test-only double for AzerothCore's QueryResult. Holds in-memory Field rows.
// The cursor starts on the first row; Fetch() returns the current row, and
// NextRow() advances. A default-constructed value is null/failed. operator->
// returns the inner state pointer (mirroring a smart pointer) so the production
// result->Foo() call sites compile unchanged.
class QueryResult
{
public:
    using Row = std::vector<Field>;

    class State
    {
    public:
        explicit State(std::vector<Row> source) : rows(std::move(source)) {}

        std::uint32_t GetFieldCount() const
        {
            if (index >= rows.size()) return 0;
            return static_cast<std::uint32_t>(rows[index].size());
        }

        Field* Fetch()
        {
            if (index >= rows.size()) return nullptr;
            return rows[index].data();
        }

        bool NextRow()
        {
            if (index + 1 >= rows.size()) return false;
            ++index;
            return true;
        }

        std::vector<Row> rows;
        std::size_t index = 0;
    };

    QueryResult() = default;
    explicit QueryResult(std::vector<Row> rows)
        : state(new State(std::move(rows))) {}

    explicit operator bool() const { return static_cast<bool>(state); }

    State* operator->() const { return state.get(); }

private:
    std::shared_ptr<State> state;
};

#endif