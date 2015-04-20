#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/*
        This file contains the initialization code for SDL2, so we can
        generate valgrind rules.
*/

int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  IMG_Init(IMG_INIT_PNG);

  SDL_AudioDeviceID dev = SDL_OpenAudioDevice(NULL, 0, NULL, NULL, 0);

  // create a window
  SDL_Window *window =
      SDL_CreateWindow("Minimal Init STUB", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

  // draw one empty frame
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  // clean up
  SDL_CloseAudioDevice(dev);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();

  return 0;
}
