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
#include "gazeapi.h"


class ofxEyeTribe : public gtl::ICalibrationProcessHandler
{
    // listener
    void on_calibration_started();
    void on_calibration_progress( double progress );
    void on_calibration_processing();
    void on_calibration_result( bool is_calibrated, gtl::CalibResult const & calib_result );
    
protected:
    gtl::GazeApi    api;
    gtl::GazeData   mGazeData;
    gtl::Screen     mScreen;
    gtl::CalibResult mCalibResult;
    bool            mfAutoUpdate;

    // calibration
    vector<ofPoint> mCalibPoints;
    int             mCurrentCalibIndex;
    bool            mfCalibrating;
    float           mTick;
    float           mDuration;
    enum            { CALIB_STAND_BY, CALIB_START, CALIB_FOLLOW_POINT, CALIB_POINT };
    int             mCalibState;
    float           mCalibFollowPointTime;
    float           mCalibPointSize;
    
protected:
    ofPoint         point2dToOfVec2d(const gtl::Point2D point2d);
    void            normalize(gtl::Point2D & point2d);
    void            onUpdate(ofEventArgs &e);
    void            updateCalibrationProcess();
    
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
     *  If you want to call this yourself, please call ofxEyeTribe::removeUpdateListener().
     */
    void            update();
    
    /**
     *  Registering update listener
     */
    void            addUpdateListener();
    
    /**
     *  Remove update listener
     */
    void            removeUpdateListener();
    
    /**
     *  Draw calibration view
     */
    void            drawCalibration();
    
    
    
    //------------------------------------------------------------------------------------------
    //                                  setter
    //------------------------------------------------------------------------------------------
    
    /**
     *  Define screen values
     *
     *  @param screenIndex    Screen index
     *  @param widthInPixels  Screen resolution width in pixels
     *  @param heightInPixels Screen resolution height in pixels
     *  @param widthInMeters  Screen physical width in meters
     *  @param heightInMeters Screen physical height in meters
     */
    void setScreen(const int screenIndex,
                   const int widthInPixels,
                   const int heightInPixels,
                   const float widthInMeters,
                   const float heightInMeters);
    
    
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
    
    /** is now calibrating? @return bool */
    bool            isCalibrating();
    
    /** was the calibration successful? @return succeed is true */
    bool            isCalibrationSucceed();
    
    
    
    /** get a api's object includ server state values @return struct gtl::ServerState */
    gtl::ServerState const & getServerState();
    
    /** get a api's object includ screen values @return struct gtl::Screen */
    gtl::Screen const & getScreen();
    
    /** get a api's object includ gaze data @return struct gtl::GazeData */
    gtl::GazeData const & getGazeData();
    
    /** get a api's object includ calibration results @return struct gtl::CalibResult */
    gtl::CalibResult const & getCalibResult();
    
    //------------------------------------------------------------------------------------------
    //                                  calibration
    //------------------------------------------------------------------------------------------
    
    /** Begin new calibration sesssion.
     *
     * \param[in] point_count The number of points to use for calibration. (only 9/12/16)
     * \returns indication of the request processed okay.
     */
    bool calibrationStart(const int numCalibrationPoints = 9);
    
    /** Abort the current calibration session.
     *
     * Aborts the current calibration session, but does not clear any valid calibration in the server
     */
    void calibrationAbort();
    
    /** Begin calibration a new calibration point.
     *
     * \param[in] x x-coordinate of calibration point.
     * \param[in] y y-coordinate of calibration point.
     * \sa calibrationPointEnd.
     */
    void calibrationPointStart(const int x, const int y);
    
    /** Begin calibration a new calibration point.
     *
     * \param[in] p ofPoint of calibration point.
     * \sa calibrationPointEnd.
     */
    void calibrationPointStart(const ofPoint& p);
    
    /** End current calibration point.
     * \sa calibrationPointStart(const int x, const int y).
     */
    void calibrationPointEnd();
    
    
    /**
     *  Begin easily calibration process (process automatically), you have to call update() and drawCalibration().
     *  IF you want abort calibration, call stopCalibrationProcess.
     *
     *  @param numCalibrationPoints The number of points to use for calibration. (only 9/12/16)
     *  @param followPointTime Time (sec.) to following for each calibration point (minimum = 1.0)
     *  @param calibPointSize Size (pixel) of each calibration point.
     *  @return indication of the request processed okay.
     *  @sa drawCalibration, stopCalibrationProcess
     */
    bool startCalibrationProcess(const int numCalibrationPoints = 9, const float followPointTime = 1.5, const float calibPointSize = 25.0);
    
    /**
     *  Stop calibration process.
     *  @sa startCalibrationProcess.
     */
    void stopCalibrationProcess();
};

