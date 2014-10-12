#include <iostream>
#include <SDL2/SDL.h>
#include "cleanup.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void logSDLError(std::ostream &os, const std::string &msg)
{
  os << msg << " Error: " << SDL_GetError() << std::endl;
}

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
{
  SDL_Texture *texture = nullptr;

  SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());

  if (loadedImage != nullptr)
  {
    texture = SDL_CreateTextureFromSurface(ren, loadedImage);
    SDL_FreeSurface(loadedImage);

    if (texture == nullptr)
    {
      logSDLError(std::cout, "CreateTextureFromSurface");
    }
  }
  else
  {
    logSDLError (std::cout, "LoadBMP");
  }
  return texture;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y)
{
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;

  SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
  SDL_RenderCopy(ren, tex, NULL, &dst);
}

int main(int argc, char* argv[])
{

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    logSDLError(std::cout, "SDL_Init");
    return 1;
  }

  SDL_Window *win = SDL_CreateWindow("Hello world!", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (win == nullptr)
  {
    logSDLError(std::cout, "SDL_CreateWindow");
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == nullptr)
  {
    logSDLError(std::cout, "SDL_CreateRenderer");
    cleanup(win);
    SDL_Quit();
    return 1;
  }

  SDL_Texture *background = loadTexture ("background.bmp", ren);
  SDL_Texture *image = loadTexture ("image.bmp", ren);
  if (background == nullptr || image == nullptr)
  {
    cleanup (background, image, ren, win);
    SDL_Quit();
    return 1;
  }

  SDL_RenderClear(ren);
  int bW, bH;
  SDL_QueryTexture(background, NULL, NULL, &bW, &bH);

// TODO: Replace with for loop

  renderTexture(background, ren, 0, 0);
  renderTexture(background, ren, bW, 0);
  renderTexture(background, ren, 0, bH);
  renderTexture(background, ren, bW, bH);

  int iW, iH;
  SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
  int x = SCREEN_WIDTH / 2 - iW / 2;
  int y = SCREEN_HEIGHT / 2 - iH / 2;
  renderTexture(image, ren, x, y);

  SDL_RenderPresent(ren);
  SDL_Delay (2000);

  cleanup(background, image, ren, win);

  SDL_Quit();

  return 0;
}
