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
    SymbolInfo slot_info0(1.5f, 0, 0, 0, "wild", "wild_b");
    SymbolInfo slot_info1(4.0f, 30, 120, 300, "tropicalisland1", "tropicalisland1_b");
    SymbolInfo slot_info2(4.0f, 30, 150, 450, "tropicalisland2", "tropicalisland2_b");
    SymbolInfo slot_info3(4.0f, 60, 300, 700, "tropicalisland3", "tropicalisland3_b");
    SymbolInfo slot_info4(4.0f, 100, 400, 1000, "tropicalisland4", "tropicalisland4_b");
    SymbolInfo slot_info5(3.5f, 150, 500, 1200, "tropicalisland5", "tropicalisland5_b");
    SymbolInfo slot_info6(3.0f, 250, 1000, 2000, "tropicalisland6", "tropicalisland6_b");
    SymbolInfo slot_info7(2.0f, 800, 3000, 7000, "tropicalisland7", "tropicalisland7_b");
    SymbolInfo slot_info8(1.8f, 1000, 5000, 10000, "tropicalisland8", "tropicalisland8_b");
    _symbols_infos[0] = slot_info0;
    _symbols_infos[1] = slot_info1;
    _symbols_infos[2] = slot_info2;
    _symbols_infos[3] = slot_info3;
    _symbols_infos[4] = slot_info4;
    _symbols_infos[5] = slot_info5;
    _symbols_infos[6] = slot_info6;
    _symbols_infos[7] = slot_info7;
    _symbols_infos[8] = slot_info8;
    
    _coins = 100000;
}

void DataManager::appendCoins(int coins)
{
    _coins += coins;
}

int DataManager::getWinAmount(int symbol, int symbols_count)
{
    const SymbolInfo& slot_info = getSymbolInfo(symbol);
    return slot_info.rewards[symbols_count - 3];
}

int DataManager::getCoinsCount()
{
    return _coins;
}

void DataManager::spendCoins(int bet_size)
{
    _coins -= bet_size;
    EventsController::instance().fireEvent("event.coins_changed");
}

int DataManager::calculateNewSlot()
{
    float sum_of_weight = 0;
    for(const auto& it : _symbols_infos)
    {
        const SymbolInfo& slot_info = it.second;
        sum_of_weight += slot_info.weight;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sum_of_weight * 10);
    int rnd = dis(gen);
    int slot_number = 0;
    for(const auto& it : _symbols_infos)
    {
        const SymbolInfo& slot_info = it.second;
        if(rnd < slot_info.weight * 10)
        {
            slot_number = it.first;
            break;
        }
        rnd -= slot_info.weight * 10;
    }
    return slot_number;
}

const symbols_infos_t& DataManager::getSymbolsInfos()
{
    return _symbols_infos;
}
