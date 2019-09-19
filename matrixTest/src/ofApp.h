#pragma once

#include "ofMain.h"

using namespace glm;
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		mat4 createViewMatrix(vec3 e, vec3 u, vec3 t);

		mat4 gm, gv, gp, gmv, gmvp;
		mat4 myview;
		ofMatrix4x4 m, v, p;
		ofVboMesh mesh;
		ofEasyCam cam;
		ofShader shader;
};
