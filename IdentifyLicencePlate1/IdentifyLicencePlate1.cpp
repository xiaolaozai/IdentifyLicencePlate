// IdentifyLicencePlate1.cpp : �������̨Ӧ�ó������ڵ㡣

#include "stdafx.h"
#include "FileOperation.h"
#include "IplImageProcessing.h"
#include "getSource.h"
#include "IdentifyCharSVM.h"
#include "PlateChar.h"
#include "IdentifyCharANN.h"

int _tmain(int argc, _TCHAR* argv[])
{
	/**********************
	��һ�׶Σ�Ԥ����
	��ʼ
	**********************/
	int i,j;
	IplImageProcessing* pImgProcess=new IplImageProcessing();//ͼƬ������
	getSource* pSrc=new getSource();//��Դ·����
	IdentifyCharSVM* pSVM=new IdentifyCharSVM();//svm�ַ�ʶ����
	PlateChar* pPlate=new PlateChar();//�����ַ���
	IdentifyCharANN *pAnn=new IdentifyCharANN();//ann�ַ�ʶ����

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
	pImg_threshold=pImgProcess->myThreshold(pImg_stretch,0);
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
	double minX=pImg_src->width*0.25,minY=pImg_src->height*0.40;//����λ�ó��ֵ���С����λ��(minX,minY)
	double minWidth=0,minHeight=0;//��С����
	double maxWidth=pImg_src->width;//����
	double minK=2.0,maxK=5.5;//��С��󳤿�ȷ�ֵ
	int minArea=300;//Ŀ��λ����С���
	list_rects=pImgProcess->myRoughing(pImg_threshold,pImg_contour,list_rects,minX,minY,minWidth,minHeight,maxWidth,minK,maxK,minArea);
	//cvShowImage("myRoughing",pImg_contour);

	//��ɫ��λ
	//������ɫ�����װ��֣��Ƶ׺��֣��ڵװ��֣��׵׺���
	CvRect rect2;
	IplImage* pImg_selectColor=NULL;
	IplImage* pImg_selectRecorrect=NULL;
	IplImage* pImg_selectGray=NULL;
	IplImage* pImg_selectThreshold=NULL;
	IplImage* pImg_selectLicence=NULL;
	IplImage* pImg_PlateResize=NULL;
	IplImage* pImg_PlateAdaptiveThreshold=NULL;
	IplImage* pImg_PlateAdaptiveThreshold2=NULL;
	IplImage* pImg_PlateColor=NULL;
	IplImage* pImg_hist=NULL;
	IplImage* pImg_LicenceContour=NULL;
	IplImage* pImg_Canny=NULL;
	IplImage* pImg_Erode=NULL;
	IplImage* pImg_HoughLines=NULL;
	IplImage* pImg_LicenceChar[7];//��ų����ַ�
	int colorType=-1;//��¼���ף��Ƶף��ڵף��׵�
	if(list_rects.empty())
	{
		cout<<"list_rects is empty()"<<endl;
		goto Release;//ת��ȥ�ͷ���Դ������
	}
	else
	{
		cout<<"list_rects is not empty()"<<endl;
	}

	//����svmѵ�����Ʒ�����xml�ļ�
	//pSVM->plateSVMTrain(pSrc->getImageTrainPath(),pSrc->getXMLTrainPath(),"plate");
	//����svmѵ�������ַ�������xml�ļ�
	//pSVM->setTrainFile(pSrc->getImageTrainPath(),pSrc->getXMLTrainPath(),"chn");
	//pSVM->setTrainFile(pSrc->getImageTrainPath(),pSrc->getXMLTrainPath(),"letter");
	//pSVM->setTrainFile(pSrc->getImageTrainPath(),pSrc->getXMLTrainPath(),"num");
	//pSVM->setTrainFile(pSrc->getImageTrainPath(),pSrc->getXMLTrainPath(),"num_letter");

	////����annѵ�������ַ�������yml�ļ�
	//pAnn->setDataFile(pSrc->getImageTrainPath(),pSrc->getYMLTrainPath(),"chn");
	//pAnn->setDataFile(pSrc->getImageTrainPath(),pSrc->getYMLTrainPath(),"letter");
	//pAnn->setDataFile(pSrc->getImageTrainPath(),pSrc->getYMLTrainPath(),"num");
	//pAnn->setDataFile(pSrc->getImageTrainPath(),pSrc->getYMLTrainPath(),"num_letter");

	////����annѵ�������ַ�������xml�ļ�
	//pAnn->setTrainFile(pSrc->getYMLTrainPath(),pSrc->getXMLTrainPath(),"chn",10,15);
	//pAnn->setTrainFile(pSrc->getYMLTrainPath(),pSrc->getXMLTrainPath(),"letter",10,15);
	//pAnn->setTrainFile(pSrc->getYMLTrainPath(),pSrc->getXMLTrainPath(),"num",10,15);
	//pAnn->setTrainFile(pSrc->getYMLTrainPath(),pSrc->getXMLTrainPath(),"num_letter",10,15);

	int kk=1;
	while(!list_rects.empty()&&colorType==-1)//�ǿղ���û�ҵ���ɫ
	{
		rect2=list_rects.front();
		pImg_selectColor=cvCreateImage(cvSize(rect2.width,rect2.height),IPL_DEPTH_8U,3);
		pImgProcess->myCutImage(pImg_src,pImg_selectColor,rect2);
		//cvSetImageROI(pImg_src,rect2);
		//cvCopy(pImg_src,pImg_selectColor);
		//cvResetImageROI(pImg_src);
		cvShowImage("pImg_selectColor",pImg_selectColor);
		//Ѱ�Ҿ�ȷ����
		//�ҶȻ�
		pImg_selectGray=cvCreateImage(cvGetSize(pImg_selectColor), IPL_DEPTH_8U,1);
		pImg_selectGray=pImgProcess->myRGB2Gray(pImg_selectColor,pImg_selectGray);
		cvShowImage("pImg_selectGray",pImg_selectGray);

		//������ѵ������Сһ����ͼƬ
		pImg_PlateResize=cvCreateImage(cvSize(144,33),IPL_DEPTH_8U,1);
		cvResize(pImg_selectGray,pImg_PlateResize,CV_INTER_LINEAR); //���Բ�ֵ
		//char platename[64];
		//sprintf(platename,"../img_model/plate/%d.jpg",kk++);
		//cvSaveImage(platename,pImg_PlateResize);
		//cvShowImage(platename,pImg_PlateResize);

		//��ȡ���Ƿ�Ϊ����
		int p=0;
		p=pSVM->plateSVMPredict(pImg_PlateResize,pSrc->getXMLTrainPath(),"plate");
		//��ȷ��ѯ����λ��
		if(p==1)//�ǳ��ƵĴ�ѡλ��
		{
			//����У��
			pImg_selectRecorrect=cvCreateImage(cvGetSize(pImg_selectColor), IPL_DEPTH_8U,3);
			pImgProcess->reCorrectPosition(pImg_selectColor,pImg_selectRecorrect,1);
			cvShowImage("pImg_selectRecorrect",pImg_selectRecorrect);

			//�����и�
			pImg_Canny = cvCreateImage(cvGetSize(pImg_selectRecorrect), IPL_DEPTH_8U,1);
			cvSmooth(pImg_selectGray, pImg_Canny, CV_BLUR, 3, 3, 0, 0 );
			cvCanny(pImg_selectGray, pImg_Canny, 50, 150, 3);  //����canny�㷨���б�Ե���

			int x0=0,x1=pImg_Canny->width;
			int y0=0,y1=pImg_Canny->height;
			float xT=0.65;
			float xV=0.75;
			float yT=0.45;
			float yV=0.75;
			pPlate->getLicencePosition(pImg_Canny,x0,x1,xV,xT,y0,y1,yV,yT);

			pImg_selectGray=pImgProcess->myRGB2Gray(pImg_selectRecorrect,pImg_selectGray);
			cvShowImage("pImg_selectGray2",pImg_selectGray);
			//��ֵ��
			pImg_PlateAdaptiveThreshold = cvCreateImage(cvGetSize(pImg_selectRecorrect), IPL_DEPTH_8U,1);
			//cvThreshold(pImg_selectGray,pImg_PlateAdaptiveThreshold,0,255,CV_THRESH_OTSU);
			pImg_PlateAdaptiveThreshold=pImgProcess->myThreshold(pImg_selectGray,0);
			cvShowImage("pImg_PlateAdaptiveThreshold",pImg_PlateAdaptiveThreshold);

			//�����и�
			int yy0=y0,yy1=y1;
			int nTimes=4;
			pImgProcess->mySearchVerPosition(pImg_PlateAdaptiveThreshold,y0,y1,nTimes);
			
			if(y1>yy1)
			{
				y1=yy1;
			}

			int cutW=x1-x0;
			int cutH=y1-y0;
			pImg_selectLicence=cvCreateImage(cvSize(cutW,cutH),IPL_DEPTH_8U,3);
			pImgProcess->myCutImage(pImg_selectRecorrect,pImg_selectLicence,cvRect(x0,y0,cutW,cutH));
			cvShowImage("pImg_selectLicence",pImg_selectLicence);
			cout<<"pImg_selectLicence->width:"<<pImg_selectLicence->width<<endl;
			cout<<"pImg_selectLicence->height:"<<pImg_selectLicence->height<<endl;

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
	IplImage* pImg_LicenceThreshold=NULL;
	IplImage* pImg_LicenceGray2=NULL;
	IplImage* pImg_Plate[7];//��Ź�һ�������ַ�
	if(pImg_selectLicence!=NULL)
	{
		
		//��һ������ͼ���С
		int nRwidth=144,nRheight=33;//�µĿ����߶�
		pImg_LicenceResize=cvCreateImage(cvSize(nRwidth,nRheight),IPL_DEPTH_8U,3);
		cvResize(pImg_selectLicence,pImg_LicenceResize,CV_INTER_LINEAR); //���Բ�ֵ
		cvShowImage("pImg_LicenceResize",pImg_LicenceResize);

		
		//pImg_PlateAdaptiveThreshold = cvCreateImage(cvGetSize(pImg_Canny), IPL_DEPTH_8U,1);
		pImg_LicenceGray=cvCreateImage(cvGetSize(pImg_LicenceResize), IPL_DEPTH_8U,1);
		pImg_LicenceGray2 = cvCreateImage(cvGetSize(pImg_LicenceResize), IPL_DEPTH_8U,1);
		pImg_LicenceGray=pImgProcess->myRGB2Gray(pImg_LicenceResize,pImg_LicenceGray);
		//cvThreshold(pImg_LicenceResize,pImg_LicenceGray,0,255,CV_THRESH_OTSU);
		pImg_LicenceGray2=cvCloneImage(pImg_LicenceGray);
		cvShowImage("pImg_LicenceGray",pImg_LicenceGray);

		//����Ӧ��ֵ��(�ֲ���ֵ��)��������Ӱ��
/*		int blockSize = 25;  
		int param1 = 10;*/  
		pImg_LicenceThreshold = cvCreateImage(cvGetSize(pImg_LicenceResize), IPL_DEPTH_8U,1);
		//cvAdaptiveThreshold(pImg_LicenceGray,pImg_LicenceThreshold,255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY);
		cvThreshold(pImg_LicenceGray,pImg_LicenceThreshold,0,255,CV_THRESH_OTSU);
		cvShowImage("pImg_LicenceThreshold",pImg_LicenceThreshold);

		if(colorType==1||colorType==2)//��ɫΪǳɫʱ���׻�ƣ�ȡ������
		{
			//cvAdaptiveThreshold(pImg_LicenceGray,pImg_LicenceThreshold,255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV);
			cvThreshold(pImg_LicenceThreshold,pImg_LicenceThreshold,0,255,CV_THRESH_BINARY_INV);
		}

		cvShowImage("pImg_LicenceThreshold2",pImg_LicenceThreshold);

		//��̬ѧ��λ�����ַ�
		list<CvRect> list_char_rects;//�洢�����ַ�λ��
		pImg_LicenceContour=cvCreateImage(cvGetSize(pImg_LicenceResize),IPL_DEPTH_8U,1);//1ͨ���Ҷ�ͼ���洢������Ϣ
		double minX=0,minY=0;//����λ�ó��ֵ���С����λ��(minX,minY)
		double minWidth=2,minHeight=pImg_LicenceResize->height*0.64;//��С�����С��
		double maxWidth=pImg_LicenceResize->width*0.20;//����
		double minK=0.0,maxK=33/20.0*1.25;//��С��󳤿�ȷ�ֵ
		int minArea=20;//Ŀ��λ����С���
		list_char_rects=pImgProcess->myRoughing(pImg_LicenceThreshold,pImg_LicenceContour,list_char_rects,minX,minY,minWidth,minHeight,maxWidth,minK,maxK,minArea);
		cvShowImage("pImg_LicenceContour",pImg_LicenceContour);

		if(list_char_rects.empty())
		{
			cout<<"�и��ַ�����"<<endl;
			goto Release;
		}

		//��ɸѡ�����ַ������ҽ������������CvRect���������Ƚ϶���
		list_char_rects.sort();
		//�Ե�һ�������ַ������и�
		CvRect rect_char=list_char_rects.front();
		cout<<"x="<<rect_char.x<<" y="<<rect_char.y<<endl;
		cout<<"width="<<rect_char.width<<" height="<<rect_char.height<<endl;
		cout<<endl;
		int min_x=6,min_y=3;
		int max_x=20,max_y=10;
		int minW=10,maxW=30;
		int k=0;
		const char* name_LicenceChar[]={"pImg_LicenceChar1","pImg_LicenceChar2","pImg_LicenceChar3","pImg_LicenceChar4","pImg_LicenceChar5","pImg_LicenceChar6","pImg_LicenceChar7"};
		bool setFirflag=false;
		i=0;
		for(list<CvRect>::iterator iter=list_char_rects.begin();iter!=list_char_rects.end()&&i<2;iter++)
		{
			CvRect rect_temp=*iter;
			if(rect_temp.height*0.92>rect_char.height&&rect_char.width>=minW)
			{
				setFirflag=true;
				break;
			}
			i++;
		}

		if(setFirflag||(list_char_rects.size()>=7&&(rect_char.width<minW||rect_char.width>maxW))||6==list_char_rects.size())//��Ҫ�Ե�һ�������и�����
		{
			int x=2,y=4;
			int ww=18,hh=26;
			if(list_char_rects.size()>=7)
			{
				list_char_rects.pop_front();
				//cout<<"made list_char_rects.pop_front"<<endl;
			}

			if(list_char_rects.size()<7||setFirflag)
			{
				CvRect rect_tmp=list_char_rects.front();
				if(rect_tmp.x>max_x)//˵����ǰ�ڶ����ַ����ڵ�λ�úϷ�����ǰ���ַ��Ǻ���
				{
					y=rect_tmp.y;
					ww=rect_tmp.x-x;
					hh=rect_tmp.height;//ȡ�ڶ����ַ��ĸ߶�

					pImg_LicenceChar[k]=cvCreateImage(cvSize(ww,hh),IPL_DEPTH_8U,1);
					pImgProcess->myCutImage(pImg_LicenceGray2,pImg_LicenceChar[k],cvRect(x,y,ww,hh));
					k++;
				}
				//cvShowImage(name_LicenceChar[k],pImg_LicenceChar[k]);
			}
		}
		cout<<"pImg_LicenceResize->height*0.60="<<pImg_LicenceResize->height*0.60<<endl;
		CvRect rect_pre=list_char_rects.front();
		for(list<CvRect>::iterator iter=list_char_rects.begin();iter!=list_char_rects.end()&&k<7;iter++)
		{
			CvRect rect_temp=*iter;
			//if(k==0)
			//{
			//	cout<<"rect_temp.height="<<rect_temp.height<<endl;
			//}
			cout<<"x="<<rect_temp.x<<" y="<<rect_temp.y<<endl;
			cout<<"width="<<rect_temp.width<<" height="<<rect_temp.height<<endl;

			if(rect_temp.x>rect_pre.x&&rect_temp.x<(rect_pre.x+rect_pre.width))//������ڰ�����ϵ
			{
				rect_pre=rect_temp;
				continue;
			}

			if(k>=3&&rect_temp.x>(rect_pre.x+maxW))//�ӵ�����λ�ÿ�ʼ�����������
			{
				pImg_LicenceChar[k]=cvCreateImage(cvSize(rect_pre.width,rect_pre.height),IPL_DEPTH_8U,1);
				pImgProcess->myCutImage(pImg_LicenceGray2,pImg_LicenceChar[k],cvRect(rect_pre.x+rect_pre.width,rect_pre.y,rect_pre.width,rect_pre.height));
				k++;
			}

			pImg_LicenceChar[k]=cvCreateImage(cvSize(rect_temp.width,rect_temp.height),IPL_DEPTH_8U,1);
			pImgProcess->myCutImage(pImg_LicenceGray2,pImg_LicenceChar[k],rect_temp);

			//cvShowImage(name_LicenceChar[k],pImg_LicenceChar[k]);
			k++;
			rect_pre=rect_temp;
		}
		if(k!=7)
		{
			cout<<"k="<<k<<endl;
			cout<<"�и��ַ�����"<<endl;
			goto Release;
		}
	}

	//ʹ�ÿ�������goto֮���ʼ������
	{
		//��һ���ַ�ͼƬ��С
		for(int i=0;i<7;i++)
		{
			char name[64];

			cvThreshold(pImg_LicenceChar[i],pImg_LicenceChar[i],0,255,CV_THRESH_OTSU);

			if(colorType==1||colorType==2)//��ɫΪǳɫʱ���׻�ƣ�ȡ������
			{
				cvThreshold(pImg_LicenceChar[i],pImg_LicenceChar[i],0,255,CV_THRESH_BINARY_INV);
			}

			Mat mat_resize=pPlate->reSizeChar(pImg_LicenceChar[i],20,20);//��С��Ϊ20*20
			IplImage *pImg_tmp=&IplImage(mat_resize);
			sprintf(name,"../img_model/test/%d.jpg",(i+1));
			cvSaveImage(name,pImg_tmp);
			pImg_Plate[i]=cvLoadImage(name,CV_LOAD_IMAGE_GRAYSCALE);
			//cvShowImage(name,pImg_Plate[i]);
			//imwrite(filename,mat_resize);//����
		}

		string chn[]={"��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","³","��","��","��","��","��","��","ʹ","��","��","��","��","��","��","ԥ","��","��","��"};
		string letter[]={"A","B","C","D","E","F","G","H","J","K","L","M","N","P","Q","R","S","T","U","V","W","X","Y","Z"};
		string num[]={"0","1","2","4","3","5","6","7","8","9"};
		string num_letter[]={"0","1","2","4","3","5","6","7","8","9","A","B","C","D","E","F","G","H","J","K","L","M","N","P","Q","R","S","T","U","V","W","X","Y","Z"};
		
		int p=0;
		//��һ���ַ���ʹ�ú��ַ�����
		p=pAnn->getPredictPosition(pImg_Plate[0],pSrc->getXMLTrainPath(),"chn",15);
		cout<<"��һ���ַ�Ϊ��"<<chn[p]<<endl;

		//�ڶ����ַ���ʹ����ĸ������
		p=pAnn->getPredictPosition(pImg_Plate[1],pSrc->getXMLTrainPath(),"letter",15);
		//cout<<"p="<<p<<endl;
		cout<<"�ڶ����ַ�Ϊ��"<<letter[p]<<endl;

		//�������ַ���ʹ������+��ĸ������
		p=pAnn->getPredictPosition(pImg_Plate[2],pSrc->getXMLTrainPath(),"num_letter",15);
		cout<<"�������ַ�Ϊ��"<<num_letter[p]<<endl;

		//���ĸ��ַ���ʹ������+��ĸ������
		p=pAnn->getPredictPosition(pImg_Plate[3],pSrc->getXMLTrainPath(),"num_letter",15);
		cout<<"���ĸ��ַ�Ϊ��"<<num_letter[p]<<endl;

		//������ַ���ʹ�����ַ�����
		p=pAnn->getPredictPosition(pImg_Plate[4],pSrc->getXMLTrainPath(),"num",15);
		cout<<"������ַ�Ϊ��"<<num[p]<<endl;

		//�������ַ���ʹ�����ַ�����
		p=pAnn->getPredictPosition(pImg_Plate[5],pSrc->getXMLTrainPath(),"num",15);
		cout<<"�������ַ�Ϊ��"<<num[p]<<endl;

		//���߸��ַ���ʹ�����ַ�����
		p=pAnn->getPredictPosition(pImg_Plate[6],pSrc->getXMLTrainPath(),"num",15);
		cout<<"���߸��ַ�Ϊ��"<<num[p]<<endl;
	}

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
	if(pImg_selectRecorrect!=NULL)
	{
		cvReleaseImage(&pImg_selectRecorrect);
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
	if(pImg_PlateResize!=NULL)
	{
		cvReleaseImage(&pImg_PlateResize);
	}
	if(pImg_Canny!=NULL)
	{
		cvReleaseImage(&pImg_Canny);
	}
	if(pImg_Erode!=NULL)
	{
		cvReleaseImage(&pImg_Erode);
	}
	if(pImg_PlateAdaptiveThreshold!=NULL)
	{
		cvReleaseImage(&pImg_PlateAdaptiveThreshold);
	}
	if(pImg_PlateAdaptiveThreshold2!=NULL)
	{
		cvReleaseImage(&pImg_PlateAdaptiveThreshold2);
	}
	if(pImg_LicenceResize!=NULL)
	{
		cvReleaseImage(&pImg_LicenceResize);
	}
	if(pImg_LicenceGray!=NULL)
	{
		cvReleaseImage(&pImg_LicenceGray);
	}
	if(pImg_LicenceGray2!=NULL)
	{
		cvReleaseImage(&pImg_LicenceGray2);
	}
	if(pImg_LicenceThreshold!=NULL)
	{
		cvReleaseImage(&pImg_LicenceThreshold);
	}
	if(pImg_HoughLines!=NULL)
	{
		cvReleaseImage(&pImg_HoughLines);
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
		if(pImg_Plate[i]!=NULL)
		{
			cvReleaseImage(&pImg_Plate[i]);
		}
	}

	if(pImg_hist!=NULL)
	{
		cvReleaseImage(&pImg_hist);
	}
	cvReleaseImage(&pImg_contour);
	cvReleaseImage(&pImg_threshold);
	cvReleaseImage(&pImg_dilate);
	cvReleaseImage(&pImg_sobel);
	cvReleaseImage(&pImg_stretch);
	cvReleaseImage(&pImg_smooth);
	cvReleaseImage(&pImg_gray);
	cvReleaseImage(&pImg_src);
	delete pSVM;
	delete pSrc;
	delete pImgProcess;
	delete pPlate;
	delete pAnn;
	return 0;
}

