#include "StdAfx.h"
#include "PlateChar.h"


PlateChar::PlateChar(void)
{
}


PlateChar::~PlateChar(void)
{
}

/****************************************
reSizeChar
统一字符大小

src_img
源图片

width
设置图片宽

height
设置图片高
******************************************/
Mat PlateChar::reSizeChar(IplImage* pImg_src,int width,int height)
{
	Mat mat_src=pImg_src;
	int h=mat_src.rows,w=mat_src.cols;
	/// / 创建2 x 3 的对角矩阵并在对角线上以1.0的比率缩放尺寸。
	Mat transformMat=Mat::eye(2,3,CV_32F);
	int m=max(w,h);
	transformMat.at<float>(0,2)=m/2-w/2;//宽 对应3
	transformMat.at<float>(1,2)=m/2-h/2;//高 对应2

	Mat mat_tmp(m,m,mat_src.type());//输出图像
	//变换图像，并用黑色填充其余值  
	warpAffine(mat_src, mat_tmp, transformMat, mat_tmp.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0)); 

	Mat mat_dst;
	resize(mat_tmp,mat_dst,Size(width,height));//归一化字符图像大小

	return mat_dst;
}

/****************************************
getHistogram
获取行或列的颜色直方图

mat_src
源图片矩阵

type
行列类型，1表示行，0表示列
******************************************/
Mat PlateChar::getHistogram(Mat mat_src,int type)
{
	int size=(type)?mat_src.rows:mat_src.cols;
	Mat mat_hist=Mat::zeros(1,size,CV_32F);//初始化

	for(int i=0;i<size;i++)
	{
		Mat data=(type)?mat_src.row(i):mat_src.col(i);//行或列数据
		mat_hist.at<float>(i)=countNonZero(data);//统计这一行或一列中，非零元素的个数，并保存到mat_hist中  
	}

	//取mat_hist直方图中的最大值 
	double min,max;
	minMaxLoc(mat_hist,&min,&max);

	//归一化直方图
	if(max>0)
	{
		mat_hist.convertTo(mat_hist,-1,1.0f/max, 0); // 缩放并转换到另外一种数据类型：
		//src.convertTo(dst, type, scale, shift)
		//type 如果是负值（常用-1）则输出矩阵和输入矩阵类型相同
	}

	return mat_hist;
}

/****************************************
getFeatureHist
获取颜色直方图特征值

mat_src
源图片矩阵

******************************************/
Mat PlateChar::getFeatureHist(Mat mat_src,int size)
{
	//获取行列直方图
	Mat verHist=getHistogram(mat_src,0);
	Mat horHist=getHistogram(mat_src,1);

	//低频图像
	Mat lowData;  
    resize(mat_src, lowData, Size(size, size)); 

	int numCols=verHist.cols+horHist.cols+lowData.cols*lowData.cols;
	Mat mat_dst=Mat::zeros(1,numCols,CV_32F);
	
	//记录行列直方图数据
	int k=0;
	for(int i=0;i<verHist.cols;i++)
	{
		mat_dst.at<float>(k++)=verHist.at<float>(i);
	}

	for(int i=0;i<horHist.cols;i++)
	{
		mat_dst.at<float>(k++)=horHist.at<float>(i);
	}

	for(int x=0; x<lowData.cols; x++)  
    {  
        for(int y=0; y<lowData.rows; y++){  
            mat_dst.at<float>(k++)=(float)lowData.at<unsigned char>(x,y);   
        }  
    }

	return mat_dst;
}