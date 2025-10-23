#ifndef VERTEXPROCESS_H
#define VERTEXPROCESS_H

#include "common.h"

VSToRaster VertexShaderProcess(FrameTask &frameTask, IAToVertex &InTriangle);

class VertexProcess {
    private:
        Eigen::Matrix4f MVP;
        Eigen::Matrix4f MatrixSrcOne;
        Eigen::Matrix4f MatrixSrcTwo;
        Eigen::Matrix4f AheadProjectMatrix;
        Eigen::Matrix4f MatrixOut; 
        int VertexMatrixLoad;
    public:

        VertexProcess() {
            MVP = Eigen::Matrix4f::Identity();
            MatrixSrcOne = Eigen::Matrix4f::Identity();
            MatrixSrcTwo = Eigen::Matrix4f::Identity();
            AheadProjectMatrix = Eigen::Matrix4f::Identity();
            MatrixOut = Eigen::Matrix4f::Identity();
            VertexMatrixLoad = 0;
        };

        int MatrixMulProcess() {
            MatrixOut = MatrixSrcOne * MatrixSrcTwo;
            return 0;
        }

        Eigen::Matrix4f SFUMatrixInverse(Eigen::Matrix4f &InMatrix){
            return InMatrix.inverse();
        }

        Eigen::Matrix4f SFUMatrixTranslate(Eigen::Matrix4f &InMatrix){
            return InMatrix.transpose();
        }

        int MatrixLoad(Eigen::Matrix4f &matrix, std::string type) {
            if(type == "SrcOne") {
                MatrixSrcOne = matrix;
            } 
            else if(type == "SrcTwo") {
                MatrixSrcTwo = matrix;
            }
            else if(type == "AheadProject") {
                AheadProjectMatrix = matrix;
            }
            else if(type == "MVP") {
                MVP = matrix;
            }
            else {
                LOG_ERROR("VertexProcess Matrix Load Error: type not defined!");
                return -1;
            }
            return 0;
        }

        Eigen::Matrix4f MatrixRead(std::string type) {
        
            if(type == "MVP"){
                return MVP ;   
            }
            else if(type == "AheadProject"){
                return AheadProjectMatrix;
            }
            else{
                LOG_ERROR("VertexProcess Matrix Read Error: type not defined!");
                return Eigen::Matrix4f::Zero() ;
            }
        }

        int GetMatrix(Eigen::Matrix4f &outMatrix) {
            outMatrix = MatrixOut;
            return 0;
        }

        int SetVertexMatrixLoad(int Signal) {
            VertexMatrixLoad = Signal;
            return 0;
        }

        int GetLoadSignal() {
            return VertexMatrixLoad;
        }


    // int Clip () {
    //     if()
    // } 裁剪我打算临时添加到main函数中,在设计的时候忽略了

        //Homegeness->Screen
    Eigen::Matrix4f Homegeness2Screen (Eigen::Matrix4f InMVPProj, int Height, int Width, int Near, int Far){

        //homegeness divder
        Eigen::Matrix4f inM ;
        inM.col(0) << InMVPProj.col(0).x()/InMVPProj.col(0).w(), InMVPProj.col(0).y()/InMVPProj.col(0).w(), InMVPProj.col(0).z()/InMVPProj.col(0).w(), InMVPProj.col(0).w();
        inM.col(1) << InMVPProj.col(1).x()/InMVPProj.col(1).w(), InMVPProj.col(1).y()/InMVPProj.col(1).w(), InMVPProj.col(1).z()/InMVPProj.col(1).w(), InMVPProj.col(1).w();
        inM.col(2) << InMVPProj.col(2).x()/InMVPProj.col(2).w(), InMVPProj.col(2).y()/InMVPProj.col(2).w(), InMVPProj.col(2).z()/InMVPProj.col(2).w(), InMVPProj.col(2).w();
        inM.col(3) << InMVPProj.col(3);

        //NDC 2 screen
        Eigen::Matrix4f RetMatrix ;
        float f1 = (50-0.1) / 2.0 ;
        float f2 = (50+0.1) / 2.0;

        RetMatrix.col(0) << (0.5*Width*(inM.col(0).x()+1.0f)), (0.5*Height*(inM.col(0).y()+1.0f)), inM.col(0).z()*f1+f2,inM.col(0).w();
        RetMatrix.col(1) << (0.5*Width*(inM.col(1).x()+1.0f)), (0.5*Height*(inM.col(1).y()+1.0f)), inM.col(1).z()*f1+f2,inM.col(1).w();
        RetMatrix.col(2) << (0.5*Width*(inM.col(2).x()+1.0f)), (0.5*Height*(inM.col(2).y()+1.0f)), inM.col(2).z()*f1+f2,inM.col(2).w();
        RetMatrix.col(3) = inM.col(3);

        return RetMatrix; 
    }

};

#endif // VERTEXPROCESS_H