#ifndef __JSON_UTIL_H__
#define __JSON_UTIL_H__

#include <map>
#include <string>
using namespace std;

class IJSonReader;
class IJSonWriter;

class CJsonReader         //该类就是json对象，创建该类的对象，就是创建一个json对象，是json读对象
{                         //刚创建出来是一个空对象，因为还没进行初始化，调用parse(string)判断是否是json字符串，同时根据这个json字符串对该对象进行初始化
public:                   //这样，这个json对象就是传入的字符串的json对象了，即json对象有数据了，数据就是传入的json字符串
	CJsonReader();
	virtual ~CJsonReader();

	bool			parse(const string & json);   //判断字符串是否为json字符串，同时对该对象初始化，这样就把json字符串初始化成json对象了
	string			getParam(const string & key); //获取对象中的字段值 {name:zjs}
	int			    getParamInt(const string & key, int default_vale = 0); //获取对象中的整型字段值{code:200} 非整型返回的为0 
	string			getParam(const string & key,const string & attr);//获取对象套对象中的字段值 {data:{name:zjs}}
	string			getParam(const string &key, const string & attr, const int index);//获取对象套数组中的字段值  {data:[{name:zjs}]}   getParam(data,name,0)
	string          getParam(const string &key,const string &attr1,const string &attr2,const int index);//{data:{list:[{name:zjs}]}} getParam(data,list,name,0)
	string			getParam(const string &key, const string & attr1, const int index1, const string & attr2, const int index2);//{data:[{list:[{name:zjs}]}]} getParam(data,list,0,name,0)
	string			getParamCommon(const string &key, const string & attr, const int index);//同上
	string			getParamCommon(const string &key, const string & attr, const int index, const string &ckey, const int cindex);//同上
	void *          getParam2Node(const string &key, const string & attr, const int index);//获取节点  {data:[{name:zjs}]}  getParam2Node(data,name,0)-->name节点  name:zjs
	string          getParamNode(void * node, const string & attr, const int index);  //获取数组节点中的index下标的attr字段值  node数组父节点 attr字段  index下标
	IJSonReader *	getJsonReader() { return reader_; }       //获取reader_,reader_----->json对象
	void *          getRoot();                                //获取根节点
	void *			release();                                //清空json对象
	//获取节点  节点=key:value  节点值=value  节点名=key
	void *			getNode(void * parent, const char * key);     //获取子节点 参数1  LPJSON（void *）类型的父节点，第二个参数：char * 字符数组/字符串的首地址（注意：父节点是非数组，获取的是非数组元素）
	void *			getNode(void * parent, const char * key, int index);//同上，注意：父节点是数组，获取的是数组元素，index为下标
	const char *	getNodeValue(void * node);                       //获取节点的值
	const char*     getNodeName(void* node, int index);              //获取节点名

	/*注意：操作数组节点，并不是先拿数组节点，再从数组节点中拿某个下标的元素，不是这样的。事实上是，不单独拿数组节点，而是在拿数组节点的时候直接拿数组节点中的某个元素节点*/
	/*换句话就是说：不能直接操作数组节点，只能操作数组节点中的某个元素节点*/
	//const char * == string ==char数组名  =》字符串
private:
	void            clear();                                  //删除json，释放内存
	
private:
	IJSonReader *  reader_;                                  //（读）json对象
};

class CJSonWriter
{
public:
	CJSonWriter();              //初始化一个写json对象
	virtual ~CJSonWriter();

	void     add(const char * key, const char * value);             //添加一个key:value 传字符数组地址（字符数组）
	void     addList(const char * key, int index, const char * value);//添加数组元素
	void     addNode(const char* key, void* data);          //添加节点，添加子节点
	string   getJson();                               //json对象转json字符串
	void     setRoot(void* root);
	void     setJson(const char * json);              //json字符串转写json对象
	void *   getRoot();                              //获取根节点

private:
	void     clear();            //删除，释放内存

private:
	IJSonWriter *  writer_;     //写json对象
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