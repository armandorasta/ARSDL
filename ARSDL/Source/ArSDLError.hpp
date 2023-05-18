#pragma once
#include "STD.hpp"
#include "ArGenericError.hpp"

namespace ArSDL {
	class ArSDLError : public ArGenericError
	{
	public:
		ArSDLError() = default;
		ArSDLError(std::string_view message, std::string_view file, size_t line);

		ArSDLError(ArSDLError const&) = default;
		ArSDLError(ArSDLError&&) = default;
		ArSDLError& operator=(ArSDLError const&) = default;
		ArSDLError& operator=(ArSDLError&&) = default;

	public:
		std::string GetMessage() const;

	private:
		std::string m_sdlMessage{};
		std::string_view m_file;
		size_t m_line;
	};
}

#define ARSDL_ERROR_HANDLE_NEG(_call)  \
	if (0        > _call) throw ArSDLError{#_call, __FILE__, static_cast<std::size_t>(__LINE__)}
#define ARSDL_ERROR_HANDLE_NULL(_call) \
	if (nullptr == _call) throw ArSDLError{#_call, __FILE__, static_cast<std::size_t>(__LINE__)}