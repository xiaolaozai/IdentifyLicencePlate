#include "StdAfx.h"
#include "CompareStruct.h"


CompareStruct::CompareStruct(void)
{
}


CompareStruct::~CompareStruct(void)
{
}

//结构比较函数（按照结构中的x值进行排序）  
bool CompareStruct::compare_struct_CvRect(const CvRect &a,const CvRect &b)
{  
	return a.x<b.x;  
}  