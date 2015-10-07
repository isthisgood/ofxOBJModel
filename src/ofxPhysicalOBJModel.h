//
//  PhysicalOBJModel.h
//  ofxOBJModel_example
//
//  Created by Will Gallia on 07/10/2015.
//
//

#pragma once

#include "ofxOBJModel.h"

class ofxPhysicalOBJFace : public ofxOBJFace {
public:
    ofxPhysicalOBJFace(string name="");
    
protected:
    ofVec3f center;
    ofVec3f vel, acc;
    float floorPos;
    float rotation;
    
public:
    void applyForce(ofVec3f force);
    void update();
    void setFloor(float floorYPos);
    ofVec3f moveOutFrom(ofVec3f p, float amount);
    void rotate(ofVec3f axis);
    
protected:
    
    void calcCenter();
};

typedef _ofxOBJGroup<ofxPhysicalOBJFace> ofxPhysicalOBJGroup;

class ofxPhysicalOBJModel : public _ofxOBJModel<ofxPhysicalOBJGroup> {
public:

    ofxPhysicalOBJModel(): _ofxOBJModel<ofxPhysicalOBJGroup>() {}
    static const ofVec3f MODEL_CENTER;
    void explodeMesh(float amount=10.0, ofVec3f fromPoint=ofxPhysicalOBJModel::MODEL_CENTER);
    
    ofVec3f modelCenter;
    
    void reset();
    void update();
    
//    vector<ofVec3f> paintPoints;
//    ofVboMesh paintMesh;
//    int paintAlpha;
};

