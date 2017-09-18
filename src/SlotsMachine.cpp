//
//  SlotsMachine.cpp
//  Slots_ios
//
//  Created by Alexey Skorokhodov on 17/09/2017.
//  Copyright © 2017 Mac. All rights reserved.
//

#include <algorithm>
#include "SlotsMachine.h"
#include "Dispatcher.h"
#include "DataManager.h"
#include "EventsController.h"

using namespace oxygine;

const int delay_between_reels_ms = 200;
const int spinning_time_ms = 7000;
const int calculation_time_point = 3000;

SlotsMachine::SlotsMachine(int reel_size, int reels_count, const Vector2& slot_size)
: _reel_size(reel_size)
, _reels_count(reels_count)
{
    setSize({reels_count * slot_size.x, reel_size * slot_size.y});
    initReels(slot_size);
    initVirtualStop();
}

bool SlotsMachine::checkCoins()
{
    return DataManager::instance().getCoinsCount() >= bet_size;
}

void SlotsMachine::spin()
{
    if(!checkCoins())
    {
        EventsController::instance().fireEvent("event.spin_end");
        return;
    }
    DataManager::instance().spendCoins(bet_size);
    _start_spinning_time = std::chrono::steady_clock::now();
    _spinning = true;
    for(int reel_index = 0; reel_index < _reels.size(); reel_index++)
    {
        spReel reel = _reels[reel_index];
        Dispatcher::instance().runAfter([=]{
           reel->spin();
        }, reel_index * delay_between_reels_ms);
    }
    Dispatcher::instance().runAfter([=]{
        calculateVirtualStop();
        pushVirtualStop();
    }, calculation_time_point);
}

void SlotsMachine::pushVirtualStop()
{
    for(int reel_index = 0; reel_index < _reels_count; reel_index++)
    {
        spReel& reel = _reels[reel_index];
        std::vector<int>& virtual_stop_reel = _virtual_stop[reel_index];
        for(const auto& slot : virtual_stop_reel)
        {
            reel->pushSlot(slot);
        }
    }
}

void SlotsMachine::calculateVirtualStop()
{
    std::vector<bool> wilds_on_reels(_reels_count, false);
    for(int reel_index = 0; reel_index < _reels_count; reel_index++)
    {
        std::vector<int>& reel = _virtual_stop[reel_index];
        for(auto& slot : reel)
        {
            slot = DataManager::instance().calculateNewSlot();
            if(slot == 0) // wild
            {
                if(std::count(wilds_on_reels.begin(), wilds_on_reels.end(), true) < _reels_count)
                {
                    wilds_on_reels[reel_index] = true;
                }
                else
                {
                    do
                    {
                        slot = DataManager::instance().calculateNewSlot();
                    }
                    while(slot == 0);
                }
            }
        }
    }
}

void SlotsMachine::initVirtualStop()
{
    for(int reel_index = 0; reel_index < _reels_count; reel_index++)
    {
        std::vector<int> reel(_reel_size, 0);
        _virtual_stop.push_back(reel);
    }
}

void SlotsMachine::initReels(const Vector2& slot_size)
{
    for(int reel_index = 0; reel_index < _reels_count; reel_index++)
    {
        spReel new_reel = new Reel(_reel_size, slot_size);
        _reels.push_back(new_reel);
        new_reel->setPosition({reel_index * slot_size.x, 0});
        addChild(new_reel);
    }
}

void SlotsMachine::update(float delta_time)
{
    bool reels_stopped = true;
    for(spReel& reel : _reels)
    {
        reel->update(delta_time);
        if(reel->isSpinning())
        {
            reels_stopped = false;
        }
    }
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    int time_since_spin_begin = (int)std::chrono::duration_cast<std::chrono::milliseconds>(now - _start_spinning_time).count();
    if(_spinning && reels_stopped && time_since_spin_begin > 200)
    {
        _spinning = false;
        EventsController::instance().fireEvent("event.spin_end");
    }
}
