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
	void setImagePlatePath(string file_path);
	static string getImagePlatePath();
	void setXMLTrainPath(string file_path);
	static string getXMLTrainPath();
	~getSource(void);
private:
	static char* IMG_PATH;
	static string FILE_PATH;
	static string IMG_TRAIN_PATH;
	static string IMG_PLATE_PATH;
	static string XML_TRAIN_PATH;
};


