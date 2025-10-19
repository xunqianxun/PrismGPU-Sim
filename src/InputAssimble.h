#ifndef INPUTASSEMBLE_H
#define INPUTASSEMBLE_H

#include "common.h"

int InputAssimble(const FrameTask& frameTask, TriangleAssemble& outTriangleA,
                  TriangleAssemble& outTriangleB,
                  TriangleAssemble& outTriangleC);


class InputAssemble {
    private:
        TriangleAssemble triangleA;
        TriangleAssemble triangleB;
        TriangleAssemble triangleC;
    public:
        InputAssemble() = default;
        
        void loadTriangle(const TriangleAssemble& tri1,
                          const TriangleAssemble& tri2,
                          const TriangleAssemble& tri3) {
            triangleA = tri1;
            triangleB = tri2;
            triangleC = tri3;
        }

        int ToVertexShader(TriangleAssemble& outTri1,
                           TriangleAssemble& outTri2,
                           TriangleAssemble& outTri3) {
            outTri1 = triangleA;
            outTri2 = triangleB;
            outTri3 = triangleC;
            return 0;
        }

    };

#endif // INPUTASSEMBLE_H