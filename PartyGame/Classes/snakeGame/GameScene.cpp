#include "GameScene.h"
#include "modules/eventListSystem/eventListSystem.h"
#include "startMenu/mainGUI.h"

USING_NS_CC;

std::vector<std::string> GameScene::gamePlayers;

Scene* GameScene::createScene(std::vector<std::string> aPlayers)
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	layer->addPlayers(aPlayers);
	scene->addChild(layer);
	return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto background = Sprite::create("background.jpg");
	background->setPosition(POSITION_CENTER_CENTER);

	this->addChild(background, 0);

	auto gui = mainGUi::createGUI(true);
	gui->setPosition(Vec2::ZERO);
	this->addChild(gui, 1);

	auto playSprite = Sprite::create("play_button.png");
	playSprite->setPosition(POSITION_CENTER_CENTER);
	this->addChild(playSprite, 1);

	food = Food::create();
	addChild(food);

	eventListSystem::getInstance()->addEventCallback("GameScene", [this, gui, playSprite](std::string idPlayer, eventListSystem::eEventList aEvent) {
		switch (aEvent)
		{
		case eventListSystem::eEventList::LEFT_PRESSED:
			break;
		case eventListSystem::eEventList::RIGHT_PRESSED:
			break;
		case eventListSystem::eEventList::A_PRESSED:
			if (!started) {
				started = true;
				auto callAction = CallFunc::create([this, gui, playSprite]() {
					gui->setEnabled(false);
					gui->setVisible(false);
					for (auto snake : snakes) {
						snake->moveSnake();
						snake->scheduleUpdate();
					}
					removeChild(playSprite);
					eventListSystem::getInstance()->removeEventCallbackById("GameScene");
					});
				this->runAction(callAction);
			}
			break;
		case eventListSystem::eEventList::B_PRESSED:
			break;
		default:
			break;
		}
	});

	return true;
}


void GameScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void GameScene::addPlayers(std::vector<std::string> aPlayers)
{
	if (!aPlayers.empty()) {
		gamePlayers = aPlayers;
	}

	for (auto player : gamePlayers) {
		auto snake = Snake::create();
		snake->setCurrentPlayer(player);
		addChild(snake, 3);
		snake->currentFood = food;
		snakes.push_back(snake);
	}

}
