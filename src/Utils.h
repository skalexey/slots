//
//  Utils.h
//  Slots_ios
//
//  Created by Alexey Skorokhodov on 18/09/2017.
//  Copyright © 2017 Mac. All rights reserved.
//

#ifndef Utils_h
#define Utils_h

#include <sstream>
namespace Utils
{    
    template <typename T>
    std::string to_string(T value)
    {
        std::ostringstream os ;
        os << value ;
        return os.str() ;
    }
}

#endif /* Utils_h */
