#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"

#define NUM 40

class PointLight {
public:
	PointLight() {
		sphere = ofSpherePrimitive(10, 8);
		velocity = ofVec3f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
		specular = ofVec3f(1.0, 1.0, 1.0);
	}

	// color
	void setColor(float r, float g, float b) {
		color = ofVec3f(r, g, b);
	}
	void setColor(float _color) {
		color = ofVec3f(_color, _color, _color);
	}
	void setGray(float _gray) {
		gray = _gray;
	}
	void setColor(ofVec3f _color) {
		color = _color;
	}
	ofVec3f getColor() const { return color; }
	float getGray() const { return gray; }

	void setSpecular(float r, float g, float b, float a = 1.0f)
	{
		specular = ofVec3f(r, g, b);
	}
	ofVec3f getSpecular() const
	{
		return specular;
	}

	// position
	void setPosition(float x, float y, float z) {
		position = ofVec3f(x, y, z);
	}
	void setPosition(ofVec3f _position) {
		position = _position;
	}
	ofVec3f getPosition() const { return position; }

	void setVelocity(float x, float y, float z) {
		velocity = ofVec3f(x, y, z);
	}
	ofVec3f getVelocity() const { return velocity; }

	void setRadius(float _radius) {
		radius = _radius;
	}
	float getRadius() const { return radius; }

	void draw() {
		//ofSetColor(color.x, color.y, color.z);
		sphere.draw();
	}
private:
	ofVec3f position;
	ofVec3f color;
	ofVec3f specular;
	float gray;
	ofVec3f velocity;
	float radius;

	ofSpherePrimitive sphere;
};



class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void createFrameBuffer();

		GLuint gBuffer, depthBuffer;
		GLuint gPosition, gNormal, gAlbedo;
		ofTexture albedoTexture;

		ofEasyCam cam;
		ofVboMesh vboMesh;
		ofVboMesh quad, plane;
		ofMesh mesh;

		ofShader geometryPass, renderPass;
		PointLight light[NUM];
		ofVec3f pos[NUM];
		float omega[NUM];

		ofxPanel gui;
		ofParameter<int> renderMode;
		ofParameter<float> disCoef;
		ofParameter<float> lightAttenuation;
		ofxToggle drawLight;
		ofxAssimpModelLoader model3d;
		string path = "lucy.obj";
};