#include "ofApp.h"


void ofApp::setup() {

    ofBackground(0);
    
    model.load("pigeon.obj");
    
}


void ofApp::update() {

}


void ofApp::draw() {


    ofEnableDepthTest();

    cam.begin();
    
    model.getVboMesh()->draw();
    
    
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

