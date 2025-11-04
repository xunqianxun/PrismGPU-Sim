#ifndef COMMON_H
#define COMMON_H

#include <eigen3/Eigen/Dense>
#include <algorithm>
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <array>
#include "Log.h"

#define TEMPTEST 

#define DEBUGING

    enum TopologyType {
        POIN = 0,
        LINE = 1,
        TRIANGLE = 2

    };

    // now we const width and height
    #define WIDTH  640
    #define HEIGHT  480 

    // const near and far also 
    #define NEAR  0.1
    #define FAR  50

    struct IAToVertex{
        Eigen::Matrix4f TriVertexPos ;
        Eigen::Matrix3f Normal ;
        Eigen::Matrix3f Color ;
        std::array<Eigen::Vector2f, 3> TexCoord ;
    };

    typedef Eigen::Matrix4f  TriangleAssemble;

    struct VSToRaster {
        Eigen::Matrix4f TriVertexPos ;
        Eigen::Matrix4f TriVertexNoProj ;
        Eigen::Matrix3f Normal ;
        Eigen::Matrix3f Color ;
        std::array<Eigen::Vector2f, 3> TexCoord ;
    };

    //正常情况下在硬件上应该是光栅化应该是直接向pixel shader 传送当前点的属性，并且针对这些属性直接进行渲染，但是事实上这么做并不方便，所以现在的
    //做法是光栅化对一个三角形完全执行完，把这些属性值存储在容器里。在光栅化结束之后将数据从容器送入pixel shader
    struct RasterToPixel{
        Eigen::Vector2i index ;
        Eigen::Vector3f Word3DPoint ;
        Eigen::Vector3f Normal ;
        Eigen::Vector2f TexCoord ;
        Eigen::Vector3f Color ;
    };


#endif // COMMON_H