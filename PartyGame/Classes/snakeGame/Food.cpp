#include "Food.h"

USING_NS_CC;

Food* Food::create(){
	Food* f = new (std::nothrow)Food();

	if (f&& f->init()){
		f->autorelease();
	}
	else{
		CC_SAFE_DELETE(f);
	}

	return f;

}

bool Food::init(){
	if (!Node::init()){
		return false;
	}

	glow = Sprite::create("glow.png");
	addChild(glow);

	sprite = Sprite::create("apple.png");
	addChild(sprite);
	setContentSize(sprite->getContentSize());
	
	reposition();
	rotateGlow();

	return true;
}

void Food::rotateGlow(){
	auto rotate = RotateBy::create(GLOW_ROTATE_SPEED / DIVIDER, 360);
	std::function<void()> rotateGlowLoopFunc = std::bind(&Food::rotateGlowLoop, this);
	auto end = CallFunc::create(rotateGlowLoopFunc);

	glow->runAction(Sequence::create(rotate, end, NULL));
}

void Food::rotateGlowLoop(){
	rotateGlow();
}

void Food::reposition(){
	auto size = this->getContentSize();

	this->setPosition(Vec2(
		random(SCREEN_POSITION_LEFT + size.width, SCREEN_POSITION_RIGHT - size.width),
		random(SCREEN_POSITION_BOTTOM + size.height, SCREEN_POSITION_TOP - size.height)));
}

Sprite* Food::getFoodSprite(){
	return sprite;
}