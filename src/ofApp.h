#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxFaceTracker.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        ofVideoGrabber cam;
        ofxFaceTracker tracker;
        ofxFaceTracker imgTracker;
        ofImage faceImage;
    
        ofEasyCam easyCam;
    
        float y_base;
        float scale_base;
        bool debug;
        bool cPressed;
        bool shoda;
    
        ofSoundPlayer alert;
};
