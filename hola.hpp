#pragma once

#include <type_traits>
#include <tuple>
#include <array>
#include <math.h>

namespace hola
{
namespace internal
{
template <typename Vec, size_t... I>
constexpr auto dot_impl(Vec const& a, Vec const& b, std::index_sequence<I...>)
{
    return ((std::get<I>(a) * std::get<I>(b)) + ...);
}

template <typename VecTo, typename VecFrom, size_t... I>
constexpr VecTo convert_to_impl(VecFrom const& from, std::index_sequence<I...>)
{
    return VecTo{static_cast<typename VecTo::Type>(std::get<I>(from))...};
}
}  // namespace internal

template <typename T, size_t Size>
class vec : public std::array<T, Size>
{
   public:
    using Type = T;

    constexpr static auto SIZE = Size;

    constexpr bool operator==(vec<T, Size> const& o) const
    {
        return equals(o, std::make_index_sequence<SIZE>{});
    }

    constexpr bool operator!=(vec<T, Size> const& o) const
    {
        return !equals(o, std::make_index_sequence<SIZE>{});
    }

    template <typename MultT>
    constexpr auto operator*(MultT const& mult) const
    {
        return std::apply(
            [&](auto const&... a) {
                return vec<
                    typename std::common_type<decltype(mult * a)...>::type,
                    Size>{mult * a...};
            },
            get());
    }

   private:
    template <size_t... I>
    constexpr bool equals(
        vec<T, Size> const& o, std::index_sequence<I...>) const
    {
        return ((std::get<I>(*this) == std::get<I>(o)) && ...);
    }

    constexpr const std::array<T, Size>& get() const { return *this; }
};

template <typename... T>
vec(T&&...)->vec<typename std::common_type<T...>::type, sizeof...(T)>;

using vec2i = vec<int32_t, 2>;
using vec3i = vec<int32_t, 3>;
using vec2f = vec<float_t, 2>;
using vec3f = vec<float_t, 3>;

template <typename Vec>
constexpr auto get_x(Vec const& v)
{
    return std::get<0>(v);
}

template <typename Vec>
constexpr auto get_y(Vec const& v)
{
    return std::get<1>(v);
}

template <typename Vec>
constexpr auto get_z(Vec const& v)
{
    return std::get<2>(v);
}

template <typename Vec>
void set_x(Vec& v, decltype(get_x(v)) const& value)
{
    std::get<0>(v) = value;
}

template <typename Vec>
void set_y(Vec& v, decltype(get_y(v)) const& value)
{
    std::get<1>(v) = value;
}

template <typename Vec>
void set_z(Vec& v, decltype(get_z(v)) const& value)
{
    std::get<2>(v) = value;
}

template <typename VecTo, typename VecFrom>
constexpr VecTo convert_to(const VecFrom& from)
{
    return internal::convert_to_impl<VecTo>(
        from, std::make_index_sequence<VecFrom::SIZE>{});
}

template <typename Vec>
constexpr auto dot(Vec const& a, Vec const& b)
{
    return internal::dot_impl(a, b, std::make_index_sequence<Vec::SIZE>{});
}

template <typename Vec>
constexpr auto cross(Vec const& a, Vec const& b)
{
    return Vec{get_y(a) * get_z(b) - get_z(a) * get_y(b),
        get_z(a) * get_x(b) - get_x(a) * get_z(b),
        get_x(a) * get_y(b) - get_y(a) * get_x(b)};
}
}  // namespace hola
