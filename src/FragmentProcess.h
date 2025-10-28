#ifndef FRAGMENTPROCESS_H
#define FRAGMENTPROCESS_H

#include "common.h"

struct InPixelData {
    //Eigen::Vector2i index ; 不需要传入用不到
    Eigen::Vector3f Word3DPoint ;
    Eigen::Vector3f Normal ;
};

int FragementShaderProcess(FrameTask &InFramTask, std::vector<RasterToPixel> &InRasterData);

class FragmentProcess{
    private:
        Eigen::Vector3f ka ;
        Eigen::Vector3f kd ;
        Eigen::Vector3f ks ;
        int Exp ;
        Eigen::Vector3i lightPos ;
        Eigen::Vector3i lightintensity ;
        Eigen::Vector3f ambientLight ;
        Eigen::Vector3i viewPos ;
        Eigen::Vector3f FragColor ;

    public:

        FragmentProcess() {
            ka = Eigen::Vector3f::Identity();
            kd = Eigen::Vector3f::Identity();
            ks = Eigen::Vector3f::Identity();
            lightPos = Eigen::Vector3i(10, 10, 10); //也是固定了
            lightintensity = Eigen::Vector3i(500, 500, 500);
            ambientLight = Eigen::Vector3f(10.0f, 10.0f, 10.0f); 
            viewPos = Eigen::Vector3i(0, 0, 10); //相当于固定相机位置 这也就是说相机位置也被固定了。
        };

        int LoadLightParam(Eigen::Vector3f Inka, Eigen::Vector3f Inkd, Eigen::Vector3f Inks, int InExp) {
            ka = Inka ;
            kd = Inkd ; //这个就算已经是采样器采样的到的数据了，或者是顶点插值后得到的颜色
            ks = Inks ;
            Exp = InExp ;
            return 0;
        }

        Eigen::Vector3f PixelShading(InPixelData &Data) {
            Eigen::Vector3f norm = Data.Normal.normalized();
            Eigen::Vector3f lightDir = (lightPos.cast<float>() - Data.Word3DPoint).normalized();
            Eigen::Vector3f viewDir = (viewPos.cast<float>() - Data.Word3DPoint).normalized();
            Eigen::Vector3f HalfVec = (lightDir + viewDir).normalized();

            float NdotL = norm.dot(lightDir);
            float NdotH = norm.dot(HalfVec);

            // Ambient
            Eigen::Vector3f ambient = ka.cwiseProduct(ambientLight);

            // Diffuse
            Eigen::Vector3f diffuse = kd.cwiseProduct(lightintensity.cast<float>()) * std::max(0.0f, NdotL);

            // Specular
            Eigen::Vector3f specular = ks.cwiseProduct(lightintensity.cast<float>()) * std::pow(std::max(0.0f, NdotH), Exp);

            FragColor = ambient + diffuse + specular;

            return FragColor * 255.0f;

        }


}; //光没有定义，服了，现在打算自己定义一个，但是实际上应该是和图形一起进行VS和光栅化的，不对，按道理来说只需要知道这个点就好了，但是这个点也是要进行MVP变换的

#endif // FRAGMENTPROCESS_H