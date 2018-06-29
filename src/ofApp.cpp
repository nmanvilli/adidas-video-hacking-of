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

    // Calculate webcam detection field
    camDetectionWidth = CAM_WIDTH - CAM_X_BEGIN_GAP - CAM_X_END_GAP;
    camDetectionHeight = CAM_HEIGHT - CAM_Y_BEGIN_GAP - CAM_Y_END_GAP;
    camDetectionDiagonal = sqrt( pow(camDetectionWidth, 2) + pow(camDetectionHeight, 2) );

    // Calculate webcam to screen scale
    webcamScreenScale = SCREEN_WIDTH / camDetectionWidth;
    
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
        for (int x = CAM_X_BEGIN_GAP; x < (camDetectionWidth + CAM_X_BEGIN_GAP); x++) {
            for (int y = CAM_Y_BEGIN_GAP; y < (camDetectionHeight + CAM_Y_BEGIN_GAP); y++) {
                ofColor pointColor = camPixels.getColor(x, y);
                float pointBrightness = pointColor.getBrightness();
                if (pointBrightness > maxBrightness) {
                    maxBrightness = pointBrightness;
                    maxBrightnessX = camDetectionWidth - (x - CAM_X_BEGIN_GAP);
                    maxBrightnessY = y - CAM_Y_BEGIN_GAP;
                }
            }
        }

        // Check if the conditions are right to send the position
        if (maxBrightness >= BRIGHTNESS_TRESHOLD) {
            detectionTimer = 0;
            
            if (strokeInitiated) {
                // If the stroke has already started, interpolate to new position
                ofVec2f newPoint = ofVec2f( maxBrightnessX * webcamScreenScale, maxBrightnessY * webcamScreenScale );
                float distToNewPoint = point.distance(newPoint);
                if (distToNewPoint > 0.0 ) {
                    float speedToNewPoint = ofMap(distToNewPoint, 0, camDetectionDiagonal, 0, 1) * MOVEMENT_INTERPOLATION_SPEED_FACTOR;
                    cout << speedToNewPoint << endl;
                    point.interpolate(newPoint, speedToNewPoint);
                }
            }
            else {
                // New stroke: set starting position
                point.x = maxBrightnessX * webcamScreenScale;
                point.y = maxBrightnessY * webcamScreenScale;
            }
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
        cam.draw(SCREEN_WIDTH, 0, -SCREEN_WIDTH, SCREEN_HEIGHT);
        
        if (isDrawing) {
            ofPushStyle();
            ofSetColor(255, 0, 0);
            ofNoFill();
            ofDrawCircle(point.x, point.y, 40);
            ofPopStyle();
        }
        
        //cout << "X: " + ofToString(point.x) + ", Y: " + ofToString(point.y) + ", B: " + ofToString(maxBrightness) << endl;
    }
}
