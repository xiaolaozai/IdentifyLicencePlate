#include "StdAfx.h"
#include "IdentifyCharSVM.h"
#include "PlateChar.h"

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
	//1������ѵ��������
    /**
	��Ҫ�������ݣ�һ�������ݵ����res[]��һ�������ݵ�������Ϣdata[]��
	**/
	cvInitMatHeader(&data_mat,s,2,CV_32FC1,data);
	cvInitMatHeader(&res_mat,s,1,CV_32SC1,res);
	
	//2������SVM����
	/**
	����CvSVMParams��ʵ�����ڵĳ�Ա����svm_type��ʾSVM���ͣ�

	CvSVM����C_SVC  C-SVC

	CvSVM����NU_SVC v-SVC

	CvSVM����ONE_CLASS һ��SVM

	CvSVM����EPS_SVR e-SVR

	CvSVM����NU_SVR v-SVR

	��Ա����kernel_type��ʾ�˺��������ͣ�

	CvSVM����LINEAR ���ԣ�u��v

	CvSVM����POLY ����ʽ��(r*u'v + coef0)^degree

	CvSVM����RBF RBF������exp(-r|u-v|^2)

	CvSVM����SIGMOID sigmoid������tanh(r*u'v + coef0)

	��Ա����degree��Զ���ʽ�˺���degree�����ã�
	gamma��Զ���ʽ/rbf/sigmoid�˺��������ã�
	coef0��Զ���ʽ/sigmoid�˺��������ã�
	CvalueΪ��ʧ��������C-SVC��e-SVR��v-SVR����Ч��nu����v-SVC��һ��SVM��v-SVR������
	pΪ����e-SVR����ʧ������ֵ��class_weightsC_SVC��Ȩ�أ�
	term_critΪSVMѵ�����̵���ֹ������
	����Ĭ��ֵdegree = 0��gamma = 1��coef0 = 0��Cvalue = 1��nu = 0��p = 0��class_weights = 0
	**/
	criteria=cvTermCriteria(CV_TERMCRIT_EPS,1000,FLT_EPSILON);//ֹͣ�����ı�׼,����
	param=CvSVMParams(CvSVM::C_SVC,CvSVM::RBF,10.0,8.0,1.0,10.0,0.5,0.1,NULL,criteria);

	//3��ѵ��SVM
	/**
	����CvSVM����train��������SVMģ�ͣ���һ������Ϊѵ�����ݣ��ڶ�������Ϊ�����������һ��������CvSVMParams
	**/
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
			//4�������SVM���з���
			/**
			���ú���CvSVM����predictʵ�ַ���
			**/
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

	//5�����֧������
	/**
	���˷��࣬Ҳ���Եõ�SVM��֧�����������ú���CvSVM����get_support_vector_count���֧�������ĸ�����CvSVM����get_support_vector��ö�Ӧ��������ŵ�֧��������
	**/
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

