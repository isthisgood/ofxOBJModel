#include "ofApp.h"


void ofApp::setup() {

    ofBackground(0);
    
    model.load("pigeon.obj");
    
    cam.setPosition(0, 0, 300);
}


void ofApp::update() {

}


void ofApp::draw() {

    cam.begin();
    
//    float start = ofGetElapsedTimef();
//    model.draw();
//    cout << "model.draw() = " << (ofGetElapsedTimef() - start) << endl;

    float start = ofGetElapsedTimef();
    model.getVboMesh()->draw();
    ofDrawBitmapString(ofToString(ofGetElapsedTimef() - start), 100, 100);

    
    cam.end();
}


void ofApp::keyPressed(int key) {

}


void ofApp::keyReleased(int key) {

}


void ofApp::mouseMoved(int x, int y) {

}


void ofApp::mouseDragged(int x, int y, int button) {

}


void ofApp::mousePressed(int x, int y, int button) {

}


void ofApp::mouseReleased(int x, int y, int button) {

}


void ofApp::windowResized(int w, int h) {

}

