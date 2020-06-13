#include "startMenu.h"
#include "mainGUI.h"
#include "modules/serverSystem/serverSystem.h"
#include "snakeGame/GameScene.h"
#include "modules/qrCode/qrNode.h"

USING_NS_CC;

std::vector<std::string> startMenu::players;

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

    std::string address = serverSystem::getInstance()->getPublicAddress();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Director::getInstance()->setClearColor(Color4F(Color4B(78,179,255,255))); 

    auto mainMenu = mainGUi::createGUI();
    mainMenu->setPosition(Vec2::ZERO);
    this->addChild(mainMenu, 1);
    auto label = Label::createWithTTF("PartyGame", "fonts/8bit.ttf", 30);
    if (label) {
        label->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - label->getContentSize().height));
        this->addChild(label, 1);
    }

    Node* descNode = Node::create();
    description = Label::createWithTTF(u8"Пожалйста просканируйте QR Код!", "fonts/8bit.ttf",40);
    if (description && descNode) {
        description->enableBold();
        description->setAnchorPoint(Vec2(0.0, 0.0));
        descNode->setAnchorPoint(Vec2(0.5, 1.5));
        descNode->setPosition(Vec2((label->getContentSize().width / 2.f), label->getContentSize().height / 2.f));
        label->addChild(descNode, 1);
        descNode->setContentSize(Size(Vec2(description->getContentSize().width, description->getContentSize().height)));
        descNode->addChild(description, 1);
    }

    auto firstPlayer = Label::createWithTTF(u8"1 игрок", "fonts/8bit.ttf", 40);
    firstPlayer->enableBold();
    if (firstPlayer) {
        firstPlayer->setAnchorPoint(Vec2(0.0, 1.5));
        firstPlayer->setPosition(Vec2(0.f, description->getContentSize().height / 2.f));
        auto firstPlayerQrNode = qrNode::createQrNode("first:" + address);
        if (firstPlayerQrNode) {
            firstPlayerQrNode->setPosition(Vec2((firstPlayer->getContentSize().width - firstPlayerQrNode->getContentSize().width) / 2.f,
                                                -firstPlayer->getContentSize().height));
            firstPlayer->addChild(firstPlayerQrNode, 1);
        }
        descNode->addChild(firstPlayer, 1);
    }

    auto secondPlayer = Label::createWithTTF(u8"2 игрок", "fonts/8bit.ttf", 40);
    secondPlayer->enableBold();
    if (secondPlayer) {
        secondPlayer->setAnchorPoint(Vec2(0.5, 1.5));
        secondPlayer->setPosition(Vec2((description->getContentSize().width / 2.f), description->getContentSize().height / 2.f));
        auto secondPlayerQrNode = qrNode::createQrNode("second:" + address);
        if (secondPlayerQrNode) {
            secondPlayerQrNode->setPosition(Vec2((secondPlayer->getContentSize().width - secondPlayerQrNode->getContentSize().width) / 2.f,
                                                 -secondPlayer->getContentSize().height));
            secondPlayer->addChild(secondPlayerQrNode, 1);
        }
        descNode->addChild(secondPlayer, 1);
    }

    auto thridPlayer = Label::createWithTTF(u8"3 игрок", "fonts/8bit.ttf", 40);
    thridPlayer->enableBold();
    if (thridPlayer) {
        thridPlayer->setAnchorPoint(Vec2(1.0, 1.5));
        thridPlayer->setPosition(Vec2((description->getContentSize().width), description->getContentSize().height / 2.f));
        auto thridPlayerQrNode = qrNode::createQrNode("thrid:" + address);
        if (thridPlayerQrNode) {
            thridPlayerQrNode->setPosition(Vec2((thridPlayer->getContentSize().width - thridPlayerQrNode->getContentSize().width) / 2.f,
                                                -thridPlayer->getContentSize().height));
            thridPlayer->addChild(thridPlayerQrNode, 1);
        }
        descNode->addChild(thridPlayer, 1);
    }

    auto landSprite = Sprite::create("background/land.png");
    landSprite->setPosition(Vec2(origin.x + visibleSize.width / 2, landSprite->getContentSize().height/2.f));
    this->addChild(landSprite);
    this->addChild(createBackground());
    
    if (!players.empty())
        setStartText();

    callback = std::bind(&startMenu::playerEventCallback, this, std::placeholders::_1, std::placeholders::_2);
    eventListSystem::getInstance()->addEventCallback("startMenu", callback);
    return true;
}

void startMenu::playerEventCallback(std::string idPlayer, eventListSystem::eEventList aEvent) {
    switch (aEvent)
    {
    case eventListSystem::eEventList::CONNECTED:
        if(std::find(players.begin(), players.end(), idPlayer) == players.end())
            players.push_back(idPlayer);
        setStartText();
        break;
    case eventListSystem::eEventList::A_PRESSED:
        if (!started) {
            started = true;
            auto callAction = CallFunc::create([this]() {
                auto snakeScene = GameScene::createScene(players);
                Director::getInstance()->replaceScene(TransitionSlideInB::create(1.5f, snakeScene));
                eventListSystem::getInstance()->removeEventCallbackById("startMenu");
                });
            this->runAction(callAction);
        }
        break;
    default:
        break;
    }
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
    auto posY = origin.y + visibleSize.height - ((float)(std::rand() % (int)(visibleSize.height) + (int)sprite->getContentSize().height)) / 2;
    sprite->setPosition(Vec2(posX, posY));
    sprite->setAnchorPoint(Vec2(0, 0));
    parent->addChild(sprite);

    auto moveAction = MoveBy::create((float)(std::rand() % 20 + 15), Vec2(-posX - sprite->getContentSize().width, 0));
    auto EndCallback = CallFunc::create([this, parent, sprite]() {
        sprite->getParent()->removeChild(sprite);
        cloudAnim(sprite, parent, false);
        });
    auto seq = Sequence::create(moveAction, EndCallback, nullptr);
    sprite->runAction(seq);
}

void startMenu::setStartText() {
    description->setString(u8"Нажмите А для начала");
    description->setAnchorPoint(Vec2(-0.32, 0.0));
}


void startMenu::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
