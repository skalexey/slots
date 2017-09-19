//
//  Dispatcher.h
//  Slots_ios
//
//  Created by Alexey Skorokhodov on 18/09/2017.
//  Copyright © 2017 Mac. All rights reserved.
//

#ifndef Dispatcher_h
#define Dispatcher_h

#include <functional>
#include <vector>
#include <utility>
#include <chrono>
#include <unordered_map>

struct Timeout
{
    Timeout(){}
    Timeout(std::function<void()> callback_to_do, int timeout_ms, bool repeat);
    std::function<void()> callback_to_do;
    int timeout_ms;
    std::chrono::steady_clock::time_point creation_time;
    bool _repeat;
    bool isInterval();
};

class Dispatcher
{
public:
    static Dispatcher& instance();
    int runAfter(std::function<void()> callback_to_do, int timeout_ms);
    int runAndRepeatAfter(std::function<void()> callback_to_do, int timeout_ms);
    void update();
    void cancel(int id);
private:
    int generateId();
    std::unordered_map<int, Timeout> _timeouts;
};

#endif /* Dispatcher_h */
