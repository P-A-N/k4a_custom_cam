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
			_fx= 252.248932; _cx= 256.460632; _fy= 252.244553; _cy= 256.971558;
			_k1 = 5.242249; _k2 = 3.345925; _k3 = 0.167012; _k4 = 5.571374; _k5 = 5.087757; _k6 = 0.897048;
			_p1 = 0.000028; _p2 = 0.000047;
			_width = 512; _height = 512;
		}
		
		void setup()
		{
			auto conf = ofx::AzureKinectDK::DeviceConfiguration();
			conf.index = 0;
			conf.camera_fps = K4A_FRAMES_PER_SECOND_30;
			conf.color_format = K4A_IMAGE_FORMAT_COLOR_BGRA32;
			conf.color_resolution = K4A_COLOR_RESOLUTION_1080P;
			conf.depth_mode = K4A_DEPTH_MODE_WFOV_2X2BINNED;
			conf.synchronized_images_only = true;
			conf.depth_delay_off_color_usec = 0;
			conf.wired_sync_mode = K4A_WIRED_SYNC_MODE_STANDALONE;
			conf.disable_streaming_indicator = false;
			conf.subordinate_delay_off_master_usec = 0;

			//device = std::make_shared<ofx::AzureKinectDK::Device>(conf);
			//device->startCameras();


			playback = std::make_shared<ofx::AzureKinectDK::Playback>("output.mkv", ofx::AzureKinectDK::PixelAligneSetting());
			playback->startCameras(true);

			//azure_kinects.emplace_back(device);
			azure_kinects.emplace_back(playback);

		}

		void update()
		{
			calibTexture = azure_kinects[0]->getImage<ofx::AzureKinectDK::Image::Infrared>()->getTexture();

		}

		void drawCalib(int x, int y)
		{
			ofPushStyle();
			azure_kinects[0]->getImage<ofx::AzureKinectDK::Image::Infrared>()->draw(0, 0);
			//ofScale(0.5, 0.5);
			//if (bDrawUndistort)
			//{
			//	if (inputs.size() > 0)
			//		drawUndistort(inputs[0]->getPixels(), 0, 0);
			//	drawUndistort(calibPixels, 0, 0);
			//}
			//else
			//{
			//	if (inputs.size() > 0)
			//		inputs[0]->draw(x, y);
			//	calibTexture.draw(x, y);
			//}
			ofPopStyle();
		}

		void debugDraw()
		{
			ofPushStyle();
			ofScale(0.5, 0.5);
			azure_kinects[0]->getImage<ofx::AzureKinectDK::Image::Infrared>()->draw(0, 0);
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
