#include "FragmentProcess.h"
#include "simple.h"

Simple simple(WIDTH, HEIGHT);

int FragementShaderProcess(FrameTask &InFramTask, std::vector<RasterToPixel> &InRasterData){
    FragmentProcess fragmentProcessor;
    Eigen::Vector3f CurrentColor;
    InPixelData PixelData;

    std::vector<uint8_t> PixelAmbBuffer;
    int AmbOffset = InFramTask.MashEntries;
    MashEntry MashentriDataPixel;
    bool State = vram.read(AmbOffset, PixelAmbBuffer, sizeof(MashEntry));
    std::memcpy(&MashentriDataPixel, PixelAmbBuffer.data(), sizeof(MashEntry));

    for(size_t i = 0; i < InRasterData.size(); ++i){
        RasterToPixel CurrentPixelData = InRasterData[i];

        int TexBase = MashentriDataPixel.TEXOffset;

        // if(TexBase != NULL)
        //     CurrentColor = simple.SimpleProcess(
        //         TexBase,
        //         CurrentPixelData.TexCoord,
        //         "TEXTURE"
        //     );
        // else 
            CurrentColor = CurrentPixelData.Color;

        fragmentProcessor.LoadLightParam(
            MashentriDataPixel.AmbientColor,
            CurrentColor,
            MashentriDataPixel.SpecularColor,
            static_cast<int>(MashentriDataPixel.SpecularExponent)
        );

        PixelData.Word3DPoint = CurrentPixelData.Word3DPoint;
        PixelData.Normal = CurrentPixelData.Normal;

        Eigen::Vector3f ShadedColor = fragmentProcessor.PixelShading(PixelData, CurrentPixelData.index);
        int offset = (CurrentPixelData.index.y() * WIDTH + CurrentPixelData.index.x()) * sizeof(Eigen::Vector3f);

        vram.write(FrambufferStart + offset , std::vector<uint8_t>(reinterpret_cast<uint8_t*>(ShadedColor.data()), reinterpret_cast<uint8_t*>(ShadedColor.data()) + sizeof(Eigen::Vector3f))); //也有问题其实直接用3i也是一样的

    }

    return 0;
}