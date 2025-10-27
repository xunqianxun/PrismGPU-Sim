#ifndef SIMPLE_H
#define SIMPLE_H


#include "common.h"
#include "Vram.hpp"

class Simple {
    private:
        int Width;
        int Height;
        std::string  SimpleType;
    public:
        Simple(int InWidth, int InHeight );

        Eigen::Vector3f SimpleProcess(Eigen::Vector2i InIndex, std::string SimpleType) {
            InIndex.x() = std::clamp(InIndex.x(), 0.0f, 1.0f);
            InIndex.y() = std::clamp(InIndex.y(), 0.0f, 1.0f);
            
            int Uindex = InIndex.x() * Width;
            int Vindex = (1.0f - InIndex.y()) * Height;


        }


};



#endif // SIMPLE_H