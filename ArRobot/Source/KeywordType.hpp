#pragma once
#include "ArRobotCore.hpp"
#include "ArRobotException.hpp"

namespace ArRobot {
	enum class KeywordType
	{
		Proc,
		Sticker,
		Enum,
	};

	namespace KeywordTypeEnum {
		using enum KeywordType;
		using MappedType = std::pair<KeywordType, std::string_view>;
		static constexpr std::array<MappedType, 3> gc_KeywordStringMap {{
			{ Proc,    "proc"    },
			{ Sticker, "sticker" },
			{ Enum,    "enum"    },
		}};

		constexpr bool IsKeyword(std::string_view what)
		{
			auto const it {std::ranges::find(gc_KeywordStringMap, what, &MappedType::second)};
			return it != gc_KeywordStringMap.end();
		}

		constexpr std::string_view ToString(KeywordType kwT)
		{
			if (auto const it {std::ranges::find(gc_KeywordStringMap, kwT, &MappedType::first)};
				it != gc_KeywordStringMap.end())
			{
				return it->second;
			}
			else
			{
				return "(Invalid Keyword)";
			}
		}

		constexpr KeywordType FromString(std::string_view glyph)
		{
			AROBOT_DA(IsKeyword(glyph));
			if (auto const it {std::ranges::find(gc_KeywordStringMap, glyph, &MappedType::second)};
				it != gc_KeywordStringMap.end())
			{
				return it->first;
			}
		}
	}
}

namespace std {
	template <>
	struct formatter<ArRobot::KeywordType> : formatter<std::string_view>
	{
		auto format(ArRobot::KeywordType kwT, format_context context) const
		{
			auto const str {ArRobot::KeywordTypeEnum::ToString(kwT)};
			return formatter<std::string_view>{}.format(str, context);
		}
	};
}