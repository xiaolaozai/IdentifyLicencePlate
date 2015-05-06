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
getLicencePosition
ͨ����ɫֱ��ͼ��ȡ���ƾ�ȷλ�ã�x0,y0��,(x1,y1)

pImg_src
��ֵ��ԴͼƬ

x0
�п�ʼλ��

x1
�н���λ��

xV
����С�ָ�ֵ

xT
�зָֵ

y0
�п�ʼλ��

y1
�н���λ��

yV
����С�ָ�ֵ

yT
�зָֵ
******************************************/
void PlateChar::getLicencePosition(IplImage* pImg_src,int &x0,int &x1, float xV, float xT,int &y0,int &y1,float yV,float yT)
{
	Mat mat_src=pImg_src;
	Mat mat_hist_x=getHistogram(mat_src,1,1);
	Mat mat_hist_y=getHistogram(mat_src,0,1);
	float value_min=0.10;
	float value_pre,value_cur,value_nex;
	float xs=0.20,ys=0.30;
	int count=0,minCount=3;

	cout<<"xV:"<<xV<<endl;
	cout<<"xT:"<<xT<<endl;
	cout<<"mat_hist_y.cols*xs:"<<mat_hist_y.cols*xs<<endl;
	value_pre=mat_hist_y.at<float>(0,0);
	for(int j=1;j<=mat_hist_y.cols*xs;j++)//����������
	{
		value_cur=mat_hist_y.at<float>(0,j);
		if(value_pre>=xV||value_cur>=xV)
		{
			if(abs(value_pre-value_cur)>=xT)
			{
				x0=j;
				cout<<"abs(value_pre-value_cur):"<<abs(value_pre-value_cur)<<endl;
				//if(value_cur<=value_min||value_cur>=xV)
				//{
				//	continue;
				//};
				break;
			}
		}
		if(value_pre<=value_min)
		{
			count++;
			if(count>=minCount)//һ����
			{
				minCount++;
				x0=j;
				cout<<"value_cur:"<<value_cur<<endl;
			}
		}
		else
		{
			count=0;
		}
		//cout<<"value_cur:"<<value_cur<<endl;
		//cout<<"value_pre:"<<value_pre<<endl;
		value_pre=value_cur;
	}
	
	cout<<"mat_hist_y.cols*(1-xs):"<<mat_hist_y.cols*(1-xs)<<endl;
	value_nex=mat_hist_y.at<float>(0,mat_hist_y.cols-1);
	count=0;minCount=3;
	for(int j=mat_hist_y.cols-2;j>=mat_hist_y.cols*(1-xs);j--)//����������
	{
		value_cur=mat_hist_y.at<float>(0,j);
		if(value_cur>=xV||value_nex>=xV)
		{
			if(abs(value_cur-value_nex)>=xT)
			{
				x1=j+1;
				cout<<"abs(value_cur-value_nex):"<<abs(value_cur-value_nex)<<endl;
				//if(value_cur<=value_min||value_cur>=xV)
				//{
				//	continue;
				//};
				break;
			}
		/*	cout<<"value_cur:"<<value_cur<<endl;*/
		}
		if(value_nex<=value_min)
		{
			count++;
			if(count>=minCount)//һ����
			{
				minCount++;
				x1=j;
				cout<<"value_cur:"<<value_cur<<endl;
			}
		}
		else
		{
			count=0;
		}
		value_nex=value_cur;
	}
	

	cout<<"yV:"<<yV<<endl;
	cout<<"yT:"<<yT<<endl;
	cout<<"mat_hist_x.cols*ys:"<<mat_hist_x.cols*ys<<endl;
	value_pre=mat_hist_x.at<float>(0,0);
	count=0;minCount=3;
	for(int j=1;j<=mat_hist_x.cols*ys;j++)//������
	{
		value_cur=mat_hist_x.at<float>(0,j);
		if(value_pre>=yV||value_cur>=yV)
		{
			if(abs(value_pre-value_cur)>=yT)
			{
				y0=j;
				cout<<"abs(value_pre-value_cur):"<<abs(value_pre-value_cur)<<endl;
				break;
			}
		/*	cout<<"abs(value_pre-value_cur):"<<abs(value_pre-value_cur)<<endl;*/
		}
		if(value_pre<=value_min)
		{
			count++;
			if(count>=minCount)//һ����
			{
				minCount++;
				y0=j;
				cout<<"value_cur:"<<value_cur<<endl;
			}
		}
		else
		{
			count=0;
		}
		//cout<<"value_cur:"<<value_cur<<endl;
		//cout<<"value_pre:"<<value_pre<<endl;
		value_pre=value_cur;
	}
	
	cout<<"mat_hist_x.cols*(1-ys):"<<mat_hist_x.cols*(1-ys)<<endl;
	value_nex=mat_hist_x.at<float>(0,mat_hist_x.cols-1);
	count=0;minCount=3;
	for(int j=mat_hist_x.cols-2;j>=mat_hist_x.cols*(1-ys);j--)//������
	{
		value_cur=mat_hist_x.at<float>(0,j);
		if(value_cur>=yV||value_nex>=yV)
		{
			if(abs(value_cur-value_nex)>=yT)
			{
				y1=j;
				cout<<"abs(value_cur-value_nex):"<<abs(value_cur-value_nex)<<endl;
				break;
			}
	/*		cout<<"abs(value_cur-value_nex):"<<abs(value_cur-value_nex)<<endl;*/
		}
		if(value_nex<=value_min)
		{
			count++;
			if(count>=minCount)//һ����
			{
				minCount++;
				y1=j;
				cout<<"value_cur:"<<value_cur<<endl;
			}
		}
		else
		{
			count=0;
		}
		//cout<<"value_nex:"<<value_nex<<endl;
		//cout<<"value_cur:"<<value_cur<<endl;
		value_nex=value_cur;
	}
}

