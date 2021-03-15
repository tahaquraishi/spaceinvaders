#include "ofApp.h"

MovementParticle::MovementParticle() {
	position.set(0, 0, 0); // set pos to 0, 0
	velocity.set(0, 0, 0); // set velocity to 0, 0
	acceleration.set(0, 0, 0); // set acceleration to 0, 0
	damping = .98; // slow to stop coefficient set to 0.98
	mass = 1; // mass is set to 1
	upOrDownPressed = false; // up or down key intially not pressed
}

void MovementParticle::integrate() {
	// 2 dimensional (x and y)
	position = position + (velocity * 1 / 60); // update position with velocity taken into account
	velocity = velocity + (acceleration * 1 / 60); // update velocity with acceleration taken into account
	if (!upOrDownPressed) {
		velocity = velocity * damping; // damp if up or down are released
	}
}


//  for each missle check to see which invaders you hit and remove them
void ofApp::checkCollisions() {

	// find the distance at which the two sprites (missles and invaders) will collide
	// detect a collision when we are within that distance.
	Emitter *gun = emitters[0];
	Emitter *invadersL = emitters[1];
	Emitter *invadersR = emitters[2];
//	float collisionDist = gun->childHeight / 6 + invadersL->childHeight / 12;
//	cout << collisionDist << endl;
	// loop through all the missiles, then remove any invaders that are within
	// "collisionDist" of the missiles.  the removeNear() function returns the
	// number of missiles removed.
	for (int i = 0; i < gun->sys->sprites.size(); i++) {
		score += invadersL->sys->removeNear(gun->sys->sprites[i].trans, 25); // add to score upon collision of left emitters
		score += invadersR->sys->removeNear(gun->sys->sprites[i].trans, 25); // add to score upon collision of right invaders
	}
}

// initial start of app
void ofApp::setup() {
	ofSetVerticalSync(true); // turns on V-Sync

	turbForce = new TurbulenceForce(ofVec3f(-20, -20, -20), ofVec3f(20, 20, 20));
	gravityForce = new GravityForce(ofVec3f(0, -10, 0));
	radialForce = new ImpulseRadialForce(1000.0);

	emitter.sys->addForce(turbForce);
	emitter.sys->addForce(gravityForce);
	emitter.sys->addForce(radialForce);

	emitter.setVelocity(ofVec3f(0, 0, 0));
	emitter.setOneShot(true);
	emitter.setEmitterType(RadialEmitter);
	emitter.setGroupSize(2500);

	score = 0; // score is currently 0
	music.load("backgroundmusic.mp3"); // https://www.youtube.com/watch?v=06ZucU7ZP4M is where the background music is from
	background.load("background.png"); // https://chrome.google.com/webstore/detail/dark-space/idmnciiknelklflhdlnkokcoihkidkln is where the background is from
	string filename = "turret.png"; // image name for turret
	if (!sprite.image.load(filename)) { // loads sprite image or exists if it can't
		cout << "ERROR: Can't load image file: " << filename << endl;
		ofExit(1);
	}
	sprite.setImage((ofImage)"turret.png"); // sets sprite image
	start_point = ofVec3f(ofGetWindowWidth() / 2, (ofGetWindowHeight() / 10) * 9);
	sprite.setPosition(start_point); // starting position of sprite
	par.position = start_point;
	sprite.speed = 100; // sprite speed
	moveDir = MoveStop; // currently sprite is stopped

	// load bullet image
	if (defaultImage.load("bullet.png")) { // sets the image of the missile or exists if it can't
		imageLoaded = true;
	}
	else {
		ofExit(1);
	}

	imageLoaded = false;

	// load alien type 1 image
	if (invaderType1.load("alien1.png")) { // sets the image of the missile or exists if it can't
		imageLoaded = true;
	}
	else {
		ofExit(1);
	}

	imageLoaded = false;

	// load alien type 2 image
	if (invaderType2.load("alien2.png")) { // sets the image of the missile or exists if it can't
		imageLoaded = true;
	}
	else {
		ofExit(1);
	}

	// gui setup
	gui.setup();

	// gui for rate
	gui.add(rate.setup("rate (turret)", 5, 1, 10));
	gui.add(rateL.setup("rate (left invaders)", 1, 1, 10));
	gui.add(rateR.setup("rate (right invaders)", 1, 1, 10));

	// gui for life
	gui.add(life.setup("life (turret)", .5, .1, 10));
	gui.add(lifeL.setup("life (left invaders)", 10, .1, 10));
	gui.add(lifeR.setup("life (right invaders)", 10, .1, 10));

	// gui for velocity
	gui.add(velocity.setup("dir (turret)", ofVec3f(0, -500, 0), ofVec3f(-1000, -1000, -1000), ofVec3f(1000, 1000, 1000)));
	gui.add(velocityL.setup("dir (left invaders)", ofVec3f(0, 100, 0), ofVec3f(-1000, -1000, -1000), ofVec3f(1000, 1000, 1000)));
	gui.add(velocityR.setup("dir (right invaders)", ofVec3f(0, 100, 0), ofVec3f(-1000, -1000, -1000), ofVec3f(1000, 1000, 1000)));

	bHide = true; // currently hidden

	// emitter setup and sets its position to the just above the nozzle of the turret
	Emitter *emit = new Emitter(new SpriteSystem());
	emit->setPosition(ofVec3f((ofGetWindowWidth() / 2) + 10, (ofGetWindowHeight() / 10) * 8.5)); // set position to nozzle of turret
	emit->drawable = false; // bullet is not drawable
	//emit->setChildImage(defaultImage);
	emitters.push_back(emit); // add to emitters vector
	emit->loadSound(); // load sound for bullet
	emit->start(); // start bullet 

	// create left invaders
	Emitter *leftInvaders = new Emitter(new SpriteSystem());
	leftInvaders->setPosition(ofVec3f((ofGetWindowWidth() / 5), 0)); // set pos of left invaders
	leftInvaders->drawable = false; // left invaders are not drawable
	leftInvaders->setChildImage(invaderType1); // set child image of left invaders
	emitters.push_back(leftInvaders); // add to emitters vector
	leftInvaders->move(1); // set absolute motion path
	leftInvaders->start(); // start left invaders

	// create right invaders
	Emitter *rightInvaders = new Emitter(new SpriteSystem());
	rightInvaders->setPosition(ofVec3f((ofGetWindowWidth() / 5) * 4, 0)); // set pos of right invaders
	rightInvaders->drawable = false; // right invaders are not drawable
	rightInvaders->setChildImage(invaderType2); // set child image of right invaders
	emitters.push_back(rightInvaders); // add to emitters vector
	rightInvaders->move(2); // set linear motion path
	rightInvaders->start(); // start right invaders

}

