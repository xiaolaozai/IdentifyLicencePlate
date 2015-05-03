#include "stdafx.h"
#include "IplImageProcessing.h"

//ͼƬ����

IplImageProcessing::IplImageProcessing(void)
{
}

IplImageProcessing::~IplImageProcessing(void)
{
}

/****************************************
myRGB2Gray
RGBת�Ҷ�ͼ
src
ԴͼƬ
dst
��ͨ��Ŀ��Ҷ�ͼƬ
******************************************/
IplImage* IplImageProcessing::myRGB2Gray(IplImage* src,IplImage* dst)
{
	if(src==NULL)
	{
		cout<<"myRGB2Gray image source is NULL"<<endl;
		return NULL;
	}

	for(int i=0;i<src->height;i++)
	{
		for(int j=0;j<src->width;j++)
		{
			CV_IMAGE_ELEM(dst,unsigned char,i,j)=(unsigned char)(CV_IMAGE_ELEM(src,unsigned char,i,j*3)*0.114
			+CV_IMAGE_ELEM(src,unsigned char,i,j*3+1)*0.587+CV_IMAGE_ELEM(src,unsigned char,i,j*3+2)*0.299);
		}
	}
	return dst;
}

/****************************************
myGrayStretch
�Ҷ�����
src
��ͨ��Դ�Ҷ�ͼƬ
threshold1
��ͷ�ֵ,��0-255��
threshold2
��߷�ֵ,��0-255��
threshold1<threshold2
******************************************/
IplImage* IplImageProcessing::myGrayStretch(IplImage* src,int threshold1,int threshold2)
{
	if(src==NULL)
	{
		cout<<"myGrayStretch image source is NULL"<<endl;
		return NULL;
	}
	if(threshold1<0||threshold1>255||threshold2<0||threshold2>255)
	{
		threshold1=0;
		threshold2=255;
	}
	//��С����
	if(threshold1>threshold2)
	{
		threshold1 = threshold1^threshold2;
		threshold2 = threshold1^threshold2;
		threshold1 = threshold1^threshold2;
	}

	for(int i=0;i<src->height;i++)
	{
		for(int j=0;j<src->width;j++)
		{
			int gxy=CV_IMAGE_ELEM(src,uchar,i,j);
			if(gxy<=threshold1)
			{
				CV_IMAGE_ELEM(src,uchar,i,j)=0;
			}
			else if(gxy>=threshold2)
			{
				CV_IMAGE_ELEM(src,uchar,i,j)=255;
			}
			else
			{
				CV_IMAGE_ELEM(src,uchar,i,j)=(gxy-threshold1)*255/(threshold2-threshold1);
			} 

		}
	}
	return src;
}


/****************************************
myDilate
��̬ѧ����

src
��ͨ��Դ�Ҷ�ͼƬ

nVer
��ֱ��ֵ

nVerTimes
��ֱ���ʹ���

nHor
ˮƽ��ֵ

nHorTimes
ˮƽ���ʹ���
******************************************/
IplImage* IplImageProcessing::myDilate(IplImage* src,int nVer,int nVerTimes,int nHor,int nHorTimes)
{
	if(src==NULL)
	{
		cout<<"myDilate image source is NULL"<<endl;
		return NULL;
	}
	IplConvKernel* VerStruct=cvCreateStructuringElementEx(1,nVer,0,nVer/2,CV_SHAPE_RECT);
	IplConvKernel* HorStruct=cvCreateStructuringElementEx(nHor,1,nHor/2,0,CV_SHAPE_RECT);

	cvDilate(src,src,VerStruct,nVerTimes);//��ֱ����

	cvDilate(src,src,HorStruct,nHorTimes);//ˮƽ����

	//�ͷ���Դ
	cvReleaseStructuringElement(&VerStruct);
	cvReleaseStructuringElement(&HorStruct);
	return src;
}


