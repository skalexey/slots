//
//  SlotsMachine.h
//  Slots_ios
//
//  Created by Alexey Skorokhodov on 17/09/2017.
//  Copyright © 2017 Mac. All rights reserved.
//

#ifndef SlotsMachine_h
#define SlotsMachine_h

#include <vector>
#include <chrono>
#include "oxygine-framework.h"
#include "Reel.h"
#include "Payline.h"

DECLARE_SMART(SlotsMachine, spSlotsMachine);

class SlotsMachine : public oxygine::Actor
{
public:
    SlotsMachine(int reel_size, int reels_count, const oxygine::Vector2& slot_size);
    void spin();
    void update(float delta_time);
    void calculateVirtualStop();
    int getTotalWin();
private:
    const int bet_size = 100;
    int calculateTotalWin(const std::vector<Payline>& paylines);
    void showPaylines(const std::vector<Payline>& paylines);
    void showPayline(const Payline& payline);
    void shadeSlot(int reel_index, int slot_index);
    void animateSlot(int reel_index, int slot_index);
    bool checkCoins();
    void onSpinEnd();
    void getSymbolsInWinCombination(const slots_table_t& win_combination, std::vector<int>& symbols_in_win_combination);
    void findPaylines(std::vector<Payline>& paylines);
    void resetEffects();
    void initReels(const oxygine::Vector2& reel_size);
    void initVirtualStop();
    void pushVirtualStop();
    void initWinCombinations();
    int _reel_size;
    int _reels_count;
    std::vector<spReel> _reels;
    std::chrono::steady_clock::time_point _start_spinning_time;
    slots_table_t _virtual_stop;
    std::array<slots_table_t, 9> _win_combinations;
    bool _spinning;
    int _show_paylines_interval;
    int _total_win;
};
#endif /* SlotsMachine_h */
