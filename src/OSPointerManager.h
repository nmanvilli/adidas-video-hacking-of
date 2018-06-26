#pragma once
#include "ofMain.h"

class OSPointerManager {
    
    public:
        CGPoint macPoint;
        CGEventRef mouseEvent;
    
    void dragTo(int x, int y);
    void leftButtonDown();
    void leftButtonUp();

};
