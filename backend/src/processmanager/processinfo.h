#pragma once

#include <string>
#include <string_view>

constexpr std::string_view pretty_delimiter{" - "};

struct ProcessInfo
{
    ProcessInfo() = default;
    ProcessInfo(std::string name, unsigned long pid, bool x64)
        : m_name{name}
        , m_pid{pid}
        , m_x64{x64} {};

    std::string m_name;
    unsigned long m_pid; // DWORD of microsoft;
    bool m_x64;          // Is process 64 bit?
    const std::string pretty() const{
        return m_name + std::string(pretty_delimiter) + std::to_string(m_pid)
               + std::string(pretty_delimiter) + (m_x64 ? "64 bit" : "32 bit");
    }
};
