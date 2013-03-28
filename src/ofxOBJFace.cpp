//
//  ofxOBJFace.cpp
//  SphereMapper
//
//  Created by Marek Bereza on 15/05/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "ofxOBJFace.h"
#include <float.h>

ofxOBJFace::ofxOBJFace(string name) {
	this->name = name;
}

void ofxOBJFace::addVertex(const ofVec3f &vertex) {
	vertices.push_back(vertex);
}
void ofxOBJFace::addNormal(const ofVec3f &normal) {
	normals.push_back(normal);
}
void ofxOBJFace::addTexCoord(const ofVec3f &texCoord) {
	texCoords.push_back(texCoord);
}
ofRectangle ofxOBJFace::get2DRect() {
	ofRectangle rect;
	float minX = FLT_MAX;
	float minY = FLT_MAX;
	float maxX = -FLT_MAX;
	float maxY = -FLT_MAX;
	for(int i = 0; i < vertices.size(); i++) {
		if(vertices[i].x>maxX) maxX = vertices[i].x;
		if(vertices[i].y>maxY) maxY = vertices[i].y;
		if(vertices[i].x<minX) minX = vertices[i].x;
		if(vertices[i].y<minY) minY = vertices[i].y;
	}

	rect.x = minX;
	rect.y = minY;
	rect.width = maxX - minX;
	rect.height = maxY - minY;
	return rect;
}


void ofxOBJFace::swapYZ() {

    for(int i = 0; i < vertices.size(); i++) {
        float tmp = vertices[i].z;
        vertices[i].z = vertices[i].y;
        vertices[i].y = tmp;
        vertices[i].y *= -1;
    }
    for(int i = 0; i < normals.size(); i++) {
        float tmp = normals[i].z;
        normals[i].z = normals[i].y;
        normals[i].y = tmp;
        normals[i].y *= -1;
    }
}

void ofxOBJFace::createFlatProjectionTexCoords(const ofRectangle &rect) {
    texCoords.clear();
    for(int i = 0; i < vertices.size(); i++) {
        // map the coordinates of the vertex with respect to the rect to tex coords
        float x = (vertices[i].x - rect.x)/rect.width;
        float y = (vertices[i].y - rect.y)/rect.height;
        texCoords.push_back(ofVec2f(x, y));
    }
}

void ofxOBJFace::scale(float factor) {
    for(int i = 0; i < vertices.size(); i++) {
        vertices[i] *= factor;
    }
}


void ofxOBJFace::draw(bool drawSolid) {

	if(drawSolid) {
		if(vertices.size()==3) {
			glBegin(GL_TRIANGLES);
		} else if(vertices.size()==4) {
			glBegin(GL_QUADS);
		} else {
			glBegin(GL_POLYGON);
		}

		bool hasNormals = vertices.size()==normals.size();
		bool hasTexCoords = vertices.size()==texCoords.size();

		for(int i = 0; i < vertices.size(); i++) {
			if(hasNormals) glNormal3f(normals[i].x, normals[i].y, normals[i].z);
			if(hasTexCoords) glTexCoord2f(texCoords[i].x, texCoords[i].y);
			glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
		}
		glEnd();
		for(int i = 0; i < vertices.size(); i++) {
			glPushMatrix();
			glTranslatef(vertices[i].x, vertices[i].y, vertices[i].z);
			//ofDrawBitmapString(ofToString(texCoords[i].x)+","+ofToString(texCoords[i].y), 0, 0);
			glPopMatrix();
		}
	} else {
		glBegin(GL_LINE_LOOP);
		for(int i = 0; i < vertices.size(); i++) {
			glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
		}
		glEnd();
	}
}


/**
 * Changes the direction of all the vertices.
 * i.e. clockwise to anti-clockwise
 */
void ofxOBJFace::flipDirection() {
	std::reverse(vertices.begin(), vertices.end());
}


/**
 * Shifts the points along one
 */
void ofxOBJFace::shiftPointsLeft() {
	std::rotate(vertices.begin(), vertices.begin()+vertices.size()-1, vertices.end());
}

void ofxOBJFace::shiftPointsRight() {
	std::rotate(vertices.begin(), vertices.begin()+1, vertices.end());
}



void ofxOBJFace::getBounds(ofVec3f &min, ofVec3f &max) {

	min.x = min.y = min.z = FLT_MAX;
	max.x = max.y = max.z = -FLT_MAX;

	for(int i = 0; i < vertices.size(); i++) {
		if(vertices[i].x>max.x) max.x = vertices[i].x;
		if(vertices[i].y>max.y) max.y = vertices[i].y;
		if(vertices[i].z>max.z) max.z = vertices[i].z;
		if(vertices[i].x<min.x) min.x = vertices[i].x;
		if(vertices[i].y<min.y) min.y = vertices[i].y;
		if(vertices[i].z<min.z) min.z = vertices[i].z;
	}
}

void ofxOBJFace::flipNormals() {
	for(int i = 0; i < normals.size(); i++) {
		normals[i] *= -1;
	}
}

void ofxOBJFace::calculateFlatNormals() {
	normals.clear();

	ofVec3f c = vertices[0] - vertices[1];
	ofVec3f a = vertices[2] - vertices[1];
	
	
	ofVec3f normal = a.getCrossed(c);
	normal.normalize();
	for(int i = 0; i < vertices.size(); i++) {
		normals.push_back(normal);
	}
	
}