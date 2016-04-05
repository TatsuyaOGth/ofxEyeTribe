#pragma once

#include "ofMain.h"
#include "ofxEyeTribe.h"

class testApp : public ofBaseApp
{
public:
    void setup();
    void draw();
    
    void keyPressed(int key);
    
    ofxEyeTribe tet;
};
