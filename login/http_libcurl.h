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
libcurlʹ������

1��curl_global_init()         //��ʼ��libcurlҪ�õ���һϵ�л��� 
2��curl_easy_init()��         //��ʼ��һ��easy handle ����CURL *curl��һ��CURL���͵�ָ��
3��curl_easy_setopt()         //��CURL *curl����һЩhttp����Ĳ�������������url�����ô�����ɽ��յ���������ô��������httpͷ����
4��curl_easy_perform()        //����������������ʼ����httpͨ���ˣ�����״̬��
����״̬�룺CURLE_OK -- �������һ�ж��� 
CURLE_UNSUPPORTED_PROTOCOL -- ��֧�ֵ�Э�飬��URL��ͷ��ָ��
CURLE_COULDNT_CONNECT -- �������ӵ�remote �������ߴ���
CURLE_REMOTE_ACCESS_DENIED -- ���ʱ��ܾ�
CURLE_HTTP_RETURNED_ERROR -- Http���ش���
CURLE_READ_ERROR -- �������ļ�����
5��curl_easy_cleanup()       //�����ʼ����CURL���ͷ���ռ�õ��ڴ�
6��curl_global_cleanup()����//CURLʹ����ϣ�����ʹ�ã���ִ�иú������ͷ��ڴ�
*/

/*
CURLcode curl_global_init(long flags)
������
CURL_GLOBAL_ALL: ��ʼ��ȫ������Ҫ�õ��Ĺ��ܣ�<�Ƽ�>
CURL_GLOABL_WIN32: ��windowsƽ̨ʹ��
CURL_GLOBAL_SSL: ʹ��ssl�⣬֧��https�Ļ��������ǰ���ǣ�����libcurl��ʱ�������֧�ֵ�ssl��ѡ�

1.�������ֻ��Ҫ��ִ��һ�Ρ�
2.����û����ʽ������������Ļ���������������������curl_easy_init()��ʱ���Զ�ִ��
3.һ������£�û�е��������������curl_easy_init()�б���ʽ������û������ģ����ǵ����̵߳�����£�����ܻ���ֱ���ε��õ�����
4.���ԣ������ֶ�����һ�αȽ�����
*/

/*
CURLcode curl_easy_setopt(CURL *handle, opt, param)
������
handle: curl_easy_init()������ʼ�����ص�CURL *
opt��param���Ӧ:

CURLOPT_URL: ��Ӧ�����url��ַ
CURLOPT_VERBOSE: ��Ӧ1����ʾҪ�����������Ϣ(��libcurl�ڲ���һ���߳������)
CURLOPT_DEBUGFUNCTION: ��ʾ������ϢҪ����ļ���
CURLOPT_WRITEFUNCTION: ���ûص�����������ص����������ڲ�socketһ�����յ�����ʱ�����ã�detail1:�ص�������ʽ��size_t req_reply(void *ptr, size_t size, size_t nmemb, void *stream)
CURLOPT_HTTPHEADER: ����http�����ͷ�� detail2:����˵��
CURLOPT_CUSTOMREQUEST: �Զ�������ʽ��Ĭ�ϲ�post�κ�����ʱ������ʽΪGET
CURLOPT_COOKIE: ����Ҫ����ʱЯ����COOKIE�ֶΣ������ƺ�ֱ������ͷ��ʱд��cookie�ֶΣ�Ҳ��û������ģ����ַ�������
CURLOPT_COOKIEFILE: ��̬������cookie  detail3:����˵��
CURLOPT_FOLLOWLOCATION:���ø����ض��򣻵�libcurl������Ϊ1ʱ�����libcurl���յ���һ��response���ض���ģ���ʱ��libcurl�����Զ����浽����ض���ָ���ĵ�ַ
CURLOPT_POSTFIELDS: POST���ݣ�����ӦPOST����  detail4:����˵��
CURLOPT_HTTP_VERSION: ����libcurl��ʹ�õ�http�汾��Ĭ����ʹ��http1.1�İ汾������ͨ�����ó�1.0�İ汾
curl_easy_setopt(easyhandle, CURLOPT_PROXY, "proxy-host.com:8080");  //�������ã���������Ϊ���ַ���ʱ��������ô���
curl_easy_setopt(easyhandle, CURLOPT_PROXYUSERPWD, "user:password"); //������Ҫ��֤ʱ
��ʱ���ã�
CURLOPT_TIMEOUT: ���û�ȡ���ݳ�ʱ������tcp�����ѽ��������ǻ�ȡhttp�������ݳ�ʱ��
CURLOPT_TIMEOUT_MS: ͬ�ϣ�ֻ��������õĵ�λ�Ǻ���
CURLOPT_CONNECTTIMEOUT: ����tcp���ӽ����ĳ�ʱ����������tcp����ʱ��ʱ��
CURLOPT_CONNECTTIMEOUT_MS: ͬ�ϣ�ֻ��������õĵ�λ�Ǻ���
*/

