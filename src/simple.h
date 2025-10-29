#ifndef SIMPLE_H
#define SIMPLE_H


#include "common.h"
#include "Vram.hpp"
#include <algorithm>

class Simple {
    private:
        int Width;
        int Height;
        std::string  SimpleType;
    public:
        Simple(int InWidth, int InHeight ){
            Width = InWidth ;
            Height = InHeight ;
        }

        Eigen::Vector3f SimpleProcess(int Base, Eigen::Vector2f InIndex, std::string SimpleType) {
            InIndex.x() = std::clamp(InIndex.x(), 0.0f, 1.0f);
            InIndex.y() = std::clamp(InIndex.y(), 0.0f, 1.0f);

            int Uindex = InIndex.x() * Width;
            int Vindex = (1.0f - InIndex.y()) * Height;

            int Index = Base + (Vindex * Width + Uindex) * sizeof(Eigen::Vector3f);

            std::vector<uint8_t> bytes;
            Eigen::Vector3f ColorData;

            vram.read(Index, bytes, sizeof(Eigen::Vector3f));

            std::memcpy(&ColorData, bytes.data(), sizeof(Eigen::Vector3f));

            return ColorData;
        }


};



#endif // SIMPLE_H