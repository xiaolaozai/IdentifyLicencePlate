#pragma once
class IdentifyCharANN
{
public:
	IdentifyCharANN(void);
	void testRead();
	void setTrainFile(string dir_path, string dir_type, string xml_path);
	CvANN_MLP getTrain(CvANN_MLP mlp,Mat mat_trainData, Mat mat_classes, int nClass, int nHiddenLayer);
	int getPredictPosition(CvANN_MLP mlp,Mat mat_model,int nClass);
	~IdentifyCharANN(void);
};

