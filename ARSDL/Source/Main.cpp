#include "pch.h"
#include "Engine.hpp"
#include "ArRobot/Robot.hpp"
#include "ArRobot/ArRobotException.hpp"
#include "Math/Camera.hpp"
#include "Math/CameraDragger.hpp"
#include "Math/CameraWheelScalar.hpp"

namespace sdl  = ArSDL;
namespace robo = ArRobot;

class MyGame : public sdl::Engine 
{
private:
	static constexpr auto CamSpeed{300.0f};

public:
	using Engine::Engine;

	void OnSetup() override 
	{
		pCam0 = std::make_unique<sdl::Camera>(GetWindow());
		pCam1 = std::make_unique<sdl::Camera>(GetWindow());

		pCam0->SetScale(2.0f);
		pCam1->TranslateBy({-100.0f, -100.0f});
	}

	void OnUpdate([[maybe_unused]] float dt) override 
	{
		[[maybe_unused]]
		auto& gfx{GetRenderer()};

		// camDrag.UpdateFromScreenPointOfView(*pCam0, mouse);
		pCam0->SetTranslation(mouse.pos);
		camScal.Update(*pCam0, *this);

		gfx.Clear(sdl::Colors::DarkMagenta);

		gfx.SetBlendMode(sdl::BlendMode::Normal);
		gfx.FillCircle(pCam0->Trans({300.0f, 200.0f}), 100.f * pCam0->GetScale(), 0x22FF0000);
		gfx.FillCircle({300.0f, 200.0f}, 100.f, 0xFFFF0000);
		gfx.SetBlendMode(sdl::BlendMode::Off);

		DrawCamRect(*pCam0);
	}

	void DrawCamRect(sdl::Camera const& cam)
	{
		auto const camRect{cam.GetRect(GetWindow())};
		GetRenderer().DrawRect(camRect, sdl::Colors::PaleVioletRed, 3.0f);
	}

private:
	robo::Robot robot{};

	std::unique_ptr<sdl::Camera> pCam0{};
	std::unique_ptr<sdl::Camera> pCam1{};
	sdl::CameraDragger camDrag{};
	sdl::CameraWheelScalar camScal{};
};

int main(int, char**)
{
	MyGame game{"Tiddies", 1080, 720};
	game.Run();
	return 0;
}