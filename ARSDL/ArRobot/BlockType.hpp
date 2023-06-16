#pragma once

namespace ArRobot {
	enum class BlockType
	{
		Nothing = 0,
		Wall,
		Pit,
		Item,
	};

	namespace BlockTypeEnum {
		constexpr std::string_view ToString(BlockType block)
		{
			using enum BlockType;
			switch (block)
			{
			case Nothing: return "Nothing";
			case Wall:    return "Wall";
			case Pit:     return "Pit";
			case Item:    return "Item";
			}

			return "Invalid BlockType";
		}
	}
}

namespace std {
	template <>
	struct formatter<ArRobot::BlockType> : formatter<string_view>
	{
		auto format(ArRobot::BlockType block, std::format_context context) const
		{
			auto fmt = std::formatter<string_view>{};
			return fmt.format(ArRobot::BlockTypeEnum::ToString(block), context);
		}
	};
}