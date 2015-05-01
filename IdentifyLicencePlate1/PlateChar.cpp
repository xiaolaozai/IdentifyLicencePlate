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
ͳһ�ַ���С

src_img
ԴͼƬ

width
����ͼƬ��

height
����ͼƬ��
******************************************/
Mat PlateChar::reSizeChar(IplImage* pImg_src,int width,int height)
{
	Mat mat_src=pImg_src;
	int h=mat_src.rows,w=mat_src.cols;
	/// / ����2 x 3 �ĶԽǾ����ڶԽ�������1.0�ı������ųߴ硣
	Mat transformMat=Mat::eye(2,3,CV_32F);
	int m=max(w,h);
	transformMat.at<float>(0,2)=m/2-w/2;//�� ��Ӧ3
	transformMat.at<float>(1,2)=m/2-h/2;//�� ��Ӧ2

	Mat mat_tmp(m,m,mat_src.type());//���ͼ��
	//�任ͼ�񣬲��ú�ɫ�������ֵ  
	warpAffine(mat_src, mat_tmp, transformMat, mat_tmp.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0)); 

	Mat mat_dst;
	resize(mat_tmp,mat_dst,Size(width,height));//��һ���ַ�ͼ���С

	return mat_dst;
}

/****************************************
getHistogram
��ȡ�л��е���ɫֱ��ͼ

mat_src
ԴͼƬ����

type
�������ͣ�1��ʾ�У�0��ʾ��
******************************************/
Mat PlateChar::getHistogram(Mat mat_src,int type)
{
	int size=(type)?mat_src.rows:mat_src.cols;
	Mat mat_hist=Mat::zeros(1,size,CV_32F);//��ʼ��

	for(int i=0;i<size;i++)
	{
		Mat data=(type)?mat_src.row(i):mat_src.col(i);//�л�������
		mat_hist.at<float>(i)=countNonZero(data);//ͳ����һ�л�һ���У�����Ԫ�صĸ����������浽mat_hist��  
	}

	//ȡmat_histֱ��ͼ�е����ֵ 
	double min,max;
	minMaxLoc(mat_hist,&min,&max);

	//��һ��ֱ��ͼ
	if(max>0)
	{
		mat_hist.convertTo(mat_hist,-1,1.0f/max, 0); // ���Ų�ת��������һ���������ͣ�
		//src.convertTo(dst, type, scale, shift)
		//type ����Ǹ�ֵ������-1�������������������������ͬ
	}

	return mat_hist;
}

/****************************************
getFeatureHist
��ȡ��ɫֱ��ͼ����ֵ

mat_src
ԴͼƬ����

******************************************/
Mat PlateChar::getFeatureHist(Mat mat_src,int size)
{
	//��ȡ����ֱ��ͼ
	Mat verHist=getHistogram(mat_src,0);
	Mat horHist=getHistogram(mat_src,1);

	//��Ƶͼ��
	Mat lowData;  
    resize(mat_src, lowData, Size(size, size)); 

	int numCols=verHist.cols+horHist.cols+lowData.cols*lowData.cols;
	Mat mat_dst=Mat::zeros(1,numCols,CV_32F);
	
	//��¼����ֱ��ͼ����
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