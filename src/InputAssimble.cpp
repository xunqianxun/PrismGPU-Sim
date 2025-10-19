#include "InputAssimble.h"
#include "Vram.hpp"

int EBOTriangleCount = 0;

int InputAssimble(const FrameTask& frameTask, TriangleAssemble& outTriangleA,
                  TriangleAssemble& outTriangleB,
                  TriangleAssemble& outTriangleC) {
    InputAssemble inputAssembler;

    std::vector<uint8_t> InAssembleBuffer;
    int MashEntriesaddrIA = frameTask.MashEntries;
    MashEntry MashentriDataIA;
    bool State = vram.read(MashEntriesaddrIA, InAssembleBuffer, sizeof(MashEntry));
    std::memcpy(&MashentriDataIA, InAssembleBuffer.data(), sizeof(MashEntry));
    if(!State)
    {
        LOG_ERROR("Mash Entry Read from VRAM Failed!");
        return -1;
    }   

    std::vector<uint8_t> EBOInAssembleBuffer;
    std::vector<Eigen::Vector3i> EBODInAssembleData;
    State = vram.read(MashentriDataIA.EBOOffset, EBOInAssembleBuffer, sizeof(Eigen::Vector3i) * MashentriDataIA.EBOCount);
    std::memcpy(&EBODInAssembleData, EBOInAssembleBuffer.data(), sizeof(Eigen::Vector3i) * MashentriDataIA.EBOCount);
    if(!State)
    {
        LOG_ERROR("EBO Data Read from VRAM Failed!");
        return -1;
    }

    std::vector<uint8_t> VBOInAssembleBuffer;
    std::vector<TestVBO>  VBODInAssembleData;
    State = vram.read(MashentriDataIA.VBOOffset, VBOInAssembleBuffer, sizeof(TestVBO) * MashentriDataIA.VBOCount);
    std::memcpy(&VBODInAssembleData, VBOInAssembleBuffer.data(), sizeof(TestVBO) * MashentriDataIA.VBOCount);
    if(!State)
    {
        LOG_ERROR("VBO Data Read from VRAM Failed!");
        return -1;
    }
    TriangleAssemble triAssemA, triAssemB, triAssemC; //这里明天在改，主要是要使用matrix4f，其次只保留一个就可以
    triAssemA.vertices.col(0) = VBODInAssembleData[EBODInAssembleData[EBOTriangleCount].x()].positions;
    triAssemA.vertices.col(1) = VBODInAssembleData[EBODInAssembleData[EBOTriangleCount].y()].positions;
    triAssemA.vertices.col(2) = VBODInAssembleData[EBODInAssembleData[EBOTriangleCount].z()].positions;

    triAssemB.vertices.col(0) = VBODInAssembleData[EBODInAssembleData[EBOTriangleCount+1].x()].positions;
    triAssemB.vertices.col(1) = VBODInAssembleData[EBODInAssembleData[EBOTriangleCount+1].y()].positions;
    triAssemB.vertices.col(2) = VBODInAssembleData[EBODInAssembleData[EBOTriangleCount+1].z()].positions;

    triAssemC.vertices.col(0) = VBODInAssembleData[EBODInAssembleData[EBOTriangleCount+2].x()].positions;
    triAssemC.vertices.col(1) = VBODInAssembleData[EBODInAssembleData[EBOTriangleCount+2].y()].positions;
    triAssemC.vertices.col(2) = VBODInAssembleData[EBODInAssembleData[EBOTriangleCount+2].z()].positions;

    EBOTriangleCount += 3;

    inputAssembler.loadTriangle(triAssemA, triAssemB, triAssemC);

    return inputAssembler.ToVertexShader(outTriangleA, outTriangleB, outTriangleC);
}