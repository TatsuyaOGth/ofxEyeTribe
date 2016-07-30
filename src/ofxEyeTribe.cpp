/**
 The MIT License (MIT)
 
 Copyright (c) 2015 TatsuyaOGth (https://github.com/TatsuyaOGth)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 NOTE: The Eye Tribe SDK is licensed under the BSD-style license found in the LICENSE file.
 */
#include "ofxEyeTribe.h"

ofxEyeTribe::ofxEyeTribe(bool autoUpdate)
: mfAutoUpdate(autoUpdate)
, mfCalibrating(false)
, mCalibState(CALIB_STAND_BY)
{
    if (mfAutoUpdate)
    {
        ofAddListener(ofEvents().update, this, &ofxEyeTribe::onUpdate, OF_EVENT_ORDER_BEFORE_APP);
    }
    api.add_listener(*this);
}

ofxEyeTribe::~ofxEyeTribe()
{
    if (mfAutoUpdate)
    {
        ofRemoveListener(ofEvents().update, this, &ofxEyeTribe::onUpdate);
    }
    api.remove_listener(*this);
    this->close();
}

ofPoint ofxEyeTribe::point2dToOfVec2d(const gtl::Point2D point2d)
{
    return ofPoint(point2d.x, point2d.y);
}

void ofxEyeTribe::normalize(gtl::Point2D & point2d)
{
    point2d.x = point2d.x / mScreen.screenresw;
    point2d.y = point2d.y / mScreen.screenresh;
}

void ofxEyeTribe::onUpdate(ofEventArgs &e)
{
    update();
}

string ofxEyeTribe::startServer()
{
    string res;
    // TODO: support multi platform
    switch (ofGetTargetPlatform())
    {
        case OF_TARGET_OSX:
            res = ofSystem("open -n /Applications/EyeTribe/EyeTribe");
            break;
            
        default:
            ofLogError("ofxEyeTribe", "sorry, this addon is not supported your platform...");
            break;
    }
    ofLogNotice("ofxEyeTribe", res);
    return res;
}

bool ofxEyeTribe::open(unsigned short port)
{
    if (api.is_connected())
    {
        ofLogNotice("ofxEyeTribe", "is already connected");
        return true;
    }
    if (api.connect(port))
    {
        ofLogNotice("ofxEyeTribe", "connecte - port(" + ofToString(port) + ")");
        return true;
    }
    ofLogError("ofxEyeTribe", "faild connect");
    return false;
}

void ofxEyeTribe::close()
{
    if (api.is_connected())
    {
        api.disconnect();
        ofLogNotice("ofxEyeTribe", "disconnect");
    }
}

void ofxEyeTribe::update()
{
    if (api.is_connected())
    {
        if (mCalibState != CALIB_STAND_BY)
        {
            updateCalibrationProcess();
        }
        api.get_frame(mGazeData);
        api.get_screen(mScreen);
        api.get_calib_result(mCalibResult);
    }
}

void ofxEyeTribe::addUpdateListener()
{
    if (mfAutoUpdate == false)
    {
        ofAddListener(ofEvents().update, this, &ofxEyeTribe::onUpdate, OF_EVENT_ORDER_BEFORE_APP);
        mfAutoUpdate = true;
    }
    else ofLogWarning("ofxEyeTribe", "already registerd update listener");
}

void ofxEyeTribe::removeUpdateListener()
{
    if (mfAutoUpdate)
    {
        ofRemoveListener(ofEvents().update, this, &ofxEyeTribe::onUpdate);
        mfAutoUpdate = false;
    }
    else ofLogWarning("ofxEyeTribe", "update listener was unregisterd");
}

//------------------------------------------------------------------------------------------
//                                  setter
//------------------------------------------------------------------------------------------

void ofxEyeTribe::setScreen(const int screenIndex,
                            const int widthInPixels,
                            const int heightInPixels,
                            const float widthInMeters,
                            const float heightInMeters)
{
    gtl::Screen newScreen;
    newScreen.set(screenIndex, widthInPixels, heightInPixels, widthInMeters, heightInMeters);
    api.set_screen(newScreen);
}

//------------------------------------------------------------------------------------------
//                                  getter
//------------------------------------------------------------------------------------------

ofPoint ofxEyeTribe::getPoint2dRaw()
{
    return point2dToOfVec2d(mGazeData.raw);
}

ofPoint ofxEyeTribe::getPoint2dAvg()
{
    return point2dToOfVec2d(mGazeData.avg);
}

ofPoint ofxEyeTribe::getLeftEyeRaw()
{
    return point2dToOfVec2d(mGazeData.lefteye.raw);
}

ofPoint ofxEyeTribe::getLeftEyeAvg()
{
    return point2dToOfVec2d(mGazeData.lefteye.avg);
}

float ofxEyeTribe::getLeftEyePupilSize()
{
    return mGazeData.lefteye.psize;
}

ofPoint ofxEyeTribe::getLeftEyePcenter()
{
    return point2dToOfVec2d(mGazeData.lefteye.pcenter);
}

