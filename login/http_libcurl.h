#ifndef _HTTP_LIBCURL_
#define _HTTP_LIBCURL_

#include<Windows.h>
#include<curl/curl.h>
#include <string>  

#pragma comment(lib, "libcurl.lib")   
#pragma comment(lib, "wldap32.lib")   
#pragma comment(lib, "ws2_32.lib")   
#pragma comment(lib, "winmm.lib")

using namespace std;

/*
libcurl使用流程

1、curl_global_init()         //初始化libcurl要用到的一系列环境 
2、curl_easy_init()，         //初始化一个easy handle 返回CURL *curl，一个CURL类型的指针
3、curl_easy_setopt()         //给CURL *curl设置一些http传输的参数，比如设置url，设置传输完成接收到的数据怎么处理，设置http头部等
4、curl_easy_perform()        //字如其名，真正开始进行http通信了，返回状态码
返回状态码：CURLE_OK -- 任务完成一切都好 
CURLE_UNSUPPORTED_PROTOCOL -- 不支持的协议，由URL的头部指定
CURLE_COULDNT_CONNECT -- 不能连接到remote 主机或者代理
CURLE_REMOTE_ACCESS_DENIED -- 访问被拒绝
CURLE_HTTP_RETURNED_ERROR -- Http返回错误
CURLE_READ_ERROR -- 读本地文件错误
5、curl_easy_cleanup()       //清除初始化的CURL，释放所占用的内存
6、curl_global_cleanup()　　//CURL使用完毕，不在使用，则执行该函数，释放内存
*/

/*
CURLcode curl_global_init(long flags)
参数：
CURL_GLOBAL_ALL: 初始化全部可能要用到的功能，<推荐>
CURL_GLOABL_WIN32: 在windows平台使用
CURL_GLOBAL_SSL: 使用ssl库，支持https的话用这个（前提是，编译libcurl的时候添加了支持的ssl的选项）

1.这个函数只需要被执行一次。
2.当你没有显式调用这个函数的话，这个函数将会在你调用curl_easy_init()的时候被自动执行
3.一般情况下，没有调用这个函数而在curl_easy_init()中被隐式调用是没有问题的，但是当多线程的情况下，则可能会出现被多次调用的问题
4.所以，还是手动调用一次比较稳妥
*/

/*
CURLcode curl_easy_setopt(CURL *handle, opt, param)
参数：
handle: curl_easy_init()函数初始化返回的CURL *
opt和param相对应:

CURLOPT_URL: 对应请求的url地址
CURLOPT_VERBOSE: 对应1，表示要求输出调试信息(是libcurl内部另开一个线程输出的)
CURLOPT_DEBUGFUNCTION: 表示调试信息要输出的级别
CURLOPT_WRITEFUNCTION: 设置回调函数，这个回调函数将在内部socket一旦接收到数据时被调用，detail1:回调函数格式：size_t req_reply(void *ptr, size_t size, size_t nmemb, void *stream)
CURLOPT_HTTPHEADER: 设置http请求的头部 detail2:举例说明
CURLOPT_CUSTOMREQUEST: 自定义请求方式，默认不post任何数据时，请求方式为GET
CURLOPT_COOKIE: 设置要发送时携带的COOKIE字段，但是似乎直接设置头部时写上cookie字段，也是没有问题的，两种方法都行
CURLOPT_COOKIEFILE: 动态的设置cookie  detail3:举例说明
CURLOPT_FOLLOWLOCATION:设置跟随重定向；当libcurl被设置为1时，如果libcurl接收到了一个response是重定向的，此时，libcurl将会自动跟随到这个重定向指定的地址
CURLOPT_POSTFIELDS: POST数据，（对应POST请求）  detail4:举例说明
CURLOPT_HTTP_VERSION: 设置libcurl所使用的http版本；默认是使用http1.1的版本，可以通过设置成1.0的版本
curl_easy_setopt(easyhandle, CURLOPT_PROXY, "proxy-host.com:8080");  //代理设置；当被设置为空字符串时，将会禁用代理
curl_easy_setopt(easyhandle, CURLOPT_PROXYUSERPWD, "user:password"); //代理需要认证时
超时设置：
CURLOPT_TIMEOUT: 设置获取内容超时秒数（tcp连接已建立，但是获取http返回内容超时）
CURLOPT_TIMEOUT_MS: 同上，只是这个设置的单位是毫秒
CURLOPT_CONNECTTIMEOUT: 设置tcp连接建立的超时秒数（建立tcp连接时超时）
CURLOPT_CONNECTTIMEOUT_MS: 同上，只是这个设置的单位是毫秒
*/

