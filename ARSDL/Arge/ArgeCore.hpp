#pragma once

#include <expected>
#include <vector>
#include <cassert>
#include <map>
#include <list>
#include <unordered_map>
#include <string>
#include <string_view>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <concepts>
#include <type_traits>
#include <functional>
#include <numbers>
#include <variant>
#include <array>
#include <stack>
#include <format>
#include <exception>
#include <stdexcept>
#include <optional>
#include <source_location>
#include <filesystem>
#include <stacktrace>
#include <random>
#include <charconv>
#include <iomanip>

#define ARSDL_DA(_cond) assert(_cond)

#ifndef NDEBUG
#define ARSDL_DEBUG_MODE
#endif // ^^^^ NDEBUG

namespace Arge {
	// Unsigned
	using size_t   = std::size_t;
	using uint8_t  = std::uint8_t;
	using uint16_t = std::uint16_t;
	using uint32_t = std::uint32_t;
	using uint64_t = std::uint64_t;
	// Signed
	using ssize_t  = std::make_signed_t<size_t>;
	using int8_t   = std::make_signed_t<uint8_t>;
	using int16_t  = std::make_signed_t<uint16_t>;
	using int32_t  = std::make_signed_t<uint32_t>;
	using int64_t  = std::make_signed_t<uint64_t>;

	// Much needed shorthands
	namespace views  = std::views;
	namespace ranges = std::ranges;
	namespace fs     = std::filesystem;
}