#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    // Prepare app window
    ofSetFrameRate(FRAME_RATE);
    ofSetVerticalSync(true);

    // Prepare webcam
    cam.listDevices();
    cam.setDeviceID(1);
    cam.setup(CAM_WIDTH, CAM_HEIGHT);
    cam.setDesiredFrameRate(CAM_FRAME_RATE);
    webcamScreenScale = SCREEN_WIDTH / CAM_WIDTH;
    
    // Set detection timer as expired
    detectionTimer = DETECTION_TIMEOUT;
    
    // Prepare pointer
    pointerManager = OSPointerManager();
    strokeInitiated = false;
}

//--------------------------------------------------------------
void ofApp::update(){

    isDrawing = false;

    if ( cam.isInitialized() ) {
        
        // Update webcam image
        cam.update();
        camPixels = cam.getPixels();
        
        // Use grayscale for better performance
        camPixels.setImageType(OF_IMAGE_GRAYSCALE);
        
        // Get brightest point
        maxBrightness = 0;
        maxBrightnessX = 0;
        maxBrightnessY = 0;
        for (int x = 0; x < CAM_WIDTH; x++) {
            for (int y = 0; y < CAM_HEIGHT; y++) {
                ofColor pointColor = camPixels.getColor(x, y);
                float pointBrightness = pointColor.getBrightness();
                if (pointBrightness > maxBrightness) {
                    maxBrightness = pointBrightness;
                    maxBrightnessX = CAM_WIDTH - x;
                    maxBrightnessY = y;
                }
            }
        }
        
        // Check if the conditions are right to send the position
        if (maxBrightness >= BRIGHTNESS_TRESHOLD) {
            detectionTimer = 0;
            point.x = maxBrightnessX * webcamScreenScale;
            point.y = maxBrightnessY * webcamScreenScale;
            isDrawing = true;
        }
        else if (detectionTimer < DETECTION_TIMEOUT) {
            detectionTimer++;
            isDrawing = true;
        }
        
        if (isDrawing) {
            pointerManager.dragTo(point.x, point.y);
            if (!strokeInitiated) {
                pointerManager.leftButtonDown();
                strokeInitiated = true;
            }

        }
        else {
            if (strokeInitiated) {
                pointerManager.leftButtonUp();
                strokeInitiated = false;
            }
        }
        
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (DEBUG) {
        cam.draw(0,0);
        
        if (isDrawing) {
            ofPushStyle();
            ofSetColor(255, 0, 0);
            ofNoFill();
            ofDrawCircle(maxBrightnessX, maxBrightnessY, 40);
            ofPopStyle();
        }
        
        cout << "X: " + ofToString(point.x) + ", Y: " + ofToString(point.y) + ", B: " + ofToString(maxBrightness) << endl;
    }
}
