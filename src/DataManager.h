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

typedef std::map<int, SymbolInfo> symbols_infos_t;

class DataManager
{
public:    
    static DataManager& instance();
    const SymbolInfo& getSymbolInfo(int slot_index) const;
    const symbols_infos_t& getSymbolsInfos() const;
    int calculateNewSlot() const;
    int getCoinsCount() const;
    void spendCoins(int bet_size);
    void appendCoins(int coins);
    int getWinAmount(int symbol, int symbols_count) const;
private:
    DataManager();
    symbols_infos_t _symbols_infos;
    int _coins;
};

#endif /* DataManager_h */
