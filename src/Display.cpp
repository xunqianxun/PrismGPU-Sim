#include "Display.h"

void displayFramebuffer(SDL_Window* window, SDL_Renderer* renderer, const std::vector<Eigen::Vector3f>& framebuffer, int width, int height)
{
    // 1. 创建纹理
    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGB24,          // 3字节RGB格式
        SDL_TEXTUREACCESS_STREAMING,
        width, height
    );

    // 2. 写入像素数据
    void* pixels = nullptr;
    int pitch = 0;
    SDL_LockTexture(texture, nullptr, &pixels, &pitch);
    uint8_t* dst = static_cast<uint8_t*>(pixels);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const Eigen::Vector3f& color = framebuffer[y * width + x];
            uint8_t* p = dst + y * pitch + x * 3;

            // 假如 framebuffer 是 [0,255]
            p[0] = static_cast<uint8_t>(std::clamp(color.x(), 0.0f, 255.0f));
            p[1] = static_cast<uint8_t>(std::clamp(color.y(), 0.0f, 255.0f));
            p[2] = static_cast<uint8_t>(std::clamp(color.z(), 0.0f, 255.0f));
        }
    }

    SDL_UnlockTexture(texture);

    // 3. 显示
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(texture);
}