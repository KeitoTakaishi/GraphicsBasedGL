#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetVerticalSync(true);
	gui.setup();
	gui.add(pNear.set("Near", 0.3, 0.0, 100.0));
	gui.add(pFar.set("Far", 500, 0.0, 1000.0));
	gui.add(pFov.set("Fov", 60, 0.0, 90.0));

	gui.add(x.set("x", 0, 0.0, 1000.0));
	gui.add(y.set("y", 0, 0.0, 1000.0));
	gui.add(z.set("z", 0, 0.0, 1000.0));
	gui.add(lx.set("lx", planeSize, -1000, 1000.0));
	gui.add(ly.set("ly", planeSize * 0.5, -1000, 1000.0));
	gui.add(lz.set("lz", planeSize, -1000, 1000.0));
	ofDisableArbTex();
	ofLoadImage(texture, "oF.png");
	cout << "texture ID : " << texture.getTextureData().textureID << endl;
	shader.load("shaders/shader");
	//------------------------------------------------------
	//camera
	mu = new MatrixUtils();
	//eyePos = glm::vec3(planeSize, planeSize * 0.5, planeSize);
	eyePos = glm::vec3(0, 0.0, 400.0);
	eyeTarget = ofVec3f(0.0, 0.0, 0.0);
	eyeUpVec = ofVec3f(0.0, 1.0, 0.0);
	aspect = float(ofGetWidth()) / float(ofGetHeight());


	/*cam.setPosition(eyePos);
	cam.lookAt(eyeTarget, eyeUpVec);
	cam.setAspectRatio(aspect);
	cam.setupPerspective(true, 60, pNear, pFar);*/

	vm = mu->lookAt(eyePos, eyeUpVec, eyeTarget); //viewMatrix
	pm = mu->perspective(cam.getFov(), aspect, pNear, pFar); //proj matrix


	

	//------------------------------------------------------
	//lightsetting
	lightPos = glm::vec3(planeSize, planeSize * 0.5, planeSize);
	tm = mu->textureMatrix();
	/*cout << "---texture matrix---" << endl;
	cout << tm << endl;
	cout << "---texture view  matrix---" << endl;
	*/
	lightRight = glm::vec3(-1.0, 0.0, 1.0);
	lightTarget = glm::vec3(0.0, 0.0, 0.0) - lightPos;
	lightUpVec = glm::normalize(glm::cross(lightTarget, lightRight));
	tvm = mu->lookAt(lightPos, lightUpVec, glm::vec3(0.0, 0.0, 0.0));	//view matrix from light
	/*cout << "---texture proj matrix---" << endl;
	cout << "fov :" << fov << endl;
	cout << "aspect : " << aspect << endl;
	cout << "Near : " << pNear << endl;
	cout << "Far : " << pFar << endl;
	cout << tvm << endl;*/
	tpm = mu->perspective(fov, aspect, pNear, pFar); // proj matrix from light
	//cout << tpm << endl;

	//objects-------------------------------------------------------------------------------------
	{
		for (int i = 0; i < 3; i++) {
			plane[i] = ofPlanePrimitive(planeSize, planeSize, 20, 20).getMesh();
		}
		for (int i = 0; i < 5; i++) {
			if (i < 5) {
				sphere[i] = ofSpherePrimitive(ofRandom(80, 300), 16).getMesh();
				spherePos[i] = glm::vec3(ofRandom(-planeSize * 0.3, planeSize * 0.3), ofRandom(planeSize * 0.7) + 50.0, ofRandom(-planeSize * 0.3, planeSize * 0.3));
			}
		}
		light = ofSpherePrimitive(30, 16).getMesh();
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	lightTarget = glm::vec3(0.0, 0.0, 0.0) - lightPos;
	lightUpVec = glm::normalize(glm::cross(lightTarget, lightRight));
	lightPos = glm::vec3(lx, ly, lz);
	tvm = mu->lookAt(lightPos, lightUpVec, glm::vec3(0.0 - planeSize / 2.0, 0.0, 0.0 - planeSize / 2.0));	//view matrix from light

	
	fov = pFov; 
	tpm = mu->perspective(fov, aspect, pNear, pFar); // proj matrix from light
	tpvm = tm * tpm;
	tpvm = tpvm * tvm;

	//tpvm = tpm * tm;
	//tpvm =tvm * tpvm;
}

