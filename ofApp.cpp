#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofNoFill();
	ofEnableDepthTest();
	ofSetRectMode(ofRectMode::OF_RECTMODE_CENTER);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	ofColor color;
	vector<int> hex_list = { 0xf72585, 0xb5179e, 0x7209b7, 0x480ca8, 0x3f37c9, 0x4cc9f0 };
	for (auto hex : hex_list) {

		color.setHex(hex);
		this->base_color_list.push_back(color);
	}

	for (int x = -200; x <= 200; x += 1) {

		this->base_location_list.push_back(glm::vec3(x, -200, -300));
		this->base_location_list.push_back(glm::vec3(x, -200, 300));
		this->base_location_list.push_back(glm::vec3(x, -200, 0));
		this->base_location_list.push_back(glm::vec3(x, 200, 0));
		this->base_location_list.push_back(glm::vec3(x, 200, -300));
		this->base_location_list.push_back(glm::vec3(x, 200, 300));
	}

	for (int y = -200; y <= 200; y += 1) {

		this->base_location_list.push_back(glm::vec3(-200, y, -300));
		this->base_location_list.push_back(glm::vec3(-200, y, 300));
		this->base_location_list.push_back(glm::vec3(200, y, 0));
		this->base_location_list.push_back(glm::vec3(-200, y, 0));
		this->base_location_list.push_back(glm::vec3(200, y, -300));
		this->base_location_list.push_back(glm::vec3(200, y, 300));
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	int radius = 5;
	while (this->log_list.size() < 3500) {

		int font_location_index = ofRandom(this->base_location_list.size());
		vector<glm::vec3> log;
		log.push_back(this->base_location_list[font_location_index]);
		this->log_list.push_back(log);
		this->color_list.push_back(this->base_color_list[(int)ofRandom(this->base_color_list.size())]);
		this->life_list.push_back(ofRandom(10, 30));
	}

	for (int i = this->log_list.size() - 1; i >= 0; i--) {

		this->life_list[i] -= 1;
		if (this->life_list[i] < 0) {

			this->log_list.erase(this->log_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
			this->life_list.erase(this->life_list.begin() + i);

			continue;
		}

		auto deg = ofMap(ofNoise(glm::vec4(this->log_list[i].back() * 0.0035, ofGetFrameNum() * 0.003)), 0, 1, -360, 360);
		auto theta = ofMap(ofNoise(glm::vec4(this->log_list[i].back() * 0.0035, (ofGetFrameNum() + 10000) * 0.003)), 0, 1, -360, 360);
		this->log_list[i].push_back(this->log_list[i].back() + glm::vec3(radius * cos(deg * DEG_TO_RAD) * sin(theta * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD) * sin(theta * DEG_TO_RAD), radius * cos(theta * DEG_TO_RAD)));
		while (this->log_list[i].size() > 100) {

			this->log_list[i].erase(this->log_list[i].begin());
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(270);
	ofRotateZ(ofGetFrameNum() * 0.5);

	for (int i = 0; i < this->log_list.size(); i++) {

		ofSetColor(this->color_list[i]);

		if (this->life_list[i] > 30) {

			ofSetLineWidth(1.5);
		}
		else {

			ofSetLineWidth(ofMap(this->life_list[i], 0, 30, 0, 1.5));
		}

		ofSetColor(this->color_list[i]);

		ofBeginShape();
		ofVertices(this->log_list[i]);
		ofEndShape();
	}

	ofSetColor(255);
	ofDrawRectangle(glm::vec3(), 400, 400);
	ofDrawRectangle(glm::vec3(0, 0, 300), 400, 400);
	ofDrawRectangle(glm::vec3(0, 0, -300), 400, 400);

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}