/****************************************
myErode
��̬ѧ��ʴ

src
��ͨ��Դ�Ҷ�ͼƬ

nVer
��ֱ��ֵ

nVerTimes
��ֱ��ʴ����

nHor
ˮƽ��ֵ

nHorTimes
ˮƽ��ʴ����
******************************************/
IplImage* IplImageProcessing::myErode(IplImage* src,int nVer,int nVerTimes,int nHor,int nHorTimes)
{
	if(src==NULL)
	{
		cout<<"myErode image source is NULL"<<endl;
		return NULL;
	}
	IplConvKernel* VerStruct=cvCreateStructuringElementEx(1,nVer,0,nVer/2,CV_SHAPE_RECT);
	IplConvKernel* HorStruct=cvCreateStructuringElementEx(nHor,1,nHor/2,0,CV_SHAPE_RECT);

	cvErode(src,src,VerStruct,nVerTimes);//��ֱ��ʴ
	//cvShowImage("cvErode_y",src);

	cvErode(src,src,HorStruct,nHorTimes);//ˮƽ��ʴ
	//cvShowImage("cvErode_x",src);

	//�ͷ���Դ
	cvReleaseStructuringElement(&VerStruct);
	cvReleaseStructuringElement(&HorStruct);
	return src;
}

/****************************************
myThreshold
��ֵ��

src
��ͨ��Դ�Ҷ�ͼƬ

type
ȡ�����ͣ�0��ʾ������1��ʾȡ��
******************************************/
IplImage* IplImageProcessing::myThreshold(IplImage* src,int type)
{
	if(src==NULL)
	{
		cout<<"myThreshold image source is NULL"<<endl;
		return NULL;
	}
	//ȫ�ֶ�ֵ��
	//double min=255,max=0;
	//cvMinMaxLoc(src,&min,&max);
	//double threshold=(min+max)/2;
	//cvThreshold(src,src,threshold,255,CV_THRESH_BINARY);

	int min=255,max=0;
	for(int i=0;i<src->height;i++)
	{
		for(int j=0;j<src->width;j++)
		{
			if(CV_IMAGE_ELEM(src,unsigned char,i,j)>max)
			{
				max=CV_IMAGE_ELEM(src,unsigned char,i,j);
			}
			else if(CV_IMAGE_ELEM(src,unsigned char,i,j)<min)
			{
				min=CV_IMAGE_ELEM(src,unsigned char,i,j);
			}
		}
	}

	int threshold=(max+min)/2;
	for(int i=0;i<src->height;i++)
	{
		for(int j=0;j<src->width;j++)
		{
			if(type)
			{
				if(CV_IMAGE_ELEM(src,unsigned char,i,j)>threshold)
				{
					CV_IMAGE_ELEM(src,unsigned char,i,j)=0;
				}
				else
				{
					CV_IMAGE_ELEM(src,unsigned char,i,j)=255;
				}
			}
			else
			{
				if(CV_IMAGE_ELEM(src,unsigned char,i,j)>threshold)
				{
					CV_IMAGE_ELEM(src,unsigned char,i,j)=255;
				}
				else
				{
					CV_IMAGE_ELEM(src,unsigned char,i,j)=0;
				}
			}
		}
	}

	//cvShowImage("myThreshold",src);

	return src;
}

