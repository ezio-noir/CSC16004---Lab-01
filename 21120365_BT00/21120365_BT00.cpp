#include <iostream>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

void printHelp() {
	cout << 
R"(
Usage: <program> <path_to_image> <option> [option's argument]
Options:
	- /RGB2GRAY: converts RGB image to gray scale image (image must have 3 channels).
	- /GRAY2RGB: converts gray image to RGB image (image must have a single channel).
	- /BRIGHTNESS <beta: double>: adds `beta` to each pixel brighness.
	- /CONTRAST <alpha: double>: multiplies `alpha` to each pixel brightness.
There are 3 samples of RGB image and 3 of grayscale image inside the folder `sample`.
Check the file `output.png` for the last success output image.
)"
	<< endl;
}

bool writeImage(const string &fileName, Mat &img) {
	if (!img.empty()) {
		return imwrite(fileName, img);
	}
	return false;
}

void showImage(const string windowName, Mat& img) {
	if (!img.empty()) {
		namedWindow(windowName, WINDOW_AUTOSIZE);
		imshow(windowName, img);
	}
}

// Converts RGB image to grayscale image
Mat convertRgbToGray(Mat& rgbImg) {
	Mat grayImg;
	cvtColor(rgbImg, grayImg, COLOR_BGR2GRAY);
	return grayImg;
}

// Convert black-and-white image to RGB image
Mat convertGrayToRgb(Mat& grayImg) {
	Mat rgbImg;

	cvtColor(grayImg, rgbImg, COLOR_GRAY2BGR);
	return rgbImg;
}

Mat changeBrightness(Mat& img, double beta) {
	Mat result;

	img.convertTo(result, -1, 1.0, beta);
	return result;
}

Mat changeContrast(Mat& img, double alpha) {
	Mat result;
	img.convertTo(result, -1, alpha, 0.0);
	return result;
}

template<typename... Args>
Mat processImage(Mat& originalImage, Mat (*func)(Mat&, Args...), Args... args) {
	return func(originalImage, args...);
}

string originalFilePath;
Mat originalImage = Mat();

int main(int argc, char* argv[]) {
	try {
		if (argc < 3 || argc > 4) {
			throw invalid_argument("Invalid command!");
		}

		originalFilePath = argv[1];
		originalImage = imread(originalFilePath, IMREAD_ANYCOLOR);
		Mat result;
		if (argc == 3) {
			// Use case 1: Convert color image (RGB) to grayscale image
			if (strcmp(argv[2], "/RGB2GRAY") == 0) {
				result = convertRgbToGray(originalImage);
			}
			// Use case 2: Convert grayscale to RGB image
			else if (strcmp(argv[2], "/GRAY2RGB") == 0) {
				result = convertGrayToRgb(originalImage);
			}
			else throw invalid_argument("Unknown options");
		}
		else {
			// Use case 3: Change image's brightness
			if (strcmp(argv[2], "/BRIGHTNESS") == 0) {
				result = changeBrightness(originalImage, stod(argv[3]));
			}
			// Use case 4: Change image's contrast
			else if (strcmp(argv[2], "/CONTRAST") == 0) {
				result = changeContrast(originalImage, stod(argv[3]));
			}
			else throw invalid_argument("Unknown options");
		}

		showImage("Original", originalImage);
		showImage("Result", result);
		writeImage("output.png", result);
		waitKey();
	}
	catch (invalid_argument &e) {
		cerr << e.what() << endl;
		printHelp();
		return -1;
	}

	return 0;
}