// updates the screen
void ofApp::update() {

	par.integrate(); // integrate particle
	sprite.setPosition(par.position);

	Emitter *gun = emitters[0];
	gun->setPosition(par.position);

	ofSeedRandom();
	emitter.setLifespan(1.0);
	emitter.setRate(1.0);
	emitter.setParticleRadius(30);
	emitter.update();


	updateSprite(); // updates sprite

	// update bullet
	Emitter *emit = emitters[0];
	emit->setRate(rate); // set bullet rate
	emit->setLifespan(life * 1000); // set bullet life
	emit->setVelocity(ofVec3f(velocity->x, velocity->y, velocity->z)); // set bullet velocity
	emit->update(); // update bullet

	// update left invaders
	Emitter *leftInvaders = emitters[1];
	leftInvaders->setRate(rateL); // set left invaders rate
	leftInvaders->setLifespan(lifeL * 1000); // set left invaders life
	leftInvaders->setVelocity(ofVec3f(velocityL->x, velocityL->y, velocityL->z)); // set left invaders velocity
	leftInvaders->update(); // update left velocity

	// update right invaders
	Emitter *rightInvaders = emitters[2];
	rightInvaders->setRate(rateR); // set right invaders rate
	rightInvaders->setLifespan(lifeR * 1000); // set right invaders life
	rightInvaders->setVelocity(ofVec3f(velocityR->x, velocityR->y, velocityR->z)); // set right invaders velocity
	rightInvaders->update(); // update right velocity

	checkCollisions(); // check collision

	// increase velocity every ~10 seconds
	float t = ofGetElapsedTimeMillis();
	int timeIteration = 10 * 1000;
	int timeElapsed = t - gameStartTime;
	if (timeElapsed % timeIteration == 0) {
		velocityL = ofVec3f(0, velocityL->y + 50, 0); // increase speed by 50 for left invaders
		velocityR = ofVec3f(0, velocityR->y + 50, 0); // increase speed by 50 for right invaders 
		emitter.sys->reset();
		emitter.start();
	}

}

// updates sprites
void ofApp::updateSprite() {
	float dist = sprite.speed * 1 / ofGetFrameRate();
	ofVec3f dir;
	// moves the sprite in a given direction
	switch (moveDir) {
	case MoveUp:
		dir = ofVec3f(0, -dist, 0);
		break;
	case MoveDown:
		dir = ofVec3f(0, dist, 0);
		break;
	case MoveLeft:
		dir = ofVec3f(-dist, 0, 0);
		break;
	case MoveRight:
		dir = ofVec3f(dist, 0, 0);
		break;
	}
	// only moves the sprite if the next move keeps the sprite in bounds
	if (((sprite.trans.x + dir.x) >= 30) && ((sprite.trans.x + dir.x) <= 590) && ((sprite.trans.y + dir.y) <= 940) && ((sprite.trans.y + dir.y) >= 600)) {
		sprite.setPosition(sprite.trans + dir);
		Emitter *emitter = emitters[0];
		glm::vec3 emitterPosition = emitter->trans;
		emitter->setPosition(emitterPosition + dir);
	}
	//cout << "x: " << sprite.trans.x << endl;
	//cout << "y: " << sprite.trans.y << endl;
}

// moves sprite
void ofApp::moveSprite(MoveDir dir) {
	moveDir = dir; // direction set to given direction
}

// stops sprite
void ofApp::stopSprite() {
	moveDir = MoveStop; // direction set to stop
}

