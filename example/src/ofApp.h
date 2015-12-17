#pragma once

#include "ofMain.h"
#include "ofxEyeTribe.h"

class ofApp : public ofBaseApp
{
public:
    void setup();
    void draw();
    
    void keyPressed(int key);
    
    ofxEyeTribe tet;
};
