#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void createFrameBuffer();
		
		GLuint frameBuffer, depthBuffer;
		GLuint colorTexture;

		ofEasyCam cam;
		ofVboMesh mesh;
		ofVboMesh quad;
		ofShader geometryPass, renderPass;
		ofVec3f pos[30];

};