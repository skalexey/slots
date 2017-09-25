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
#include "TweenAnimRows.h"
#include "Utils.h"

using namespace oxygine;

extern Resources gameResources;
extern float ui_scale;

const double acceleration = 980.0;
const double max_speed = 1500.0;
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

void Reel::shadeSlot(int slot_index)
{
    Color color(0x00000055);
    spSprite slot_sprite = new oxygine::ColorRectSprite();
    slot_sprite->setColor(color);
    slot_sprite->setSize(_slot_size);
    slot_sprite->setPosition(0, slot_index * _slot_size.y);
    _inner_container->addChild(slot_sprite);
    _effects_nodes.push_back(slot_sprite);
}

void Reel::animateSlot(int slot_index)
{
    int symbol = _slots[slot_index].symbol_info.symbol;
    spSprite slot_sprite = new Sprite();
    _inner_container->addChild(slot_sprite);
    slot_sprite->setSize(_slot_size);
    slot_sprite->setAnchor(1, 1);
    slot_sprite->setScale(ui_scale * 0.75f);
    slot_sprite->setPosition(_slot_size.x, (slot_index + 1) * _slot_size.y);
    ResAnim* animation = gameResources.getResAnim((symbol == 0 ? "wild" : "tropicalisland") + (symbol == 0 ? std::string() : Utils::to_string(symbol)) + "_spritesheet");
    slot_sprite->addTween(TweenAnimRows(animation), 600, -1);
    _effects_nodes.push_back(slot_sprite);
}

void Reel::resetEffects()
{
    for(const spActor& effect_node : _effects_nodes)
    {
        effect_node->getParent()->removeChild(effect_node);
    }
    _effects_nodes.clear();
}

void Reel::setSlotSymbol(int slot_index, int symbol)
{
    _slots[slot_index].setSlotSymbol(symbol);
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

int Reel::getQueueSize() const
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

void Reel::_update(float delta_time)
{
    if(!_spinning)
    {
        return;
    }
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    int time_since_start = (int)std::chrono::duration_cast<std::chrono::milliseconds>(now - _start_spinning_time).count();
    
    double dt = delta_time / 1000;
    
    const Vector2& inner_container_position = _inner_container->getPosition();
    
    if(inner_container_position.y > 0.0 & getQueueSize() > 0)
    {
        int additional_times_to_shift = floor(inner_container_position.y / _slot_size.y);
        int additional_shifts_count = 0;
        do
        {
            pushInnerContainerUp();
            shiftSlotsDown();
            takeSlotFromQueue();
            additional_shifts_count++;
        }
        while(additional_shifts_count < additional_times_to_shift);
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
            double s_to_end = (double)getQueueSize() * _slot_size.y + fabs(inner_container_position.y);;
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

bool Reel::isSpinning() const
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
    _slots[0].setSlotSymbol(slot_index);
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
