//
//  SlotsMachine.cpp
//  Slots_ios
//
//  Created by Alexey Skorokhodov on 17/09/2017.
//  Copyright © 2017 Mac. All rights reserved.
//

#include <algorithm>
#include "SlotsMachine.h"
#include "Dispatcher.h"
#include "DataManager.h"
#include "EventsController.h"

using namespace oxygine;

extern Resources gameResources;

const float ui_scale = 0.7f;
const int delay_between_reels_ms = 200;
const int spinning_time_ms = 7000;
const int calculation_time_point = 3000;
const int delay_between_showing_paylines = 2000;
const int wild_symbol = 0;
SlotsMachine::SlotsMachine(int reel_size, int reels_count, const Vector2& slot_size)
: _reel_size(reel_size)
, _reels_count(reels_count)
, _spinning(false)
, _show_paylines_interval(0)
, _total_win(0)
{
    _inner_container = new Actor;
    setSize({reels_count * slot_size.x, reel_size * slot_size.y});
    spClipRectActor clp = new ClipRectActor;
    clp->setSize(getSize());
    clp->addChild(_inner_container);
    addChild(clp);
    initReels(slot_size);
    initVirtualStop();
    initWinCombinations();
    initGraphics();
}

void SlotsMachine::initGraphics()
{
    Vector2 parent_size = getSize();
    spSprite top_left_part = new Sprite;
    top_left_part->setResAnim(gameResources.getResAnim("frame_top"));
    top_left_part->setScale(ui_scale);
    top_left_part->setPosition(-112, -50);
    addChild(top_left_part);
    
    spSprite top_right_part = new Sprite;
    top_right_part->setResAnim(gameResources.getResAnim("frame_top"));
    top_right_part->setScale(-ui_scale, ui_scale);
    top_right_part->setPosition(900, -50);
    addChild(top_right_part);
    
    spSprite title = new Sprite;
    title->setResAnim(gameResources.getResAnim("frame_title"));
    title->setAnchor(0.5, 0.5);
    title->setScale(ui_scale);
    title->setPosition(parent_size.x / 2, -35);
    addChild(title);
    
    spSprite bottom_left_part = new Sprite;
    bottom_left_part->setResAnim(gameResources.getResAnim("frame_bottom"));
    bottom_left_part->setScale(ui_scale);
    bottom_left_part->setPosition(-112, parent_size.y - 8);
    addChild(bottom_left_part);
    
    spSprite bottom_right_part = new Sprite;
    bottom_right_part->setResAnim(gameResources.getResAnim("frame_bottom"));
    bottom_right_part->setScale(-ui_scale, ui_scale);
    bottom_right_part->setPosition(900, parent_size.y - 8);
    addChild(bottom_right_part);
    
    spSprite left_part = new Sprite;
    left_part->setResAnim(gameResources.getResAnim("frame_side"));
    left_part->setScale(-ui_scale * 0.76, ui_scale * 0.76);
    left_part->setPosition(20, -88);
    addChild(left_part);
    
    spSprite right_part = new Sprite;
    right_part->setResAnim(gameResources.getResAnim("frame_side"));
    right_part->setScale(ui_scale * 0.76, ui_scale * 0.76);
    right_part->setPosition(parent_size.x - 21, -88);
    addChild(right_part);
}

