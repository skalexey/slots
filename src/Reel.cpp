//
//  Reel.cpp
//  Slots_ios
//
//  Created by Alexey Skorokhodov on 17/09/2017.
//  Copyright © 2017 Mac. All rights reserved.
//
#include <math.h>
#include <cstdlib>
#include "Reel.h"
#include "DataManager.h"
#include "EventsController.h"

using namespace oxygine;

const double acceleration = 580.0;
const double max_speed = 2000.0;
const int slowdown_point_ms = 3000;
const int slowdown_time_ms = 6000;
const double blur_limit_speed = 900.0;

Reel::Reel(int size, Vector2 slot_size)
: _size(size)
, _slot_size(slot_size)
, _v(0.0)
, _v0(0.0)
, _ba(0.0)
, _spinning(false)
{
    setSize({slot_size.x, slot_size.y * _size});
    _inner_container = new Actor();
    Vector2 reel_size = getSize();
    _inner_container->setSize(reel_size.x, reel_size.y + _slot_size.y);
    addChild(_inner_container);
    for(int slot_index = 0; slot_index < _size + 1; slot_index++)
    {
        spSprite slot_sprite = new oxygine::Sprite();
        Slot slot(slot_sprite, slot_index);
        slot_sprite->setSize(_slot_size);
        slot_sprite->setPosition(0, slot_index * _slot_size.y);
        _inner_container->addChild(slot_sprite);
        _slots.push_back(slot);
    }
    
}

void Reel::pushInnerContainerUp()
{
    Vector2 container_position = _inner_container->getPosition();
    _inner_container->setPosition(container_position.x, container_position.y - _slot_size.y);
}

void Reel::resetInnerContainerPosition()
{
    _inner_container->setPosition(0, 0);
}

int Reel::getQueueSize()
{
    return (int)_slots_queue.size();
}



void Reel::pushSlot(int slot_index)
{
    _slots_queue.push_front(slot_index);
}

void Reel::pushRandomSlot()
{
    int slot_index = DataManager::instance().calculateNewSlot();
    pushSlot(slot_index);
}

void Reel::update(float delta_time)
{
    if(!_spinning)
    {
        return;
    }
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    int time_since_start = (int)std::chrono::duration_cast<std::chrono::milliseconds>(now - _start_spinning_time).count();
    
    double dt = 0;
    
    dt = delta_time / 1000;
    
    const Vector2& inner_container_position = _inner_container->getPosition();
    
    if(inner_container_position.y > 0.0 & getQueueSize() > 0)
    {
        pushInnerContainerUp();
        shiftSlotsDown();
        takeSlotFromQueue();
    }
    if(time_since_start < slowdown_point_ms)
    {
        _v += acceleration * dt;
        _v = fmin(_v, max_speed);
    }
    else
    {
        if(_v0 == 0)
        {
            _v0 = _v;
            double part1 = (double)getQueueSize() * _slot_size.y;
            double part2 = fabs(inner_container_position.y);
            double s_to_end = part1 + part2;
            _ba = _v0 * _v0 / 2.0 / s_to_end;
            double additional_ds = _v * dt / 2;
            _inner_container->setPosition(inner_container_position.x, inner_container_position.y + additional_ds);
        }
        if(_v > 0)
        {
            _v -= _ba * dt;
        }
        else
        {
            _v0 = 0;
            _ba = 0;
            _v = 0;
            _spinning = false;
            resetInnerContainerPosition();
        }
    }
    
    if(_v > blur_limit_speed)
    {
        blurIfNotBlured();
    }
    else
    {
        unblurIfBlured();
    }

    double ds = _v * dt;
    _inner_container->setPosition(inner_container_position.x, inner_container_position.y + ds);
}

bool Reel::isSpinning()
{
    return _spinning;
}

void Reel::unblurIfBlured()
{
    for(auto& slot : _slots)
    {
        slot.unblur();
    }
}

void Reel::blurIfNotBlured()
{
    for(auto& slot : _slots)
    {
        slot.blur();
    }
}

void Reel::shiftSlotsDown()
{
    for(int slot_index = (int)_slots.size() - 1; slot_index > 0; slot_index--)
    {
        Slot& slot = _slots[slot_index];
        slot = _slots[slot_index - 1];
    }
}

void Reel::takeSlotFromQueue()
{
    int slot_index = _slots_queue[_slots_queue.size() - 1];
    _slots_queue.pop_back();
    _slots[0].setSlot(slot_index);
}



void Reel::spin()
{
    resetInnerContainerPosition();
    while(getQueueSize() < 30)
    {
        pushRandomSlot();
    }
    _start_spinning_time = std::chrono::steady_clock::now();
    _spinning = true;
}
