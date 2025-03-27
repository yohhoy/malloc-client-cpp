// malloc-client-mdspan.hpp
// SPDX-License-Identifier: MIT
#ifndef MALLOC_CLIENT_MDSPAN_HPP_
#define MALLOC_CLIENT_MDSPAN_HPP_

#include "malloc-client.hpp"

namespace mc {

template <typename T>
class RemoteMemoryProxy {
public:
    RemoteMemoryProxy(RemoteMemory& rmem, RemoteAddress addr)
        : rmem_{rmem}, addr_{addr}
    {}
    ~RemoteMemoryProxy() = default;

    RemoteMemoryProxy(const RemoteMemoryProxy&) = delete;
    RemoteMemoryProxy& operator=(const RemoteMemoryProxy&) = default;

    operator T()
    {
        return rmem_.read<T>(addr_);
    }
    T operator=(T val)
    {
        return rmem_.write<T>(addr_, val);
    }

private:
    RemoteMemory& rmem_;
    const RemoteAddress addr_;
};

// AccessorPolicy of std::mdspan
template <typename T>
class RemoteMemoryAccessor {
private:
    struct ElementProxy {
        RemoteMemory& rmem_;
        const RemoteAddress addr_;

        ElementProxy(RemoteMemory& rmem, RemoteAddress addr)
            : rmem_{rmem}, addr_{addr} {}
        ~ElementProxy() = default;
        ElementProxy(const ElementProxy&) = delete;
        void operator=(const ElementProxy&) = delete;

        operator T() { return rmem_.read<T>(addr_); }
        T operator=(T val) { return rmem_.write<T>(addr_, val); }
    };

public:
    using element_type = T;
    using data_handle_type = RemoteAddress;
    using reference = ElementProxy;
    using offset_policy = RemoteMemoryAccessor<T>;

    RemoteMemoryAccessor() = default;
    RemoteMemoryAccessor(RemoteMemory& rmem)
        : rmem_(&rmem) {}
    ~RemoteMemoryAccessor() = default;
    RemoteMemoryAccessor(const RemoteMemoryAccessor&) = default;
    RemoteMemoryAccessor& operator=(const RemoteMemoryAccessor&) = default;

    reference access(data_handle_type p, size_t i) const
    {
        return reference{*rmem_, p + sizeof(T) * i};
    }

    data_handle_type offset(data_handle_type p, size_t i) const noexcept
    {
        return p + sizeof(T) * i;
    }

private:
    RemoteMemory* rmem_ = nullptr;
};

} // namespace mc

#endif // MALLOC_CLIENT_MDSPAN_HPP_
