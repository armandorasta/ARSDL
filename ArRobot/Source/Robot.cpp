#include "pch.hpp"
#include "Robot.hpp"

namespace ArRobot {
	Robot::Robot(Arge::Grid<BlockType>& parentGrid) : Robot{parentGrid, 0, 0} {};
	Robot::Robot(Arge::Grid<BlockType>& parentGrid, std::int32_t x, std::int32_t y)
		: m_ParentGrid{parentGrid}, m_X{x}, m_Y{y}
	{
		if (!m_Commands.empty())
		{
			// So the first command does not execute instantly.
			m_Cooldown = m_Commands.front().GetTickCount();
		}
	}

	void Robot::AddCommand(Command const& newCommand)
	{
		if (newCommand.GetType() == CommandType::MarkLabel)
		{
			m_LabelMap.try_emplace(newCommand.As<CommandType::MarkLabel>().label, m_Commands.size());
		}
		else
		{
			m_Commands.push_back(newCommand);
		}
	}

	void Robot::Tick()
	{
		if constexpr (false && m_Cooldown > 0)
		{
			m_Cooldown -= 1;
		}
		else
		{
			if (m_CommandPtr >= m_Commands.size())
			{
				AddCommand(Command::MakeHalt());
				m_CommandPtr = m_Commands.size() - 1;
			}

			auto const& currCommand{m_Commands[m_CommandPtr]};
			auto const type{currCommand.GetType()};
			Execute(currCommand);

			using enum CommandType;
			if (type != Halt && 
				type != Jump && type != JumpTrue && type != JumpFalse)
			{
				m_CommandPtr += 1;
			}
			
			m_Cooldown = m_Commands[m_CommandPtr].GetTickCount();
		}
	}

