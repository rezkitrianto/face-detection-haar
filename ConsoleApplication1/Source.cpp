#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\objdetect\objdetect.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <opencv2\core\core.hpp>
#include <highgui.h>

using namespace std;
using namespace cv;

int main() {
	/*
	what to do:
	1. Read the video
	2. Convert each frame to gray level image.
	3. Use a CascadeClassifier to collect subWindow(faces) information.
	4. Use another CascadeClassifier to do nestedCascade to collect subWindow(eyes) informations.
	5. Label the location of face and eyes with rectangles or circles
	6. Save six image results.
	*/

	string fileName = "SRC.mp4";
	VideoCapture theVideo(fileName);

	if (!theVideo.isOpened()){
		throw "file is missing or corrupt";
	}
	else{
		int counter = 1;
		Mat frame, frameGray, roi;
		vector<Rect> vecDetFace, vetDetEye;
		CascadeClassifier faceCascade, eyeCascade;
		faceCascade.load("haarcascade_frontalface_alt.xml");
		eyeCascade.load("haarcascade_eye_tree_eyeglasses.xml");

		
		while (theVideo.read(frame)){
			//1. Convert to gray level
			cvtColor(frame, frameGray, CV_BGR2GRAY);
			faceCascade.detectMultiScale(frameGray, vecDetFace, 1.1, 3, 0, Size(60, 60), Size(200, 200));
			printf("%d", vecDetFace.size());
			for (int i = 0; i < vecDetFace.size(); i++){
				rectangle(frame,
					Point(vecDetFace[i].x, vecDetFace[i].y),
					Point(vecDetFace[i].x + vecDetFace[i].width, vecDetFace[i].y + vecDetFace[i].height),
					Scalar(255, 0, 0), 4, 8, 0);

				roi = frameGray(vecDetFace[i]);
				eyeCascade.detectMultiScale(roi, vetDetEye, 1.1, 3, 0, Size(5, 10));

				for (unsigned int j = 0; j < vetDetEye.size(); j++){
					Point pt = Point(vecDetFace[i].x + vetDetEye[j].x + vetDetEye[j].width*0.5, vecDetFace[i].y + vetDetEye[j].y + vetDetEye[j].height*0.5);
					int radius = vetDetEye[j].width*0.5;
					circle(frame, pt, radius, Scalar(0, 255, 0), 1, 8, 0);
				}
						
			}
			imshow("The Video", frame);

			
			if (counter == 100 || counter == 300){
				string outputFileName = "resultFrame_" + std::to_string(counter) + ".jpg";
				imshow(outputFileName, frame);
				imwrite(outputFileName, frame);
			}
			

			counter++;
			waitKey(3);
		}
		
	}


	waitKey(0);
	return 0;
}