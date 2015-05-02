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
	//1）设置训练样本集
    /**
	需要两组数据，一组是数据的类别res[]，一组是数据的向量信息data[]。
	**/
	cvInitMatHeader(&data_mat,s,2,CV_32FC1,data);
	cvInitMatHeader(&res_mat,s,1,CV_32SC1,res);
	
	//2）设置SVM参数
	/**
	利用CvSVMParams类实现类内的成员变量svm_type表示SVM类型：

	CvSVM：：C_SVC  C-SVC

	CvSVM：：NU_SVC v-SVC

	CvSVM：：ONE_CLASS 一类SVM

	CvSVM：：EPS_SVR e-SVR

	CvSVM：：NU_SVR v-SVR

	成员变量kernel_type表示核函数的类型：

	CvSVM：：LINEAR 线性：u‘v

	CvSVM：：POLY 多项式：(r*u'v + coef0)^degree

	CvSVM：：RBF RBF函数：exp(-r|u-v|^2)

	CvSVM：：SIGMOID sigmoid函数：tanh(r*u'v + coef0)

	成员变量degree针对多项式核函数degree的设置，
	gamma针对多项式/rbf/sigmoid核函数的设置，
	coef0针对多项式/sigmoid核函数的设置，
	Cvalue为损失函数，在C-SVC、e-SVR、v-SVR中有效，nu设置v-SVC、一类SVM和v-SVR参数，
	p为设置e-SVR中损失函数的值，class_weightsC_SVC的权重，
	term_crit为SVM训练过程的终止条件。
	其中默认值degree = 0，gamma = 1，coef0 = 0，Cvalue = 1，nu = 0，p = 0，class_weights = 0
	**/
	criteria=cvTermCriteria(CV_TERMCRIT_EPS,1000,FLT_EPSILON);//停止迭代的标准,传入
	param=CvSVMParams(CvSVM::C_SVC,CvSVM::RBF,10.0,8.0,1.0,10.0,0.5,0.1,NULL,criteria);

	//3）训练SVM
	/**
	调用CvSVM：：train函数建立SVM模型，第一个参数为训练数据，第二个参数为分类结果，最后一个参数即CvSVMParams
	**/
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
			//4）用这个SVM进行分类
			/**
			调用函数CvSVM：：predict实现分类
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

	//5）获得支持向量
	/**
	除了分类，也可以得到SVM的支持向量，调用函数CvSVM：：get_support_vector_count获得支持向量的个数，CvSVM：：get_support_vector获得对应的索引编号的支持向量。
	**/
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

