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
, mfFrameNew(false)
, mApiTime(0)
{
    if (mfAutoUpdate)
    {
        ofAddListener(ofEvents().update, this, &ofxEyeTribe::onUpdate, OF_EVENT_ORDER_BEFORE_APP);
    }
}

ofxEyeTribe::~ofxEyeTribe()
{
    if (mfAutoUpdate)
    {
        ofRemoveListener(ofEvents().update, this, &ofxEyeTribe::onUpdate);
    }
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
        case OF_TARGET_OSX: res = ofSystem("open -n /Applications/EyeTribe/EyeTribe"); break;
        default: ofLogError("ofxEyeTribe", "sorry, this addon is not supported your platform..."); break; //TODO: multi pratform
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
        mfFrameNew = mGazeData.time > mApiTime;
        mApiTime = mGazeData.time;
    }
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
    return mfFrameNew;
}

gtl::ServerState const & ofxEyeTribe::getServerState()
{
    return api.get_server_state();
}

gtl::Screen const & ofxEyeTribe::getScreen()
{
    return mScreen;
}
    