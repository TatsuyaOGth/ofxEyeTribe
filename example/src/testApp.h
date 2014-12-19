#pragma once

#include "ofMain.h"
#include "ofxEyeTribe.h"

class testApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    void keyPressed(int key);
    
    ofxEyeTribe tet;
    deque<ofVec2f> pts;
};
