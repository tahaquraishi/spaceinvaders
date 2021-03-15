#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "BaseObject.h"
#include "Sprite.h"
#include "SpriteSystem.h"
#include "Emitter.h"
#include "Particle.h"
#include "ParticleEmitter.h"

typedef enum { MoveStop, MoveLeft, MoveRight, MoveUp, MoveDown } MoveDir; // enums for direction

// particle object
class MovementParticle {
public:
	MovementParticle(); // constructor
	ofVec3f position; // pos of particle
	ofVec3f velocity; // velocity of particle
	ofVec3f acceleration; // accel of particle

	bool upOrDownPressed; // if up or down key is currently pressed

	float damping; // slowing to stop
	float mass; // mass of particle
	void integrate(); // physics of particle
};

class ofApp : public ofBaseApp{

	public:
		void setup(); // initial start of app
		void update(); // updating the screen
		void draw(); // drawing to the screen

		void checkCollisions(); // check collions between bullet and invaders

		void keyPressed(int key); // key pressed listener
		void keyReleased(int key); // key released listener
		void mouseMoved(int x, int y ); // mouse moved listener
		void mouseDragged(int x, int y, int button); // mouse dragged listener
		void mousePressed(int x, int y, int button); // mouse pressed listener
		void mouseReleased(int x, int y, int button); // mouse released listener
		void mouseEntered(int x, int y); // mouse entered listener
		void mouseExited(int x, int y); // mouse existed listener
		void windowResized(int w, int h); // window resized listener
		void dragEvent(ofDragInfo dragInfo); // drag event listener
		void gotMessage(ofMessage msg); // message listener

		void updateSprite(); // updates the sprite
		void moveSprite(MoveDir); // moves the sprite in a givven direction
		void stopSprite(); // stops the sprite
		glm::vec3 start_point; // starting position of the sprite
		glm::vec3 mouse_last; // last mouse position of the sprite
 		MoveDir moveDir; // move direction of the sprite 
		Sprite sprite; // sprite object

		bool startGame = false; // checks if the game has started

		vector<Emitter *> emitters; // collection of emitters

		ofImage defaultImage; // default image
		ofImage invaderType1; // image for left invaders
		ofImage invaderType2; // image for right invaders
		bool imageLoaded; // is image loaded

		bool bHide; // should gui be hidden
		ofxFloatSlider rate; // rate slider
		ofxFloatSlider life; // life slider
		ofxVec3Slider velocity; // velocity slider

		ofxFloatSlider rateL; // rate slider
		ofxFloatSlider lifeL; // life slider
		ofxVec3Slider velocityL; // velocity slider

		ofxFloatSlider rateR; // rate slider
		ofxFloatSlider lifeR; // life slider
		ofxVec3Slider velocityR; // velocity slider

		ofxLabel screenSize; // screensize
		ofxPanel gui; // where sliders are located

		ofImage background; // background of screen

		int score; // current score
		float gameStartTime; // when game begins

		ofSoundPlayer music; // background music

		ParticleEmitter emitter;
		ParticleEmitter ringEmitter;

		// adding forces
		//
		TurbulenceForce *turbForce;
		GravityForce *gravityForce;
		ImpulseRadialForce *radialForce;

		MovementParticle par;
		bool movementKeyDown = false; // if movement keys are currently pressed
};
