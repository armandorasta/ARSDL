#include "ArgeCore.hpp"
#include "Vec2.hpp"
#include "Color.hpp"

#include <SDL/SDL.h>

namespace Arge {
	class Vertex : public SDL_Vertex 
	{
	public:
		constexpr Vertex() : SDL_Vertex{} { }
		constexpr Vertex(Vec2 const& pos) : SDL_Vertex{pos} { }
		constexpr Vertex(Vec2 const& pos, Color color) : SDL_Vertex{pos, color} { }
		constexpr Vertex(Vec2 const& pos, Vec2 const& texCoord) : SDL_Vertex{pos, {}, texCoord} { }
	};

	class ColorVertex : public Vertex
	{
	public:
		constexpr ColorVertex() : Vertex{} { }
		constexpr ColorVertex(Vec2 const& pos, Color color) : Vertex{pos, color} { }
	};

	class TexVertex : public Vertex
	{
	public:
		constexpr TexVertex() : Vertex{} { }
		constexpr TexVertex(Vec2 const& pos, Vec2 const& color) : Vertex{pos, color} { }
	};
}