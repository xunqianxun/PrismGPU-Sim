#include "RasterizerProcess.h"
#include "Vram.hpp"

ResterizerProcess Rasterizer;

std::vector<RasterToPixel> RasterizerProcessor(FrameTask &InFramTask, VSToRaster &InTriAmb){
    std::vector<RasterToPixel> ResterReturn;

    Eigen::Vector4f InV1, InV2, InV3;
    InV1 = InTriAmb.TriVertexPos.col(0);
    InV2 = InTriAmb.TriVertexPos.col(1);
    InV3 = InTriAmb.TriVertexPos.col(2);
    Rasterizer.LoadVertex(InV1,InV2, InV3);

    Bounding BoundData;
    Barycentrie BaryData;
    BoundData = Rasterizer.TriBoundFind();
    
    for (size_t x = BoundData.Minx; x < BoundData.Maxx; ++x)
    {
        for (size_t y = BoundData.Miny; y < BoundData.Maxy; ++y)
        {
            if(!Rasterizer.IsInsideTri(x+0.5f, y+0.5f)){
                continue;
            }
            else {
                BaryData = Rasterizer.PointBaryCentrie(x, y);
                float AlphaW = BaryData.alpha / InV1.w();
                float BetaW = BaryData.beta / InV2.w();
                float GammaW = BaryData.gamma / InV3.w();

                float Zweight = 1.0f / (AlphaW + BetaW + GammaW);

                if()

            }

        }
        
    }
    


    return ResterReturn;
}