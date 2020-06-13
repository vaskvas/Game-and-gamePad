#include "startMenu.h"
#include "gui\mainGUI.h"
#include "AudioEngine.h"

USING_NS_CC;

Scene* startMenu::createScene()
{
    return startMenu::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in startMenu.cpp\n");
}

bool startMenu::init()
{
    if (!Scene::init())
    {
        return false;
    }
    std::srand(time(0));

    AudioEngine::play2d("audio/mainMenu.mp3", true);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Director::getInstance()->setClearColor(Color4F(Color4B(78,179,255,255))); 

    auto mainMenu = mainGUi::createGUI();
    mainMenu->setPosition(Vec2::ZERO);
    this->addChild(mainMenu, 1);
    auto label = Label::createWithTTF("PartyGame", "fonts/8bit.ttf", 24);
    if (label) {
        label->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - label->getContentSize().height));
        this->addChild(label, 1);
    }

    auto description = Label::createWithTTF(u8"Пожалйста просканируйте QR Код!", "fonts/8bit.ttf",40);
    description->enableBold();
    if (description) {
        description->setAnchorPoint(Vec2(0.5, 1.5));
        description->setPosition(Vec2((label->getContentSize().width / 2.f), label->getContentSize().height / 2.f));
        label->addChild(description, 1);
    }
 
    //this->setPosition(Vec2((getContentSize().width), getContentSize().height));
    this->addChild(createBackground());
    return true;
}

Node* startMenu::createBackground() {
    auto node = Node::create();
    node->setName("background");
    node->setContentSize(Director::getInstance()->getVisibleSize());
    for (int j = 0; j < 2; j++) {
        for (int i = 1; i <= 7; i++) {
            auto cloud = Sprite::create("background/" + std::to_string(i) + ".png");
            if (cloud) {
                cloudAnim(cloud, node);
            }
        }
    }
    return node;
}

void startMenu::cloudAnim(Sprite* sprite, Node* parent, bool firstTime) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto startPos = (float)(std::rand() % (int)(visibleSize.width) + (int)sprite->getContentSize().width);
    auto posX = visibleSize.width + origin.x - (firstTime ? startPos : 0.f); 
    auto posY = origin.y + visibleSize.height - (float)(std::rand() % (int)(visibleSize.height) + (int)sprite->getContentSize().height);
    sprite->setPosition(Vec2(posX, posY));
    sprite->setAnchorPoint(Vec2(0, 0));
    parent->addChild(sprite);

    auto moveAction = MoveBy::create((float)(std::rand() % 20 + 15), Vec2(-posX -sprite->getContentSize().width , 0));
    auto EndCallback = CallFunc::create([this, parent, sprite]() {
        sprite->getParent()->removeChild(sprite);
        cloudAnim(sprite, parent, false);
        });
    auto seq = Sequence::create(moveAction, EndCallback, nullptr);
    sprite->runAction(seq);
}


void startMenu::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
