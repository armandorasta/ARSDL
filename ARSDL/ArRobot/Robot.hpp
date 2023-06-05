#pragma once 
#include "STD.hpp"
#include "Engine.hpp"

#include "Command.hpp"
#include "ArRobotException.hpp"

namespace ArRobot {
	struct TileSpaceLoc
	{
		std::int32_t x;
		std::int32_t y;
	};

	class Robot
	{
	public:
		constexpr Robot() : Robot{0, 0} {};
		constexpr Robot(std::int32_t x, std::int32_t y) : m_x{x}, m_y{y}
		{
		}


		void AddCommand(Command newCommand)
		{
			m_commands.push_back(newCommand);
		}

		void RunCode()
		{
			if (m_commandPtr >= m_commands.size())
			{
				return;
			}

			auto const type{m_commands[m_commandPtr].type};
			Execute(m_commands[m_commandPtr]);
			if (type != CommandType::Jump && 
				type != CommandType::JumpTrue && 
				type != CommandType::JumpFalse)
			{
				m_commandPtr += 1;
			}
		}

		constexpr void SetPosition(std::int32_t x, std::int32_t y)
		{
			m_x = x;
			m_y = y;
		}

		constexpr void SetScreenPos(ArSDL::FPoint const& where, float tileWidth)
		{
			m_x = static_cast<std::int32_t>(where.x / tileWidth);
			m_y = static_cast<std::int32_t>(where.y / tileWidth);
		}

		void Draw(ArSDL::Renderer& renny, float tileWidth) const
		{
			constexpr auto Padding{5.0f};
			ArSDL::FPoint const screenPos{GetScreenPos(tileWidth)};

			ArSDL::FRect const roboRect{
				screenPos.x + Padding,
				screenPos.y + Padding,
				tileWidth - 2 * Padding,
				tileWidth - 2 * Padding,
			};
			renny.FillRect(roboRect, ArSDL::Colors::DarkOrange);
			renny.DrawRect(roboRect, ArSDL::Colors::Black, Padding);

			if (m_bItem)
			{
				ArSDL::FPoint const center{
					screenPos.x + tileWidth * 0.65f,
					screenPos.y + tileWidth * 0.35f,
				};
				auto const radius{0.25f * (tileWidth - 2 * Padding)};
				renny.FillCircle(center, radius, 0xFF00FF00);
				renny.DrawCircle(center, radius, ArSDL::Colors::Black, Padding * 0.5f);
			}
		}


		constexpr bool IsCarryingItem() const
		{
			return m_bItem;
		}

		constexpr TileSpaceLoc GetLoc() const
		{
			return {m_x, m_y};
		}

		constexpr ArSDL::FPoint GetScreenPos(float tileWidth) const
		{
			return {m_x * tileWidth, m_y * tileWidth};
		}

	private:
		void Execute(Command const& cmd)
		{
			switch (cmd.type)
			{
				using enum CommandType;
			case Move:
				m_x += cmd.data[0];
				m_y += cmd.data[1];

				using namespace std::chrono_literals;
				std::this_thread::sleep_for(0.5s);
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
			case Jump:
				m_commandPtr = cmd.data[0];
				break;
			case JumpTrue:
				if (m_memorySlots.back())
					m_commandPtr = cmd.data[0];
				break;
			case JumpFalse:
				if (!m_memorySlots.back())
					m_commandPtr = cmd.data[0];
				break;
			case MemSet:    m_memorySlots[cmd.data[0]]  = cmd.data[1]; break;
			case Increment: m_memorySlots[cmd.data[0]] += cmd.data[1]; break;
			case MemCopy:   m_memorySlots[cmd.data[0]]  = m_memorySlots[cmd.data[1]]; break;

			case BinaryOp:
			{
				using BinaryOpFunc = std::function<std::int32_t(std::int32_t, std::int32_t)>;
				static std::unordered_map<OpCode, BinaryOpFunc> const sc_Lookup
				{
					{OpCode::Add, [](auto lhs, auto rhs) { return lhs + rhs; }},
					{OpCode::Sub, [](auto lhs, auto rhs) { return lhs - rhs; }},
					{OpCode::Mul, [](auto lhs, auto rhs) { return lhs * rhs; }},
					{OpCode::Div, [](auto lhs, auto rhs) { return lhs / rhs; }},

					{OpCode::Equal,     [](auto lhs, auto rhs) { return lhs == rhs; }},
					{OpCode::NotEqual,  [](auto lhs, auto rhs) { return lhs != rhs; }},
					{OpCode::Greater,   [](auto lhs, auto rhs) { return lhs >  rhs; }},
					{OpCode::GreaterEq, [](auto lhs, auto rhs) { return lhs >= rhs; }},
					{OpCode::Less,      [](auto lhs, auto rhs) { return lhs <  rhs; }},
					{OpCode::LessEq,    [](auto lhs, auto rhs) { return lhs >= rhs; }},
				};

				auto const [opCode, lhsAddr, rhsAddr] = cmd.data;
				if (auto const it{sc_Lookup.find(static_cast<OpCode>(opCode))}; it != sc_Lookup.end())
				{
					auto& lhs{m_memorySlots[lhsAddr]};
					auto  rhs{m_memorySlots[rhsAddr]};
					lhs = it->second(lhs, rhs);
				}
				else
				{
					throw GenericError{"Invalid OpCode: {}", opCode};
				}
				break;
			}
			default:
				throw GenericError{"Executing invalid command"};
			}
		}

	public:
		std::vector<Command> m_commands{};
		std::size_t m_commandPtr{};

		std::array<std::int32_t, 4> m_memorySlots{};

		std::int32_t m_x{};
		std::int32_t m_y{};
		bool m_bItem{};
	};
}