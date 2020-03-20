#include "Core/profiler.hpp"


/*----------------------------------------------------------------------------------*/

Instrumentor::~Instrumentor() {
    endSession();
}

void Instrumentor::beginSession(const std::string& name, const std::string& filepath) {
    if (mActiveSession) { 
        endSession(); 
    }
    mActiveSession = true;
    mOutputStream.open(filepath);
    writeHeader();
    mSessionName = name;
}
void Instrumentor::endSession() {
    if (!mActiveSession) { 
        return; 
    }
    mActiveSession = false;
    writeFooter();
    mOutputStream.close();
    mProfileCount = 0;
}

void Instrumentor::writeProfile(const ProfileResult& result) {
    std::lock_guard<std::mutex> lock(mLock);

    if (mProfileCount++ > 0)
        mOutputStream << ",";

    std::string name = result.name;
    std::replace(name.begin(), name.end(), '"', '\'');

    mOutputStream << "{";
    mOutputStream << "\"cat\":\"function\",";
    mOutputStream << "\"dur\":" << (result.end - result.start) << ',';
    mOutputStream << "\"name\":\"" << name << "\",";
    mOutputStream << "\"ph\":\"X\",";
    mOutputStream << "\"pid\":0,";
    mOutputStream << "\"tid\":" << result.thread_id << ",";
    mOutputStream << "\"ts\":" << result.start;
    mOutputStream << "}";

    mOutputStream.flush();
}
void Instrumentor::writeHeader() {
    mOutputStream << "{\"otherData\": {},\"traceEvents\":[";
    mOutputStream.flush();
}
void Instrumentor::writeFooter() {
    mOutputStream << "]}";
    mOutputStream.flush();
}

Instrumentor& Instrumentor::get() {
    static Instrumentor instance;
    return instance;
}

/*----------------------------------------------------------------------------------*/

InstrumentationTimer::InstrumentationTimer(const char* name)
    : mResult({ name, 0, 0, 0 }), mStopped(false) {
    mStartTimepoint = std::chrono::high_resolution_clock::now();
}
InstrumentationTimer::~InstrumentationTimer() {
    if (!mStopped) {
        stop();
    }
}

void InstrumentationTimer::stop() {
    auto end_timepoint = std::chrono::high_resolution_clock::now();

    mResult.start = std::chrono::time_point_cast<std::chrono::microseconds>(mStartTimepoint).time_since_epoch().count();
    mResult.end = std::chrono::time_point_cast<std::chrono::microseconds>(end_timepoint).time_since_epoch().count();
    mResult.thread_id = std::hash<std::thread::id>{}(std::this_thread::get_id());
    Instrumentor::get().writeProfile(mResult);

    mStopped = true;
}

/*----------------------------------------------------------------------------------*/