/****************************************
myBaseAdaptiveThreshold
����Ӧ��Ҫ�ɷֶ�ֵ��

src
��ͨ��Դ�Ҷ�ͼƬ

******************************************/
IplImage* IplImageProcessing::myBaseAdaptiveThreshold(IplImage* src)
{
	if(src==NULL)
	{
		cout<<"myBaseAdaptiveThreshold image source is NULL"<<endl;
		return NULL;
	}

	int min=255,max=0;
	for(int i=0;i<src->height;i++)
	{
		for(int j=0;j<src->width;j++)
		{
			if(CV_IMAGE_ELEM(src,unsigned char,i,j)>max)
			{
				max=CV_IMAGE_ELEM(src,unsigned char,i,j);
			}
			else if(CV_IMAGE_ELEM(src,unsigned char,i,j)<min)
			{
				min=CV_IMAGE_ELEM(src,unsigned char,i,j);
			}
		}
	}

	int threshold=(max+min)/2;
	int count=0;
	for(int i=0;i<src->height;i++)
	{
		for(int j=0;j<src->width;j++)
		{
			if(CV_IMAGE_ELEM(src,unsigned char,i,j)>threshold)
			{
				count++;
			}
		}
	}
	double scale=count*1.0/(src->height*src->width);
	cout<<"scale="<<scale<<endl;
	if(scale>0.5)//���ڷ�ֵ����ɫ��Ϊ��Ҫ�ɷ֣���Ҫ�ɷ�Ҫ��Ϊ��ɫ��ɫ����ô�����ֵ��
	{
		for(int i=0;i<src->height;i++)
		{
			for(int j=0;j<src->width;j++)
			{
				if(CV_IMAGE_ELEM(src,unsigned char,i,j)>threshold)
				{
					CV_IMAGE_ELEM(src,unsigned char,i,j)=0;
				}
				else
				{
					CV_IMAGE_ELEM(src,unsigned char,i,j)=255;
				}
			}
		}
	}else//С�ڷ�ֵ����ɫ��Ϊ��Ҫ�ɷ֣���Ҫ�ɷ�Ҫ��Ϊ��ɫ��ɫ����ô�����ֵ��
	{
		for(int i=0;i<src->height;i++)
		{
			for(int j=0;j<src->width;j++)
			{
				if(CV_IMAGE_ELEM(src,unsigned char,i,j)>threshold)
				{
					CV_IMAGE_ELEM(src,unsigned char,i,j)=255;
				}
				else
				{
					CV_IMAGE_ELEM(src,unsigned char,i,j)=0;
				}
			}
		}
	}
	return src;
}

/****************************************
myThresholdCount
ͳ��С�ڻ���ڷ�ֵ�ĸ���

src
��ͨ��Դ�Ҷ�ͼƬ

threshold
��ֵ

scale
����

type
���ͣ�0��ʾͳ��С�ڵ��ڷ�ֵ�ĸ��ʣ�1��ʾͳ�ƴ��ڷ�ֵ�ĸ���

******************************************/
bool IplImageProcessing::isMyThresholdCount(IplImage* src,int threshold,double scale,int type)
{
	if(src==NULL)
	{
		cout<<"isMyThresholdCount image source is NULL"<<endl;
		return false;
	}
	if(threshold>255||threshold<0)
	{
		threshold=128;
	}

	int count=0;
	for(int i=0;i<src->height;i++)
	{
		for(int j=0;j<src->width;j++)
		{
			if(type==1&&CV_IMAGE_ELEM(src,unsigned char,i,j)>threshold)
			{
				count++;
			}
			if(type==0&&CV_IMAGE_ELEM(src,unsigned char,i,j)<=threshold)
			{
				count++;
			}
		}
	}
	if(count*1.0/(src->height*src->width)>=scale)
	{
		return true;
	}
	return false;
}

/****************************************
myCutImageLine
����ͼƬ�ж����߶�

src
��ͨ��Դ�Ҷ�ͼƬ

verMinCount
��ֱ�Ϸ�����������߶�

horMinCount
ˮƽ�Ϸ�����������߶�

type
���ͣ���ֱΪ0��,ˮƽ��Ϊ0

******************************************/
IplImage* IplImageProcessing::myCutImageLine(IplImage* src,int verMinCount,int horMinCount,int type)
{
	if(src==NULL)
	{
		cout<<"myCutImageLine image source is NULL"<<endl;
		return NULL;
	}

	int StartPoint_x=0,StartPoint_y=0,PointCount=0;
	bool flag=false;
	if(type==0)
	{
		//��ֱ�����̲�����
		for(int y=0;y<src->width;y++)
		{
			for(int x=0;x<src->height;x++)
			{
				if(CV_IMAGE_ELEM(src,unsigned char,x,y)>=200)//�������ֵ�ӽ���ɫ
				{
					PointCount++;
					if(!flag)
					{
						StartPoint_x=x;
						flag=true;
					}
				}
				else
				{
					if(PointCount<verMinCount)
					{
						for(int i=StartPoint_x;i<StartPoint_x+PointCount&&i<src->height;i++)
						{
							CV_IMAGE_ELEM(src,unsigned char,i,y)=0;//����Ϊ��ɫ
						}
					}

					PointCount=0;
					flag=false;
				}
			}
	
		}
		//cvShowImage("��ֱ������",src);
	}
	else
	{
		//ˮƽ�����̲�����
		PointCount=0;
		for(int x=0;x<src->height;x++)
		{
			for(int y=0;y<src->width;y++)
			{
				if(CV_IMAGE_ELEM(src,unsigned char,x,y)>=200)//�������ֵ�ӽ���ɫ
				{
					PointCount++;
					if(!flag)
					{
						StartPoint_y=y;
						flag=true;
					}
				}
				else
				{
					if(PointCount<horMinCount)
					{
						for(int j=StartPoint_y;j<StartPoint_y+PointCount&&j<src->width;j++)
						{
							CV_IMAGE_ELEM(src,uchar,x,j)=0;//����Ϊ��ɫ
						}
					}

					PointCount=0;
					flag=false;
				}
			}
	
		}
		//cvShowImage("ˮƽ������",src);
	}
	return src;
}

