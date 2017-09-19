//
//  Payline.h
//  Slots_ios
//
//  Created by Alexey Skorokhodov on 19/09/2017.
//  Copyright © 2017 Mac. All rights reserved.
//

#ifndef Payline_h
#define Payline_h

#include "Slot.h"

struct Payline
{
    Payline(const slots_table_t& win_combination, const std::vector<int>& end_payline);
    const slots_table_t& win_combination;
    std::vector<int> symbols_in_win_combination;
    int getWinAmount() const;
};

#endif /* Payline_h */