/****************************************
plateSVMTrain
���ó���ѵ�����ļ�

dir_path
�ļ���·��

xml_path
xml�ļ�����·��

dir_type
�ļ������ͣ�plate

******************************************/
void IdentifyCharSVM::plateSVMTrain(string dir_path,string xml_path,string dir_type)
{
	int i,j,ii,jj;
	PlateChar plate;
	Mat data_mat,res_mat;
	vector<int> resLabels;

	//1)����ѵ��������
    /**
	��Ҫ�������ݣ�һ�������ݵ����res[]��һ�������ݵ�������Ϣdata[]��
	**/
	string dir_tmp=dir_path+"/"+dir_type;
	//ʹ��c++�ļ�����ϵͳ
	_finddata_t FileInfo1;
    string strfind = dir_tmp + "/*";
    long Handle1 = _findfirst(strfind.c_str(), &FileInfo1);
    if (Handle1 == -1L)
    {
        cerr << "error IdentifyCharSVM::plateSVMTrain can not match the folder path Handle1" << endl;
        return;
    }
	int k=-1;
    do
	{
        //�ж��Ƿ�����Ŀ¼
        if (FileInfo1.attrib && _A_SUBDIR)    
        {
            //���������Ҫ
            if( (strcmp(FileInfo1.name,".") != 0 ) &&(strcmp(FileInfo1.name,"..") != 0))   
            {
				k++;
                string newPath = dir_tmp + "/" + FileInfo1.name;
				string fileExtension = "jpg";  
				string fileFolder = newPath + "/*." + fileExtension;  
				// �����ļ���  
				char filename[260];  
  
				struct _finddata_t fileInfo2;    // �ļ���Ϣ�ṹ��  
	
				// 1. ��һ�β���  
				long Handle2 = _findfirst(fileFolder.c_str(), &fileInfo2);   
				if (Handle2 == -1L)  
				{  
					_findclose(Handle2);
					cerr << "error IdentifyCharSVM::plateSVMTrain can not match the folder path Handle2" << endl;
					break;  
				}  
      
				// 2. ѭ������  
				do  
				{  
					sprintf(filename, "%s/%s", newPath.c_str(), fileInfo2.name);  
					if ( fileInfo2.attrib == _A_ARCH)  // �Ǵ浵�����ļ�  
					{  
						IplImage *pImg_tmp=cvLoadImage(filename,0);//��ȡ��ͨ��
						Mat mat_img=pImg_tmp;
						Mat mat_tmp;
						if(mat_img.empty())
						{
							cout<<"error IdentifyCharSVM::plateSVMTrain mat_img is empty()"<<endl;
							break;
						}
						
						//equalizeHist(mat_img,mat_tmp);
						Mat f20=plate.getFeatureHist(mat_img,20);
						data_mat.push_back(f20);
						//һ��Ŀ¼��һ������ 
						resLabels.push_back(k);//������ֵΪ1,������ֵΪ0
					}  
  
				}while (!_findnext(Handle2, &fileInfo2));    
				_findclose(Handle2); 
            }
        }
    }while (_findnext(Handle1, &FileInfo1) == 0);

    _findclose(Handle1);

	if(data_mat.empty())
	{
		cout<<"plateSVMTrain fail to match the folder path"<<endl;
		return;
	}

	data_mat.convertTo(data_mat, CV_32FC1); 
	Mat(resLabels).copyTo(res_mat);  
	
	CvSVM svm;
	CvMat data_cvmat=data_mat;//��¼����������Ϣ
	CvMat res_cvmat=res_mat;//��¼�������

	//2������SVM����
	/**
	��ϸ˵�����£�
	**/
	//CvSVMParams params;
 //   params.svm_type = CvSVM::C_SVC;  
 //   params.kernel_type = CvSVM::LINEAR; //CvSVM::LINEAR;  
 ////   params.degree = 0;  
 ////   params.gamma = 1;  
 ////   params.coef0 = 0;  
 ////   params.C = 1;  
 ////   params.nu = 0;  
 ////   params.p = 0;  
	////params.class_weights =0;
 //   params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 1000, FLT_EPSILON); 

	CvTermCriteria criteria=cvTermCriteria(CV_TERMCRIT_EPS,1000,FLT_EPSILON);//ֹͣ�����ı�׼,����
	CvSVMParams params=CvSVMParams(CvSVM::C_SVC,CvSVM::RBF,10.0,8.0,1.0,10.0,0.5,0.1,NULL,criteria);


	//3��ѵ��SVM
	/**
	����CvSVM����train��������SVMģ�ͣ���һ������Ϊѵ�����ݣ��ڶ�������Ϊ�����������һ��������CvSVMParams
	**/
	svm.train(&data_cvmat,&res_cvmat,NULL,NULL,params);

	//cout<<"dir_type:"<<dir_type<<endl;
	//��Ҫ��stringת��Ϊchar[]
	string xmlpath=xml_path+"/"+dir_type+".xml";
	const char *xml_tmp=xmlpath.c_str();
	char *xmlname=(char*)xml_tmp;
	//sprintf(xmlname, "%s.xml",xmlname); 
	svm.save(xmlname);
	//cout<<"xmlname:"<<xmlname<<endl;
	cout<<"plateSVMTrain save xml success"<<endl;
}

