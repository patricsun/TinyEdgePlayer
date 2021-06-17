#include "Server.h"

Server::Server(int cpu, int ram, int id)
    : cpu_(cpu + 1),        // 多出一个线程用来执行“本地资源管理"
        storage_(ram), id_(id),
        rate_limiter_(Config::kDefaultRateLimit)
{
    shutdown_ = false;

    // 本地资源管理
    cpu_.ExecuteTask([this] { GcFunc(); });
}


auto Server::Execute(Task t) -> std::future<bool>
{
    rate_limiter_.pass();

    return cpu_.template ExecuteTask([this, t]() -> bool {
        if (t.storage != 0)     // 对于纯计算型任务，跳过操作内存的操作
            storage_.Malloc(t.storage);

        std::this_thread::sleep_for(std::chrono::milliseconds(t.time));


        if (t.storage != 0)     // 对于纯计算型任务，跳过操作内存的操作
            storage_.Free(t.storage * 0.2);

        return true;
    });
}

void Server::Stop()
{
    shutdown_ = true;

    // 先停止博弈线程，以防cpu_结束后博弈线程去向它要东西
    if (game_thread_.joinable())
        game_thread_.join();

//    cpu_.Stop();
    cpu_.JoinAll();
}

std::string Server::GetStatusLogString_()
{
    std::string ret;
    ret += "server[" + std::to_string(id_) + "] - load:"  + std::to_string(GetCpuLoad())
                + ",queue:" + std::to_string(GetTaskQueueSize())
                + ",speed:" + std::to_string(GetCurrentSpeed());
    return ret;
}

void Server::PrintStatus()
{
    LOG(INFO) << GetStatusLogString_();
}


void Server::GcFunc()
{
    while (!shutdown_)
    {
        Execute(Task(Config::kGcTime, 0));
        storage_.Free(Config::kGcSize);
        LOG(INFO) << "Freed " << Config::kGcSize << "MB RAM";

        if (shutdown_)
            return;

        std::this_thread::sleep_for(std::chrono::milliseconds(g_config.GcInterval));
    }
}
