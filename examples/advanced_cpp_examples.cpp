// Advanced modern C++20/23 example demonstrating concepts, constexpr, ranges,
// CRTP, strong types, fold expressions, and lightweight type erasure.
// This file is self-contained and can be compiled with a C++20 compiler.
//
// Example compile commands (Windows PowerShell):
//   - Using g++ (MinGW): g++ -std=c++20 -O2 -Wall -Wextra -pedantic examples\advanced_cpp_examples.cpp -o bin\advanced_cpp_examples.exe
//   - Using clang++:     clang++ -std=c++20 -O2 -Wall -Wextra -pedantic examples\advanced_cpp_examples.cpp -o bin\advanced_cpp_examples.exe
//
// Run:
//   .\bin\advanced_cpp_examples.exe

#include <algorithm>
#include <array>
#include <concepts>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <optional>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

// 1) Concepts: constraints for numeric types and streamable types.

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <typename T>
concept Streamable = requires(std::ostream& os, T const& t) {
    { os << t } -> std::same_as<std::ostream&>;
};

// 2) Strong types: type safe wrappers around primitive values (no accidental mixing).
//    Uses CRTP for shared operators and construction.

template <typename Derived, typename Rep>
struct Strong
{
    using rep = Rep;

    constexpr Strong() = default;
    explicit constexpr Strong(Rep v) : value(v) {}

    // Accessor with explicit intent
    constexpr Rep get() const { return value; }

    // Common comparable ops via CRTP hook in Derived
    friend constexpr auto operator<=>(Strong const& a, Strong const& b) = default;

protected:
    Rep value{};
};

struct Meters : Strong<Meters, double>
{
    using Strong::Strong; // inherit constructors
};

struct Seconds : Strong<Seconds, double>
{
    using Strong::Strong;
};

// Derived unit from strong types
struct MetersPerSecond : Strong<MetersPerSecond, double>
{
    using Strong::Strong;
};

constexpr MetersPerSecond operator/(Meters d, Seconds t)
{
    return MetersPerSecond{ d.get() / t.get() };
}

// 3) Fold expression for variadic sum with constraints

template <Arithmetic... Ts>
constexpr auto sum(Ts... xs)
{
    // Common type ensures no narrowing surprises
    using Common = std::common_type_t<Ts...>;
    return (Common{} + ... + static_cast<Common>(xs));
}

// 4) Constexpr algorithms: compile-time prime checking and prime generation view

constexpr bool is_prime(std::uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (std::uint64_t d = 3; d * d <= n; d += 2)
    {
        if (n % d == 0) return false;
    }
    return true;
}

// 5) CRTP printable mixin: enforces Derived to implement repr()

template <typename Derived>
struct Printable
{
    std::string to_string() const
    {
        auto const& self = static_cast<Derived const&>(*this);
        return self.repr();
    }
};

struct Point2D : Printable<Point2D>
{
    double x{};
    double y{};

    std::string repr() const { return std::string{"Point("} + std::to_string(x) + "," + std::to_string(y) + ")"; }
};

// 6) Lightweight type erasure for a "Drawable" concept using std::function
//    This avoids inheritance for simple polymorphism and demonstrates type erasure.

struct Drawable
{
    std::function<void(std::ostream&)> draw_fn;

    template <typename T>
        requires requires (T const& t, std::ostream& os) { { t.draw(os) }; }
    Drawable(T const& t) : draw_fn([=](std::ostream& os) { t.draw(os); }) {}

    void draw(std::ostream& os) const { draw_fn(os); }
};

struct Circle
{
    Point2D c; double r{};
    void draw(std::ostream& os) const { os << "Circle(c=" << c.to_string() << ", r=" << r << ")"; }
};

struct Rectangle
{
    Point2D p; double w{}, h{};
    void draw(std::ostream& os) const { os << "Rectangle(p=" << p.to_string() << ", w=" << w << ", h=" << h << ")"; }
};

// 7) Ranges: pipeline to produce the first N primes and transform them

std::vector<std::uint64_t> first_n_primes(std::size_t N)
{
    namespace rv = std::ranges::views;

    auto primes_view = rv::iota(std::uint64_t{2})
        | rv::filter([](std::uint64_t x) { return is_prime(x); })
        | rv::take(N);

    std::vector<std::uint64_t> out;
    out.reserve(N);
    for (auto v : primes_view) out.push_back(v);
    return out;
}

// 8) Small utility: safe_print for any Streamable type

template <Streamable T>
void safe_print(T const& t, std::string_view label = {})
{
    if (!label.empty()) std::cout << label << ": ";
    std::cout << t << '\n';
}

int main()
{
    // Strong types and units
    Meters d{42.0};
    Seconds t{6.0};
    auto v = d / t; // MetersPerSecond
    safe_print(v.get(), "speed (m/s)");

    // Fold expression sum
    auto s = sum(1, 2.5, 3u);
    safe_print(s, "sum(1, 2.5, 3u)");

    // CRTP to_string
    Point2D p{ .x = 1.25, .y = -3.5 };
    safe_print(p.to_string(), "point");

    // Type-erased drawables
    std::vector<Drawable> scene;
    scene.emplace_back(Circle{ .c = {0.0, 0.0}, .r = 10.0 });
    scene.emplace_back(Rectangle{ .p = { -5.0, 2.0 }, .w = 8.0, .h = 3.0 });

    std::cout << "scene: ";
    for (std::size_t i = 0; i < scene.size(); ++i)
    {
        scene[i].draw(std::cout);
        if (i + 1 != scene.size()) std::cout << ", ";
    }
    std::cout << '\n';

    // Ranges: first 15 primes and their squares
    auto primes = first_n_primes(15);
    std::cout << "first 15 primes: ";
    for (auto p : primes) std::cout << p << ' ';
    std::cout << '\n';

    std::cout << "squares: ";
    for (auto q : primes | std::ranges::views::transform([](auto x) { return x * x; }))
        std::cout << q << ' ';
    std::cout << '\n';

    // constexpr usage confirmation
    static_assert(is_prime(97));
    static_assert(!is_prime(100));

    return 0;
}