/****************************************
plateSVMPredict
��ȡ������ߵķ����λ��

pImg_src
ԴͼƬ��ͨ��

xml_path
xml�ļ�����·��

dir_type
�ļ������ͣ�plate

******************************************/
int IdentifyCharSVM::plateSVMPredict(IplImage *pImg_src,string xml_path,string dir_type)
{
	PlateChar plate;
	CvSVM svm;
	CvMat sample_cvmat;
	Mat sample_mat,img_mat;
	Mat mat_trainingData5;
	Mat mat_trainingData10;
	Mat mat_trainingData15;
	Mat mat_trainingData20;
	//��Ҫ��stringת��Ϊchar[]
	string xmlpath=xml_path+"/"+dir_type+".xml";
	const char *xml_tmp=xmlpath.c_str();
	char *xmlname=(char*)xml_tmp;

	//��ȡ������Ϣ
	svm.load(xmlname);

	img_mat=pImg_src;
	if(img_mat.empty())
	{
		cout<<"error IdentifyCharSVM::plateSVMPredict img_mat is empty()"<<endl;
		return 0;
	}

	Mat mat_tmp;
	//equalizeHist(img_mat,mat_tmp);
	Mat f20=plate.getFeatureHist(img_mat,20);
	sample_mat.push_back(f20);
	/*sample_mat.push_back(mat_tmp);*/
	sample_mat.convertTo(sample_mat, CV_32FC1); 
	sample_cvmat=sample_mat;

	//4�������SVM���з���
	/**
	���ú���CvSVM����predictʵ�ַ���
	**/
	return (int)svm.predict(&sample_cvmat);
}

