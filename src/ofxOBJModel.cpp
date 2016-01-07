/*
 *  ofxOBJModel.cpp
 *
 */

#include "ofxOBJModel.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#ifndef FLT_MAX
	#include <float.h>
#endif

#pragma mark ofxOBJModel

template<class GroupClass>
_ofxOBJModel<GroupClass>::_ofxOBJModel(): meshDirty(true) {
    
    
}

template<class GroupClass>
void _ofxOBJModel<GroupClass>::flipNormals() {
	for(int i = 0; i < groups.size(); i++) {
		groups[i].flipNormals();
	}

}

template<class GroupClass>
bool _ofxOBJModel<GroupClass>::load(string path, bool objectsAsGroups) {
	filePath = path;
	path = ofToDataPath(path, true);

	string line;

	vector<string> defs;

	// this is a list of all points
	// that we can drop after parsing
	vector<ofVec3f> vertices;
	vector<ofVec3f> normals;
	vector<ofVec2f> texCoords;

	// obj file format vertexes are 1-indexed
	vertices.push_back(ofVec3f());
	normals.push_back(ofVec3f());
	texCoords.push_back(ofVec2f());


	ifstream myfile (path.c_str());
	if (myfile.is_open()) {
		while (! myfile.eof()) {
			getline (myfile,line);


			// parse the obj format here.
			//
			// the only things we're interested in is
			// lines beginning with 'g' - this says start of new object
			// lines beginning with 'v ' - coordinate of a vertex
			// lines beginning with 'vn ' - vertex normals
			// lines beginning with 'vt ' - texcoords (either 2 or 3 values)
			// lines beginning with 'f ' - specifies a face of a shape
			// 			we take each number before the slash as the index
			// 			of the vertex to join up to create a face.



			if(line.find("g ")==0) { // new group definition
				string groupName = line.substr(2);
				defs.push_back(line);
			} else if(line.find("o ")==0) {
				string objectName = line.substr(2);
				defs.push_back(line);
			} else if(line.find("v ")==0) { // new vertex
				vertices.push_back(parseCoords(line));
			} else if(line.find("vn ")==0) {
				normals.push_back(parseCoords(line));
			} else if(line.find("vt ")==0) {
				texCoords.push_back(parseCoords(line));
			} else if(line.find("f ")==0) { // face definition
				defs.push_back(line);
			} else {
				//printf("Not parsing '%s\n'", line.c_str());
			}
		}


		myfile.close();

        
		string nextName = "";
		for(int i = 0; i < defs.size(); i++) {
			if(defs[i].find("g ") == 0 || (defs[i].find("o ") == 0 && objectsAsGroups)) {
				groups.push_back(GroupClass(defs[i].substr(2)));
			} else if(defs[i].find("o ")==0) {
				nextName = defs[i].substr(2);
				
				if(nextName[nextName.size()-1]=='\r') {
					nextName = nextName.substr(0, nextName.size()-1);
					//printf("Next name: '%s'\n", nextName.c_str());
				}
				
			} else if(defs[i].find("f ")==0) {
                groups.back().faces.push_back(typename GroupClass::faceClass(nextName));
				parseFace(groups.back().faces.back(), defs[i], vertices, normals, texCoords);
				nextName = "";
			}
		}


		int numFaces = 0;
		for(int i = 0; i < groups.size(); i++) {
			numFaces += groups[i].faces.size();
		}
        
        ofVec3f sum;
        for (auto vert : vertices) {
            sum+= vert;
        }
        modelCenter = sum / vertices.size();

		printf("Successfully loaded %s\n-----\nVertices: %lu\nGroups: %lu\nNormals: %lu\nTexCoords: %lu\nFaces: %d\n----\n",
					path.c_str(), vertices.size()-1, groups.size(), normals.size()-1, texCoords.size()-1, numFaces);


        ofVec3f min, max;
        getBounds(min, max);
		printf("Size (x,y,z): %.2f, %.2f %.2f\n", max.x-min.x, max.y-min.y, max.z-min.z);
		createMesh();
		return true;
	} else {
		printf("Couldn't find the OBJ file %s\n", path.c_str());
		return false;
	}
}

template<class GroupClass>
void _ofxOBJModel<GroupClass>::clear() {
	groups.clear();
	createMesh();
}

template<class GroupClass>
void _ofxOBJModel<GroupClass>::addGroup(GroupClass group) {
	groups.push_back(group);
	createMesh();
}

template<class GroupClass>
void _ofxOBJModel<GroupClass>::triangulateQuads() {
	
	
	for(int i = 0; i < groups.size(); i++) {
		GroupClass *group = &groups[i];
		group->triangulateQuads();
	}
	createMesh();
}