/****************************************
myCutImage
�и�ͼƬ

src
ԴͼƬ

dst
Ŀ��ͼƬ

rect
�и�Դͼ��Ŀ�����

******************************************/
IplImage* IplImageProcessing::myCutImage(IplImage* src,IplImage* dst,CvRect rect)
{
	if(src==NULL)
	{
		cout<<"myCutImage image source is NULL"<<endl;
		return NULL;
	}
	cvSetImageROI(src,rect);
	cvCopy(src,dst);
	cvResetImageROI(src);
	//cvShowImage("myCutImage",dst);
	return dst;
}

/****************************************
myRoughing
���ƴ�ѡ

src
ԴͼƬ

pImg_contour
3ͨ���Ҷ�ͼ���洢������Ϣ

list_rects
�洢����λ��

minX
����λ�ó��ֵ���С������λ��

minY
����λ�ó��ֵ���Сֱ����λ��

minWidth
��С��

minHeight
��С��

minK
��С����ȷ�ֵ

maxK
��󳤿�ȷ�ֵ

minArea
��С���
******************************************/
list<CvRect> IplImageProcessing::myRoughing(IplImage* src,IplImage* pImg_contour,list<CvRect> list_rects,double minX,double minY,double minWidth,double minHeight,double minK,double maxK,int minArea)
{
	if(src==NULL)
	{
		cout<<"myRoughing image source is NULL"<<endl;
		return list_rects;
	}
	if(minX>src->width||minX<0)
	{
		minX=src->width/4;
	}

	if(minY>src->height||minY<0)
	{
		minY=src->height/2;
	}

	CvMemStorage* sto=cvCreateMemStorage(0);//��������������Ϣ
	CvSeq* con=NULL;//�洢��һ�������ָ��
	int num=cvFindContours(src,sto,&con,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_NONE,cvPoint(0,0));
	//�ð�ɫ��������������
	//IplImage* pImg_contour=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,3);
	//��ʼ��Ϊ��ɫ
	cvZero(pImg_contour);
	//con=cvApproxPoly(con,sizeof(CvContour),sto,CV_POLY_APPROX_DP,3,1);//���ȱƽ�

	for(;con!=NULL;con=con->h_next)
	{
		CvRect rect=cvBoundingRect(con,0);
		//cvRectangle(pImg_contour,cvPoint(rect.x,rect.y),cvPoint(rect.x+rect.width,rect.y+rect.height),cvScalarAll(255));
		int tmparea=rect.height*rect.width;  
		if(rect.x>=minX&&rect.y>=minY)
		{
			if(rect.width>=minWidth&&rect.height>=minHeight)
			{
				if (((double)rect.width/(double)rect.height>minK)  
				&& ((double)rect.width/(double)rect.height<maxK)&& tmparea>=minArea)  
				{
					cvRectangle(pImg_contour,cvPoint(rect.x,rect.y),cvPoint(rect.x+rect.width,rect.y+rect.height),cvScalarAll(255),2);
					//cvRectangle(dst,cvPoint(aRect.x,aRect.y),cvPoint(aRect.x+aRect.width ,aRect.y+aRect.height),color,2);   
					//cvDrawContours(pImg_contour,con,cvScalarAll(255),cvScalarAll(0),0,CV_FILLED);
					list_rects.push_back(rect);//�������Ҫ��ĳ���λ��
				} 
			} 
		}
	}

	cvReleaseMemStorage(&sto);
	//cvReleaseImage(&pImg_contour);
	return list_rects;
}

