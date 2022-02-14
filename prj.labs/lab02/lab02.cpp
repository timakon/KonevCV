#include <opencv2/opencv.hpp>

int main() {
	cv::Mat image_png = cv::imread("../../../data/cross_0256x0256.png");

	std::vector<int> quality;
	cv::imwrite("cross_0256x0256_025.jpg", image_png, { cv::IMWRITE_JPEG_QUALITY, 25 });

	cv::Mat channels_split[3];
	cv::split(image_png, channels_split);

	cv::Mat zero_matrix(256, 256, CV_8UC1);
	zero_matrix = 0;

	cv::Mat channels_merge_red[3] = { zero_matrix, zero_matrix, channels_split[2] };
	cv::Mat channels_merge_green[3] = { zero_matrix, channels_split[1], zero_matrix };
	cv::Mat channels_merge_blue[3] = { channels_split[0], zero_matrix, zero_matrix };

	cv::Mat red;
	cv::Mat green;
	cv::Mat blue;

	cv::merge(channels_merge_red, 3, red);
	cv::merge(channels_merge_green, 3, green);
	cv::merge(channels_merge_blue, 3, blue);

	cv::Mat mosaic_top;
	cv::Mat mosaic_bottom;
	cv::Mat mosaic;

	cv::hconcat(image_png, red, mosaic_top);
	cv::hconcat(green, blue, mosaic_bottom);
	cv::vconcat(mosaic_top, mosaic_bottom, mosaic);

	cv::imwrite("cross_0256x0256_png_channels.png", mosaic);

	cv::imwrite("image_jpg.jpg", image_png, { cv::IMWRITE_JPEG_QUALITY, 100 });

	cv::Mat image_jpg = cv::imread("./image_jpg.jpg");

	cv::split(image_jpg, channels_split);

	cv::Mat channels_merge_red_jpg[3] = { zero_matrix, zero_matrix, channels_split[2] };
	cv::Mat channels_merge_green_jpg[3] = { zero_matrix, channels_split[1], zero_matrix };
	cv::Mat channels_merge_blue_jpg[3] = { channels_split[0], zero_matrix, zero_matrix };

	cv::merge(channels_merge_red_jpg, 3, red);
	cv::merge(channels_merge_green_jpg, 3, green);
	cv::merge(channels_merge_blue_jpg, 3, blue);

	cv::hconcat(image_png, red, mosaic_top);
	cv::hconcat(green, blue, mosaic_bottom);
	cv::vconcat(mosaic_top, mosaic_bottom, mosaic);

	cv::imwrite("cross_0256x0256_jpg_channels.png", mosaic);

	cv::Mat mosaic_jpg = cv::imread("./cross_0256x0256_jpg_channels.png");
	cv::Mat mosaic_png = cv::imread("./cross_0256x0256_png_channels.png");




	cv::waitKey(0);
}