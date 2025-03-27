// malloc-client.hpp
// SPDX-License-Identifier: MIT
#ifndef MALLOC_CLIENT_HPP
#define MALLOC_CLIENT_HPP

#include <string>
#include <type_traits>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>


namespace mc {

using RemoteAddress = std::uint64_t;

class RemoteMemory {
public:
    RemoteMemory(std::string host = "http://localhost:8080")
        : endpoint_(host + "/memory")
        , header_{{"Content-Type", "application/json"}}
    {}
    ~RemoteMemory() = default;
    RemoteMemory(const RemoteMemory&) = delete;
    void operator=(const RemoteMemory&) = delete;

    RemoteAddress malloc(size_t size)
    {
        cpr::Body req = nlohmann::json{{"size", size}}.dump();
        cpr::Response r = cpr::Post(cpr::Url{endpoint_ + "/malloc"}, header_, req);
        if (r.error.code != cpr::ErrorCode::OK) {
            throw std::runtime_error(r.error.message);
        }
        auto resp = nlohmann::json::parse(r.text);
        if (r.status_code != 200) {
            throw std::runtime_error(resp["error"]);
        }
        return resp["addr"];
    }

    void free(RemoteAddress addr)
    {
        cpr::Body req = nlohmann::json{{"addr", addr}}.dump();
        cpr::Response r = cpr::Post(cpr::Url{endpoint_ + "/free"}, header_, req);
        if (r.error.code != cpr::ErrorCode::OK) {
            throw std::runtime_error(r.error.message);
        }
        auto resp = nlohmann::json::parse(r.text);
        if (r.status_code != 200) {
            throw std::runtime_error(resp["error"]);
        }
    }

    std::uint8_t read_byte(RemoteAddress addr) {
        cpr::Response r = cpr::Get(cpr::Url{std::format("{}/{}", endpoint_, addr)});
        if (r.error.code != cpr::ErrorCode::OK) {
            throw std::runtime_error(r.error.message);
        }
        auto resp = nlohmann::json::parse(r.text);
        if (r.status_code != 200) {
            throw std::runtime_error(resp["error"]);
        }
        return resp["val"];
    }

    void write_byte(RemoteAddress addr, std::uint8_t val) {
        cpr::Body req = nlohmann::json{{"val", val}}.dump();
        cpr::Response r = cpr::Put(cpr::Url{std::format("{}/{}", endpoint_, addr)}, header_, req);
        if (r.error.code != cpr::ErrorCode::OK) {
            throw std::runtime_error(r.error.message);
        }
        auto resp = nlohmann::json::parse(r.text);
        if (r.status_code != 200) {
            throw std::runtime_error(resp["error"]);
        }
    }

    template <typename T>
    T read(RemoteAddress addr) {
        static_assert(std::is_trivially_copyable_v<T>, "T shall be trivially copyable.");
        std::uint8_t mem[sizeof(T)];
        for (size_t i = 0; i < sizeof(T); i++) {
            mem[i] = read_byte(addr + i);
        }
        return std::bit_cast<T>(mem);
    }

    template <typename T>
    void write(RemoteAddress addr, T val) {
        static_assert(std::is_trivially_copyable_v<T>, "T shall be trivially copyable.");
        std::uint8_t mem[sizeof(T)];
        std::memcpy(mem, &val, sizeof(T));
        for (size_t i = 0; i < sizeof(T); i++) {
            write_byte(addr + i, mem[i]);
        }
    }

private:
    const std::string endpoint_;
    const cpr::Header header_; // cache
};

} // namespace mc

#endif // MALLOC_CLIENT_HPP