/*detail1:
size_t req_reply(void *ptr, size_t size, size_t nmemb, void *stream)
(**ע�Ⲣ���ǽ��յ�ȫ�����ݲű����ã�����ֻҪ���յ����ݾ͵��ã����ݴ�ʱ���ᱻ��ε���**)
ptr�����ص������׵�ַ
size�����ص�ÿһ����������ֽ���
nmemb��������ĸ���
stream���û���������ʹ�õ�ָ��,�����ں������ڲ��������ݣ�һ������������һ���ļ�ָ�롣
��curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response)   stream �� response ��һ�µ�
����response�ǽ����stream����ʵ�ֹ��̣�ע��ص��������õĴ������ܲ�ֹһ�Σ��ʷ��ص�������ͨ����λص��������ع����ģ�����stream������ʵ�ֱ��淵�ص�����������̵ġ�
*/

/*detail2:
���ӣ�curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);
		headers: һ��curl_slist���͵�ָ�룬���÷������£�
		struct curl_slist *headers = NULL;//���뱻����NULL
	    headers = curl_slist_append(headers, "Hey-server-hey: how are you?");
	    headers = curl_slist_append(headers, "X-silly-content: yes");
	    �����õ�����ͷ�ֶ����ڲ�Ԥ������ֶ����غ�ʱ���ڲ�Ԥ��������ݽ����滻Ϊ�û����õģ������Ҫɾ��ĳ���ڲ�Ԥ�����ͷ�ֶ�ʱ
	    headers = curl_slist_append(headers, "Host:");
	    ����������ʽ��ð�ź�����κ��ַ�
*/

/*detail3:
		FILE *file = fopen("/dev/null", "r");
	    curl_easy_setopt(handle, CURLOPT_COOKIEFILE, file);
		��һ��easy handle�������˴�ѡ��ʱ���ֶ������cookie���ᱻ���ԡ�
		��easy handle������һ��ҳ������response��ͷ����set-cookie�ֶ�ʱ�����cookie���ᱻ�����ڶ�Ӧ���ļ��У����ٴε������easy handleʱ��easy handle���������ļ��в��Ҷ�Ӧ������cookie������ҵ��ˣ������Զ�����ҵ���cookie��requestͷ����
		�������ļ�ָ��ָ��һ�������ڵ��ļ�����ô���ᱣ����cookie���ڴ��У�ͨ������һ�ֳ��õ�����
*/

/*detail4:
char *data="name=daniel&project=curl";
		curl_easy_setopt(easyhandle, CURLOPT_POSTFIELDS, data);
		һ�ֳ����򵥵ķ�ʽ����post�����ݣ�������ֻ��post�ַ�����
		post���������ݵ����ӣ�
		struct curl_slist *headers=NULL;
headers = curl_slist_append(headers, "Content-Type: text/xml");
		curl_easy_setopt(easyhandle, CURLOPT_POSTFIELDS, binaryptr);
		curl_easy_setopt(easyhandle, CURLOPT_POSTFIELDSIZE, 23L);
		curl_easy_setopt(easyhandle, CURLOPT_HTTPHEADER, headers);
		��CURLOPT_POSTFIELDSѡ����ú�libcurl�Զ�������ʽ���ΪPOST
		���ƺ��Ǽ����£���������easy handle�������õ����Ե��ǲ�������perform֮����Զ���λ�ģ����Ե���һ��Ҫ����GET����ʱ����Ҫ�ֶ���������ʽΪGET
		 curl_easy_setopt(easyhandle, CURLOPT_HTTPGET, 1L)��
		 ����
		 curl_easy_setopt(easyhandle, CURLOPT_CUSTOMREQUEST, "GET");
*/

