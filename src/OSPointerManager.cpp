
#include "OSPointerManager.h"

//--------------------------------------------------------------
void OSPointerManager::dragTo(int x, int y) {
    // Move mouse pointer
    macPoint.x = x;
    macPoint.y = y;
    mouseEvent = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDragged, macPoint, kCGMouseButtonLeft);
    CGEventPost(kCGHIDEventTap, mouseEvent);
}

//--------------------------------------------------------------
void OSPointerManager::leftButtonDown() {
    mouseEvent = CGEventCreateMouseEvent (NULL, kCGEventLeftMouseDown, macPoint, kCGMouseButtonLeft);
    CGEventPost(kCGHIDEventTap, mouseEvent);
}

//--------------------------------------------------------------
void OSPointerManager::leftButtonUp() {
    mouseEvent = CGEventCreateMouseEvent (NULL, kCGEventLeftMouseUp, macPoint, kCGMouseButtonLeft);
    CGEventPost(kCGHIDEventTap, mouseEvent);
}
