#pragma once

#include "ofMain.h"
#include "../MatrixUtils.h"
#include "ofxGui.h"
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		
		ofShader shader;
		ofVboMesh plane[3], sphere[5], light;
		glm::vec3 spherePos[5];
		float planeSize = 1000.0;
		ofTexture texture;
		glm::mat4 mm, vm, pm;
		glm::mat4 tm; //texture matrix
		glm::mat4 tvm; //texture view
		glm::mat4 tpm; //texture projection
		glm::mat4 tpvm; //texture * view * projection
		glm::mat4 lightm;
		MatrixUtils* mu;


		ofxPanel gui;
		ofParameter<float> pNear, pFar, pFov;
		ofParameter<float> x, y ,z;
		ofParameter<float> lx, ly, lz;

		ofEasyCam cam;
		float aspect;
		float fov;
		glm::vec3 eyePos, eyeUpVec, eyeTarget;
		glm::vec3 lightPos, lightUpVec, lightTarget, lightRight;
		
};