/****************************************
myIsLicence
��ȷ��ȡ����

src
��ͨ��ԴͼƬ

subGray
�Ҷȷ�ֵ

x1
���п�ʼλ��

x2
���н���λ��

nX
�з�ֵ

y1
���п�ʼλ��

y2
���н���λ��

nY
�з�ֵ
******************************************/
bool IplImageProcessing::isMyLicence(IplImage* src,int subGray,int &x1,int &x2,int nX,int &y1,int &y2,int nY)
{
	bool flag_col_left=false;
	bool flag_col_right=false;
	bool flag_row_top=false;
	bool flag_row_bottom=false;
	x1=0;
	x2=src->width;
	y1=0;
	y2=src->height;

	//cout<<"x11="<<x1<<" x22="<<x2<<endl;
	//cout<<"y11="<<y1<<" y22="<<y2<<endl;

	if(src==NULL)
	{
		cout<<"isMyLicence image source is NULL"<<endl;
		return false;
	}
	//��ѯ����
	for(int j=0;j<src->width/2&&!flag_col_left;j++)
	{
		int count=0;
		for(int i=0;i<src->height;i++)
		{   
			if(abs(CV_IMAGE_ELEM(src,unsigned char,i,j)-CV_IMAGE_ELEM(src,unsigned char,i,j+1))>=subGray)
			{
				count++;
			}
			if(count>=nY)
			{
				x1 = j;
				flag_col_left=true;
			}
		}
	}
	//��ѯ����
	for(int j=src->width-1;j>src->width/2&&!flag_col_right;j--)
	{
		int count=0;
		for(int i=0;i<src->height;i++)
		{   
			if(abs(CV_IMAGE_ELEM(src,unsigned char,i,j)-CV_IMAGE_ELEM(src,unsigned char,i,j-1))>=subGray)
			{
				count++;
			}
			if(count>=nY)
			{
				x2 = j;
				flag_col_right=true;
			}
		}
	}

	//��ѯ����
	for(int i=0;i<src->height/2&&!flag_row_top;i++)
	{
		int count=0;
		for(int j=0;j<src->width;j++)
		{   
			if(abs(CV_IMAGE_ELEM(src,unsigned char,i,j)-CV_IMAGE_ELEM(src,unsigned char,i+1,j))>=subGray)
			{
				count++;
			}
			if(count>=nX)
			{
				y1 = i;
				flag_row_top=true;
			}
		}
	}

	//��ѯ����
	for(int i=src->height-1;i<src->height/2&&!flag_row_bottom;i--)
	{
		int count=0;
		for(int j=0;j<src->width;j++)
		{   
			if(abs(CV_IMAGE_ELEM(src,unsigned char,i,j)-CV_IMAGE_ELEM(src,unsigned char,i-1,j))>=subGray)
			{
				count++;
			}
			if(count>=nX)
			{
				y2 = i;
				flag_row_bottom=true;
			}
		}
	}

	//�����Ϊ440:140=22:7
	double w=x2-x1;
	double h=y2-y1;

	//cout<<"x1="<<x1<<" x2="<<x2<<endl;
	//cout<<"y1="<<y1<<" y2="<<y2<<endl;
	//cout<<"w/h="<<w/h<<endl;

	if(w/h>=3.0&&w/h<=3.5)
	{
		//cout<<"find it"<<endl;
		return true;
	}
	return false;
}

