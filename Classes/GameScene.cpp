#include "GameScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/UIButton.h"
#include "PassengerModel.h"
#include "PassengerManager.h"
#include "CarModel.h"
#include "CarManager.h"
#include "GameConfig.h"
#include "DifficultMonitor.h"
#include "util.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

#define PASSENGER_QUEUE_RECT Rect(0, 0, 300, 200)

#define HUD_LAYER_INDEX 1
#define CAR_LAYER_INDEX 10
#define PASSENGER_INDEX 50
#define ZOMBIE_INDEX 60
#define RESULT_LAYER_INDEX 100

#define ROAD1_Y 330
#define ROAD2_Y 500

Vec2 dragPassengerOriginPos;

bool isMatch(CarModel* car, PassengerModel* passenger){
    return true;
}

bool GameLayer::init(){
	if (!Layer::init()){
		return false;
	}
    
    this->m_score       = 0;
    this->m_missCount   = 0;
    
    //ui
	Size winSize = Director::getInstance()->getWinSize();
    
    //layer
    this->initHUDLayer();
    
    this->m_carLayer = Layer::create();
    this->addChild(this->m_carLayer, CAR_LAYER_INDEX);
    
    this->m_resultLayer = ResultLayer::create();
    this->addChild(m_resultLayer, RESULT_LAYER_INDEX);
    
    //bg
	Node* bg = Sprite::create("bg/bg.png");
	bg->setPosition(winSize.width / 2, winSize.height / 2);
	this->addChild(bg);
    
    // leave btn
    Button* btn = Button::create("Default/Button_Normal.png");
    btn->setPosition(Vec2(940, 50));
    btn->addClickEventListener([](Ref* sender){
        Director::getInstance()->popScene();
    });
    this->addChild(btn);
    
    for(int i = 0; i < GameConfig::getInstance()->carMaxCount; i++){
        m_carSymbol[i] = false;
    }
	
    //shader
    GLProgram* program = GLProgram::createWithFilenames("shader/colorChange.vsh", "shader/colorChange.fsh");
    GLProgramCache::getInstance()->addGLProgram(program, "colorChangeShader");
    
    //关联乘客管理器和车管理器
    this->m_passengerManager->setCarManager(this->m_carManager);
    this->initSchedule();
    this->initEventDispatcher();
    
    this->initRoad();
    
    this->addZombie();
    
	return true;
}

void GameLayer::initHUDLayer(){
    Layer* layer     = static_cast<Layer*>(CSLoader::createNode("HUDLayer.csb"));
    Text* scoreLabel = static_cast<Text*>(layer->getChildByName("scoreLabel"));
    Text* missLabel  = static_cast<Text*>(layer->getChildByName("missLabel"));
    
    scoreLabel->setString(convertToString(this->m_score));
    missLabel->setString(convertToString(this->m_missCount));
    
    this->addChild(layer, HUD_LAYER_INDEX);
    
    this->m_hudLayer = layer;
    this->m_scoreLabel = scoreLabel;
    this->m_missLabel  = missLabel;
}

void GameLayer::initRoad(){
    Node* road = Sprite::create("bg/road.png");
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    road->setPosition(visibleSize.width / 2, ROAD1_Y);
    this->m_carLayer->addChild(road);
}

void GameLayer::initSchedule(){
    this->schedule(CC_SCHEDULE_SELECTOR(GameLayer::gameTimer), 1.0f);
}

