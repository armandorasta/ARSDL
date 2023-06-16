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

		playField.SetTickMilliseconds(50.0f);
		playField.ForEachRobot([](auto& robot) 
		{ 
			robot.ToggleDebugPrinting(true); 
			robot.ToggleDebugVisuals(true);
		});
		playField.GetGrid().At(5, 0) = robo::BlockType::Wall;
		playField.GetGrid().At(7, 1) = robo::BlockType::Wall;
		playField.GetGrid().At(7, 2) = robo::BlockType::Wall;
		playField.GetGrid().At(9, 3) = robo::BlockType::Wall;
		playField.GetGrid().At(8, 4) = robo::BlockType::Wall;

		std::vector<robo::Command> const commands{
			robo::Command::MakeMarkLabel("Start"),
			robo::Command::MakeMove(1, 0),
			robo::Command::MakeMarkLabel("RightWallCheck"),
			robo::Command::MakeCheckDir(1, 0, robo::BlockType::Wall),
			robo::Command::MakeJumpFalse("Start"),
			robo::Command::MakeCheckDir(0, 1, robo::BlockType::Wall),
			robo::Command::MakeJumpFalse("NoWall"),
			robo::Command::MakeHalt(),
			robo::Command::MakeMarkLabel("NoWall"),
			robo::Command::MakeMove(0, 1),
			robo::Command::MakeJump("RightWallCheck"),
		};

		for (auto const& cmd : commands)
		{
			playField.AddCommand(cmd);
		}
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
	try 
	{ 
		game.Run(); 
	}
	catch (std::exception const& err)
	{
		std::cout << "Standard exception: " << err.what();
	}
	catch (robo::ArRobotException const& err)
	{
		std::cout << err.GetMessage() << '\n';
	}
	return 0;
}