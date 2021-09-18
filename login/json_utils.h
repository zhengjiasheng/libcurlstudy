#ifndef __JSON_UTIL_H__
#define __JSON_UTIL_H__

#include <map>
#include <string>
using namespace std;

class IJSonReader;
class IJSonWriter;

class CJsonReader         //�������json���󣬴�������Ķ��󣬾��Ǵ���һ��json������json������
{                         //�մ���������һ���ն�����Ϊ��û���г�ʼ��������parse(string)�ж��Ƿ���json�ַ�����ͬʱ�������json�ַ����Ըö�����г�ʼ��
public:                   //���������json������Ǵ�����ַ�����json�����ˣ���json�����������ˣ����ݾ��Ǵ����json�ַ���
	CJsonReader();
	virtual ~CJsonReader();

	bool			parse(const string & json);   //�ж��ַ����Ƿ�Ϊjson�ַ�����ͬʱ�Ըö����ʼ���������Ͱ�json�ַ�����ʼ����json������
	string			getParam(const string & key); //��ȡ�����е��ֶ�ֵ {name:zjs}
	int			    getParamInt(const string & key, int default_vale = 0); //��ȡ�����е������ֶ�ֵ{code:200} �����ͷ��ص�Ϊ0 
	string			getParam(const string & key,const string & attr);//��ȡ�����׶����е��ֶ�ֵ {data:{name:zjs}}
	string			getParam(const string &key, const string & attr, const int index);//��ȡ�����������е��ֶ�ֵ  {data:[{name:zjs}]}   getParam(data,name,0)
	string          getParam(const string &key,const string &attr1,const string &attr2,const int index);//{data:{list:[{name:zjs}]}} getParam(data,list,name,0)
	string			getParam(const string &key, const string & attr1, const int index1, const string & attr2, const int index2);//{data:[{list:[{name:zjs}]}]} getParam(data,list,0,name,0)
	string			getParamCommon(const string &key, const string & attr, const int index);//ͬ��
	string			getParamCommon(const string &key, const string & attr, const int index, const string &ckey, const int cindex);//ͬ��
	void *          getParam2Node(const string &key, const string & attr, const int index);//��ȡ�ڵ�  {data:[{name:zjs}]}  getParam2Node(data,name,0)-->name�ڵ�  name:zjs
	string          getParamNode(void * node, const string & attr, const int index);  //��ȡ����ڵ��е�index�±��attr�ֶ�ֵ  node���鸸�ڵ� attr�ֶ�  index�±�
	IJSonReader *	getJsonReader() { return reader_; }       //��ȡreader_,reader_----->json����
	void *          getRoot();                                //��ȡ���ڵ�
	void *			release();                                //���json����
	//��ȡ�ڵ�  �ڵ�=key:value  �ڵ�ֵ=value  �ڵ���=key
	void *			getNode(void * parent, const char * key);     //��ȡ�ӽڵ� ����1  LPJSON��void *�����͵ĸ��ڵ㣬�ڶ���������char * �ַ�����/�ַ������׵�ַ��ע�⣺���ڵ��Ƿ����飬��ȡ���Ƿ�����Ԫ�أ�
	void *			getNode(void * parent, const char * key, int index);//ͬ�ϣ�ע�⣺���ڵ������飬��ȡ��������Ԫ�أ�indexΪ�±�
	const char *	getNodeValue(void * node);                       //��ȡ�ڵ��ֵ
	const char*     getNodeName(void* node, int index);              //��ȡ�ڵ���

	/*ע�⣺��������ڵ㣬��������������ڵ㣬�ٴ�����ڵ�����ĳ���±��Ԫ�أ����������ġ���ʵ���ǣ�������������ڵ㣬������������ڵ��ʱ��ֱ��������ڵ��е�ĳ��Ԫ�ؽڵ�*/
	/*���仰����˵������ֱ�Ӳ�������ڵ㣬ֻ�ܲ�������ڵ��е�ĳ��Ԫ�ؽڵ�*/
	//const char * == string ==char������  =���ַ���
private:
	void            clear();                                  //ɾ��json���ͷ��ڴ�
	
private:
	IJSonReader *  reader_;                                  //������json����
};

class CJSonWriter
{
public:
	CJSonWriter();              //��ʼ��һ��дjson����
	virtual ~CJSonWriter();

	void     add(const char * key, const char * value);             //���һ��key:value ���ַ������ַ���ַ����飩
	void     addList(const char * key, int index, const char * value);//�������Ԫ��
	void     addNode(const char* key, void* data);          //��ӽڵ㣬����ӽڵ�
	string   getJson();                               //json����תjson�ַ���
	void     setRoot(void* root);
	void     setJson(const char * json);              //json�ַ���תдjson����
	void *   getRoot();                              //��ȡ���ڵ�

private:
	void     clear();            //ɾ�����ͷ��ڴ�

private:
	IJSonWriter *  writer_;     //дjson����
};

#endif


static void replace(string &target, const string & search, const string & replacement) 
{
	if (search.compare(replacement) == 0)
		return;

	else if (search.compare("") == 0)
		return;

	string::size_type i = string::npos;
	string::size_type lastPos = 0;
	while ((i = target.find(search, lastPos)) != string::npos) {
		target.replace(i, search.length(), replacement);
		lastPos = i + replacement.length();
	}
}

static void replace(wstring& target, const wstring& search, const wstring& replacement)
{
    if (search.compare(replacement) == 0)
        return;

    else if (search.compare(L"") == 0)
        return;

    string::size_type i = string::npos;
    string::size_type lastPos = 0;
    while ((i = target.find(search, lastPos)) != string::npos) {
        target.replace(i, search.length(), replacement);
        lastPos = i + replacement.length();
    }
}