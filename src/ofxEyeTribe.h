#pragma once

#include "ofMain.h"
#include <include/gazeapi.h>

class ofxEyeTribe : public gtl::IGazeListener
{
    
    gtl::GazeApi api;
    ofVec2f mPoint2d;
    
public:
    
    ofxEyeTribe()
    {
    }
    
    ~ofxEyeTribe()
    {
        this->close();
    }
    
    bool open()
    {
        if (api.is_connected()) return true;
        if (api.connect(true))
        {
            api.add_listener(*this);
            return true;
        }
        return false;
    }
    
    void close()
    {
        api.remove_listener(*this);
        api.disconnect();
    }
    
    void on_gaze_data(const gtl::GazeData &gaze_data)
    {
        if (gaze_data.state && gtl::GazeData::GD_STATE_TRACKING_GAZE)
        {
            gtl::Point2D p = gaze_data.avg;
            mPoint2d.set(p.x, p.y);
        }
    }
    
    ofVec2f & getPoint2d() { return mPoint2d; }
    bool isConnected() { return api.is_connected(); }
    
};

/*
class ofxEyeTribe
{
    ofxEyeTribe(){}
    ofxEyeTribe(const ofxEyeTribe&){};
    ofxEyeTribe& operator=(const ofxEyeTribe&){};
    
    static GazeListener * mGazeListener;
    
    
public:
    
    ~ofxEyeTribe()
    {
        if (mGazeListener != NULL) delete mGazeListener;
    }
    
    static bool open()
    {
        if (mGazeListener == NULL) mGazeListener = new GazeListener();
        return mGazeListener->open();
    }
    
    static bool close()
    {
        if (mGazeListener == NULL)
        {
            ofLogError("ofxEyeTribe", "listener still not opened");
            return false;
        }
        mGazeListener->close();
        return true;
    }
    
    static ofVec2f getGazePoint()
    {
        if (mGazeListener == NULL)
        {
            ofLogError("ofxEyeTribe", "listener still not opened");
            return ofVec2f(-1, -1);
        }
        return mGazeListener->getPoint2d();
    }
    
    static bool isConnected()
    {
        if (mGazeListener == NULL)
        {
            ofLogError("ofxEyeTribe", "listener still not opened");
            return false;
        }
        return mGazeListener->isConnected();
    }
};*/


