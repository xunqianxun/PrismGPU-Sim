#include <vector>
#include <assert.h>
#include <cstdint>
#include <eigen3/Eigen/Dense>

struct MashEntry
{
    int TopologyType; 
    int EBOOffset;
    Eigen::Vector3f AmbientColor;
    Eigen::Vector3f DiffuseColor;
    Eigen::Vector3f SpecularColor;
    float SpecularExponent;
    int TEXOffset;
    int BUMPOffset; //还有很多关于光线追踪和材质的信息没有加，等待进一步的设计增加
};


struct FrameTesk
{
    int TaskStartAddr;
    std::vector<MashEntry> MashEntries;
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

        bool write( int addr, const std::vector<uint8_t>& data) {
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