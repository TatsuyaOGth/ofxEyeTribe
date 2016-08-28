#pragma once

#include "ofMain.h"
#include "ofxEyeTribe.h"

class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    
    ofxEyeTribe tet;

    
    
    
    
    
    //------------------------------------------------------------------------------------------
    //                               Calibration Example
    //------------------------------------------------------------------------------------------
    /*
        Calibration example.
        ofxEyeTribe has functions fo calibration: calibrationStart(int), calibrationPointStart(), calibrationPointEnd() and calibrationAbort()
        First of all, call calibrationStart(int) and begin calibration mode. You can set the number of points to use for calibration in augument.
        You should draw a point for guide to user's gaze, and guide user's gaze to first/next calibration point.
        Call calibrationPointStart() and begin calibration of gaze point. Wait for few second...
        Call calibrationPointEnd(), and guide user's gaze to next calibration point.
        Repeat N time: N is specified number of points to use for calibration.
        Check consol log, if the message "calibrate succeed" was outputted, your calibration sequence was succeed.
        Try make your functions and animation for calibration sequence.
     */
    
    // values for calibration
    vector<ofPoint> mCalibPoints;
    int             mCurrentCalibIndex;
    bool            mfCalibrating;
    float           mTick;
    float           mDuration;
    enum            { CALIB_STAND_BY, CALIB_START, CALIB_FOLLOW_POINT, CALIB_POINT };
    int             mCalibState;
    float           mCalibPointTime;
    float           mCalibFollowPointTime;
    float           mCalibPointSize;
    
    /**
     *  Begin easily calibration sequence, you have to call updateCalibration() and drawCalibration() after called this.
     *  if you want cancel calibration, call stopCalibrationProcess.
     *
     *  @param numCalibrationPoints The number of points to use for calibration. (only 9/12/16)
     *  @param time of each gaze point calibration.
     *  @param followPointTime Time (sec.) to following for each calibration point (minimum = 1.0)
     *  @param calibPointSize Size (pixel) of each calibration point.
     *  @param random shuffle order of follow points.
     *  @return indication of the request processed okay.
     *  @sa stopCalibrationProcess.
     */
    bool startCalibrationSequence(const int numCalibrationPoints,
                                  const float calibPointTime,
                                  const float followPointTime,
                                  const float calibPointSize,
                                  bool random);
    
    /**
     *  Stop calibration sequence.
     *  @sa startCalibrationProcess.
     */
    void stopCalibrationSequence();
    
    void updateCalibration();
    void drawCalibration();
};
