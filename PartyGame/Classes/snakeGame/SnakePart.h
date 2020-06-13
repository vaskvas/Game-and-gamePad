#ifndef __SNAKEPART_H__
#define __SNAKEPART_H__

#include "Globals.h"

class SnakePart : public Sprite{
public:

	static SnakePart* create(PolygonInfo&);

	virtual bool init();

	void rotatePart();
	void movePart();

};

#endif //__SNAKEPART_H__