#ifndef CAR_MODEL_H
#define CAR_MODEL_H

#include "cocos2d.h"

using namespace cocos2d;


enum CarType{
    Yellow, Red, Blue
};

struct CarConfig{
    int number;
    float speedSec;     //移动整个屏幕时间
    CarType carType;    //车的类型
};

class CarModel{
    
public:
    static CarModel* create(const CarConfig& config, EventDispatcher* dispatcher = NULL);
	CarModel();
	~CarModel();

public:
	Node* getNode(){ return m_node; }
    int getNumber(){ return m_number; }

protected:
    void initNode(CarType type);
	void init(const CarConfig& config, EventDispatcher* dispatcher);

private:
    CarType m_type;
	Node* m_node;
	int m_number;
    float m_speedSec;
    EventDispatcher* m_sceneEventDispatcher;
};

#endif