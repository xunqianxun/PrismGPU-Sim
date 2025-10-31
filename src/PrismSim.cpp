#define STB_IMAGE_IMPLEMENTATION
#include "Vram.hpp"
#include "common.h"
#include "stb_image.h"
#include "InputAssimble.h"
#include "VertexProcess.h"
#include "RasterizerProcess.h"
#include "FragmentProcess.h"
#include "Display.h"


#define PCIEWORK 

 FrameTask FrameOne;

int main(int argc, char* argv[]) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        LOG_INFO("SDL_Init Error!");
        return 1;
    }
    // creat a windows
    SDL_Window* window = SDL_CreateWindow("PrismGPU-Sim", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

#ifdef PCIEWORK
    if(argc != 2 )
    {
        SDL_Quit();
        LOG_INFO("Usage: ./app <testfile>");
        return 1;
    }
    else 
    {
        std::string testfile = argv[1];


        std::vector<TestVBO> VBO = {

            {{0.5f, -0.5f,  0.5f},     {0.0f, 0.0f, 1.0f},   {1.0f,0.0f,0.0f},   {1.0f, 0.0f}},  // 1
            {{0.5f,  0.5f,  0.5f},     {0.0f, 0.0f, 1.0f},   {1.0f,0.0f,0.0f},   {1.0f, 1.0f}},  // 2
            {{-0.5f,  0.5f,  0.5f},    {0.0f, 0.0f, 1.0f},   {1.0f,0.0f,0.0f},   {0.0f, 1.0f}},  // 3

            // -Z 面 (back)  法线 (0,0,-1) 颜色: 绿
            {{0.5f, -0.5f, -0.5f},     {0.0f, 0.0f,-1.0f},   {0.0f,1.0f,0.0f},   {0.0f, 0.0f}},  // 4
            {{-0.5f, -0.5f, -0.5f},    {0.0f, 0.0f,-1.0f},   {0.0f,1.0f,0.0f},   {1.0f, 0.0f}},  // 5
            {{-0.5f,  0.5f, -0.5f},    {0.0f, 0.0f,-1.0f},   {0.0f,1.0f,0.0f},   {1.0f, 1.0f}},  // 6
            {{0.5f,  0.5f, -0.5f},     {0.0f, 0.0f,-1.0f},   {0.0f,1.0f,0.0f},   {0.0f, 1.0f}},  // 7

            // +X 面 (right) 法线 (1,0,0)  颜色: 蓝
            {{0.5f, -0.5f,  0.5f},     {1.0f, 0.0f, 0.0f},   {0.0f,0.0f,1.0f},   {0.0f, 0.0f}},  // 8
            {{0.5f, -0.5f, -0.5f},     {1.0f, 0.0f, 0.0f},   {0.0f,0.0f,1.0f},   {1.0f, 0.0f}},  // 9
            {{0.5f,  0.5f, -0.5f},     {1.0f, 0.0f, 0.0f},   {0.0f,0.0f,1.0f},   {1.0f, 1.0f}},  // 10
            {{0.5f,  0.5f,  0.5f},     {1.0f, 0.0f, 0.0f},   {0.0f,0.0f,1.0f},   {0.0f, 1.0f}},  // 11

            // -X 面 (left) 法线 (-1,0,0) 颜色: 黄
            {{-0.5f, -0.5f, -0.5f},    {-1.0f, 0.0f, 0.0f},  {1.0f,1.0f,0.0f},   {0.0f, 0.0f}},  // 12
            {{-0.5f, -0.5f,  0.5f},    {-1.0f, 0.0f, 0.0f},  {1.0f,1.0f,0.0f},   {1.0f, 0.0f}},  // 13
            {{-0.5f,  0.5f,  0.5f},    {-1.0f, 0.0f, 0.0f},  {1.0f,1.0f,0.0f},   {1.0f, 1.0f}},  // 14
            {{-0.5f,  0.5f, -0.5f},    {-1.0f, 0.0f, 0.0f},  {1.0f,1.0f,0.0f},   {0.0f, 1.0f}},  // 15

            // +Y 面 (top) 法线 (0,1,0)  颜色: 品红
            {{-0.5f,  0.5f,  0.5f},    {0.0f, 1.0f, 0.0f},   {1.0f,0.0f,1.0f},   {0.0f, 0.0f}},  // 16
            {{0.5f,  0.5f,  0.5f},     {0.0f, 1.0f, 0.0f},   {1.0f,0.0f,1.0f},   {1.0f, 0.0f}},  // 17
            {{0.5f,  0.5f, -0.5f},     {0.0f, 1.0f, 0.0f},   {1.0f,0.0f,1.0f},   {1.0f, 1.0f}},  // 18
            {{-0.5f,  0.5f, -0.5f},    {0.0f, 1.0f, 0.0f},   {1.0f,0.0f,1.0f},   {0.0f, 1.0f}},  // 19

            // -Y 面 (bottom) 法线 (0,-1,0) 颜色: 青
            {{-0.5f, -0.5f, -0.5f},    {0.0f,-1.0f, 0.0f},   {0.0f,1.0f,1.0f},   {0.0f, 0.0f}},  // 20
            {{0.5f, -0.5f, -0.5f},     {0.0f,-1.0f, 0.0f},   {0.0f,1.0f,1.0f},   {1.0f, 0.0f}},  // 21
            {{0.5f, -0.5f,  0.5f},     {0.0f,-1.0f, 0.0f},   {0.0f,1.0f,1.0f},   {1.0f, 1.0f}},  // 22
            {{-0.5f, -0.5f,  0.5f},    {0.0f,-1.0f, 0.0f},   {0.0f,1.0f,1.0f},   {0.0f, 1.0f}}   // 23
        };

        int NowVBO = vram.GetUsedAddr();
        bool State = true;

        

        State = vram.write(NowVBO, std::vector<uint8_t>(reinterpret_cast<uint8_t*>(VBO.data()), reinterpret_cast<uint8_t*>(VBO.data()) + sizeof(TestVBO) * VBO.size()));
        LOG_INFO("VBO Data Write in VRAM!");
        if(!State){
            LOG_ERROR("VBO Data Write in VRAM Failed!");
        }

        std::vector<Eigen::Vector3i> EBO = {
            // front
            {0,  1,  2},   {0,  2,  3},
            // back
            {4,  5,  6},   {4,  6,  7},
            // right
            {8,  9, 10},   {8, 10, 11},
            // left
            {12, 13, 14},  {12, 14, 15},
            // top
            {16, 17, 18},  {16, 18, 19},
            // bottom
            {20, 21, 22},  {20, 22, 23}
        };

        int NowEBO = vram.GetUsedAddr();
        int NowTEX;
        LOG_INFO("EBO Data Write in VRAM!");
        State = vram.write(NowEBO, std::vector<uint8_t>(reinterpret_cast<uint8_t*>(EBO.data()), reinterpret_cast<uint8_t*>(EBO.data()) + sizeof(Eigen::Vector3i) * EBO.size()));
        if(!State){
            LOG_ERROR("EBO Data Write in VRAM Failed!");
        } 

        int width, height, channels;
        std::vector<Eigen::Vector3f> textureData;
        unsigned char* ImgState = stbi_load(testfile.c_str(), &width, &height, &channels, 3);  //这个数组从左到右，从上到下
        LOG_INFO("load texture image!");
        if(!ImgState){
            NowTEX = NULL;
            SDL_Quit();
            stbi_image_free(ImgState);
            LOG_ERROR("Failed to load texture image!");
            return 1;
        }        
        else {
            for(int i=0; i<width * height; i++){
                Eigen::Vector3f color;
                color[0] = ImgState[i * 3 + 0] / 255.0f; // R
                color[1] = ImgState[i * 3 + 1] / 255.0f; // G
                color[2] = ImgState[i * 3 + 2] / 255.0f; // B
                textureData.push_back(color);
            }
        }
        stbi_image_free(ImgState);
        NowTEX = vram.GetUsedAddr();
        LOG_INFO("write texture data to VRAM!");
        State = vram.write(NowTEX, std::vector<uint8_t>(reinterpret_cast<uint8_t*>(textureData.data()), reinterpret_cast<uint8_t*>(textureData.data()) + sizeof(Eigen::Vector3f) * textureData.size()));
        if(!State){
            LOG_ERROR("Failed to write texture data to VRAM!");
            return 1;
        }

        MashEntry MashTaskone = {.TopologyType = TRIANGLE,
                             .EBOOffset = NowEBO,
                             .EBOCount = static_cast<int>(EBO.size()),
                             .VBOOffset = NowVBO,
                             .VBOAtribute = sizeof(TestVBO),
                             .VBOCount = static_cast<int>(VBO.size()),
                             .AmbientColor = {0.005f, 0.005f, 0.005f},
                             .DiffuseColor = {0.5f, 0.5f, 0.5f},
                             .SpecularColor = {0.79f, 0.79f, 0.79f},
                             .SpecularExponent = 150.0f,
                             .TEXOffset = NowTEX,
                             .BUMPOffset = 0
                             };

        int MeshTaskAddr = vram.GetUsedAddr();
        LOG_INFO("Mash Entry Write in VRAM!");
        State = vram.write(MeshTaskAddr, std::vector<uint8_t>(reinterpret_cast<uint8_t*>(&MashTaskone), reinterpret_cast<uint8_t*>(&MashTaskone) + sizeof(MashEntry)));
        if(!State){
            LOG_ERROR("Mash Entry Write in VRAM Failed!");
        }

        Eigen::Matrix4f ViewLoad ;
        ViewLoad << 1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, -10.0f,
                    0.0f, 0.0f, 0.0f, 1.0f;

        Eigen::Matrix4f ProjectLoad ;
        ProjectLoad <<  1.81066, 0.0f,     0.0f,      0.0f,
                        0.0f,    2.41421,  0.0f,      0.0f,
                        0.0f,    0.0f,    -1.00401f, -0.20040f,
                        0.0f,    0.0f,    -1.0f,      0.0f;

        MashSetup Setupone = {
            .RotateMatrix = Eigen::Matrix4f::Identity(), //不旋转
            .TranslateVector = Eigen::Matrix4f::Identity(), //不平移
            .ScaleVector = Eigen::Matrix4f::Identity(), //不缩放
            .ViewMatrix =  ViewLoad,
            .ProjectionMatrix = ProjectLoad
        };

        int MeshSetupAddr = vram.GetUsedAddr();
        LOG_INFO("Mash Setup Write in VRAM!");
        State = vram.write(MeshSetupAddr, std::vector<uint8_t>(reinterpret_cast<uint8_t*>(&Setupone), reinterpret_cast<uint8_t*>(&Setupone) + sizeof(MashSetup)));
        if(!State){
            LOG_ERROR("Mash Setup Write in VRAM Failed!");
        }

        FrameOne = { //相当于只有一个项的任务队列，正常来说应该有一个ringbuffer结构体来存放这几个结构体，总之这些完全由CPU端来管理，模式暂时简单些
            .MashSetups = MeshSetupAddr,
            .MashEntries = MeshTaskAddr
        };

    } 
#endif 


    //zbuffer init
    std::vector<float> zdata(WIDTH * HEIGHT, -FLT_MAX);
    std::vector<uint8_t> bytes(zdata.size() * sizeof(float));
    std::memcpy(bytes.data(), zdata.data(), bytes.size());
    vram.write(ZbufferStart, bytes);


    SDL_Event event;
    bool running = true;
    std::vector<Eigen::Vector3f> framebuffer(WIDTH * HEIGHT);

    while (running) {
        SDL_PollEvent(&event);
            if (event.type == SDL_QUIT) {
                running = false;
            }
        //TODO: 可以使用一些函数映射一些按键去更改矩阵等渲染的参数

        //-----

        //TODO: 其次渲染时可以看是否有任务，如果没有任务的话可以一直输当前的已经渲染结果
        IAToVertex TriAmbToV;
        VSToRaster VtoRast ;
        std::vector<RasterToPixel> RasterData;
        TriAmbToV = InputAssimble(FrameOne);
        VtoRast = VertexShaderProcess(FrameOne, TriAmbToV);
        RasterData = RasterizerProcessor(FrameOne, VtoRast);
        FragementShaderProcess(FrameOne,RasterData);

        std::vector<uint8_t> framebufferi(WIDTH * HEIGHT * sizeof(Eigen::Vector3f));
        vram.read(FrambufferStart, framebufferi, WIDTH * HEIGHT * sizeof(Eigen::Vector3f));

        std::memcpy(framebuffer.data(), framebufferi.data(), WIDTH * HEIGHT * sizeof(Eigen::Vector3f));

        displayFramebuffer(window, renderer, framebuffer, WIDTH, HEIGHT);
    }
    // destroy window and quit SDL
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}