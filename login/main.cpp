#include <iostream>
#include <vector>
#include "http_libcurl.h"
#include "json_utils.h"
#include <fstream>

using namespace std;

string getKeyValue(std::string sKey, std::string sValue){    
	char szDoubleQutoes[] = "\"";    
	char szColon[] = ":";   
	string strResult;

    strResult.append(szDoubleQutoes);
    strResult.append(sKey);
    strResult.append(szDoubleQutoes);

    strResult.append(szColon);
    strResult.append(szDoubleQutoes);
    strResult.append(sValue);
    strResult.append(szDoubleQutoes);   
	return strResult;
}

string getKeyValue(std::string sKey, int iValue){   
	char szDoubleQutoes[] = "\"";  
	char szColon[] = ":";    
	char szValue[50] = { 0 };   
	string strResult;

    strResult.append(szDoubleQutoes);
    strResult.append(sKey);
    strResult.append(szDoubleQutoes);

    strResult.append(szColon);    
	sprintf(szValue, "%d", iValue);
    strResult.append(szValue);    
	return strResult;
}

std::vector<std::string> split(std::string str,std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str+=pattern;//扩展字符串以方便操作
	int size=str.size();

	for(int i=0; i<size; i++)
	{
		pos=str.find(pattern,i);
		if(pos<size)
		{
			std::string s=str.substr(i,pos-i);
			result.push_back(s);
			i=pos+pattern.size()-1;
		}
	}
	return result;
}

int main()
{
	//初始化global
	curl_global_init(CURL_GLOBAL_ALL); 

	//string get_url = "http://localhost:8088/blogger/login?user_name=admin&password=111111";
	string get_url = "http://localhost:8088/comment/findAllCommentByPage?pageNum=5&pageSize=1";
	string get_res;
	string res_data;
	
	auto res = curl_get_req(get_url,get_res);
	if (res != CURLE_OK)  
        cerr << "curl_easy_perform() failed: " + string(curl_easy_strerror(res)) << endl;  
	else  {
		res_data = UTF8ToGBK(get_res.c_str());
		cout << "数据===》" << res_data << endl;  
	}
       
	//释放global
	curl_global_cleanup();
	
	//string res_data = "{\"name\":\"smith\",\"age\":28,\"sex\":\"男\",\"school\":{\"sname\":\"南京大学\",\"address\":\"南京市鼓楼区汉口路22号\"},\"children\":\"[{\"book\":\"语文\"},{\"book\":\"数学\"}]\"}";

	//string res_data = "{\"data\":[{\"child\":[{\"name\":\"zjs\"}]]}";

	//读json对象
	CJsonReader reader;
	if(reader.parse(res_data)){
		cout << "start" << endl;
		string data = reader.getParam("data","list","user_ip",0);
		cout << data << endl;
		cout << "end" << endl;
	}

	//写json对象
	cout << "----------------------" << endl;
	CJSonWriter writer;
	CJSonWriter child;
	child.addList("list",0,"100");
	writer.add("name","zjs");
	writer.addNode("children",child.getRoot());

	/*writer.addList("list/key",0,"111");
	writer.addList("list/value",0,"222");*/
	//writer.setJson(res_data.c_str());
	//writer.add("code","400");
	string person = writer.getJson();
	cout << person << endl;

	//vector转string测试
	vector<vector<POINT>> lList;
	POINT point;
	point.x = 16;
	point.y = 32;
	for (int i = 0; i < 2; i++)
	{
		vector<POINT> list;
		for (int j = 0; j < 5; j++)
		{
			list.push_back(point);
		}
		lList.push_back(list);
	}
	string strLineList = "";
	strLineList += "{\"lines\":[";
	for (int i = 0; i < lList.size(); i++)
	{
		strLineList += "{\"line\":\"";
		for (int j = 0; j < lList[i].size(); j++)
		{
			char szValue[50] = { 0 };  
			strLineList += "";
			sprintf(szValue, "%d", lList[i][j].x);
			strLineList.append(szValue);
			strLineList += ",";
			memset(szValue,0,sizeof(szValue));
			sprintf(szValue, "%d", lList[i][j].y);
			strLineList.append(szValue);
			if (j != lList[i].size() -1)
				strLineList += "|";
		}
		if (i == lList.size()-1)
		{
			strLineList += "\"}";
		}else
		{
			strLineList += "\"},";
		}
	}
	strLineList += "]}";
	cout << strLineList << endl;

	//string转vector测试
	CJsonReader readList;
	vector<vector<POINT>> vList;
	if (readList.parse(strLineList))
	{
		int i = 0;
		while (readList.getParam("lines","line",i) != "")
		{
			vector<POINT> vLine;
			string line_str = readList.getParam("lines","line",i);
			cout << line_str << endl;
			vector<string> line_vec = split(line_str,"|");
			for (int j = 0; j < line_vec.size(); j++)
			{
				POINT pint;
				vector<string> point_vec = split(line_vec[j],",");
				pint.x = stoi(point_vec[0].c_str());
				pint.y = stoi(point_vec[1].c_str());
				vLine.push_back(pint);
			}
			vList.push_back(vLine);
			i++;
		}	
	}

	ofstream ofs;
	ofs.open("data/aaa.txt",ios::out | ios::binary);
	ofs << strLineList << endl;
	ofs.close();

	ifstream ifs;
	ifs.open("data/aaa.txt",ios::in | ios::binary);
	if(!ifs.is_open()){
		cout << "文件打开失败！" << endl;
		system("pause");
		return 0;
	}
	string idata;
	ifs >> idata;
	cout << idata << endl;
	ifs.close();

	system("pause");
	return 0;
}
