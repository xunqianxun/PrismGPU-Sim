#ifndef DISPLAY_H
#define DISPLAY_H

#include "common.h"

void displayFramebuffer(SDL_Window* window, SDL_Renderer* renderer, const std::vector<Eigen::Vector3f>& framebuffer, int width, int height);

#endif // DISPLAY_H