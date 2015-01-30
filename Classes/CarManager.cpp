#include "CarManager.h"
#include "CarModel.h"
#include "PassengerManager.h"
#include "PassengerModel.h"
#include "GameConfig.h"
#include <math.h>

const std::string CarManager::EventType_RemoveCar = "RemoveCar";
const std::string CarManager::EventType_AddCar = "AddCar";

const std::string TimerKey = "CarManagerKey";

CarManager::CarManager():
m_sceneEventDispatcher(NULL)
{
    m_carCount      = 0;
    m_carCreateSec  = 0;
    m_carCreateTime = 0;
    m_totalTime     = 0;
    
    Director::getInstance()->getScheduler()->schedule(std::bind(&CarManager::update, this, std::placeholders::_1), this, 1, false, TimerKey);
}

CarManager::~CarManager(){
    for (std::map<int, CarModel*>::const_iterator it = m_carMap.begin(); it != m_carMap.end(); it++){
        delete (*it).second;
    }
    m_carMap.clear();
    
    Director::getInstance()->getScheduler()->unschedule(TimerKey, this);
}

void CarManager::addCar(){
    int num = this->generateNumber();

    CarConfig config;
    config.carType  = CarType(random<int>(0, 2));
    config.number   = num;
    config.speedSec = GameConfig::getInstance()->carSpeed;
    
    CarModel* car = CarModel::create(config, std::bind(&CarManager::removeCar, this, std::placeholders::_1));
	m_carMap.insert(std::pair<int, CarModel*>(num, car));

    this->m_sceneEventDispatcher->dispatchCustomEvent(CarManager::EventType_AddCar, car);
    
    m_carCount++;
}

void CarManager::removeCar(CarModel* car){
	for (std::map<int, CarModel*>::const_iterator it = m_carMap.begin(); it != m_carMap.end(); it++){
		if ((*it).second == car){
			m_carMap.erase(it);
            car->getNode()->removeFromParent();
			CC_SAFE_DELETE(car);
            break;
		}
	}

    m_carCount--;
}

void CarManager::addNeedCar(int symbol, float time){
    if (m_needCarMap.find(symbol) == m_needCarMap.end()){
        //没在需求列表里则加入
        m_needCarMap[symbol] = time;
    }
}

int CarManager::generateNumber(){
    for (std::map<int, float>::const_iterator it = m_needCarMap.begin(); it != m_needCarMap.end(); it++){
        if (m_totalTime >= (*it).second){
            m_needCarMap.erase(it);
            return (*it).first;
        }
    }
    
    return random(GameConfig::getInstance()->numberRange.x, GameConfig::getInstance()->numberRange.y);
}

void CarManager::update(float dt){
    m_totalTime += dt;
    
    GameConfig* config = GameConfig::getInstance();
    
    //判断是否到时间产生汽车
    if(m_carCreateTime >= m_carCreateSec){
        if (m_carCount < config->carMaxCount)
            this->addCar();
        m_carCreateTime = 0;
        m_carCreateSec = random<float>(1.0, config->carCreateRate);
    }else{
        m_carCreateTime += dt;
    }
}