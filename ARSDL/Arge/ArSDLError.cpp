#include "pch.h"
#include "ArSDLError.hpp"

#include <SDL/SDL.h>

namespace Arge {
	ArSDLError::ArSDLError(std::string_view message, std::string_view file, size_t line) :
		ArGenericError{message}, m_sdlMessage{SDL_GetError()},
		m_file{file}, m_line{line}
	{
	}

	std::string ArSDLError::GetMessage() const
	{
		if (ArGenericError::GetMessage().empty())
			return std::format("{}.\nFile: {}.\nLine: {}.", m_sdlMessage, m_file, m_line);
		else
			return std::format("{}: {}.\nFile: {}.\nLine: {}.", 
				ArGenericError::GetMessage(), m_sdlMessage, m_file, m_line);
	}
}