// draws to the screen
void ofApp::draw() {
	background.draw(0, 0); // draws background
	sprite.draw(); // draws sprite

	ofPushMatrix();
	ofTranslate(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2);
	emitter.draw();
	ofPopMatrix();


	for (int i = 0; i < emitters.size(); i++) { // draws emitters
		emitters[i]->draw();
	}
	if (!bHide) { // draws gui if not hidden
		gui.draw();
	}
	string scoreText;
	scoreText += "Score: " + std::to_string(score);
	ofDrawBitmapString(scoreText, ofPoint(10, 20)); // display score on top left
}

// key pressed listener
void ofApp::keyPressed(int key) {
	switch (key) {
	case ' ':
		if (!startGame) { // start game if not already started
			startGame = true;
			music.play(); // begin background music
			gameStartTime = ofGetElapsedTimeMillis(); // start time for game time
			Emitter *leftInvaders = emitters[1];
			Emitter *rightInvaders = emitters[2];
			leftInvaders->functioning = true; // begin emitting left invaders
			rightInvaders->functioning = true; // begin emitting right invaders
			cout << "game started" << endl;
		}
		else { // fire missiles
			Emitter *emit = emitters[0];
			emit->functioning = true; // start bullets shooting
		}
		break;
	case OF_KEY_LEFT:
		if (startGame) { // move left if started
			movementKeyDown = true;
			par.upOrDownPressed = true;
			par.acceleration.set(ofVec3f(-20, 0, 0)); // set acceleration to use heading vector if space is pressed
			cout << "move left" << endl;
		}
		else {
			cout << "game not started" << endl;
		}
		break;
	case OF_KEY_RIGHT:
		if (startGame) { // move right if started
			movementKeyDown = true;
			par.upOrDownPressed = true;
			par.acceleration.set(ofVec3f(20, 0, 0)); // set acceleration to use heading vector if space is pressed
			cout << "move right" << endl;
		}
		else {
			cout << "game not started" << endl;
		}
		break;
	case OF_KEY_UP:
		if (startGame) { // move up if started
			movementKeyDown = true;
			par.upOrDownPressed = true;
			par.acceleration.set(ofVec3f(0, -20, 0)); // set acceleration to use heading vector if space is pressed
			cout << "move up" << endl;
		}
		else {
			cout << "game not started" << endl;
		}
		break;
	case OF_KEY_DOWN:
		if (startGame) { // move down if started
			movementKeyDown = true;
			par.upOrDownPressed = true;
			par.acceleration.set(ofVec3f(0, 20, 0)); // set acceleration to use heading vector if space is pressed
			cout << "move down" << endl;
		}
		else {
			cout << "game not started" << endl;
		}
		break;
	default:
		break;
	}

}

// key released listener
void ofApp::keyReleased(int key) {
	switch (key) {
	case ' ':
		if (true) {
			Emitter *emit = emitters[0];
			emit->functioning = false; // stop bullets from shooting
		}
		break;
	case OF_KEY_LEFT:
		if (startGame) { // stop sprite if started
			cout << "move left stopped" << endl;
			movementKeyDown = false;
			par.upOrDownPressed = false;
			par.acceleration.set(0, 0, 0); // acceleration set to 0 when down let go
		}
		break;
	case OF_KEY_RIGHT:
		if (startGame) { // stop sprite if started
			cout << "move right stopped" << endl;
			movementKeyDown = false;
			par.upOrDownPressed = false;
			par.acceleration.set(0, 0, 0); // acceleration set to 0 when down let go
		}
		break;
	case OF_KEY_UP:
		if (startGame) { // stop sprite if started
			cout << "move up stopped" << endl;
			movementKeyDown = false;
			par.upOrDownPressed = false;
			par.acceleration.set(0, 0, 0); // acceleration set to 0 when down let go
		}
		break;
	case OF_KEY_DOWN:
		if (startGame) { // stop sprite if started
			cout << "move down stopped" << endl;
			movementKeyDown = false;
			par.upOrDownPressed = false;
			par.acceleration.set(0, 0, 0); // acceleration set to 0 when down let go
		}
		break;
	case 'h':
		bHide = !bHide; // hides the gui
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

// mouse dragged listener
void ofApp::mouseDragged(int x, int y, int button) {
	if (startGame) {
		ofPoint mouse_cur = ofPoint(x, y);
		ofVec3f delta = mouse_cur - mouse_last;
		// makes sure the next position of sprite is within bounds
		if (((sprite.trans.x + delta.x) >= 30) && ((sprite.trans.x + delta.x) <= 590) && ((sprite.trans.y + delta.y) <= 940) && ((sprite.trans.y + delta.y) >= 600)) {
			sprite.setPosition(sprite.trans + delta);
			// sets the emitter location of missle launcher to move with sprite
			Emitter *emitter = emitters[0];
			glm::vec3 emitterPosition = emitter->trans;
			emitter->setPosition(emitterPosition + delta);
		}
		mouse_last = mouse_cur;
	}
}

// mouse pressed listener
void ofApp::mousePressed(int x, int y, int button) {
	mouse_last = ofPoint(x, y); // stores last mouse pressed location
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------  
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
