#include <opencv2/opencv.hpp>

void countBrightness(int* pixel_array) {
	for (int i = 0; i < 256; i++) {
		pixel_array[i] = i*i*i/(256*256);
	}	
}
void changeBrightness(cv::Mat grayscale_image, int* pixel_array) {
	for (int i = 0; i < grayscale_image.rows; i++) {
		for (int j = 0; j < grayscale_image.cols; j++) {
			grayscale_image.at<uchar>(i, j) = pixel_array[grayscale_image.at<uchar>(i, j)];
		}
	}
}

int main() {
	cv::Mat image_png = cv::imread("../../../data/cross_0256x0256.png");
	cv::imwrite("lab03_rgb.png", image_png);

	cv::Mat channels_split[3];
	cv::split(image_png, channels_split);

	cv::imwrite("lab03_gre.png", channels_split[0]);
	
	int pixels_count[256];

	countBrightness(pixels_count);

	changeBrightness(channels_split[0], pixels_count);
	cv::imwrite("lab03_gre_res.png", channels_split[0]);

	changeBrightness(channels_split[1], pixels_count);
	changeBrightness(channels_split[2], pixels_count);

	for (size_t i = 0; i < 256; i++)
	{
		std::cout << pixels_count[i] << '\n';
	}
	cv::Mat channels_merge[3] = { channels_split[0],channels_split[1], channels_split[2] };
	cv::Mat changed_image;
	cv::merge(channels_merge, 3, changed_image);
	cv::imshow("1", changed_image);
	cv::imwrite("lab03_rgb_res.png", changed_image);
	
	cv::Mat graphic(256, 256, CV_8UC3, cv::Scalar(240, 240, 240));

	for (int i = 0; i < 255; i++)
	{
		cv::line(graphic, cv::Point((i), 256-pixels_count[i]),
			cv::Point(i+1, 256 - pixels_count[i+1]),
			cv::Scalar(255, 0, 0), 1, 8, 0);
	}
	cv::imshow("2", graphic);
	cv::imwrite("lab03_viz_func.png", graphic);
	
	cv::waitKey(0);
}