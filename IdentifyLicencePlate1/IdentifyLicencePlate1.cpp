// IdentifyLicencePlate1.cpp : �������̨Ӧ�ó������ڵ㡣

#include "stdafx.h"
#include "FileOperation.h"
#include "IplImageProcessing.h"
#include "getSource.h"

int _tmain(int argc, _TCHAR* argv[])
{
	/**********************
	��һ�׶Σ�Ԥ����
	��ʼ
	**********************/
	IplImageProcessing* pImgProcess=new IplImageProcessing();//ͼƬ������
	getSource* pSrc=new getSource();//��Դ·����

	IplImage* pImg_src=cvLoadImage(pSrc->getImgpath(),CV_LOAD_IMAGE_COLOR);
	IplImage* pImg_gray = cvCreateImage(cvGetSize(pImg_src), IPL_DEPTH_8U,1);
	pImg_gray=pImgProcess->myRGB2Gray(pImg_src,pImg_gray);
	cvShowImage("ORIGNAL_src",pImg_src);
	cvShowImage("ORIGNAL_gray",pImg_gray);

	//��ֵ�˲�
	IplImage* pImg_smooth = cvCreateImage(cvGetSize(pImg_src), IPL_DEPTH_8U,1);
	//cvSmooth(pImg_gray,pImg_smooth,CV_MEDIAN,3,3,0);
	//˫���˲�
	cvSmooth(pImg_gray,pImg_smooth,CV_BILATERAL,3,3,0);
	//cvShowImage("ORIGNAL_smooth",pImg_smooth);

	//�Ҷ�����
	IplImage* pImg_stretch = cvCreateImage(cvGetSize(pImg_src), IPL_DEPTH_8U,1);
	pImg_stretch=pImgProcess->myGrayStretch(pImg_smooth,20,240);
	//cvShowImage("�Ҷ�����",pImg_stretch);

	//ͼ���Ե���
	IplImage* pImg_sobel = cvCreateImage(cvGetSize(pImg_src), IPL_DEPTH_8U,1);
	//cvSobel(pImg_stretch,pImg_sobel,0,1,3);//��ֱ���
	//cvShowImage("cvSobel_y",pImg_sobel);
	cvSobel(pImg_stretch,pImg_sobel,2,0,3);//ˮƽ���
	//cvShowImage("cvSobel_x",pImg_sobel);

	//��̬ѧ����
	int nVer=5,nHor=8;
	IplImage* pImg_dilate = cvCreateImage(cvGetSize(pImg_src), IPL_DEPTH_8U,1);
	//pImg_stretch=pImgProcess->myDilate(pImg_sobel,nVer,1,nHor,0);//��ֱ����
	//cvShowImage("cvDilate_y",pImg_stretch);
	pImg_stretch=pImgProcess->myDilate(pImg_sobel,nVer,0,nHor,1);//ˮƽ����
	//cvShowImage("cvDilate_x",pImg_stretch);

	//ȫ�ֶ�ֵ��
	IplImage* pImg_threshold = cvCreateImage(cvGetSize(pImg_src), IPL_DEPTH_8U,1);
	pImg_threshold=pImgProcess->myThreshold(pImg_stretch);
	//cvShowImage("cvThreshold",pImg_threshold);

	////����Ӧ��ֵ��
	//IplImage* pImg_threshold = cvCreateImage(cvGetSize(pImg_src), IPL_DEPTH_8U,1);
	//cvThreshold(pImg_stretch,pImg_threshold,0,255,CV_THRESH_OTSU);
	//cvShowImage("cvThreshold",pImg_threshold);

	/**********************
	��һ�׶Σ�Ԥ����
	����
	**********************/

	/**********************
	�ڶ��׶Σ����ƶ�λ����ȡ
	��ʼ
	**********************/

	//��̬ѧ��λ
	list<CvRect> list_rects;//�洢����λ��
	IplImage* pImg_contour=cvCreateImage(cvGetSize(pImg_src),IPL_DEPTH_8U,3);//3ͨ���Ҷ�ͼ���洢������Ϣ
	double minX=pImg_src->width/4,minY=pImg_src->height/2.5;//����λ�ó��ֵ���С����λ��(minX,minY)
	double minWidth=0,minHeight=0;//��С��󳤿�ȷ�ֵ
	double minK=2.0,maxK=5.5;//��С��󳤿�ȷ�ֵ
	int minArea=300;//Ŀ��λ����С���
	list_rects=pImgProcess->myRoughing(pImg_threshold,pImg_contour,list_rects,minX,minY,minWidth,minHeight,minK,maxK,minArea);
	//cvShowImage("myRoughing",pImg_contour);

	//��ɫ��λ
	//������ɫ�����װ��֣��Ƶ׺��֣��ڵװ��֣��׵׺���
	CvRect rect2;
	IplImage* pImg_selectColor=NULL;
	IplImage* pImg_selectGray=NULL;
	IplImage* pImg_selectThreshold=NULL;
	IplImage* pImg_selectLicence=NULL;
	int colorType=-1;//��¼���ף��Ƶף��ڵף��׵�
	bool colorFlag=false;//�ҵ���ɫ���
	if(list_rects.empty())
	{
		cout<<"list_rects is empty()"<<endl;
		goto Release;//ת��ȥ�ͷ���Դ������
	}
	else
	{
		cout<<"list_rects is not empty()"<<endl;
	}

	while(!list_rects.empty()&&!colorFlag)//�ǿղ���û�ҵ���ɫ
	{
		rect2=list_rects.front();
		pImg_selectColor=cvCreateImage(cvSize(rect2.width,rect2.height),IPL_DEPTH_8U,3);
		cvSetImageROI(pImg_src,rect2);
		cvCopyImage(pImg_src,pImg_selectColor);
		cvResetImageROI(pImg_src);
		cvShowImage("pImg_selectColor",pImg_selectColor);
		//Ѱ�Ҿ�ȷ����
		//�ҶȻ�
		pImg_selectGray=cvCreateImage(cvGetSize(pImg_selectColor), IPL_DEPTH_8U,1);
		pImg_selectGray=pImgProcess->myRGB2Gray(pImg_selectColor,pImg_selectGray);
		cvShowImage("pImg_selectGray",pImg_selectGray);

		//ȫ�ֶ�ֵ��
		pImg_selectThreshold = cvCreateImage(cvGetSize(pImg_selectColor), IPL_DEPTH_8U,1);
		pImg_selectThreshold=pImgProcess->myThreshold(pImg_selectGray);
		//cvThreshold(pImg_selectGray,pImg_selectThreshold,0,255,CV_THRESH_OTSU);
		cvShowImage("pImg_selectThreshold",pImg_selectThreshold);
		int x1=0,x2=0;
		int y1=0,y2=0;
		//cout<<"pImg_selectThreshold->height="<<pImg_selectThreshold->height<<" pImg_selectThreshold->width="<<pImg_selectThreshold->width<<endl;
		//cout<<"rect2.height="<<rect2.height<<" rect2.width="<<rect2.width<<endl;
		bool flagLicence=pImgProcess->isMyLicence(pImg_selectThreshold,240,x1,x2,pImg_selectThreshold->width/3,y1,y2,pImg_selectThreshold->height/2);
		if(flagLicence)//��ȷ�鵽����ʱ
		{
			int nHorK=2,nVerK=2;//������С��
			int nWidth=x2-x1-2*nHorK,nHeight=y2-y1-2*nVerK;//���п��
			pImg_selectLicence=cvCreateImage(cvSize(nWidth,nHeight),IPL_DEPTH_8U,3);
			cvSetImageROI(pImg_src,cvRect(rect2.x+x1+nHorK,rect2.y+y1+nVerK,nWidth,nHeight));
			cvCopyImage(pImg_src,pImg_selectLicence);
			cvResetImageROI(pImg_src);
			cvShowImage("pImg_selectLicence",pImg_selectLicence);
			//Ѱ�ҵ�ɫ
			pImgProcess->myExtractHSV(pImg_selectLicence,colorType);//colorType!=-1��˵���ҵ���ɫ
			break;
		}
		list_rects.pop_front();
	}

	cout<<"������ɫ���ͣ�";
	switch(colorType)
	{
		case 0:
			cout<<"����"<<endl;
			break;
		case 1:
			cout<<"����"<<endl;
			break;
		case 2:
			cout<<"����"<<endl;
			break;
		case 3:
			cout<<"����"<<endl;
			break;
		default:
			cout<<"no found"<<endl;
			goto Release;//ת��ȥ�ͷ���Դ������
	}
	/**********************
	�ڶ��׶Σ����ƶ�λ
	����
	**********************/

	/**********************
	�����׶Σ�����ʶ��
	��ʼ
	**********************/

	//�ַ��и�
	IplImage* pImg_LicenceResize=NULL;
	IplImage* pImg_LicenceGray=NULL;
	IplImage* pImg_LicenceContour=NULL;
	IplImage* pImg_LicenceThreshold=NULL;
	IplImage* pImg_LicenceThreshold2=NULL;
	IplImage* pImg_LicenceChar[7];//��ų����ַ�
	if(pImg_selectLicence!=NULL)
	{
		//��һ������ͼ���С
		int nRwidth=144,nRheight=33;//�µĿ����߶�
		pImg_LicenceResize=cvCreateImage(cvSize(nRwidth,nRheight),IPL_DEPTH_8U,3);
		cvResize(pImg_selectLicence,pImg_LicenceResize,CV_INTER_LINEAR); //���Բ�ֵ
		cvShowImage("pImg_LicenceResize",pImg_LicenceResize);

		//�ҶȻ�
		pImg_LicenceGray=cvCreateImage(cvGetSize(pImg_LicenceResize), IPL_DEPTH_8U,1);
		pImg_LicenceGray=pImgProcess->myRGB2Gray(pImg_LicenceResize,pImg_LicenceGray);
		cvShowImage("pImg_LicenceGray",pImg_LicenceGray);

		//����Ӧ���ɷֶ�ֵ�������ɷֱ�ɺ�ɫ��������Ϊ��ɫ��
		pImg_LicenceThreshold = cvCreateImage(cvGetSize(pImg_LicenceResize), IPL_DEPTH_8U,1);
		pImg_LicenceThreshold2 = cvCreateImage(cvGetSize(pImg_LicenceResize), IPL_DEPTH_8U,1);
		//pImg_LicenceThreshold=pImgProcess->myThreshold(pImg_LicenceGray);
		//cvThreshold(pImg_LicenceGray,pImg_LicenceThreshold,140,255,CV_THRESH_OTSU);
		pImg_LicenceThreshold=pImgProcess->myBaseAdaptiveThreshold(pImg_LicenceGray);
		cvShowImage("pImg_LicenceThreshold",pImg_LicenceThreshold);
		//����pImg_LicenceThreshold
		pImg_LicenceThreshold2=cvCloneImage(pImg_LicenceThreshold);
		cvShowImage("pImg_LicenceThreshold2",pImg_LicenceThreshold2);

		//��̬ѧ��λ�����ַ�
		list<CvRect> list_char_rects;//�洢�����ַ�λ��
		pImg_LicenceContour=cvCreateImage(cvGetSize(pImg_LicenceResize),IPL_DEPTH_8U,3);//3ͨ���Ҷ�ͼ���洢������Ϣ
		double minX=0,minY=0;//����λ�ó��ֵ���С����λ��(minX,minY)
		double minWidth=0,minHeight=nRheight*0.70;//��С�����С��
		double minK=0.0,maxK=33/20.0*1.35;//��С��󳤿�ȷ�ֵ
		int minArea=20;//Ŀ��λ����С���
		list_char_rects=pImgProcess->myRoughing(pImg_LicenceThreshold,pImg_LicenceContour,list_char_rects,minX,minY,minWidth,minHeight,minK,maxK,minArea);
		cvShowImage("pImg_LicenceContour",pImg_LicenceContour);

		//��ɸѡ�����ַ������ҽ������������CvRect���������Ƚ϶���
		list_char_rects.sort();
		//�Ե�һ�������ַ������и�
		CvRect rect_char=list_char_rects.front();
		cout<<"x="<<rect_char.x<<" y="<<rect_char.y<<endl;
		cout<<"width="<<rect_char.width<<" height="<<rect_char.height<<endl;
		int min_x=6,min_y=3;
		int max_x=20,max_y=10;
		int k=0;
		const char* name_LicenceChar[]={"pImg_LicenceChar1","pImg_LicenceChar2","pImg_LicenceChar3","pImg_LicenceChar4","pImg_LicenceChar5","pImg_LicenceChar6","pImg_LicenceChar7"};
		if((7==list_char_rects.size()&&rect_char.x>min_x&&rect_char.x<max_x)||6==list_char_rects.size())//��Ҫ�Ե�һ�������и�����
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

			if(rect_temp.width<10)//�����ַ���1
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


	//�ַ�ʶ��



	/**********************
	�����׶Σ�����ʶ��
	����
	**********************/

	//�ͷ���Դģ��
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

