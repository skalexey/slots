//
//  TweenAnimRows.h
//  Slots_ios
//
//  Created by Alexey Skorokhodov on 25/09/2017.
//  Copyright © 2017 Mac. All rights reserved.
//

#ifndef TweenAnimRows_h
#define TweenAnimRows_h

#include "oxygine-framework.h"
namespace oxygine
{
    class TweenAnimRows : public TweenAnim
    {
    public:
        TweenAnimRows(const ResAnim *rs);  
        void update(Sprite& actor, float p, const UpdateState& us);
    };
}

#endif /* TweenAnimRows_h */
