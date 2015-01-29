//
//  util.h
//  takeonbus
//
//  Created by luozhipeng on 15/1/25.
//
//

#ifndef takeonbus_util_h
#define takeonbus_util_h

template<class T>
std::string convertToString(T value){
    std::stringstream ss;
    ss << value;
    
    return ss.str();
}

#endif
