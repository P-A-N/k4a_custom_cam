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

		virtual void setDistCoeffs() = 0;
		void setDrawUndistort(bool val) { bDrawUndistort = val; }

	protected:
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
	};
}