/****************************************
getPlatePosition
ͨ����ɫֱ��ͼ��ȡ���ƾ�ȷλ�ã�x0,y0��,(x1,y1)

pImg_src
��ֵ��ԴͼƬ

x0
�п�ʼλ��

x1
�н���λ��

xT
�зָֵ

y0
�п�ʼλ��

y1
�н���λ��

yT
�зָֵ
******************************************/
void PlateChar::getPlatePosition(IplImage* pImg_src,int &x0,int &x1, float xT,int &y0,int &y1,float yT)
{
	Mat mat_src=pImg_src;
	Mat mat_hist_x=getHistogram(mat_src,1,1);
	Mat mat_hist_y=getHistogram(mat_src,0,1);
	int maxNum=3,count=0;
	bool flag=false;
	for(int j=3;j<(int)mat_hist_y.cols*0.2;j++)//����������
	{
		float value=mat_hist_y.at<float>(0,j);
		if(value>xT)
		{
			if(!flag)
			{
				x0=j;
				cout<<"mat_hist_y.cols*0.3="<<mat_hist_y.cols*0.3<<endl;
				cout<<"mat_hist_y xT="<<xT<<endl;
				cout<<"mat_hist_y x0 value="<<value<<endl;
				cout<<"mat_hist_y x0="<<x0<<endl;
				flag=true;
			}
			if(value==1)
			{
				count++;	
			}
			else
			{
				count=0;
			}
			if(count>=maxNum)
			{
				maxNum++;
				x0=j;
				cout<<"mat_hist_y x0 value="<<value<<endl;
				cout<<"mat_hist_y x0="<<x0<<endl;
				
			}
		}
	}
	flag=false;count=0;maxNum=3;
	for(int j=mat_hist_y.cols-4;j>=(int)mat_hist_y.cols*0.8;j--)//����������
	{
		float value=mat_hist_y.at<float>(0,j);
		if(value>xT)
		{
			if(!flag)
			{
				x1=j;
				cout<<"mat_hist_y.cols*0.8="<<mat_hist_y.cols*0.8<<endl;
				cout<<"mat_hist_y x1 value="<<value<<endl;
				cout<<"mat_hist_y x1="<<x1<<endl;
				flag=true;
			}
			if(value==1)
			{
				count++;	
			}
			else
			{
				count=0;
			}
			if(count>=maxNum)
			{
				maxNum++;
				x1=j;
				cout<<"mat_hist_y x1 value="<<value<<endl;
				cout<<"mat_hist_y x1="<<x1<<endl;
			}
		}
	}

	flag=false;
	for(int j=2;j<mat_hist_x.cols;j++)
	{
		float value=mat_hist_x.at<float>(0,j);
		if(value>yT)
		{
			if(!flag)
			{
				y0=j;//������
				flag=true;
				cout<<"mat_hist_x yT="<<yT<<endl;
				cout<<"mat_hist_x y0 value="<<value<<endl;
				cout<<"mat_hist_x y0="<<y0<<endl;
			}

			if(value>0.95)//�ر���
			{
				if(j<mat_hist_x.cols*0.3)//����λ��
				{
					y0=j+1;
					cout<<"mat_hist_x y0 value="<<value<<endl;
					cout<<"mat_hist_x y0="<<y0<<endl;
				}
				if(j>mat_hist_x.cols*0.7)//����λ��
				{
					cout<<"mat_hist_x y1 value="<<value<<endl;
					cout<<"mat_hist_x y1="<<y1<<endl;
					break;
				}
			}
			y1=j;//��������������ķ���һ����������
		}
		else
		{
			if(flag&&j>mat_hist_x.cols*0.7) 
			{
				cout<<"mat_hist_x y1 value="<<value<<endl;
				cout<<"mat_hist_x y1="<<y1<<endl;
				break;
			}
		}
	}
}