// reply of the requery 
static size_t req_reply(void *ptr, size_t size, size_t nmemb, void *stream);  
// http GET 
static CURLcode curl_get_req(const std::string &url, std::string &response); 
// http POST  
static CURLcode curl_post_req(const string &url, const string &postParams, string &response);
//UTF8תGBK/GB2312
static string UTF8ToGBK(const char* strUTF8);
//GBK/GB2312תUTF8
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
���������е�&��ʾ���ã�����ʵ�Σ�������ʵ�ε�һ���������������ñ������൱�ڲ���ʵ�α���
&�ڱ�������������ʾȡ��ַ����
&�ڱ�������������ʾ���á����ó����ں��������У�����������ʹ�������൱�ڰ�ʵ����������ˣ��������൱�ڲ���ʵ���ˡ�
*/
// http GET  
static CURLcode curl_get_req(const std::string &url, std::string &response)  
{  
    // init curl  
    CURL *curl = curl_easy_init();                  //��ʼ��CURL ����CURL *
    // res code  
    CURLcode res;  
	struct curl_slist *head = NULL;
    if (curl)  
    {  
		//��������ͷ��Content-Type����ʽ��application/json,UTF-8
		curl_slist_append(head,"Content-Type:application/x-www-form-urlencoded;charset=UTF-8");
		curl_easy_setopt(curl,CURLOPT_HTTPHEADER,head);

        // set params  
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url  ��������url
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https �����Ƿ�ʹ��https  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false ��������SSL֤�����Ƿ����һ��������(common name)   0/1/2
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);            //�Ƿ����������Ϣ��libcurl�ڲ���һ���߳����
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);    //�ص�����
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);   //�ص����� req_reply ���յ����ݾʹ��������ܻᴥ����Ρ�ʵ�ֱ������ݵĹ��̣�����������
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);  //���ڱ������󷵻ص����ݣ�ʵ�ֽ������Ҫ���浽��λ�õ�ָ�룬�ò�����req_reply�ĵ��ĸ�����һ�£������ûص�����ʵ�ֱ������ݣ�Ҳ����ֱ��ʹ�������������Ʋ������ͱ�����FILEָ�룬�������ص����ݻ��Զ�д�����ļ�ָ��ָ����ļ����С�
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);           //����ʱ�������е�curl���ݸ�php���е��źš���SAPI���̴߳���ʱ���Ĭ�����á� 
        curl_easy_setopt(curl, CURLOPT_HEADER, 0);            //��������Ϊ 1����ô��֪ͨ curl �������ʱҪͬʱ���� "ͷ���� �� "��������" �������֡���ѡ����Ƕ���Щͬʱ�����ˡ�ͷ�����͡��������ݡ��������ֵ�Э�����(�� HTTP)��
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3); // set transport and time out time  ����tcp���ӽ����ĳ�ʱ�������������ӳ�ʱ��������Ϊ0�������޵ȴ���3��
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);        //���û�ȡ���ݳ�ʱ������tcp�����ѽ��������ǻ�ȡhttp�������ݳ�ʱ��3��
        // start req  
        res = curl_easy_perform(curl);           //ִ��CURL�����úø������ݵ�CURL��ִ����������״̬��  CURLE_OK���ɹ�
    } 
    // release curl  
    curl_easy_cleanup(curl);                    //ִ����ϣ����CURL���ͷ��ڴ� 
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
        curl_easy_setopt(curl, CURLOPT_POST, 1); // post req  ����ʱ�ᷢ��һ�������POST��������Ϊ��application/x-www-form-urlencoded��������ύ��һ����
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url  ��������url
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postParams.c_str()); // params  post����Ҫ�ύ�����ݣ��������Ҫ���ݷ������ľ���Ҫ����д
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https  ͬ��
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false  ͬ��
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);            //ͬ��
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);    //ͬ��
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);  //ͬ��
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response); //ͬ��  
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);         //ͬ��
        curl_easy_setopt(curl, CURLOPT_HEADER, 0);           //ͬ��
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);   //ͬ��
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);          //ͬ��
        // start req  
        res = curl_easy_perform(curl);  
    }  
    // release curl  
    curl_easy_cleanup(curl);  
    return res;  
}  

/*
����ת��
*/
//UTF8תGBK/GB2312
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

//GBK/GB2312תUTF8
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