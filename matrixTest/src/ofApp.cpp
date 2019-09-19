#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	mesh = ofSpherePrimitive(50, 16).getMesh();
	shader.load("shaders/shader");
	gm = glm::translate(glm::mat4(), glm::vec3(100.0f, 0.0f, 0.0f));
	myview = createViewMatrix(vec3(0.0, 0.0, 150), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 0.0));
}
//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	shader.begin();
	cam.begin();
	cam.setPosition(ofVec3f(0.0, 0.0, -150.0));
	cam.lookAt(ofVec3f(0.0, 0.0, 0.0), ofVec3f(0.0, 1.0, 0.0));
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

	mesh.drawWireframe();
	shader.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		shader.load("shaders/shader");
	}
}

//--------------------------------------------------------------
mat4 ofApp::createViewMatrix(vec3 e, vec3 u, vec3 t) {
	mat4 dest;
	vec3 eye = e;
	vec3 up = u;
	vec3 target = t;
	float x0, x1, x2, y0, y1, y2, z0, z1, z2;
	z0 = eye[0] - target[0];
	z1 = eye[1] - target[1];
	z2 = eye[2] - target[2];
	float l = 1.0 / sqrtf( pow(z0, 2) + pow(z1, 2) + pow(z2, 2) );
	z0 *= l;
	z1 *= l;
	z2 *= l;

	x0 = up[1] * z2 - up[2] * z1;
	x1 = up[2] * z0 - up[0] * z2;
	x2 = up[0] * z1 - up[1] * z0;
	l = 1.0 / sqrtf(pow(x0, 2) + pow(x1, 2) + pow(x2, 2));
	x0 *= l;
	x1 *= l;
	x2 *= l;

	y0 = z1 * x2 - z2 * x1;
	y1 = z2 * x0 - z0 * x2;
	y2 = z0 * x1 - z1 * x0;
	l = 1.0 / sqrtf(pow(y0, 2) + pow(y1, 2) + pow(y2, 2));
	y0 *= l;
	y1 *= l;
	y2 *= l;

	vec4 x = glm::vec4(x0, x1, x2, 0);
	vec4 y = glm::vec4(y0, y1, y2, 0);
	vec4 z = glm::vec4(z0, z1, z2, 0);
	vec4 w = glm::vec4( -(x0 * eye[0] + x1 * eye[1] + x2 * eye[2]),
									-(y0 * eye[0] + y1 * eye[1] + y2 * eye[2]), 
									-(z0 * eye[0] + z1 * eye[1] + z2 * eye[2]),
									1.0);
	dest = glm::mat4(x, y, z, w);
	
	return dest;
}
