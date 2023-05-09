#include <iostream>
#include <chrono>
#include <thread>

#include <SDL.h>
#include <glad/glad.h>

int main(int, char**) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << SDL_GetError() << '\n';
		std::terminate();
	}

	auto pWindow = SDL_CreateWindow("Tiddies", 300, 200, 1080, 720, SDL_WINDOW_OPENGL);
	if (pWindow == nullptr) {
		std::cout << SDL_GetError() << '\n';
		std::terminate();
	}

	auto pWindowSurface = SDL_GetWindowSurface(pWindow);
	SDL_UpdateWindowSurface(pWindow);

	void GameLoop(SDL_Window&);
	GameLoop(*pWindow);

	SDL_DestroyWindow(*&pWindow);
	SDL_Quit();
	return 0;
}

#define CHECK_FOR_NULL(_call, _msg) \
	if (_call == nullptr) { \
		std::cout << _msg << SDL_GetError() << '\n'; \
		std::terminate(); \
	}

void PutPixel(SDL_Surface& sur, int x, int y, std::uint32_t color) {
	if (x >= 0 && x < 1080 && y >= 0 && y < 720) {
		SDL_LockSurface(&sur);
		static_cast<std::uint32_t*>(sur.pixels)[x + y * sur.w] = color;
		SDL_UnlockSurface(&sur);
	}
}

void DrawCircle(SDL_Renderer& renny, float cx, float cy, float r, std::uint32_t color = -1) {
	auto const steps{static_cast<std::size_t>(70 * 0.01f * r)};
	auto const fAngle{2.f * 3.1415926535f / steps};
	std::vector<SDL_FPoint> points{};
	points.reserve(steps);
	for (std::size_t i{}; i < steps + 1; ++i) {
		points.push_back(SDL_FPoint{
			r * std::cos(i * fAngle) + cx,
			r * std::sin(i * fAngle) + cy,
			});
	}
	SDL_RenderDrawLinesF(&renny, points.data(), static_cast<int>(points.size()));
}

void FillCircle(SDL_Renderer& renny, float cx, float cy, float r, std::uint32_t color = -1) {
	auto const steps{static_cast<std::size_t>(70 * 0.01f * r)};
	auto const fAngle{2.f * 3.1415926535f / steps};
	std::vector<SDL_Vertex> verts{};
	verts.reserve(steps);
	verts.push_back({
		.position{cx, cy},
		.color{
			static_cast<std::uint8_t>((color >>  0) & 0xFF),
			static_cast<std::uint8_t>((color >>  8) & 0xFF),
			static_cast<std::uint8_t>((color >> 16) & 0xFF),
			static_cast<std::uint8_t>((color >> 24) & 0xFF),
		},
		.tex_coord{}
	});

	std::vector<int> inds{};
	inds.reserve(3 * steps);
	for (std::size_t i{}; i < steps + 1; ++i) {
		verts.push_back({
			.position{
				r * std::cos(i * fAngle) + cx,
				r * std::sin(i * fAngle) + cy,
			},
			.color{
				static_cast<std::uint8_t>((color >>  0) & 0xFF),
				static_cast<std::uint8_t>((color >>  8) & 0xFF),
				static_cast<std::uint8_t>((color >> 16) & 0xFF),
				static_cast<std::uint8_t>((color >> 24) & 0xFF),
			},
			.tex_coord{}
		});
		inds.push_back(0);
		inds.push_back(static_cast<int>(i));
		inds.push_back(static_cast<int>(i) + 1);
	}
	SDL_RenderGeometry(&renny, nullptr, verts.data(), static_cast<int>(verts.size()), inds.data(),
		static_cast<int>(inds.size()));
}

void GameLoop(SDL_Window& window) {
	auto& windowSurface{*SDL_GetWindowSurface(&window)};
	auto mouseX{0};
	auto mouseY{0};
	auto pRenderer{SDL_CreateRenderer(&window, -1, SDL_RendererFlags::SDL_RENDERER_ACCELERATED)};
	if (pRenderer == nullptr) {
		std::cout << "Renderer sucked\n";
		std::terminate();
	}
	auto& renderer{*pRenderer};

	auto pImageSur{SDL_LoadBMP("resources/Hello.bmp")};
	CHECK_FOR_NULL(pImageSur, "LoadBMP_RW sucked: ");
	auto pTex{SDL_CreateTextureFromSurface(&renderer, pImageSur)};
	CHECK_FOR_NULL(pImageSur, "CreateTextureFromSurface sucked: ");

	for (auto bGameIsRunning{true}; bGameIsRunning;) {
		// auto constexpr windowRect = SDL_Rect{0, 0, 1080, 720};
		// SDL_FillRect(&windowSurface, &windowRect, 0xFF882255);

		SDL_SetRenderDrawColor(&renderer, 0x11, 0x22, 0x33, 0xFF);
		SDL_RenderClear(&renderer);

		constexpr auto W{100.f};
		SDL_FRect const myRect{mouseX - W, mouseY - W, W * 2, W * 2};
		// SDL_SetRenderDrawColor(&renderer, 0x22, 0x88, 0x11, 0xFF);
		// // SDL_RenderFillRectF(&renderer, &myRect) < 0) 
		// FillCircle(renderer, mouseX * 1.f, mouseY * 1.f, W, 0xFF6622FF);
		SDL_RenderCopyF(&renderer, pTex, nullptr, &myRect);
		SDL_RenderPresent(&renderer);

		SDL_Event currEvent{};
		while (SDL_PollEvent(&currEvent)) {
			switch (currEvent.type) {
			case SDL_EventType::SDL_QUIT:
				bGameIsRunning = false;
				break;
			case SDL_MOUSEMOTION:
				mouseX = currEvent.motion.x;
				mouseY = currEvent.motion.y;
				break;
			default:
				break;
			}
		}

		//DrawCircle(windowSurface, mouseX, mouseY, 0xFF338811, RectWidth / 2.f);
	}
}