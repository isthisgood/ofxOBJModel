//
//  ofxOBJGroup.h
//  SphereMapper
//
//  Created by Marek Bereza on 15/05/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once


#include "ofxOBJFace.h"

template <class FaceClass>
class _ofxOBJGroup {
public:
	vector<FaceClass> faces;
	string name;
	
	_ofxOBJGroup(string name = "untitled");
	
	void addFace(FaceClass face);
	
	void getBounds(ofVec3f &minPoint, ofVec3f &maxPoint);
	
	void draw(bool drawSolid = true);
	
	// this moves the whole mesh by the specified amount
	void translate(float dx, float dy, float dz);
	
	
	void triangulateQuads();
	

	
	// these functions move the extremity of the mesh.
	// eg moveTop() will move the top vertex/vertices by
	// the specified amount.
	void moveTop	(float delta);
	void moveBottom	(float delta);
	void moveLeft	(float delta);
	void moveRight	(float delta);
	
	/**
	 * Changes the direction of all the vertices. 
	 * i.e. clockwise to anti-clockwise
	 */
	void flipDirection();
	
	
	/**
	 * Shifts the points along one
	 */
	void shiftPointsLeft();
	void shiftPointsRight();
	
	void flipNormals();
	
    typedef FaceClass faceClass;
    
private:
	FaceClass createTriFromPoly(const FaceClass &poly, int offset);
};


typedef _ofxOBJGroup<ofxOBJFace> ofxOBJGroup;