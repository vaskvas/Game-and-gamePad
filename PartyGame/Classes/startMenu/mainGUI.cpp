#include "mainGUI.h"
#include "startMenu.h"
#include "AudioEngine.h"

USING_NS_CC;

Menu* mainGUi::createGUI(bool allGUI)
{
    mainGUi* ret = new (std::nothrow) mainGUi();
    if (ret)
    {
        ret->setGui(allGUI);
        if (ret->init()) {
            ret->autorelease();
            return ret;
        }
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in mainGUi.cpp\n");
}

bool mainGUi::init()
{
    if (!Menu::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Director::getInstance()->setClearColor(Color4F(Color4B(78, 179, 255, 255)));

    auto closeItem = MenuItemImage::create("hud/close.png", "hud/close_click.png", CC_CALLBACK_1(mainGUi::menuCloseCallback, this));
    if (closeItem) {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
        float y = origin.y + visibleSize.height - closeItem->getContentSize().width / 2;
        closeItem->setPosition(Vec2(x, y));
    }
    this->addChild(closeItem);

    if (isAllGUI) {
        auto backItem = MenuItemImage::create("hud/back.png", "hud/back_click.png", CC_CALLBACK_1(mainGUi::menuBackCallback, this));
        if (backItem) {
            float x = origin.x + visibleSize.width - backItem->getContentSize().width - closeItem->getContentSize().width;
            float y = origin.y + visibleSize.height - backItem->getContentSize().width / 2;
            backItem->setPosition(Vec2(x, y));
        }
        this->addChild(backItem);
        return true;
    }
    return true;
}

void mainGUi::changeSoundCallback(cocos2d::Ref* pSender)
{
    if (isEnabledSound)
        AudioEngine::pauseAll();
    else
        AudioEngine::resumeAll();
    isEnabledSound = !isEnabledSound;
}

void mainGUi::menuBackCallback(Ref* pSender)
{
    auto scene = startMenu::createScene();
    Director::getInstance()->replaceScene(TransitionSlideInT::create(1.5f, scene));
}


void mainGUi::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}
