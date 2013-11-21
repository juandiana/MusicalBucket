MusicalBucket
=============

Interesting experiment that lets you play a simple instrument.
Made using OpenFrameworks with the following addons: ofxCv, ofxMidi & ofxXmlSettings.

It starts with a calibration phase in which you will be asked to select balls of multiple colors and it will start track them in real time using a camera and OpenCV. Once you have your setup ready, you can start playing by touching and moving the balls. Every time a ball is touched an expansive wave is released, making the ball touched sound and also the other balls as well once the wave gets to them.

This software acts as a controller only, sending MIDI notes to a MIDI channel. On the other end, you will have a generator such as Ableton Live producing different kind of sounds, thus letting you have very different sets of arrangements.

Art Concept
===========
![alt art concept](artConcept/Image1.png)
