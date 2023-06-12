#pragma once
#include "STD.hpp"
#include "Util/Grid.hpp"
#include "Util/Camera.hpp"
#include "Robot.hpp"
#include "BlockType.hpp"

namespace ArRobot {
	class PlayField
	{
	public:
		PlayField()
		{
			m_Robots.emplace_back();
		}

		void AddCommand(Command const& newCommand);
		void Update(float dt);
		void RunCode();
		void Draw(Arge::Renderer& gfx, Arge::Camera const& camera);
		void DrawGrid(Arge::Renderer& gfx, Arge::Camera const& camera);

		constexpr float GetTickMilliseconds() const
		{
			return m_TickMilliseconds.count();
		}

		constexpr std::chrono::duration<float, std::milli> GetTickTime() const
		{
			return m_TickMilliseconds;
		}

		constexpr void SetTickMilliseconds(float howManySeconds)
		{
			using namespace std::chrono_literals;
			SetTickMilliseconds(howManySeconds * 1ms);
		}

		constexpr void SetTickMilliseconds(std::chrono::duration<float, std::milli> howManySeconds)
		{
			m_TickMilliseconds = howManySeconds;
		}

		constexpr Robot& GetRobot(std::size_t index)
		{
			return m_Robots[index];
		}

	private:
		std::chrono::duration<float, std::milli> m_TickAcc{};
		std::chrono::duration<float, std::milli> m_TickMilliseconds{1000.0f};

		std::vector<Robot> m_Robots{};
		Arge::Grid<BlockType> m_Grid{20, 20, 50.0f, 50.0f};
	};
}