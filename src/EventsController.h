//
//  EventsManager.h
//  Slots_ios
//
//  Created by Alexey Skorokhodov on 16/09/2017.
//  Copyright © 2017 Mac. All rights reserved.
//

#ifndef EventsManager_h
#define EventsManager_h

#include <stdio.h>
#include <string>
#include <map>
#include "oxygine-framework.h"

class EventsController
{
private:
    EventsController();
public:
    void subscribeToEvent(const std::string& event_name, std::function<void()>);
    void unsubscribeFromEvent(const std::string& event_name);
    void fireEvent(const std::string& event_name);
    static EventsController& instance();
private:
    std::multimap<std::string, std::function<void()> > _events_callbacks;
};

#endif /* EventsManager_h */
