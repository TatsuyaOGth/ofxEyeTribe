#include "testApp.h"

void testApp::setup()
{
    ofSetFrameRate(60);
    ofSetCircleResolution(45);
    
    tet.open();
    // tet.open(6555); //<---- change port
}

void testApp::draw()
{
    
    
    // draw background
    //-----------------------------------------------------------------------------
    ofBackgroundGradient(ofColor(150), ofColor(30));
    ofDisableAntiAliasing();
    ofSetColor(0, 0, 0);
    for (int i = 0; i < ofGetHeight(); i += 20)
    {
        ofLine(0, i, ofGetWidth(), i);
        for (int j = 0; j < ofGetWidth(); j += 20)
        {
            ofLine(j, 0, j, ofGetHeight());
        }
    }
    
    
    
    // draw gaze data
    //-----------------------------------------------------------------------------
    if (tet.isConnected())
    {
        ofFill();
        
        // red circle is raw gaze point
        ofSetColor(ofColor::red);
        ofCircle(tet.getPoint2dRaw(), 5);
        
        // green dot is smoothed gaze point
        ofSetColor(ofColor::green);
        ofCircle(tet.getPoint2dAvg(), 10);
        
        // when fixated is show orenge circle
        if (tet.isFix())
        {
            ofSetColor(ofColor::orange, 100);
            ofCircle(tet.getPoint2dAvg(), 40);
        }
        
        // and draw data from each eyes
        ofNoFill();
        ofSetColor(ofColor::aqua);
        ofCircle(tet.getLeftEyeRaw(), 5);
        ofCircle(tet.getRightEyeRaw(), 5);
        
        ofSetColor(ofColor::purple);
        ofCircle(tet.getLeftEyeAvg(), 5);
        ofCircle(tet.getRightEyeAvg(), 5);
        
        ofSetColor(ofColor::yellow);
        ofCircle(tet.getLeftEyePcenter().x * ofGetWidth(), tet.getLeftEyePcenter().y * ofGetHeight(), 20);
        ofCircle(tet.getRightEyePcenter().x * ofGetWidth(), tet.getRightEyePcenter().y * ofGetHeight(), 20);
    }

    
    
    // get gaze data and server state
    //-----------------------------------------------------------------------------
    gtl::ServerState s = tet.getServerState();
    stringstream ss;
    ss << "fps: " << ofGetFrameRate() << endl;
    ss << endl;
    ss << "[ API ]" << endl;
    ss << "timestamp: " << tet.getTimestamp() << endl;
    ss << endl;
    ss << "[ GAZE DATA ]" << endl;
    ss << "is fixed: " << (tet.isFix() ? "TRUE" : "FALSE") << endl;
    ss << "point 2d raw: " << tet.getPoint2dRaw() << endl;
    ss << "point 2d avg: " << tet.getPoint2dAvg() << endl;
    ss << "left eye raw: " << tet.getLeftEyeRaw() << endl;
    ss << "left eye avg: " << tet.getLeftEyeAvg() << endl;
    ss << "left eye pupil size: " << tet.getLeftEyePupilSize() << endl;
    ss << "left eye pupil coordinates normalized: " << tet.getLeftEyePcenter() << endl;
    ss << "right eye raw: " << tet.getRightEyeRaw() << endl;
    ss << "right eye avg: " << tet.getRightEyeAvg() << endl;
    ss << "right eye pupil size: " << tet.getRightEyePupilSize() << endl;
    ss << "right eye pupil coordinates normalized: " << tet.getRightEyePcenter() << endl;
    ss << endl;
    ss << "[ SERVER STATE ]" << endl;
    ss << "framerate: " << s.framerate << endl;
    ss << "heartbeat interval: " << s.heartbeatinterval << endl;
    ss << "is calibrated: " << s.iscalibrated << endl;
    ss << "is calibrating: " << s.iscalibrating << endl;
    ss << "push: " << s.push << endl;
    ss << "tracker state: " << s.trackerstate << endl;
    ss << "version: " << s.version << endl;
    ss << endl;
    ss << "[ KEY ]" << endl;
    ss << "f: " << "toggle fulscreen" << endl;
    ss << "o: " << "open(connect)" << endl;
    ss << "c: " << "close(disconnect)" << endl;
    ss << "s: " << "starting server";
    
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(ss.str(), 20, 20);


}

void testApp::keyPressed(int key)
{
    if (key == 'f') ofToggleFullscreen();
    if (key == 'o') tet.open();
    if (key == 'c') tet.close();
    if (key == 's') tet.startServer();
}

