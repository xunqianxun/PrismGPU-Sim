#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <mutex>

//TODO: chiose do you want print log to console
#define LOG_TO_CONSOLE

class Logger {
private:
    std::ofstream file;
    std::mutex mtx;

public:
    enum Level { INFO, WARNING, ERROR, DEBUG };

    Logger(const std::string& filename = "../log.txt") {
        file.open(filename, std::ios::out | std::ios::app); //写入追加模式
    }

    ~Logger() { file.close(); }

    void log(Level level, const std::string& func, const std::string& msg) {
        std::lock_guard<std::mutex> lock(mtx); //是一个模板类，用于自动管理互斥锁的锁定和释放，为了保证数据的有序性，但是会降低性能，所以选择性的开启DEBUG模式
        std::stringstream ss;
        ss << "[" << timestamp() << "] "
           << "[" << levelToStr(level) << "] "
           << "[" << func << "] "
           << msg << std::endl;
        file << ss.str();
#ifdef LOG_TO_CONSOLE
        std::cout << ss.str();
#endif
    }

    static std::string levelToStr(Level l) {
        switch (l) {
            case INFO: return "INFO";
            case WARNING: return "WARN";
            case ERROR: return "ERR";
            case DEBUG: return "DBG";
        }
        return "";
    }

    static std::string timestamp() {
        time_t now = time(nullptr);
        char buf[32];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        return buf;
    }
};

#define LOG_INFO(msg)    logger.log(Logger::INFO,    __FUNCTION__, msg)
#define LOG_WARN(msg)    logger.log(Logger::WARNING, __FUNCTION__, msg)
#define LOG_ERROR(msg)   logger.log(Logger::ERROR,   __FUNCTION__, msg)
#define LOG_DEBUG(msg)   logger.log(Logger::DEBUG,   __FUNCTION__, msg)

extern Logger logger;

