#pragma once

#include "ofMain.h"
#include "ofxEyeTribe.h"

class testApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    
    ofxEyeTribe tet;
    
    
    
    
    // ---------- Calibration Example ----------
    
    vector<ofPoint> mCalibPoints;
    int             mCurrentCalibIndex;
    float           mTick;
    float           mDuration;
    enum            { CALIB_STAND_BY, CALIB_START, CALIB_FOLLOW_POINT, CALIB_POINT };
    int             mCalibState;
    float           mCalibFollowPointTime;
    float           mCalibPointSize;
    
    /**
     *  Begin calibration sequence, if you want stop calibration, call stopCalibrationProcess.
     *
     *  @param numCalibrationPoints     The number of points to use for calibration. (9 or 12 or 16)
     *  @param followPointTime          Time (sec.) to following for each calibration point (>= 1.0)
     *  @param calibPointSize           Size (pixel) of each calibration point.
     *  @return                         indication of the request processed okay.
     */
    void startCalibrationSequence(const int numCalibrationPoints, const float followPointTime, const float calibPointSize);
    
    /**
     *  Stop calibration process.
     */
    void stopCalibrationProcess();
};
