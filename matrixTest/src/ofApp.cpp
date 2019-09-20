#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	mesh = ofSpherePrimitive(50, 16).getMesh();
	shader.load("shaders/shader");
	gm = glm::translate(glm::mat4(), glm::vec3(100.0f, 0.0f, 0.0f));
	myview = MatrixUtils::lookAt(vec3(0.0, 0.0, 150), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 0.0));
	
	
	myproj = MatrixUtils::perspective(90.0, 1.0 , 0.3, 1000);

	float aspect = (float)ofGetWidth() / (float)ofGetHeight();
	cam.setupPerspective(true, 90, aspect, 1000);
	cam.setAspectRatio(1.0);


	gui.setup();
	gui.add(pFov.set("Fov", 60, 0.0, 180.0));
	gui.add(pAspect.set("Aspect", aspect, 0.0, 2.0));
	gui.add(pNear.set("Near", 0.3, 0.0, 20.0));
	gui.add(pFar.set("Fov", 700, 0.0, 1000.0));

	ofBackground(0);
	glEnable(GL_DEPTH_TEST);
}
//--------------------------------------------------------------
void ofApp::update(){
	myproj = MatrixUtils::perspective(pFov, pAspect, pNear, pFar);
}

//--------------------------------------------------------------
void ofApp::draw(){
	glEnable(GL_DEPTH_TEST);
	shader.begin();
	cam.begin();
	cam.setPosition(ofVec3f(0.0, 0.0, -150.0));
	cam.lookAt(ofVec3f(0.0, 0.0, 0.0), ofVec3f(0.0, 1.0, 0.0));
	/*cam.setupPerspective(true, 90, 0.3, 1000);
	cam.setAspectRatio(1.0);
	cout << cam.getProjectionMatrix() << endl;*/
	v = ofGetCurrentViewMatrix();
	gv = ofGetCurrentViewMatrix();
	gmv = cam.getModelViewMatrix();	
	p = cam.getProjectionMatrix();
	gp = cam.getProjectionMatrix();

	gmvp = cam.getModelViewProjectionMatrix();
	
	cam.end();
	shader.setUniformMatrix4f("gm", gm );
	shader.setUniformMatrix4f("gv",gv);
	shader.setUniformMatrix4f("gp", gp);

	shader.setUniformMatrix4f("gvm", gmv);
	shader.setUniformMatrix4f("m", m);
	shader.setUniformMatrix4f("v", v);
	shader.setUniformMatrix4f("p", p);
	shader.setUniformMatrix4f("gmv", gm * gv);
	shader.setUniformMatrix4f("gmvp", gmvp);
	shader.setUniformMatrix4f("mv", m * v);
	shader.setUniformMatrix4f("view", myview);
	shader.setUniformMatrix4f("proj", myproj);


	mesh.drawWireframe();
	shader.end();


	glDisable(GL_DEPTH_TEST);
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		shader.load("shaders/shader");
	}
}

//--------------------------------------------------------------
