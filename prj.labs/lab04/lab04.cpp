#include <opencv2/opencv.hpp>

double quality(cv::Mat mask, cv::Mat standard_mask) {
    double quality = mask.rows* mask.cols;

    for (int i = 0; i < mask.rows; i++) {
        for (int j = 0; j < mask.cols; j++) {
            if (mask.at<uint8_t>(i, j) != standard_mask.at<uint8_t>(i, j)) {
                quality--;
            }
        }
    }
    quality = quality / (mask.rows * mask.cols) * 100;

    return quality;
}

void lab4(std::string nameOfVideo)
{
    cv::VideoCapture video("../../../data/4laba/" + nameOfVideo + ".mp4");

    if (!video.isOpened()) {
        std::cout << "Error opening video stream or file" << std::endl;
    }

    int nFrames = video.get(cv::CAP_PROP_FRAME_COUNT);

    cv::Mat frames[3];

    int frame_number;
    for (size_t i = 0; i < 3; i++)
    {
        frame_number = nFrames / 5 * (i + 2);
        video.set(cv::CAP_PROP_POS_FRAMES, frame_number);
        video >> frames[i];
        cv::imwrite("frames/" + nameOfVideo + "Original" + std::to_string(i + 1) + ".png", frames[i]);
        cv::cvtColor(frames[i], frames[i], cv::COLOR_BGR2GRAY);
        cv::threshold(frames[i], frames[i], 187, 255, cv::THRESH_BINARY);
        cv::imwrite("frames/" + nameOfVideo + "Binarizacia" + std::to_string(i + 1) + ".png", frames[i]);
        //cv::imshow("frame"+ std::to_string(i), frames[i]);
        cv::Mat maska;
        cv::morphologyEx(frames[i], maska, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(140, 140)));
        cv::morphologyEx(maska, maska, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10)));
        cv::imwrite("frames/" + nameOfVideo + "WithMask" + std::to_string(i + 1) + ".png", maska);
        cv::Mat srcOut(maska.size(), CV_32S);;
        cv::Mat stats;
        cv::Mat centroids;
        int nLabels = cv::connectedComponentsWithStats(maska, srcOut, stats, centroids);
        std::vector<cv::Vec3b> colors(nLabels);

        std::cout << "\nnLables: " << nLabels << "\n";
        std::cout << nameOfVideo << std::endl;
        int max = 0;
        int maxLabel = 0;
        for (int j = 1; j < nLabels; j++) {
            if (max < stats.at<int>(j, cv::CC_STAT_AREA)) {
                max = stats.at<int>(j, cv::CC_STAT_AREA);
                maxLabel = j;
            }

            std::cout << "frame: " << i << " label: " << j << " area: " << stats.at<int>(j, cv::CC_STAT_AREA) << std::endl;
        }

        for (size_t i = 0; i < nLabels; i++)
        {
            colors[i] = cv::Vec3b(0, 0, 0);
        }
        colors[maxLabel] = cv::Vec3b(255, 255, 255);

        cv::Mat dst(maska.size(), CV_8UC3);
        for (int r = 0; r < dst.rows; ++r) {
            for (int c = 0; c < dst.cols; ++c) {
                int label = srcOut.at<int>(r, c);
                cv::Vec3b& pixel = dst.at<cv::Vec3b>(r, c);
                pixel = colors[label];
            }
        }
        cv::imwrite("frames/" + nameOfVideo + "ConnectedComponents" + std::to_string(i + 1) + ".png", dst);
        std::cout<< quality(dst, cv::imread("../../../data/4laba/idealMask" + nameOfVideo + ".png")) << "------------------------------\n";
    }

    video.release();
    cv::waitKey(0);
    cv::destroyAllWindows();
}

int main() {
    lab4("5k");
    lab4("2k");
    lab4("1k");
    lab4("100rub");
    lab4("500rub");
}