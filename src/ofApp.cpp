#include "ofApp.h"

bool state = false; //on / off
int numLights = 3;

void ofApp::setup(){

	lastBrightness = 0;
	ofBackground(0);
//	ofSetFrameRate(25); //ghetto limit for the api call rate
	ofSetWindowTitle("Marsico_PCA");
    

    // load the config.xml and get it's settings
    config.load("config.xml");
    hueIP = config.getValue("HUEIP", "192.168.0.12");
    hueUserName = config.getValue("HUEUSER", "2234a0e441e0502689a6f3c75d25f");
    string movieName = config.getValue("MOVIENAME", "movie.mov");
    string transName = config.getValue("TRANSNAME", "transitions.csv");
    int framerate = config.getValue("FRAMERATE", 30);
    ofSetFrameRate(framerate);
    
    int r = config.getValue("HUECOLR", 255);
    int g = config.getValue("HUECOLG", 255);
    int b = config.getValue("HUECOLB", 255);
    
    hueColor.set(r,g,b);
    hueTemp = config.getValue("COLTEMP", 0.7);
  
    
    // Load a CSV File for transitions.
    csv.loadFile(ofToDataPath(transName));
    numTransitions = csv.numRows;
    gui.setup();
    gui.add(vidPos.set("video position", 0.0, 0.0, 1.0));
    gui.setPosition(10, 10);
    
    bDisplayState = true;
    resetState = false;
    
    
    //setup the Hue call thread
    //setup the thread with IP, username, number of transitions, light to be turned ON
    hueCall.setup(hueIP, hueUserName, numTransitions, hueTemp, hueColor);
    
    for(int i = 0; i < csv.numRows; i++)
    {
        transition t;
        t.time = csv.getInt(i, 0);
        t.light = csv.getInt(i, 1);
        t.prevState = false;
        t.curState = false;
        transitions.push_back(t);
    }
    
    
//    vid.setUseTexture(true);
    vid.loadMovie(movieName);
    vid.setLoopState(OF_LOOP_NORMAL);
    vid.play();
    vidTime = 0;
    
}

void ofApp::update(){
    
    vid.update();
    vidTime = vid.getDuration() * vid.getPosition();
    
    if( lastTime!= 1 && vidTime == 1)
    {
        ofLog() << "RESETING -------------------------------";
        hueCall.setup(hueIP, hueUserName, numTransitions, hueTemp, hueColor);
//        hueCall.startAllOff();
    }
    
    //run through all of the transitions
    for(int i = 0; i < transitions.size(); i++)
    {
        //if a transition time happens, change state to true
        if(vidTime == transitions[i].time)
        {
            transitions[i].curState = true;
        }
        
        //otherwise, keep the transitions state false
        else
        {
            transitions[i].curState = false;
        }
        
        //if a trans state was false and is now true, trigger the light
        if(!transitions[i].prevState && transitions[i].curState)
        {
            
            ofLog() << "transition time: " << ofToString(transitions[i].time) << " | light " << ofToString(transitions[i].light);
    
            //if the transition light number is 100, turn all lights on
            if(transitions[i].light == 100)
            {
                hueCall.startAllOn();
            }
            
            //if a transistion light is 101, turn all lights off
            else if(transitions[i].light == 101)
            {
                hueCall.startAllOff();
            }
            
            //otherwise, change that one light
            else
            {
                //start the hue thread to make the calls, it will stop when finished
                hueCall.start(transitions[i].light);
                ofLog() << "start single light";
            }
        }
        
        //copy the current state to the prev state.
        transitions[i].prevState = transitions[i].curState;
    }
    
    //update the lasttime variable
    lastTime = vidTime;
}

//-------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    vid.draw(0, 0, ofGetWidth(), ofGetWidth() * 9/16);
    

    
    if(!bDisplayState)
    {
        
        ofSetColor(0, 200);
        ofRect(30, 30, 300, 240);
        
        gui.draw();
        ofSetColor(0, 255, 0);
        stringstream ss;
        ss << "Config Settings: \n"
        << "Number of Transitions: " << ofToString(numTransitions) << endl
        << "Hue IP: " << hueIP << endl
        << "Hue User: " << hueUserName << endl
        << "framerate: " << ofToString(ofGetFrameRate()) << endl
        << "screen width: " << ofToString(ofGetWidth()) << endl
        << "vidTime: " << ofToString(vidTime) << endl
        << "current frame: " << ofToString(vid.getCurrentFrame()) << endl
        << "playback speed: " << ofToString(vid.getSpeed());

        ofDrawBitmapString(ss.str(), 40, 40);
        
        
        ofSetColor(hueColor);
        ofCircle(80,250,50);
        ofShowCursor();
    } else ofHideCursor();

}


void ofApp::keyPressed(int key){
    if(key == 'f')
    {
       ofToggleFullscreen();
        bDisplayState = !bDisplayState;
    }
    
}

void ofApp::mousePressed( int x, int y, int button ){

}
