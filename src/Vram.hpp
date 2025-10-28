#include <assert.h>
#include <cstdint>
#include "common.h"


#define FrambufferStart 1024 * 1024 * 64 - (WIDTH * HEIGHT * 12)
#define ZbufferStart FrambufferStart - (WIDTH * HEIGHT * 4) - 4

struct TestVBO {
    Eigen::Vector3f positions;
    Eigen::Vector3f normals;
    Eigen::Vector3f colors;
    Eigen::Vector2f texcoords;
};

struct MashEntry //从某种意义上来说它本身就是VAO数据,只不过存储了个更多的信息
{
    int TopologyType; 
    int EBOOffset;
    int EBOCount; //之所以有count的目的还是为了规定每一个部分的内存范围
    int VBOOffset;
    int VBOAtribute; //每个顶点属性的大小
    int VBOCount;
    Eigen::Vector3f AmbientColor; //环境光
    Eigen::Vector3f DiffuseColor; //漫反射颜色光照下的主体颜色，搞错了，这个不应该在这里的他在有纹理的时候是纹理来决定，没有的时候是由颜色插值来决定
    Eigen::Vector3f SpecularColor; //镜面反射颜色也就是高光
    float SpecularExponent; //焯，应该是int呀，脑抽
    int TEXOffset;
    int BUMPOffset; //还有很多关于光线追踪和材质的信息没有加，等待进一步的设计增加
};

struct MashSetup
{
    Eigen::Matrix4f RotateMatrix; //用于物体的旋转
    Eigen::Matrix4f TranslateVector; //用于物体的平移
    Eigen::Matrix4f ScaleVector; //用于物体的缩放
    Eigen::Matrix4f ViewMatrix; //用于摄像机的视角转换
    Eigen::Matrix4f ProjectionMatrix; //用于投影转换
};

struct FrameTask
{
    int MashSetups; //本身应该是一个指针队列因为一个帧渲染任务会有很多的Mesh 
    int MashEntries;
};

class Vram
{
    private:
        std::vector<uint8_t> memory;
        const size_t memory_size = 1024 * 1024 * 64; // 64MB VRAM
        int UsedAddr = 0;
    public:
        Vram() : memory(memory_size, 0) {}

        int GetUsedAddr() const {
            return UsedAddr;
        }

        bool write( int addr, const std::vector<uint8_t>& data) 
        {
            if (addr < 0 || addr + data.size() > memory_size) {
                throw std::string("VRAM write out of bounds");  
                return false; // Address out of bounds
            }
            std::copy(data.begin(), data.end(), memory.begin() + addr);
            UsedAddr = addr + static_cast<int>(data.size());
            return true;
        }

        bool read(int addr, std::vector<uint8_t>& data, size_t size)
        {
            if(addr <0 || addr + size > memory_size){
                throw std::string("VRAM read out of bounds");
                return false; // Address out of bounds
            }
            data.resize(size);
            std::copy(memory.begin() + addr, memory.begin() + addr + size, data.begin());
            return true;
        }
          
        
 };

 Vram vram;

 FrameTask FrameOne;