//--------------------------------------------------------------
void ofApp::draw(){
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	shader.begin();
	
	cam.begin();
	//cam.setPosition(ofVec3f(0.0, 300.0, 700));
	//cam.lookAt(ofVec3f(0.0, 0.0, 0.0), ofVec3f(0.0, 1.0, 0.0));
	//cam.setAspectRatio(aspect);
	//cam.setupPerspective(true, 60, pNear, pFar);
	vm = ofGetCurrentViewMatrix();
	//vm = mu->lookAt(eyePos, eyeUpVec, eyeTarget);
	pm = cam.getProjectionMatrix();
	cam.end();
	
	{
		//plane-bottom
		glm::mat4 _mm;
		_mm = glm::translate(glm::mat4(), glm::vec3(x, y, z));
		_mm *= glm::rotate<float>(glm::mat4(), glm::radians(90.0), glm::vec3(1.0, 0.0, 0.0));
		shader.setUniformMatrix4f("m", _mm);
		plane[0].draw();
		plane[0].drawWireframe();
	}
	

	{
		//plane//front
		glm::mat4 _mm;
		_mm = glm::translate(glm::mat4(), glm::vec3(x, y + planeSize / 2.0, z - planeSize / 2.0));
		shader.setUniformMatrix4f("m", _mm);
		plane[1].draw();
	}

	{
		//plane//side
		glm::mat4 _mm;
		_mm = glm::rotate<float>(glm::mat4(), glm::radians(90.0), glm::vec3(0.0, 1.0, 0.0));
		_mm *= glm::translate(glm::mat4(), glm::vec3(x , y + planeSize / 2.0, z - planeSize / 2.0));
		shader.setUniformMatrix4f("m", _mm);
		plane[2].draw();
		//plane[2].drawWireframe();
	}

	for(int i = 0; i < 5; i++)
	{
		glm::mat4 _mm;
		_mm = glm::translate(glm::mat4(), spherePos[i]);
		shader.setUniformMatrix4f("m", _mm);
		sphere[i].draw();
		//sphere[i].drawWireframe();
	}
		


	shader.setUniformMatrix4f("v", vm);
	shader.setUniformMatrix4f("p", pm);
	shader.setUniformMatrix4f("tMatrix", tm);
	shader.setUniformMatrix4f("tvMatrix", tvm);
	shader.setUniformMatrix4f("tpMatrix", tpm);
	shader.setUniformMatrix4f("tpvMatrix", tpvm);
	shader.setUniformMatrix4f("invLight", glm::inverse(lightm));
	shader.setUniform3f("lightPos", lightPos);
	shader.setUniformTexture("t", texture, texture.getTextureData().textureID);


	float time = ofGetElapsedTimef();
	shader.setUniform1f("time", time);
	
	shader.end();


	cam.begin();
	lightTarget = glm::vec3(0.0 - planeSize / 2.0, 0.0, 0.0 - planeSize / 2.0) - lightPos;
	lightUpVec = glm::normalize(glm::cross(lightTarget, lightRight));
	
	ofPushStyle();
	ofSetColor(0, 0, 255);
	ofDrawLine(lightPos, lightPos + lightTarget);
	ofSetColor(255, 0.0, 0.0);
	ofDrawLine(lightPos, lightPos + lightRight * 100.0);
	ofSetColor(0, 255, 0.0);
	ofDrawLine(lightPos, lightPos + lightUpVec * 100.0);
	ofPopStyle();


	//light
	ofPushMatrix();
	ofTranslate(ofVec3f(lx, ly, lz));
	light.draw();
	ofPopMatrix();
	cam.end();

	glDisable(GL_DEPTH);
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		shader.load("shaders/shader");
	}
}

