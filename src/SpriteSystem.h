#pragma once

#include "ofMain.h"
#include "Sprite.h"

// manages all Sprites in a system
// you can create multiple systems
class SpriteSystem {

public:
	void add(Sprite); // adds sprite to system
	void remove(int); // removes from sprite system
	void update(); // updates sprite system
	void draw(); // draws sprite system
	vector<Sprite> sprites; // collection of sprites
	int removeNear(ofVec3f point, float dist); // remove sprites near given point and distance

};