#pragma once
#include "ArRobotCore.hpp"
#include "Robot.hpp"
#include "BlockType.hpp"

#include <Arge/Arge.hpp>

namespace ArRobot {
	class PlayField
	{
	public:
		PlayField()
		{
			m_Robots.emplace_back(m_Grid);
		}

		void AddCommand(Command const& newCommand);
		void Update(float dt);
		void Draw(Arge::Renderer& gfx, Arge::Camera const& camera);
		void DrawGrid(Arge::Renderer& gfx, Arge::Camera const& camera);
		Arge::Color GetBlockColor(BlockType block) const;

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

		template <std::invocable<Robot&> Callable>
		constexpr void ForEachRobot(Callable const& doWhat)
		{
			for (auto& robot : m_Robots)
			{
				doWhat(robot);
			}
		}

		constexpr Arge::Grid<BlockType>& GetGrid()
		{
			return m_Grid;
		}

	private:
		std::chrono::duration<float, std::milli> m_TickAcc{};
		std::chrono::duration<float, std::milli> m_TickMilliseconds{1000.0f};

		std::vector<Robot> m_Robots{};
		Arge::Grid<BlockType> m_Grid{20, 20, 50.0f, 50.0f};
	};
}