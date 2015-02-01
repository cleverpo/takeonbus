//
//  GameConfig.h
//  takeonbus
//
//  Created by luozhipeng on 15/1/26.
//
//

#ifndef takeonbus_config_h
#define takeonbus_config_h

#include <string>
#include "cocos2d.h"

class GameConfig {
    
protected:
    GameConfig(){
        passengerMaxCount   = 11;            // 1~11
        passengerCreateRate = 5.f;          // 1~11
        carMaxCount         = 5;            //
        carChannelCount     = 1;            //
        carSpeed            = 10.f;         // 5~21
        carCreateRate       = 5.f;          // 1~11
        carMatchRate        = 0.7f;         // 0.5~1
        passengerThingSec   = 60.f;         // 3~30
        passengerLeaveSec   = 10.f;         // 3~30
        numberRange         = cocos2d::Vec2(1, 5);      //(1,10)
    };
    
public:
    static GameConfig* getInstance();
    
public:
    void setConfig(std::string filename, int difficult);

public:
    //排队乘客最大值
    int passengerMaxCount;
    //乘客出现频率
    float passengerCreateRate;
    //单条通道汽车最大值
    int carMaxCount;
    //汽车通道数量
    int carChannelCount;
    //汽车速度
    float carSpeed;
    //汽车出现频率
    float carCreateRate;
    //汽车符合现有乘客想法的概率
    float carMatchRate;
    //乘客想法时间
    float passengerThingSec;
    //乘客的离开时间
    float passengerLeaveSec;
    //数字范围
    cocos2d::Vec2 numberRange;
};

#endif
