#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#define COCOS2D_DEBUG 1

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Snake.h"
#include "Food.h"

class GameScene : public cocos2d::Layer
{
public:
	static Scene* createScene(std::vector<std::string> aPlayers);
	virtual bool init();
	void menuCloseCallback(cocos2d::Ref* pSender);

	static std::vector<std::string> gamePlayers;

	void addPlayers(std::vector<std::string> aPlayers);
	CREATE_FUNC(GameScene);

private:
	bool started = false;

	std::vector<Snake*> snakes;
	Food* food;
};

#endif // __HELLOWORLD_SCENE_H__
