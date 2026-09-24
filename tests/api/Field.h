#ifndef REALM_CONFIG_TEST_FIELD_H
#define REALM_CONFIG_TEST_FIELD_H

#include "Common.h"

#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>

// Test-only double for AzerothCore's Field. Stores a stringy value; numeric
// Get<T> parses it. A default-constructed Field reads as NULL.
class Field
{
public:
    Field() = default;
    Field(const char* value) : _null(false), _value(value) {}
    Field(std::string value) : _null(false), _value(std::move(value)) {}
    Field(std::uint64_t value) : _null(false), _value(std::to_string(value)) {}

    bool IsNull() const { return _null; }

    template<class T> T Get() const;

private:
    bool _null = true;
    std::string _value;
};

template<> inline std::string Field::Get<std::string>() const
{
    if (_null) throw std::runtime_error("Field::Get<std::string> on NULL");
    return _value;
}

template<> inline std::uint64_t Field::Get<std::uint64_t>() const
{
    if (_null) throw std::runtime_error("Field::Get<uint64> on NULL");
    return std::stoull(_value);
}

template<> inline std::uint32_t Field::Get<std::uint32_t>() const
{
    if (_null) throw std::runtime_error("Field::Get<uint32> on NULL");
    return static_cast<std::uint32_t>(std::stoul(_value));
}

#endif