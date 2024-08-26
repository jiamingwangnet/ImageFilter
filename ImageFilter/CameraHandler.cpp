#include "include/CameraHandler.h"
#include <glad/gl.h>
#include <iostream>

void CameraHandler::Open(int device)
{
	cap.open(device, cv::CAP_DSHOW);
	open = true;

	upscaled.resize((size_t)4 * cap.get(cv::CAP_PROP_FRAME_WIDTH) * cap.get(cv::CAP_PROP_FRAME_HEIGHT) * 3);
}

void CameraHandler::Close()
{
	cap.release();
	open = false;
}

void CameraHandler::Capture(unsigned int texture)
{
	if (!srcM.try_lock()) return;

	int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
	
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 2 * width, 2 * height, GL_BGR, GL_UNSIGNED_BYTE, upscaled.data()); // assume RGB format

	srcM.unlock();
}

void CameraHandler::BeginReadBuf()
{
	bufferT = std::thread{
		[this]() {
			while (!end)
			{
				if (!open) continue;

				srcM.lock();

				cap >> src;
				cv::flip(src, src, 1);

				int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
				int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

				// 2x upscale nearest
				for (size_t y = 0; y < height; y++)
				{
					for (size_t x = 0; x < width * 3; x += 3)
					{
						upscaled[(4 * y * width * 3 + 2 * x)] = src.ptr()[y * width * 3 + (x)];
						upscaled[(4 * y * width * 3 + 2 * x + 1)] = src.ptr()[y * width * 3 + (x + 1)];
						upscaled[(4 * y * width * 3 + 2 * x + 2)] = src.ptr()[y * width * 3 + (x + 2)];

						upscaled[(4 * y * width * 3 + 2 * x + 3)] = src.ptr()[y * width * 3 + (x)];
						upscaled[(4 * y * width * 3 + 2 * x + 4)] = src.ptr()[y * width * 3 + (x + 1)];
						upscaled[(4 * y * width * 3 + 2 * x + 5)] = src.ptr()[y * width * 3 + (x + 2)];
					}

					std::copy_n(upscaled.begin() + 2 * y * width * 3 * 2, 3 * 2 * width, upscaled.begin() + 2 * y * width * 3 * 2 + 3 * 2 * width);
				}

				srcM.unlock();

				cv::waitKey(30);
			}
		}
	};
}

void CameraHandler::EndReadBuf()
{
	end = true;
	bufferT.join();
}
