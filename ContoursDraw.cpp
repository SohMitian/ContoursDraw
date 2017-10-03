#include <iostream>
#include <cstdio>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;
using namespace cv;

int
main(int argc, char *argv[])
{
	cv::Mat img = cv::imread("xy.jpg", 1);

	//画像の存在確認
	if (img.empty())
	{
		cerr << "画像がない" << endl;
		//system("pause");
		return -1;
	}

	int flag, Threshold;
	float x, y;

	//std::vector <std::vector <cv::Point>> contours; //輪郭 x,y
	std::vector <cv::Point> contours;
	std::vector< cv::Point > approx;

	cv::Point temp;
	

	ifstream ifs("pointXY.csv");


	//csvファイルを1行ずつ読み込む
	string str;
	while (getline(ifs, str)) {
		string token;
		istringstream stream(str);
		flag = 0;
		x = 0;
		y = 0;
		//1行のうち、文字列とコンマを分割する
		while (getline(stream, token, ',')) {
			//すべて文字列として読み込まれるため
			//数値は変換が必要
			if (flag == 0) {
				x = stof(token); //stof(string str) : stringをfloatに変換
				x *= 100;
				x -= 92800;
				temp.x = x;
				
			}
			else {
				y = stof(token); //stof(string str) : stringをfloatに変換
				y *= 100;
				y -= 90100;
				temp.y = y;
			}
			flag += 1;
		}
		contours.push_back(temp);
	}


	// 二値化
	cv::Mat gray_img, bin_img;
	cv::cvtColor(img, gray_img, CV_BGR2GRAY);
	cv::threshold(gray_img, bin_img, 0, 255, cv::THRESH_BINARY);

	//輪郭の直線近似
	Threshold = 80;
	cv::approxPolyDP(cv::Mat(contours), approx, Threshold, false);
	//ライン書き出し
	cv::polylines(img, approx, true, cv::Scalar(0, 255, 255), 2);
	//文字書き出し
	putText(img, "Threshold:" + to_string(Threshold), Point(500, 2500), FONT_HERSHEY_DUPLEX, 10, Scalar(255, 255, 255), 8, CV_AA);
	putText(img, "Points:" + to_string(approx.size()), Point(500, 3000), FONT_HERSHEY_DUPLEX, 10, Scalar(255, 255, 255), 8, CV_AA);
	

	cv::imwrite("out.jpg", img);
}
