#pragma once
#include <cstdint>
#include <string>

struct Snowflake
{
    int64_t value;

    Snowflake(int64_t val) : value(val) {};
    Snowflake() : value(0L) {};
    int8_t getUnixTimestamp() { return (value >> 22) + 1420070400000; }
    int8_t getWorkerID()      { return (value & 0x3E0000) >> 17; }
    int8_t getProcessID()     { return (value & 0x1F000) >> 12; }
    int8_t getIncrement()     { return value & 0xFFF; }
    std::string toString()    { return std::to_string(value); }
};