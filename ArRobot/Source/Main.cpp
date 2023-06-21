#include "pch.hpp"
#include "ArRobotCore.hpp"
#include "Robot.hpp"
#include "ArRobotException.hpp"
#include "PlayField.hpp"

namespace arge = Arge;

namespace ArRobot {
	class MyGame : public arge::Engine
	{
	private:
		static constexpr auto CamSpeed {300.0f};

	public:
		using Engine::Engine;

		void OnSetup() override
		{
			pCam0 = std::make_unique<arge::Camera>(GetWindow());
			pCam0->TranslateBy(GetWindow().GetCenter());

			playField.SetTickMilliseconds(100.0f);
			playField.ForEachRobot([](auto& robot) {
				robot.ToggleDebugPrinting(true);
				robot.ToggleDebugVisuals(true);
			});
			playField.GetGrid().At(5, 0) = BlockType::Wall;
			playField.GetGrid().At(7, 1) = BlockType::Wall;
			playField.GetGrid().At(7, 2) = BlockType::Wall;
			playField.GetGrid().At(9, 3) = BlockType::Wall;
			playField.GetGrid().At(8, 4) = BlockType::Wall;

			std::vector<Command> const commands{
				Command::MakeMarkLabel("Start"),
				Command::MakeMove(1, 0),
				Command::MakeMarkLabel("RightWallCheck"),
				Command::MakeCheckDir(1, 0, BlockType::Wall),
				Command::MakeJumpFalse("Start"),
				Command::MakeCheckDir(0, 1, BlockType::Wall),
				Command::MakeJumpFalse("NoWall"),
				Command::MakeHalt(),
				Command::MakeMarkLabel("NoWall"),
				Command::MakeMove(0, 1),
				Command::MakeJump("RightWallCheck"),
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
			auto& gfx {GetRenderer()};
			gfx.Clear(arge::Colors::DarkMagenta);
			playField.Draw(gfx, *pCam0);
		}

		void DrawCamRect(arge::Camera const& cam)
		{
			auto const camRect {cam.GetRect(GetWindow())};
			GetRenderer().DrawRect(camRect, arge::Colors::PaleVioletRed, 3.0f);
		}

	private:
		std::unique_ptr<arge::Camera> pCam0 {};
		arge::CameraDragger camDragger{};
		arge::CameraWheelScalar camScaler{};
		PlayField playField{};
	};
}

int SDL_main(int, char**)
{
	ArRobot::MyGame game{"Tiddies", 1080, 720};
	try 
	{ 
		game.Run(); 
	}
	catch (std::exception const& err)
	{
		std::cout << "Standard exception: " << err.what();
	}
	catch (ArRobot::ArRobotException const& err)
	{
		std::cout << err.GetMessage() << '\n';
	}
	return 0;
}

#undef main
extern "C" int main(int argc, char** argv)
{
	SDL_main(argc, argv);
}