#include "pch.h"
#include "CameraDragger.hpp"
#include "Mouse.hpp"

namespace ArSDL {
	namespace {
		auto const sc_DragButt{&Mouse::left};
	}

	void CameraDragger::Update(Camera& camera, Mouse const& mouse)
	{
		auto const& dragButt{mouse.*sc_DragButt};

		if (m_bMouseLanded)
		{
			m_bMouseLanded = dragButt.IsDown();
			camera.SetTranslation(m_TransCache + (m_MousePosCache - mouse.pos) / camera.GetScale());
		}
		else
		{
			m_bMouseLanded = dragButt.IsPressed();
			
			if (m_bMouseLanded)
			{
				m_MousePosCache = mouse.pos;
				m_TransCache = camera.GetTranslation();
			}
		}
	}

	void CameraDragger::UpdateFromScreenPointOfView(Camera& camera, Mouse const& mouse)
	{
		auto const& dragButt{mouse.*sc_DragButt};

		if (m_bMouseLanded)
		{
			m_bMouseLanded = dragButt.IsDown();
			camera.SetTranslation(m_TransCache + (mouse.pos - m_MousePosCache));
		}
		else
		{
			m_bMouseLanded = dragButt.IsPressed();

			if (m_bMouseLanded)
			{
				m_MousePosCache = mouse.pos;
				m_TransCache = camera.GetTranslation();
			}
		}
	}
}