/*detail1:
size_t req_reply(void *ptr, size_t size, size_t nmemb, void *stream)
(**注意并不是接收到全部数据才被调用，而是只要接收到数据就调用，数据大时将会被多次调用**)
ptr：返回的数据首地址
size：返回的每一个数据项的字节数
nmemb：数据项的个数
stream：用户可以自由使用的指针,用来在函数体内部保存数据，一般是用来传递一个文件指针。
由curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response)   stream 和 response 是一致的
不过response是结果，stream用于实现过程，注意回调函数调用的次数可能不止一次，故返回的数据是通过多次回调函数返回过来的，所以stream是用来实现保存返回的数据这个过程的。
*/

/*detail2:
例子：curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);
		headers: 一个curl_slist类型的指针，设置方法如下：
		struct curl_slist *headers = NULL;//必须被赋予NULL
	    headers = curl_slist_append(headers, "Hey-server-hey: how are you?");
	    headers = curl_slist_append(headers, "X-silly-content: yes");
	    当设置的请求头字段与内部预定义的字段相重合时，内部预定义的内容将被替换为用户设置的，如果想要删除某个内部预定义的头字段时
	    headers = curl_slist_append(headers, "Host:");
	    采用这种形式，冒号后不添加任何字符
*/

/*detail3:
		FILE *file = fopen("/dev/null", "r");
	    curl_easy_setopt(handle, CURLOPT_COOKIEFILE, file);
		当一个easy handle被设置了此选项时，手动的添加cookie将会被忽略。
		当easy handle访问了一次页面后，如果response的头部有set-cookie字段时，这个cookie将会被保存在对应的文件中，当再次调用这个easy handle时，easy handle将会从这个文件中查找对应域名的cookie，如果找到了，将会自动添加找到的cookie到request头部。
		如果这个文件指针指向一个不存在的文件，那么将会保存获得cookie到内存中，通常这是一种常用的做法
*/

/*detail4:
char *data="name=daniel&project=curl";
		curl_easy_setopt(easyhandle, CURLOPT_POSTFIELDS, data);
		一种超级简单的方式，就post了数据，但是这只能post字符数据
		post二进制数据的例子：
		struct curl_slist *headers=NULL;
headers = curl_slist_append(headers, "Content-Type: text/xml");
		curl_easy_setopt(easyhandle, CURLOPT_POSTFIELDS, binaryptr);
		curl_easy_setopt(easyhandle, CURLOPT_POSTFIELDSIZE, 23L);
		curl_easy_setopt(easyhandle, CURLOPT_HTTPHEADER, headers);
		当CURLOPT_POSTFIELDS选项被设置后，libcurl自动将请求方式变更为POST
		这似乎是件好事，但是由于easy handle上面设置的属性的是不会随着perform之后而自动复位的，所以当下一次要进行GET请求时，需要手动更改请求方式为GET
		 curl_easy_setopt(easyhandle, CURLOPT_HTTPGET, 1L)；
		 或者
		 curl_easy_setopt(easyhandle, CURLOPT_CUSTOMREQUEST, "GET");
*/

// reply of the requery 
static size_t req_reply(void *ptr, size_t size, size_t nmemb, void *stream);  
// http GET 
static CURLcode curl_get_req(const std::string &url, std::string &response); 
// http POST  
static CURLcode curl_post_req(const string &url, const string &postParams, string &response);
//UTF8转GBK/GB2312
static string UTF8ToGBK(const char* strUTF8);
//GBK/GB2312转UTF8
static string GBKToUTF8(const char* strGBK);


// reply of the requery  
static size_t req_reply(void *ptr, size_t size, size_t nmemb, void *stream)  
{  
    cout << "----->reply" << endl;  
    string *str = (string*)stream;  
    cout << *str << endl;  
    (*str).append((char*)ptr, size*nmemb);  
    return size * nmemb;  
}  

