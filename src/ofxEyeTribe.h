#pragma once

#include "ofMain.h"
#include <include/gazeapi.h>


class ofxEyeTribe
{
protected:
    gtl::GazeApi api;
    gtl::GazeData mGazeData;
    gtl::Screen mScreen;
    bool mfAutoUpdate;
    
    ofVec2f point2dToOfVec2d(const gtl::Point2D point2d)
    {
        return ofVec2f(point2d.x, point2d.y);
    }
    
    void normalize(gtl::Point2D & point2d)
    {
        point2d.x = point2d.x / mScreen.screenresw;
        point2d.y = point2d.y / mScreen.screenresh;
    }
    
public:
    
    ofxEyeTribe(bool autoUpdate = true): mfAutoUpdate(autoUpdate)
    {
        if (mfAutoUpdate)
        {
            ofAddListener(ofEvents().update, this, &ofxEyeTribe::onUpdate);
        }
    }
    
    virtual ~ofxEyeTribe()
    {
        if (mfAutoUpdate)
        {
            ofRemoveListener(ofEvents().update, this, &ofxEyeTribe::onUpdate);
        }
        this->close();
    }
    
    void onUpdate(ofEventArgs &e)
    {
        update();
    }
    
    string startServer()
    {
        string res;
        switch (ofGetTargetPlatform())
        {
            case OF_TARGET_OSX:
                res = ofSystem("open -n /Applications/EyeTribe/EyeTribe");
                break;
            default:
                ofLogError("ofxEyeTribe", "sorry, this addon is not supported your platform...");
                break;
            //TODO: multi platform
        }
        ofLogNotice("ofxEyeTribe", res);
        return res;
    }
    
    bool open(unsigned short port = 6555)
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
    
    void close()
    {
        if (api.is_connected())
        {
            api.disconnect();
            ofLogNotice("ofxEyeTribe", "disconnect");
        }
    }
    
    void update()
    {
        if (api.is_connected())
        {
            api.get_frame(mGazeData);
            api.get_screen(mScreen);
        }
    }
    
    
    //------------------------------------------------------------------------------------------
    //                                  getter
    //------------------------------------------------------------------------------------------
    
    /** raw gaze coordinates in pixels @return ofVec2d */
    ofVec2f getPoint2dRaw() { return point2dToOfVec2d(mGazeData.raw); }
    
    /** smoothed coordinates in pix @return ofVec2d */
    ofVec2f getPoint2dAvg() { return point2dToOfVec2d(mGazeData.avg); }
    
    
    
    /** raw gaze coordinates in pixels @return ofVec2d */
    ofVec2f getLeftEyeRaw() { return point2dToOfVec2d(mGazeData.lefteye.raw); }
    
    /** smoothed coordinates in pix @return ofVec2d */
    ofVec2f getLeftEyeAvg() { return point2dToOfVec2d(mGazeData.lefteye.avg); }
    
    /** pupil size @return float */
    float getLeftEyePupilSize() { return mGazeData.lefteye.psize; }
    
    /** pupil coordinates normalized @return ofVec2d */
    ofVec2f getLeftEyePcenter() { return point2dToOfVec2d(mGazeData.lefteye.pcenter); }
    
    
    
    /** raw gaze coordinates in pixels @return ofVec2d */
    ofVec2f getRightEyeRaw() { return point2dToOfVec2d(mGazeData.righteye.raw); }
    
    /** smoothed coordinates in pix @return ofVec2d */
    ofVec2f getRightEyeAvg() { return point2dToOfVec2d(mGazeData.righteye.avg); }
    
    /** pupil size @return float */
    float getRightEyePupilSize() { return mGazeData.righteye.psize; }
    
    /** pupil coordinates normalized @return ofVec2d */
    ofVec2f getRightEyePcenter() { return point2dToOfVec2d(mGazeData.righteye.pcenter); }
    
    
    /** timestamp @return int */
    int getTimestamp() { return mGazeData.time; }
    
    /** is fixated? @return bool */
    bool isFix() { return mGazeData.fix; }
    
    
    
    bool isConnected() { return api.is_connected(); }
    gtl::ServerState const & getServerState() { return api.get_server_state(); }
    
    
    
    //------------------------------------------------------------------------------------------
    //                                  TODO: calibration
    //------------------------------------------------------------------------------------------
    
    
};

