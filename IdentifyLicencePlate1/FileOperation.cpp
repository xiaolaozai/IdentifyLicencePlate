#include "StdAfx.h"
#include "getSource.h"
#include "FileOperation.h"

//�ļ�����
FileOperation::FileOperation(void)
{
}

void FileOperation::writeInFile(Mat mat)
{
	 // ���ļ�  
	getSource Src;
	string fileName=Src.getFilepath();
    ofstream outFile(fileName.c_str(), ios_base::out);  //���½��򸲸Ƿ�ʽд��  
    if (!outFile.is_open())  
    {  
        cout << "���ļ�ʧ��" << endl;   
    }

	// �������Ƿ�Ϊ��  
	if (mat.empty())  
	{  
		cout << "����Ϊ��" << endl;   
	}  
  
	// д������  
	for (int r = 0; r < mat.rows; r++)  
	{  
		for (int c = 0; c < mat.cols; c++)  
		{  
			int data = mat.at<uchar>(r,c);  //��ȡ���ݣ�at<type> - type �Ǿ���Ԫ�صľ������ݸ�ʽ  
			outFile << data << "\t" ;   //ÿ�������� tab ����  
		}  
		outFile << endl;  //����  
	}
	outFile << endl;  //���� 
	outFile.close(); 
}

FileOperation::~FileOperation(void)
{
}
