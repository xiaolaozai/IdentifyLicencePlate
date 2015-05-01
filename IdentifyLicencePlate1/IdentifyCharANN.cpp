#include "StdAfx.h"
#include "IdentifyCharANN.h"
#include "PlateChar.h"


IdentifyCharANN::IdentifyCharANN(void)
{
}


IdentifyCharANN::~IdentifyCharANN(void)
{
}

//测试读取文件目录及文件
void IdentifyCharANN::testRead()
{
	//出错，不支持opencv的Directory
//	 //循环读取文件  
//	 string dir_path = "F:/char";    
//	 Directory dir;    
//	 vector<std::string> filepath = dir.GetListFolders(dir_path);  
//	 for (int i = 0;i<filepath.size();i++)  
//	 {  
//		cout<<"filepath[i]:"<<filepath[i]<<endl;
//		vector<std::string> fileNames = dir.GetListFiles(filepath[i], "*.jpg", false);    
//		for(int j=0; j < fileNames.size(); j++)    
//		{    
//		string fileName = fileNames[j];  //文件名  
//		string fileFullName = filepath[i] + '/' + fileName;  //文件具体路径  
//		cout<<"fileFullName:"<<fileFullName<<endl;
///*	   Mat src = imread(fileFullName ,0);  
//		threshold(src,src,125,255,THRESH_OTSU);  
//		imshow("src",src);  
//		waitKey(0);*/  
//		}    
//	 }  



	//string fileFolderPath = "../img_model/char/A";  
 //   string fileExtension = "jpg";  
 //     
 //   string fileFolder = fileFolderPath + "/*." + fileExtension;  
	//// 遍历文件夹  
 //   char fileName[2000];  
 // 
 //   struct _finddata_t fileInfo;    // 文件信息结构体  
	//
 //   // 1. 第一次查找  
 //   long findResult = _findfirst(fileFolder.c_str(), &fileInfo);   
	//cout<<"fileFolder.c_str()："<<fileFolder.c_str()<<endl;
 //   if (findResult == -1)  
 //   {  
 //       _findclose(findResult);
	//	cout<<"findResult == -1"<<endl;
 //       return;  
 //   }  
 //     
 //   // 2. 循环查找  
 //   do   
 //   {  
 //       sprintf(fileName, "%s/%s", fileFolderPath.c_str(), fileInfo.name);  
	//	cout<<"fileFolderPath.c_str()："<<fileFolderPath.c_str()<<endl;
	//	//cout<<"fileInfo.attrib："<<fileInfo.attrib<<endl;
	//	cout<<"fileInfo.name："<<fileInfo.name<<endl;
 //       if ( fileInfo.attrib == _A_ARCH)  // 是存档类型文件  
 //       {  
 //           cout<<"fileName: "<<fileName<<endl;  
 //       }  
 // 
 //   } while (!_findnext(findResult, &fileInfo));    
 // 
 // 
 //   _findclose(findResult);   

	string folderPath = "../img_model/chn";
	_finddata_t FileInfo;
    string strfind = folderPath + "/*";
    long Handle = _findfirst(strfind.c_str(), &FileInfo);
    if (Handle == -1L)
    {
        cerr << "can not match the folder path" << endl;
        return;
    }
    do{
        //判断是否有子目录
        if (FileInfo.attrib && _A_SUBDIR)    
        {
            //这个语句很重要
            if( (strcmp(FileInfo.name,".") != 0 ) &&(strcmp(FileInfo.name,"..") != 0))   
            {
                string newPath = folderPath + "/" + FileInfo.name;
				cout<<"FileInfo.name:"<<FileInfo.name<<endl;
				cout<<"newPath:"<<newPath<<endl;
				string fileExtension = "jpg";  
				string fileFolder = newPath + "/*." + fileExtension;  
				// 遍历文件夹  
				char filename[260];  
  
				struct _finddata_t fileInfo2;    // 文件信息结构体  
	
				// 1. 第一次查找  
				long Handle2 = _findfirst(fileFolder.c_str(), &fileInfo2);   
				//cout<<"fileFolder.c_str()："<<fileFolder.c_str()<<endl;
				if (Handle2 == -1)  
				{  
					_findclose(Handle2);
					cout<<"Handle2 == -1"<<endl;
					return;  
				}  
      
				// 2. 循环查找  
				do   
				{  
					sprintf(filename, "%s/%s", newPath.c_str(), fileInfo2.name);  
					//cout<<"newPath.c_str()："<<newPath.c_str()<<endl;
					//cout<<"fileInfo.attrib："<<fileInfo.attrib<<endl;
					cout<<"fileInfo2.name："<<fileInfo2.name<<endl;
					if ( fileInfo2.attrib == _A_ARCH)  // 是存档类型文件  
					{  
						cout<<"filename: "<<filename<<endl;  
					}  
  
				} while (!_findnext(Handle2, &fileInfo2));    
				_findclose(Handle2); 
            }
        }
    }while (_findnext(Handle, &FileInfo) == 0);

    _findclose(Handle);
  
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
void IdentifyCharANN::setTrainFile(string dir_path, string dir_type, string xml_path)
{
	PlateChar plate;
	cv::Directory dir;
	Mat mat_classes;
	Mat mat_trainingData5;
	Mat mat_trainingData10;
	Mat mat_trainingData15;
	Mat mat_trainingData20;
	vector<int> trainingLabels;
	string dir_tmp=dir_path+"/"+dir_type;

	//使用c++文件处理系统
	_finddata_t FileInfo1;
    string strfind = dir_tmp + "/*";
    long Handle1 = _findfirst(strfind.c_str(), &FileInfo1);
    if (Handle1 == -1L)
    {
        cerr << "can not match the folder path" << endl;
        return;
    }
	int k=0;
    do{
        //判断是否有子目录
        if (FileInfo1.attrib && _A_SUBDIR)    
        {
            //这个语句很重要
            if( (strcmp(FileInfo1.name,".") != 0 ) &&(strcmp(FileInfo1.name,"..") != 0))   
            {
                string newPath = dir_tmp + "/" + FileInfo1.name;
				cout<<"FileInfo.name:"<<FileInfo1.name<<endl;
				cout<<"newPath:"<<newPath<<endl;
				string fileExtension = "jpg";  
				string fileFolder = newPath + "/*." + fileExtension;  
				// 遍历文件夹  
				char filename[260];  
  
				struct _finddata_t fileInfo2;    // 文件信息结构体  
	
				// 1. 第一次查找  
				long Handle2 = _findfirst(fileFolder.c_str(), &fileInfo2);   
				//cout<<"fileFolder.c_str()："<<fileFolder.c_str()<<endl;
				if (Handle2 == -1)  
				{  
					_findclose(Handle2);
					cout<<"Handle2 == -1"<<endl;
					break;  
				}  
      
				// 2. 循环查找  
				do   
				{  
					sprintf(filename, "%s/%s", newPath.c_str(), fileInfo2.name);  
					//cout<<"newPath.c_str()："<<newPath.c_str()<<endl;
					//cout<<"fileInfo.attrib："<<fileInfo.attrib<<endl;
					//cout<<"fileInfo2.name："<<fileInfo2.name<<endl;
					if ( fileInfo2.attrib == _A_ARCH)  // 是存档类型文件  
					{  
						cout<<"filename: "<<filename<<endl;
						//string filename_str=string(filename);
						//Mat mat_img=imread(filename_str,0);//读取单通道
						//imread/imwrite在本机已失效
						IplImage *pImg_tmp=cvLoadImage(filename,0);//读取单通道
						Mat mat_img=pImg_tmp;
						if(mat_img.empty())
						{
							cout<<"mat_img is empty()"<<endl;
							break;
						}
						else
						{
							cout<<"mat_img is not empty()"<<endl;
						}
						Mat f5=plate.getFeatureHist(mat_img,5);
						Mat f10=plate.getFeatureHist(mat_img,10);
						Mat f15=plate.getFeatureHist(mat_img,15);
						Mat f20=plate.getFeatureHist(mat_img,20);

						mat_trainingData5.push_back(f5);  
						mat_trainingData10.push_back(f10);  
						mat_trainingData15.push_back(f15);  
						mat_trainingData20.push_back(f20);
						//每一幅字符图片所对应的字符类别索引下标  
						trainingLabels.push_back(k);//一个目录即一个分类
					}  
  
				} while (!_findnext(Handle2, &fileInfo2));    
				_findclose(Handle2); 
            }
        }
		k++;//目录代表分类,一个目录即一个分类
    }while (_findnext(Handle1, &FileInfo1) == 0);

    _findclose(Handle1);


	////遍历指定文件夹下的所有文件夹，不包括指定文件夹下的文件
	//vector<string> fileFolders = dir.GetListFolders(dir_tmp);

	//for(int i=0;i<fileFolders.size();i++)
	//{
	//	//遍历指定文件夹下的所有文件，不包括指定文件夹内的文件夹
	//	vector<string> fileNames = dir.GetListFiles(fileFolders[i], "*.jpg", false);
	//	for(int j=0;j<fileNames.size();j++)
	//	{
	//		string fileFullName = fileFolders[i] + "/" + fileNames[j];  //文件具体路径  
	//		Mat mat_img=imread(fileFullName,0);
	//		Mat f5=plate.getFeatureHist(mat_img,5);
	//		Mat f10=plate.getFeatureHist(mat_img,10);
	//		Mat f15=plate.getFeatureHist(mat_img,15);
	//		Mat f20=plate.getFeatureHist(mat_img,20);

	//		mat_trainingData5.push_back(f5);  
 //           mat_trainingData10.push_back(f10);  
 //           mat_trainingData15.push_back(f15);  
 //           mat_trainingData20.push_back(f20);
	//		//每一幅字符图片所对应的字符类别索引下标  
 //           trainingLabels.push_back(i);  
	//	}
	//}

	mat_trainingData5.convertTo(mat_trainingData5, CV_32FC1);  
    mat_trainingData10.convertTo(mat_trainingData10, CV_32FC1);  
    mat_trainingData15.convertTo(mat_trainingData15, CV_32FC1);  
    mat_trainingData20.convertTo(mat_trainingData20, CV_32FC1);  
    Mat(trainingLabels).copyTo(mat_classes);  
  
	//char xmlname[200];
	//sprintf(xmlname, "%s/%s.xml", xml_path, dir_type); 
	string xmlname=xml_path+"/"+dir_type+".xml";
    FileStorage fs("OCR.xml", FileStorage::WRITE);
    fs << "TrainingDataF5" << mat_trainingData5;  
    fs << "TrainingDataF10" << mat_trainingData10;  
    fs << "TrainingDataF15" << mat_trainingData15;  
    fs << "TrainingDataF20" << mat_trainingData20;  
    fs << "classes" << mat_classes;  
    fs.release();  
}

/****************************************
getTrain
获取ann分类器

mlp
CvANN_MLP类

mat_trainData
训练集数据

mat_classes
训练集分类

nClass
训练集分类数目，神经网络输出层节点数

nHiddenLayer
神经网络隐含层节点数

******************************************/
CvANN_MLP IdentifyCharANN::getTrain(CvANN_MLP mlp,Mat mat_trainData, Mat mat_classes, int nClass, int nHiddenLayer)
{
	Mat layerSizes(1,3,CV_32SC1);//一个3层的神经网络
    layerSizes.at<int>(0)= mat_trainData.cols;//输入层 第一层节点数
    layerSizes.at<int>(1)= nHiddenLayer; //一个隐含层 第二层节点数 
    layerSizes.at<int>(2)= nClass;  //输出层 第三层节点数
	//创建神经网络
    mlp.create(layerSizes, CvANN_MLP::SIGMOID_SYM, 1, 1);//CvANN_MLP::SIGMOID_SYM  
														//CvANN_MLP::GAUSSIAN  
													   //CvANN_MLP::IDENTITY 

	Mat mat_trainClasses;
	mat_trainClasses.create(mat_trainData.rows,nClass,CV_32FC1);
	for(int i=0;i<mat_trainData.rows;i++)
	{
		for(int j=0;j<mat_trainData.cols;j++)
		{
			if(j==mat_classes.at<int>(i))
			{
				mat_trainClasses.at<float>(i,j)=1;
			}
			else
			{
				mat_trainClasses.at<float>(i,j)=0;
			}
		}
	}
	Mat mat_sampleWeights(1,mat_trainData.rows,CV_32FC1,Scalar::all(1));

	//训练分类
	mlp.train(mat_trainData,mat_trainClasses,mat_sampleWeights);
	return mlp;
}

/****************************************
getPredictPosition
获取神经网络概率最高的分类的位置

mlp
CvANN_MLP类

mat_model
预测模型

nClass
训练集分类数目，神经网络输出层节点数

******************************************/
int IdentifyCharANN::getPredictPosition(CvANN_MLP mlp,Mat mat_model,int nClass)
{
	//根据类型数量创建输出分类
	Mat mat_output(1,nClass,CV_32FC1);
	mlp.predict(mat_model,mat_output);//神经网络预测
	Point maxLoc;
	double maxVal;//获取概率最高的分类 
    minMaxLoc(mat_output, 0, &maxVal, 0, &maxLoc); 
	//返回概率最高分类的位置
	return maxLoc.x;
}
