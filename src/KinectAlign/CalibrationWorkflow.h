#pragma once
#include "ofxCv.h"
#include "KinectAlign\CalibCam.h"
namespace KinectAlign
{
	class CalibrationWorkflow
	{
	public:
		void setup()
		{
			
		}

		void updateCalib1()
		{

		}

		void alignCameras()
		{

		}


	private:
		shared_ptr<CalibCam> camera1Carib, camera2Calib;

	};
}