#include <type_traits>
#include <array>

namespace la
{
namespace internal
{
template <typename Vec, size_t... I>
constexpr auto dot_impl(Vec const& a, Vec const& b, std::index_sequence<I...>)
{
    return ((std::get<I>(a) * std::get<I>(b)) + ...);
}
}  // namespace internal

template <typename... T>
class vec : public std::array<std::common_type_t<T...>, sizeof...(T)>
{
   public:
    constexpr static auto size = sizeof...(T);

    constexpr bool operator==(vec<T...> const& o) const
    {
        return equals(o, std::make_index_sequence<size>{});
    }

   private:
    template <size_t... I>
    constexpr bool equals(vec<T...> const& o, std::index_sequence<I...>) const
    {
        return ((std::get<I>(*this) == std::get<I>(o)) && ...);
    }
};

template <typename... T>
vec(T&&...)->vec<T...>;

template <typename Vec>
constexpr auto x(Vec const& v)
{
    return std::get<0>(v);
}

template <typename Vec>
constexpr auto y(Vec const& v)
{
    return std::get<1>(v);
}

template <typename Vec>
constexpr auto z(Vec const& v)
{
    return std::get<2>(v);
}

template <typename Vec>
constexpr auto dot(Vec const& a, Vec const& b)
{
    return internal::dot_impl(a, b, std::make_index_sequence<Vec::size>{});
}

template <typename Vec>
constexpr auto cross(Vec const& a, Vec const& b)
{
    return Vec{y(a) * z(b) - z(a) * y(b),
        z(a) * x(b) - x(a) * z(b),
        x(a) * y(b) - y(a) * x(b)};
}
}  // namespace la
