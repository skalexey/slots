//
//  DataManager.cpp
//  Slots_ios
//
//  Created by Alexey Skorokhodov on 17/09/2017.
//  Copyright © 2017 Mac. All rights reserved.
//

#include <math.h>
#include <cstdlib>
#include <ctime>
#include <random>
#include "DataManager.h"
#include "Slot.h"
#include "EventsController.h"

DataManager& DataManager::instance()
{
    static DataManager _instance;
    return _instance;
}

const SymbolInfo& DataManager::getSymbolInfo(int slot_index) const
{
    return _symbols_infos.at(slot_index);
}

DataManager::DataManager()
{
    SymbolInfo symbol_info0(0, 1.5f, 0, 0, 0, "wild", "wild_b");
    SymbolInfo symbol_info1(1, 4.0f, 30, 120, 300, "tropicalisland1", "tropicalisland1_b");
    SymbolInfo symbol_info2(2, 4.0f, 30, 150, 450, "tropicalisland2", "tropicalisland2_b");
    SymbolInfo symbol_info3(3, 4.0f, 60, 300, 700, "tropicalisland3", "tropicalisland3_b");
    SymbolInfo symbol_info4(4, 4.0f, 100, 400, 1000, "tropicalisland4", "tropicalisland4_b");
    SymbolInfo symbol_info5(5, 3.5f, 150, 500, 1200, "tropicalisland5", "tropicalisland5_b");
    SymbolInfo symbol_info6(6, 3.0f, 250, 1000, 2000, "tropicalisland6", "tropicalisland6_b");
    SymbolInfo symbol_info7(7, 2.0f, 800, 3000, 7000, "tropicalisland7", "tropicalisland7_b");
    SymbolInfo symbol_info8(8, 1.8f, 1000, 5000, 10000, "tropicalisland8", "tropicalisland8_b");
    _symbols_infos[0] = symbol_info0;
    _symbols_infos[1] = symbol_info1;
    _symbols_infos[2] = symbol_info2;
    _symbols_infos[3] = symbol_info3;
    _symbols_infos[4] = symbol_info4;
    _symbols_infos[5] = symbol_info5;
    _symbols_infos[6] = symbol_info6;
    _symbols_infos[7] = symbol_info7;
    _symbols_infos[8] = symbol_info8;
    
    _coins = 100000;
}

void DataManager::appendCoins(int coins)
{
    _coins += coins;
}

int DataManager::getWinAmount(int symbol, int symbols_count) const
{
    const SymbolInfo& symbol_info = getSymbolInfo(symbol);
    return symbol_info.rewards[symbols_count - 3] * symbols_count;
}

int DataManager::getCoinsCount() const
{
    return _coins;
}

void DataManager::spendCoins(int bet_size)
{
    _coins -= bet_size;
    EventsController::instance().fireEvent("event.coins_changed");
}

int DataManager::calculateNewSlot() const
{
    float sum_of_weight = 0;
    for(const auto& it : _symbols_infos)
    {
        const SymbolInfo& symbol_info = it.second;
        sum_of_weight += symbol_info.weight;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sum_of_weight * 10);
    int rnd = dis(gen);
    int slot_number = 0;
    for(const auto& it : _symbols_infos)
    {
        const SymbolInfo& symbol_info = it.second;
        if(rnd < symbol_info.weight * 10)
        {
            slot_number = it.first;
            break;
        }
        rnd -= symbol_info.weight * 10;
    }
    return slot_number;
}

const symbols_infos_t& DataManager::getSymbolsInfos() const
{
    return _symbols_infos;
}
