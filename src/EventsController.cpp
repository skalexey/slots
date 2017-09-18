//
//  EventsManager.cpp
//  Slots_ios
//
//  Created by Alexey Skorokhodov on 16/09/2017.
//  Copyright © 2017 Mac. All rights reserved.
//

#include "EventsController.h"

EventsController& EventsController::instance()
{
    static EventsController _instance;
    return _instance;
}

EventsController::EventsController()
{

}

void EventsController::unsubscribeFromEvent(const std::string& event_name)
{
    auto range = _events_callbacks.equal_range(event_name);
    if(range.first == _events_callbacks.end())
    {
        return;
    }
    _events_callbacks.erase(range.first, range.second);
}

void EventsController::subscribeToEvent(const std::string& event_name, std::function<void()> callback)
{
    _events_callbacks.insert(std::pair<std::string, std::function<void()> >(event_name, callback));
}
void EventsController::fireEvent(const std::string& event_name)
{
    auto range = _events_callbacks.equal_range(event_name);
    if(range.first == _events_callbacks.end())
    {
        return;
    }
    for(auto it = range.first; it != range.second; ++it)
    {
        it->second();
    }
}
