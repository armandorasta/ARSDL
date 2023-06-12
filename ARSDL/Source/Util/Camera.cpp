#include "pch.h"
#include "Camera.hpp"
#include "Window.hpp"

namespace Arge {
    RectF Camera::GetRect(Window const& window) const
    {
        auto const windowRect{window.GetSize()};
        auto const w{windowRect.width  / m_Scale};
        auto const h{windowRect.height / m_Scale};
        auto [x, y] {m_Translation};
        return {x - 0.5f * w, y - 0.5f * h, w, h};
    }

	Vec2 Camera::operator()(Vec2 const& vec) const
	{
		return (vec - m_Translation) * m_Scale + m_pWin->GetCenter();
	}

	Vec2 Camera::Trans(Vec2 const& vec) const
	{
		return operator()(vec);
	}

	Vec2 Camera::operator[](Vec2 const& vec) const
	{
		return (vec - m_pWin->GetCenter()) / m_Scale + m_Translation;
	}

	Vec2 Camera::RevTrans(Vec2 const& vec) const
	{
		return operator[](vec);
	}
}
