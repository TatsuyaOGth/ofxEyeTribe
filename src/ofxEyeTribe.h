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
#pragma once

#include "ofMain.h"
#include <include/gazeapi.h>


class ofxEyeTribe
{
protected:
    gtl::GazeApi    api;
    gtl::GazeData   mGazeData;
    gtl::Screen     mScreen;
    bool            mfAutoUpdate;
    
    ofPoint         point2dToOfVec2d(const gtl::Point2D point2d);
    void            normalize(gtl::Point2D & point2d);
    void            onUpdate(ofEventArgs &e);
    
public:
    ofxEyeTribe(bool autoUpdate = true);
    virtual ~ofxEyeTribe();
    
    string          startServer();
    
    /**
     *  Open port
     *
     *  @param port number of port
     *  @return bool (if faled this process than return false)
     */
    bool            open(unsigned short port = 6555);
    
    /**
     *  Close port
     */
    void            close();
    
    /**
     *  Update api values.
     *  You do not need to call this function because this will register on oF's update event listener at the constructor.
     *  If you want to call this function by yourself.
     */
    void            update();
    
    //------------------------------------------------------------------------------------------
    //                                  getter
    //------------------------------------------------------------------------------------------
    
    /** raw gaze coordinates in pixels @return ofVec2d */
    ofPoint         getPoint2dRaw();
    
    /** smoothed coordinates in pix @return ofVec2d */
    ofPoint         getPoint2dAvg();
    
    /** raw gaze coordinates in pixels @return ofVec2d */
    ofPoint         getLeftEyeRaw();
    
    /** smoothed coordinates in pix @return ofVec2d */
    ofPoint         getLeftEyeAvg();
    
    /** pupil size @return float */
    float           getLeftEyePupilSize();
    
    /** pupil coordinates normalized @return ofVec2d */
    ofPoint         getLeftEyePcenter();
    
    
    
    /** raw gaze coordinates in pixels @return ofVec2d */
    ofPoint         getRightEyeRaw();
    
    /** smoothed coordinates in pix @return ofVec2d */
    ofPoint         getRightEyeAvg();
    
    /** pupil size @return float */
    float           getRightEyePupilSize();
    
    /** pupil coordinates normalized @return ofVec2d */
    ofPoint         getRightEyePcenter();
    
    
    /** timestamp @return int */
    int             getTimestamp();
    
    /** is fixated? @return bool */
    bool            isFix();
    
    /** check connection @return bool */
    bool            isConnected();
    
    /** check frame new @return bool */
    OF_DEPRECATED_MSG("ofxEyeTribe::isFrameNew() was deprecated from ver.0.4, now this is allways return true.", bool isFrameNew());
    
    /** get a api's object includ server state values @return struct gtl::ServerState */
    gtl::ServerState const & getServerState();
    
    /** get a api's object includ screen values @return struct gtl::Screen */
    gtl::Screen const & getScreen();
    
    //------------------------------------------------------------------------------------------
    //                                  TODO: calibration
    //------------------------------------------------------------------------------------------
    
    
};

