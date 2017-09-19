//
//  Dispatcher.cpp
//  Slots_ios
//
//  Created by Alexey Skorokhodov on 18/09/2017.
//  Copyright © 2017 Mac. All rights reserved.
//

#include <math.h>
#include <cstdlib>
#include <ctime>
#include <random>
#include "Dispatcher.h"

Dispatcher& Dispatcher::instance()
{
    static Dispatcher _instance;
    return _instance;
}

int Dispatcher::runAfter(std::function<void()> callback_to_do, int timeout_ms)
{
    Timeout timeout(callback_to_do, timeout_ms, false);
    int id = generateId();
    _timeouts[id] = timeout;
    return id;
}

int Dispatcher::runAndRepeatAfter(std::function<void()> callback_to_do, int timeout_ms)
{
    callback_to_do();
    Timeout timeout(callback_to_do, timeout_ms, true);
    int id = generateId();
    _timeouts[id] = timeout;
    return id;
}

void Dispatcher::cancel(int id)
{
    _timeouts.erase(id);
}

int Dispatcher::generateId()
{
    int id = 0;
    do
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 0xFFFFFFFF);
        id = dis(gen);
    }
    while(_timeouts.find(id) != _timeouts.end() && id == 0);
    return id;
}

void Dispatcher::update()
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    for(auto it = _timeouts.begin(); it != _timeouts.end();)
    {
        Timeout& timeout = it->second;
        long time_since_start = std::chrono::duration_cast<std::chrono::milliseconds>(now - timeout.creation_time).count();
        if(time_since_start >= timeout.timeout_ms)
        {
            timeout.callback_to_do();
            if(!timeout.isInterval())
            {
                it = _timeouts.erase(it);
            }
            else
            {
                timeout.creation_time = now;
            }
        }
        else
        {
            ++it;
        }
    }
}

Timeout::Timeout(std::function<void()> callback_to_do, int timeout_ms, bool _repeat)
: callback_to_do(callback_to_do)
, timeout_ms(timeout_ms)
, _repeat(false)
{
    creation_time = std::chrono::steady_clock::now();
}

bool Timeout::isInterval()
{
    return _repeat;
}
