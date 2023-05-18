#include "STD.hpp"
#include "FPoint.hpp"
#include "Color.hpp"

#include <SDL.h>

namespace ArSDL {
	class Vertex : public SDL_Vertex 
	{
	public:
		constexpr Vertex() : SDL_Vertex{} { }
		constexpr Vertex(FPoint const& pos) : SDL_Vertex{pos} { }
		constexpr Vertex(FPoint const& pos, Color color) : SDL_Vertex{pos, color} { }
		constexpr Vertex(FPoint const& pos, FPoint const& texCoord) : SDL_Vertex{pos, {}, texCoord} { }
	};

	class ColorVertex : public Vertex
	{
	public:
		constexpr ColorVertex() : Vertex{} { }
		constexpr ColorVertex(FPoint const& pos, Color color) : Vertex{pos, color} { }
	};

	class TexVertex : public Vertex
	{
	public:
		constexpr TexVertex() : Vertex{} { }
		constexpr TexVertex(FPoint const& pos, FPoint const& color) : Vertex{pos, color} { }
	};
}