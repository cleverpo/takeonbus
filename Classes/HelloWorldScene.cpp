#include "HelloWorldScene.h"
#include "AppMacros.h"
#include "ui/UIButton.h"
#include "ui/UISlider.h"
#include "ui/UIText.h"
#include "cocostudio/CocoStudio.h"
#include "GameScene.h"
#include "GameConfig.h"
#include "util.h"
#include <math.h>

using namespace cocostudio;
USING_NS_CC;
using namespace ui;

Scene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
	
    auto closeItem = MenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        CC_CALLBACK_1(HelloWorld::menuCloseCallback,this));
    
    closeItem->setPosition(origin + Vec2(visibleSize) - Vec2(closeItem->getContentSize() / 2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...
	//auto node = SceneReader::getInstance()->createNodeWithSceneFile("MainScene.csb");
	Node* node = CSLoader::createNode("MainScene.csb");
	this->addChild(node, 1);

	Button* startBtn = static_cast<Button*>(node->getChildByName("startBtn"));
	startBtn->addClickEventListener([=](Ref* sender){
		Scene* gameScene = GameLayer::scene();
		Director::getInstance()->pushScene(gameScene);
	});
    
    
    //config
    GameConfig* config = GameConfig::getInstance();
    
    Slider* slider = static_cast<Slider*>(node->getChildByName("passengerCountSlider"));
    Text* textField = static_cast<Text*>(node->getChildByName("passengerCountLabel"));
    float maxValue = 11;
    float minValue = 1;
    float current = config->passengerMaxCount;
    slider->setPercent(40);
    textField->setString(convertToString<int>(current));
    slider->addEventListener([=](Ref* sender, Slider::EventType eventType){
        Slider* node = static_cast<Slider*>(sender);
        if(eventType == Slider::EventType::ON_PERCENTAGE_CHANGED){
            int percent     = node->getPercent();
            int final_value = minValue + percent * (maxValue - minValue) / 100;
            textField->setString((convertToString<int>(final_value)));
            
            GameConfig::getInstance()->passengerMaxCount = final_value;
        }
    });
    
    
    slider = static_cast<Slider*>(node->getChildByName("passengerCreateRateSlider"));
    textField = static_cast<Text*>(node->getChildByName("passengerCreateRateLabel"));
    maxValue = 11.0f;
    minValue = 1.0f;
    current = config->passengerCreateRate;
    slider->setPercent(40);
    textField->setString(convertToString<float>(current));
    slider->addEventListener([=](Ref* sender, Slider::EventType eventType){
        Slider* node = static_cast<Slider*>(sender);
        if(eventType == Slider::EventType::ON_PERCENTAGE_CHANGED){
            float percent       = node->getPercent();
            float final_value   = minValue + percent * (maxValue - minValue) / 100;
            textField->setString((convertToString<float>(final_value)));
            
            GameConfig::getInstance()->passengerCreateRate = final_value;
        }
    });

    slider = static_cast<Slider*>(node->getChildByName("carSpeedSlider"));
    textField = static_cast<Text*>(node->getChildByName("carSpeedLabel"));
    maxValue = 21.0f;
    minValue = 1.0f;
    current = config->carSpeed;
    slider->setPercent(50);
    textField->setString(convertToString<float>(current));
    slider->addEventListener([=](Ref* sender, Slider::EventType eventType){
        Slider* node = static_cast<Slider*>(sender);
        if(eventType == Slider::EventType::ON_PERCENTAGE_CHANGED){
            float percent       = node->getPercent();
            float final_value   = minValue + percent * (maxValue - minValue) / 100;
            textField->setString((convertToString<float>(final_value)));
            
            GameConfig::getInstance()->carSpeed = final_value;
        }
    });
    
    slider = static_cast<Slider*>(node->getChildByName("carCreateRateSlider"));
    textField = static_cast<Text*>(node->getChildByName("carCreateRateLabel"));
    maxValue = 11.0;
    minValue = 1.0f;
    current = config->carCreateRate;
    slider->setPercent(40);
    textField->setString(convertToString<float>(current));
    slider->addEventListener([=](Ref* sender, Slider::EventType eventType){
        Slider* node = static_cast<Slider*>(sender);
        if(eventType == Slider::EventType::ON_PERCENTAGE_CHANGED){
            float percent       = node->getPercent();
            float final_value   = minValue + percent * (maxValue - minValue) / 100;
            textField->setString((convertToString<float>(final_value)));
            
            GameConfig::getInstance()->carCreateRate = final_value;
        }
    });
    
    slider = static_cast<Slider*>(node->getChildByName("carMatchSlider"));
    textField = static_cast<Text*>(node->getChildByName("carMatchLabel"));
    maxValue = 1.0f;
    minValue = 0.0f;
    current = config->carMatchRate;
    slider->setPercent(30);
    textField->setString(convertToString<float>(current));
    slider->addEventListener([=](Ref* sender, Slider::EventType eventType){
        Slider* node = static_cast<Slider*>(sender);
        if(eventType == Slider::EventType::ON_PERCENTAGE_CHANGED){
            float percent       = node->getPercent();
            float final_value   = ((int)((minValue + percent * (maxValue - minValue) / 100) * 10 + 0.5)) / 10.f;
            textField->setString((convertToString<float>(final_value)));
            
            GameConfig::getInstance()->carMatchRate = final_value;
        }
    });
    
    slider = static_cast<Slider*>(node->getChildByName("numberRangerSlider"));
    textField = static_cast<Text*>(node->getChildByName("numberRangerLabel"));
    maxValue = 10;
    minValue = 1;
    current = config->numberRange.y;
    slider->setPercent(50);
    textField->setString(convertToString<int>(current));
    slider->addEventListener([=](Ref* sender, Slider::EventType eventType){
        Slider* node = static_cast<Slider*>(sender);
        if(eventType == Slider::EventType::ON_PERCENTAGE_CHANGED){
            float percent       = node->getPercent();
            int final_value   = minValue + percent * (maxValue - minValue) / 100;
            textField->setString((convertToString<int>(final_value)));
            
            GameConfig::getInstance()->numberRange.y = final_value;
        }
    });
    
    slider = static_cast<Slider*>(node->getChildByName("thingSecRangeSlider"));
    textField = static_cast<Text*>(node->getChildByName("thingSecRangeLabel"));
    maxValue = 30.f;
    minValue = 3.f;
    current = config->passengerThingSec;
    slider->setPercent(50);
    textField->setString(convertToString<float>(current));
    slider->addEventListener([=](Ref* sender, Slider::EventType eventType){
        Slider* node = static_cast<Slider*>(sender);
        if(eventType == Slider::EventType::ON_PERCENTAGE_CHANGED){
            float percent       = node->getPercent();
            float final_value   = (int)((minValue + percent * (maxValue - minValue) / 100) * 10 + 0.5) / 10.f;
            textField->setString((convertToString<float>(final_value)));
            
            GameConfig::getInstance()->passengerThingSec = final_value;
        }
    });
    
    slider = static_cast<Slider*>(node->getChildByName("passengerLeaveSecSlider"));
    textField = static_cast<Text*>(node->getChildByName("passengerLeaveSecLabel"));
    maxValue = 30.f;
    minValue = 3.f;
    current = config->passengerLeaveSec;
    slider->setPercent(50);
    textField->setString(convertToString<float>(current));
    slider->addEventListener([=](Ref* sender, Slider::EventType eventType){
        Slider* node = static_cast<Slider*>(sender);
        if(eventType == Slider::EventType::ON_PERCENTAGE_CHANGED){
            float percent       = node->getPercent();
            float final_value   = (int)((minValue + percent * (maxValue - minValue) / 100) * 10 + 0.5) / 10.f;
            textField->setString((convertToString<float>(final_value)));
            
            GameConfig::getInstance()->passengerLeaveSec = final_value;
        }
    });
    	
    return true;
}

void HelloWorld::menuCloseCallback(Ref* sender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
