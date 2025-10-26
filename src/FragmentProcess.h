#ifndef FRAGMENTPROCESS_H
#define FRAGMENTPROCESS_H

#include "common.h"


int FragmentShaderProcess(FrameTask &InFramTask, std::vector<RasterToPixel> &InRasterData);

class FragmentProcess{
    private:


    public:


}; //光没有定义，服了，现在打算自己定义一个，但是实际上应该是和图形一起进行VS和光栅化的，不对，按道理来说只需要知道这个点就好了，但是这个点也是要进行MVP变换的

#endif // FRAGMENTPROCESS_H