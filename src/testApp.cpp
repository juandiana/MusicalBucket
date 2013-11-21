#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	cam.initGrabber(640, 480);

	// Load settings from XML
	ofxXmlSettings settings;
	if (!settings.loadFile("settings.xml")) {
		ofExit(0);
	}

	string midiPort = settings.getValue("midiPort", "CosasLocas");
	int defaultThreshold = settings.getValue("defaultThreshold", 27);
	float defaultWaveSpeed = settings.getValue("defaultWaveSpeed", 1.5f);
	touchDetectionInterval = settings.getValue("touchDetectionInterval", 10);
	totalBalls = settings.getValue("totalBalls", 3);
	for (int i = 0; i < totalBalls; i++) {
		int pitch = settings.getValue("pitches:p" + ofToString(i + 1), 50);
		pitches.push_back(pitch);
	}

	// Display settings on console
	cout << "Settings loaded successfully!\n";
	cout << "-----------------------------\n";
	cout << "Midi port                = " + midiPort + "\n";
	cout << "Default threshold        = " + ofToString(defaultThreshold) + "\n";
	cout << "Default wave speed       = " + ofToString(defaultWaveSpeed) + "\n";
	cout << "Touch detection interval = " + ofToString(touchDetectionInterval) + "\n";
	cout << "Pitch values             = ";

	for (int i = 0; i < pitches.size(); i++) {
		cout << ofToString(pitches[i]);
		if (i < pitches.size() - 1) {
			cout << ", ";
		} else {
			cout << "\n\n";
		}
	}

	midiOut.openPort(midiPort);

	threshold = defaultThreshold;
	color = ofColor::white;
	trackingColorMode = TRACK_COLOR_RGB;
	colorCount = 0;
	calibrationMode = true;
	windowTitle = "Musical Bucket";
	currentSetting = 0;
	waveSpeed = defaultWaveSpeed;

	ofBackground(ofColor::white);
}

void testApp::update() {
	cam.update();

	windowTitle = "Musical Bucket - " + ofToString(ofGetFrameRate(), 2) + " fps";

	if (calibrationMode) {
		if (colorCount > 0) {
			if (cam.isFrameNew()) {
				for (int i = 0; i < colorCount; i++) {
					contourFinder[i].findContours(cam);
				}
			}
		}
		windowTitle += " Calibration Mode";
	} else {
		if (cam.isFrameNew()) {
			for (int i = 0; i < totalBalls; i++) {
				contourFinder[i].findContours(cam);
			}
		}

		for (int i = 0; i < totalBalls; i++) {
			bool visible = contourFinder[i].getBoundingRects().size() == 1;
			balls[i].setVisibility(visible);

			if (visible) {
				cv::Point2f center = contourFinder[i].getCenter(0);
				balls[i].setPosition(center.x, center.y);

				// onRelease
				if (balls[i].getTimeout() >= touchDetectionInterval) {
					balls[i].release();
				}

				balls[i].setTimeout(0);
			} else if (balls[i].getTimeout() < touchDetectionInterval) {
				int timeout = balls[i].getTimeout();
				timeout++;
				balls[i].setTimeout(timeout);

				// onTouch
				if (timeout == touchDetectionInterval) {
					balls[i].hit(0);
					cout << "Ball " + ofToString(i + 1) + " touched!\n";
				}
			}
		}
	}
}

