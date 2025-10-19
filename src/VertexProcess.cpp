#include "VertexProcess.h"
#include "Vram.hpp"

    int VertexShaderProcess(FrameTask &frameTask,
                            TriangleAssemble &inTriangleA,
                            TriangleAssemble &inTriangleB,
                            TriangleAssemble &inTriangleC) {

        VertexProcess vertexProcessor;
        if(vertexProcessor.GetLoadSignal() == 0){


            int MashEntriesaddrVS = frameTask.MashSetups;
            std::vector<uint8_t> InAssembleBuffer;
            MashSetup MashentriDataVS;
            bool State = vram.read(MashEntriesaddrVS, InAssembleBuffer, sizeof(MashSetup));
            std::memcpy(&MashentriDataVS, InAssembleBuffer.data(), sizeof(MashSetup));
            if(!State)
            {
                LOG_ERROR("Mash Entry Read from VRAM Failed!");
                return -1;
            }   

            vertexProcessor.MatrixLoad(MashentriDataVS.RotateMatrix, "SrcOne");
            vertexProcessor.MatrixLoad(MashentriDataVS.ScaleVector, "SrcTwo");
            vertexProcessor.MatrixMulProcess();

            vertexProcessor.MatrixLoad(MashentriDataVS.TranslateVector, "SrcOne");
            Eigen::Matrix4f TempOutMatrix;
            vertexProcessor.GetMatrix(TempOutMatrix);
            vertexProcessor.MatrixLoad(TempOutMatrix, "SrcTwo");
            vertexProcessor.MatrixMulProcess();

            vertexProcessor.MatrixLoad(MashentriDataVS.ViewMatrix, "SrcOne");
            vertexProcessor.GetMatrix(TempOutMatrix);
            vertexProcessor.MatrixLoad(TempOutMatrix, "SrcTwo");
            vertexProcessor.MatrixMulProcess();

            vertexProcessor.GetMatrix(TempOutMatrix);
            vertexProcessor.MatrixLoad(TempOutMatrix, "AheadProject");

            vertexProcessor.MatrixLoad(MashentriDataVS.ProjectionMatrix, "SrcOne");
            //vertexProcessor.GetMatrix(TempOutMatrix);
            vertexProcessor.MatrixLoad(TempOutMatrix, "SrcTwo");
            vertexProcessor.MatrixMulProcess();

            vertexProcessor.SetVertexMatrixLoad(true);

        }

        

        return 0;
    }