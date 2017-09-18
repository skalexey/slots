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

struct Timeout
{
    Timeout(std::function<void()> callback_to_do, int timeout_ms);
    std::function<void()> callback_to_do;
    int timeout_ms;
    std::chrono::steady_clock::time_point creation_time;
};

class Dispatcher
{
public:
    static Dispatcher& instance();
    void runAfter(std::function<void()> callback_to_do, int timeout_ms);
    void update();
private:
    std::vector<Timeout> _timeouts;
};

#endif /* Dispatcher_h */
