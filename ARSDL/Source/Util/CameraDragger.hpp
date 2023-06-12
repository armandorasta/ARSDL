#pragma once
#include "STD.hpp"
#include "Camera.hpp"

namespace Arge {
	class Mouse;

	class CameraDragger
	{
	public:
		CameraDragger()                                = default;
		CameraDragger(CameraDragger const&)            = default;
		CameraDragger(CameraDragger&&)                 = default;
		CameraDragger& operator=(CameraDragger const&) = default;
		CameraDragger& operator=(CameraDragger&&)      = default;

		/* Only one of these two may be used per dragger */

		// Will update the camera from its own point of view, meaning it's being used
		// while being dragged.
		void Update(Camera& camera, Mouse const& mouse);

		// Will allow for dragging the camera around the window, meaning it's not being
		// used while being dragged.
		void UpdateFromScreenPointOfView(Camera& camera, Mouse const& mouse);

		constexpr bool IsDragging() const
		{
			return m_bMouseLanded;
		}

		// Will be in screen-space.
		constexpr Vec2 const& GetCamDragBeginPos() const
		{
			return m_TransCache;
		}

		// Will be in screen-space.
		constexpr Vec2 const& GetMouseDragBeginPos() const
		{
			return m_MousePosCache;
		}

		// Use this before switching cameras or dragging functions.
		constexpr void Reset()
		{
			operator=({});
		}

	private:
		bool m_bMouseLanded{};
		Vec2 m_TransCache{};
		Vec2 m_MousePosCache{};
	};
}