/****************************************
myExtractRGB
RGB��ɫ��ȡ

src
ԴͼƬ

colorType
��ɫ���� -1,0,1,2,3�ֱ����������ף��Ƶף��׵ף��ڵ�

blueThreshold
���׷�ֵ

yellowThreshold
�Ƶ׷�ֵ

whiteThreshold
�׵׷�ֵ

blackThreshold
�ڵ׷�ֵ
******************************************/
void IplImageProcessing::myExtractRGB(IplImage* src,int &colorType,float blueThreshold,float yellowThreshold,float whiteThreshold,float blackThreshold)
{
	if(src==NULL)
	{
		cout<<"myExtractRGB image source is NULL"<<endl;
		colorType=-1;
		return;
	}
	int blueCount=0,yellowCount=0,whiteCount=0,blackCount=0;
	float blue_scale=0,yellow_scale=0,white_scale=0,black_scale=0;
	int cPointB,cPointG,cPointR;
	//cout<<"src->height="<<src->height<<" src->width="<<src->width<<endl;

	for(int x=0;x<src->height;x++)
	{
		for(int y=0;y<src->width;y++)
		{
			cPointR = CV_IMAGE_ELEM(src, unsigned char, x, y * 3 + 2);
			cPointG = CV_IMAGE_ELEM(src, unsigned char, x, y * 3 + 1);
			cPointB = CV_IMAGE_ELEM(src, unsigned char, x, y * 3);
			if(cPointB-cPointR>=40&&cPointB-cPointG>=40)    //��ȡ��ɫ 
			{  
					blueCount++;
			}  
			else if(cPointB>=180&&cPointG>=180&&cPointR>=180)  //��ȡ��ɫ  
			{  
					whiteCount++;
			}
			else if(cPointB<=30&&cPointG<=30&&cPointR<=30)  //��ȡ��ɫ
			{  
					blackCount++;
			}
			else if((cPointR>=130)&&(cPointG>=130)&&(cPointB<=70))  //��ȡ��ɫ
			{  
					yellowCount++;
			} 
		}
		cout<<endl;
	}

	blue_scale=blueCount*1.0/(src->height*src->width);
	white_scale=whiteCount*1.0/(src->height*src->width);
	black_scale=blackCount*1.0/(src->height*src->width);
	yellow_scale=yellowCount*1.0/(src->height*src->width);

	if(blue_scale>yellow_scale&&blue_scale>=blueThreshold&&white_scale>=whiteThreshold)//���װ���
	{
		colorType=0;
		return;
	}

	if(yellow_scale>blue_scale&&yellow_scale>=yellowThreshold&&black_scale>=blackThreshold)//�Ƶ׺���
	{
		colorType=1;
		return;
	}

	if(white_scale>black_scale&&white_scale>=whiteThreshold&&black_scale>=blackThreshold)//�׵׺���
	{
		colorType=2;
		return;
	}

	if(black_scale>white_scale&&white_scale>=whiteThreshold&&black_scale>=blackThreshold)//�ڵװ���
	{
		colorType=3;
		return;
	}

	//��������
	colorType=-1;

	//cout<<"blueCount:"<<blueCount<<" Count:"<<src->height*src->width<<" scale:"<<blue<<endl;
	//cout<<"whiteCount:"<<whiteCount<<" Count:"<<src->height*src->width<<" scale:"<<white<<endl;
	//cout<<"blackCount:"<<blackCount<<" Count:"<<src->height*src->width<<" scale:"<<black<<endl;
	//cout<<"yellowCount:"<<yellowCount<<" Count:"<<src->height*src->width<<" scale:"<<yellow<<endl;

	//cout<<endl;

	//cvShowImage("src",src);
}

