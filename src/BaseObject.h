#pragma once

#include "ofMain.h"

// this is a base object that all drawable object inherit from
// it is possible this will be replaced by ofNode when we move to 3D
class BaseObject {

public:
	BaseObject(); // constructor
	glm::vec3 trans, scale; // trans is for object movement and scale is for object size
	float rot; // rotation of object
	bool bSelected; // object is selected or not
	void setPosition(glm::vec3 position); // sets position of object

};