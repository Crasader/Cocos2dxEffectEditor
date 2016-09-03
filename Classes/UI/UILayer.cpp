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
#include "ShaderSprite.hpp"
#include "LinkEffect.hpp"
#include "RingEffect.hpp"
#include "ThunderLinkEffect.hpp"
#include <chrono>
#include "DrawLineLayer.hpp"
#include "Project.hpp"
#include "MainScene.hpp"
#include "UIControlPropertySlider.hpp"
#include "MessageDispatcher.hpp"

NS_EE_BEGIN

USING_NS_CC;

using namespace cocos2d::ui;

bool UILayer::init(void)
{
    mContainer = CSLoader::createNode("res/main_scene.csb");
    auto size = Director::getInstance()->getWinSize();
    this->addChild(mContainer, (int)SPRITE_ZORDER::UI);
    
    bindListener();
    
    mProjectView = new UIProjectView(mContainer->getChildByName("project_view"));
    mPropertyView = new UIPropertyView(mContainer->getChildByName("property_view"));
    
    auto lineLayer = DrawLineLayer::create();
    addChild(lineLayer);
    
    mState = UI_STATE::NONE;
    
    return Layer::init();
}

void UILayer::bindListener()
{
    Layout *uiRoot = static_cast<Layout*>(mContainer->getChildByName("root_node"));
    mStatusText = static_cast<Text*>(mContainer->getChildByName("tb_status"));
    
    Button *pButton = static_cast<Button*>(ui::Helper::seekWidgetByName(uiRoot, "bt_sprite"));
    pButton->addTouchEventListener(CC_CALLBACK_2(UILayer::onUserTouchEvent, this));
    
    auto penButton = static_cast<Button*>(ui::Helper::seekWidgetByName(uiRoot, "bt_pen"));
    penButton->addTouchEventListener(CC_CALLBACK_2(UILayer::onPenTouchEvent, this));
    
    auto saveButton = static_cast<Button*>(ui::Helper::seekWidgetByName(uiRoot, "bt_save"));
    saveButton->addTouchEventListener(CC_CALLBACK_2(UILayer::onSaveTouchEvent, this));
    
    MessageDispatcher::getInstance()->addListener("msg_hue_hue", this, std::bind(&UILayer::onSliderMessage1, this));
}

UI_STATE UILayer::getState()
{
    return mState;
}

void UILayer::onPenTouchEvent(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        mState = mState == UI_STATE::PEN ? UI_STATE::NONE : UI_STATE::PEN;
        auto pButton = static_cast<Button*>(sender);
        if(mState == UI_STATE::PEN){
            pButton->setColor(Color3B(200, 200, 255));
        }else{
            pButton->setColor(Color3B(255, 255, 255));
        }
    }
}

void UILayer::onUserTouchEvent(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        
        MainLayer::getInstance()->getSprite("13")->setColor(Color3B(0, 255, 0));
        
        
        /*std::string csbFile = "res/horizontal_effect2.csb";
        auto horizontalEffect = cocos2d::CSLoader::createNode(csbFile);
        auto timeLine = CSLoader::createTimeline(csbFile);
        addChild(horizontalEffect, 9999999);
        horizontalEffect->setPosition(Vec2(300, 300));
        horizontalEffect->runAction(timeLine);
        timeLine->setLastFrameCallFunc([horizontalEffect](){
            horizontalEffect->removeFromParent();
        });
        timeLine->gotoFrameAndPlay(0, false);
         */
        // ===================
        
        /*auto shaderSprite = MainLayer::getInstance()->getSprite("1");
        auto ringEffect = RingEffect::create(300.0f, 2.0f);
        addChild(ringEffect);
        ringEffect->setPosition(shaderSprite->getPosition());
         */
        
        /*shaderSprite->runAction(Sequence::create(DelayTime::create(2.0f),
                                                                       MoveBy::create(2.0f, Vec2(400.0f, 0.0f)),
                                                                       DelayTime::create(1.0f),
                                                                       MoveBy::create(2.0f, Vec2(-400.0f, 0.0f)),
                                                                       RemoveSelf::create(true), NULL));
          */
        //shaderSprite->resetTimeUniform();
        
        //auto sprite2 = MainLayer::getInstance()->getSprite("2");
        //auto link = LinkEffect::create(sprite2, shaderSprite, 500.0f);
        //addChild(link);
        
        /*auto test = ParticleSystemQuad::create("res/link_light.plist");
        addChild(test, 99999);
        test->setAnchorPoint(Vec2(0.0f, 0.0f));
        test->setPosition(shaderSprite->getPosition());
        test->setRotation(60);*/
    }
}

void UILayer::onSaveTouchEvent(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        if(MainScene::getInstance()->getCurrentProject()->saveProject()){
            setStatus("Save Complete!");
        }else{
            setStatus("Save Failed!");
        }
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

void onSliderMessage1(void* sender, MessageParam *param)
{
    SliderMessageParam *p = static_cast<SliderMessageParam*>(param);
    MainLayer::getInstance()->getSprite("13")->getGLProgramState()->setUniformFloat("u_hue", p->value);
}
void onSliderMessage2(void* sender, MessageParam *param)
{
    
}
void onSliderMessage3(void* sender, MessageParam *param)
{
    
}

NS_EE_END