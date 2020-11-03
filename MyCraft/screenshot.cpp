#include "screenshot.h"


void Screenshot::invertY(int w, int h, vector<unsigned char>& buf)	// 需要转换Y轴
{
	std::vector<unsigned char> tmp_rgbdata(buf);	

	for (int i = h - 1; i >= 0; i--)				
	{
		memcpy(&buf[i * w * 3],						
			&tmp_rgbdata[(h - i - 1) * w * 3],		
			w * 3 * sizeof(unsigned char));			
	}
	tmp_rgbdata.clear();							
}

// 保存截图——调用SOIL库相关函数
void Screenshot::save_img(struct tm* ptminfo)
{
	// 存储当前图像的像素点
	// glPixelStore：对齐像素字节函数，四字节对齐
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	GLint viewPort[4] = { 0 };
	// 获取视口的范围
	glGetIntegerv(GL_VIEWPORT, viewPort);

	vector<unsigned char> buf(viewPort[2] * viewPort[3] * 3);
	// 读像素
	glReadPixels(viewPort[0], viewPort[1], viewPort[2], viewPort[3], GL_RGB, GL_UNSIGNED_BYTE, &buf[0]);
	// 反转Y坐标
	invertY(viewPort[2], viewPort[3], buf);

	// 获取当前的时间
	string year = to_string(ptminfo->tm_year + 1900);
	string month = to_string(ptminfo->tm_mon + 1);
	string day = to_string(ptminfo->tm_mday);
	string hour = to_string(ptminfo->tm_hour);
	string minute = to_string(ptminfo->tm_min);
	string second = to_string(ptminfo->tm_sec);
	string d = ".\\screenshot\\img" + year + "_" + month + "_" + day + "_" + hour + "_" + minute + "_" +  second + ".jpg";
	
	// 调用函数存储
	int err = SOIL_save_image(d.c_str(), SOIL_SAVE_TYPE_BMP, viewPort[2], viewPort[3], 3, &buf[0]);
	if (err == 0) {
		cout << "can't save the image" << endl;
	}
	else {
		cout << "image is save into " << d << endl;
	}
}