/****************************************
setTrainFile
����ѵ�����ļ�

dir_path
�ļ���·��

dir_type
�ļ������ͣ�char,chn,digi,digi_char

xml_path
xml�ļ�����·��

******************************************/
void IdentifyCharSVM::setTrainFile(string dir_path,string xml_path,string dir_type)
{
	PlateChar plate;
	Mat mat_trainingData5;
	Mat mat_trainingData10;
	Mat mat_trainingData15;
	Mat mat_trainingData20;
	Mat data_mat,res_mat;
	vector<int> resLabels;

	string dir_tmp=dir_path+"/"+dir_type;

	//ʹ��c++�ļ�����ϵͳ
    string strfind = dir_tmp + "/*";
	long hFile;  
    _finddata_t fileinfo; 
	if ((hFile=_findfirst(strfind.c_str(),&fileinfo)) == -1)  
    {  
		cout<<"error IdentifyCharSVM::setTrainFile can not match the folder path"<<endl;
		return;
    }

	int k=-1;
    do  
    {  
        //����ǲ���Ŀ¼  
        if (fileinfo.attrib && _A_SUBDIR)  
        {  
			if( (strcmp(fileinfo.name,".") != 0 ) &&(strcmp(fileinfo.name,"..") != 0))   
			{
				k++;
				string newPath = dir_tmp + "/" + fileinfo.name;
				//cout<<"FileInfo.name:"<<FileInfo1.name<<endl;
				//cout<<"newPath:"<<newPath<<endl;
				string fileExtension = "jpg";  
				string fileFolder = newPath + "/*." + fileExtension;  
				// �����ļ���  
				char filename[260];  
				struct _finddata_t fileinfo2;    // �ļ���Ϣ�ṹ��  
				long hFile2;   
				if ((hFile2=_findfirst(fileFolder.c_str(),&fileinfo2)) != -1)  
				{
					//ѭ������  
					do   
					{  
						sprintf(filename, "%s/%s", newPath.c_str(), fileinfo2.name);  
						//cout<<"newPath.c_str()��"<<newPath.c_str()<<endl;
						//cout<<"fileInfo.attrib��"<<fileInfo.attrib<<endl;
						//cout<<"fileInfo2.name��"<<fileInfo2.name<<endl;
						if ( fileinfo2.attrib == _A_ARCH)  // �Ǵ浵�����ļ�  
						{  
							//cout<<"filename: "<<filename<<endl;
							//string filename_str=string(filename);
							//Mat mat_img=imread(filename_str,0);//��ȡ��ͨ��
							//imread/imwrite�ڱ�����ʧЧ
							IplImage *pImg_tmp=cvLoadImage(filename,0);//��ȡ��ͨ��
							Mat mat_img=pImg_tmp;
							if(mat_img.empty())
							{
								cout<<"error IdentifyCharSVM::setTrainFile mat_img is empty()"<<endl;
								break;
							}
						
							Mat f5=plate.getFeatureHist(mat_img,5);
							Mat f10=plate.getFeatureHist(mat_img,10);
							Mat f15=plate.getFeatureHist(mat_img,15);
							Mat f20=plate.getFeatureHist(mat_img,20);

							mat_trainingData5.push_back(f5);  
							mat_trainingData10.push_back(f10);  
							mat_trainingData15.push_back(f15);  
							mat_trainingData20.push_back(f20);

							data_mat.push_back(f20);
							//ÿһ���ַ�ͼƬ����Ӧ���ַ���������±�  
							resLabels.push_back(k);//һ��Ŀ¼��һ������
						}  
  
					} while (!_findnext(hFile2, &fileinfo2));    
					_findclose(hFile2);
				}
			}
        }  
    } while (_findnext(hFile,&fileinfo) == 0);  
    _findclose(hFile);  

	if(data_mat.empty())
	{
		cout<<"setTrainFile fail to match the folder path"<<endl;
		return;
	}

	//���Ų�ת��������һ���������� src.convertTo(dst, type, scale, shift) 
	mat_trainingData5.convertTo(mat_trainingData5, CV_32FC1);  
    mat_trainingData10.convertTo(mat_trainingData10, CV_32FC1);  
    mat_trainingData15.convertTo(mat_trainingData15, CV_32FC1);  
    mat_trainingData20.convertTo(mat_trainingData20, CV_32FC1); 
	data_mat.convertTo(data_mat, CV_32FC1); 
	Mat(resLabels).copyTo(res_mat);  
	//��ν��ĸ�����ת����һ��data_mat?

	/********************************SVM CODE**************************************/
	//1)����ѵ��������
    /**
	��Ҫ�������ݣ�һ�������ݵ����res[]��һ�������ݵ�������Ϣdata[]��
	**/
	CvSVM svm;
	CvMat data_cvmat=data_mat;//��¼����������Ϣ
	CvMat res_cvmat=res_mat;//��¼�������
    
	//2������SVM����
	/**
	��ϸ˵�����£�
	**/
	CvSVMParams params;
	params.svm_type  = CvSVM::C_SVC;//��ʾSVM����
									//CvSVM����C_SVC  C-SVC

									//CvSVM����NU_SVC v-SVC

									//CvSVM����ONE_CLASS һ��SVM

									//CvSVM����EPS_SVR e-SVR

									//CvSVM����NU_SVR v-SVR
    params.kernel_type = CvSVM::RBF;//��ʾ�˺���������
										//CvSVM����LINEAR ���ԣ�u��v

										//CvSVM����POLY ����ʽ��(r*u'v + coef0)^degree

										//CvSVM����RBF RBF������exp(-r|u-v|^2)

										//CvSVM����SIGMOID sigmoid������tanh(r*u'v + coef0)

	params.degree =10.0; //�ں˺�����POLY���Ĳ���degree
	
	params.gamma =0.09; //�ں˺�����POLY/ RBF/ SIGMOID���Ĳ���y��
	
	params.coef0 =1.0; //�ں˺�����POLY/ SIGMOID���Ĳ���coef0��
	
	params.C =10.0; //SVM���ͣ�C_SVC/ EPS_SVR/ NU_SVR���Ĳ���C��
	
	params.nu =0.5; //SVM���ͣ�NU_SVC/ ONE_CLASS/ NU_SVR���Ĳ���v��
	
	params.p =1.0; //SVM���ͣ�EPS_SVR���Ĳ���e��

	params.class_weights =0;//C_SVC�еĿ�ѡȨ�أ�����ָ�����࣬����C�Ժ��� class\_weights_i * C��������ЩȨ��Ӱ�첻ͬ���Ĵ������ͷ��Ȩ��Խ��ĳһ������������ݵĳͷ����Խ��

    params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100, FLT_EPSILON);//SVM�ĵ���ѵ�����̵���ֹ���������������Լ�������������⡣

	//3��ѵ��SVM
	/**
	����CvSVM����train��������SVMģ�ͣ���һ������Ϊѵ�����ݣ��ڶ�������Ϊ�����������һ��������CvSVMParams
	**/
	svm.train(&data_cvmat,&res_cvmat,NULL,NULL,params);
	//cout<<"dir_type:"<<dir_type<<endl;
	//��Ҫ��stringת��Ϊchar[]
	string xmlpath=xml_path+"/"+dir_type+".xml";
	const char *xml_tmp=xmlpath.c_str();
	char *xmlname=(char*)xml_tmp;
	//sprintf(xmlname, "%s.xml",xmlname); 
	svm.save(xmlname);
	//cout<<"xmlname:"<<xmlname<<endl;
	cout<<"setTrainFile save xml success"<<endl;
}

