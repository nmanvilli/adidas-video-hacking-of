#pragma once

#include "ofMain.h"
#include "OSPointerManager.h"

class ofApp : public ofBaseApp{

	public:

        // If true, draw inside window, show detection marker and print position and brightness
        const bool DEBUG = true;
    
        // Screen configuration
        const int SCREEN_WIDTH = 1024;
        const int SCREEN_HEIGHT = 768;
        const int FRAME_RATE = 60;
    
        // Webcam configuration
        const int CAM_WIDTH = 640;
        const int CAM_HEIGHT = 480;
        const int CAM_FRAME_RATE = 60;

        const int CAM_X_BEGIN_GAP = 40;
        const int CAM_X_END_GAP = 20;
        const int CAM_Y_BEGIN_GAP = 5;
        const int CAM_Y_END_GAP = 5;
    
        float camDetectionWidth;
        float camDetectionHeight;

        // Detection configuration
        const int BRIGHTNESS_TRESHOLD = 220;
        const int DETECTION_TIMEOUT = 12; // expressed in number of frames

        // Base OF methods
		void setup();
		void update();
        void draw();

        // Class that handles the webcam input
        ofVideoGrabber cam;
        ofPixels camPixels;
    
        // Ratio between webcam resolution and screen resolution (depends on width)
        float webcamScreenScale;
    
        // Flag: sending coordinates while it is true
        bool isDrawing;
        bool strokeInitiated;
    
        /* Timer used to keep pointer in position in case of brief
         * interruptions of the light.
         */
        int detectionTimer;
    
        // Information about the light detected
        float maxBrightness;
        int maxBrightnessX;
        int maxBrightnessY;

        // Pointer management
        ofVec2f point;
        OSPointerManager pointerManager;
        float diagonal;

};
