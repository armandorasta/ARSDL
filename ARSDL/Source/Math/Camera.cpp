#include "pch.h"
#include "Camera.hpp"
#include "Window.hpp"

namespace ArSDL {
    FRect Camera::GetRect(Window const& window) const
    {
        auto const windowRect{window.GetSize()};
        auto const w{windowRect.width  / m_Scale};
        auto const h{windowRect.height / m_Scale};
        auto [x, y] {m_Translation};
        return {x - 0.5f * w, y - 0.5f * h, w, h};
    }

	FPoint Camera::operator()(FPoint const& vec) const
	{
		return (vec - m_Translation) * m_Scale + m_pWin->GetCenter();
	}

	FPoint Camera::Trans(FPoint const& vec) const
	{
		return operator()(vec);
	}

	FPoint Camera::operator[](FPoint const& vec) const
	{
		return (vec - m_pWin->GetCenter()) / m_Scale + m_Translation;
	}

	FPoint Camera::RevTrans(FPoint const& vec) const
	{
		return operator[](vec);
	}
}
