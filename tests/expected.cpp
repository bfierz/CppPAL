#include <catch2/catch.hpp>

#include <string>

#include <cpppal/expected.h>

cpppal::expected<int, std::string> mightFail(bool fail)
{
    if (!fail) {
        return 42;
    } else {
        return cpppal::make_unexpected("The call failed");
    }
}

TEST_CASE("cpppal::expected")
{
    REQUIRE(mightFail(false).has_value() == true);
    REQUIRE(mightFail(false).value() == 42);
    REQUIRE(mightFail(true).has_value() == false);
    REQUIRE(mightFail(true).error() == "The call failed");
}
