#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	createFrameBuffer();
	mesh = ofSpherePrimitive(50, 16).getMesh();
	glEnable(GL_DEPTH_TEST);
	geometryPass.load("shaders/geom");
	renderPass.load("shaders/render");

	float w = 1.0;
	float h = 1.0;
	quad.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	quad.addVertex(ofVec3f(w, h, 0.0));
	quad.addTexCoord(ofVec2f(1.0f, 1.0f));
	quad.addVertex(ofVec3f(w, -h, 0.0));
	quad.addTexCoord(ofVec2f(1.0f, 0.0f));
	quad.addVertex(ofVec3f(-w, -h, 0.0));
	quad.addTexCoord(ofVec2f(0.0f, 0.0f));
	quad.addVertex(ofVec3f(-w, h, 0.0));
	quad.addTexCoord(ofVec2f(0.0f, 1.0f));

	cam.lookAt(ofVec3f(0.0, 0.0, 0.0), ofVec3f(0.0, 1.0, 0.0));


	float r = 300;
	for (int i = 0; i < 30; i++) {
		pos[i] = ofVec3f(ofRandom(-r, r), ofRandom(-r, r), ofRandom(-r, r));
	}
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	geometryPass.begin();
	cam.begin();
	ofMatrix4x4 proj, view;
	view = ofGetCurrentViewMatrix();
	proj = cam.getProjectionMatrix();
	cam.end();
	for (int i = 0; i < 30; i++) {
		ofMatrix4x4 model;
		model.translate(pos[i]);
		mesh.draw();
		geometryPass.setUniformMatrix4f("model", model);
	}
	geometryPass.setUniformMatrix4f("view", view);
	geometryPass.setUniformMatrix4f("proj", proj);
	
	geometryPass.end();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	//-----------------------------------------
	renderPass.begin();
	renderPass.setUniformTexture("colorBuffer", GL_TEXTURE_2D, colorTexture, 0);

	quad.draw(OF_MESH_FILL);
	//quad.draw(OF_MESH_WIREFRAME);
	renderPass.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		geometryPass.load("shaders/geom");
		renderPass.load("shaders/render");
	}
}

//--------------------------------------------------------------
void ofApp::createFrameBuffer() {
	//frameBufferを作成
	glGenFramebuffers(1, & frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	ofVec2f res = ofVec2f(ofGetWidth(), ofGetHeight());

	glGenTextures(1, &colorTexture);
	glBindTexture(GL_TEXTURE_2D, colorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, res.x, res.y,
		0, GL_RGB, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//----FrameBufferにtextureをアタッチ(ColorBufferとしてアタッチされているTextureBuffer)
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

	GLuint attachment = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &attachment);

	
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	//glTexImage2D的な立ち位置
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, res.x, res.y);
	//attach
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	
	/*
	glGenTextures(1, &depthBuffer);
	glBindTexture(GL_TEXTURE_2D, depthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, res.x, res.y,
		0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);
	*/
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}