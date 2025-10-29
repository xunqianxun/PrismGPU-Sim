#ifndef INPUTASSEMBLE_H
#define INPUTASSEMBLE_H

#include "common.h"
#include "Vram.hpp"

IAToVertex InputAssimble(const FrameTask& frameTask);


class InputAssemble {
    private:
        IAToVertex InputAmb;
    public:
        InputAssemble() = default;
        
        void loadAmb(const IAToVertex& tri ) {
            InputAmb = tri;
        }

        IAToVertex ReadAmb() {
            return InputAmb ;
        }

    };

#endif // INPUTASSEMBLE_H