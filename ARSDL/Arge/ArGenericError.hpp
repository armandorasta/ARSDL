#pragma once
#include "ArgeCore.hpp"

namespace Arge {
	class ArGenericError
	{
	public:
		explicit ArGenericError(std::string_view message) : m_Message{message} {}

		ArGenericError()                                 = default;
		ArGenericError(ArGenericError const&)            = default;
		ArGenericError(ArGenericError&&)                 = default;
		ArGenericError& operator=(ArGenericError const&) = default;
		ArGenericError& operator=(ArGenericError&&)      = default;
		virtual ~ArGenericError()                        = default;

		virtual std::string GetMessage() const
		{
			return m_Message;
		}

	private:
		std::string m_Message{};
	};
}