void GameLayer::initEventDispatcher(){
    m_customEventDispatcher->addCustomEventListener(PassengerManager::EventType_RemovePassenger, [=](EventCustom* event){
        PassengerModel* model = static_cast<PassengerModel*>(event->getUserData());
        
        this->passengerLeave(model);
        
    });
    
    m_customEventDispatcher->addCustomEventListener(CarManager::EventType_RemoveCar, [=](EventCustom* event){
        CarModel* model = static_cast<CarModel*>(event->getUserData());
        
        this->removeCar(model);
    });
    m_customEventDispatcher->addCustomEventListener(CarManager::EventType_AddCar, [=](EventCustom* event){
        CarModel* model = static_cast<CarModel*>(event->getUserData());
        
        this->addCar(model);
    });
    
    m_customEventDispatcher->addCustomEventListener(PassengerManager::EventType_AddPassenger, [=](EventCustom* event){
        PassengerModel* model = static_cast<PassengerModel*>(event->getUserData());
        
        this->addPassengerToQueue(model);
    });
    
    m_customEventDispatcher->addCustomEventListener(PassengerManager::EventType_ShowResult, [=](EventCustom* event){
        std::string* text = static_cast<std::string*>(event->getUserData());
        this->m_resultLayer->showString(*text);
    });
    
    m_passengerManager->setSceneEventDispatcher(m_customEventDispatcher);
    m_carManager->setSceneEventDispatcher(m_customEventDispatcher);
    
    m_customEventDispatcher->setEnabled(true);
}


// there's no 'id' in cpp, so we recommend returning the class instance pointer
cocos2d::Scene* GameLayer::scene(){
	Scene* scene = Scene::create();
	if (scene)
	{
		GameLayer* layer = GameLayer::create();
		scene->addChild(layer);

		return scene;
	}

	return NULL;
}

GameLayer::GameLayer():
m_score(0),
m_missCount(0),
m_totolTime(0),
m_carLayer(NULL),
m_hudLayer(NULL),
m_resultLayer(NULL),
m_scoreLabel(NULL),
m_missLabel(NULL)
{
    m_carSymbol             = new bool[GameConfig::getInstance()->carMaxCount];
    m_passengerManager      = new PassengerManager();
    m_carManager            = new CarManager();
    m_customEventDispatcher = new EventDispatcher();
}

GameLayer::~GameLayer(){
	CC_SAFE_DELETE(m_passengerManager);
	CC_SAFE_DELETE(m_carManager);
    CC_SAFE_DELETE(m_customEventDispatcher);
    
    delete[] m_carSymbol;
    m_carSymbol = NULL;
    m_hudLayer  = NULL;
    m_resultLayer = NULL;
    m_carLayer  = NULL;
    m_scoreLabel  = NULL;
    m_missLabel = NULL;
}

void GameLayer::passengerLeave(PassengerModel* model){
    //扣分
    this->changeScore(10, false);
    this->changeMissCount(1);
    
    //移除
    this->removePassengerFromQueue(model, false);
}

void GameLayer::removePassengerFromQueue(PassengerModel* model, bool isRemove){
    if (isRemove){
        this->removeChild(model->getNode());
    }
    
    this->m_passengerManager->removePassenger(model);
    this->_eventDispatcher->removeEventListenersForTarget(model->getNode());
}

bool GameLayer::addPassengerToQueue(PassengerModel* passenger){
	Node* passengerNode = passenger->getNode();

	EventListenerTouchOneByOne* eventListenerTouch = EventListenerTouchOneByOne::create();
	eventListenerTouch->setSwallowTouches(true);
	eventListenerTouch->onTouchBegan = [=](Touch* touch, Event* event)->bool{
		Node* target = event->getCurrentTarget();
		Vec2 touchPoint = target->convertToNodeSpace(touch->getLocation());
		Rect nodeRect = Rect(0, 0, target->getContentSize().width, target->getContentSize().height);

		if (nodeRect.containsPoint(touchPoint))
		{
			// mark drag target origin position
			dragPassengerOriginPos = target->getPosition();
            passenger->setIsDrag(true);
			return true;
		}
		return false;
	};
	eventListenerTouch->onTouchMoved = [=](Touch* touch, Event* event){
		Node* target = event->getCurrentTarget();
		Node* targetParent = target->getParent();

		if (targetParent){
			Vec2 position = targetParent->convertToNodeSpace(touch->getLocation());
			target->setPosition(position);
		}
	};
	eventListenerTouch->onTouchEnded = [=](Touch* touch, Event* event){
        passenger->setIsDrag(false);
        Rect passengerRect = passengerNode->getBoundingBox();
        
		if (PASSENGER_QUEUE_RECT.intersectsRect(passengerRect)){
            passengerNode->setPosition(dragPassengerOriginPos);
            dragPassengerOriginPos = Vec2::ZERO;
		}else{
            //check is in car position?
            bool isCorrect = false;
            const std::multimap<int, CarModel*>& carMap = m_carManager->getCarMap();
            
            CarModel* ret = NULL;
            int minIndex        = 0;
            
            for(std::multimap<int, CarModel*>::const_iterator it = carMap.begin(); it != carMap.end(); it++){
                CarModel* carModel = (*it).second;
                Node* carNode   = carModel->getNode();
                Rect rect       = carNode->getBoundingBox();
                
                if(rect.intersectsRect(passengerRect)){
                    //必须位于最前并正确才正确；
                    if(carNode->getOrderOfArrival() > minIndex){
                        minIndex = carNode->getOrderOfArrival();
                        ret = carModel;
                    }
                }
            }
            
            if (ret && ret->getNumber() == passenger->getThinkNum()){
                isCorrect = true;
            }
            
            this->changeScore(10, isCorrect);
            
            //remove passenger
            this->removePassengerFromQueue(passenger, true);
		}
	};

	this->addChild(passengerNode, PASSENGER_INDEX);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListenerTouch, passengerNode);

	return true;
}

