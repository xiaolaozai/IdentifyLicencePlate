#pragma once
class PlateChar
{
public:
	PlateChar(void);
	Mat reSizeChar(IplImage* pImg_src,int width,int height);
	IplImage* showHistogram(IplImage* pImg_src,IplImage* pImg_hist,int type);
	void getPlatePosition(IplImage* pImg_src,int &x0,int &x1, float xT,int &y0,int &y1,float yT);
	Mat getHistogram(Mat mat_src,int type);
	Mat getFeatureHist(Mat mat_src,int size);
	~PlateChar(void);
};

