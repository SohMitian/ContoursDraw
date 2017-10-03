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

	//�摜�̑��݊m�F
	if (img.empty())
	{
		cerr << "�摜���Ȃ�" << endl;
		//system("pause");
		return -1;
	}

	int flag, Threshold;
	float x, y;

	//std::vector <std::vector <cv::Point>> contours; //�֊s x,y
	std::vector <cv::Point> contours;
	std::vector< cv::Point > approx;

	cv::Point temp;
	

	ifstream ifs("pointXY.csv");


	//csv�t�@�C����1�s���ǂݍ���
	string str;
	while (getline(ifs, str)) {
		string token;
		istringstream stream(str);
		flag = 0;
		x = 0;
		y = 0;
		//1�s�̂����A������ƃR���}�𕪊�����
		while (getline(stream, token, ',')) {
			//���ׂĕ�����Ƃ��ēǂݍ��܂�邽��
			//���l�͕ϊ����K�v
			if (flag == 0) {
				x = stof(token); //stof(string str) : string��float�ɕϊ�
				x *= 100;
				x -= 92800;
				temp.x = x;
				
			}
			else {
				y = stof(token); //stof(string str) : string��float�ɕϊ�
				y *= 100;
				y -= 90100;
				temp.y = y;
			}
			flag += 1;
		}
		contours.push_back(temp);
	}


	// ��l��
	cv::Mat gray_img, bin_img;
	cv::cvtColor(img, gray_img, CV_BGR2GRAY);
	cv::threshold(gray_img, bin_img, 0, 255, cv::THRESH_BINARY);

	//�֊s�̒����ߎ�
	Threshold = 80;
	cv::approxPolyDP(cv::Mat(contours), approx, Threshold, false);
	//���C�������o��
	cv::polylines(img, approx, true, cv::Scalar(0, 255, 255), 2);
	//���������o��
	putText(img, "Threshold:" + to_string(Threshold), Point(500, 2500), FONT_HERSHEY_DUPLEX, 10, Scalar(255, 255, 255), 8, CV_AA);
	putText(img, "Points:" + to_string(approx.size()), Point(500, 3000), FONT_HERSHEY_DUPLEX, 10, Scalar(255, 255, 255), 8, CV_AA);
	

	cv::imwrite("out.jpg", img);
}
