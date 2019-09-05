//
//  UILayer.cpp
//  EffectEditor
//
//  Created by Matrix on 16/7/26.
//
//

#include "UILayer.hpp"
#include "editor-support/cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MainLayer.hpp"
#include "EngineEx/ShaderSprite.hpp"
#include "Effect/LinkEffect.hpp"
#include "Effect/RingEffect.hpp"
#include "Effect/ThunderLinkEffect.hpp"
#include <chrono>
#include "Project/Project.hpp"
#include "MainScene.hpp"
#include "UIControlPropertySlider.hpp"
#include "Message/MessageDispatcher.hpp"
#include "Audio/AudioPlayer.hpp"
#include "Timeline/ActionEx.hpp"
#include "Timeline/BezierPathManager.hpp"
#include "EngineEx/ParticleSystemExt.hpp"
#include "Common/PlatformAdapter.h"
#include "Timeline/ActionDrawBezierPath.hpp"
#include "Timeline/TimelineManager.hpp"
#include "UI/UIParticleEditor.hpp"
#include "UI/UIMenuBar.hpp"
#include "UI/UIProjectPanel.hpp"



#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#import "ARManager.hpp"
#include "ARLayer.hpp"
#endif

NS_EE_BEGIN

USING_NS_CC;

using namespace cocos2d::ui;

bool UILayer::init(void)
{
    //mContainer = CSLoader::createNode("res/main_scene.csb");
    //auto size = Director::getInstance()->getWinSize();
    //this->addChild(mContainer, (int)SPRITE_ZORDER::UI);
    
    //bindListener();
    
    //mProjectView = new UIProjectView(mContainer->getChildByName("project_view"));
    //mPropertyView = new UIPropertyView(mContainer->getChildByName("property_view"));
    
    
    mState = UI_STATE::NONE;

	Size winSize = Director::getInstance()->getWinSize();
	UIMenuBar *pMenuBar = UIMenuBar::create();
	addChild(pMenuBar, (int)SPRITE_ZORDER::UI + 1);
	pMenuBar->setPosition(Vec2(0, winSize.height - 20.0f));

	UIProjectPanel *pProjectPanel = UIProjectPanel::create();
	addChild(pProjectPanel);

	auto particleEditor = UIParticleEditor::getInstance();
	addChild(particleEditor);
	particleEditor->setPosition(winSize.width - 400.0f, winSize.height);
    
    return Layer::init();
}

void UILayer::bindListener()
{
    Layout *uiRoot = static_cast<Layout*>(mContainer->getChildByName("root_node"));
    mStatusText = static_cast<Text*>(mContainer->getChildByName("tb_status"));
    
    auto *pTest1Button = static_cast<Button*>(ui::Helper::seekWidgetByName(uiRoot, "bt_test1"));
    pTest1Button->addTouchEventListener(CC_CALLBACK_2(UILayer::onTest1Event, this));
    auto *pTest2Button = static_cast<Button*>(ui::Helper::seekWidgetByName(uiRoot, "bt_test2"));
    pTest2Button->addTouchEventListener(CC_CALLBACK_2(UILayer::onTest2Event, this));
    auto *pTest3Button = static_cast<Button*>(ui::Helper::seekWidgetByName(uiRoot, "bt_test3"));
    pTest3Button->addTouchEventListener(CC_CALLBACK_2(UILayer::onTest3Event, this));
    
    auto pButton = static_cast<Button*>(ui::Helper::seekWidgetByName(uiRoot, "bt_sprite"));
    pButton->addTouchEventListener(CC_CALLBACK_2(UILayer::onUserTouchEvent, this));
    
    auto penButton = static_cast<Button*>(ui::Helper::seekWidgetByName(uiRoot, "bt_pen"));
    penButton->addTouchEventListener(CC_CALLBACK_2(UILayer::onPenTouchEvent, this));
    
    auto saveButton = static_cast<Button*>(ui::Helper::seekWidgetByName(uiRoot, "bt_save"));
    saveButton->addTouchEventListener(CC_CALLBACK_2(UILayer::onSaveTouchEvent, this));
    
    auto loadButton = static_cast<Button*>(ui::Helper::seekWidgetByName(uiRoot, "bt_load"));
    loadButton->addTouchEventListener(CC_CALLBACK_2(UILayer::onLoadTouchEvent, this));
    
    auto newButton = static_cast<Button*>(ui::Helper::seekWidgetByName(uiRoot, "bt_new"));
    newButton->addTouchEventListener(CC_CALLBACK_2(UILayer::onNewTouchEvent, this));
    
    mDrawPanel =  new UIDrawView(mContainer->getChildByName("draw_panel"));
    mDrawPanel->setVisible(false);
    
    MessageDispatcher::getInstance()->addListener("msg_hue_hue", this, std::bind(&UILayer::onSliderMessage1, this, std::placeholders::_1, std::placeholders::_2));
    MessageDispatcher::getInstance()->addListener("msg_hue_saturation", this, std::bind(&UILayer::onSliderMessage2, this, std::placeholders::_1, std::placeholders::_2));
    MessageDispatcher::getInstance()->addListener("msg_hue_value", this, std::bind(&UILayer::onSliderMessage3, this, std::placeholders::_1, std::placeholders::_2));
    
}

