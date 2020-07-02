#pragma once

// CppPAL
#include <cpppal/compiler.h>

// C++ standard library
#include <array>

// Abseil
#include <absl/types/span.h>

namespace stdext
{
	template<typename T>
	using span = absl::Span<T>;

	template <typename ElementType>
	span<ElementType> make_span(span<ElementType> s) noexcept
	{
		return s;
	}

	template <typename T, std::size_t N>
	span<T> make_span(T (&arr)[N]) noexcept
	{
		return span<T>{arr};
	}

	template <typename T, std::size_t N>
	span<T> make_span(std::array<T, N>& arr) noexcept
	{
		return stdext::span<T>{arr};
	}

	template <typename T, std::size_t N>
	span<const T> make_span(const std::array<T, N>& arr) noexcept
	{
		return span<const T>{arr};
	}

	template <typename T>
	span<T> make_span(T* arr, size_t N) noexcept
	{
		return span<T>{arr, N};
	}

	template <typename Container>
	span<typename Container::value_type> make_span(Container& cont)
	{
		return span<typename Container::value_type>{cont};
	}

	template <typename Container>
	span<const typename Container::value_type> make_span(const Container& cont)
	{
		return span<const typename Container::value_type>{cont};
	}
}

