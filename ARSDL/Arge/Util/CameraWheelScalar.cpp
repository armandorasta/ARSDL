#include "pch.h"
#include "CameraWheelScalar.hpp"
#include "Engine.hpp"

namespace Arge {
	void CameraWheelScalar::Update(Camera& camera, Engine& engine) const
	{
		while (auto const ev{engine.PollNextEvent()})
		{
			if (ev->GetType() == EventType::MouseWheel)
			{
				auto const mwEv{ev->AsMouseWheelEvent()};
				auto const del{mwEv.GetVertical()};
				camera.ScaleBy(del > 0 ? m_ScaleFactor : 1.0f / m_ScaleFactor);
			}
			else
			{
				engine.UnhandleLastEvent();
			}
		}
	}
}