void SlotsMachine::initWinCombinations()
{
    slots_table_t win_combination1 = {{0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}};
    slots_table_t win_combination2 = {{1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0}};
    slots_table_t win_combination3 = {{0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1}};
    slots_table_t win_combination4 = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {0, 1, 0}, {1, 0, 0}};
    slots_table_t win_combination5 = {{0, 0, 1}, {0, 1, 0}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    slots_table_t win_combination6 = {{0, 1, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {0, 1, 0}};
    slots_table_t win_combination7 = {{0, 1, 0}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 1, 0}};
    slots_table_t win_combination8 = {{1, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {0, 0, 1}};
    slots_table_t win_combination9 = {{0, 0, 1}, {0, 0, 1}, {0, 1, 0}, {1, 0, 0}, {1, 0, 0}};
    _win_combinations[0] = win_combination1;
    _win_combinations[1] = win_combination2;
    _win_combinations[2] = win_combination3;
    _win_combinations[3] = win_combination4;
    _win_combinations[4] = win_combination5;
    _win_combinations[5] = win_combination6;
    _win_combinations[6] = win_combination7;
    _win_combinations[7] = win_combination8;
    _win_combinations[8] = win_combination9;
}

void SlotsMachine::onSpinEnd()
{
    std::vector<Payline> paylines;
    findPaylines(paylines);
    showPaylines(paylines);
    _total_win = calculateTotalWin(paylines);
    DataManager::instance().appendCoins(_total_win);
}

int SlotsMachine::getTotalWin() const
{
    return _total_win;
}

int SlotsMachine::calculateTotalWin(const std::vector<Payline>& paylines)
{
    int total_win = 0;
    for(int payline_index = 0; payline_index < paylines.size(); payline_index++)
    {
        const Payline& payline = paylines[payline_index];
        total_win += payline.getWinAmount();
    }
    return total_win;
}

void SlotsMachine::showPaylines(const std::vector<Payline>& paylines)
{
    for(int payline_index = 0; payline_index < paylines.size(); payline_index++)
    {
        const Payline& payline = paylines[payline_index];
        _show_paylines_interval = Dispatcher::instance().runAndRepeatAfter([=]()
        {
            Dispatcher::instance().runAfter([=]()
            {
                showPayline(payline);
                
            }, payline_index * delay_between_showing_paylines);
        }, (int)(paylines.size() + 1) * delay_between_showing_paylines);
    }
}

void SlotsMachine::resetEffects()
{
    for(const spReel& reel : _reels)
    {
        reel->resetEffects();
    }
}

void SlotsMachine::showPayline(const Payline& payline)
{
    resetEffects();
    for(int reel_index = 0; reel_index < _reels_count; reel_index++)
    {
        std::vector<int> reel_in_win_combination = payline.win_combination[reel_index];
        
        for(int slot_index = 0; slot_index < _reel_size; slot_index++)
        {
            int slot_symbol_in_win_combination = payline.symbols_in_win_combination[reel_index];
            if(reel_in_win_combination[slot_index] == 1 && slot_symbol_in_win_combination > 0)
            {
                if(slot_symbol_in_win_combination != _virtual_stop[reel_index][slot_index])
                {
                    _reels[reel_index]->setSlotSymbol(slot_index, slot_symbol_in_win_combination);
                    _virtual_stop[reel_index][slot_index] = slot_symbol_in_win_combination;
                }
                animateSlot(reel_index, slot_index);
            }
            else
            {
                shadeSlot(reel_index, slot_index);
            }
        }
    }
}

void SlotsMachine::animateSlot(int reel_index, int slot_index)
{
    
}

void SlotsMachine::shadeSlot(int reel_index, int slot_index)
{
    spReel reel = _reels[reel_index];
    reel->shadeSlot(slot_index);
}

void SlotsMachine::getSymbolsInWinCombination(const slots_table_t& win_combination, std::vector<int>& symbols_in_win_combination) const
{
    for(int reel_index = 0; reel_index < _reels_count; reel_index++)
    {
        const std::vector<int>& reel_in_combination = win_combination[reel_index];
        const std::vector<int>& reel_in_virtual_stop = _virtual_stop[reel_index];
        for(int slot_index = 0; slot_index < reel_in_combination.size(); slot_index++)
        {
            if(reel_in_combination[slot_index] == 1)
            {
                symbols_in_win_combination.push_back(reel_in_virtual_stop[slot_index]);
            }
        }
    }
}

void SlotsMachine::findPaylines(std::vector<Payline>& paylines) const
{
    for(int win_combination_index = 0; win_combination_index < _win_combinations.size(); win_combination_index++)
    {
        const slots_table_t& win_combination = _win_combinations[win_combination_index];
        std::vector<int> symbols_in_win_combination;
        getSymbolsInWinCombination(win_combination, symbols_in_win_combination);
        int last_symbol = -1;
        int win_symbol = -1;
        std::vector<int> win_set;
        std::vector<int> wild_set;
        for(int reel_index = 0; reel_index < _reels_count; reel_index++)
        {
            int& symbol = symbols_in_win_combination[reel_index];
            if(win_symbol == -1)
            {
                if(symbol == wild_symbol)
                {
                    wild_set.push_back(symbol);
                }
                else
                {
                    win_symbol = symbol;
                    if(!wild_set.empty())
                    {
                        for(int i = reel_index - 1; i >= 0; i--)
                        {
                            symbols_in_win_combination[i] = win_symbol;
                        }
                    }
                }
            }
            else if(symbol != win_symbol)
            {
                if(symbol == wild_symbol)
                {
                    symbol = win_symbol;
                }
                else
                {
                    break;
                }
                
            }
            win_set.push_back(symbol);
        }
        if(win_set.size() >= 3)
        {
            // remove all symbols instead of payable
            for(size_t i = win_set.size(); i < _reels_count; i++)
            {
                symbols_in_win_combination[i] = 0;
            }
            paylines.push_back(Payline(win_combination, symbols_in_win_combination));
        }
    }
}

bool SlotsMachine::checkCoins() const
{
    return DataManager::instance().getCoinsCount() >= bet_size;
}

void SlotsMachine::spin()
{
    resetEffects();
    
    if(_show_paylines_interval != 0)
    {
        Dispatcher::instance().cancelAll();
        _show_paylines_interval = 0;
    }
    
    if(!checkCoins())
    {
        EventsController::instance().fireEvent("event.spin_end");
        return;
    }
    
    DataManager::instance().spendCoins(bet_size);
    _start_spinning_time = std::chrono::steady_clock::now();
    _spinning = true;
    for(int reel_index = 0; reel_index < _reels.size(); reel_index++)
    {
        spReel reel = _reels[reel_index];
        Dispatcher::instance().runAfter([=]{
           reel->spin();
        }, reel_index * delay_between_reels_ms);
    }
    Dispatcher::instance().runAfter([=]{
        calculateVirtualStop();
        pushVirtualStop();
    }, calculation_time_point);
}

void SlotsMachine::pushVirtualStop()
{
    for(int reel_index = 0; reel_index < _reels_count; reel_index++)
    {
        spReel& reel = _reels[reel_index];
        std::vector<int>& virtual_stop_reel = _virtual_stop[reel_index];
        for(auto slot_it = virtual_stop_reel.rbegin(); slot_it != virtual_stop_reel.rend(); ++slot_it)
        {
            int slot = *slot_it;
            reel->pushSlot(slot);
        }
    }
}

void SlotsMachine::calculateVirtualStop()
{
    std::vector<bool> wilds_on_reels(_reels_count, false);
    for(int reel_index = 0; reel_index < _reels_count; reel_index++)
    {
        std::vector<int>& reel = _virtual_stop[reel_index];
        for(auto& slot : reel)
        {
            slot = DataManager::instance().calculateNewSlot();
            if(slot == wild_symbol) // wild
            {
                if(std::count(wilds_on_reels.begin(), wilds_on_reels.end(), true) < _reels_count)
                {
                    wilds_on_reels[reel_index] = true;
                }
                else
                {
                    do
                    {
                        slot = DataManager::instance().calculateNewSlot();
                    }
                    while(slot == 0);
                }
            }
        }
    }
}

void SlotsMachine::initVirtualStop()
{
    for(int reel_index = 0; reel_index < _reels_count; reel_index++)
    {
        std::vector<int> reel(_reel_size, 0);
        _virtual_stop.push_back(reel);
    }
}

void SlotsMachine::initReels(const Vector2& slot_size)
{
    for(int reel_index = 0; reel_index < _reels_count; reel_index++)
    {
        spReel new_reel = new Reel(_reel_size, slot_size);
        _reels.push_back(new_reel);
        new_reel->setPosition({reel_index * slot_size.x, 0});
        _inner_container->addChild(new_reel);
    }
}

void SlotsMachine::update(float delta_time)
{
    bool reels_stopped = true;
    for(spReel& reel : _reels)
    {
        reel->update(delta_time);
        if(reel->isSpinning())
        {
            reels_stopped = false;
        }
    }
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    int time_since_spin_begin = (int)std::chrono::duration_cast<std::chrono::milliseconds>(now - _start_spinning_time).count();
    if(_spinning && reels_stopped && time_since_spin_begin > 200)
    {
        _spinning = false;
        onSpinEnd();
        EventsController::instance().fireEvent("event.spin_end");
    }
}
