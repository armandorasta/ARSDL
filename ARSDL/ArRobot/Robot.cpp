#include "pch.h"
#include "Robot.hpp"

namespace ArRobot {
	void Robot::ExecuteOneCommand()
	{
		if constexpr (false && m_Cooldown > 0)
		{
			m_Cooldown -= 1;
		}
		else
		{
			using enum CommandType;

			if (m_CommandPtr >= m_Commands.size())
			{
				return;
			}

			auto const& currCommand{m_Commands[m_CommandPtr]};
			auto const type{currCommand.type};
			Execute(currCommand);

			if (type != Jump && type != JumpTrue && type != JumpFalse)
			{
				m_CommandPtr += 1;
			}
		}
	}

	void Robot::Draw(Arge::Renderer& gfx, Arge::Camera const& camera, Arge::Grid<BlockType> const& grid) const
	{
		auto constexpr Padding{5.0f};
		auto const cellWidth{grid.GetCellWidth()};
		auto const screenPos{
			grid.GridToScreen(static_cast<std::size_t>(m_X), static_cast<std::size_t>(m_Y))
		};

		auto const myWidth{cellWidth - 2 * Padding};
		Arge::RectF const roboRect
		{
			screenPos.x + Padding, 
			screenPos.y + Padding,
			myWidth,
			myWidth,
		};

		camera.FillRect(gfx, roboRect, Arge::Colors::DarkOrange);
		camera.DrawRect(gfx, roboRect, Arge::Colors::Black, Padding * camera.GetScale());

		if (m_bItem)
		{
			Arge::Vec2 const center{screenPos.x + cellWidth * 0.65f, screenPos.y + cellWidth * 0.35f};
			auto const radius{0.25f * myWidth};
			camera.FillCircle(gfx, center, radius, 0xFF00FF00);
			camera.DrawCircle(gfx, center, radius, Arge::Colors::Black, Padding * 0.5f);
		}
	}

	void Robot::Execute(Command const& cmd)
	{
		switch (cmd.type)
		{
			using enum CommandType;
		case Move:
		{
			m_X += cmd.data[0];
			m_Y += cmd.data[1];

			if (bDebugPrint) std::cout << cmd;
			break;
		}
		case PickUp:
		{
			if (m_bItem)
			{
				throw GenericError{"Already holding something"};
			}

			m_bItem = true;

			if (bDebugPrint) std::cout << cmd;
			break;
		}
		case Drop:
			if (!m_bItem)
			{
				throw GenericError{"Dropping nothing"};
			}

			m_bItem = false;

			if (bDebugPrint) std::cout << cmd;
			break;
		case Jump:
			m_CommandPtr = cmd.data[0];

			if (bDebugPrint) std::cout << cmd;
			break;
		case JumpTrue:
			if (m_MemSlots.back())
				m_CommandPtr = cmd.data[0];

			if (bDebugPrint) std::cout << cmd;
			break;
		case JumpFalse:
			if (!m_MemSlots.back())
				m_CommandPtr = cmd.data[0];

			if (bDebugPrint) std::cout << cmd;
			break;
		case MemSet:    
			m_MemSlots[cmd.data[0]]  = cmd.data[1]; 
			if (bDebugPrint) std::cout << cmd;
			break;

		case Increment: 
			m_MemSlots[cmd.data[0]] += cmd.data[1]; 
			if (bDebugPrint) std::cout << cmd;
			break;
		case MemCopy:   
			m_MemSlots[cmd.data[0]]  = m_MemSlots[cmd.data[1]]; 
			if (bDebugPrint) std::cout << cmd;
			break;

		case BinaryOp:
		{
			auto const [opCode, lhsAddr, rhsAddr] = cmd.data;
			auto& lhs{m_MemSlots[lhsAddr]};
			auto  rhs{m_MemSlots[rhsAddr]};
			lhs = OpCodeEval(static_cast<OpCode>(opCode), lhs, rhs);

			if (bDebugPrint) std::cout << cmd;
			break;
		}
		case MemPrint:
		{
			auto const addr{cmd.data[0]};
			std::cout << std::format("mem[{}] = {}\n", addr, m_MemSlots[addr]);
			break;
		}
		case MemPrintAll:
		{
			// Top line
			std::cout << std::string(m_MemSlots.size() * 4 + 1, '=') << "\n|";

			// Memory indices...
			for (std::size_t i{}; i < m_MemSlots.size(); ++i)
			{
				std::cout << std::format("{:^3}|", i);
			}

			// Middle line
			std::cout << '\n' << std::string(m_MemSlots.size() * 4 + 1, '-') << "\n|";

			// Memory values...
			for (auto const mem : m_MemSlots)
			{
				std::cout << std::format("{:<3}|", mem);
			}

			// Bottom line
			std::cout << '\n' << std::string(m_MemSlots.size() * 4 + 1, '=') << "\n";
			break;
		}
		default:
			throw GenericError{"Executing invalid Command: {}", cmd};
		}

		m_Cooldown = cmd.ticks;
	}
}