template<class GroupClass>
void _ofxOBJModel<GroupClass>::swapYZ() {
    for(int i = 0; i < groups.size(); i++) {
        for(int j = 0; j < groups[i].faces.size(); j++) {
            groups[i].faces[j].swapYZ();
        }
    }
    createMesh();
}

template<class GroupClass>
void _ofxOBJModel<GroupClass>::scale(float factor) {
    for(int i = 0; i < groups.size(); i++) {
        for(int j = 0; j < groups[i].faces.size(); j++) {
            groups[i].faces[j].scale(factor);
        }
    }
    createMesh();
}

template<class GroupClass>
void _ofxOBJModel<GroupClass>::createFlatProjectionTexCoords() {
    ofVec3f min, max;
    getBounds(min, max);
    ofRectangle rect(min.x, min.y, max.x-min.x, max.y-min.y);
    for(int i = 0; i < groups.size(); i++) {
        for(int j = 0; j < groups[i].faces.size(); j++) {
            groups[i].faces[j].createFlatProjectionTexCoords(rect);
        }
    }
   // swapYZ();
    createMesh();
}

template<class GroupClass>
void _ofxOBJModel<GroupClass>::parseFace(ofxOBJFace &face, const string &def, const vector<ofVec3f> &vertices,
			   const vector<ofVec3f> &normals, const vector<ofVec2f> &texCoords) {

	// these are the possible options in an obj
	// f v1 v2 v3 v4
	// f v1/vt1 v2/vt2 v3/vt3
	// f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
	// f v1//vn1 v2//vn2 v3//vn3

	if(def.find("f ")==0) {

		vector<string> indices =  ofSplitString(def.substr(2), " ");
		for(int i = 0; i < indices.size(); i++) {
			vector<string> parts = ofSplitString(indices[i], "/");
			if(parts.size()>=1) {
				face.addVertex(vertices[ofToInt(parts[0])]);
			}
			if(parts.size()>=2 && parts[1]!="") {
				face.addTexCoord(texCoords[ofToInt(parts[1])]);
			}
			if(parts.size()>=3) {
				face.addNormal(normals[ofToInt(parts[2])]);
			}
		}

	} else {
		printf("Not a face string in obj file '%s'\n", def.c_str());
	}

}

template<class GroupClass>
bool _ofxOBJModel<GroupClass>::save(string file) {
	if(file=="") {
		file = filePath;
		if(filePath=="") {
			filePath = "untitled.obj";
		}
	}
	file = ofToDataPath(file, true);
	string contents = "# _ofxOBJModel<GroupClass> output\r\n\r\n";
	int vertexIndex = 1;
	string faceStrings = "";
	string n = "\r\n";
	for(int i = 0; i < groups.size(); i++) {

		GroupClass *group = &groups[i];

		contents += n + n + "g " + groups[i].name + n;


		for(int j = 0; j < groups[i].faces.size(); j++) {
			ofxOBJFace *face = &group->faces[j];

			if(face->name!="") {
				contents += "o " + face->name + n;
			}

			bool doingNormals = face->normals.size()==face->vertices.size();
			bool doingTexCoords = face->texCoords.size()==face->vertices.size();


			for(int k = 0; k < face->vertices.size(); k++) {
				ofVec3f p = face->vertices[k];
				contents += "v " + ofToString(p.x) + " " + ofToString(p.y) + " " + ofToString(p.z) + " " +n;
			}



			if(doingNormals) {
				contents += n;
				for(int k = 0; k < face->vertices.size(); k++) {
					ofVec3f p = face->normals[k];
					contents += "vn " + ofToString(p.x) + " " + ofToString(p.y) + " " + ofToString(p.z) + " " +n;
				}

			}

			if(doingTexCoords) {
				contents += n;
				for(int k = 0; k < face->vertices.size(); k++) {
					ofVec3f p = face->texCoords[k];
					contents += "vt " + ofToString(p.x) + " " + ofToString(p.y) + " " + ofToString(p.z) + " " +n;
				}
			}

			if(!doingNormals && !doingTexCoords) {
				contents += "f";
				for(int k = 0; k < face->vertices.size(); k++) {
					contents += " " + ofToString(vertexIndex);
					vertexIndex++;
				}
				contents += n;
			} else if(doingNormals && !doingTexCoords) {
				contents += "f";
				for(int k = 0; k < face->vertices.size(); k++) {
					contents += " " + ofToString(vertexIndex) + "//" + ofToString(vertexIndex);
					vertexIndex++;
				}
				contents += n;

			} else if(doingNormals && doingTexCoords) {
				contents += "f";
				for(int k = 0; k < face->vertices.size(); k++) {
					contents += " " + ofToString(vertexIndex) + "/"+ofToString(vertexIndex)+"/" + ofToString(vertexIndex);
					vertexIndex++;
				}
				contents += n;
			} else if(!doingNormals && doingTexCoords) {
				contents += "f";
				for(int k = 0; k < face->vertices.size(); k++) {
					contents += " " + ofToString(vertexIndex) + "/" + ofToString(vertexIndex);
					vertexIndex++;
				}
				contents += n;
			}





		}

		contents += n;
	}


	ofstream myfile (file.c_str());
	if (myfile.is_open()) {
		myfile << contents;
		myfile.close();
		return true;
	} else {
		printf("Could not write to file\n");
		return false;
	}


}

