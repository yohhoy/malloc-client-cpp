// malloc-client-mdspan.hpp
// SPDX-License-Identifier: MIT
#ifndef MALLOC_CLIENT_MDSPAN_HPP_
#define MALLOC_CLIENT_MDSPAN_HPP_

#include <cassert>
#include "malloc-client.hpp"

namespace mc {

// AccessorPolicy of std::mdspan
template <typename T>
class RemoteMemoryAccessor {
    struct ElementProxy;
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
        assert(rmem_ != nullptr);
        return reference{*rmem_, p + sizeof(T) * i};
    }

    data_handle_type offset(data_handle_type p, size_t i) const noexcept
    {
        return p + sizeof(T) * i;
    }

private:
    struct ElementProxy {
        ElementProxy(RemoteMemory& rmem, RemoteAddress addr)
            : rmem_{rmem}, addr_{addr} {}
        ~ElementProxy() = default;
        ElementProxy(const ElementProxy&) = delete;
        void operator=(const ElementProxy&) = delete;

        operator element_type()
        {
            return rmem_.read<element_type>(addr_);
        }
        element_type operator=(T val)
        {
            return rmem_.write<element_type>(addr_, val);
        }

        RemoteMemory& rmem_;
        const RemoteAddress addr_;
    };

private:
    RemoteMemory* rmem_ = nullptr;
};

} // namespace mc

#endif // MALLOC_CLIENT_MDSPAN_HPP_