ofPoint ofxEyeTribe::getRightEyeRaw()
{
    return point2dToOfVec2d(mGazeData.righteye.raw);
}

ofPoint ofxEyeTribe::getRightEyeAvg()
{
    return point2dToOfVec2d(mGazeData.righteye.avg);
}

float ofxEyeTribe::getRightEyePupilSize()
{
    return mGazeData.righteye.psize;
}

ofPoint ofxEyeTribe::getRightEyePcenter()
{
    return point2dToOfVec2d(mGazeData.righteye.pcenter);
}

int ofxEyeTribe::getTimestamp()
{
    return mGazeData.time;
}

bool ofxEyeTribe::isFix()
{
    return mGazeData.fix;
}

bool ofxEyeTribe::isConnected()
{
    return api.is_connected();
}

bool ofxEyeTribe::isFrameNew()
{
    return true;
}

bool ofxEyeTribe::isCalibrating()
{
    return mfCalibrating;
}

bool ofxEyeTribe::isCalibrationSucceed()
{
    return mCalibResult.result;
}

gtl::ServerState const & ofxEyeTribe::getServerState()
{
    return api.get_server_state();
}

gtl::Screen const & ofxEyeTribe::getScreen()
{
    return mScreen;
}

gtl::GazeData const & ofxEyeTribe::getGazeData()
{
    return mGazeData;
}

gtl::CalibResult const & ofxEyeTribe::getCalibResult()
{
    return mCalibResult;
}


//------------------------------------------------------------------------------------------
//                                  calibration
//------------------------------------------------------------------------------------------

static const float CALIB_STAND_BY_TIME  = 3.0;
static const float CALIB_POINT_TIME     = 0.5;


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


bool ofxEyeTribe::calibrationStart(const int numCalibrationPoints)
{
    if (numCalibrationPoints == 9 || numCalibrationPoints == 12 || numCalibrationPoints == 16)
    {
        api.calibration_clear();
        
        const gtl::ServerState& serverState = api.get_server_state();
        if (serverState.iscalibrating)
        {
            api.calibration_abort();
        }
        
        bool res = api.calibration_start(numCalibrationPoints);
        mfCalibrating = res;
        return res;
    }
    
    ofLogError("ofxEyeTribe", "set which number of calibration points is 9, 12 or 16");
    return false;
}

void ofxEyeTribe::calibrationAbort()
{
    api.calibration_abort();
    mfCalibrating = false;
    mCalibState = CALIB_STAND_BY;
}

void ofxEyeTribe::calibrationPointStart(const int x, const int y)
{
    if (mfCalibrating)
    {
        api.calibration_point_start(x, y);
    }
    else ofLogError("ofxEyeTribe", "calibration session has not began");
}

void ofxEyeTribe::calibrationPointStart(const ofPoint &p)
{
    calibrationPointStart(p.x, p.y);
}

void ofxEyeTribe::calibrationPointEnd()
{
    if (mfCalibrating)
    {
        api.calibration_point_end();
    }
    else ofLogError("ofxEyeTribe", "calibration session has not began");
}

bool ofxEyeTribe::startCalibrationProcess(const int numCalibrationPoints, const float followPointTime, const float calibPointSize)
{
    bool bCalibStartSuccess = calibrationStart(numCalibrationPoints);
    
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
        const int width = mScreen.screenresw;
        const int height = mScreen.screenresh;
        
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
        mDuration = CALIB_STAND_BY_TIME;
        return true;
    }
    else {
        return false;
    }
}

void ofxEyeTribe::stopCalibrationProcess()
{
    calibrationAbort();
}

void ofxEyeTribe::updateCalibrationProcess()
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
            calibrationPointStart(mCalibPoints[mCurrentCalibIndex]);
            // -> goto next state
        }
    }
    
    if (mCalibState == CALIB_POINT)
    {
        if (mTick > mDuration)
        {
            // end current calib pts
            calibrationPointEnd();
            
            mCurrentCalibIndex++;
            
            if (mCurrentCalibIndex == mCalibPoints.size())
            {
                mCalibState = CALIB_STAND_BY;
                mfCalibrating = false;
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

void ofxEyeTribe::drawCalibration()
{
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetCircleResolution(90);
    
    const int width  = mScreen.screenresw;
    const int height = mScreen.screenresh;
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
        for (const auto& e : mCalibResult.calibpoints)
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
}

void ofxEyeTribe::on_calibration_started()
{
}

void ofxEyeTribe::on_calibration_processing()
{
}

void ofxEyeTribe::on_calibration_progress(double progress)
{
}

void ofxEyeTribe::on_calibration_result(bool is_calibrated, const gtl::CalibResult &calib_result)
{
    if (is_calibrated)
    {
        ofLogNotice("ofxEyeTribe", "calibrate succeed");
    }
    else {
        ofLogWarning("ofxEyeTribe", "calibrate failed");
    }
    mCalibResult = calib_result;
    mfCalibrating = false;
}
