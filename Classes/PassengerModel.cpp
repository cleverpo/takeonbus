#include "PassengerModel.h"
#include "PassengerManager.h"
#include "cocostudio/CocoStudio.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

#define PASSENGER_THINK_POSITION Vec2(50, 110)
#define PASSENGER_MODEL_SIZE Size(100, 120)
#define PASSENGER_X -50
#define PASSENGER_Y 100

PassengerModel* PassengerModel::create(const PassengerConfig& config, EventDispatcher* dispatcher){
    PassengerModel* ret = new PassengerModel();
    
    if(ret){
        ret->init(config, dispatcher);
        return ret;
    }
    
    return NULL;
}

PassengerModel::PassengerModel():
m_node(NULL),
m_sceneEventDispatcher(NULL)
{

}

PassengerModel::~PassengerModel(){
	Director::getInstance()->getScheduler()->unscheduleUpdate(this);
	CC_SAFE_RELEASE(this->m_node);
}

void PassengerModel::init(const PassengerConfig& config, EventDispatcher* dispatcher){
    
    this->m_isDrag      = false;
    this->m_totalTime   = 0;
    this->m_thinkNumber = config.thinkNumber;
    this->m_leaveSec    = config.leaveSec;
    this->m_sceneEventDispatcher = dispatcher;

	auto manSprite = CSLoader::createNode("person/person_walk.csb");
	auto walkAction = CSLoader::createTimeline("person/person_walk.csb");

	manSprite->setAnchorPoint(Vec2(0.5, 0.5));
	manSprite->setContentSize(PASSENGER_MODEL_SIZE);
	manSprite->setPosition(Vec2(PASSENGER_X, PASSENGER_Y));

	manSprite->runAction(walkAction);
	walkAction->gotoFrameAndPlay(0, 15, true);
    
	this->m_node = manSprite;
	this->m_node->retain();
	this->m_size = PASSENGER_MODEL_SIZE;
    
    std::stringstream ss;
    ss << this->m_thinkNumber;
    
    //Think fade out
    Label* thingLabel = Label::createWithTTF(ss.str(), "fonts/arial.ttf", 28);
    thingLabel->setColor(Color3B(0, 0, 0));
    thingLabel->setPosition(PASSENGER_THINK_POSITION);
    thingLabel->setAnchorPoint(Vec2(0.5, 0.5));
    manSprite->addChild(thingLabel);
    thingLabel->runAction(Sequence::createWithTwoActions(DelayTime::create(config.thingSec), FadeOut::create(0.5)));
    
    Director::getInstance()->getScheduler()->scheduleUpdate<PassengerModel>(this, 0, false);
}

void PassengerModel::update(float dt){
    
    m_totalTime += dt;
    
    if(m_isDrag)
        return;
    
    if(m_totalTime >= m_leaveSec){
        this->m_node->runAction(Sequence::createWithTwoActions(MoveBy::create(3.0, Vec2(1000, 0)), CallFuncN::create([](Node* sender){
            sender->removeFromParent();
        })));
        //到时间离开
        this->m_sceneEventDispatcher->dispatchCustomEvent(PassengerManager::EventType_RemovePassenger, this);
        return;
    }
    
	Node* node = this->m_node;
	int currentX = node->getPositionX();

	//move
	if (currentX < m_endX){
		int moveX = MIN(6, m_endX - currentX);
		node->setPositionX(currentX + moveX);
	}
}