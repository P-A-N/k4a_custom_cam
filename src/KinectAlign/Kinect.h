#pragma once
#include "ofxCv.h"
#include "ofxAzureKinectDK.h"
#include "KinectAlign/CalibCam.h"
namespace KinectAlign
{
	class Kinect : public CalibCam
	{
	public:
		Kinect()
		{
			_fx=918.356934; _cx=956.913147; _fy=917.901733; _cy=553.601135;
			_k1 = 0.845932; _k2 = -3.123762; _k3 = 1.713453; _k4 = 0.719347; _k5 = -2.945117; _k6 = 1.641835;
			_p1 = 0.000795; _p2 = -0.000256;
			_width = 1920; _height = 1080;
		}
		
		void setup()
		{
			auto conf = ofx::AzureKinectDK::DeviceConfiguration();
			conf.index = 0;
			conf.aligne = ofx::AzureKinectDK::PixelAligneTo::COLOR;
			conf.camera_fps = K4A_FRAMES_PER_SECOND_30;
			conf.color_format = K4A_IMAGE_FORMAT_COLOR_BGRA32;
			conf.color_resolution = K4A_COLOR_RESOLUTION_1080P;
			conf.depth_mode = K4A_DEPTH_MODE_WFOV_2X2BINNED;
			conf.synchronized_images_only = true;
			conf.depth_delay_off_color_usec = 0;
			conf.wired_sync_mode = K4A_WIRED_SYNC_MODE_STANDALONE;
			conf.disable_streaming_indicator = false;
			conf.subordinate_delay_off_master_usec = 0;

			device = std::make_shared<ofx::AzureKinectDK::Device>(conf);
			device->startCameras();


			playback = std::make_shared<ofx::AzureKinectDK::Playback>("output.mkv", ofx::AzureKinectDK::PixelAligneTo::COLOR);
			playback->startCameras(true);

			//azure_kinects.emplace_back(device);
			azure_kinects.emplace_back(playback);

		}

		void debugDraw()
		{
			ofPushStyle();
			ofScale(0.5, 0.5);
			azure_kinects[0]->getImage<ofx::AzureKinectDK::Image::Color>()->draw(0, 0);
			azure_kinects[0]->getImage<ofx::AzureKinectDK::Image::Depth>()->draw(1920, 0);
			azure_kinects[0]->getImage<ofx::AzureKinectDK::Image::PointCloud>()->draw(1920, 1080);
			//flow.drawUndistort(azure_kinects[0]->getImage< ofx::AzureKinectDK::Image::Color>(), 0, 1080);
			ofPopStyle();
		}

		virtual void setDistCoeffs() override
		{
			 calibration.setDistortionCoefficients(_k1, _k2, _p1, _p2, _k3, _k4, _k5, _k6);
		}
	private:
		std::vector<std::shared_ptr<ofx::AzureKinectDK::Base>> azure_kinects;
		std::shared_ptr<ofx::AzureKinectDK::Device> device;
		std::shared_ptr<ofx::AzureKinectDK::Playback> playback;
		float _fx, _fy, _cx, _cy;
		int _width, _height;
		float _sensorSizeX, _sensorSizeY;
		float _k1, _k2, _k3, _k4, _k5, _k6; //radial distortion coefficients
		float _p1, _p2;//tangental distotion coefficients
	};
}
