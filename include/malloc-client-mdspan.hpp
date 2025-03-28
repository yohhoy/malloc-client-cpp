// malloc-client-mdspan.hpp
// SPDX-License-Identifier: MIT
#ifndef MALLOC_CLIENT_MDSPAN_HPP_
#define MALLOC_CLIENT_MDSPAN_HPP_

#include <cassert>
#include <type_traits>
#include "malloc-client.hpp"


namespace mc {

// AccessorPolicy of std::mdspan class template
template <typename ElementType>
class RemoteMemoryAccessor {
    struct ElementProxy;
public:
    using element_type = ElementType;
    using data_handle_type = RemoteAddress;
    using reference = ElementProxy;
    using offset_policy = RemoteMemoryAccessor<ElementType>;

    static_assert(std::is_trivially_copyable_v<ElementType>,
        "ElementType shall be trivially copyable.");

    RemoteMemoryAccessor() = default;
    explicit RemoteMemoryAccessor(RemoteMemory& rmem)
        : rmem_(&rmem) {}
    ~RemoteMemoryAccessor() = default;
    RemoteMemoryAccessor(const RemoteMemoryAccessor&) = default;
    RemoteMemoryAccessor& operator=(const RemoteMemoryAccessor&) = default;

    reference access(data_handle_type p, size_t i) const
    {
        assert(rmem_ != nullptr);
        return {*rmem_, p + sizeof(element_type) * i};
    }

    data_handle_type offset(data_handle_type p, size_t i) const noexcept
    {
        return p + sizeof(element_type) * i;
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
        element_type operator=(element_type val)
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
