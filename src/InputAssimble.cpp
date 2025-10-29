#include "InputAssimble.h"

int EBOTriangleCount = 0;

IAToVertex InputAssimble(const FrameTask& frameTask) {
    InputAssemble inputAssembler;

    IAToVertex AmbGroup ;

    std::vector<uint8_t> InAssembleBuffer;
    int MashEntriesaddrIA = frameTask.MashEntries;
    MashEntry MashentriDataIA;
    bool State = vram.read(MashEntriesaddrIA, InAssembleBuffer, sizeof(MashEntry));
    std::memcpy(&MashentriDataIA, InAssembleBuffer.data(), sizeof(MashEntry));
    if(!State)
    {
        LOG_ERROR("Mash Entry Read from VRAM Failed!");
        throw std::string("Mash Entry Read from VRAM Failed!");
    }   

    std::vector<uint8_t> EBOInAssembleBuffer;
    std::vector<Eigen::Vector3i> EBODInAssembleData;
    State = vram.read(MashentriDataIA.EBOOffset, EBOInAssembleBuffer, sizeof(Eigen::Vector3i) * MashentriDataIA.EBOCount);
    std::memcpy(&EBODInAssembleData, EBOInAssembleBuffer.data(), sizeof(Eigen::Vector3i) * MashentriDataIA.EBOCount);
    if(!State)
    {
        LOG_ERROR("EBO Data Read from VRAM Failed!");
        throw std::string("EBO Data Read from VRAM Failed!");
    }

    std::vector<uint8_t> VBOInAssembleBuffer;
    std::vector<TestVBO>  VBODInAssembleData;
    State = vram.read(MashentriDataIA.VBOOffset, VBOInAssembleBuffer, sizeof(TestVBO) * MashentriDataIA.VBOCount);
    std::memcpy(&VBODInAssembleData, VBOInAssembleBuffer.data(), sizeof(TestVBO) * MashentriDataIA.VBOCount);
    if(!State)
    {
        LOG_ERROR("VBO Data Read from VRAM Failed!");
        throw std::string("VBO Data Read from VRAM Failed!");
    }
    Eigen::Vector4f ZeroVec = {0.0f,0.0f,0.0f,0.0f};
    TriangleAssemble triAssem; 
    triAssem.col(0) = (VBODInAssembleData[EBODInAssembleData[EBOTriangleCount].x()].positions).homogeneous();
    triAssem.col(1) = (VBODInAssembleData[EBODInAssembleData[EBOTriangleCount].y()].positions).homogeneous();
    triAssem.col(2) = (VBODInAssembleData[EBODInAssembleData[EBOTriangleCount].z()].positions).homogeneous();
    triAssem.col(3) = ZeroVec;

    Eigen::Matrix3f TriNormal ;
    TriNormal.col(0) = (VBODInAssembleData[EBODInAssembleData[EBOTriangleCount].x()].normals);
    TriNormal.col(1) = (VBODInAssembleData[EBODInAssembleData[EBOTriangleCount].y()].normals);
    TriNormal.col(2) = (VBODInAssembleData[EBODInAssembleData[EBOTriangleCount].z()].normals);

    Eigen::Matrix3f TriColor ;
    TriColor.col(0) = (VBODInAssembleData[EBODInAssembleData[EBOTriangleCount].x()].colors);
    TriColor.col(1) = (VBODInAssembleData[EBODInAssembleData[EBOTriangleCount].y()].colors);
    TriColor.col(2) = (VBODInAssembleData[EBODInAssembleData[EBOTriangleCount].z()].colors);

    std::array<Eigen::Vector2f, 3> TriTexCoord ;
    TriTexCoord[0] =  (VBODInAssembleData[EBODInAssembleData[EBOTriangleCount].x()].texcoords);
    TriTexCoord[1] =  (VBODInAssembleData[EBODInAssembleData[EBOTriangleCount].y()].texcoords);
    TriTexCoord[2] =  (VBODInAssembleData[EBODInAssembleData[EBOTriangleCount].z()].texcoords);

    EBOTriangleCount += 1;


    AmbGroup.TriVertexPos = triAssem;
    AmbGroup.Color = TriColor ;
    AmbGroup.Normal = TriNormal ;
    AmbGroup.TexCoord = TriTexCoord ;


    inputAssembler.loadAmb(AmbGroup);

    return inputAssembler.ReadAmb();
}