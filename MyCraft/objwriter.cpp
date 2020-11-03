#include "objwriter.h"


myObjwriter::myObjwriter(string objfilename)
{
	file_name = objfilename;
}
//void loadMtl(string mtlfilename);

void myObjwriter::Output(struct tm* ptminfo)
{
	string year = to_string(ptminfo->tm_year + 1900);
	string month = to_string(ptminfo->tm_mon + 1);
	string day = to_string(ptminfo->tm_mday);
	string hour = to_string(ptminfo->tm_hour);
	string minute = to_string(ptminfo->tm_min);
	string second = to_string(ptminfo->tm_sec);
	string address = ".\\export\\Obj" + year + "_" + month + "_" + day + "_" + hour + "_" + minute + "_" + second + ".obj";


	ofstream fs(address);
	fs << "g default " << endl;
	int i = 0;
	for (i = 0; i < Vertices.size(); i++)
	{
		glm::vec3 temp;
		temp = Vertices[i];
		fs << "v ";
		fs << temp.x << " " << temp.y << " " << temp.z << endl;
	}

	for (i = 0; i < tVertices.size(); i++)
	{
		glm::vec3 temp;
		temp = tVertices[i];
		fs << "vt ";
		fs << temp.x << " " << temp.y << " " << temp.z << endl;
	}
	for (i = 0; i < Normals.size(); i++)
	{
		glm::vec3 temp;
		temp = Normals[i];
		fs << "vn ";
		fs << temp.x << " " << temp.y << " " << temp.z << endl;
	}
	fs << "s off" << endl;
	for (int i = 0; i < Faces.size(); i = i + 12)
	{
		unsigned char cube = i / 12;
		string group = "g pCube" + (cube - '0');
		fs << group << endl;
		for (int j = 0; j < 12; j++)
		{
			fs << "f ";
			ObjFace temp = Faces[i + j];
			for (int k = 0; k < 3; k++)
			{
				fs << temp.v[k] << "//" << temp.vn[k]<< " ";
			}
			fs << endl;
		}
	}
	

}

void myObjwriter::addCude(glm::vec3 new_vertice)
{
	glm::vec3 temp;
	temp = addvector(new_vertice, 0, 0, 1);
	Vertices.push_back(temp);
	temp = addvector(new_vertice, 1, 0, 1);
	Vertices.push_back(temp);
	temp = addvector(new_vertice, 0, 1, 1);
	Vertices.push_back(temp);
	temp = addvector(new_vertice, 1, 1, 1);
	Vertices.push_back(temp);
	temp = addvector(new_vertice, 0, 1, 0);
	Vertices.push_back(temp);
	temp = addvector(new_vertice, 1, 1, 0);
	Vertices.push_back(temp);
	temp = addvector(new_vertice, 0, 0, 0);
	Vertices.push_back(temp);
	temp = addvector(new_vertice, 1, 0, 0);
	Vertices.push_back(temp);

	for (int i = 0; i < 4; i++)
	{
		glm::vec3 temp(0, 0, 1);
		Normals.push_back(temp);
	}
	for (int i = 0; i < 4; i++)
	{
		glm::vec3 temp(0, 1, 0);
		Normals.push_back(temp);
	}
	for (int i = 0; i < 4; i++)
	{
		glm::vec3 temp(0, 0, -1);
		Normals.push_back(temp);
	}
	for (int i = 0; i < 4; i++)
	{
		glm::vec3 temp(0, -1, 0);
		Normals.push_back(temp);
	}
	for (int i = 0; i < 4; i++)
	{
		glm::vec3 temp(1, 0, 0);
		Normals.push_back(temp);
	}
	for (int i = 0; i < 4; i++)
	{
		glm::vec3 temp(-1, 0, 0);
		Normals.push_back(temp);
	}


	int group = Vertices.size() / 8 - 1;
	ObjFace facetemp;

	facetemp.change_value(group, 1, 1, 2, 2, 3, 3);
	Faces.push_back(facetemp);
	facetemp.change_value(group, 3, 3, 2, 2, 4, 4);
	Faces.push_back(facetemp);
	facetemp.change_value(group, 3, 5, 4, 6, 5, 7);
	Faces.push_back(facetemp);
	facetemp.change_value(group, 5, 7, 4, 6, 6, 8);
	Faces.push_back(facetemp);
	facetemp.change_value(group, 5, 9, 6, 10, 7, 11);
	Faces.push_back(facetemp);
	facetemp.change_value(group, 7, 11, 6, 10, 8, 12);
	Faces.push_back(facetemp);
	facetemp.change_value(group, 7, 13, 8, 14, 1, 15);
	Faces.push_back(facetemp);
	facetemp.change_value(group, 1, 15, 8, 14, 2, 16);
	Faces.push_back(facetemp);
	facetemp.change_value(group, 2, 17, 8, 18, 4, 19);
	Faces.push_back(facetemp);
	facetemp.change_value(group, 4, 19, 8, 18, 6, 20);
	Faces.push_back(facetemp);
	facetemp.change_value(group, 7, 21, 1, 22, 5, 23);
	Faces.push_back(facetemp);
	facetemp.change_value(group, 5, 23, 1, 22, 3, 24);
	Faces.push_back(facetemp);
}

glm::vec3 myObjwriter::addvector(glm::vec3 oldvec, int xx, int yy, int zz)
{
	glm::vec3 temp;
	temp.x = oldvec.x + xx;
	temp.y = oldvec.y + yy;
	temp.z = oldvec.z + zz;
	return temp;
}
