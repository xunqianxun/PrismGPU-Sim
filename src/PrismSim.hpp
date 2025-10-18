#include <vector>
#include <eigen3/Eigen/Dense>
#include <algorithm>
#include <SDL2/SDL.h>
#include "Vram.hpp"


int main(int argc, char* argv[]) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::string("SDL_Init Error!");
        return 1;
    }
    // creat a windows
    SDL_Window* window = SDL_CreateWindow("PrismGPU-Sim", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Quit();
        return 1;
    }
    if(argc != 2 )
    {
        SDL_Quit();
        throw std::string("Usage: ./PrismSim <testfile>");
        return 1;
    }
    else 
    {
        std::string testfile = argv[1];

#ifdef TEMPTEST
        Vram vram;

        //假设这是第一次的任务他的ringbuffer指示任务的数据起始地址是0x0000
        vram.write(0x00000000,); 
#endif
    }
    

    SDL_Event event;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        SDL_Delay(16);
    }
    // destroy window and quit SDL
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}