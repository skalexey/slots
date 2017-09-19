//
//  Payline.cpp
//  Slots_ios
//
//  Created by Alexey Skorokhodov on 19/09/2017.
//  Copyright © 2017 Mac. All rights reserved.
//
#include <algorithm>
#include "Payline.h"
#include "DataManager.h"

Payline::Payline(const slots_table_t& win_combination, const std::vector<int>& symbols_in_win_combination)
: win_combination(win_combination)
, symbols_in_win_combination(symbols_in_win_combination)
{
    
}

int Payline::getWinAmount() const
{
    int win_symbol = symbols_in_win_combination[0];
    int win_symbols_count = (int)std::count(symbols_in_win_combination.begin(), symbols_in_win_combination.end(), win_symbol);
    int win_amount = DataManager::instance().getWinAmount(win_symbol, win_symbols_count);
    return win_amount;
}
