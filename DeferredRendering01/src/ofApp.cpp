#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	createFrameBuffer();
	//mesh = ofSpherePrimitive(50, 16).getMesh();
	plane = ofPlanePrimitive(1000, 1000, 20, 20).getMesh();
	model3d.loadModel(path);
	mesh = model3d.getMesh(0);
	glEnable(GL_DEPTH_TEST);
	geometryPass.load("shaders/geom");
	renderPass.load("shaders/render");
	ofDisableArbTex();
	ofLoadImage(albedoTexture, "image/concrete.jpg");

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


	float r =50;
	float c = 1.0;
	for (int i = 0; i < NUM; i++) {
		pos[i] = ofVec3f(ofRandom(-r, r), ofRandom(-30, 100), ofRandom(-r, r));
		light[i].setPosition(pos[i]);
		light[i].setColor(ofRandom(c), ofRandom(c), ofRandom(c));
		cout << light[i].getColor() << endl;
		omega[i] = ofRandom(1.0, 3.0);
	}

	gui.setup();
	gui.add(renderMode.set("renderMode", 0, 0, 3));
	gui.add(disCoef.set("Distance Coefficient", 0.2, 0.0, 1.0));
	gui.add(lightAttenuation.set("Attenuation", 0.005, 0.0, 0.1));
	gui.add(drawLight.setup("Debug", true));
}

//--------------------------------------------------------------
void ofApp::update() {
	for (int i = 0; i < NUM; i++) {
		ofVec3f p = pos[i];
		float l = sqrt(pow(pos[i].x, 2.0) + pow(pos[i].z, 2.0));
		float t = ofGetElapsedTimef();
		float theta = ofDegToRad(t * omega[i]) * 20.0;
		pos[i] = ofVec3f(l * cos(theta), p.y, l*sin(theta));
		light[i].setPosition(pos[i]);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	geometryPass.begin();
	cam.begin();
	ofMatrix4x4 proj, view;
	view = ofGetCurrentViewMatrix();
	proj = cam.getProjectionMatrix();
	cam.end();

	if (drawLight) {
		for (int i = 0; i < NUM; i++) {
			ofMatrix4x4 model;
			model.translate(pos[i]);
			geometryPass.setUniformMatrix4f("model", model);
			light[i].draw();
		}
	}
	 {
		ofMatrix4x4 model;
		geometryPass.setUniformMatrix4f("model", model);
		mesh.draw();
	}
	{
		ofMatrix4x4 model;
		model.rotate(-90, 1, 0, 0);
		model.translate(0, -50, 0);
		geometryPass.setUniformMatrix4f("model", model);
		plane.draw();
	}
	geometryPass.setUniformMatrix4f("view", view);
	geometryPass.setUniformMatrix4f("proj", proj);
	geometryPass.setUniformTexture("albedoTexture", albedoTexture, albedoTexture.getTextureData().textureID);

	geometryPass.end();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//-----------------------------------------
	//Second Pass 
	renderPass.begin();
	for (int i = 0; i < NUM; i++) {
		ofMatrix4x4 modelMatrix;
		ofVec3f lightPos = light[i].getPosition() * view;
		renderPass.setUniform3fv("light[" + to_string(i) + "].position", &lightPos[0], 1);
		renderPass.setUniform3fv("light[" + to_string(i) + "].color", &light[i].getColor()[0], 1);
		renderPass.setUniform3fv("light[" + to_string(i) + "].specular", &light[i].getSpecular()[0], 1);
	}
	renderPass.setUniformTexture("gPosition", GL_TEXTURE_2D, gPosition, 0);
	renderPass.setUniformTexture("gNormal", GL_TEXTURE_2D, gNormal, 1);
	renderPass.setUniformTexture("gAlbedo", GL_TEXTURE_2D, gAlbedo, 2);
	renderPass.setUniform1i("renderMode", renderMode);
	renderPass.setUniform1f("disCoef", disCoef);
	renderPass.setUniform1f("lightAttenuation", lightAttenuation);
	renderPass.setUniform3f("viewPos", cam.getPosition());


	quad.draw(OF_MESH_FILL);
	renderPass.end();
	//-----------------------------------------
	glDisable(GL_DEPTH_TEST);
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == ' ') {
		geometryPass.load("shaders/geom");
		renderPass.load("shaders/render");
	}
}

//--------------------------------------------------------------
void ofApp::createFrameBuffer() {
	
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	ofVec2f res = ofVec2f(ofGetWidth(), ofGetHeight());


	//-----------------------------------
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, res.x, res.y,
		0, GL_RGB, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
	//-----------------------------------
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, res.x, res.y,
		0, GL_RGB, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	//-----------------------------------
	glGenTextures(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, res.x, res.y,
		0, GL_RGBA, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);
	//-----------------------------------
	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);


	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	//glTexImage2D�I�ȗ����ʒu
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, res.x, res.y);
	//attach
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}