#include "StdAfx.h"
#include "CompareStruct.h"


CompareStruct::CompareStruct(void)
{
}


CompareStruct::~CompareStruct(void)
{
}

//�ṹ�ȽϺ��������սṹ�е�xֵ��������  
bool CompareStruct::compare_struct_CvRect(const CvRect &a,const CvRect &b)
{  
	return a.x<b.x;  
}  