#include "RasterizerProcess.h"

ResterizerProcess Rasterizer;

float IndexZbufferGet(Eigen::Vector2i Index){
    float DepthData ;

    int ramindex = Index.y() * WIDTH + Index.x();
    int offset = ZbufferStart + ramindex ;

    std::vector<uint8_t> bytes;
    vram.read(offset, bytes, sizeof(float));    
    std::memcpy(&DepthData, bytes.data(), sizeof(float));
    return DepthData;
}

float IndexZbufferSet(Eigen::Vector2i Index, float InDepth){
    int ramindex = Index.y() * WIDTH + Index.x();
    int offset = ZbufferStart + ramindex ;

    std::vector<uint8_t> bytes(sizeof(float));
    std::memcpy(bytes.data(), &InDepth, sizeof(float));
    vram.write(offset, bytes);    
    return InDepth;
}

std::vector<RasterToPixel> RasterizerProcessor(FrameTask &InFramTask, VSToRaster &InTriAmb){
    std::vector<RasterToPixel> ResterReturn;
    float Zdata ;

    Eigen::Vector4f InV1, InV2, InV3;
    InV1 = InTriAmb.TriVertexPos.col(0);
    InV2 = InTriAmb.TriVertexPos.col(1);
    InV3 = InTriAmb.TriVertexPos.col(2);
    Rasterizer.LoadVertex(InV1,InV2, InV3);

    Eigen::Vector3f InVNProj1, InVNProj2, InVNProj3;
    InVNProj1 = InTriAmb.TriVertexNoProj.col(0).head<3>();
    InVNProj2 = InTriAmb.TriVertexNoProj.col(1).head<3>();
    InVNProj3 = InTriAmb.TriVertexNoProj.col(2).head<3>();

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
                Zdata = IndexZbufferGet({x,y});
                BaryData = Rasterizer.PointBaryCentrie(x, y);
                float AlphaW = BaryData.alpha / InV1.w();
                float BetaW = BaryData.beta / InV2.w();
                float GammaW = BaryData.gamma / InV3.w();

                float Zweight = 1.0f / (AlphaW + BetaW + GammaW); //w↑⇒zbuffer​↑,所以可以使用Zweight来进行深度比较

                if(Zweight > Zdata){
                    IndexZbufferSet({x,y}, Zweight);

                    RasterToPixel PixelData;
                    PixelData.index = {x, y};
                    PixelData.Word3DPoint = (InVNProj1 * AlphaW + InVNProj2 * BetaW + InVNProj3 * GammaW) * Zweight ;
                    PixelData.Normal = (InTriAmb.Normal.col(0) * AlphaW + InTriAmb.Normal.col(1) * BetaW + InTriAmb.Normal.col(2) * GammaW) * Zweight ;
                    PixelData.TexCoord = (InTriAmb.TexCoord[0] * AlphaW + InTriAmb.TexCoord[1] * BetaW + InTriAmb.TexCoord[2] * GammaW) * Zweight ;
                    PixelData.Color = (InTriAmb.Color.col(0) * AlphaW + InTriAmb.Color.col(1) * BetaW + InTriAmb.Color.col(2) * GammaW) * Zweight ;

                    ResterReturn.push_back(PixelData);
                }

            }

        }
        
    }
    


    return ResterReturn;
}