#pragma once
#include "Components.h"

class UIHueSaturationValue : public Component {
private:

	SDL_Surface * surface;
	SDL_Texture * texture;
	SDL_Rect position;
	SDL_Color color = { 255, 0, 0 };
	int brightness = 255;
	Uint32 rmask, gmask, bmask, amask;

public:

	UIHueSaturationValue(int x, int y, int h, int w) {
		position.x = x;
		position.y = y;
		position.h = h;
		position.w = w;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
#endif

		//surface = SDL_CreateRGBSurface(0, 255 * 6, 255, 32, rmask, gmask, bmask, amask);

		surface = SDL_CreateRGBSurface(0, 255 * 6, 255, 32, 0, 0, 0, 0);

		if (surface == NULL) {
			SDL_Log("SDL_CreateRGBSurface() failed: %s", SDL_GetError());
			exit(1);
		}

		//SDL_CreateTexture(Game::renderer, SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING, 255 * 6, 255);
	}

	~UIHueSaturationValue() {}

	void init() override {
		drawColors();
		texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
		//drawColorWheel();
	}

	void update() override {
		if (Game::event.type == SDL_MOUSEBUTTONDOWN && Utils::isMouseOver(&position)) {
			printf("clicked!\n");

			//	printf("color[ r: %d, g: %d, b: %d]\n", );
		}
	}

	void render() override {
		SDL_RenderCopy(Game::renderer, texture, nullptr, &position);
	}

	void drawColors() {
		for (int y = 0; y < 255; y++) {
			for (int x = 0; x < 255 * 6; x++) {
				//Red 255 - Green 0-254
				if (color.r == brightness && color.g < brightness && color.b == 0) {
					color.g += 1;

					//color.R -= (byte)y;
					//color.G += (byte)y;
					//color.B += (byte)y;
				}
				//Green 255 - Red 255-0
				else if (color.r > 0 && color.g == brightness && color.b == 0) {
					color.r -= 1;

					//color.R -= (byte)y;
					//color.G -= (byte)y;
					//color.B += (byte)y;
				}
				//Green 255 - Blue 0-255
				else if (color.r == 0 && color.g == brightness && color.b < brightness) {
					color.b += 1;

					//color.R += (byte)y;
					//color.G -= (byte)y;
					//color.B += (byte)y;
				}
				//Blue 255 - Green 255-0
				else if (color.r == 0 && color.g > 0 && color.b == brightness) {
					color.g -= 1;

					//color.R += (byte)y;
					//color.G -= (byte)y;
					//color.B -= (byte)y;
				}
				//Blue 255 - Red 0-255
				else if (color.r < brightness && color.g == 0 && color.b == brightness) {
					color.r += 1;

					//color.R += (byte)y;
					//color.G += (byte)y;
					//color.B -= (byte)y;
				}
				//Red 255 - Blue 255-0
				else if (color.r == brightness && color.g == 0 && color.b > 0) {
					color.b -= 1;

					//color.R -= (byte)y;
					//color.G += (byte)y;
					//color.B -= (byte)y;
				}

				//.SetPixel((uint)x, (uint)y, color);
				SDL_Rect rect = { x,y,1,1 };
				SDL_UpdateTexture(texture, &rect, &color, brightness);
			}
			//brightness--;
		}
	}

	void drawColorWheel() {
	}
};