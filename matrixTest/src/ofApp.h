#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "../MatrixUtils.h"
using namespace glm;
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);

		mat4 gm, gv, gp, gmv, gmvp;
		mat4 myview, myproj;
		ofMatrix4x4 m, v, p;
		ofVboMesh mesh;
		ofEasyCam cam;
		ofShader shader;


		ofxPanel gui;
		ofParameter<float> pFov;
		ofParameter<float> pAspect;
		ofParameter<float> pNear;
		ofParameter<float> pFar;
		

};
