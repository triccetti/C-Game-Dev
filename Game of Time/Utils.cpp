#include "Utils.h"
#include "Components.h"

bool Utils::isMouseOver(SDL_Rect * rect) {
	int x, y;
	SDL_GetMouseState(&x, &y);

	return (x <= rect->x + rect->w && x >= rect->x
		&& y <= rect->y + rect->h && y >= rect->y);
}

bool Utils::collision(const SDL_Rect& recA, const SDL_Rect& recB) {
	if (
		recA.x + recA.w >= recB.x &&
		recB.x + recB.w >= recA.x &&
		recA.y + recA.h >= recB.y &&
		recB.y + recB.h >= recA.y
		) {
		return true;
	}

	return false;
}

bool Utils::collision(const CollisionComponent& colA, const CollisionComponent& colB) {
	return collision(colA.collider, colB.collider);
}

void Utils::saveTexturePng(SDL_Renderer* renderer, SDL_Texture* texture, const char* file_name) {
	SDL_Texture* target = SDL_GetRenderTarget(renderer);
	SDL_SetRenderTarget(renderer, texture);
	int width, height;
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	SDL_Surface* pngSurf = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
	SDL_FreeSurface(surface);
	SDL_RenderReadPixels(renderer, NULL, pngSurf->format->format, pngSurf->pixels, pngSurf->pitch);
	IMG_SavePNG(pngSurf, file_name);
	SDL_FreeSurface(pngSurf);
	SDL_SetRenderTarget(renderer, target);
}

/*
	Saves given SDL_Texture to the given file.
	REF: https://stackoverflow.com/questions/34255820/save-sdl-texture-to-file
*/
void Utils::save_texture(SDL_Renderer *ren, SDL_Texture *tex, const char *filename) {
	SDL_Texture *ren_tex;
	SDL_Surface *surf;
	int st;
	int w;
	int h;
	int format;
	void *pixels;

	pixels = NULL;
	surf = NULL;
	ren_tex = NULL;
	format = SDL_PIXELFORMAT_RGBA32;

	/* Get information about texture we want to save */
	st = SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	if (st != 0) {
		SDL_Log("Failed querying texture: %s\n", SDL_GetError());
		goto cleanup;
	}

	ren_tex = SDL_CreateTexture(ren, format, SDL_TEXTUREACCESS_TARGET, w, h);
	if (!ren_tex) {
		SDL_Log("Failed creating render texture: %s\n", SDL_GetError());
		goto cleanup;
	}

	/*
	 * Initialize our canvas, then copy texture to a target whose pixel data we
	 * can access
	 */
	st = SDL_SetRenderTarget(ren, ren_tex);
	if (st != 0) {
		SDL_Log("Failed setting render target: %s\n", SDL_GetError());
		goto cleanup;
	}

	SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(ren);

	st = SDL_RenderCopy(ren, tex, NULL, NULL);
	if (st != 0) {
		SDL_Log("Failed copying texture data: %s\n", SDL_GetError());
		goto cleanup;
	}

	/* Create buffer to hold texture data and load it */
	pixels = malloc(w * h * SDL_BYTESPERPIXEL(format));
	if (!pixels) {
		SDL_Log("Failed allocating memory\n");
		goto cleanup;
	}

	st = SDL_RenderReadPixels(ren, NULL, format, pixels, w * SDL_BYTESPERPIXEL(format));
	if (st != 0) {
		SDL_Log("Failed reading pixel data: %s\n", SDL_GetError());
		goto cleanup;
	}

	/* Copy pixel data over to surface */
	surf = SDL_CreateRGBSurfaceWithFormatFrom(pixels, w, h, SDL_BITSPERPIXEL(format), w * SDL_BYTESPERPIXEL(format), format);
	if (!surf) {
		SDL_Log("Failed creating new surface: %s\n", SDL_GetError());
		goto cleanup;
	}

	/* Save result to an image */
	st = SDL_SaveBMP(surf, filename);
	if (st != 0) {
		SDL_Log("Failed saving image: %s\n", SDL_GetError());
		goto cleanup;
	}

	SDL_Log("Saved texture as BMP to \"%s\"\n", filename);

cleanup:
	SDL_FreeSurface(surf);
	free(pixels);
	SDL_DestroyTexture(ren_tex);
}