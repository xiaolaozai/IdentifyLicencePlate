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
	const int s=1000;//������Ϣ��С
	const float *support;
	int size=400;//ͼ���С
	int i,j,sv_num;
	IplImage *pImg;
	CvSVM svm;
	CvSVMParams param;//svm����
	CvTermCriteria criteria;//ֹͣ�����ı�׼,����
	CvRNG rng=CvRNG(time(NULL));//�����
	CvPoint pts[s];//������Ϣ��
	float data[s*2];
	int res[s];//������Ϣ
	CvMat data_mat,res_mat;
	CvScalar rcolor;
	
	//��ʼ��
	pImg=cvCreateImage(cvSize(size,size),IPL_DEPTH_8U,3);
	cvZero(pImg);

	//������ɲ���ͼ
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

	//��ʾ����ͼ
	cvShowImage("SVM test img",pImg);

	//ѧϰ��������
	for(int i=0;i<s;i++)
	{
		data[i*2]=float(pts[i].x)/size;
		data[i*2+1]=float(pts[i].y)/size;
	}
	//�������ռ�
	cvInitMatHeader(&data_mat,s,2,CV_32FC1,data);
	cvInitMatHeader(&res_mat,s,1,CV_32SC1,res);
	
	criteria=cvTermCriteria(CV_TERMCRIT_EPS,1000,FLT_EPSILON);//ֹͣ�����ı�׼,����
	param=CvSVMParams(CvSVM::C_SVC,CvSVM::RBF,10.0,8.0,1.0,10.0,0.5,0.1,NULL,criteria);

	//ѵ��
	svm.train(&data_mat,&res_mat,NULL,NULL,param);

	//ѧϰ��� �軭
	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			CvMat m;
			float ret=0.0;
			float a[]={float(j)/size,float(i)/size};
			cvInitMatHeader(&m,1,2,CV_32FC1,a);//�������ռ�
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

	//���軭
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

	//����֧������
	sv_num=svm.get_support_vector_count();
	for(i=0;i<sv_num;i++)
	{
		support=svm.get_support_vector(i);
		cvCircle(pImg,cvPoint((int)(support[0]*size),(int)(support[1]*size)),5,CV_RGB(200,200,200));
	}

	//��ʾ���ͼ
	cvShowImage("SVM result",pImg);
	cvWaitKey(0);
	//�ͷ���Դ
	cvReleaseImage(&pImg);
}

void IdentifyCharSVM::readFaceSVMStudy()
{
	const int pImage_num=500;//����ͼƬ����
	const int nImage_num=1000;//����ͼƬ����
	const int allImage_num=pImage_num+nImage_num;//��ͼƬ����

	int i,j,ii,jj;
	int width=28,height=30;//ͼ���С
	int image_dim=width*height;
	IplImage *pImg_org;
	IplImage *pImg_spl;
	int res[allImage_num];
	float data[allImage_num*allImage_num];
	CvMat data_mat,res_mat;
	CvTermCriteria criteria;
	CvSVM svm;
	CvSVMParams param;
	char filename[64];
	char xmlfilename[]="../file_src/svm_image.xml";

	//����
	for(i=0;i<pImage_num;i++)
	{
		sprintf(filename,"positive/%03d.png",i);
		pImg_org=cvLoadImage(filename,CV_LOAD_IMAGE_GRAYSCALE);
		pImg_spl=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
		cvResize(pImg_org,pImg_spl);
		cvSmooth(pImg_spl,pImg_spl,CV_GAUSSIAN,3,0,0,0);
		for(ii=0;ii<height;ii++)
		{
			for(jj=0;jj<width;jj++)
			{
				data[i*image_dim+(ii*width)+jj]=float((int)((uchar)(pImg_spl->imageData[ii*pImg_spl->widthStep+jj]))/255.0);
			}
		}
		res[i]=1;
	}
	
	//����
	j=i;
	for(i=j;i<j+nImage_num;i++)
	{
		sprintf(filename,"negative/%03d.jpg",i-j);
		pImg_org=cvLoadImage(filename,CV_LOAD_IMAGE_GRAYSCALE);
		pImg_spl=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
		cvResize(pImg_org,pImg_spl);
		cvSmooth(pImg_spl,pImg_spl,CV_GAUSSIAN,3,0,0,0);
		for(ii=0;ii<height;ii++)
		{
			for(jj=0;jj<width;jj++)
			{
				data[i*image_dim+(ii*width)+jj]=float((int)((uchar)(pImg_spl->imageData[ii*pImg_spl->widthStep+jj]))/255.0);
			}
		}
		res[i]=0;
	}

	//��ʼ��
	cvInitMatHeader(&data_mat,allImage_num,image_dim,CV_32FC1,data);
	cvInitMatHeader(&res_mat,allImage_num,image_dim,CV_32SC1,res);

	criteria=cvTermCriteria(CV_TERMCRIT_EPS,1000,FLT_EPSILON);
	param=CvSVMParams(CvSVM::C_SVC,CvSVM::RBF,10.0,0.09,1.0,10.0,0.5,1.0,NULL,criteria);

	//����
	svm.train(&data_mat,&res_mat,NULL,NULL,param);
	svm.save(xmlfilename);

	//�ͷ���Դ
	cvReleaseImage(&pImg_org);
	cvReleaseImage(&pImg_spl);
}

