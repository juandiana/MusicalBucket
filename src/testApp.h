#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxMidi.h"
#include "ofxXmlSettings.h"
#include "ball.h"



class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void mousePressed(int x, int y, int button);
	void keyPressed(int key);
	
	ofVideoGrabber cam;
	int touchDetectionInterval;
	int totalBalls;
	vector<ofxCv::ContourFinder> contourFinder;
	vector<int> colorsTimeout;
	ofxCv::TrackingColorMode trackingColorMode;
	int threshold;
	ofColor color;
	int colorCount;
	bool calibrationMode;
	string windowTitle;
	int currentSetting;
	float waveSpeed;
	vector<int> pitches;
	

	vector<Ball> balls;
	ofSoundPlayer sound;
	ofxMidiOut midiOut;
	int timeSinceLastDraw;
	int lastDrawTime;
};
