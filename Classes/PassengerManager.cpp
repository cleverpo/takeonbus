#include "PassengerManager.h"
#include "PassengerModel.h"
#include "CarManager.h"
#include "GameConfig.h"

#define PASSENGER_X_MAX 350
#define PASSENGER_MODEL_SIZE Size(100, 120)
#define PASSENGER_X_GAP 30

const std::string PassengerManager::EventType_RemovePassenger = "removePassenger";
const std::string PassengerManager::EventType_AddPassenger = "addPassenger";
const std::string PassengerManager::EventType_ShowResult = "showResult";

const std::string TimerKey = "PassengerManagerTimer";

PassengerManager::PassengerManager():
m_modelList(NULL),
m_sceneEventDispatcher(NULL),
m_carManager(NULL)
{
	m_passengerCount  = 0;
    m_passengerCount1 = 0;
    m_passengerCreateSec  = 0;
    m_passengerCreateTime = 0;
    m_totalTime = 0;
    
    Director::getInstance()->getScheduler()->schedule(std::bind(&PassengerManager::update, this, std::placeholders::_1), this, 1, false, TimerKey);
}

PassengerManager::~PassengerManager(){
    for (std::list<PassengerModel*>::iterator it = m_modelList.begin(); it != m_modelList.end(); it++){
        delete *it;
    }
    m_modelList.clear();
    
    Director::getInstance()->getScheduler()->unschedule(TimerKey, this);
}

void PassengerManager::adjustPassengerEndX(){
	int x = PASSENGER_X_MAX;

	for (std::list<PassengerModel*>::iterator it = m_modelList.begin(); it != m_modelList.end(); it++){
		PassengerModel* model = *it;
		model->setEndX(x);
		x -= model->getSize().width / 3 + PASSENGER_X_GAP;
	}
}

PassengerModel* PassengerManager::addPassenger(){
    PassengerConfig config;
    config.thinkNumber = random(GameConfig::getInstance()->numberRange.x, GameConfig::getInstance()->numberRange.y);
    config.thingSec = GameConfig::getInstance()->passengerThingSec;
    config.leaveSec = GameConfig::getInstance()->passengerLeaveSec;
    
    PassengerModel* passenger = PassengerModel::create(config, m_sceneEventDispatcher);
    
	m_modelList.push_back(passenger);
    
	m_passengerCount++;
    m_passengerCount1++;

	this->adjustPassengerEndX();
    
    this->m_sceneEventDispatcher->dispatchCustomEvent(PassengerManager::EventType_AddPassenger, passenger);
    
    //通知carManager保证在离开时候至少出现一辆车；
    float limitTime = m_totalTime + config.leaveSec - 2;
    limitTime = random<float>(m_totalTime, limitTime);
    
    m_carManager->addNeedCar(config.thinkNumber, limitTime);
	return passenger;
}

bool PassengerManager::removePassenger(PassengerModel* model){
	m_modelList.remove(model);
	CC_SAFE_DELETE(model);
	m_passengerCount--;

	this->adjustPassengerEndX();

	return true;
}

void PassengerManager::update(float dt){
    m_totalTime += dt;
    
    GameConfig* config = GameConfig::getInstance();
    
    if(m_passengerCount1 == config->passengerMaxCount){
        std::string text = "结束一波";
        this->m_sceneEventDispatcher->dispatchCustomEvent(PassengerManager::EventType_ShowResult, &text);
        return;
    }
    
    //判断是否到时间产生乘客
    if(m_passengerCreateTime >= m_passengerCreateSec){
        if (m_passengerCount < config->passengerMaxCount)
        this->addPassenger();
        m_passengerCreateTime = 0;
        float max = config->passengerCreateRate;
        m_passengerCreateSec = random<float>(1.0, max);
    }else{
        m_passengerCreateTime += dt;
    }
}