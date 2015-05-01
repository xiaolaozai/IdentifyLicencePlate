#include "StdAfx.h"
#include "IdentifyCharANN.h"
#include "PlateChar.h"


IdentifyCharANN::IdentifyCharANN(void)
{
}


IdentifyCharANN::~IdentifyCharANN(void)
{
}

//���Զ�ȡ�ļ�Ŀ¼���ļ�
void IdentifyCharANN::testRead()
{
	//������֧��opencv��Directory
//	 //ѭ����ȡ�ļ�  
//	 string dir_path = "F:/char";    
//	 Directory dir;    
//	 vector<std::string> filepath = dir.GetListFolders(dir_path);  
//	 for (int i = 0;i<filepath.size();i++)  
//	 {  
//		cout<<"filepath[i]:"<<filepath[i]<<endl;
//		vector<std::string> fileNames = dir.GetListFiles(filepath[i], "*.jpg", false);    
//		for(int j=0; j < fileNames.size(); j++)    
//		{    
//		string fileName = fileNames[j];  //�ļ���  
//		string fileFullName = filepath[i] + '/' + fileName;  //�ļ�����·��  
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
	//// �����ļ���  
 //   char fileName[2000];  
 // 
 //   struct _finddata_t fileInfo;    // �ļ���Ϣ�ṹ��  
	//
 //   // 1. ��һ�β���  
 //   long findResult = _findfirst(fileFolder.c_str(), &fileInfo);   
	//cout<<"fileFolder.c_str()��"<<fileFolder.c_str()<<endl;
 //   if (findResult == -1)  
 //   {  
 //       _findclose(findResult);
	//	cout<<"findResult == -1"<<endl;
 //       return;  
 //   }  
 //     
 //   // 2. ѭ������  
 //   do   
 //   {  
 //       sprintf(fileName, "%s/%s", fileFolderPath.c_str(), fileInfo.name);  
	//	cout<<"fileFolderPath.c_str()��"<<fileFolderPath.c_str()<<endl;
	//	//cout<<"fileInfo.attrib��"<<fileInfo.attrib<<endl;
	//	cout<<"fileInfo.name��"<<fileInfo.name<<endl;
 //       if ( fileInfo.attrib == _A_ARCH)  // �Ǵ浵�����ļ�  
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
        //�ж��Ƿ�����Ŀ¼
        if (FileInfo.attrib && _A_SUBDIR)    
        {
            //���������Ҫ
            if( (strcmp(FileInfo.name,".") != 0 ) &&(strcmp(FileInfo.name,"..") != 0))   
            {
                string newPath = folderPath + "/" + FileInfo.name;
				cout<<"FileInfo.name:"<<FileInfo.name<<endl;
				cout<<"newPath:"<<newPath<<endl;
				string fileExtension = "jpg";  
				string fileFolder = newPath + "/*." + fileExtension;  
				// �����ļ���  
				char filename[260];  
  
				struct _finddata_t fileInfo2;    // �ļ���Ϣ�ṹ��  
	
				// 1. ��һ�β���  
				long Handle2 = _findfirst(fileFolder.c_str(), &fileInfo2);   
				//cout<<"fileFolder.c_str()��"<<fileFolder.c_str()<<endl;
				if (Handle2 == -1)  
				{  
					_findclose(Handle2);
					cout<<"Handle2 == -1"<<endl;
					return;  
				}  
      
				// 2. ѭ������  
				do   
				{  
					sprintf(filename, "%s/%s", newPath.c_str(), fileInfo2.name);  
					//cout<<"newPath.c_str()��"<<newPath.c_str()<<endl;
					//cout<<"fileInfo.attrib��"<<fileInfo.attrib<<endl;
					cout<<"fileInfo2.name��"<<fileInfo2.name<<endl;
					if ( fileInfo2.attrib == _A_ARCH)  // �Ǵ浵�����ļ�  
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
����ѵ�����ļ�

dir_path
�ļ���·��

dir_type
�ļ������ͣ�char,chn,digi,digi_char

xml_path
xml�ļ�����·��

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

	//ʹ��c++�ļ�����ϵͳ
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
        //�ж��Ƿ�����Ŀ¼
        if (FileInfo1.attrib && _A_SUBDIR)    
        {
            //���������Ҫ
            if( (strcmp(FileInfo1.name,".") != 0 ) &&(strcmp(FileInfo1.name,"..") != 0))   
            {
                string newPath = dir_tmp + "/" + FileInfo1.name;
				cout<<"FileInfo.name:"<<FileInfo1.name<<endl;
				cout<<"newPath:"<<newPath<<endl;
				string fileExtension = "jpg";  
				string fileFolder = newPath + "/*." + fileExtension;  
				// �����ļ���  
				char filename[260];  
  
				struct _finddata_t fileInfo2;    // �ļ���Ϣ�ṹ��  
	
				// 1. ��һ�β���  
				long Handle2 = _findfirst(fileFolder.c_str(), &fileInfo2);   
				//cout<<"fileFolder.c_str()��"<<fileFolder.c_str()<<endl;
				if (Handle2 == -1)  
				{  
					_findclose(Handle2);
					cout<<"Handle2 == -1"<<endl;
					break;  
				}  
      
				// 2. ѭ������  
				do   
				{  
					sprintf(filename, "%s/%s", newPath.c_str(), fileInfo2.name);  
					//cout<<"newPath.c_str()��"<<newPath.c_str()<<endl;
					//cout<<"fileInfo.attrib��"<<fileInfo.attrib<<endl;
					//cout<<"fileInfo2.name��"<<fileInfo2.name<<endl;
					if ( fileInfo2.attrib == _A_ARCH)  // �Ǵ浵�����ļ�  
					{  
						cout<<"filename: "<<filename<<endl;
						//string filename_str=string(filename);
						//Mat mat_img=imread(filename_str,0);//��ȡ��ͨ��
						//imread/imwrite�ڱ�����ʧЧ
						IplImage *pImg_tmp=cvLoadImage(filename,0);//��ȡ��ͨ��
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
						//ÿһ���ַ�ͼƬ����Ӧ���ַ���������±�  
						trainingLabels.push_back(k);//һ��Ŀ¼��һ������
					}  
  
				} while (!_findnext(Handle2, &fileInfo2));    
				_findclose(Handle2); 
            }
        }
		k++;//Ŀ¼�������,һ��Ŀ¼��һ������
    }while (_findnext(Handle1, &FileInfo1) == 0);

    _findclose(Handle1);


	////����ָ���ļ����µ������ļ��У�������ָ���ļ����µ��ļ�
	//vector<string> fileFolders = dir.GetListFolders(dir_tmp);

	//for(int i=0;i<fileFolders.size();i++)
	//{
	//	//����ָ���ļ����µ������ļ���������ָ���ļ����ڵ��ļ���
	//	vector<string> fileNames = dir.GetListFiles(fileFolders[i], "*.jpg", false);
	//	for(int j=0;j<fileNames.size();j++)
	//	{
	//		string fileFullName = fileFolders[i] + "/" + fileNames[j];  //�ļ�����·��  
	//		Mat mat_img=imread(fileFullName,0);
	//		Mat f5=plate.getFeatureHist(mat_img,5);
	//		Mat f10=plate.getFeatureHist(mat_img,10);
	//		Mat f15=plate.getFeatureHist(mat_img,15);
	//		Mat f20=plate.getFeatureHist(mat_img,20);

	//		mat_trainingData5.push_back(f5);  
 //           mat_trainingData10.push_back(f10);  
 //           mat_trainingData15.push_back(f15);  
 //           mat_trainingData20.push_back(f20);
	//		//ÿһ���ַ�ͼƬ����Ӧ���ַ���������±�  
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
��ȡann������

mlp
CvANN_MLP��

mat_trainData
ѵ��������

mat_classes
ѵ��������

nClass
ѵ����������Ŀ�������������ڵ���

nHiddenLayer
������������ڵ���

******************************************/
CvANN_MLP IdentifyCharANN::getTrain(CvANN_MLP mlp,Mat mat_trainData, Mat mat_classes, int nClass, int nHiddenLayer)
{
	Mat layerSizes(1,3,CV_32SC1);//һ��3���������
    layerSizes.at<int>(0)= mat_trainData.cols;//����� ��һ��ڵ���
    layerSizes.at<int>(1)= nHiddenLayer; //һ�������� �ڶ���ڵ��� 
    layerSizes.at<int>(2)= nClass;  //����� ������ڵ���
	//����������
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

	//ѵ������
	mlp.train(mat_trainData,mat_trainClasses,mat_sampleWeights);
	return mlp;
}

/****************************************
getPredictPosition
��ȡ�����������ߵķ����λ��

mlp
CvANN_MLP��

mat_model
Ԥ��ģ��

nClass
ѵ����������Ŀ�������������ڵ���

******************************************/
int IdentifyCharANN::getPredictPosition(CvANN_MLP mlp,Mat mat_model,int nClass)
{
	//�����������������������
	Mat mat_output(1,nClass,CV_32FC1);
	mlp.predict(mat_model,mat_output);//������Ԥ��
	Point maxLoc;
	double maxVal;//��ȡ������ߵķ��� 
    minMaxLoc(mat_output, 0, &maxVal, 0, &maxLoc); 
	//���ظ�����߷����λ��
	return maxLoc.x;
}
