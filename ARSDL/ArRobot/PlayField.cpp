#include "pch.h"
#include "PlayField.hpp"

namespace ArRobot {
	void PlayField::AddCommand(Command const& newCommand)
	{
		for (auto& robot : m_Robots)
		{
			robot.AddCommand(newCommand);
		}
	}

	void PlayField::Update(float dt)
	{
		if (m_TickAcc < m_TickMilliseconds)
		{
			using namespace std::chrono_literals;
			m_TickAcc += 1s * dt;
		}
		else while (m_TickAcc > m_TickMilliseconds) 
			// This loop is necessary because of potential lag spikes.
		{
			std::cout << "TICK\n";
			m_TickAcc -= m_TickMilliseconds;
			RunCode();
		}
	}

	void PlayField::RunCode()
	{
		for (auto& robot : m_Robots)
		{
			robot.ExecuteOneCommand();
		}
	}

	void PlayField::Draw(Arge::Renderer& gfx, Arge::Camera const& camera)
	{
		DrawGrid(gfx, camera);

		for (auto const& robot : m_Robots)
		{
			robot.Draw(gfx, camera, m_Grid);
		}
	}

	void PlayField::DrawGrid(Arge::Renderer& gfx, Arge::Camera const& camera)
	{
		auto const cellSize{m_Grid.GetCellWidth()};
		for (size_t i{}; i < m_Grid.GetWidth(); ++i)
		{
			for (size_t j{}; j < m_Grid.GetHeight(); ++j)
			{
				camera.FillRect(gfx,
					Arge::Vec2{static_cast<float>(i), static_cast<float>(j)} *cellSize,
					cellSize, cellSize,
					m_Grid.At(i, j) == BlockType::Item ? Arge::Colors::Green : Arge::Colors::Red
				);

				camera.DrawRect(gfx,
					Arge::Vec2{static_cast<float>(i), static_cast<float>(j)} *cellSize,
					cellSize, cellSize, 0, 2.0f
				);
			}
		}
	}
}