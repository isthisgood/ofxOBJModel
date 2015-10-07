#pragma once

#include "ofMain.h"

#include "ofxOBJModel.h"

#include "ofxPhysicalOBJModel.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);

    ofEasyCam cam;
//    ofxOBJModel model;
    ofxPhysicalOBJModel model;
    ofImage modelTexture;
    
    bool startExplosion;
    int explosionForce;
    bool useTexture;
};
