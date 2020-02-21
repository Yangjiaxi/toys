#define CATCH_CONFIG_MAIN

#include <Array.h>
#include <catch2/catch.hpp>

using std::string;
using std::vector;

Array<int> one() { return std::move(Array<int>{1, 2, 3, 4, 5}); }

TEST_CASE("Constructor::Fill") { REQUIRE(Array<int>(5, 1).data == vector<int>{1, 1, 1, 1, 1}); }

TEST_CASE("Constructor::init list") { REQUIRE(one().data == vector<int>{1, 2, 3, 4, 5}); }

TEST_CASE("Constructor::vector") {
    vector<int> a{1, 2, 3, 4, 5};
    REQUIRE(Array<int>(a).data == a);
}

TEST_CASE("Constructor::vector iterator") {
    vector<int> a{1, 2, 3, 4, 5};
    REQUIRE(
        Array<int>(a.begin() + 1, a.begin() + 4).data == vector<int>(a.begin() + 1, a.begin() + 4));
}

TEST_CASE("Operator::[]") {
    auto a = one();
    REQUIRE(a[0] == 1);
    REQUIRE(a[4] == 5);
    a[3] = 666;
    REQUIRE(a[3] == 666);
}

TEST_CASE("Method::pop/push/shift/unshift") {
    Array<int> a{0};
    REQUIRE(a.size() == 1);
    REQUIRE(a.push(1).data == vector<int>{0, 1});
    REQUIRE(a.unshift(2).data == vector<int>{2, 0, 1});
    REQUIRE(a.pop() == 1);
    REQUIRE(a.shift() == 2);
}

TEST_CASE("Method::every/some/find/findIndex") {
    auto a    = one();
    auto cond = [](int el) { return !(el % 2); };
    REQUIRE_FALSE(a.every(cond));
    REQUIRE(*(a.find(cond)) == 2);
    REQUIRE(a.some(cond));
    REQUIRE(a.findIndex(cond) == 1);
}

TEST_CASE("Method::includes") {
    auto a = one();
    int e  = 4;
    int ee = 6;
    REQUIRE(a.includes(e));
    REQUIRE(a.includes(4));
    REQUIRE_FALSE(a.includes(ee));
    REQUIRE_FALSE(a.includes(6));
}

TEST_CASE("Method::indexOf/lastIndexOf") {
    auto a = one(); // [1,2,3,4,5]
    int el = 6;
    REQUIRE(a.indexOf(3) == 2);
    REQUIRE(a.indexOf(3, 3) == -1);
    REQUIRE(a.indexOf(el) == -1);
    REQUIRE(a.indexOf(6) == -1);

    REQUIRE(a.lastIndexOf(3) == 2);
    REQUIRE(a.lastIndexOf(3, 3) == 2);
    REQUIRE(a.lastIndexOf(el) == -1);
    REQUIRE(a.lastIndexOf(6) == -1);
}

TEST_CASE("Method::copyWithin") {
    Array<char> a{'a', 'b', 'c', 'd', 'e'};
    REQUIRE(a.copyWithin(0, 3, 4).data == vector<char>{'d', 'b', 'c', 'd', 'e'});
    REQUIRE(a.copyWithin(1, 3).data == vector<char>{'d', 'd', 'e', 'd', 'e'});
}

TEST_CASE("Method::fill") {
    auto a = one();
    REQUIRE(a.fill(-1, 2, 4).data == vector<int>{1, 2, -1, -1, 5});
    REQUIRE(a.fill(-2, 3).data == vector<int>{1, 2, -1, -2, -2});
    REQUIRE(a.fill(-3).data == vector<int>{-3, -3, -3, -3, -3});
}

TEST_CASE("Method::reverse") { REQUIRE(one().reverse().data == vector<int>{5, 4, 3, 2, 1}); }

TEST_CASE("Method::sort") {
    Array<int> a{1, 1, 4, 5, 1, 4};
    REQUIRE(a.sort().data == vector<int>{1, 1, 1, 4, 4, 5});
    REQUIRE(a.sort([](int l, int r) {
                 return 1.0 / l < 1.0 / r;
             }).data == vector<int>{5, 4, 4, 1, 1, 1});
}

TEST_CASE("Method::splice") {
    Array<string> a{"angel", "clown", "mandarin", "sturgeon"};

    REQUIRE(
        a.splice(2, 0, {"drum"}).data ==
        vector<string>{"angel", "clown", "drum", "mandarin", "sturgeon"});

    REQUIRE(a.splice(3, 1).data == vector<string>{"angel", "clown", "drum", "sturgeon"});

    REQUIRE(
        a.splice(2, 1, {"trumpet"}).data ==
        vector<string>{"angel", "clown", "trumpet", "sturgeon"});

    REQUIRE(
        a.splice(0, 2, {"parrot", "anemone", "blue"}).data ==
        vector<string>{"parrot", "anemone", "blue", "trumpet", "sturgeon"});

    REQUIRE(a.splice(a.size() - 3, 2).data == vector<string>{"parrot", "anemone", "sturgeon"});

    REQUIRE(a.splice(1).data == vector<string>{"parrot"});
}

TEST_CASE("Method::from") { REQUIRE(one().from().data == one().data); }

TEST_CASE("Method::concat") {
    Array<int> a{1, 2, 3};
    Array<int> b{4, 5, 6};
    Array<int> c{7, 8, 9};
    REQUIRE(a.concat(b).concat(c).data == vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9});
}

TEST_CASE("Method::filter") {
    Array<string> a{"spray", "limit", "elite", "exuberant", "destruction", "present"};
    auto flt = [](const string s) { return s.length() > 6; };
    auto res = a.filter(flt);
    REQUIRE(res.data == vector<string>{"exuberant", "destruction", "present"});
}

TEST_CASE("Method::map") {
    auto mpr = [](int a) { return a * a; };
    auto res = one().map<int>(mpr);
    REQUIRE(res.data == vector<int>{1, 4, 9, 16, 25});
}

TEST_CASE("Method::slice") {
    Array<string> a{"ant", "bison", "camel", "duck", "elephant"};
    REQUIRE(a.slice(2).data == vector<string>{"camel", "duck", "elephant"});
    REQUIRE(a.slice(2, 4).data == vector<string>{"camel", "duck"});
    REQUIRE(a.slice(1, 5).data == vector<string>{"bison", "camel", "duck", "elephant"});
}
TEST_CASE("Method::reduce") {
    Array<int> a{1, 2, 3, 4};
    auto reducer = [](int acc, int cur) -> int { return acc + cur; };
    REQUIRE(a.reduce<int>(reducer, 0) == 10);
    REQUIRE(a.reduce<int>(reducer, 5) == 15);
}

TEST_CASE("Method::reduceRight") {
    Array<Array<int>> a{{0, 1}, {2, 3}, {4, 5}};
    auto rightReducer = [](Array<int> acc, Array<int> cur) { return acc.concat(cur); };
    auto res          = a.reduceRight<Array<int>>(rightReducer, Array<int>{});
    REQUIRE(res.data == vector<int>{4, 5, 2, 3, 0, 1});
}

TEST_CASE("Method::forEach") {
    auto a = one();
    vector<int> res;
    a.forEach([&res](int a) { res.push_back(a * 2); });
    REQUIRE(res == vector<int>{2, 4, 6, 8, 10});
}

TEST_CASE("Method::join") {
    REQUIRE(one().join() == "1, 2, 3, 4, 5");
    REQUIRE(one().join("") == "12345");
    REQUIRE(one().join("-") == "1-2-3-4-5");
}