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

typedef std::vector<std::vector<int> > slots_table_t;

struct SymbolInfo
{
    SymbolInfo();
    SymbolInfo(float weight, int reward1, int reward2, int reward3, std::string image_file_name, std::string image_blured_file_name);
    float weight;
    std::array<int, 3> rewards;
    std::string image_file_name;
    std::string image_blured_file_name;
};

struct Slot
{
    Slot(oxygine::spSprite p_sprite, int symbol);
    SymbolInfo slot_info;
    oxygine::spSprite getSprite();
    void operator=(Slot&);
    void setSlotSymbol(int symbol);
    void blur();
    void unblur();
private:
    oxygine::spSprite _sprite;
    bool _blured;
};

#endif /* Slot_h */
