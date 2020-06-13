#ifndef __FOOD_H__
#define __FOOD_H__

#include "Globals.h"

class Food : public Node{
public:

	enum FoodProperties{
		DIVIDER = 1000,

		GLOW_ROTATE_SPEED = 3000
	};

	static Food* create();
	virtual bool init();

	void reposition();

	Sprite* getFoodSprite();

private:
	Sprite* glow;
	Sprite* sprite;

	void initializeSprites();

	void rotateGlow();
	void rotateGlowLoop();

};

#endif // __FOOD_H__