#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//ofSetVerticalSync(false);
	//ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	//fbo
	//createFrameBuffer();
	CreateFbo();
	initQuad();
	mu = new ofxMatrixUtils();
	glEnable(GL_DEPTH_TEST);
	//ofDisableArbTex();
	//ofLoadImage(texture, "oF.png");
	//cout << "texture ID : " << texture.getTextureData().textureID << endl;
	//----------------------------------------------
	gui.setup();
	gui.add(gFov.set("Fov", 90.0, 0.0, 90.0));
	gui.add(gAspect.set("Aspect", (float)ofGetWidth()/(float)ofGetHeight(), 1.0, 1.5));
	gui.add(gNear.set("Near", 0.01, 0.0, 10.0));
;	gui.add(gFar.set("Far", 550.0, 10.0, 10000.0));
	gui.add(lightPos.setup("lightPos", ofVec3f(0.0, 0.0, 300.0), ofVec3f(-1000.0, -1000.0, -1000.0), ofVec3f(1000.0, 1000.0, 1000.0)));
	gui.add(debug.set("debug", 2, 0, 2));
	gui.add(depthMode.set("depthMode", 0, 0, 1));

	//----------------------------------------------
	float theta = 360.0 / boxNum;
	float radius = 70.0;
	for (int i = 0; i < boxNum; i++) {
		spherePos[i] = glm::vec3(cos(ofDegToRad(theta * i))*radius , sin(ofDegToRad(theta * i))*radius, 50);
		//sphere[i] = ofSpherePrimitive(16, 16).getMesh();
		float boxSize = ofRandom(10.0, 30.0);
		sphere[i] = ofBoxPrimitive(boxSize, boxSize, boxSize).getMesh();
	}
	plane = ofPlanePrimitive(planeSize, planeSize, 2, 2).getMesh();
	light = ofBoxPrimitive(10, 10, 10).getMesh();
	//----------------------------------------------------------
	shader.load("shader"); 
	renderPass.load( "renderPass");
	test.load("test");
	cam.setPosition(ofVec3f(0.0, 0.0, 200.0));
	cam.lookAt(ofVec3f(0.0, 0.0, 0.0), ofVec3f(0.0, 1.0, 0.0) );

	scale = glm::scale(scale, glm::vec3(ofGetWidth(), ofGetHeight(), 1.0f));
	cout << scale << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
	cam.setFarClip(1500.);
	textureMatrix = mu->textureMatrix();
	vlMatrix = mu->lookAt(lightPos, glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 0.0));
	plMatrix = mu->perspective(gFov, gAspect, gNear,gFar);
	mlight = glm::translate(glm::mat4(), glm::vec3(lightPos));
	
	pvMatrix = plMatrix * vlMatrix;
	tpvMatrix = textureMatrix * plMatrix ;
	tpvMatrix = tpvMatrix * vlMatrix;
}

