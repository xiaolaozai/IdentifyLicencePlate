#pragma once
class IplImageProcessing
{
public:
	IplImageProcessing(void);
	IplImage* myRGB2Gray(IplImage* src,IplImage* dst);
	void myReSizeAllImage(string dir_path,string save_path,int new_height,int new_width);
	void myHistogramStretch(IplImage *pImg_src,IplImage *pImg_dst);
	IplImage* myGrayStretch(IplImage* src,int threshold1,int threshold2);
	void reCorrectPosition(IplImage* pImg_src,IplImage* pImg_dst,int type);
	IplImage* myDilate(IplImage* src,int nVer,int nVerTimes,int nHor,int nHorTimes);
	IplImage* myErode(IplImage* src,int nVer,int nVerTimes,int nHor,int nHorTimes);
	IplImage* myThreshold(IplImage* src,int type);
	IplImage* myBaseAdaptiveThreshold(IplImage* src);
	bool isMyThresholdCount(IplImage* src,int threshold,double scale,int type);
	IplImage* myCutImageLine(IplImage* src,int verMinCount,int horMinCount,int type);
	IplImage* myCutImage(IplImage* src,IplImage* dst,CvRect rect);
	list<CvRect> myRoughing(IplImage* src,IplImage* pImg_contour,list<CvRect> list_rects,double minX,double minY,double minWidth,double minHeight,double maxWidth,double minK,double maxK,int minArea);
	void mySearchVerPosition(IplImage* pImg_src,int &x1,int &x2,int nTimes);
	void mySearchHorPosition(IplImage* pImg_src,int &x1,int &x2,int subGray,int nY);
	bool isMyLicence(IplImage* src,int subGray,int &x1,int &x2,int nX,int &y1,int &y2,int nY);
	void myExtractRGB(IplImage* src,int &colorType,float blueThreshold,float yellowThreshold,float whiteThreshold,float blackThreshold);
	void myExtractHSV(IplImage* src,int &colorType);
	void myGetPlateBackColor(IplImage* src,IplImage* dst,int &colorType);
	void myAffineImage(IplImage* pImg_src,IplImage* pImg_dst,int angel);
	~IplImageProcessing(void);
};
