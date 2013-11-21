#pragma once
#include "ofMain.h"
#include "ofxMidi.h"

#define RADIUS 30.0f

class Ball {

	private:
		float x;
		float y;
		ofSoundPlayer sound;
		float waveRadius;
		float waveSpeed;
		vector<float> waveRadiuses;
		bool isWaveActive;
		vector<Ball>* balls;
		int numBall;
		ofxMidiOut* midiOut;
		ofColor color;
		int timeout;
		bool visible;
		int pitch;

	public:
		Ball(ofColor color, vector<Ball>* balls, int numBall, ofxMidiOut* midiOut, float waveSpeed, int pitch);
		int getNumBall();
		void draw(float time);
		double calculateDistance(int x1, int y1, int x2, int y2);
		void hit(float distance);
		void release();
		void playSound(float distance);
		float getX();
		float getY();
		ofPoint getCenter();
		vector<Ball>* getBalls();
		void setPosition(float aX, float aY);
		void setColor(ofColor color);
		ofColor getColor();
		void setTimeout(int timeout);
		int getTimeout();
		void setVisibility(bool visibility);
		bool isVisible();
};
