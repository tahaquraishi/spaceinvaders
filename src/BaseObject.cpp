#include "BaseObject.h"

// constructor of baseobject
BaseObject::BaseObject() {
	trans = ofVec3f(0, 0, 0); // sets position of object
	scale = ofVec3f(1, 1, 1); // sets scale of object
	rot = 0; // sets rotation of object
}

// sets position of object
void BaseObject::setPosition(glm::vec3 pos) {
	trans = pos; // sets the new position of the object given a position
}