
#include "OpenCV/FaceRecognizer.h"
#include <QDebug>

using namespace cv;

OpenCV::FaceRecognizer::FaceRecognizer()
{
	if (!this->haar_cascade.load(file))
	{
		qDebug() << "Cannot load file: " << QString::fromStdString(file);
	}
	this->detected=false;
	this->notShaking=false;
}

OpenCV::FaceRecognizer::~FaceRecognizer()
{
	this->faces.clear();
}

void OpenCV::FaceRecognizer::detectFaces(Mat gray)
{
	if (this->detected)
	{
			cv::Mat subImg = gray(cv::Rect(this->rect.x,this->rect.y,
										   this->rect.width,this->rect.height));
			this->haar_cascade.detectMultiScale(subImg, this->faces,1.1, 2,
												0|CV_HAAR_SCALE_IMAGE|
												CV_HAAR_FIND_BIGGEST_OBJECT|
												CV_HAAR_DO_ROUGH_SEARCH);
	}
	else
	{
		this->haar_cascade.detectMultiScale(gray, this->faces,1.1, 2,
											0|CV_HAAR_SCALE_IMAGE|
											CV_HAAR_FIND_BIGGEST_OBJECT|
											CV_HAAR_DO_ROUGH_SEARCH);
	}
}

void OpenCV::FaceRecognizer::annotateFaces(Mat frame)
{
	if (this->faces.size()>0)
	{
		Rect face_i = this->faces[0];
		if (detected)
		{
			face_i.x=(face_i.x-face_i.width*0.1+this->rect.x);
			face_i.width=face_i.width*1.2;
			face_i.y=(face_i.y-face_i.height*0.1+this->rect.y);
			face_i.height=face_i.height*1.2;
		}
		rectangle(frame, face_i, CV_RGB(0, 255,0), 1);
		computeEyesCoordinations(face_i,frame.size());
		this->rect=face_i;
		if (!detected)
		{
			detected=true;
		}
		else
		{
			notShaking=computeMovement(frame.cols,frame.rows);
			if (notShaking)
			{
				previousEyesCoord.x=eyesCoord.x;
				previousEyesCoord.y=eyesCoord.y;
			}
		}

	}
	else
	{
		detected=false;
	}
}

cv::CascadeClassifier OpenCV::FaceRecognizer::getCascadeClassifier()
{
	return this->haar_cascade;
}

void OpenCV::FaceRecognizer::computeEyesCoordinations(Rect face, Size size)
{
	this->eyesCoord.x = (((float)(face.x+face.width/2) / (float)size.width-0.5)/0.5)*100;
	this->eyesCoord.y = (((float)(face.y+face.height/3) / (float)size.height-0.5)/0.5)*100;
}

cv::Point2i OpenCV::FaceRecognizer::getEyesCoords()
{
	return this->eyesCoord;
}

float OpenCV::FaceRecognizer::getHeadDistance(int screenWidth)
{
	if (this->faces.size()>0)
	{
		Rect face = this->faces[0];
		// 1.0 when the face width is 3.5 times smaller than frame
		return ((float)screenWidth/(float)face.width)/3.5f;
	}
	return 0.0f;
}

bool OpenCV::FaceRecognizer::computeMovement(int width, int height)
{
	// movement if difference on x axis is more than threshold of 0,8% of screen width
	if (((float)abs(previousEyesCoord.x-eyesCoord.x)/(float)width)>0.008)
	{
		previousEyesCoord.x=eyesCoord.x;
		previousEyesCoord.y=eyesCoord.y;
		return true;
	}
	// movement if difference on y axis is more than threshold of 1,05% of screen width
	if (((float)abs(previousEyesCoord.y-eyesCoord.y)/(float)width)>0.0105)
	{
		previousEyesCoord.x=eyesCoord.x;
		previousEyesCoord.y=eyesCoord.y;
		return true;
	}
	return false;
}

