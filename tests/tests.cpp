#define CATCH_CONFIG_MAIN
#include "../hola.hpp"
#include "catch.hpp"

using namespace hola;

TEST_CASE("Vector can be modified by using mutators", "[vector]")
{
    vec v{ 20, 30, 40 };
    set_x(v, 1);
    REQUIRE(v == vec{ 1, 30, 40 });
    set_y(v, 3);
    REQUIRE(v == vec{ 1, 3, 40 });
    set_z(v, 5);
    REQUIRE(v == vec{ 1, 3, 5 });
}

//constexpr tests, if they fail it won't compile
constexpr vec a{ 1,2,3 };
constexpr vec b{ 3,4,5 };

static_assert(get_x(a) == 1);
static_assert(get_y(a) == 2);
static_assert(get_z(a) == 3);
static_assert((a == b) == false);
static_assert((a != b) == true);
static_assert(dot(a, b) == 26);
static_assert(cross(a, b) == vec{ -2, 4, -2 });