template<class GroupClass>
ofVec3f _ofxOBJModel<GroupClass>::parseCoords(string line) {
	ofVec3f p;
	line = line.substr(line.find(" ")+1);
	vector<string> elements = ofSplitString(line, " ");
	if(elements.size()<2) {
		printf("Error line does not have at least 2 coordinates: \"%s\"\n", line.c_str());
		return p;
	}
	p.x = atof(elements[0].c_str());
	p.y = atof(elements[1].c_str());
	if(elements.size()>=3) {
		p.z = atof(elements[2].c_str());
	}

	return p;
}

template<class GroupClass>
void _ofxOBJModel<GroupClass>::draw(bool drawSolid) {
	for(int i = 0; i < groups.size(); i++) {
		groups[i].draw(drawSolid);
	}
}


template<class GroupClass>
vector<string> _ofxOBJModel<GroupClass>::getGroupNames() {
	vector<string> groupNames;
	for(int i = 0; i < groups.size(); i++) {
		groupNames.push_back(groups[i].name);
	}
	return groupNames;
}

template<class GroupClass>
void _ofxOBJModel<GroupClass>::getBounds(ofVec3f &minPoint, ofVec3f &maxPoint) {
	minPoint.x = minPoint.y = minPoint.z = FLT_MAX;
	maxPoint.x = maxPoint.y = maxPoint.z = FLT_MIN;

	for(int i = 0; i < groups.size(); i++) {
		ofVec3f min, max;
		groups[i].getBounds(min, max);

		if(max.x>maxPoint.x) maxPoint.x = max.x;
		if(max.y>maxPoint.y) maxPoint.y = max.y;
		if(max.z>maxPoint.z) maxPoint.z = max.z;

		if(min.x<minPoint.x) minPoint.x = min.x;
		if(min.y<minPoint.y) minPoint.y = min.y;
		if(min.z<minPoint.z) minPoint.z = min.z;
	}

}

template<class GroupClass>
GroupClass *_ofxOBJModel<GroupClass>::getGroup(string name) {
	for(int i = 0; i < groups.size(); i++) {
		if(groups[i].name==name) {
			return &groups[i];
		}
	}
	printf("\n--------\nWarning Warning!!!! No mesh named '%s', the program will probably crash.\nPlease update the OBJ file in cinema 4d to have a mesh called '%s'\n", name.c_str(), name.c_str());
	return NULL;
}

template<class GroupClass>
ofVboMesh *_ofxOBJModel<GroupClass>::getVboMesh() {
    if (meshDirty) {
        createMesh();
        meshDirty = false;
    }
	return &mesh;
}

template<class GroupClass>
void _ofxOBJModel<GroupClass>::createMesh() {

	mesh.clear();

	mesh.setUsage(GL_STATIC_DRAW);
	for(int i = 0; i < groups.size(); i++) {
		GroupClass *group = &groups[i];
		for(int j = 0; j < group->faces.size(); j++) {
			ofxOBJFace *face = &group->faces[j];
			bool doingNormals = face->normals.size()==face->vertices.size();
			bool doingTexCoords = face->texCoords.size()==face->vertices.size();


			for(int k = 0; k < face->vertices.size(); k++) {
				mesh.addVertex(face->vertices[k]);
				if(doingNormals) {
					mesh.addNormal(face->normals[k]);
				}
				if(doingTexCoords) {
					mesh.addTexCoord(face->texCoords[k]);
				}
			}
		}
	}
    
}

template<class GroupClass>
ofRectangle _ofxOBJModel<GroupClass>::getTexCoordBounds() {
	bool started = false;
	ofRectangle r;
	for(int i = 0; i < groups.size(); i++) {
		for(int j = 0; j < groups[i].faces.size(); j++) {
			for(int k = 0; k < groups[i].faces[j].texCoords.size(); k++) {
				if(!started) {
					r.set(groups[i].faces[j].texCoords[k].x,groups[i].faces[j].texCoords[k].y, 0, 0);
					started = true;
				} else {
					r.growToInclude(groups[i].faces[j].texCoords[k]);
				}
				
			}
		}
	}
	return r;
}


template class _ofxOBJModel<ofxOBJGroup>;



