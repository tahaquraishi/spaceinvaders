#pragma once

#include "ofMain.h"
#include "BaseObject.h"
#include "SpriteSystem.h"

//  general purpose emitter class for emitting sprites
class Emitter : public BaseObject {

public:
	Emitter(SpriteSystem *); // constructor (uses a sprite system)
	void loadSound(); // loads sound for emitter
	void Emitter::move(int type); // sets motion path for the emitter
	void setChildSize(float w, float h) { childWidth = w; childHeight = h; } // sets child size of emitter
	void draw(); // draws the emitter
	void start(); // starts the emitter
	void stop(); // stops the emitter (freeze)
	void setLifespan(float); // sets lifespan of the emitter
	void setVelocity(ofVec3f); // sets velocity of the emitter
	void setChildImage(ofImage); // sets the child image of the emitter (objects being emitted)
	void setImage(ofImage); // sets the image of the emitter
	void setRate(float); // sets the rate of burst of the emitter
	void update(); // updates the emitter
	SpriteSystem *sys; // the sprite system being used by the emitter
	float rate; // rate of the emitter
	glm::vec3 velocity; // velocity of the emitter
	float lifespan; // lifespan of the emitter
	bool started; // if the emitter has started bursting
	float lastSpawned; // last spawned object from emitter
	ofImage childImage; // child image of the emitter
	ofImage image; // image of the emitter
	bool drawable; // if the emitter should be drawn
	bool haveChildImage; // if the emitter has a child iamge
	bool haveImage; // if the emitter has an image
	float width, height; // width and height of the emitter
	bool functioning; // if the emitter is currently emitting
	ofSoundPlayer sound; // sound of the emitter
	float childWidth, childHeight; // child width and height of emitter
	bool hasSound = false; // if the emitter has sound
	int count; // used for motion path
	bool reachedMax = false; // if max distance has been reached for motion path
	ofVec3f path; // path of emitter
	int motionPath; // motion path type
};