	void Robot::Draw(Arge::Renderer& gfx, Arge::Camera const& camera) const
	{
		auto constexpr Padding{5.0f};
		auto const scaledPadding{Padding * camera.GetScale()};
		auto const cellWidth{m_ParentGrid.GetCellWidth()};
		auto const screenPos{
			m_ParentGrid.GridToScreen(static_cast<std::size_t>(m_X), static_cast<std::size_t>(m_Y))
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
		camera.DrawRect(gfx, roboRect, Arge::Colors::Black, scaledPadding);

		if (m_bItem)
		{
			Arge::Vec2 const center{screenPos.x + cellWidth * 0.65f, screenPos.y + cellWidth * 0.35f};
			auto const radius{0.25f * myWidth};
			camera.FillCircle(gfx, center, radius, 0xFF00FF00);
			camera.DrawCircle(gfx, center, radius, Arge::Colors::Black, Padding * 0.5f);
		}

		if (bDebugVisuals)
		{
			// Can't extract it out because it depends on a lot of variables in the current scope.
			using enum CommandType;
			switch (auto const& currCommand{m_Commands[m_CommandPtr]}; currCommand.GetType())
			{
			case Move:
			{
				auto const& data{currCommand.As<Move>()};
				Arge::Vec2 const centerOffset{cellWidth * 0.5f, cellWidth * 0.5f};
				auto const screenDelta{m_ParentGrid.GridToScreen(data.x, data.y)};
				auto const targetPos{screenPos + screenDelta + centerOffset};
				camera.DrawLine(gfx, screenPos + centerOffset, targetPos, Arge::Colors::DarkGreen,
					0.5f * scaledPadding);
				camera.FillPolygon(gfx, targetPos, cellWidth * 0.25f, 3, Arge::Colors::DarkGreen,
					std::atan2(screenDelta.y, screenDelta.x));
				break;
			}
			case CheckDir:
			{
				auto const& data{currCommand.As<CheckDir>()};
				Arge::Vec2 const centerOffset{cellWidth * 0.5f, cellWidth * 0.5f};
				auto const screenDelta{m_ParentGrid.GridToScreen(data.x, data.y)};
				auto const targetPos{screenPos + screenDelta + centerOffset};
				camera.DrawCircle(
					gfx, targetPos, cellWidth * 0.3f, Arge::Colors::PaleVioletRed, 0.5f * scaledPadding);
				break;
			}
			default:
				break;
			}
		}
	}

	void Robot::Execute(Command const& cmd)
	{
		// None of the branches is allowed to return early!!! There is code under the 
		// switch that needs to be executed for all commands.
		switch (cmd.GetType()) 
		{ 
			using enum CommandType;
		case DoNothing: 
			/* Sleep a little. */ 
			break;
		case Move:      
			HandleMove(cmd); 
			break;
		case PickUp:
			if (m_bItem)
				throw GenericError{"Already holding something"};
			m_bItem = true;
			break;
		case Drop:
			if (!m_bItem)
				throw GenericError{"Dropping nothing"};
			m_bItem = false;
			break;
		case CheckDir:  
			HandleCheckDir(cmd); 
			break;
		case MarkLabel: 
			throw GenericError{"MarkLabel may not be executed"};
		case Jump:      
			HandleJump(true, cmd.As<Jump>().label); 
			break;
		case JumpTrue:  
			HandleJump(m_GlobalMemory.back() != 0, cmd.As<JumpTrue>().label); 
			break;
		case JumpFalse: 
			HandleJump(m_GlobalMemory.back() == 0, cmd.As<JumpFalse>().label); 
			break;
		case Halt:      
			/* Stop. */ 
			break;
		case MemSet:    
		{
			auto const& [addr, value] {cmd.As<MemSet>()};
			m_GlobalMemory[addr] = value; 
			break;
		}
		case MemCopy:   
		{
			auto const& [to, from] {cmd.As<MemCopy>()};
			m_GlobalMemory[to] = m_GlobalMemory[from];
			break;
		}
		case BinaryOp: HandleBinaryOp(cmd); break;
		case MemPrint:
		{
			auto const& [addr] {cmd.As<MemPrint>()};
			std::cout << std::format("mem[{}] = {}\n", addr, m_GlobalMemory[addr]);
			break;
		}
		case MemPrintAll: 
			HandleMemPrintAll(); 
			break;
		default: 
			throw GenericError{"Executing invalid Command: {}", cmd};
		}

		if (bDebugPrint)
		{
			std::cout << cmd;
		}
	}

	void Robot::HandleMove(Command const& cmd)
	{
		auto const& [x, y] {cmd.As<CommandType::Move>()};
		m_X += x;
		m_Y += y;

		if (auto const gridWidth{m_ParentGrid.GetWidth()}; m_X < 0 || gridWidth <= m_X)
		{
			throw GenericError{"Moved out of the grid! (x is {} but width is {})", m_X, gridWidth};
		}

		// No else if, because I don't want gridWidth to be in scope.
		if (auto const gridHeight{m_ParentGrid.GetHeight()};  m_Y < 0 || gridHeight <= m_Y)
		{
			throw GenericError{"Moved out of the grid! (y is {} but height is {})", m_Y, gridHeight};
		}
	}

	void Robot::HandleCheckDir(Command const& cmd)
	{
		auto const& [x, y, block] {cmd.As<CommandType::CheckDir>()};
		auto const targetX{m_X + x};
		auto const targetY{m_Y + y};
		auto const adjacentBlock = m_ParentGrid.IsInBounds(targetX, targetY) ?
			m_ParentGrid.At(targetX, targetY) : BlockType::Wall;
		m_GlobalMemory.back() = (adjacentBlock == block);
	}

	void Robot::HandleBinaryOp(Command const& cmd)
	{
		auto const& [opCode, lhs, rhs] {cmd.As<CommandType::BinaryOp>()};
		m_GlobalMemory[lhs] = OpCodeEnum::Eval(opCode, m_GlobalMemory[lhs], m_GlobalMemory[rhs]);
	}

	void Robot::HandleMemPrintAll() const 
	{
		// Top line
		std::cout << std::string(m_GlobalMemory.size() * 4 + 1, '-') << "\n|";

		// Memory indices...
		for (std::size_t i{}; i < m_GlobalMemory.size(); ++i)
		{
			std::cout << std::format("{:^3}|", i);
		}

		// Middle line
		std::cout << '\n' << std::string(m_GlobalMemory.size() * 4 + 1, '-') << "\n|";

		// Memory values...
		for (auto const mem : m_GlobalMemory)
		{
			std::cout << std::format("{:<3}|", mem);
		}

		// Bottom line
		std::cout << '\n' << std::string(m_GlobalMemory.size() * 4 + 1, '-') << "\n";
	}

	void Robot::HandleJump(bool cond, std::string_view label)
	{
		if (cond)
		{
			if (auto const it{m_LabelMap.find(label)}; it != m_LabelMap.end())
			{
				m_CommandPtr = it->second;
			}
			else
			{
				throw GenericError{"Jumping to non-existant label: {}", label};
			}
		}
		else
		{
			m_CommandPtr += 1;
		}
	}
}
