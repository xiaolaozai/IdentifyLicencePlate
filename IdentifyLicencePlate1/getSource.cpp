#include "StdAfx.h"
#include "getSource.h"

//图片文件等资源路径
char* getSource::IMG_PATH="../img_src/licencePlate1.jpg";
string getSource::FILE_PATH="../file_src/mat.txt";
string getSource::IMG_TRAIN_PATH="../img_model";
string getSource::XML_TRAIN_PATH="../xml";

getSource::getSource(void)
{
}

getSource::~getSource(void)
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


string getSource::getImageTrainPath()
{
	return IMG_TRAIN_PATH;
}

void getSource::setImageTrainPath(string file_path)
{
	IMG_TRAIN_PATH=file_path;
}


string getSource::getXMLTrainPath()
{
	return XML_TRAIN_PATH;
}

void getSource::setXMLTrainPath(string file_path)
{
	XML_TRAIN_PATH=file_path;
}

