//
//  Dispatcher.cpp
//  Slots_ios
//
//  Created by Alexey Skorokhodov on 18/09/2017.
//  Copyright © 2017 Mac. All rights reserved.
//

#include "Dispatcher.h"

Dispatcher& Dispatcher::instance()
{
    static Dispatcher _instance;
    return _instance;
}

void Dispatcher::runAfter(std::function<void()> callback_to_do, int timeout_ms)
{
    Timeout timeout(callback_to_do, timeout_ms);
    _timeouts.push_back(timeout);
}

void Dispatcher::update()
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    for(auto it = _timeouts.begin(); it != _timeouts.end();)
    {
        Timeout& timeout = *it;
        long time_since_start = std::chrono::duration_cast<std::chrono::milliseconds>(now - timeout.creation_time).count();
        if(time_since_start >= timeout.timeout_ms)
        {
            timeout.callback_to_do();
            it = _timeouts.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

Timeout::Timeout(std::function<void()> callback_to_do, int timeout_ms)
: callback_to_do(callback_to_do)
, timeout_ms(timeout_ms)
{
    creation_time = std::chrono::steady_clock::now();
}
