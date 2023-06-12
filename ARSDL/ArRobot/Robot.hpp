#pragma once 
#include "STD.hpp"
#include "Engine.hpp"
#include "Util/Grid.hpp"
#include "Util/Camera.hpp"

#include "Command.hpp"
#include "ArRobotException.hpp"
#include "BlockType.hpp"

namespace ArRobot {
	class Robot
	{
	public:
		constexpr Robot() : Robot{0, 0} {};
		constexpr Robot(std::int32_t x, std::int32_t y) : m_X{x}, m_Y{y}
		{
		}

		constexpr void AddCommand(Command const& newCommand)
		{
			m_Commands.push_back(newCommand);
		}

		void ExecuteOneCommand();

		constexpr void SetPosition(std::int32_t x, std::int32_t y)
		{
			m_X = x;
			m_Y = y;
		}

		void Draw(Arge::Renderer& renny, Arge::Camera const& camera, Arge::Grid<BlockType> const& grid) const;

		constexpr bool IsCarryingItem() const
		{
			return m_bItem;
		}

		constexpr std::pair<std::int32_t, std::int32_t> GetPos() const
		{
			return {m_X, m_Y};
		}

		constexpr std::size_t MemorySize() const
		{
			return m_MemSlots.size();
		}

		constexpr std::int32_t Deref(std::size_t addr) const
		{
			return m_MemSlots[addr];
		}

		constexpr bool IsDebugPrintingEnabled() const
		{
			return bDebugPrint;
		}

		constexpr void ToggleDebugPrint(bool newValue)
		{
			bDebugPrint = newValue;
		}

	private:
		void Execute(Command const& cmd);

	private:
		std::vector<Command> m_Commands{};
		std::size_t m_CommandPtr{};
		std::size_t m_Cooldown{};
		bool bDebugPrint{};

		std::array<std::int32_t, 4> m_MemSlots{};
		std::int32_t m_X{};
		std::int32_t m_Y{};
		bool m_bItem{};
	};
}