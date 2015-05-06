#pragma once
class IdentifyCharANN
{
public:
	IdentifyCharANN(void);
	void testRead();
	void setDataFile(string dir_path, string save_path,string dir_type);
	void setTrainFile(string dir_path,string save_path, string dir_type, int nHiddenLayer,int datatype);
	int getPredictPosition(IplImage *pImg_src,string xml_path,string dir_type,int datatype);
	~IdentifyCharANN(void);
};

