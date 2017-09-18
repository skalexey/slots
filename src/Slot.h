//
//  Slot.h
//  Slots_ios
//
//  Created by Alexey Skorokhodov on 17/09/2017.
//  Copyright © 2017 Mac. All rights reserved.
//

#ifndef Slot_h
#define Slot_h

#include <array>
#include "oxygine-framework.h"

struct SlotInfo
{
    SlotInfo();
    SlotInfo(float weight, std::array<int, 3> rewards, std::string image_file_name, std::string image_blured_file_name);
    float weight;
    std::array<int, 3> rewards;
    std::string image_file_name;
    std::string image_blured_file_name;
};

struct Slot
{
    Slot(oxygine::spSprite p_sprite, int slot_index);
    SlotInfo slot_info;
    oxygine::spSprite getSprite();
    void operator=(Slot&);
    void setSlot(int slot_index);
    void blur();
    void unblur();
private:
    oxygine::spSprite _sprite;
    bool _blured;
};

#endif /* Slot_h */
