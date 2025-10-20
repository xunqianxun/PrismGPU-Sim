#include "VertexProcess.h"
#include "Vram.hpp"

    VSToRaster VertexShaderProcess(FrameTask &frameTask, IAToVertex &InAssmbData) {

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
                throw std::string("Mash Entry Read from VRAM Failed!");
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
            vertexProcessor.GetMatrix(TempOutMatrix);
            vertexProcessor.MatrixLoad(TempOutMatrix, "MVP");

            vertexProcessor.SetVertexMatrixLoad(true);

        }

        Eigen::Matrix4f MVPReault, NoProjectResult;
        Eigen::Matrix4f MVPSource, NoProjectSource;

        MVPSource = vertexProcessor.MatrixRead("MVP");
        vertexProcessor.MatrixLoad(MVPSource, "SrcOne");
        vertexProcessor.MatrixLoad(InAssmbData.TriVertexPos, "SrcTwo");
        vertexProcessor.MatrixMulProcess();
        vertexProcessor.GetMatrix(MVPReault);

        NoProjectSource = vertexProcessor.MatrixRead("AheadProject");
        vertexProcessor.MatrixLoad(NoProjectSource, "SrcOne");
        vertexProcessor.MatrixLoad(InAssmbData.TriVertexPos, "SrcTwo");
        vertexProcessor.MatrixMulProcess();
        vertexProcessor.GetMatrix(NoProjectResult);

        Eigen::Matrix4f InvertM, TransposeM;
        InvertM = vertexProcessor.SFUMatrixTranslate(NoProjectSource);
        TransposeM = vertexProcessor.SFUMatrixTranslate(InvertM);

        Eigen::Matrix4f Normal4f ;
        Normal4f.setZero();
        Normal4f.topLeftCorner<3,3>() = InAssmbData.Normal;

        vertexProcessor.MatrixLoad(TransposeM, "SrcOne");
        vertexProcessor.MatrixLoad(Normal4f, "SrcTwo");
        vertexProcessor.MatrixMulProcess(); //先这样，明天在改，还差没有把法线转换过来，以及组成先后传递的株距结构。

        VSToRaster VsResult ;

        VsResult.MVPtransfer = MVPReault ;
        VsResult.NoProject = NoProjectResult ;

        return VsResult;
    }