#include "../la.hpp"

using namespace la;

int main()
{
    constexpr vec a{1,2,3};
    constexpr vec b{3,4,5};
    
    static_assert(x(a) == 1);
    static_assert(y(a) == 2);
    static_assert(z(a) == 3);
    static_assert((a == b) == false);
    static_assert((a != b) == true);
    static_assert(dot(a, b) == 26);
    static_assert(cross(a, b) == vec{-2, 4, -2});
    
    return 0;
}
