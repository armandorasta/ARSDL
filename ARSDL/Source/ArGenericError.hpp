#include "STD.hpp"

namespace ArSDL {
	class ArGenericError
	{
	public:
		ArGenericError() = default;
		explicit ArGenericError(std::string_view message) : m_Message{message} {}
		virtual ~ArGenericError() = default;

		ArGenericError(ArGenericError const&) = default;
		ArGenericError(ArGenericError&&) = default;
		ArGenericError& operator=(ArGenericError const&) = default;
		ArGenericError& operator=(ArGenericError&&) = default;


		virtual std::string GetMessage() const
		{
			return m_Message;
		}

	private:
		std::string m_Message{};
	};
}