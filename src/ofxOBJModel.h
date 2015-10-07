/*
 *  ofxOBJModel.h
 *
 *  This is a really simple wavefront obj loader/saver.
 *  It's no good for anything complicated, just good for
 *  loading and saving simple 3d meshes for projection mapping.
 *
 *  You can create files this format reads and writes in
 *  Cinema4D, you can name the objects in cinema 4d so you
 *  can reference them here. Each object shows up as an "ObjMesh"
 *  here.
 *
 *  You must make sure in cinema4d that you convert your shapes
 *  to meshes - you do this by selecting the object and pressing
 *  the 'c' key - the key for 'make editable.. It'll only work on
 *  3d objects (including planes) so if you have a path, you'll need
 *  to use the extrude nurbs object, with the extrusion depth set to
 *  zero, then press 'c'.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxOBJGroup.h"

template<class GroupClass>
class _ofxOBJModel {
public:

	_ofxOBJModel();
	/**
	 * Accessible array of groups in the OBJ file.
	 */
	vector<GroupClass> groups;

	/**
	 * load an obj file, put in the data/ dir.
	 */
	bool load(string path, bool objectsAsGroups = false);
	
	
	/**
	 * Divides any quads into triangles.
	 */
	void triangulateQuads();

	/**
	 * save the obj file, relative to data/ dir.
	 * If you call save with no parameter, it saves
	 * to where the obj was loaded
	 */
	bool save(string file = "");

	void swapYZ();
    void createFlatProjectionTexCoords();


	void clear();
	
	void addGroup(GroupClass group);
	
	
	
    /**
     * This scales the whole model, whilst preserving the normals.
     */
    void scale(float factor);

	/**
	 * Finds the minimum and maximum points in all the meshes.
	 */
	void getBounds(ofVec3f &minPoint, ofVec3f &maxPoint);

	/**
	 * Draw all the meshes.
	 */
	void draw(bool drawSolid = true);

	ofVboMesh *getVboMesh();

	GroupClass *getGroup(string name);

	vector<string> getGroupNames();
	
	void flipNormals();
	
	ofRectangle getTexCoordBounds();
    ofVec3f modelCenter;
    
private:

	void parseFace(ofxOBJFace &face, const string &def, const vector<ofVec3f> &vertices,
				   const vector<ofVec3f> &normals, const vector<ofVec2f> &texCoords);
	void createMesh();
	ofVboMesh mesh;

	ofVec3f parseCoords(string line);
	string filePath;
    
protected:
    bool meshDirty;
};

typedef _ofxOBJModel<ofxOBJGroup> ofxOBJModel;

