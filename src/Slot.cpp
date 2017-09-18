//
//  Slot.cpp
//  Slots_ios
//
//  Created by Alexey Skorokhodov on 17/09/2017.
//  Copyright © 2017 Mac. All rights reserved.
//

#include "Slot.h"
#include "DataManager.h"

extern oxygine::Resources gameResources;

Slot::Slot(oxygine::spSprite p_sprite, int slot_index)
: _sprite(p_sprite)
, _blured(false)
{
    setSlot(slot_index);
}

void Slot::setSlot(int slot_index)
{
    slot_info = DataManager::instance().get_slot_info(slot_index);
    _sprite->setResAnim(gameResources.getResAnim(_blured ? slot_info.image_blured_file_name : slot_info.image_file_name));
}

SlotInfo::SlotInfo()
: weight(0)
{
    
}

SlotInfo::SlotInfo(float weight, std::array<int, 3> rewards, std::string image_file_name, std::string image_blured_file_name)
: weight(weight)
, image_file_name(image_file_name)
, image_blured_file_name(image_blured_file_name)
{
    
}

oxygine::spSprite Slot::getSprite()
{
    return _sprite;
}

void Slot::operator=(Slot& new_slot)
{
    slot_info = new_slot.slot_info;
    _sprite->setResAnim(gameResources.getResAnim(_blured ? slot_info.image_blured_file_name : slot_info.image_file_name));
}

void Slot::blur()
{
    if(_blured)
    {
        return;
    }
    _sprite->setResAnim(gameResources.getResAnim(slot_info.image_blured_file_name));
    _blured = true;
}

void Slot::unblur()
{
    if(!_blured)
    {
        return;
    }
    _sprite->setResAnim(gameResources.getResAnim(slot_info.image_file_name));
    _blured = false;
}
