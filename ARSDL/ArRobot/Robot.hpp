#pragma once 
#include "Arge.hpp"

#include "Command.hpp"
#include "ArRobotException.hpp"
#include "BlockType.hpp"

namespace ArRobot {
	class Robot
	{
	private:
		struct MyStringHasher
		{
			using is_transparent = void;
			auto operator()(std::string_view str) const 
			{
				return std::hash<std::string_view>{}(str);
			}
		};

	public:
		explicit Robot(Arge::Grid<BlockType>& parentGrid);
		explicit Robot(Arge::Grid<BlockType>& parentGrid, std::int32_t x, std::int32_t y);

	private:
		void Execute(Command const& cmd);
		void HandleCall(Command const& cmd);
		void HandleReturn();
		void HandleMove(Command const& cmd);
		void HandleCheckDir(Command const& cmd);
		void HandleBinaryOp(Command const& cmd);
		void HandleMemPrintAll() const;
		void HandleJump(bool cond, std::string_view label);

	public:
		void AddCommand(Command const& newCommand);
		void Tick();

		constexpr void SetPosition(std::int32_t x, std::int32_t y)
		{
			m_X = x; 
			m_Y = y;
		}

		void Draw(Arge::Renderer& renny, Arge::Camera const& camera) const;

		[[nodiscard]]
		constexpr bool IsCarryingItem() const
		{
			return m_bItem;
		}

		[[nodiscard]]
		constexpr std::pair<std::int32_t, std::int32_t> GetPos() const
		{
			return {m_X, m_Y};
		}

		[[nodiscard]]
		constexpr std::size_t MemorySize() const
		{
			return m_GlobalMemory.size();
		}

		[[nodiscard]]
		constexpr std::int32_t Deref(std::size_t addr) const
		{
			return m_GlobalMemory[addr];
		}

		[[nodiscard]]
		constexpr bool IsDebugPrintingEnabled() const
		{
			return bDebugPrint;
		}

		constexpr void ToggleDebugPrinting(bool newValue)
		{
			bDebugPrint = newValue;
		}

		[[nodiscard]]
		constexpr bool IsDebugVisualsEnabled() const
		{
			return bDebugVisuals;
		}

		constexpr void ToggleDebugVisuals(bool newValue)
		{
			bDebugVisuals = newValue;
		}

	private:
		Arge::Grid<BlockType>& m_ParentGrid;

		// Switching the hasher and the equal_to operator will give you a cryptic error message,
		// and will not be caught by intellisense.
		std::unordered_map<
			std::string, std::size_t, MyStringHasher, std::equal_to<>
		> m_LabelMap{};
		std::vector<Command> m_Commands{};
		std::size_t m_CommandPtr{};
		std::size_t m_Cooldown{};
		bool bDebugPrint{};
		bool bDebugVisuals{};

		// Brace initialization does not work here.
		std::vector<std::int32_t> m_GlobalMemory = std::vector<std::int32_t>(16);

		std::int32_t m_X{};
		std::int32_t m_Y{};
		bool m_bItem{};
	};
}