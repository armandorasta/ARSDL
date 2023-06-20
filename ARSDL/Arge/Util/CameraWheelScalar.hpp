#pragma once
#include "ArgeCore.hpp"
#include "Camera.hpp"

namespace Arge {
	class Engine;

	class CameraWheelScalar
	{
	public:
		CameraWheelScalar()                                    = default;
		CameraWheelScalar(CameraWheelScalar const&)            = default;
		CameraWheelScalar(CameraWheelScalar&&)                 = default;
		CameraWheelScalar& operator=(CameraWheelScalar const&) = default;
		CameraWheelScalar& operator=(CameraWheelScalar&&)      = default;

	public:
		void Update(Camera& camera, Engine& window) const;

	public:
		constexpr void SetScaleFactor(float newFactor)
		{
			m_ScaleFactor = newFactor;
		}

		constexpr float GetScaleFactor() const
		{
			return m_ScaleFactor;
		}
		
	private:
		float m_ScaleFactor{1.1f};
	};
}