/****************************************
plateSVMTrain
设置车牌训练集文件

dir_path
文件夹路径

xml_path
xml文件保存路径

dir_type
文件夹类型，plate

******************************************/
void IdentifyCharSVM::plateSVMTrain(string dir_path,string xml_path,string dir_type)
{
	int i,j,ii,jj;
	PlateChar plate;
	Mat data_mat,res_mat;
	vector<int> resLabels;

	//1)设置训练样本集
    /**
	需要两组数据，一组是数据的类别res[]，一组是数据的向量信息data[]。
	**/
	string dir_tmp=dir_path+"/"+dir_type;
	//使用c++文件处理系统
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
        //判断是否有子目录
        if (FileInfo1.attrib && _A_SUBDIR)    
        {
            //这个语句很重要
            if( (strcmp(FileInfo1.name,".") != 0 ) &&(strcmp(FileInfo1.name,"..") != 0))   
            {
				k++;
                string newPath = dir_tmp + "/" + FileInfo1.name;
				string fileExtension = "jpg";  
				string fileFolder = newPath + "/*." + fileExtension;  
				// 遍历文件夹  
				char filename[260];  
  
				struct _finddata_t fileInfo2;    // 文件信息结构体  
	
				// 1. 第一次查找  
				long Handle2 = _findfirst(fileFolder.c_str(), &fileInfo2);   
				if (Handle2 == -1L)  
				{  
					_findclose(Handle2);
					cerr << "error IdentifyCharSVM::plateSVMTrain can not match the folder path Handle2" << endl;
					break;  
				}  
      
				// 2. 循环查找  
				do  
				{  
					sprintf(filename, "%s/%s", newPath.c_str(), fileInfo2.name);  
					if ( fileInfo2.attrib == _A_ARCH)  // 是存档类型文件  
					{  
						IplImage *pImg_tmp=cvLoadImage(filename,0);//读取单通道
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
						//一个目录即一个分类 
						resLabels.push_back(k);//正集数值为1,负集数值为0
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
	CvMat data_cvmat=data_mat;//记录数据向量信息
	CvMat res_cvmat=res_mat;//记录数据类别

	//2）设置SVM参数
	/**
	详细说明如下：
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

	CvTermCriteria criteria=cvTermCriteria(CV_TERMCRIT_EPS,1000,FLT_EPSILON);//停止迭代的标准,传入
	CvSVMParams params=CvSVMParams(CvSVM::C_SVC,CvSVM::RBF,10.0,8.0,1.0,10.0,0.5,0.1,NULL,criteria);


	//3）训练SVM
	/**
	调用CvSVM：：train函数建立SVM模型，第一个参数为训练数据，第二个参数为分类结果，最后一个参数即CvSVMParams
	**/
	svm.train(&data_cvmat,&res_cvmat,NULL,NULL,params);

	//cout<<"dir_type:"<<dir_type<<endl;
	//需要对string转化为char[]
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
获取概率最高的分类的位置

pImg_src
源图片单通道

xml_path
xml文件保存路径

dir_type
文件夹类型，plate

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
	//需要对string转化为char[]
	string xmlpath=xml_path+"/"+dir_type+".xml";
	const char *xml_tmp=xmlpath.c_str();
	char *xmlname=(char*)xml_tmp;

	//读取样本信息
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

	//4）用这个SVM进行分类
	/**
	调用函数CvSVM：：predict实现分类
	**/
	return (int)svm.predict(&sample_cvmat);
}

/****************************************
setTrainFile
设置训练集文件

dir_path
文件夹路径

dir_type
文件夹类型，char,chn,digi,digi_char

xml_path
xml文件保存路径

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

	//使用c++文件处理系统
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
        //检查是不是目录  
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
				// 遍历文件夹  
				char filename[260];  
				struct _finddata_t fileinfo2;    // 文件信息结构体  
				long hFile2;   
				if ((hFile2=_findfirst(fileFolder.c_str(),&fileinfo2)) != -1)  
				{
					//循环查找  
					do   
					{  
						sprintf(filename, "%s/%s", newPath.c_str(), fileinfo2.name);  
						//cout<<"newPath.c_str()："<<newPath.c_str()<<endl;
						//cout<<"fileInfo.attrib："<<fileInfo.attrib<<endl;
						//cout<<"fileInfo2.name："<<fileInfo2.name<<endl;
						if ( fileinfo2.attrib == _A_ARCH)  // 是存档类型文件  
						{  
							//cout<<"filename: "<<filename<<endl;
							//string filename_str=string(filename);
							//Mat mat_img=imread(filename_str,0);//读取单通道
							//imread/imwrite在本机已失效
							IplImage *pImg_tmp=cvLoadImage(filename,0);//读取单通道
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
							//每一幅字符图片所对应的字符类别索引下标  
							resLabels.push_back(k);//一个目录即一个分类
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

	//缩放并转换到另外一种数据类型 src.convertTo(dst, type, scale, shift) 
	mat_trainingData5.convertTo(mat_trainingData5, CV_32FC1);  
    mat_trainingData10.convertTo(mat_trainingData10, CV_32FC1);  
    mat_trainingData15.convertTo(mat_trainingData15, CV_32FC1);  
    mat_trainingData20.convertTo(mat_trainingData20, CV_32FC1); 
	data_mat.convertTo(data_mat, CV_32FC1); 
	Mat(resLabels).copyTo(res_mat);  
	//如何将四个矩阵转化成一个data_mat?

	/********************************SVM CODE**************************************/
	//1)设置训练样本集
    /**
	需要两组数据，一组是数据的类别res[]，一组是数据的向量信息data[]。
	**/
	CvSVM svm;
	CvMat data_cvmat=data_mat;//记录数据向量信息
	CvMat res_cvmat=res_mat;//记录数据类别
    
	//2）设置SVM参数
	/**
	详细说明如下：
	**/
	CvSVMParams params;
	params.svm_type  = CvSVM::C_SVC;//表示SVM类型
									//CvSVM：：C_SVC  C-SVC

									//CvSVM：：NU_SVC v-SVC

									//CvSVM：：ONE_CLASS 一类SVM

									//CvSVM：：EPS_SVR e-SVR

									//CvSVM：：NU_SVR v-SVR
    params.kernel_type = CvSVM::RBF;//表示核函数的类型
										//CvSVM：：LINEAR 线性：u‘v

										//CvSVM：：POLY 多项式：(r*u'v + coef0)^degree

										//CvSVM：：RBF RBF函数：exp(-r|u-v|^2)

										//CvSVM：：SIGMOID sigmoid函数：tanh(r*u'v + coef0)

	params.degree =10.0; //内核函数（POLY）的参数degree
	
	params.gamma =0.09; //内核函数（POLY/ RBF/ SIGMOID）的参数y。
	
	params.coef0 =1.0; //内核函数（POLY/ SIGMOID）的参数coef0。
	
	params.C =10.0; //SVM类型（C_SVC/ EPS_SVR/ NU_SVR）的参数C。
	
	params.nu =0.5; //SVM类型（NU_SVC/ ONE_CLASS/ NU_SVR）的参数v。
	
	params.p =1.0; //SVM类型（EPS_SVR）的参数e。

	params.class_weights =0;//C_SVC中的可选权重，赋给指定的类，乘以C以后变成 class\_weights_i * C。所以这些权重影响不同类别的错误分类惩罚项。权重越大，某一类别的误分类数据的惩罚项就越大。

    params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100, FLT_EPSILON);//SVM的迭代训练过程的中止条件，解决部分受约束二次最优问题。

	//3）训练SVM
	/**
	调用CvSVM：：train函数建立SVM模型，第一个参数为训练数据，第二个参数为分类结果，最后一个参数即CvSVMParams
	**/
	svm.train(&data_cvmat,&res_cvmat,NULL,NULL,params);
	//cout<<"dir_type:"<<dir_type<<endl;
	//需要对string转化为char[]
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
获取概率最高的分类的位置

pImg_src
源图片

xml_path
xml文件保存路径

dir_type
文件夹类型，char,chn,digi,digi_char

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
	//需要对string转化为char[]
	string xmlpath=xml_path+"/"+dir_type+".xml";
	const char *xml_tmp=xmlpath.c_str();
	char *xmlname=(char*)xml_tmp;

	//读取样本信息
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
	//如何整合4个矩阵？
	sample_mat.push_back(f20);
	sample_mat.convertTo(sample_mat, CV_32FC1); //缩放并转换到另外一种数据类型 src.convertTo(dst, type, scale, shift) 

	sample_cvmat=sample_mat;

	//4）用这个SVM进行分类
	/**
	调用函数CvSVM：：predict实现分类
	**/
	return (int)svm.predict(&sample_cvmat);
}



