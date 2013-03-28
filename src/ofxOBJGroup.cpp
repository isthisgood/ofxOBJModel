//
//  ofxOBJGroup.cpp
//  SphereMapper
//
//  Created by Marek Bereza on 15/05/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "ofxOBJGroup.h"


#include <float.h>

#pragma mark ObjMesh
ofxOBJGroup::ofxOBJGroup(string name) {
	this->name = name;
}



void ofxOBJGroup::addFace(ofxOBJFace face) {
	faces.push_back(face);
}

void ofxOBJGroup::draw(bool drawSolid) {
	for(int i = 0; i < faces.size(); i++) {
		faces[i].draw(drawSolid);
	}
}

ofxOBJFace ofxOBJGroup::createTriFromPoly(const ofxOBJFace &poly, int offset) {
	ofxOBJFace face;
	for(int i = offset; i < offset + 3; i++) {
		int index = i % poly.vertices.size();
		face.addVertex(poly.vertices[index]);
		if(poly.normals.size()>i)   face.addNormal(poly.normals[index]);
		if(poly.texCoords.size()>i) face.addTexCoord(poly.texCoords[index]);
	}
	
	return face;
}
void ofxOBJGroup::triangulateQuads() {
	vector<ofxOBJFace> newFaces;
	for(int i = 0; i < faces.size(); i++) {
		if(faces[i].vertices.size()==4) {
			
			
			newFaces.push_back(createTriFromPoly(faces[i], 0));
			newFaces.push_back(createTriFromPoly(faces[i], 2));
			faces.erase(faces.begin()+i);
			i--;
			// this is a quad so triangulate it.
		}
	}
	
	for(int i = 0; i < newFaces.size(); i++) {
		faces.push_back(newFaces[i]);
	}
}


// makes the code look nicer.
#define FOR_EACH_POINT_IN_MESH for(int i = 0; i < faces.size(); i++) for(int j = 0; j < faces[i].vertices.size(); j++)

void ofxOBJGroup::getBounds(ofVec3f &minPoint, ofVec3f &maxPoint) {
	minPoint.x = minPoint.y = minPoint.z = FLT_MAX;
	maxPoint.x = maxPoint.y = maxPoint.z = -FLT_MAX;
	FOR_EACH_POINT_IN_MESH {
		ofVec3f p = faces[i].vertices[j];
		if(p.x>maxPoint.x) maxPoint.x = p.x;
		if(p.y>maxPoint.y) maxPoint.y = p.y;
		if(p.z>maxPoint.z) maxPoint.z = p.z;

		if(p.x<minPoint.x) minPoint.x = p.x;
		if(p.y<minPoint.y) minPoint.y = p.y;
		if(p.z<minPoint.z) minPoint.z = p.z;
	}
}

// this moves the whole mesh by the specified amount
void ofxOBJGroup::translate(float dx, float dy, float dz) {
	ofVec3f delta = ofVec3f(dx, dy, dz);
	FOR_EACH_POINT_IN_MESH {
		faces[i].vertices[j] += delta;
	}
}


// these functions move the extremity of the mesh.
// eg moveTop() will move the top vertex/vertices by
// the specified amount.
void ofxOBJGroup::moveTop	(float delta) {
	float extrema = FLT_MAX;

	// find the extreme
	FOR_EACH_POINT_IN_MESH {
		if(faces[i].vertices[j].y<extrema) {
			extrema = faces[i].vertices[j].y;
		}
	}

	// alter all the values that have the same extreme
	FOR_EACH_POINT_IN_MESH {
		if(faces[i].vertices[j].y==extrema) {
			faces[i].vertices[j].y += delta;
		}
	}
}

void ofxOBJGroup::moveBottom	(float delta) {

	float extrema = -FLT_MAX;

	// find the extreme
	FOR_EACH_POINT_IN_MESH {
		if(faces[i].vertices[j].y>extrema) {
			extrema = faces[i].vertices[j].y;
		}
	}

	// alter all the values that have the same extreme
	FOR_EACH_POINT_IN_MESH {
		if(faces[i].vertices[j].y==extrema) {
			faces[i].vertices[j].y += delta;
		}
	}
}


void ofxOBJGroup::moveLeft	(float delta) {


	float extrema = FLT_MAX;

	// find the extreme
	FOR_EACH_POINT_IN_MESH {
		if(faces[i].vertices[j].x<extrema) {
			extrema = faces[i].vertices[j].x;
		}
	}

	// alter all the values that have the same extreme
	FOR_EACH_POINT_IN_MESH {
		if(faces[i].vertices[j].x==extrema) {
			faces[i].vertices[j].x += delta;
		}
	}
}

void ofxOBJGroup::moveRight	(float delta) {
	float extrema = -FLT_MAX;

	// find the extreme
	FOR_EACH_POINT_IN_MESH {
		if(faces[i].vertices[j].x>extrema) {
			extrema = faces[i].vertices[j].x;
		}
	}

	// alter all the values that have the same extreme
	FOR_EACH_POINT_IN_MESH {
		if(faces[i].vertices[j].x==extrema) {
			faces[i].vertices[j].x += delta;
		}
	}
}

/**
 * Changes the direction of all the vertices.
 * i.e. clockwise to anti-clockwise
 */
void ofxOBJGroup::flipDirection() {
	for(int i = 0; i < faces.size(); i++) {
		faces[i].flipDirection();
	}
}


/**
 * Shifts the points along one
 */
void ofxOBJGroup::shiftPointsLeft() {
	for(int i = 0; i < faces.size(); i++) {
		faces[i].shiftPointsLeft();
	}
}

void ofxOBJGroup::shiftPointsRight() {
	for(int i = 0; i < faces.size(); i++) {
		faces[i].shiftPointsRight();
	}
}





void ofxOBJGroup::flipNormals() {
	for(int i = 0; i < faces.size(); i++) {
		faces[i].flipNormals();
	}
}