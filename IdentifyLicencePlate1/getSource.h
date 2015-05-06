#pragma once
class getSource
{
public:
	getSource(void);
	void setImgpath(char* img_path);
	static char* getImgpath();
	void setFilepath(string file_path);
	static string getFilepath();
	void setImageTrainPath(string file_path);
	static string getImageTrainPath();
	void setXMLTrainPath(string file_path);
	static string getXMLTrainPath();
	void setYMLTrainPath(string file_path);
	static string getYMLTrainPath();
	~getSource(void);
private:
	static char* IMG_PATH;
	static string FILE_PATH;
	static string IMG_TRAIN_PATH;
	static string XML_TRAIN_PATH;
	static string YML_TRAIN_PATH;
};