bool GameLayer::addCar(CarModel* car){
    int carMaxCount = GameConfig::getInstance()->carMaxCount;
	Node* carNode = car->getNode();
    int index = 0;
    
    for(int i = 0; i < carMaxCount; i++){
        if(m_carSymbol[i] == false){
            index = i;
            break;
        }
    }
    m_carSymbol[index] = true;
	this->m_carLayer->addChild(carNode);


	return true;
}

void GameLayer::removeCar(CarModel* car){
	this->m_carLayer->removeChild(car->getNode());
}

void GameLayer::addZombie(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    Label* label = Label::createWithSystemFont("我是\n僵尸", "arial", 30);
    label->setAnchorPoint(Vec2(0.5, 0));
    label->setContentSize(Size(100,100));
    label->setPosition(Vec2(visibleSize.width, 100));
    
    label->runAction(MoveTo::create(GameConfig::getInstance()->passengerLeaveSec, Vec2(350, 100)));
    
    this->addChild(label, ZOMBIE_INDEX);
}

void GameLayer::stopTimer(){
    this->pause();
}

void GameLayer::startTimer(){
    this->resume();
}

void GameLayer::gameTimer(float dt){
//    m_totolTime += dt;
//    if (m_totolTime > 20){
//        DifficultMonitor::getInstance()->increateDifficult();
//        m_totolTime = 0;
//        m_resultLayer->showString("升级难度");
//    }
    
    
}

void GameLayer::changeScore(int score, bool add){
    if(add){
        this->m_score += score;
    }else{
        this->m_score -= score;
    }
    
    this->m_resultLayer->showScore(score, add);
    this->m_scoreLabel->setString(convertToString(this->m_score));
}

void GameLayer::changeMissCount(int changeNum){
    this->m_missCount += changeNum;
    this->m_missLabel->setString(convertToString(this->m_missCount));
}

/*
    ResultLayer
 */

ResultLayer::ResultLayer(){

}

ResultLayer::~ResultLayer(){

}

bool ResultLayer::init(){
    if (!Layer::init())
        return false;
    
    return true;
}

void ResultLayer::showString(std::string text){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Label* label = Label::createWithSystemFont(text.c_str(), "Arial", 40);
    
    label->setColor(Color3B(1.0, 0.0, 0.0));
    label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(label);
    
    FiniteTimeAction* action1 = Spawn::createWithTwoActions(MoveBy::create(2.0, Vec2(0, 100)), FadeOut::create(1.5));
    FiniteTimeAction* action2 = CallFunc::create([=](){
        label->removeFromParent();
    });
    label->runAction(Sequence::createWithTwoActions(action1, action2));
}

void ResultLayer::showScore(int score, bool isIncrease){
    std::stringstream ss;
    
    if (isIncrease)
        ss << "+ ";
    else
        ss << "- ";
    ss << score;
    
    std::string text= ss.str();
    this->showString(text);
}