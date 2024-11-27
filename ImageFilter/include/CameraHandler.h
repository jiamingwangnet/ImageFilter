#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <thread>
#include <mutex>

class CameraHandler
{
public:
	void Open(int device);
	void Close();
	void Capture(unsigned int texture);
	void BeginReadBuf();
	void EndReadBuf();

	void UpscaleImage(bool v) { 
		upscale = v; 
	}
	bool isUpscaling() const { return upscale; }

	bool isOpen() const { return open; }

	int GetWidth() const { return 2 / (-upscale + 2) * cap.get(cv::CAP_PROP_FRAME_WIDTH); }
	int GetHeight() const { return 2 / (-upscale + 2) * cap.get(cv::CAP_PROP_FRAME_HEIGHT); }

private:
	cv::Mat src;
	cv::VideoCapture cap;

	std::thread bufferT;
	std::mutex srcM;

	bool open = false;
	bool end = false;

	std::vector<unsigned char> upscaled;

	bool upscale = true;
};