#include "Emitter.h"

// constructor of emitter needs a spritesystem
Emitter::Emitter(SpriteSystem *spriteSys) {
	sys = spriteSys; // sets sprite system of emitter
	lifespan = 5000; // in milliseconds
	started = false; // emitter has not started
	lastSpawned = 0; // last spawned
	rate = 1; // rate set to 1
	haveChildImage = false; // does not have a child image upon creation
	haveImage = false;  // does not have an image upon creation
	velocity = glm::vec3(100, 100, 0); // sets velocity of emitter
	drawable = true; // is drawable
	width = 1; // width of emitter
	height = 1; // height of emitter
	functioning = false; // currently not firing
	childWidth = 10; // starting width of child is 10
	childHeight = 10; // starting height of child is 10
	hasSound = false; // currently has no sound
	count = 0; // initialize count variable for motion path
	//sound.load("gunshot.wav"); // http://www.simphonics.com/library/WaveFiles/Production%20Wavefiles/Weapons/m4a1_unsil-2.wav is where the sound is downloaded from
}

// loads sound for for emitter
void Emitter::loadSound() {
	sound.load("gunshot.wav"); // load sound
	hasSound = true; // set check for sound to true
}

// draw the emitter if it is drawable
void Emitter::draw() {
	if (drawable) {
		if (haveImage) {
			image.draw(-image.getWidth() / 2.0 + trans.x, -image.getHeight() / 2.0 + trans.y); // draws emitter with an image
		}
		else {
			ofSetColor(0, 0, 200);
			ofDrawRectangle(-width / 2 + trans.x, -height / 2 + trans.y, width, height); // draws a rectangle if it doesn't have an image
		}
	}
	sys->draw(); // draws sprite system
}

// set motion path given type
void Emitter::move(int type) {
	if (type == 1) {
		motionPath = 1; // set motion path to 1
	}
	else {
		motionPath = 2; // set motion path to 2 if not 1 to stop errors
	}
}

// update the emitter
// if it has been started, spawn new sprites with initial velocity, lifespan, birthtime
void Emitter::update() {
	if (!started) return;
	if (functioning) {
		float time = ofGetElapsedTimeMillis();
		if ((time - lastSpawned) > (1000.0 / rate)) {
			// spawn a new sprite
			Sprite sprite;
			if (haveChildImage) sprite.setImage(childImage);
			if (hasSound) sound.play(); // plays sound when emitter emits an object
			//cout << "sound played" << endl;
			sprite.velocity = velocity;
			sprite.lifespan = lifespan;

			if (motionPath == 1) { // motion path type 1
				if (!reachedMax) { // if not max
					count = count + 20; // raise count by 20
				}
				else {
					count = count - 20; // otherwise reduce count by 20
				}
				if (count == 100) { // if count is 100, max has been reached
					reachedMax = true;
				}
				else if (count == 0) { // if count is 0, max has not been reached
					reachedMax = false;
				}
//				cout << count << endl;
				path = ofVec3f(trans.x + count, trans.y, trans.z); // absolute value motion
			}
			else { // motion path type 2
				path = ofVec3f(trans.x, trans.y, trans.z); // linear motion
			}
			sprite.setPosition(path); // set path using motion path type 1 or 2

			sprite.birthtime = time; // set birthtime
			sprite.width = childWidth; // set width
			sprite.height = childHeight; // set height
			sys->add(sprite); // add sprite to sys
			lastSpawned = time; // set last spawned time
		}
	}
	sys->update(); // update sys
}

// starts the emitter
void Emitter::start() {
	started = true; // started set to true
	lastSpawned = ofGetElapsedTimeMillis(); // sets last spawned
}

// stops the emitter
void Emitter::stop() {
	started = false; // started set to false
}

// sets lifespan of emitter
void Emitter::setLifespan(float life) {
	lifespan = life; // lifespan set to given lifespan
}

// sets velocity of emitter
void Emitter::setVelocity(ofVec3f v) {
	velocity = v; // velocity set to given velocity
}

// sets child image of emitter
void Emitter::setChildImage(ofImage img) {
	childImage = img; // child image set to given image
	haveChildImage = true; // has a child image
	childWidth = img.getWidth(); // set child width
	childHeight = img.getHeight(); // set child height
}

// sets image of emitter
void Emitter::setImage(ofImage img) {
	image = img; // image set to given image
}

// sets rate of emitter
void Emitter::setRate(float r) {
	rate = r; // rate set to given rate
}