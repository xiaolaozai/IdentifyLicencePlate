#pragma once
class getSource
{
public:
	getSource(void);
	void setImgpath(char* img_path);
	static char* getImgpath();
	void setFilepath(string file_path);
	static string getFilepath();
	~getSource(void);
private:
	static char* IMG_PATH;
	static string FILE_PATH;
};


