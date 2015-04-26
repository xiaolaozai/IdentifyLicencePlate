#include "StdAfx.h"
#include "getSource.h"
#include "FileOperation.h"

//文件操作
FileOperation::FileOperation(void)
{
}

void FileOperation::writeInFile(Mat mat)
{
	 // 打开文件  
	getSource Src;
	string fileName=Src.getFilepath();
    ofstream outFile(fileName.c_str(), ios_base::out);  //按新建或覆盖方式写入  
    if (!outFile.is_open())  
    {  
        cout << "打开文件失败" << endl;   
    }

	// 检查矩阵是否为空  
	if (mat.empty())  
	{  
		cout << "矩阵为空" << endl;   
	}  
  
	// 写入数据  
	for (int r = 0; r < mat.rows; r++)  
	{  
		for (int c = 0; c < mat.cols; c++)  
		{  
			int data = mat.at<uchar>(r,c);  //读取数据，at<type> - type 是矩阵元素的具体数据格式  
			outFile << data << "\t" ;   //每列数据用 tab 隔开  
		}  
		outFile << endl;  //换行  
	}
	outFile << endl;  //换行 
	outFile.close(); 
}

FileOperation::~FileOperation(void)
{
}
