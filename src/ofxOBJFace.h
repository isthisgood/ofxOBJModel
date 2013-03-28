//
//  ofxOBJFace.h
//  SphereMapper
//
//  Created by Marek Bereza on 15/05/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"


class ofxOBJFace {
public:

	ofxOBJFace(string name = "");

	vector<ofVec3f> vertices;
	vector<ofPoint> normals;
	vector<ofPoint> texCoords;

	void draw(bool drawSolid = true);

	void addVertex(const ofVec3f &vertex);
	void addNormal(const ofVec3f &normal);
	void addTexCoord(const ofVec3f &texCoord);
	/**
	 * Changes the direction of all the vertices.
	 * i.e. clockwise to anti-clockwise
	 */
	void flipDirection();

	void swapYZ();
    void createFlatProjectionTexCoords(const ofRectangle &rect);
    void scale(float factor);

	/**
	 * Shifts the points along one
	 */
	void shiftPointsLeft();
	void shiftPointsRight();

	/**
	 * Gives you a rectangle in 2d
	 * of the face.
	 */
	ofRectangle get2DRect();

	/**
	 * Get the minimum and maximum bounding points
	 */
	void getBounds(ofVec3f &min, ofVec3f &max);
	void flipNormals();
	
	void calculateFlatNormals();
	string name;
};
