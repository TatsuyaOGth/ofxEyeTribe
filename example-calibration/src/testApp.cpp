#include "testApp.h"

static float QuintEaseIn(const float t)
{
    return t * t * t * t * t;
}

static float QuintEaseOut(const float t)
{
    return 1. - QuintEaseIn(1. - t);
}

static float ElasticEaseOut(const float t)
{
    float s = 1 - t;
    return 1 - powf(s, 8) + sinf(t * t * 6 * PI) * s * s;
}

static float ElasticEaseIn(const float t)
{
    return 1.0 - ElasticEaseOut(1.0 - t);
}

static float min(float v1, float v2)
{
    if (v1 > v2)
        return v2;
    else
        return v1;
}

static const float CALIB_POINT_TIME = 0.5;



void testApp::setup()
{
    ofSetFrameRate(60);
    ofSetCircleResolution(45);
    
    tet.open();
    // tet.open(6555); //<---- if you want change device port
    
    mCalibState = CALIB_STAND_BY;
}

void testApp::update()
{
    if (mCalibState != CALIB_STAND_BY)
    {
        if (mCalibState == CALIB_START)
        {
            if (mTick > mDuration)
            {
                mTick = 0;
                mDuration = mCalibFollowPointTime - CALIB_POINT_TIME;
                mCalibState = CALIB_FOLLOW_POINT;
                // -> goto next state
            }
        }
        
        if (mCalibState == CALIB_FOLLOW_POINT)
        {
            if (mTick > mDuration)
            {
                mTick = 0;
                mDuration = 0.5;
                mCalibState = CALIB_POINT;
                // begin new calib pts
                tet.calibrationPointStart(mCalibPoints[mCurrentCalibIndex]);
                // -> goto next state
            }
        }
        
        if (mCalibState == CALIB_POINT)
        {
            if (mTick > mDuration)
            {
                // end current calib pts
                tet.calibrationPointEnd();
                
                mCurrentCalibIndex++;
                
                if (mCurrentCalibIndex == mCalibPoints.size())
                {
                    mCalibState = CALIB_STAND_BY;
                    // -> goto end
                }
                else {
                    mCalibState = CALIB_FOLLOW_POINT;
                    mDuration = 1.0;
                    // -> back to previous state
                }
                mTick = 0;
            }
        }
        
        mTick += ofGetLastFrameTime();
    }
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
    

        
    // draw calibration view (calibration process/calibration result)
    //-----------------------------------------------------------------------------
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetCircleResolution(90);
    
    const int width  = tet.getScreen().screenresw;
    const int height = tet.getScreen().screenresh;
    const float timeInterp = ofClamp(1.0 - (mTick / mDuration), 0.0, 1.0);
    
    if (mCalibState == CALIB_START)
    {
        unsigned char a = QuintEaseOut(timeInterp) * 255;
        ofDrawBitmapStringHighlight("FOLLOW THE CIRCLE", width / 2 - 70, height / 2, ofColor(0, a), ofColor(255, a));
    }
    
    if (mCalibState == CALIB_FOLLOW_POINT)
    {
        ofFill();
        ofSetColor(255, 255, 255);
        const ofPoint& p = mCalibPoints[mCurrentCalibIndex];
        float size = ElasticEaseIn(timeInterp) * (mCalibPointSize * 1.5) + mCalibPointSize;
        ofCircle(p, size);
    }
    
    if (mCalibState == CALIB_POINT)
    {
        ofFill();
        ofSetColor(255, 255, 255);
        const ofPoint& p = mCalibPoints[mCurrentCalibIndex];
        float size = QuintEaseIn(timeInterp) * mCalibPointSize;
        ofCircle(p, size);
    }
    
    if (mCalibState == CALIB_STAND_BY)
    {
        ofFill();
        for (const auto& e : tet.getCalibResult().calibpoints)
        {
            ofFill();
            ofSetColor(ofColor::blue);
            ofCircle(e.cp.x, e.cp.y, 5);
            ofNoFill();
            ofSetColor(ofColor::skyBlue);
            ofCircle(e.mecp.x, e.mecp.y, 5);
        }
    }
    ofPopStyle();

    
    
    if (tet.isCalibrating() == false)
    {
        // draw gaze data
        //-----------------------------------------------------------------------------
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
        
        
        
        // get gaze data and server state
        //-----------------------------------------------------------------------------
        const gtl::ServerState& s = tet.getServerState();
        const gtl::Screen& screen = tet.getScreen();
        const gtl::CalibResult& cr = tet.getCalibResult();
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
        ss << "[ SCREEN ]" << endl;
        ss << "screen index: " << screen.screenindex << endl;
        ss << "screen resolution width in pixels  : " << screen.screenresw << endl;
        ss << "screen resolution height in pixels : " << screen.screenresh << endl;
        ss << "screen physical width in meters  : " << screen.screenpsyw << endl;
        ss << "screen physical height in meters : " << screen.screenpsyh << endl;
        ss << endl;
        ss << "[ CALIBRATION ]" << endl;
        ss << "result: " << (cr.result ? "succeed" : "failed") << endl;
        ss << "average error in degrees: " << cr.deg << endl;
        ss << "average error in degs, left eye: " << cr.degl << endl;
        ss << "average error in degs, right eye: " << cr.degr << endl;
        ss << "number of calibration points: " << cr.calibpoints.size() << endl;
        ss << endl;
        ss << "[ KEY ]" << endl;
        ss << "f: " << "toggle fulscreen" << endl;
        ss << "o: " << "open(connect)" << endl;
        ss << "c: " << "close(disconnect)" << endl;
        ss << "s: " << "starting server" << endl;
        ss << "SPACE: " << "start calibration process";
        
        ofSetColor(255, 255, 255);
        ofDrawBitmapString(ss.str(), 20, 20);
    }

}

