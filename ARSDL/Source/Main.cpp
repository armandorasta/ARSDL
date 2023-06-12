#include "pch.h"
#include "Engine.hpp"
#include "ArRobot/Robot.hpp"
#include "ArRobot/ArRobotException.hpp"
#include "Util/Camera.hpp"
#include "Util/CameraDragger.hpp"
#include "Util/CameraWheelScalar.hpp"
#include "ArRobot/PlayField.hpp"

namespace arge = Arge;
namespace robo = ArRobot;

class MyGame : public arge::Engine 
{
private:
	static constexpr auto CamSpeed{300.0f};

public:
	using Engine::Engine;

	void OnSetup() override 
	{
		pCam0 = std::make_unique<arge::Camera>(GetWindow());
		pCam0->TranslateBy(GetWindow().GetCenter());

		playField.SetTickMilliseconds(300.0f);
		playField.GetRobot(0).ToggleDebugPrint(true);
		auto const lastAddr{playField.GetRobot(0).MemorySize() - 1};
		playField.AddCommand(robo::Command::MakeMemSet(lastAddr, 5));
		playField.AddCommand(robo::Command::MakeMove(1, 0));
		playField.AddCommand(robo::Command::MakeIncrement(lastAddr, -1));
		playField.AddCommand(robo::Command::MakeMemPrintAll());
		playField.AddCommand(robo::Command::MakeJumpTrue(1));
	}

	void OnUpdate([[maybe_unused]] float dt) override 
	{
		camDragger.Update(*pCam0, mouse);
		camScaler.Update(*pCam0, *this);

		playField.Update(dt);

		Draw();
	}

	void Draw()
	{
		[[maybe_unused]]
		auto& gfx{GetRenderer()};
		gfx.Clear(arge::Colors::DarkMagenta);
		playField.Draw(gfx, *pCam0);
	}

	void DrawCamRect(arge::Camera const& cam)
	{
		auto const camRect{cam.GetRect(GetWindow())};
		GetRenderer().DrawRect(camRect, arge::Colors::PaleVioletRed, 3.0f);
	}

private:
	std::unique_ptr<arge::Camera> pCam0{};
	arge::CameraDragger camDragger{};
	arge::CameraWheelScalar camScaler{};
	robo::PlayField playField{};
};

int main(int, char**)
{
	MyGame game{"Tiddies", 1080, 720};
	game.Run();
	return 0;
}