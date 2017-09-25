//
//  TweenAnimRows.cpp
//  Slots_ios
//
//  Created by Alexey Skorokhodov on 25/09/2017.
//  Copyright © 2017 Mac. All rights reserved.
//

#include "TweenAnimRows.h"
using namespace oxygine;

TweenAnimRows::TweenAnimRows(const ResAnim *rs)
: TweenAnim(rs)
{
}

void TweenAnimRows::update(Sprite& actor, float p, const UpdateState& us)
{
    OX_ASSERT(_resAnim);
    int frame;
    
    if (p >= 1.0f)
        p = 0.9999999f;
    
    int rows_count = _resAnim->getRows();
    int columns_count = _resAnim->getColumns();
    int frames_count = columns_count * rows_count;
    int frame_index = (int)frames_count * p;
    int row_index = (int)frame_index / columns_count;
    frame = frame_index - row_index * columns_count;
    
    frame += _start;
    
    /*
     static int lastFrame = -1;
     static int ltime = 0;
     if (lastFrame != frame)
     {
     log::messageln("%d frame %d", getTimeMS() - ltime, frame);
     ltime = getTimeMS();
     }
     lastFrame = frame;
     */
    
    const AnimationFrame& fr = _row == -1 ? _resAnim->getFrame(frame) : _resAnim->getFrame(frame, row_index);
    _setAnimFrame(actor, fr);
}
