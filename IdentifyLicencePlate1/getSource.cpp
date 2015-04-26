#include "StdAfx.h"
#include "getSource.h"

//图片文件等资源路径
char* getSource::IMG_PATH="../img_src/licencePlate4.jpg";
string getSource::FILE_PATH="../file_src/mat.txt";

getSource::getSource(void)
{
}

void getSource::setImgpath(char*  img_path)
{
	IMG_PATH=img_path;
}

char*  getSource::getImgpath()
{
	return IMG_PATH;
}

string getSource::getFilepath()
{
	return FILE_PATH;
}

void getSource::setFilepath(string file_path)
{
	FILE_PATH=file_path;
}


getSource::~getSource(void)
{
}
