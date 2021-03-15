#include "SpriteSystem.h"

// add a sprite to the sprite system
void SpriteSystem::add(Sprite s) {
	sprites.push_back(s); // adds sprite to sprite system
}

// remove a sprite from the sprite system
void SpriteSystem::remove(int i) {
	sprites.erase(sprites.begin() + i); // removes sprite from the sprite system
}

// update the sprite system by checking which sprites have exceeded their lifespan (and deleting)
// also the sprite is moved to it's next location based on velocity and direction
void SpriteSystem::update() {
	if (sprites.size() == 0) return;
	vector<Sprite>::iterator s = sprites.begin();
	vector<Sprite>::iterator tmp;
	// check which sprites have exceed their lifespan and delete from list
	// When deleting multiple objects from a vector while traversing at the same time, use an iterator
	while (s != sprites.end()) {
		if (s->lifespan != -1 && s->age() > s->lifespan) {
			tmp = sprites.erase(s);
			s = tmp;
		}
		else s++;
	}
	// move sprite
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].trans += sprites[i].velocity / ofGetFrameRate();
	}
}

// remove all sprites within a given dist of point, return number removed
int SpriteSystem::removeNear(ofVec3f point, float dist) {
	vector<Sprite>::iterator s = sprites.begin();
	vector<Sprite>::iterator tmp;
	int count = 0;

	while (s != sprites.end()) {
		ofVec3f v = s->trans - point;
		if (v.length() < dist) {
			tmp = sprites.erase(s); // remove sprites near given dist of point
			count++;
			s = tmp;
		}
		else s++;
	}
	return count; // return total number of sprites removed
}

// render all the sprites
void SpriteSystem::draw() {
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].draw(); // draws the sprites
	}
}