UI_STATE UILayer::getState()
{
    return mState;
}


void UILayer::onTest1Event(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        MainLayer::getInstance()->getSprite("1")->resetTimeUniform();
        MainLayer::getInstance()->getSprite("2")->resetTimeUniform();
    }
}

void UILayer::onTest2Event(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        auto maskSprite = DrawNode::create();
        auto clipingNode = ClippingNode::create(maskSprite);
        auto show = Sprite::create(MainScene::getInstance()->getProjectPath() + "monster.png");
        show->setAnchorPoint(Vec2(0.0f, 0.0f));
        clipingNode->addChild(show);
        clipingNode->setPosition(Vec2(250, 300));
        addChild(clipingNode, (int)SPRITE_ZORDER::MASK);
        maskSprite->drawDot(Vec2(0, 0), 10, Color4F(1.0f, 1.0f, 1.0f, 1.0f));
        
        BezierPointList list = BezierPathManager::getInstance()->getBezierPath(MainScene::getInstance()->getProjectPath() + "monster2.bezier");
        maskSprite->runAction(ActionDrawBezierPath::create(5.0f, list, 10.0f));
    }
    
}

void UILayer::onTest3Event(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        ARManager::getInstance()->showSceneView();
#endif
    }
}

void UILayer::onPenTouchEvent(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        mState = mState == UI_STATE::PEN ? UI_STATE::NONE : UI_STATE::PEN;
        auto pButton = static_cast<Button*>(sender);
        if(mState == UI_STATE::PEN){
            pButton->setColor(Color3B(200, 200, 255));
            mDrawPanel->setVisible(true);
        }else{
            pButton->setColor(Color3B(255, 255, 255));
            mDrawPanel->setVisible(false);
        }
    }
}

void UILayer::onUserTouchEvent(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        AudioPlayer::getInstance()->playMusic(MainScene::getInstance()->getProjectPath() + "soda_crush.wav", true);
    }
}

void UILayer::onSaveTouchEvent(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        
    }
}

void UILayer::onLoadTouchEvent(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        
    }
}

void UILayer::onNewTouchEvent(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        
    }
}

void UILayer::setStatus(const std::string &status)
{
    mStatusText->setString(status);
}

void UILayer::loadProject(ProjectConfig* config)
{
    mProjectView->loadProject(config);
}

void UILayer::onSliderMessage1(void* sender, MessageParam *param)
{
    SliderMessageParam *p = static_cast<SliderMessageParam*>(param);
    MainLayer::getInstance()->getSprite("13")->getGLProgramState()->setUniformFloat("u_hue", p->value);
}
void UILayer::onSliderMessage2(void* sender, MessageParam *param)
{
    SliderMessageParam *p = static_cast<SliderMessageParam*>(param);
    MainLayer::getInstance()->getSprite("13")->getGLProgramState()->setUniformFloat("u_saturation", p->value);
}
void UILayer::onSliderMessage3(void* sender, MessageParam *param)
{
    SliderMessageParam *p = static_cast<SliderMessageParam*>(param);
    MainLayer::getInstance()->getSprite("13")->getGLProgramState()->setUniformFloat("u_value", p->value);
}

void UILayer::setCurrentSprite(ShaderSprite *sprite){
    
}

NS_EE_END
