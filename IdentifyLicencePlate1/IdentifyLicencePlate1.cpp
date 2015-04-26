// IdentifyLicencePlate1.cpp : 定义控制台应用程序的入口点。

#include "stdafx.h"
#include "FileOperation.h"
#include "IplImageProcessing.h"
#include "getSource.h"

int _tmain(int argc, _TCHAR* argv[])
{
	/**********************
	第一阶段：预处理
	开始
	**********************/
	IplImageProcessing* pImgProcess=new IplImageProcessing();//图片操作类
	getSource* pSrc=new getSource();//资源路径类

	IplImage* pImg_src=cvLoadImage(pSrc->getImgpath(),CV_LOAD_IMAGE_COLOR);
	IplImage* pImg_gray = cvCreateImage(cvGetSize(pImg_src), IPL_DEPTH_8U,1);
	pImg_gray=pImgProcess->myRGB2Gray(pImg_src,pImg_gray);
	cvShowImage("ORIGNAL_src",pImg_src);
	cvShowImage("ORIGNAL_gray",pImg_gray);

	//中值滤波
	IplImage* pImg_smooth = cvCreateImage(cvGetSize(pImg_src), IPL_DEPTH_8U,1);
	//cvSmooth(pImg_gray,pImg_smooth,CV_MEDIAN,3,3,0);
	//双向滤波
	cvSmooth(pImg_gray,pImg_smooth,CV_BILATERAL,3,3,0);
	//cvShowImage("ORIGNAL_smooth",pImg_smooth);

	//灰度拉伸
	IplImage* pImg_stretch = cvCreateImage(cvGetSize(pImg_src), IPL_DEPTH_8U,1);
	pImg_stretch=pImgProcess->myGrayStretch(pImg_smooth,20,240);
	//cvShowImage("灰度拉伸",pImg_stretch);

	//图像边缘检测
	IplImage* pImg_sobel = cvCreateImage(cvGetSize(pImg_src), IPL_DEPTH_8U,1);
	//cvSobel(pImg_stretch,pImg_sobel,0,1,3);//垂直检测
	//cvShowImage("cvSobel_y",pImg_sobel);
	cvSobel(pImg_stretch,pImg_sobel,2,0,3);//水平检测
	//cvShowImage("cvSobel_x",pImg_sobel);

	//形态学膨胀
	int nVer=5,nHor=8;
	IplImage* pImg_dilate = cvCreateImage(cvGetSize(pImg_src), IPL_DEPTH_8U,1);
	//pImg_stretch=pImgProcess->myDilate(pImg_sobel,nVer,1,nHor,0);//垂直膨胀
	//cvShowImage("cvDilate_y",pImg_stretch);
	pImg_stretch=pImgProcess->myDilate(pImg_sobel,nVer,0,nHor,1);//水平膨胀
	//cvShowImage("cvDilate_x",pImg_stretch);

	//全局二值化
	IplImage* pImg_threshold = cvCreateImage(cvGetSize(pImg_src), IPL_DEPTH_8U,1);
	pImg_threshold=pImgProcess->myThreshold(pImg_stretch);
	//cvShowImage("cvThreshold",pImg_threshold);

	////自适应二值化
	//IplImage* pImg_threshold = cvCreateImage(cvGetSize(pImg_src), IPL_DEPTH_8U,1);
	//cvThreshold(pImg_stretch,pImg_threshold,0,255,CV_THRESH_OTSU);
	//cvShowImage("cvThreshold",pImg_threshold);

	/**********************
	第一阶段：预处理
	结束
	**********************/

	/**********************
	第二阶段：车牌定位与提取
	开始
	**********************/

	//形态学定位
	list<CvRect> list_rects;//存储车牌位置
	IplImage* pImg_contour=cvCreateImage(cvGetSize(pImg_src),IPL_DEPTH_8U,3);//3通道灰度图，存储轮廓信息
	double minX=pImg_src->width/4,minY=pImg_src->height/2.5;//车牌位置出现的最小坐标位置(minX,minY)
	double minWidth=0,minHeight=0;//最小最大长宽比阀值
	double minK=2.0,maxK=5.5;//最小最大长宽比阀值
	int minArea=300;//目标位置最小面积
	list_rects=pImgProcess->myRoughing(pImg_threshold,pImg_contour,list_rects,minX,minY,minWidth,minHeight,minK,maxK,minArea);
	//cvShowImage("myRoughing",pImg_contour);

	//颜色定位
	//车牌颜色：蓝底白字，黄底黑字，黑底白字，白底黑字
	CvRect rect2;
	IplImage* pImg_selectColor=NULL;
	IplImage* pImg_selectGray=NULL;
	IplImage* pImg_selectThreshold=NULL;
	IplImage* pImg_selectLicence=NULL;
	int colorType=-1;//记录蓝底，黄底，黑底，白底
	bool colorFlag=false;//找到底色与否
	if(list_rects.empty())
	{
		cout<<"list_rects is empty()"<<endl;
		goto Release;//转跳去释放资源并结束
	}
	else
	{
		cout<<"list_rects is not empty()"<<endl;
	}

	while(!list_rects.empty()&&!colorFlag)//非空并且没找到底色
	{
		rect2=list_rects.front();
		pImg_selectColor=cvCreateImage(cvSize(rect2.width,rect2.height),IPL_DEPTH_8U,3);
		cvSetImageROI(pImg_src,rect2);
		cvCopyImage(pImg_src,pImg_selectColor);
		cvResetImageROI(pImg_src);
		cvShowImage("pImg_selectColor",pImg_selectColor);
		//寻找精确轮廓
		//灰度化
		pImg_selectGray=cvCreateImage(cvGetSize(pImg_selectColor), IPL_DEPTH_8U,1);
		pImg_selectGray=pImgProcess->myRGB2Gray(pImg_selectColor,pImg_selectGray);
		cvShowImage("pImg_selectGray",pImg_selectGray);

		//全局二值化
		pImg_selectThreshold = cvCreateImage(cvGetSize(pImg_selectColor), IPL_DEPTH_8U,1);
		pImg_selectThreshold=pImgProcess->myThreshold(pImg_selectGray);
		//cvThreshold(pImg_selectGray,pImg_selectThreshold,0,255,CV_THRESH_OTSU);
		cvShowImage("pImg_selectThreshold",pImg_selectThreshold);
		int x1=0,x2=0;
		int y1=0,y2=0;
		//cout<<"pImg_selectThreshold->height="<<pImg_selectThreshold->height<<" pImg_selectThreshold->width="<<pImg_selectThreshold->width<<endl;
		//cout<<"rect2.height="<<rect2.height<<" rect2.width="<<rect2.width<<endl;
		bool flagLicence=pImgProcess->isMyLicence(pImg_selectThreshold,240,x1,x2,pImg_selectThreshold->width/3,y1,y2,pImg_selectThreshold->height/2);
		if(flagLicence)//精确查到车牌时
		{
			int nHorK=2,nVerK=2;//行列缩小数
			int nWidth=x2-x1-2*nHorK,nHeight=y2-y1-2*nVerK;//行列宽高
			pImg_selectLicence=cvCreateImage(cvSize(nWidth,nHeight),IPL_DEPTH_8U,3);
			cvSetImageROI(pImg_src,cvRect(rect2.x+x1+nHorK,rect2.y+y1+nVerK,nWidth,nHeight));
			cvCopyImage(pImg_src,pImg_selectLicence);
			cvResetImageROI(pImg_src);
			cvShowImage("pImg_selectLicence",pImg_selectLicence);
			//寻找底色
			pImgProcess->myExtractHSV(pImg_selectLicence,colorType);//colorType!=-1，说明找到底色
			break;
		}
		list_rects.pop_front();
	}

	cout<<"车牌颜色类型：";
	switch(colorType)
	{
		case 0:
			cout<<"蓝牌"<<endl;
			break;
		case 1:
			cout<<"黄牌"<<endl;
			break;
		case 2:
			cout<<"白牌"<<endl;
			break;
		case 3:
			cout<<"黑牌"<<endl;
			break;
		default:
			cout<<"no found"<<endl;
			goto Release;//转跳去释放资源并结束
	}
	/**********************
	第二阶段：车牌定位
	结束
	**********************/

	/**********************
	第三阶段：车牌识别
	开始
	**********************/

	//字符切割
	IplImage* pImg_LicenceResize=NULL;
	IplImage* pImg_LicenceGray=NULL;
	IplImage* pImg_LicenceContour=NULL;
	IplImage* pImg_LicenceThreshold=NULL;
	IplImage* pImg_LicenceThreshold2=NULL;
	IplImage* pImg_LicenceChar[7];//存放车牌字符
	if(pImg_selectLicence!=NULL)
	{
		//归一化车牌图像大小
		int nRwidth=144,nRheight=33;//新的宽度与高度
		pImg_LicenceResize=cvCreateImage(cvSize(nRwidth,nRheight),IPL_DEPTH_8U,3);
		cvResize(pImg_selectLicence,pImg_LicenceResize,CV_INTER_LINEAR); //线性插值
		cvShowImage("pImg_LicenceResize",pImg_LicenceResize);

		//灰度化
		pImg_LicenceGray=cvCreateImage(cvGetSize(pImg_LicenceResize), IPL_DEPTH_8U,1);
		pImg_LicenceGray=pImgProcess->myRGB2Gray(pImg_LicenceResize,pImg_LicenceGray);
		cvShowImage("pImg_LicenceGray",pImg_LicenceGray);

		//自适应主成分二值化（主成分变成黑色，其他的为白色）
		pImg_LicenceThreshold = cvCreateImage(cvGetSize(pImg_LicenceResize), IPL_DEPTH_8U,1);
		pImg_LicenceThreshold2 = cvCreateImage(cvGetSize(pImg_LicenceResize), IPL_DEPTH_8U,1);
		//pImg_LicenceThreshold=pImgProcess->myThreshold(pImg_LicenceGray);
		//cvThreshold(pImg_LicenceGray,pImg_LicenceThreshold,140,255,CV_THRESH_OTSU);
		pImg_LicenceThreshold=pImgProcess->myBaseAdaptiveThreshold(pImg_LicenceGray);
		cvShowImage("pImg_LicenceThreshold",pImg_LicenceThreshold);
		//复制pImg_LicenceThreshold
		pImg_LicenceThreshold2=cvCloneImage(pImg_LicenceThreshold);
		cvShowImage("pImg_LicenceThreshold2",pImg_LicenceThreshold2);

		//形态学定位车牌字符
		list<CvRect> list_char_rects;//存储车牌字符位置
		pImg_LicenceContour=cvCreateImage(cvGetSize(pImg_LicenceResize),IPL_DEPTH_8U,3);//3通道灰度图，存储轮廓信息
		double minX=0,minY=0;//车牌位置出现的最小坐标位置(minX,minY)
		double minWidth=0,minHeight=nRheight*0.70;//最小宽和最小高
		double minK=0.0,maxK=33/20.0*1.35;//最小最大长宽比阀值
		int minArea=20;//目标位置最小面积
		list_char_rects=pImgProcess->myRoughing(pImg_LicenceThreshold,pImg_LicenceContour,list_char_rects,minX,minY,minWidth,minHeight,minK,maxK,minArea);
		cvShowImage("pImg_LicenceContour",pImg_LicenceContour);

		//对筛选出的字符从左到右进行排序，这里对CvRect进行新增比较定义
		list_char_rects.sort();
		//对第一个中文字符进行切割
		CvRect rect_char=list_char_rects.front();
		cout<<"x="<<rect_char.x<<" y="<<rect_char.y<<endl;
		cout<<"width="<<rect_char.width<<" height="<<rect_char.height<<endl;
		int min_x=6,min_y=3;
		int max_x=20,max_y=10;
		int k=0;
		const char* name_LicenceChar[]={"pImg_LicenceChar1","pImg_LicenceChar2","pImg_LicenceChar3","pImg_LicenceChar4","pImg_LicenceChar5","pImg_LicenceChar6","pImg_LicenceChar7"};
		if((7==list_char_rects.size()&&rect_char.x>min_x&&rect_char.x<max_x)||6==list_char_rects.size())//需要对第一个重新切割的情况
		{
			pImg_LicenceChar[k]=cvCreateImage(cvSize(20,25),IPL_DEPTH_8U,1);
			cvSetImageROI(pImg_LicenceThreshold2,cvRect(0,4,20,25));
			cvCopyImage(pImg_LicenceThreshold2,pImg_LicenceChar[k]);
			cvResetImageROI(pImg_LicenceThreshold2);
			cvShowImage(name_LicenceChar[k],pImg_LicenceChar[k]);
			if(7==list_char_rects.size())
			{
				list_char_rects.pop_front();
			}
			k++;
		}
		for(list<CvRect>::iterator iter=list_char_rects.begin();iter!=list_char_rects.end()&&k<7;iter++)
		{
			CvRect rect_temp=*iter;
			//if(k!=0)
			//{
			//	cout<<"x="<<rect_temp.x<<" y="<<rect_temp.y<<endl;
			//	cout<<"width="<<rect_temp.width<<" height="<<rect_temp.height<<endl;
			//}

			if(rect_temp.width<10)//遇到字符是1
			{
				int enX=5;
				if(rect_temp.x-enX+rect_temp.width+2*enX<pImg_LicenceThreshold2->width)
				{
					pImg_LicenceChar[k]=cvCreateImage(cvSize(rect_temp.width+2*enX,rect_temp.height),IPL_DEPTH_8U,1);
					cvSetImageROI(pImg_LicenceThreshold2,cvRect(rect_temp.x-enX,rect_temp.y,rect_temp.width+2*enX,rect_temp.height));
				}else
				{
					pImg_LicenceChar[k]=cvCreateImage(cvSize(rect_temp.width+1.5*enX,rect_temp.height),IPL_DEPTH_8U,1);
					cvSetImageROI(pImg_LicenceThreshold2,cvRect(rect_temp.x-enX,rect_temp.y,rect_temp.width+1.5*enX,rect_temp.height));
				}
			}else
			{
				pImg_LicenceChar[k]=cvCreateImage(cvSize(rect_temp.width,rect_temp.height),IPL_DEPTH_8U,1);
				cvSetImageROI(pImg_LicenceThreshold2,rect_temp);
			}
			cvCopyImage(pImg_LicenceThreshold2,pImg_LicenceChar[k]);
			cvResetImageROI(pImg_LicenceThreshold2);
			cvShowImage(name_LicenceChar[k],pImg_LicenceChar[k]);
			k++;
		}
	}


	//字符识别



	/**********************
	第三阶段：车牌识别
	结束
	**********************/

	//释放资源模块
	Release:

	cvWaitKey(0);
	if(pImg_selectColor!=NULL)
	{
		cvReleaseImage(&pImg_selectColor);
	}
	if(pImg_selectGray!=NULL)
	{
		cvReleaseImage(&pImg_selectGray);
	}
	if(pImg_selectThreshold!=NULL)
	{
		cvReleaseImage(&pImg_selectThreshold);
	}
	if(pImg_selectLicence!=NULL)
	{
		cvReleaseImage(&pImg_selectLicence);
	}
	if(pImg_LicenceResize!=NULL)
	{
		cvReleaseImage(&pImg_LicenceResize);
	}
	if(pImg_LicenceGray!=NULL)
	{
		cvReleaseImage(&pImg_LicenceGray);
	}
	if(pImg_LicenceThreshold!=NULL)
	{
		cvReleaseImage(&pImg_LicenceThreshold);
	}
	if(pImg_LicenceThreshold2!=NULL)
	{
		cvReleaseImage(&pImg_LicenceThreshold2);
	}
	if(pImg_LicenceContour!=NULL)
	{
		cvReleaseImage(&pImg_LicenceContour);
	}

	for(int i=0;i<7;i++)
	{
		if(pImg_LicenceChar[i]!=NULL)
		{
			cvReleaseImage(&pImg_LicenceChar[i]);
		}
	}
	cvReleaseImage(&pImg_contour);
	cvReleaseImage(&pImg_threshold);
	cvReleaseImage(&pImg_dilate);
	cvReleaseImage(&pImg_sobel);
	cvReleaseImage(&pImg_stretch);
	cvReleaseImage(&pImg_smooth);
	cvReleaseImage(&pImg_gray);
	cvReleaseImage(&pImg_src);
	delete pSrc;
	delete pImgProcess;
	return 0;
}

