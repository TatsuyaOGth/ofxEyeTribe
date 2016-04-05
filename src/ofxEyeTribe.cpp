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
, mCalibrationProgress(0)
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
    if (api.connect(false, port)) // always pull-mode
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

double ofxEyeTribe::getCalibrationProgress()
{
    return mCalibrationProgress;
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

bool ofxEyeTribe::calibrationStart(const int numCalibrationPoints)
{
    if (numCalibrationPoints == 9 || numCalibrationPoints == 12 || numCalibrationPoints == 16)
    {
        mfCalibrating = true;
        
        api.calibration_clear();
        
        const gtl::ServerState& serverState = api.get_server_state();
        if (serverState.iscalibrating)
        {
            api.calibration_abort();
        }
        
        if (api.is_connected())
        {
            return api.calibration_start(numCalibrationPoints);
        }
        else {
            ofLogError("ofxEyeTribe", "can not start calibration becouse device has not been connected");
            return false;
        }
    }
    else {
        ofLogError("ofxEyeTribe", "set which number of calibration points is 9, 12 or 16");
        return false;
    }
}

void ofxEyeTribe::calibrationAbort()
{
    api.calibration_abort();
    mfCalibrating = false;
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

void ofxEyeTribe::on_calibration_started()
{
}

void ofxEyeTribe::on_calibration_processing()
{
}

void ofxEyeTribe::on_calibration_progress(double progress)
{
    mCalibrationProgress = progress;
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