/****************************************
myExtractHSV
HSV��ɫ��ȡ

src
ԴͼƬ

colorType
��ɫ���� -1,0,1,2,3�ֱ����������ף��Ƶף��׵ף��ڵ�

******************************************/
void IplImageProcessing::myExtractHSV(IplImage* src,int &colorType)
{
	if(src==NULL)
	{
		cout<<"myExtractHSV image source is NULL"<<endl;
		colorType=-1;
		return;
	}

	IplImage* pImg_hsv=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,3);
	IplImage* pImg_h=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
	IplImage* pImg_s=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
	IplImage* pImg_v=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);

	IplImage *dst_blue = cvCreateImage(cvGetSize(src), src->depth, 1); //��ɫ
	IplImage* pImg_tmH1_blue=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
	IplImage* pImg_tmS1_blue=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
	IplImage* pImg_tmV1_blue=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);

	IplImage *dst_yellow = cvCreateImage(cvGetSize(src), src->depth, 1); //��ɫ
	IplImage* pImg_tmH1_yellow=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
	IplImage* pImg_tmS1_yellow=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
	IplImage* pImg_tmV1_yellow=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);

	IplImage *dst_white = cvCreateImage(cvGetSize(src), src->depth, 1);//��ɫ
	IplImage* pImg_tmH1_white=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
	IplImage* pImg_tmS1_white=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
	IplImage* pImg_tmV1_white=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);

	IplImage *dst_black = cvCreateImage(cvGetSize(src), src->depth, 1);//��ɫ
	IplImage* pImg_tmH1_black=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
	IplImage* pImg_tmS1_black=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
	IplImage* pImg_tmV1_black=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);

	IplImage* pImg_tmSrc=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,3);
	
	
	//��˹�˲�
	//cvSmooth(src,pImg_tmSrc,CV_GAUSSIAN,3,3);
	//��ֵ�˲�
	//cvSmooth(src,pImg_tmSrc,CV_MEDIAN,3,3);
	//˫���˲�
	cvSmooth(src,pImg_tmSrc,CV_BILATERAL,3,3);

	// hueɫ�ȣ�saturation���Ͷȣ�value����  
	cvCvtColor(pImg_tmSrc,pImg_hsv,CV_BGR2HSV);//ת��HSV
	cvSplit(pImg_hsv,pImg_h,pImg_s,pImg_v,NULL);

	//Ĭ��H:0-179;S:0-255;V:0-255
	//������ɫ���֣���H=240/2����ɫ����H=0����ɫ����H=120/2����ɫ��,H=60/2(��ɫ)
	//V=0�����ɫ;V=1,S=0�����ɫ
	//����(v)С��25%Ϊ��ɫ��������(v)����75%���ұ��Ͷ�(s)С��20%Ϊ��ɫ����

	//��ɫ//h:190-245 s:0.35-1 v:0.3-1
	cvInRangeS(pImg_h,cvScalar(100.0,0.0,0.0),cvScalar(140.0,0.0,0.0),pImg_tmH1_blue);//��ɫ
	cvInRangeS(pImg_s,cvScalar(64.0,0.0,0.0),cvScalar(191.0,0.0,0.0),pImg_tmS1_blue);
	cvAnd(pImg_tmH1_blue,pImg_tmS1_blue,pImg_tmH1_blue,0);

	Mat mat_result_blue=pImg_tmH1_blue;
	Scalar v_blue=sum(mat_result_blue);

	//cout<<"pImg_tmH1_blue total piexls:"<<pImg_tmH1_blue->imageSize<<endl;
	cout<<"blue:"<<v_blue[0]/255<<endl;
	//cout<<"blue scale:"<<v_blue[0]/255/pImg_tmH1_blue->imageSize<<endl;

	//float blue_scale=v_blue[0]/255/pImg_tmH1_blue->imageSize;

	//cvCopy(pImg_tmV1_blue,dst_blue);

	//��ɫ
	cvInRangeS(pImg_h,cvScalar(10.0,0.0,0.0),cvScalar(50.0,0.0,0.0),pImg_tmH1_yellow);//��ɫ
	cvInRangeS(pImg_s,cvScalar(64.0,0.0,0.0),cvScalar(191.0,0.0,0.0),pImg_tmS1_yellow);
	cvAnd(pImg_tmH1_yellow,pImg_tmS1_yellow,pImg_tmH1_yellow,0);

	Mat mat_result_yellow=pImg_tmH1_yellow;
	Scalar v_yellow=sum(mat_result_yellow);
	cout<<"yellow:"<<v_yellow[0]/255<<endl;
	//cout<<"yellow scale:"<<v_yellow[0]/255/pImg_tmH1_blue->imageSize<<endl;

	//float yellow_scale=v_yellow[0]/255/pImg_tmH1_yellow->imageSize;

	//cvCopy(pImg_tmV1_yellow,dst_yellow);

	//��ɫ
	cvInRangeS(pImg_v,cvScalar(220.0,0.0,0.0),cvScalar(255.0,0.0,0.0),pImg_tmV1_white);//��ɫ
	cvInRangeS(pImg_s,cvScalar(0.0,0.0,0.0),cvScalar(64.0,0.0,0.0),pImg_tmS1_white);
	cvAnd(pImg_tmV1_white,pImg_tmS1_white,pImg_tmV1_white,0);

	Mat mat_result_white=pImg_tmV1_white;
	Scalar v_white=sum(mat_result_white);
	cout<<"white:"<<v_white[0]/255<<endl;
	//cout<<"white scale:"<<v_white[0]/255/pImg_tmH1_blue->imageSize<<endl;

	//float white_scale=v_white[0]/255/pImg_tmV1_white->imageSize;

	//cvCopy(pImg_tmV1_white,dst_white);

	//��ɫ
	cvInRangeS(pImg_v,cvScalar(0.0,0.0,0.0),cvScalar(50.0,0.0,0.0),pImg_tmV1_black);//��ɫ
	cvInRangeS(pImg_s,cvScalar(0.0,0.0,0.0),cvScalar(255.0,0.0,0.0),pImg_tmS1_black);
	cvAnd(pImg_tmV1_black,pImg_tmS1_black,pImg_tmV1_black,0);

	Mat mat_result_black=pImg_tmV1_black;
	Scalar v_black=sum(mat_result_black);
	cout<<"black:"<<v_black[0]/255<<endl;
	//cout<<"black scale:"<<v_black[0]/255/pImg_tmH1_blue->imageSize<<endl;

	//float black_scale=v_black[0]/255/pImg_tmV1_black->imageSize;

	//cvCopy(pImg_tmV1_black,dst_black);

	//cvShowImage("src",src);
	//cvShowImage("dst_blue",dst_blue);
	//cvShowImage("dst_yellow",dst_yellow);
	//cvShowImage("dst_white",dst_white);
	//cvShowImage("dst_black",dst_black);

	//cvWaitKey(0);

	cvReleaseImage(&pImg_tmSrc);

	cvReleaseImage(&pImg_tmV1_blue);
	cvReleaseImage(&pImg_tmS1_blue);
	cvReleaseImage(&pImg_tmH1_blue);
	cvReleaseImage(&dst_blue);

	cvReleaseImage(&pImg_tmV1_yellow);
	cvReleaseImage(&pImg_tmS1_yellow);
	cvReleaseImage(&pImg_tmH1_yellow);
	cvReleaseImage(&dst_yellow);

	cvReleaseImage(&pImg_tmV1_white);
	cvReleaseImage(&pImg_tmS1_white);
	cvReleaseImage(&pImg_tmH1_white);
	cvReleaseImage(&dst_white);

	cvReleaseImage(&pImg_tmV1_black);
	cvReleaseImage(&pImg_tmS1_black);
	cvReleaseImage(&pImg_tmH1_black);
	cvReleaseImage(&dst_black);

	cvReleaseImage(&pImg_v);
	cvReleaseImage(&pImg_s);
	cvReleaseImage(&pImg_h);
	cvReleaseImage(&pImg_hsv);

	//cout<<"v_blue[0]="<<v_blue[0]<<endl;
	//cout<<"v_yellow[0]="<<v_yellow[0]<<endl;
	//cout<<"v_white[0]="<<v_white[0]<<endl;
	//cout<<"v_black[0]="<<v_black[0]<<endl;

	if(v_blue[0]>v_yellow[0]&&v_blue[0]>v_white[0]&&v_blue[0]>v_black[0])//���װ���
	{
		colorType=0;
		return;
	}

	if(v_yellow[0]>v_blue[0]&&v_yellow[0]>v_white[0]&&v_yellow[0]>v_black[0])//�Ƶ׺���
	{
		colorType=1;
		return;
	}

	if(v_white[0]>v_blue[0]&&v_white[0]>v_yellow[0]&&v_white[0]>v_black[0])//�׵׺���
	{
		colorType=2;
		return;
	}

	if(v_black[0]>v_blue[0]&&v_black[0]>v_yellow[0]&&v_black[0]>v_white[0])//�ڵװ���
	{
		colorType=3;
		return;
	}

	//��������
	colorType=-1;
}

