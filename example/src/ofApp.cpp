#include "ofApp.h"


void ofApp::setup() {

    ofBackground(0);
    
    model.load("pigeon.obj");
    
    ofEnableNormalizedTexCoords();
    modelTexture.load("M_008_Vray_mat1.jpg");
    
    cam.setPosition(0, 0, 300);
    startExplosion = false;
    explosionForce = 1;
    useTexture = false;
}


void ofApp::update() {

    if (startExplosion) {
        //        omodel.explodeMesh(5);
        model.update();
    }
}


void ofApp::draw() {


    ofEnableDepthTest();

    cam.begin();
    
    float start = ofGetElapsedTimef();
    
    if (useTexture) modelTexture.bind();
    model.getVboMesh()->draw();
    if (useTexture) modelTexture.unbind();

    
    cam.end();
    
    stringstream ss;
    ss << "explode pigeon with space bar" << endl;
    ss << "current explosion force = " << explosionForce << " (use UP & DOWN to change)" << endl;
//    ss << "drawn in " << (ofGetElapsedTimef() - start) << "s" << endl;
    
    ofSetColor(255);
    ofDrawBitmapString(ss.str(), 10, 10);
}


void ofApp::keyPressed(int key) {

    if (key == OF_KEY_UP) explosionForce++; //omodel.explodeMesh(d);
    else if (key == OF_KEY_DOWN) explosionForce--; //omodel.explodeMesh(-d);
    cout << "explosionForce = " << explosionForce << endl;
    
    if (key == 'r') {
        model.clear();
        model.load("pigeon.obj");
        
        startExplosion = false;
    }
    
    if (key == ' ') {
        startExplosion = !startExplosion;
        model.explodeMesh(explosionForce);
    }
    
    if (key == 't') {
        useTexture = !useTexture;
    }


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

