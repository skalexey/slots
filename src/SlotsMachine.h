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

DECLARE_SMART(SlotsMachine, spSlotsMachine);

class SlotsMachine : public oxygine::Actor
{
public:
    SlotsMachine(int reel_size, int reels_count, const oxygine::Vector2& slot_size);
    void spin();
    void update(float delta_time);
    void calculateVirtualStop();
private:
    const int bet_size = 100;
    bool checkCoins();
    void initReels(const oxygine::Vector2& reel_size);
    void initVirtualStop();
    void pushVirtualStop();
    int _reel_size;
    int _reels_count;
    std::vector<spReel> _reels;
    std::chrono::steady_clock::time_point _start_spinning_time;
    std::vector<std::vector<int> > _virtual_stop;
    bool _spinning;
};
#endif /* SlotsMachine_h */