/*
函数参数中的&表示引用，引用实参，即代表实参的一个别名，操作引用变量就相当于操作实参变量
&在变量操作区，表示取地址符。
&在变量定义区，表示引用。引用常用于函数参数中，函数参数中使用引用相当于把实参引入进来了，这样就相当于操作实参了。
*/
// http GET  
static CURLcode curl_get_req(const std::string &url, std::string &response)  
{  
    // init curl  
    CURL *curl = curl_easy_init();                  //初始化CURL 返回CURL *
    // res code  
    CURLcode res;  
	struct curl_slist *head = NULL;
    if (curl)  
    {  
		//设置请求头，Content-Type，格式：application/json,UTF-8
		curl_slist_append(head,"Content-Type:application/x-www-form-urlencoded;charset=UTF-8");
		curl_easy_setopt(curl,CURLOPT_HTTPHEADER,head);

        // set params  
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url  设置请求url
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https 设置是否使用https  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false 检查服务器SSL证书中是否存在一个公用名(common name)   0/1/2
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);            //是否输出调试信息，libcurl内部另开一个线程输出
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);    //回调函数
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);   //回调函数 req_reply 接收到数据就触发，可能会触发多次。实现保存数据的过程，及其他操作
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);  //用于保存请求返回的数据，实现结果，传要保存到的位置的指针，该参数和req_reply的第四个参数一致，不想用回调函数实现保存数据，也可以直接使用它，不过限制参数类型必须是FILE指针，这样返回的数据会自动写到该文件指针指向的文件流中。
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);           //启用时忽略所有的curl传递给php进行的信号。在SAPI多线程传输时此项被默认启用。 
        curl_easy_setopt(curl, CURLOPT_HEADER, 0);            //参数设置为 1，那么会通知 curl 库在输出时要同时包含 "头部“ 和 "主题内容" 两个部分。该选项仅是对那些同时包含了”头部“和”主题内容“这两部分的协议而言(如 HTTP)。
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3); // set transport and time out time  设置tcp连接建立的超时秒数，设置连接超时数，设置为0，则无限等待。3秒
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);        //设置获取内容超时秒数（tcp连接已建立，但是获取http返回内容超时）3秒
        // start req  
        res = curl_easy_perform(curl);           //执行CURL，设置好各项内容的CURL，执行它，返回状态码  CURLE_OK：成功
    } 
    // release curl  
    curl_easy_cleanup(curl);                    //执行完毕，清除CURL，释放内存 
	curl_slist_free_all(head);
    return res;  
}  
  
// http POST  
static CURLcode curl_post_req(const string &url, const string &postParams, string &response)  
{  
    // init curl  
    CURL *curl = curl_easy_init();  
    // res code  
    CURLcode res;  
    if (curl)  
    {  
        // set params  
        curl_easy_setopt(curl, CURLOPT_POST, 1); // post req  启用时会发送一个常规的POST请求，类型为：application/x-www-form-urlencoded，就像表单提交的一样。
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url  设置请求url
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postParams.c_str()); // params  post请求要提交的数据，这段数据要根据服务器的具体要求填写
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https  同上
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false  同上
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);            //同上
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);    //同上
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);  //同上
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response); //同上  
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);         //同上
        curl_easy_setopt(curl, CURLOPT_HEADER, 0);           //同上
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);   //同上
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);          //同上
        // start req  
        res = curl_easy_perform(curl);  
    }  
    // release curl  
    curl_easy_cleanup(curl);  
    return res;  
}  

/*
编码转换
*/
//UTF8转GBK/GB2312
static string UTF8ToGBK(const char* strUTF8)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);
    wchar_t* wszGBK = new wchar_t[len+1];
    memset(wszGBK, 0, len*2+2);
    MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, wszGBK, len);
    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
    char* szGBK = new char[len+1];
    memset(szGBK, 0, len+1);
    WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
    string strTemp(szGBK);
    if(wszGBK) delete[] wszGBK;
    if(szGBK) delete[] szGBK;
    return strTemp;
}

//GBK/GB2312转UTF8
static string GBKToUTF8(const char* strGBK)
{
    int len = MultiByteToWideChar(CP_ACP, 0, strGBK, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len+1];
    memset(wstr, 0, len+1);
    MultiByteToWideChar(CP_ACP, 0, strGBK, -1, wstr, len);
    len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len+1];
    memset(str, 0, len+1);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
    string strTemp = str;
    if(wstr) delete[] wstr;
    if(str) delete[] str;
    return strTemp;
}
#endif