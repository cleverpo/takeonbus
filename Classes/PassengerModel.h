#ifndef PASSENGER_MODEL_H
#define PASSENGER_MODEL_H

#include "cocos2d.h"

using namespace cocos2d;

struct PassengerConfig{
    int thinkNumber;
    float leaveSec;
    float thingSec;
};

class PassengerModel {
    
public:
    static PassengerModel* create(const PassengerConfig& config, EventDispatcher* dispatcher);
	PassengerModel();
	~PassengerModel();
    
protected:
	void init(const PassengerConfig& config, EventDispatcher* dispatcher);
    
public:
	Size getSize(){ return m_size; }
	Node* getNode(){ return m_node; }
	void setEndX(int x){ m_endX = x; }
    int getThinkNum(){ return m_thinkNumber; }
    
    void setIsDrag(bool isDrag){ m_isDrag = isDrag; }
    bool getIsDrag(){ return m_isDrag; }

public:
	void update(float dt);

private:
	Size m_size;
	Node* m_node;
	int m_endX;
    int m_thinkNumber;
    float m_leaveSec;
    
    EventDispatcher* m_sceneEventDispatcher;
    //持续时间
    float m_totalTime;
    //是否被玩家提起
    bool m_isDrag;
};

#endif