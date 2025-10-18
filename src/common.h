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

    struct TestVBO {
        Eigen::Vector3f positions;
        Eigen::Vector3f normals;
        Eigen::Vector3f colors;
        Eigen::Vector2f texcoords;
    };


#endif // COMMON_H