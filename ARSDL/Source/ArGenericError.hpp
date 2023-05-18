#include "STD.hpp"

namespace ArSDL {
	class ArGenericError
	{
	public:
		ArGenericError() = default;
		ArGenericError(std::string_view message) : m_message{message} {}

		ArGenericError(ArGenericError const&) = default;
		ArGenericError(ArGenericError&&) = default;
		ArGenericError& operator=(ArGenericError const&) = default;
		ArGenericError& operator=(ArGenericError&&) = default;

	public:
		virtual std::string GetMessage() const
		{
			return m_message;
		}

	private:
		std::string m_message{};
	};
}