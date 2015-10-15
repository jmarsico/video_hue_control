#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxPhilipsHue.h"
#include "ofxCsv.h"
#include "ofxXmlSettings.h"
#include "ofxGui.h"
#include "ofxAVFVideoPlayer.h"
#include "ofxGui.h"
#include "threaded_hue_call.h"

using namespace wng;



class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void exit(){};

    void keyPressed(int key);
    void mousePressed( int x, int y, int button );
    float lastHue, lastBrightness;
    
    ofxCsv csv;
    ofxAVFVideoPlayer vid;
    threaded_hue_call hueCall;
    
    ofxXmlSettings config;
    
    ofParameter<float> vidPos;
    ofxPanel gui;
    
    
    //struct
    struct transition {
        int time;
        int light;
        bool prevState;
        bool curState;
    };
    
    bool bDisplayState;
    int numTransitions;
    vector <transition> transitions;
    int vidStartTime;
    int vidTime;
    
    string hueIP;
    string hueUserName;
    ofColor hueColor;
    float hueTemp;
    bool resetState;
    int lastFrame;
    int lastTime;
};

#endif
