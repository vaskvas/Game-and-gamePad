#include "SnakePart.h"

USING_NS_CC;

SnakePart* SnakePart::create(PolygonInfo &polygon){
	SnakePart* s = new (std::nothrow) SnakePart();

	if (s && s->initWithPolygon(polygon))
	{
		s->autorelease();
		return s;
	}
	CC_SAFE_DELETE(s);
	return nullptr;
}

bool SnakePart::init(){
	if (!Sprite::init()){
		return false;
	}

	return true;

}
