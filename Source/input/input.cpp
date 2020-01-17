#include "input.h"



namespace
{
    std::unordered_map<input::Token, input::Callback> gCallbacks;
}

input::Token input::registerCallback(input::Callback callback) {
    int token = 0;
    gCallbacks.emplace(token, callback);
    return token;
}

bool input::unregisterCallback(input::Token token) {
    auto search = gCallbacks.find(token);
    if (search != gCallbacks.end()) {
        gCallbacks.erase(search);
        return true;
    }
    return false;
}

void input::foo(){
    
}

