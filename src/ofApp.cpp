#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	rgbCam.setup();
	kinect.setup();
	flow.setup();
}

//--------------------------------------------------------------
void ofApp::exit()
{}

//--------------------------------------------------------------
void ofApp::update()
{
	//flow.updateCalib(azure_kinects[0]->getImage < ofx::AzureKinectDK::Image::Color>());
	rgbCam.update();
	kinect.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackground(0);
	//rgbCam.setDrawUndistort(ofGetKeyPressed());
	//rgbCam.draw(0, 0);
	kinect.drawCalib(0,0);
	std::ostringstream oss;
	ofDrawBitmapStringHighlight(std::to_string(ofGetFrameRate()), 10, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//if (key == 'r') {
	//	//device->startRecording("output5.mkv");
	//}

	//if (key == 's') {
	//	//device->stopRecording();
	//}

	//if (key == ' ') {
	//	playback->setPosition(0.0);
	//}
	if (key == 'c')
	{
		rgbCam.toggleCalib();
//		rgbCam.saveImage();
	}

	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
	
}
