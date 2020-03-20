#pragma once

#include "Core/std_types.hpp"

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <thread>
#include <mutex>


/*----------------------------------------------------------------------------------*/

#define PROFILE_BEGIN_SESSION(name, path) Instrumentor::get().beginSession(name, path)
#define PROFILE_END_SESSION() Instrumentor::get().endSession()
#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)

/*----------------------------------------------------------------------------------*/

struct ProfileResult {
    const std::string name;
    s64 start, end;
    u32 thread_id;
};

/*----------------------------------------------------------------------------------*/

class Instrumentor {
private:
    std::string mSessionName = "None";
    std::ofstream mOutputStream;
    int mProfileCount = 0;
    std::mutex mLock;
    bool mActiveSession = false;


public:
    ~Instrumentor();

    void beginSession(const std::string& name, const std::string& filepath = "../ExeDirectory/profile.json");
    void endSession();

    void writeProfile(const ProfileResult& result);
    void writeHeader();
    void writeFooter();

    static Instrumentor& get();
};

/*----------------------------------------------------------------------------------*/

class InstrumentationTimer {
private:
    ProfileResult mResult;
    std::chrono::time_point<std::chrono::high_resolution_clock> mStartTimepoint;
    bool mStopped;

public:
    InstrumentationTimer(const char* name);
    ~InstrumentationTimer();

    void stop();
};

/*----------------------------------------------------------------------------------*/
