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

    struct TriangleAssemble
    {
        Eigen::Matrix3f vertices; // 三角形的三个顶点
    };

#endif // COMMON_H