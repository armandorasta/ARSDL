#include "pch.hpp"
#include "PlayField.hpp"

namespace ArRobot {
	void PlayField::AddCommand(Command const& newCommand)
	{
		ForEachRobot([&newCommand](auto& r) { r.AddCommand(newCommand); });
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
			ForEachRobot([](auto& r) { r.Tick(); });
		}
	}

	void PlayField::Draw(Arge::Renderer& gfx, Arge::Camera const& camera)
	{
		DrawGrid(gfx, camera);
		ForEachRobot([&gfx, &camera](auto const& r) { r.Draw(gfx, camera); });
	}

	void PlayField::DrawGrid(Arge::Renderer& gfx, Arge::Camera const& camera)
	{
		auto const cellSize{m_Grid.GetCellWidth()};
		for (size_t i{}; i < m_Grid.GetWidth(); ++i)
		{
			for (size_t j{}; j < m_Grid.GetHeight(); ++j)
			{
				camera.FillRect(gfx,
					Arge::Vec2{static_cast<float>(i), static_cast<float>(j)} * cellSize,
					cellSize, cellSize,
					GetBlockColor(m_Grid.At(i, j))
				);

				camera.DrawRect(gfx,
					Arge::Vec2{static_cast<float>(i), static_cast<float>(j)} * cellSize,
					cellSize, cellSize, 0, 2.0f
				);
			}
		}
	}

	Arge::Color PlayField::GetBlockColor(BlockType block) const
	{
		using enum BlockType;
		switch (block)
		{
		case Nothing: return Arge::Colors::Gray;
		case Item:    return Arge::Colors::Pink;
		case Wall:    return Arge::Colors::White;
		case Pit:     return Arge::Colors::Red;
		}

		throw GenericError{"Invalid BlockType: {}", static_cast<std::int32_t>(block)};
	}
}