/****************************************
showHistogram
չʾ��ɫֱ��ͼ

pImg_src
��ֵ��ԴͼƬ

pImg_hist
���ͼ

type
�������ͣ�1��ʾ�У�0��ʾ��
******************************************/
IplImage* PlateChar::showHistogram(IplImage* pImg_src,IplImage* pImg_hist,int type)
{
	Mat mat_src=pImg_src;
	Mat mat_hist=getHistogram(mat_src,type,1);
	int size=(type)?mat_src.rows:mat_src.cols;
	//չʾ����
	for(int j=0;j<size;j++)
	{
		cout<<mat_hist.at<float>(0,j)<<" ";
	}
	cout<<endl;

	cvSet(pImg_hist,cvScalarAll(255),0);  
	
	for(int i=0;i<size;i++)  
	{  
			CvPoint p0=cvPoint(i,0);  
			CvPoint p1=cvPoint((i+1),mat_hist.at<float>(0,i)*100);  
			cvRectangle(pImg_hist,p0,p1,cvScalar(0,0,255));  
	}  

	return pImg_hist;
}

/****************************************
getHistogram
��ȡ�л��е���ɫֱ��ͼ

mat_src
ԴͼƬ����

xyType
�������ͣ���0��ʾ�У�0��ʾ��

modelType
ģ�����ͣ���0��ʾ��׼���ͣ������ֵ��һ������0��ʾ��ͨ���ͣ���ԭͼ��size��һ����
******************************************/
Mat PlateChar::getHistogram(Mat mat_src,int xyType,int modelType)
{
	int size=(xyType)?mat_src.rows:mat_src.cols;
	Mat mat_hist=Mat::zeros(1,size,CV_32F);//��ʼ��

	for(int i=0;i<size;i++)
	{
		Mat data=(xyType)?mat_src.row(i):mat_src.col(i);//�л�������
		mat_hist.at<float>(i)=countNonZero(data);//ͳ����һ�л�һ���У�����Ԫ�صĸ����������浽mat_hist��  
	}

	//ȡmat_histֱ��ͼ�е����ֵ 
	double min,max;
	minMaxLoc(mat_hist,&min,&max);

	//cout<<"mat_src.rows:"<<mat_src.rows<<endl;
	//cout<<"mat_src.cols:"<<mat_src.cols<<endl;

	//cout<<"size:"<<size<<endl;
	//cout<<"max:"<<max<<endl;
	if(modelType)//��׼ģ������
	{
		//��һ��ֱ��ͼ
		if(max>0)
		{
			mat_hist.convertTo(mat_hist,-1,1.0f/max, 0); // ���Ų�ת��������һ���������ͣ�
			//src.convertTo(dst, type, scale, shift)
			//type ����Ǹ�ֵ������-1�������������������������ͬ
		}
	}
	else
	{
		int m=mat_src.rows+mat_src.cols-size;
		mat_hist.convertTo(mat_hist,-1,1.0f/m, 0); 
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
	Mat verHist=getHistogram(mat_src,0,1);
	Mat horHist=getHistogram(mat_src,1,1);

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