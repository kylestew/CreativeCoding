#include "ofApp.h"

void ofApp::setup() {
}

void ofApp::draw() {
    ofBackground(30);
    ofSetColor(255);
    
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    
    int resolution = 1000;
    int radius = 100;
    ofPolyline circle;
    
    for (int i = 0; i < resolution; i++) {
        float angle = TWO_PI / resolution * i;
        float x = cos(angle) * radius;
        float y = sin(angle) * radius;
        
        circle.addVertex(ofPoint(x, y));
    }
    
    circle.close();
    circle.draw();
}
