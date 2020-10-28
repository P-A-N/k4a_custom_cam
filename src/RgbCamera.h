#pragma once
//#include "ofxBlackMagic.h"
#include "KinectAlign/CalibCam.h"

class RgbCamera : public KinectAlign::CalibCam
{
public:
	RgbCamera()
	{
		_width = 3840; _height = 2160;
		_fx = _width * 0.482949;
		_fy = _width * 0.482949;
		_cx = _width * 0.506560;
		_cy = _height * 0.482734;
		_k1 = 0.002810; _k2 = 0.000903; _k3 = -0.001258; _p1 = -0.003365; _p2 = 0.003547;
	}

	void setup()
	{
		//auto deviceList = ofxBlackmagic::Iterator::getDeviceList();
		//for (auto device : deviceList) {
		//	auto input = shared_ptr<ofxBlackmagic::Input>(new ofxBlackmagic::Input());

		//	static int index = 0;
		//	auto mode = bmdMode4K2160p2997; // switch this mode to match the resolution/refresh of your input stream
		//	input->startCapture(device, mode);
		//	this->inputs.push_back(input);
		//}
		this->camName = "rgb";
		cv::FileStorage settings(ofToDataPath("calib/settings.yml"), cv::FileStorage::READ);
		if (settings.isOpened()) {
			int xCount = settings["xCount"], yCount = settings["yCount"];
			ofLog() << xCount;
			ofLog() << yCount;
			calibration.setPatternSize(xCount, yCount);
			float squareSize = settings["squareSize"];
			calibration.setSquareSize(squareSize);
			ofxCv::CalibrationPattern patternType = ofxCv::CHESSBOARD;
			switch (settings["patternType"]) {
			case 0: patternType = ofxCv::CHESSBOARD; break;
			case 1: patternType = ofxCv::CIRCLES_GRID; break;
			case 2: patternType = ofxCv::ASYMMETRIC_CIRCLES_GRID; break;
			}
			calibration.setPatternType(patternType);
		}
		//calibration.load("calib/rgb_calibration.yml");
	}


	bool updateCalib(ofPixels& img)
	{
		bool added = false;
		if (bCalib)
		{
			ofLog() << "do calibrate";
			//img.resize(1920, 1080);
			cv::Mat camMat = ofxCv::toCv(img);
			float curTime = ofGetElapsedTimef();

			if (added = calibration.add(camMat)) {
				ofLog() << "re-calibrating " << calibration.size();
				calibration.calibrate();
				if (calibration.size() > startCleaning) {
					calibration.clean();
				}
				calibration.save("calib/" + camName + "_calibration.yml");
			}
			else
			{
				ofLog() << "not found";
			}
			bCalib = false;
		}
		return added;
	}

	void update()
	{
		//inputs[0]->update();
		//updateCalib(inputs[0]->getPixels());

	}

	void draw(int x, int y)
	{
		ofPushStyle();
		ofScale(0.5, 0.5);
		//if (bDrawUndistort)
		//{
		//	if (inputs.size() > 0)
		//		drawUndistort(inputs[0]->getPixels(), 0, 0);
		//}
		//else
		//{
		//	if (inputs.size() > 0)
		//		inputs[0]->draw(x, y);
		//}
		ofPopStyle();
	}

	virtual void setDistCoeffs() override
	{
		calibration.setDistortionCoefficients(_k1, _k2, _p1, _p2, _k3);
	}


	void saveImage()
	{
		ofImage img;
		//img.setFromPixels(inputs[0]->getPixels());
		img.update();
		img.save("corner" + ofToString(ofGetFrameNum()) + ".jpg");
	}


	void toggleCalib()
	{
		bCalib = !bCalib;
	}


	void startCalib()
	{
		bCalib = true;
	}

	void stopCalib()
	{
		bCalib = false;
	}

private:
	//vector<shared_ptr<ofxBlackmagic::Input> > inputs;
	const int startCleaning = 10; // start cleaning outliers after this many samples
	bool bCalib = false;
};