//
//  Reel.h
//  Slots_ios
//
//  Created by Alexey Skorokhodov on 17/09/2017.
//  Copyright © 2017 Mac. All rights reserved.
//

#ifndef Reel_h
#define Reel_h

#include <deque>
#include <chrono>
#include "oxygine-framework.h"
#include "Slot.h"

DECLARE_SMART(Reel, spReel);
class Reel : public oxygine::Actor
{
public:
    Reel(int size, oxygine::Vector2 slot_size);
    void update(float delta_time);
    void spin();
    int getQueueSize() const;
    void pushRandomSlot();
    void pushSlot(int slot_index);
    bool isSpinning() const;
    void shadeSlot(int slot_index);
    void resetEffects();
    void setSlotSymbol(int slot_index, int symbol);
private:
    void resetInnerContainerPosition();
    void pushInnerContainerUp();
    void shiftSlotsDown();
    void takeSlotFromQueue();
    void unblurIfBlured();
    void blurIfNotBlured();
    int _size;
    oxygine::Vector2 _slot_size;
    std::vector<Slot> _slots;
    std::deque<int> _slots_queue;
    oxygine::spActor _inner_container;
    double _v;
    double _v0;
    double _ba;
    bool _spinning;
    std::chrono::steady_clock::time_point _start_spinning_time;
    std::vector<oxygine::spActor> _effects_nodes;
};

#endif /* Reel_h */
