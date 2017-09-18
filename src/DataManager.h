//
//  DataManager.h
//  Slots_ios
//
//  Created by Alexey Skorokhodov on 17/09/2017.
//  Copyright © 2017 Mac. All rights reserved.
//

#ifndef DataManager_h
#define DataManager_h
#include <map>
#include "Slot.h"

typedef std::map<int, SlotInfo> slots_infos_t;

class DataManager
{
public:    
    static DataManager& instance();
    const SlotInfo& get_slot_info(int slot_index) const;
    const slots_infos_t& get_slots_infos();
    int calculateNewSlot();
    int getCoinsCount();
    void spendCoins(int bet_size);
private:
    DataManager();
    slots_infos_t _slots_infos;
    int _coins;
};

#endif /* DataManager_h */
