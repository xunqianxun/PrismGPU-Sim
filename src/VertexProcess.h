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

};

#endif // VERTEXPROCESS_H