void testApp::draw() {
	if (calibrationMode) {
		ofSetColor(255);
		cam.draw(0, 0);

		ofColor transparent = ofColor(0, 0, 0, 0);
		
		if (colorCount > 0) {
			drawHighlightString("Settings for color " + ofToString(colorCount), 10, ofGetWindowHeight() - 190, transparent, ofColor::black);
			drawHighlightString(trackingColorMode == TRACK_COLOR_H ? "Tracking mode: H" : "Tracking mode: RGB", 10, ofGetWindowHeight() - 170, transparent, currentSetting == 0 ? ofColor::blue : ofColor::black);
			drawHighlightString("Color threshold: " + ofToString(threshold), 200, ofGetWindowHeight() - 170, transparent, currentSetting == 1 ? ofColor::blue : ofColor::black);
			drawHighlightString("Wave speed: " + ofToString(waveSpeed), 400, ofGetWindowHeight() - 170, transparent, currentSetting == 2 ? ofColor::blue : ofColor::black);

			ofSetColor(ofColor::red);
			contourFinder[colorCount - 1].draw();
		}
		
		// Display selected colors
		if (colorCount == 0) {
			drawHighlightString("Press ENTER to start selecting colors.", 10, ofGetWindowHeight() - 190, transparent, ofColor::black);
		} else {
			drawHighlightString("Selected colors ", 10, ofGetWindowHeight() - 80, transparent, ofColor::black);
		}

		int posX = 30;
		int posY = ofGetWindowHeight() - 30;
	
		for (int i = 0; i < colorCount - 1; i++) {
			ofSetColor(balls[i].getColor());
			ofCircle(posX, posY, 20);
			posX += 42;
		}
	} else {
		timeSinceLastDraw = ofGetElapsedTimeMillis() - lastDrawTime;
		lastDrawTime = ofGetElapsedTimeMillis();

		for (int i = 0; i < totalBalls; i++) {
			if (balls[i].getTimeout() < 10) {
				balls[i].draw(timeSinceLastDraw);
			}
		}
	}

	ofSetWindowTitle(windowTitle);
}

void testApp::mousePressed(int x, int y, int button) {
	if (calibrationMode && x < 640 && y < 480) {
		color = cam.getPixelsRef().getColor(x, y);
		if (colorCount > 0) {
			contourFinder[colorCount - 1].setTargetColor(color, trackingColorMode);
		}
	}
}

void testApp::keyPressed(int key) {
	if (calibrationMode) {
		switch (key) {
			case OF_KEY_UP: {
				if (colorCount > 0) {
					switch (currentSetting) {
						case 0: {
								trackingColorMode = TRACK_COLOR_H;
								contourFinder[colorCount - 1].setTargetColor(color, trackingColorMode);
								break;
						}
						case 1: {
							if (threshold < 128) {
								threshold++;
								contourFinder[colorCount - 1].setThreshold(threshold);
							}
							break;
						}
						case 2: {
							if (waveSpeed < 10) {
								waveSpeed += 0.1;
							}
							break;
						}
					}
				}
				break;
			}
			case OF_KEY_DOWN: {
				if (colorCount > 0) {
					switch (currentSetting) {
						case 0: {
								trackingColorMode = TRACK_COLOR_RGB;
								contourFinder[colorCount - 1].setTargetColor(color, trackingColorMode);
								break;
						}
						case 1: {
							if (threshold > 0) {
								threshold--;
								contourFinder[colorCount - 1].setThreshold(threshold);
							}
							break;
						}
						case 2: {
							if (waveSpeed > 0.1) {
								waveSpeed -= 0.1;
								if (waveSpeed < 0.1) {
									waveSpeed = 0.1;
								}
							}
							break;
						}
					}
				}
				break;
			}
			case OF_KEY_LEFT: {
				if (currentSetting > 0) {
					currentSetting--;
				}
				break;
			}
			case OF_KEY_RIGHT: {
				if (currentSetting < 2) {
					currentSetting++;
				}
				break;
			}
			case OF_KEY_RETURN: {
				if (colorCount > 0) {
					if (color == ofColor::white) {
						break;
					}

					Ball ball = Ball(color, &balls, colorCount, &midiOut, waveSpeed, pitches[colorCount - 1]);
					balls.push_back(ball);
					cout << "Ball " + ofToString(colorCount) + " -> Color: " + ofToString(color) + " Wave speed: " + ofToString(waveSpeed) + "\n";
				}

				colorCount++;

				if (colorCount <= totalBalls) {
					ofxCv::ContourFinder contour = ofxCv::ContourFinder();
					contour.setMinAreaRadius(10);	// Min radius in pixels
					contour.setMaxAreaRadius(70);	// Max radius in pixels
					contour.setTargetColor(color, trackingColorMode);
					contour.setThreshold(threshold);
					contourFinder.push_back(contour);
				}

				if (colorCount == totalBalls + 1) {
					calibrationMode = false;
				}
				break;
			}
		}
	}
}