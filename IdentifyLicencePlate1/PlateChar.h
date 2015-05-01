#pragma once
class PlateChar
{
public:
	PlateChar(void);
	Mat reSizeChar(IplImage* pImg_src,int width,int height);
	Mat getHistogram(Mat mat_src,int type);
	Mat getFeatureHist(Mat mat_src,int size);
	~PlateChar(void);
};

