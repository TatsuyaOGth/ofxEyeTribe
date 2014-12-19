#include "testApp.h"

void testApp::setup(){
    tet.open();
}

void testApp::update(){
    
    if (tet.isConnected())
    {
        pts.push_back(tet.getPoint2d());
        if (pts.size() > 20) pts.pop_front();
    }
    
}

void testApp::draw(){
    ofBackground(0, 0, 0);
    
    for (int i = 0; i < pts.size();  ++i)
    {
        ofSetColor(0, 255, 0, ofMap(i, 0, pts.size(), 255, 0));
        ofCircle(pts[i], 5);
    }
}

void testApp::exit()
{
    tet.close();
}

void testApp::keyPressed(int key){
    if (key == 'f') ofToggleFullscreen();
    if (key == 'o') tet.open();
    if (key == 'c') tet.close();
}

