#ifndef EDGEPLAYER_CONFIG_H
#define EDGEPLAYER_CONFIG_H

#include <string>

struct GlobalConfig
{
    GlobalConfig()
    {
        Verbose = false;
        GameMode = true;
        GcInterval = 1000;
    }

    bool Verbose;
    bool GameMode;
    int GcInterval;
};

extern GlobalConfig g_config;

namespace Config
{
    const unsigned kGameTerm = 1;    // 单位 s
    const unsigned kLatencyThreshold = 100; // 单位 ms

    // 任务的最短和最长耗时，单位 ms
    const unsigned kMinRequestTime = 30;
    const unsigned kMaxRequestTime = 150;

    // 任务的最小和最大存储占用，单位 MB
    const unsigned kMinRequestStorage = 10;
    const unsigned kMaxRequestStorage = 50;

    // proxy 节点获取服务器权重的周期，单位 s
    const unsigned kProxyUpdateServerWeightTime = kGameTerm;

    // 做一次本地资源管理的时间，ms
    const unsigned kGcTime = 100;
    // 一次本地资源管理的释放空间大小，MB
    const unsigned kGcSize = 30;
    // 默认的本地资源管理时间间隔，ms
    const unsigned kGcInterval = 1000;

    // 默认的限流参数，单位 QPS
    // 限流针对的是每台服务器
    const unsigned kDefaultRateLimit = 50;

    // 客户端发送请求的时间时隔，单位 ms
    const unsigned kRequestInterval = 20;

    // 存放实验数据的目录
    const std::string data_file_path = "/home/patric/data/";
}

#endif //EDGEPLAYER_CONFIG_H
