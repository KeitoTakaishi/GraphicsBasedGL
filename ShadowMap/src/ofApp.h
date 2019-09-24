#pragma once

#include "ofMain.h"
#include "ofxMatrixUtils.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void createFrameBuffer();	
		void CreateFbo();
		void initQuad();
			
		glm::mat4 m, v, p;
		glm::mat4 mplane, mplane2, msphere, mlight;
		glm::mat4 tpvMatrix, pvMatrix;
		glm::mat4 scale;
		glm::vec3 spherePos[10];
		//glm::vec3 lightPos = glm::vec3(0, 500, 0);
		glm::mat4 textureMatrix, vlMatrix, plMatrix;

		float planeSize = 300.0;
		ofVboMesh mesh;
		ofVboMesh plane, plane2 , sphere[10];
		float boxNum = 10;
		ofVboMesh light;
		ofVboMesh quad;

		ofShader shader, renderPass;
		ofShader test;
		ofEasyCam cam;
		ofxMatrixUtils* mu;

		GLuint frameBuffer, depthBuffer;
		GLuint colorBuffer;

		ofxPanel gui;
		ofParameter<float> gFov, gAspect, gNear, gFar;
		ofxVec3Slider lightPos;
		ofParameter<int> debug;
		ofParameter<int> depthMode;
		//ofTexture texture;
		ofFbo fbo;
};
