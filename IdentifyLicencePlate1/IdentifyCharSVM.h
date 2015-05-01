#pragma once
class IdentifyCharSVM
{
public:
	IdentifyCharSVM(void);
	void test();
	void plateSVMTrain(string dir_path,string xml_path,string dir_type);
	int plateSVMPredict(IplImage *pImg_src,string xml_path,string dir_type);
	void setTrainFile(string dir_path,string xml_path,string dir_type);
	int getPredictPosition(IplImage *pImg_src,string xml_path,string dir_type);
	~IdentifyCharSVM(void);
};

