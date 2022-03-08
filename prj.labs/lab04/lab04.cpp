#include <opencv2/opencv.hpp>

void lab4(std::string nameOfVideo, int border, int morphologySize1, int morphologySize2)
{
	cv::VideoCapture video("../../../data/" + nameOfVideo + ".mp4");

	if (!video.isOpened()) {
			std::cout << "Error opening video stream or file" << std::endl;
	}

    int nFrames = video.get(cv::CAP_PROP_FRAME_COUNT);

    cv::Mat frames[3];
    //cv::Mat framesGS[3];
    //cv::Mat framesBin[3];

    int frame_number;
    for (size_t i = 0; i < 3; i++)
    {
        frame_number = nFrames / 5 * (i + 2);
        video.set(cv::CAP_PROP_POS_FRAMES, frame_number);
        video >> frames[i];
        cv::imwrite("frames/" + nameOfVideo + "Original" + std::to_string(i + 1) + ".png", frames[i]);
        cv::cvtColor(frames[i], frames[i], cv::COLOR_BGR2GRAY);
        cv::threshold(frames[i], frames[i], border, 255, cv::THRESH_BINARY);
        cv::imwrite("frames/" + nameOfVideo + "Binarizacia" + std::to_string(i + 1) + ".png", frames[i]);
        //cv::imshow("frame"+ std::to_string(i), frames[i]);
        cv::Mat maska;
        cv::morphologyEx(frames[i], maska, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(morphologySize1, morphologySize2)));
        cv::morphologyEx(maska, maska, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10)));
        cv::imwrite("frames/"+ nameOfVideo + "WithMask" + std::to_string(i + 1) + ".png", maska);
        //cv::imshow("maska" + std::to_string(i), maska);
        cv::Mat srcOut(maska.size(), CV_32S);;
        cv::Mat stats;
        cv::Mat centroids;
        int nLabels = cv::connectedComponentsWithStats(maska, srcOut, stats, centroids);
        std::vector<cv::Vec3b> colors(nLabels);
        for (size_t i = 0; i < nLabels; i++)
        {
            colors[i] = cv::Vec3b((rand() & 255), (rand() & 255), (rand() & 255));
        }


        cv::Mat dst(maska.size(), CV_8UC3);
        for (int r = 0; r < dst.rows; ++r) {
            for (int c = 0; c < dst.cols; ++c) {
                int label = srcOut.at<int>(r, c);
                cv::Vec3b& pixel = dst.at<cv::Vec3b>(r, c);
                pixel = colors[label];
            }
        }
        cv::imwrite("frames/" + nameOfVideo + "ConnectedComponents" + std::to_string(i + 1) + ".png", dst);
    }

    video.release();
    cv::destroyAllWindows();
}

int main() {
    //lab4("5k", 188, 50,150);
    //lab4("2k", 185, 100,250);
    //lab4("1k", 187, 140,140);
    lab4("100rub", 120, 180,100);
    //lab4("500rub", 156, 30,30); //cv::THRESH_TOZERO

}