/****************************************
getPredictPosition
��ȡ������ߵķ����λ��

pImg_src
ԴͼƬ

xml_path
xml�ļ�����·��

dir_type
�ļ������ͣ�char,chn,digi,digi_char

******************************************/
int IdentifyCharSVM::getPredictPosition(IplImage *pImg_src,string xml_path,string dir_type)
{
	PlateChar plate;
	CvSVM svm;
	CvMat sample_cvmat;
	Mat sample_mat,img_mat;
	Mat mat_trainingData5;
	Mat mat_trainingData10;
	Mat mat_trainingData15;
	Mat mat_trainingData20;
	//��Ҫ��stringת��Ϊchar[]
	string xmlpath=xml_path+"/"+dir_type+".xml";
	const char *xml_tmp=xmlpath.c_str();
	char *xmlname=(char*)xml_tmp;

	//��ȡ������Ϣ
	svm.load(xmlname);

	img_mat=pImg_src;
	if(img_mat.empty())
	{
		cout<<"error IdentifyCharSVM::getPredictPosition img_mat is empty()"<<endl;
		return 0;
	}
	
	Mat f5=plate.getFeatureHist(img_mat,5);
	Mat f10=plate.getFeatureHist(img_mat,10);
	Mat f15=plate.getFeatureHist(img_mat,15);
	Mat f20=plate.getFeatureHist(img_mat,20);

	mat_trainingData5.push_back(f5);  
	mat_trainingData10.push_back(f10);  
	mat_trainingData15.push_back(f15);  
	mat_trainingData20.push_back(f20);
	//�������4������
	sample_mat.push_back(f20);
	sample_mat.convertTo(sample_mat, CV_32FC1); //���Ų�ת��������һ���������� src.convertTo(dst, type, scale, shift) 

	sample_cvmat=sample_mat;

	//4�������SVM���з���
	/**
	���ú���CvSVM����predictʵ�ַ���
	**/
	return (int)svm.predict(&sample_cvmat);
}



