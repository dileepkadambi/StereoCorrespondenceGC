#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/legacy/legacy.hpp"
#include <stdio.h>
#include <string.h>
#include <utils.h>
#include "PushRelabel.h"

using namespace cv;
using namespace std;
 

int main()
{
	char srcLeftPathComplete[500], srcRightPathComplete[500];

	std::string srcLeftPath = "\\StereoCorrespondenceGC\\testimages\\scene_left.jpg";
	std::string srcRightPath = "\\StereoCorrespondenceGC\\testimages\\scene_right.jpg";

	Utils objUtils = Utils();
	objUtils.GetFullPathFromEnv(srcLeftPath.c_str(), srcLeftPathComplete);
	objUtils.GetFullPathFromEnv(srcRightPath.c_str(), srcRightPathComplete);

	//IplImage* srcLeft = cvLoadImage("C:/Users/Ajinkya/Documents/B Tech Project/trialimages/scene_left.jpg",1);
	//IplImage* srcRight = cvLoadImage("C:/Users/Ajinkya/Documents/B Tech Project/trialimages/scene_right.jpg",1);

	IplImage* srcLeft = cvLoadImage(srcLeftPathComplete,1);
	IplImage* srcRight = cvLoadImage(srcRightPathComplete,1);

	IplImage* leftImage = cvCreateImage(cvGetSize(srcLeft), IPL_DEPTH_8U, 1);

	IplImage* rightImage = cvCreateImage(cvGetSize(srcRight), IPL_DEPTH_8U, 1);

	cvCvtColor(srcLeft, leftImage, CV_BGR2GRAY);

	cvCvtColor(srcRight, rightImage, CV_BGR2GRAY);

	CvSize size = cvGetSize(srcLeft);

	CvMat* disparity_left = cvCreateMat( size.height, size.width, CV_8S );

	CvMat* disparity_right = cvCreateMat( size.height, size.width, CV_8S );

	PushRelabel *prLabel = new PushRelabel(size.height*size.width*2);
	
	CvStereoGCState* state = cvCreateStereoGCState( 16, 2 );

	cvFindStereoCorrespondenceGC( leftImage, rightImage, disparity_left, disparity_right, state, 0 );

	cvReleaseStereoGCState( &state );

	CvMat* disparity_left_visual = cvCreateMat( size.height, size.width, CV_8U );

	cvConvertScale( disparity_left, disparity_left_visual, -16 );

	cvSaveImage("disparity.jpg", disparity_left_visual);
	cvNamedWindow("win1",1);
	cvNamedWindow("win2",1);

	cvNamedWindow("win3",1); 
	
	for(;;)
	{
		cvShowImage("win1",srcLeft);

		cvShowImage("win2",leftImage);

		cvShowImage("win3",disparity_left_visual); 
		
		std::cout << "Close Window when finished viewing" << std::endl;

		if(cvWaitKey(-10)==27) 
			break;

	}

	return 0;
}
