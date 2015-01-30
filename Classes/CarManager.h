#ifndef CAR_MANAGER_H
#define CAR_MANAGER_H

#include <map>

class PassengerManager;
class CarModel;
class CarManager{
public:
    static const std::string EventType_RemoveCar;
    static const std::string EventType_AddCar;
    
public:
	CarManager();
	~CarManager();

public:
    void addCar();
	void removeCar(CarModel* car);
    void addNeedCar(int symbol, float time);
    
    void update(float dt);
public:
    const std::multimap<int, CarModel*>& getCarMap(){ return m_carMap; }

public:
    void setSceneEventDispatcher(cocos2d::EventDispatcher* dispatcher){ m_sceneEventDispatcher = dispatcher; }
    
protected:
    int generateNumber();
    
private:
    std::map<int, float> m_needCarMap; // key: number  value: dt
	std::multimap<int, CarModel*> m_carMap;
    cocos2d::EventDispatcher* m_sceneEventDispatcher;
    
private:
    int m_carCount;
    
    //汽车乘客出现的计时器
    float m_carCreateTime;
    float m_carCreateSec;
    float m_totalTime;
};

#endif