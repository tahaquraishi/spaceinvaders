#include "Sprite.h"

// constructor of sprite
Sprite::Sprite() {
	speed = 0; // sets speed to 0
	velocity = glm::vec3(0, 0, 0); // sets velocity of sprite
	lifespan = -1; // immortal
	birthtime = 0; // birthtime set to 0
	bSelected = false; // not currently selected
	haveImage = false; // doesn't have an image upon creation
	width = 1; // width of sprite
	height = 1; // height of sprite
}

// return a sprite's age in milliseconds
float Sprite::age() {
	return (ofGetElapsedTimeMillis() - birthtime); // age in milliseconds
}

// set an image for the sprite
// if you don't set one, a rectangle gets drawn
void Sprite::setImage(ofImage img) {
	image = img; // set image attribute of sprite to given image
	haveImage = true; // set the image check to true
	width = image.getWidth(); // width set to width of image
	height = image.getHeight(); // height set to height of image
}


// renders the sprite
void Sprite::draw() {
	ofSetColor(255, 255, 255, 255);
	if (haveImage) { 
		image.draw(-width / 2.0 + trans.x, -height / 2.0 + trans.y); // draw image centered and add in translation amount
	}
	else {
		ofSetColor(255, 0, 0);
		ofDrawRectangle(-width / 2.0 + trans.x, -height / 2.0 + trans.y, width, height); // in case no image is supplied, draw something
	}
}
