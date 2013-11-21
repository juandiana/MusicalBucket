#include "ball.h"



Ball::Ball(ofColor color, vector<Ball>* balls, int numBall, ofxMidiOut* midiOut, float waveSpeed, int pitch) {
	this->color = color;		
	this->balls = balls;
	this->numBall = numBall;
	this->midiOut = midiOut;	
	this->waveRadiuses = vector<float>();
	this->waveSpeed = waveSpeed;
	this->timeout = 0;
	this->pitch = pitch; 
}

int Ball::getNumBall() {
	return numBall;
}

void Ball::draw(float time) {
	ofEnableAlphaBlending();	

	ofSetColor(color);
	ofCircle(x, y, RADIUS);

	for (int i = 0; i < waveRadiuses.size(); i++) {		
		waveRadiuses[i] += waveSpeed / 3 * time;		

		float maxRadius = 900;
		float alpha = 150 * (1 - waveRadiuses[i] / maxRadius);
		ofSetColor(color, alpha);
		ofCircle(x, y, waveRadiuses[i]);
		ofSetColor(255);

		for (int j = 0; j < balls->size(); j++) {
			Ball otherBall = (*balls)[j];

			if (otherBall.getNumBall() != getNumBall()) {
				double distance = calculateDistance(x, y, otherBall.getX(), otherBall.getY());

				if (distance > waveRadiuses[i] - 10 && distance < waveRadiuses[i] + 10) {
					otherBall.playSound(distance);							
				}
			}
		}				

		if (waveRadiuses[i] > maxRadius) {
			waveRadiuses.erase(waveRadiuses.begin());			
		}
	}
}

double Ball::calculateDistance(int x1, int y1, int x2, int y2) {
	return sqrt((float) ((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)));
}

void Ball::playSound(float distance) {
	float maxDistanceToHaveVelocity = 800;

	float velocityFactor;

	if (distance > maxDistanceToHaveVelocity) {
		velocityFactor = 0;
	} else {
		velocityFactor = 1 - distance / maxDistanceToHaveVelocity;
	}

	int velocity = floor(64 * velocityFactor);

	midiOut->sendNoteOn(1, pitch, velocity);
}

float Ball::getX() {
	return x;
}

float Ball::getY() {
	return y;
}

ofPoint Ball::getCenter() {
	return ofPoint(x, y, waveRadius);
}

vector<Ball>* Ball::getBalls() {
	return balls;
}

void Ball::setPosition(float aX, float aY) {
	x = aX;
	y = aY;
}

void Ball::setColor(ofColor color) {
	this->color = color;
}

ofColor Ball::getColor() {
	return color;
}

void Ball::hit(float distance) {	
	playSound(distance);
	waveRadiuses.push_back(0);
}

void Ball::release() {
	midiOut->sendNoteOff(1, 0);
}

void Ball::setTimeout(int timeout) {
	this->timeout = timeout;
}

int Ball::getTimeout() {
	return timeout;
}

void Ball::setVisibility(bool visibility) {
	this->visible = visibility;
}

bool Ball::isVisible() {
	return visible;
}