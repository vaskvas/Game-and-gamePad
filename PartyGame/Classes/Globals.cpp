#include "Globals.h"

Size Globals::getVisibleSize(){
	return cocos2d::Director::getInstance()->getVisibleSize();
}

Vec2 Globals::getOrigin(){
	return cocos2d::Director::getInstance()->getVisibleOrigin();
}

Vec2 Globals::getPositionCenterCenter(){
	Size visibleSize = getVisibleSize();
	Vec2 origin = getOrigin();

	return Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
}

Vec2 Globals::getPositionCenterLeft(Node* item){
	Size visibleSize = getVisibleSize();
	Vec2 origin = getOrigin();

	return Vec2(origin.x + item->getContentSize().width/2, origin.y + visibleSize.height / 2);
}
