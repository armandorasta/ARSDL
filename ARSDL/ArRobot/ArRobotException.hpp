#pragma once
#include "STD.hpp"

namespace ArRobot {
	class ArRobotException
	{
	public:
		ArRobotException();
		virtual ~ArRobotException() = default;

		template <class... FormatArgs>
		ArRobotException(std::string_view message, FormatArgs&&... fmtArgs)
		{
			std::vformat_to(std::back_inserter(m_Message), message,
				std::make_format_args(std::forward<FormatArgs>(fmtArgs)...));
		}

		std::string const& GetMessage() const noexcept;

		template <class... FormatArgs>
		void SetMessage(std::string_view newMessage, FormatArgs&&... fmtArgs)
		{
			m_Message.clear();
			std::vformat_to(std::back_inserter(m_Message), newMessage,
				std::make_format_args(std::forward<FormatArgs>(fmtArgs)...));
		}

	protected:
		std::string& GetMessage() noexcept;

	private:
		std::string m_Message{};
	};

	/// Thrown when a debug-only assertion fails. This means that the program can
	/// not continue and must terminate immediately. This may not be thrown in 
	/// Release.
	class DebugAssertionError : public ArRobotException
	{
	public:
		template <class... FormatArgs>
		DebugAssertionError(std::source_location debugInfo, std::string_view message, 
			FormatArgs&&... fmtArgs) : ArRobotException{""}
		{
			std::vformat_to(
				std::back_inserter(GetMessage()),
				std::string{message} + ".\n\tFile: {}.\n\tFunction: {}.\n\tLineNumber: {}",
				std::make_format_args(
					std::forward<FormatArgs>(fmtArgs)...,
					debugInfo.file_name(), debugInfo.function_name(), debugInfo.line()
				)
			);
		}

		template <class... FormatArgs>
		explicit DebugAssertionError(std::source_location debugInfo) 
			: DebugAssertionError{debugInfo, "No message"}
		{
		}
	};

	/// Thrown by functions can't throw any of the other types of errors.
	class GenericError : public ArRobotException
	{
	public:
		using ArRobotException::ArRobotException;
	};
}

#ifdef NDEBUG

#define AROBOT_DA(_cond, _message, ...)
#define AROBOT_UNREACHABLE_CODE() throw
#define AROBOT_NOT_IMPLEMENTED(_whatIsNotImplemented) throw

#else // ^^^^ Release, vvvv Debug

#define AROBOT_DA(_cond, ...) \
	if (!(_cond)) throw ::ArRobot::DebugAssertionError{std::source_location::current(), __VA_ARGS__}

#define AROBOT_UNREACHABLE_CODE()                     AROBOT_DA(false, "Unreachable code path")
#define AROBOT_NOT_IMPLEMENTED(_whatIsNotImplemented) AROBOT_DA(false, "Unimplemented code path: " _whatIsNotImplemented);

#endif // NDEBUG