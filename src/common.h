#ifndef COMMON_H
#define COMMON_H

#include <eigen3/Eigen/Dense>
#include <vector>
#include "Log.h"

#define TEMPTEST 

    enum TopologyType {
        POIN = 0,
        LINE = 1,
        TRIANGLE = 2

    };

    struct IAToVertex{
        Eigen::Matrix4f TriVertexPos ;
        Eigen::Matrix3f Normal ;
        Eigen::Matrix3f Color ;
        std::array<Eigen::Vector2f, 3> TexCoord ;
    };

    typedef Eigen::Matrix4f  TriangleAssemble;

    struct VSToRaster {
        Eigen::Matrix4f MVPtransfer ;
        Eigen::Matrix4f NoProject   ;
    };


#endif // COMMON_H