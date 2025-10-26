#ifndef RASTERIZERPROCESS_H
#define RASTERIZERPROCESS_H

#include "common.h"

std::vector<RasterToPixel> RasterizerProcessor(FrameTask &InFramTask, VSToRaster &InTriAmb);

struct Bounding {
    float Minx ;
    float Maxx ;
    float Miny ;
    float Maxy ;
};

struct Barycentrie{
    float alpha;
    float beta;
    float gamma;
};

class ResterizerProcess{
    private:
    Eigen::Vector4f Vertex1;
    Eigen::Vector4f Vertex2;
    Eigen::Vector4f Vertex3;

    public:

    int LoadVertex (Eigen::Vector4f InTriV1, Eigen::Vector4f InTriV2, Eigen::Vector4f InTriV3){
        Vertex1 = InTriV1 ;
        Vertex2 = InTriV2 ;
        Vertex3 = InTriV3 ;

        return true;
    }

    Bounding TriBoundFind(){
        Bounding RetBound ;

        RetBound.Minx = std::floor(std::min(Vertex1.x(), std::min(Vertex2.x(), Vertex3.x())));
        RetBound.Maxx = std::ceil(std::max(Vertex1.x(), std::max(Vertex2.x(), Vertex3.x())));
        RetBound.Miny = std::floor(std::min(Vertex1.y(), std::min(Vertex2.y(), Vertex3.y())));
        RetBound.Maxy = std::ceil(std::max(Vertex1.y(), std::max(Vertex2.y(), Vertex3.y())));

        return RetBound ;
    }

    Barycentrie PointBaryCentrie (float x, float y){

        Barycentrie RetBary ;

        RetBary.alpha = (x * (Vertex2.y() - Vertex3.y()) + (Vertex3.x() - Vertex2.x()) * y + Vertex2.x() * Vertex3.y() - Vertex3.x() * Vertex2.y()) / (Vertex1.x() * (Vertex2.y() - Vertex3.y()) + (Vertex3.x() - Vertex2.x()) * Vertex1.y() + Vertex2.x() * Vertex3.y() - Vertex3.x() * Vertex2.y());
        RetBary.beta  = (x * (Vertex3.y() - Vertex1.y()) + (Vertex1.x() - Vertex3.x()) * y + Vertex3.x() * Vertex1.y() - Vertex1.x() * Vertex3.y()) / (Vertex2.x() * (Vertex3.y() - Vertex1.y()) + (Vertex1.x() - Vertex3.x()) * Vertex2.y() + Vertex3.x() * Vertex1.y() - Vertex1.x() * Vertex3.y());
        RetBary.gamma = (x * (Vertex1.y() - Vertex2.y()) + (Vertex2.x() - Vertex1.x()) * y + Vertex1.x() * Vertex2.y() - Vertex2.x() * Vertex1.y()) / (Vertex3.x() * (Vertex1.y() - Vertex2.y()) + (Vertex2.x() - Vertex1.x()) * Vertex3.y() + Vertex1.x() * Vertex2.y() - Vertex2.x() * Vertex1.y());

        return RetBary ;
    }

    int IsInsideTri (float a, float b){
        Eigen::Vector3f V_1, V_2, V_3 ;
        Eigen::Vector3f L_1, L_2, L_3 ;

        V_1 = {Vertex1.x(), Vertex1.y(), 1.0f};
        V_2 = {Vertex2.x(), Vertex2.y(), 1.0f};
        V_3 = {Vertex3.x(), Vertex3.y(), 1.0f};

        L_1 = V_2.cross(V_1);
        L_2 = V_3.cross(V_2);
        L_3 = V_1.cross(V_3);

        Eigen::Vector3f Point = {a, b, 1.0f};

        //点乘满足交换律，所以点乘交换位置不会影响结果
        float LoadSameV3 = Point.dot(L_1) * L_1.dot(V_3);
        float LoadSameV2 = Point.dot(L_3) * L_3.dot(V_2);
        float LoadSameV1 = Point.dot(L_2) * L_2.dot(V_1);

        if((LoadSameV3 >=0) && (LoadSameV2 >=0) && (LoadSameV1 >=0)){
            return true;
        }
        else {
            return false;
        }
    }

};

#endif // RASTERIZERPROCESS_H