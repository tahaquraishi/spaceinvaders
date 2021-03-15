#pragma once

#include "ofMain.h"
#include "BaseObject.h"

// general sprite class
class Sprite : public BaseObject {

public:
	Sprite(); // constructor
	void draw(); // draws the sptire
	float age(); // age of sprite
	void setImage(ofImage); // sets image of sprite
	float speed; // speed of sprite
	glm::vec3 velocity; // velocity vector for sprite
	ofImage image; // image of sprite
	float birthtime; // birthime of sprite
	float lifespan; // lifespan of sprite
	bool haveImage; // checks if the sprite has an image
	float width, height; // width and height of sprite

};