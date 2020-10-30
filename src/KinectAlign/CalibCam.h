#pragma once
#include "ofxCv.h"
namespace KinectAlign
{
	class CalibCam
	{
	public:
		

		void setup(std::string camName, std::string setting)
		{
			this->camName = camName;
			setDistCoeffs();
			ofxCv::Intrinsics intrinsics;
			cv::Point2f sensorSize(17.3, 13);
			cv::Size imageSize(_width, _height);
			intrinsics.setup(getCamMatrix(), imageSize, sensorSize);
			calibration.setFillFrame(false);
			calibration.setIntrinsics(intrinsics);
		}


		void debugDrawImages()
		{
		}

		void drawUndistort(ofPixels& img, int x, int y)
		{
			//if (calibration.size() == 0) return;
			if (!undistort.isAllocated()) ofxCv::imitate(undistort, img);
			calibration.undistort(ofxCv::toCv(img), ofxCv::toCv(undistort));
			undistort.update();
			undistort.draw(x, y);
		}

		cv::Mat getCamMatrix()
		{
			return (cv::Mat1d(3, 3) << _fx, 0, _cx,
										0, _fy, _cy,
										0,0,1);
		}

		bool findImagePoint(ofPixels img, float scale)
		{
			img.resize(img.getWidth() * scale, img.getHeight() * scale);
			cv::Mat camMat = ofxCv::toCv(img);
			bool bFound = calibration.add(camMat);
			if (bFound)
			{
				std::vector<cv::Point2f> points;
				auto _pts = calibration.imagePoints.back();
				for (auto p : _pts)
				{
					points.push_back(cv::Point2f(p.x / scale, p.y / scale));
				}
				imagePoints.push_back(points);
				calibration.imagePoints = imagePoints;
				calibration.addedImageSize = cv::Size(img.getWidth() / scale, img.getHeight() / scale);
			}
			return bFound;
		}

		bool updateCalib(ofPixels& img)
		{
			bool added = false;
			if (bCalib)
			{

				ofLog() << "do calibrate";
				if (findImagePoint(img, 0.25)) {
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

		void drawCalib(int x, int y)
		{
			ofPushStyle();
			ofScale(0.5, 0.5);
			if (bDrawUndistort)
			{
				//if (inputs.size() > 0)
				//	drawUndistort(inputs[0]->getPixels(), 0, 0);
				drawUndistort(calibPixels, 0, 0);
			}
			else
			{
				//if (inputs.size() > 0)
				//	inputs[0]->draw(x, y);
				calibTexture.draw(x, y);
			}
			ofPopStyle();
		}


		virtual void setDistCoeffs() = 0;
		void setDrawUndistort(bool val) { bDrawUndistort = val; }

	protected:
		vector< std::vector<cv::Point2f>> imagePoints;
		ofPixels calibPixels;
		ofTexture calibTexture;
		bool bDrawUndistort = false;
		std::string camName;
		ofxCv::Calibration calibration;
		std::vector<std::shared_ptr<ofBaseHasPixels>> imgs;
		ofImage undistort;
		float _fx, _fy, _cx, _cy;
		int _width, _height;
		float _sensorSizeX, _sensorSizeY;
		float _k1, _k2, _k3, _k4, _k5, _k6; //radial distortion coefficients
		float _p1, _p2;//tangental distotion coefficients	
		bool bCalib = false;
		const int startCleaning = 10; // start cleaning outliers after this many samples
	};
}
