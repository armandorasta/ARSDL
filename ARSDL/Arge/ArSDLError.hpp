#pragma once
#include "ArgeCore.hpp"
#include "ArGenericError.hpp"

namespace Arge {
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

#define ARGE_ERROR_THROW_IF(_cond)  \
	if (_cond) throw ArSDLError{#_cond, __FILE__, static_cast<std::size_t>(__LINE__)}

#define ARGE_ERROR_HANDLE_NEG(_call)  ARGE_ERROR_THROW_IF((_call) < 0)
#define ARGE_ERROR_HANDLE_NULL(_call) ARGE_ERROR_THROW_IF((_call) == nullptr)

#ifdef ARGE_DEBUG_MODE

#define ARGE_ERROR_DEBUG_THROW_IF(_cond)    ARGE_ERROR_THROW_IF(_cond)
#define ARGE_ERROR_DEBUG_HANDLE_NEG(_call)  ARGE_ERROR_HANDLE_NEG(_call)
#define ARGE_ERROR_DEBUG_HANDLE_NULL(_call) ARGE_ERROR_HANDLE_NULL(_call)

#else // ^^^^ ARGE_DEBUG_MODE vvvv !ARGE_DEBUG_MODE

#define ARGE_ERROR_DEBUG_THROW_IF(_cond)
#define ARGE_ERROR_DEBUG_HANDLE_NEG(_call)
#define ARGE_ERROR_DEBUG_HANDLE_NULL(_call)

#endif