void IdentifyCharSVM::readFaceSVMPredict()
{
	const int width=28,height=30;//ͼ��ߴ�
	const int image_dim=width*height;

	int i,j;
	float a[image_dim];
	float ret=-1.0;
	float scale;
	IplImage *src,*src_color,*src_tmp;
	int sx,sy,tw,th;
	int stepx=3,stepy=3;
	double steps=1.2;
	int iterate;
	CvMat mat;
	CvSVM svm;
	char filename[]="../img_src/test.jpg";
	char xmlfilename[]="../file_src/svm_image.xml";

	src=cvLoadImage(filename,CV_LOAD_IMAGE_GRAYSCALE);
	src_color=cvLoadImage(filename,CV_LOAD_IMAGE_COLOR);
	//��ʾͼ��
	cvShowImage("original image",src_color);

	//��ȡ������Ϣ
	svm.load(xmlfilename);

	cvInitMatHeader(&mat,1,image_dim,CV_32FC1,NULL);
	tw=src->width;
	th=src->height;

	for(iterate=0;iterate<1;iterate++)
	{
		//������С
		src_tmp=cvCreateImage(cvSize((int)(tw/steps),(int)(th/steps)),IPL_DEPTH_8U,1);
		cvResize(src,src_tmp);
		tw=src_tmp->width;
		th=src_tmp->height;
		for(sy=0;sy<=src_tmp->height-height;sy+=stepy)
		{
			for(sx=0;sx<=src_tmp->width-width;sx+=stepx)
			{
				for(i=0;i<height;i++)
				{
					for(j=0;j<width;j++)
					{
						a[i*width+j]=float((int)((uchar)(src_tmp->imageData[(i+sy)*src_tmp->widthStep+(j+sx)]))/255.0);
					}
				}
				cvSetData(&mat,a,sizeof(float)*image_dim);

				//�軭���
				ret=svm.predict(&mat);
				if((int)ret==1)
				{
					scale=(float)src->width/tw;
					cvRectangle(src_color,cvPoint((int)(sx*scale),(int)(sy*scale)),cvPoint((int)((sx+width)*scale),(int)((sy+height)*scale)),CV_RGB(255,0,0),2);
				}
			}
		}
		cvReleaseImage(&src_tmp);
	}
	//��ʾͼ��
	cvShowImage("result",src_color);
	cvWaitKey(0);

	//�ͷ���Դ
	cvReleaseImage(&src);
	cvReleaseImage(&src_color);
}

