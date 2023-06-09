#include "pch.h"
#include "Engine.hpp"
#include "ArRobot/Robot.hpp"
#include "ArRobot/ArRobotException.hpp"
#include "Util/Camera.hpp"
#include "Util/CameraDragger.hpp"
#include "Util/CameraWheelScalar.hpp"
#include "Util/Grid.hpp"

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

		grid.At(5, 2) = 50;
	}

	void OnUpdate([[maybe_unused]] float dt) override 
	{
		camDrag.Update(*pCam0, mouse);
		camScal.Update(*pCam0, *this);
		if (mouse.right.IsPressed())
		{
			auto const [col, row] {grid.ScreenToGrid(pCam0->RevTrans(mouse.pos))};
			auto& el{grid.At(col, row)};

			el ? (el = 0) : (el = 50);
		}

		Draw();
	}

	void Draw()
	{
		[[maybe_unused]]
		auto& gfx{GetRenderer()};
		gfx.Clear(sdl::Colors::DarkMagenta);

		DrawGrid();
	}

	void DrawGrid()
	{
		auto const cellSize{grid.GetCellWidth()};

		for (size_t i{}; i < grid.GetWidth(); ++i)
		{
			for (size_t j{}; j < grid.GetHeight(); ++j)
			{
				pCam0->FillRect(GetRenderer(),
					sdl::FPoint{static_cast<float>(i), static_cast<float>(j)} * cellSize,
					cellSize, cellSize, 
					grid.At(i, j) ? sdl::Colors::Green : sdl::Colors::Red
				);

				pCam0->DrawRect(GetRenderer(),
					sdl::FPoint{static_cast<float>(i), static_cast<float>(j)} * cellSize,
					cellSize, cellSize, 0, 2.0f
				);
			}
		}
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

	sdl::Grid<int, 20, 20> grid{100.0f, 100.0f};
};

int main(int, char**)
{
	MyGame game{"Tiddies", 1080, 720};
	game.Run();
	return 0;
}