void testApp::keyPressed(int key)
{
    if (key == 'f') ofToggleFullscreen();
    if (key == 'o') tet.open();
    if (key == 'c') tet.close();
    if (key == 's') tet.startServer();
    
    // Calibration start/aboot
    if (key == ' ')
    {
        if (tet.isCalibrating())
        {
            stopCalibrationProcess();
        }
        else {
            startCalibrationSequence(9, 1.5, 25);
        }
    }
}


void testApp::startCalibrationSequence(const int numCalibrationPoints, const float followPointTime, const float calibPointSize)
{
    bool bCalibStartSuccess = tet.calibrationStart(numCalibrationPoints);
    
    if (bCalibStartSuccess)
    {
        mCalibPoints.clear();
        int nx = 0;
        int ny = 0;
        switch (numCalibrationPoints)
        {
            case 9  : nx = 3; ny = 3; break;
            case 12 : nx = 4; ny = 3; break;
            case 16 : nx = 4; ny = 4; break;
            default : ofLogError("ofxEyeTribe", "illegal number of points"); return false;
        }
        const int width = tet.getScreen().screenresw;
        const int height = tet.getScreen().screenresh;
        
        for (int i = 0; i < ny; ++i)
        {
            for (int j = 0; j < nx; ++j)
            {
                float x = width / (nx * 2) * (j * 2 + 1);
                float y = height / (ny * 2) * (i * 2 + 1);
                mCalibPoints.push_back(ofPoint(x, y));
            }
        }
        mCurrentCalibIndex = 0;
        mCalibPointSize = calibPointSize;
        mCalibFollowPointTime = followPointTime;
        if (mCalibFollowPointTime < CALIB_POINT_TIME * 2)
        {
            ofLogWarning("ofxEyeTribe", "follow point time must more than " + ofToString(CALIB_POINT_TIME * 2));
            mCalibFollowPointTime = CALIB_POINT_TIME * 2;
        }
        mCalibState = CALIB_START;
        mTick = 0;
        mDuration = 3.0; // calibration stand-by time
        return true;
    }
    else {
        return false;
    }
}

void testApp::stopCalibrationProcess()
{
    tet.calibrationAbort();
}
