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
constexpr typename Vec::held_type dot_impl(Vec const& a, Vec const& b, std::index_sequence<I...>)
{
    return ((std::get<I>(a) * std::get<I>(b)) + ...);
}

template <typename VecTo, typename VecFrom, size_t... I>
constexpr VecTo convert_to_impl(VecFrom const& from, std::index_sequence<I...>)
{
    return VecTo{static_cast<typename VecTo::held_type>(std::get<I>(from))...};
}

template <typename Vec, size_t... I>
constexpr Vec add_impl(Vec const& a, Vec const& b, std::index_sequence<I...>)
{
    return Vec{std::get<I>(a) + std::get<I>(b)...};
}

template <typename Vec, size_t... I>
constexpr Vec sub_impl(Vec const& a, Vec const& b, std::index_sequence<I...>)
{
    return Vec{ std::get<I>(a) - std::get<I>(b)... };
}

template <typename Vec, size_t... I>
constexpr bool equals_impl(Vec const& a, Vec const& b, std::index_sequence<I...>)
{
    return ((std::get<I>(a) == std::get<I>(b)) && ...);
}
}  // namespace internal

template <typename T, size_t Size>
class vec : public std::array<T, Size>
{
   public:
    using held_type = T;

    constexpr static size_t size = Size;

    constexpr bool operator==(vec<T, Size> const& o) const
    {
        return internal::equals_impl(*this, o, std::make_index_sequence<Size>{});
    }

    constexpr bool operator!=(vec<T, Size> const& o) const
    {
        return !internal::equals_impl(*this, o, std::make_index_sequence<Size>{});
    }

    constexpr vec<T, Size> operator+(vec<T, Size> const& o) const
    {
        return internal::add_impl(*this, o, std::make_index_sequence<Size>{});
    }

    constexpr vec<T, Size> operator-(vec<T, Size> const& o) const
    {
        return internal::sub_impl(*this, o, std::make_index_sequence<Size>{});
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

    //TODO: make it constexpr
    T norm() const
    {
        return std::sqrt(std::apply(
            [&](auto const&... a) {
            return ((a*a) + ...);
        },
            get()));
    }

   private:
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
void set_x(Vec& v, typename Vec::held_type const& value)
{
    std::get<0>(v) = value;
}

template <typename Vec>
void set_y(Vec& v, typename Vec::held_type const& value)
{
    std::get<1>(v) = value;
}

template <typename Vec>
void set_z(Vec& v, typename Vec::held_type const& value)
{
    std::get<2>(v) = value;
}

template <typename VecTo, typename VecFrom>
constexpr VecTo convert_to(const VecFrom& from)
{
    return internal::convert_to_impl<VecTo>(
        from, std::make_index_sequence<VecFrom::size>{});
}

template <typename Vec>
constexpr auto dot(Vec const& a, Vec const& b)
{
    return internal::dot_impl(a, b, std::make_index_sequence<Vec::size>{});
}

template <typename Vec>
constexpr auto cross(Vec const& a, Vec const& b)
{
    return Vec{get_y(a) * get_z(b) - get_z(a) * get_y(b),
        get_z(a) * get_x(b) - get_x(a) * get_z(b),
        get_x(a) * get_y(b) - get_y(a) * get_x(b)};
}

template <typename Vec>
auto normalize(Vec const& v)
{
    return v * (1 / v.norm());
}
}  // namespace hola
