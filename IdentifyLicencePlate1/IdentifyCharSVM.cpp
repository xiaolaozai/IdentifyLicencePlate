#include "StdAfx.h"
#include "IdentifyCharSVM.h"


IdentifyCharSVM::IdentifyCharSVM(void)
{
}


IdentifyCharSVM::~IdentifyCharSVM(void)
{
}

void IdentifyCharSVM::test()
{
	const int s=1000;//生成信息大小
	const float *support;
	int size=400;//图像大小
	int i,j,sv_num;
	IplImage *pImg;
	CvSVM svm;
	CvSVMParams param;//svm参数
	CvTermCriteria criteria;//停止迭代的标准,传入
	CvRNG rng=CvRNG(time(NULL));//随机数
	CvPoint pts[s];//分类信息点
	float data[s*2];
	int res[s];//分类信息
	CvMat data_mat,res_mat;
	CvScalar rcolor;
	
	//初始化
	pImg=cvCreateImage(cvSize(size,size),IPL_DEPTH_8U,3);
	cvZero(pImg);

	//随机生成测试图
	for(i=0;i<s;i++)
	{
		pts[i].x=cvRandInt(&rng)%size;
		pts[i].y=cvRandInt(&rng)%size;
		if(pts[i].y>50*cos(pts[i].x*CV_PI/100)+200)
		{
			cvLine(pImg,cvPoint(pts[i].x-2,pts[i].y-2),cvPoint(pts[i].x+2,pts[i].y+2),CV_RGB(255,0,0));
			cvLine(pImg,cvPoint(pts[i].x+2,pts[i].y-2),cvPoint(pts[i].x-2,pts[i].y+2),CV_RGB(255,0,0));
			res[i]=1;
		}
		else
		{
			if(pts[i].x>200)
			{
				cvLine(pImg,cvPoint(pts[i].x-2,pts[i].y-2),cvPoint(pts[i].x+2,pts[i].y+2),CV_RGB(0,255,0));
				cvLine(pImg,cvPoint(pts[i].x+2,pts[i].y-2),cvPoint(pts[i].x-2,pts[i].y+2),CV_RGB(0,255,0));
				res[i]=2;
			}
			else
			{
				cvLine(pImg,cvPoint(pts[i].x-2,pts[i].y-2),cvPoint(pts[i].x+2,pts[i].y+2),CV_RGB(0,0,255));
				cvLine(pImg,cvPoint(pts[i].x+2,pts[i].y-2),cvPoint(pts[i].x-2,pts[i].y+2),CV_RGB(0,0,255));
				res[i]=3;
			}
		}
	}

	//显示测试图
	cvShowImage("SVM test img",pImg);

	//学习生成数据
	for(int i=0;i<s;i++)
	{
		data[i*2]=float(pts[i].x)/size;
		data[i*2+1]=float(pts[i].y)/size;
	}
	//分配矩阵空间
	cvInitMatHeader(&data_mat,s,2,CV_32FC1,data);
	cvInitMatHeader(&res_mat,s,1,CV_32SC1,res);
	//停止迭代的标准,传入
	criteria=cvTermCriteria(CV_TERMCRIT_EPS,1000,FLT_EPSILON);
	param=CvSVMParams(CvSVM::C_SVC,CvSVM::RBF,10.0,8.0,1.0,10.0,0.5,0.1,NULL,criteria);

	//训练
	svm.train(&data_mat,&res_mat,NULL,NULL,param);

	//学习结果 描画
	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			CvMat m;
			float ret=0.0;
			float a[]={float(j)/size,float(i)/size};
			cvInitMatHeader(&m,1,2,CV_32FC1,a);//分配矩阵空间
			ret=svm.predict(&m);
			switch((int)ret)
			{
			case 1:
				rcolor=CV_RGB(100,0,0);
				break;
			case 2:
				rcolor=CV_RGB(0,100,0);
				break;
			case 3:
				rcolor=CV_RGB(0,0,100);
				break;
			}
			cvSet2D(pImg,i,j,rcolor);
		}
	}

	//再描画
	for(i=0;i<s;i++)
	{
		CvScalar rcolor;
		switch(res[i])
		{
		case 1:
			rcolor=CV_RGB(255,0,0);
			break;
		case 2:
			rcolor=CV_RGB(0,255,0);
			break;
		case 3:
			rcolor=CV_RGB(0,0,255);
			break;
		}
		cvLine(pImg,cvPoint(pts[i].x-2,pts[i].y-2),cvPoint(pts[i].x+2,pts[i].y+2),rcolor);
		cvLine(pImg,cvPoint(pts[i].x+2,pts[i].y-2),cvPoint(pts[i].x-2,pts[i].y+2),rcolor);
	}

	//绘制支持向量
	sv_num=svm.get_support_vector_count();
	for(i=0;i<sv_num;i++)
	{
		support=svm.get_support_vector(i);
		cvCircle(pImg,cvPoint((int)(support[0]*size),(int)(support[1]*size)),5,CV_RGB(200,200,200));
	}

	//显示结果图
	cvShowImage("SVM result",pImg);
	cvWaitKey(0);
	//释放资源
	cvReleaseImage(&pImg);

}