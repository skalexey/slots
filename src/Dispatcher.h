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
    Timeout(std::function<void()> callback_to_do, int timeout_ms);
    std::function<void()> callback_to_do;
    int timeout_ms;
    std::chrono::steady_clock::time_point creation_time;
};

struct Interval
{
    Interval(){}
    Interval(std::function<void()> callback_to_do, int timeout_ms);
    std::function<void()> callback_to_do;
    int timeout_ms;
    std::chrono::steady_clock::time_point last_cycle_time;
};

class Dispatcher
{
public:
    static Dispatcher& instance();
    int runAfter(std::function<void()> callback_to_do, int timeout_ms);
    int runAndRepeatAfter(std::function<void()> callback_to_do, int timeout_ms);
    void update();
    void cancelInterval(int id);
    void cancelAll();
private:
    int generateId() const;
    std::unordered_map<int, Timeout> _timeouts;
    std::unordered_map<int, Interval> _intervals;
};

#endif /* Dispatcher_h */