//--------------------------------------------------------------
void ofApp::draw(){
	//glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	fbo.begin();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//glEnable(GL_CULL_FACE);

	
	shader.begin();
	//glCullFace(GL_FRONT);
	mplane = glm::rotate<float>(glm::mat4(), glm::radians(180.0), glm::vec3(1.0, 0.0, 0.0));
	mplane *= glm::translate(glm::mat4(), glm::vec3(0, 0, 0.0));
	shader.setUniformMatrix4f("m", mplane);
	plane.draw();

	float time = ofGetElapsedTimef() * 10.0f;
	for (int i = 0; i < boxNum; i++) {
		msphere = glm::translate(glm::mat4(), spherePos[i] );
		msphere *= glm::rotate<float>(glm::mat4(), glm::radians(time), glm::vec3(1.0, 1.0, 1.0));
		shader.setUniformMatrix4f("m", msphere);
		sphere[i].draw();
	}
	shader.setUniformMatrix4f("vl", vlMatrix);// view light
	shader.setUniformMatrix4f("pl", plMatrix);//proj light
	shader.setUniformMatrix4f("pv", pvMatrix);//proj * view (from light)
	shader.setUniform1f("near", gNear);
	shader.setUniform1f("far", gFar);
	shader.setUniform1i("mode", debug);
	shader.setUniform1i("depthMode", depthMode);
	shader.setUniformMatrix4f("inv", glm::inverse(mlight));

	shader.end();
	//glCullFace(GL_BACK);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	fbo.end();
	
	//-------------------------------------------------------------------------------------------------------
	if (debug == 2) {
		//glBindTexture(GL_TEXTURE_2D, colorBuffer);
		glClearColor(0.0f, 0.7f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearDepth(1.0);
		renderPass.begin();
		cam.begin();
		v = ofGetCurrentViewMatrix();
		p = cam.getProjectionMatrix();
		cam.end();

		mplane = glm::rotate<float>(glm::mat4(), glm::radians(180.0), glm::vec3(1.0, 0.0, 0.0));
		mplane *= glm::translate(glm::mat4(), glm::vec3(0, 0, 0));
		renderPass.setUniformMatrix4f("m", mplane);
		plane.draw();


		for (int i = 0; i < boxNum; i++) {
			msphere = glm::translate(glm::mat4(), spherePos[i]);
			msphere *= glm::rotate<float>(glm::mat4(), glm::radians(time), glm::vec3(1.0, 1.0, 1.0));
			renderPass.setUniformMatrix4f("m", msphere);
			sphere[i].draw();
		}
		renderPass.setUniformMatrix4f("v", v);
		renderPass.setUniformMatrix4f("p", p);

		renderPass.setUniformMatrix4f("vl", vlMatrix);// view light
		renderPass.setUniformMatrix4f("pl", plMatrix);//proj light
		renderPass.setUniformMatrix4f("tMatrix", textureMatrix);
		renderPass.setUniformMatrix4f("tpv", tpvMatrix);
		
		renderPass.setUniform3f("lightPos", lightPos);
		renderPass.setUniform1f("near", gNear);
		renderPass.setUniform1f("far", gFar);
		renderPass.setUniform1i("depthMode", depthMode);
		renderPass.setUniformMatrix4f("inv", glm::inverse(mlight));
		renderPass.setUniformMatrix4f("scaleMat", scale);

		//renderPass.setUniformTexture("colorBuffer", GL_TEXTURE_2D, frameBuffer, 0);
		//renderPass.setUniform1i("colorBuffer", 0);
		
		ofTexture tex;
		tex = fbo.getTexture();
		//tex.setTextureWrap(GL_CLAMP_TO_BORDER_ARB, GL_CLAMP_TO_BORDER_ARB);
		renderPass.setUniformTexture("colorBuffer", tex, tex.getTextureData().textureID);
		renderPass.end();

		
		cam.begin();
		ofDrawAxis(10000);
		ofPushMatrix();
		ofPushStyle();
		ofSetColor(255, 200.0, 0.0);
		ofTranslate(lightPos);
		light.draw();
		ofPopStyle();
		ofPopMatrix();
		cam.end();
		
		
		//glBindTexture(GL_TEXTURE_2D, 0);
		

	}
	else if(debug == 0){
		test.begin();
		//test.setUniformTexture("tex", GL_TEXTURE_2D, colorBuffer, 0);
		test.setUniformTexture("tex", fbo.getTexture(), fbo.getTexture().getTextureData().textureID);
		//shader.setUniformTexture("tex", texture, texture.getTextureData().textureID);
		quad.draw(OF_MESH_FILL);
		test.end();
	}
	else {
		fbo.draw(0.0, 0.0);
	}
	
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	gui.draw();	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	shader.load("shader");
	renderPass.load("renderPass");
	test.load("test");
}

//--------------------------------------------------------------
void ofApp::createFrameBuffer() {
	
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	ofVec2f res = ofVec2f(ofGetWidth(), ofGetHeight());

	//renderBuffer
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, res.x, res.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, res.x, res.y,
		0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
	//GLuint attachment = GL_COLOR_ATTACHMENT0;
	//glDrawBuffers(1, &attachment);


	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

void ofApp::CreateFbo() {
	glEnable(GL_DEPTH_TEST);
	ofEnableDepthTest();
	
	ofFbo::Settings settings;
	settings.textureTarget = GL_TEXTURE_2D;
	settings.width = ofGetWidth();
	settings.height = ofGetHeight();
	settings.internalformat = GL_RGBA;
	settings.useDepth = true;
	settings.useStencil = false;
	settings.depthStencilAsTexture = false;
	settings.depthStencilInternalFormat = GL_DEPTH_COMPONENT16; // depth
	settings.maxFilter = GL_LINEAR;
	settings.minFilter = GL_LINEAR;
	settings.wrapModeVertical = GL_CLAMP_TO_EDGE;
	settings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
	//settings.numSamples = 64;
	fbo.allocate(settings);
	fbo.createAndAttachTexture(GL_RGBA, 1); // position


	cout << "fbo texture : " << fbo.getTexture().getTextureData().textureID << endl;
	
	/*
	float length = 4096;
	ofFbo::Settings settings;
	settings.width = length;
	settings.height = length;
	settings.useDepth = true;
	settings.useStencil = true;
	settings.depthStencilAsTexture = true;
	fbo.allocate(settings);
	*/
}

void ofApp::initQuad() {
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
}