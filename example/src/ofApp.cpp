#include "ofApp.h"
#include <random>

// save compatibility
#if OF_VERSION_MINOR <= 8
    #define ofDrawLine ofLine
    #define ofDrawCircle ofCircle
#endif



void ofApp::setup()
{
    ofSetFrameRate(60);
    
    // open port.
    // NOTE: you have to start The EYE TRIBE TRACKER Server befor call open.
    tet.open();
    //tet.open(6555); //<---- if you want change device port
    
    /*
     API need your screen information, especially for calibration.
     please put in values of your screen parameters: screen index, resolution width/height in pixels, and phisical width/height in meters.
     This parameters can setting even from "EyeTribe UI". If you have already set the parameters via "EyeTrive UI", you do not need call this.
     */
    //tet.setScreen(0, ofGetScreenWidth(), ofGetScreenHeight(), 35.89, 24.71);
}

void ofApp::update()
{
    if (mfCalibrating)
    {
        updateCalibration();
    }
}

void ofApp::draw()
{
    
    
    // draw background
    //-----------------------------------------------------------------------------
    ofBackgroundGradient(ofColor(90), ofColor(0));
    ofDisableAntiAliasing();
    ofSetColor(0, 0, 0);


    
    if (mfCalibrating)
    {
        
        // draw calibration view (calibration process/calibration result)
        //-----------------------------------------------------------------------------
        drawCalibration();
        
    }
    else {
        
        // draw gaze data
        //-----------------------------------------------------------------------------
        ofFill();
        
        // red circle is raw gaze point
        ofSetColor(ofColor::red);
        ofDrawCircle(tet.getPoint2dRaw(), 5);
        
        // green dot is smoothed gaze point
        ofSetColor(ofColor::green);
        ofDrawCircle(tet.getPoint2dAvg(), 10);
        
        // when fixated is show orenge circle
        if (tet.isFix())
        {
            ofSetColor(ofColor::orange, 100);
            ofDrawCircle(tet.getPoint2dAvg(), 40);
        }
        
        // and draw data from each eyes
        ofNoFill();
        ofSetColor(ofColor::aqua);
        ofDrawCircle(tet.getLeftEyeRaw(), 5);
        ofDrawCircle(tet.getRightEyeRaw(), 5);
        
        ofSetColor(ofColor::purple);
        ofDrawCircle(tet.getLeftEyeAvg(), 5);
        ofDrawCircle(tet.getRightEyeAvg(), 5);
        
        ofSetColor(ofColor::yellow);
        ofDrawCircle(tet.getLeftEyePcenter().x * ofGetWidth(), tet.getLeftEyePcenter().y * ofGetHeight(), 20);
        ofDrawCircle(tet.getRightEyePcenter().x * ofGetWidth(), tet.getRightEyePcenter().y * ofGetHeight(), 20);
        
        
        
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
        ss << "is calibrated: " << s.iscalibrated << endl;
        ss << "is calibrating: " << s.iscalibrating << endl;
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

void ofApp::keyPressed(int key)
{
    if (key == 'f') ofToggleFullscreen();
    if (key == 'o') tet.open();
    if (key == 'c') tet.close();
    if (key == 's') tet.startServer();
    
    if (key == ' ')
    {
        if (mfCalibrating)
        {
            stopCalibrationSequence();
        }
        else {
            bool calibration = startCalibrationSequence(9, 1.0, 2.0, 25.0, true);
            
            if (calibration)
                ofLogNotice("ofxEyeTribe", "start calibration sequence");
            else
                ofLogError("ofxEyeTribe", "calibration could not start");
        }
    }
}




//------------------------------------------------------------------------------------------
//                               Calibration Example
//------------------------------------------------------------------------------------------

static const float CALIB_STAND_BY_TIME  = 3.0;


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

bool ofApp::startCalibrationSequence(const int numCalibrationPoints,
                                     const float calibPointTime,
                                     const float followPointTime,
                                     const float calibPointSize,
                                     bool random)
{
    // start calibration
    bool calibrating = tet.calibrationStart(numCalibrationPoints);
    
    if (calibrating)
    {
        // initialize
        mCalibPoints.clear();
        mCurrentCalibIndex = 0;
        
        // get screen size from api, if you dont set yet? see setup()
        const int width = tet.getScreen().screenresw;
        const int height = tet.getScreen().screenresh;
        
        // set calibration points
        int nx = 0;
        int ny = 0;
        switch (numCalibrationPoints)
        {
            case 9  : nx = 3; ny = 3; break;
            case 12 : nx = 4; ny = 3; break;
            case 16 : nx = 4; ny = 4; break;
            default : ofLogError("ofxEyeTribe", "illegal number of points"); return false;
        }
        for (int i = 0; i < ny; ++i)
        {
            for (int j = 0; j < nx; ++j)
            {
                float x = width / (nx * 2) * (j * 2 + 1);
                float y = height / (ny * 2) * (i * 2 + 1);
                mCalibPoints.push_back(ofPoint(x, y));
            }
        }
        
        // shuffle order of calibration points
        if (random)
        {
            ofRandomize(mCalibPoints);
        }
        
        // set animation parameters
        mCalibPointSize = abs(calibPointSize);
        mCalibPointTime = abs(calibPointTime);
        mCalibFollowPointTime = followPointTime;
        if (mCalibFollowPointTime < mCalibPointTime * 2)
        {
            ofLogWarning("ofxEyeTribe", "follow point time must more than " + ofToString(mCalibPointTime * 2));
            mCalibFollowPointTime = mCalibPointTime * 2;
        }
        mCalibState = CALIB_START;
        mTick = 0;
        mDuration = CALIB_STAND_BY_TIME;
        mfCalibrating = true;
        return true;
    }
    else {
        mfCalibrating = false;
        return false;
    }
}

void ofApp::stopCalibrationSequence()
{
    // calibration abort
    tet.calibrationAbort();
    mfCalibrating = false;
    mCalibState = CALIB_STAND_BY;
}

void ofApp::updateCalibration()
{
    if (mfCalibrating)
    {
        // update animation
        
        if (mCalibState == CALIB_START)
        {
            if (mTick > mDuration)
            {
                mTick = 0;
                mDuration = mCalibFollowPointTime - mCalibPointTime;
                mCalibState = CALIB_FOLLOW_POINT;
                // -> goto next state
            }
        }
        
        if (mCalibState == CALIB_FOLLOW_POINT)
        {
            if (mTick > mDuration)
            {
                mTick = 0;
                mDuration = mCalibPointTime;
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
                    mfCalibrating = false;
                    // -> goto end
                }
                else {
                    mCalibState = CALIB_FOLLOW_POINT;
                    mDuration = mCalibFollowPointTime - mCalibPointTime;
                    // -> back to previous state
                }
                mTick = 0;
            }
        }
        
        // count up animation timeline
        mTick += ofGetLastFrameTime();
    }
}

void ofApp::drawCalibration()
{
    ofPushStyle();
    ofEnableAlphaBlending();
    ofEnableAntiAliasing();
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetCircleResolution(90);
    
    // get screen size from api
    const int width  = tet.getScreen().screenresw;
    const int height = tet.getScreen().screenresh;
    const float timeInterp = ofClamp(1.0 - (mTick / mDuration), 0.0, 1.0);
    
    // draw animation
    
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
        ofDrawCircle(p, size);
    }
    
    if (mCalibState == CALIB_POINT)
    {
        ofFill();
        ofSetColor(255, 255, 255);
        const ofPoint& p = mCalibPoints[mCurrentCalibIndex];
        float size = QuintEaseIn(timeInterp) * mCalibPointSize;
        ofDrawCircle(p, size);
    }
    
    if (mCalibState == CALIB_STAND_BY)
    {
        ofFill();
        for (const auto& e : tet.getCalibResult().calibpoints)
        {
            ofFill();
            ofSetColor(ofColor::blue);
            ofDrawCircle(e.cp.x, e.cp.y, 5);
            ofNoFill();
            ofSetColor(ofColor::skyBlue);
            ofDrawCircle(e.mecp.x, e.mecp.y, 5);
        }
    }
    
    ofPopStyle();
}
