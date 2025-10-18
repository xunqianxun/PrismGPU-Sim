#include <iostream> 
#include "common.h"

enum Wave32fMulType{
    Wave32fMulSrc1=1,
    Wave32fMulSrc2=2
};

class Wave32fMulExe {
    private: 
        Eigen::VectorXf wavesrc1[32];
        Eigen::VectorXf wavesrc2[32];
        Eigen::VectorXf waveout[32];

public:
    bool  Wave32fExe(int activations) {
        for(int i=0; i<32; i++){
            waveout[i] = wavesrc1[i].cwiseProduct(wavesrc2[i]);
        }
        return true;
    }
    //矩阵的存放方式是首地址偏移行数乘列宽加列数
    bool Wave32RegLoad(Eigen::VectorXi *addr, int type) {
        if(type == Wave32fMulSrc1){
            for(int i=0; i<32; i++){
                wavesrc1[i] = addr[i].cast<float>();
            }
        } else if(type == Wave32fMulSrc2){
            for(int i=0; i<32; i++){
                wavesrc2[i] = addr[i].cast<float>();
            }
        } else {
            std::cout << "Wave32fMulExe Load Error: type not defined!" << std::endl;
            return false;
        }
        return true;
    }


};