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

        Eigen::Matrix4f NormalMatrix ;
        NormalMatrix.setZero();
        NormalMatrix.topLeftCorner<3,3>() = TransposeM.topLeftCorner<3,3>();

        Eigen::Matrix4f Normal4f ;
        Normal4f.setZero();
        Normal4f.topLeftCorner<3,3>() = InAssmbData.Normal;

        vertexProcessor.MatrixLoad(NormalMatrix, "SrcOne");
        vertexProcessor.MatrixLoad(Normal4f, "SrcTwo");
        vertexProcessor.MatrixMulProcess(); 
        vertexProcessor.GetMatrix(NoProjectResult);
        Eigen::Matrix4f NormalResult ;
        vertexProcessor.GetMatrix(NormalResult);

        VSToRaster VsResult ;
        Eigen::Matrix4f VertexPos;
        VertexPos = vertexProcessor.Homegeness2Screen(MVPReault, HEIGHT, WIDTH, NEAR, FAR);

        VsResult.TriVertexPos = VertexPos ;
        VsResult.TriVertexNoProj = NoProjectResult ;
        VsResult.Normal = NormalResult.topLeftCorner<3,3>();
        VsResult.TexCoord = InAssmbData.TexCoord ;
        VsResult.Color = InAssmbData.Color ;

        return VsResult;
    }