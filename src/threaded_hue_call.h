//
//  threaded_hue_call.h
//  video_hue_control
//
//  Created by Jakob Marsico on 8/16/15.
//
//
#pragma once


#include "ofThread.h"
#include "ofxPhilipsHue.h"


/// This is a simple example of a ThreadedObject created by extending ofThread.
/// It contains data (count) that will be accessed from within and outside the
/// thread and demonstrates several of the data protection mechanisms (aka
/// mutexes).
class threaded_hue_call: public ofThread
{
public:
    /// Create a ThreadedObject and initialize the member
    /// variable in an initialization list.
    threaded_hue_call()
    {
        
    }
    
    void setup(string IP, string username, int numTrans, float ct, ofColor col)
    {
        hue.setup(IP.c_str(), username.c_str());
        numTransitions = numTrans;
        colTemp = ct;
        color = col;
        _light = 0;
        lastLight = 0;
        allOn = false;
        allOff = false;
        
        startAllOff();
    
    }

    
    /// Start the thread.
    void start(int light)
    {
        _light = light;
        allOn = false;
        allOff = false;
        // Mutex blocking is set to true by default
        // It is rare that one would want to use startThread(false).
        startThread();
    }
    
    
    void startAllOn()
    {
        allOn = true;
        allOff = false;
        startThread();
    }
    
    void startAllOff()
    {
        allOff = true;
        allOn = false;
        startThread();
    }
    
    /// Signal the thread to stop.  After calling this method,
    /// isThreadRunning() will return false and the while loop will stop
    /// next time it has the chance to.
    void stop()
    {
        stopThread();
    }
    
    /// Our implementation of threadedFunction.
    void threadedFunction()
    {
        while(isThreadRunning())
        {
            if(allOn == false && allOff == false)
            {
                //starting single transition
                ofLog() << "starting sing gtansitions in thread";
                for(int i = 1; i <= 9; i++)
                {
                    //if we have a match, turn it on
                    if(i == _light)
                    {
                        ofLog() << "LIGHT MATCH";
                        
                        //if the light is 9, make that blue
                        if(_light == 9)
                        {
                            hue.setLightColor(9,
                                              true,
                                              color,
                                              1.0,
                                              300
                                              );
                        }
                        
                        //otherwise, make the _light warm
                        else
                        {
                            ofLog() << "CALLING from thread";
                            hue.setLightState(
                                              i,		//light bulb ID #
                                              true,			//on-off
                                              colTemp,  //color Temperature
                                              300   //transition duration in
                                              );
                        }
                        
                    }
                    
                    //otherwise, turn it off (and make sure it's off!)
                    else
                    {
                        hue.setLightState(i, false, colTemp, 300);
                    }
                    ofSleepMillis(100);
                }
                lastLight = _light;
            }
            
            //if we have the allOn flag
            else if(allOn == true)
            {
                //turn off light 9
                hue.setLightState(9, false);
                //turn on lights 1-8
                for(int i = 1; i < 9 ; i++)
                {
                    hue.setLightState(
                                      i,		//light bulb ID #
                                      true,			//on-off
                                      colTemp,  //color Temperature
                                      300   //transition duration in
                                      );
                    ofSleepMillis(100);
                }
                
                lastLight = 0;
            }
            
            //if we ahve the allOff flag
            else if(allOff == true)
            {
                for(int i = 1; i <=9 ; i++)
                {
                    hue.setLightState(
                                      i,		//light bulb ID #
                                      false,			//on-off
                                      colTemp,  //color Temperature
                                      300   //transition duration in
                                      );
                    ofSleepMillis(100);
                }
//                hufe.setLightState(8, false);
            }
            
            
            stopThread();
        }
    }
   
    
protected:
    // This is a simple variable that we aim to always access from both the
    // main thread AND this threaded object.  Therefore, we need to protect it
    // with the mutex.  In the case of simple numerical variables, some
    // garuntee thread safety for small integral types, but for the sake of
    // illustration, we use an int.  This int could represent ANY complex data
    // type that needs to be protected.
    int numTransitions;
    ofxPhilipsHue hue;
    int _light;
    int lastLight;
    ofColor color;
    float colTemp;
    bool allOn;
    bool allOff;
    
    
};
