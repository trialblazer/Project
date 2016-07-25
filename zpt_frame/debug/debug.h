#ifndef ZPT_DEBUG_H_
#define ZPT_DEBUG_H_

//开启下面的宏就会显示调试信息
#define DEBUG_FRAME_

//开启下面的宏就把调试信息输出到文件，注释输出到终端
//#define DEBUG_TO_FILE

#ifdef DEBUG_FRAME_
    #ifdef DEBUG_TO_FILE
	#include <fstream>
        using namespace std;
    #endif
    #include <stdio.h>
#endif

namespace zpt_frame {

#ifdef DEBUG_FRAME_
    #ifdef DEBUG_TO_FILE
	//调试信息输出到以下文件
	#define DEBUG_FILE "debug.txt" 
	//调试信息缓冲区大小
	#define DEBUG_BUFFER_MAX 4096
	//将调试信息输出到文件中, 格式：【模块名称: 函数名】调试信息 File：%s， Line： %d
	#define coutDebugMsg(moduleName, format, ...) {	    \
	    char buffer[DEBUG_BUFFER_MAX+1]	 = {0};	    \
	    snprintf(buffer, DEBUG_BUFFER_MAX, "[%s: %s] "format"  File:%s, Line:%d", moduleName, __FUNCTION__, ##__VA_ARGS__, __FILE__, __LINE__);						      \
	    ofstream out;				    \
	    out.open(DEBUG_FILE, ios::out | ios::app);	    \
	    if (!out) {					    \
		out.open(DEBUG_FILE);			    \
	    }						    \
	    out << buffer << endl;			    \
	    out.close();				    \
	}

	#define DebugMsg(x) {				    \
		ofstream out;				    \
		out.open(DEBUG_FILE, ios::out | ios::app);  \
		if (!out)				    \
		    out.open(DEBUG_FILE);		    \
		out << x << endl;			    \
		out.close();				    \
	    }
    #else
	//将调试信息输出到终端
	#define coutDebugMsg(moduleName, format, ...)	    \
	    printf("[%s: %s] "format"  File:%s,  Line:%d\n", moduleName, __FUNCTION__, ##__VA_ARGS__, __FILE__, __LINE__);
	
	#define DebugMsg(x)				    \
	    cout << x << endl;
    #endif //end for #ifdef DEBUG_TO_FILE
#else
    #define coutDebugMsg(moduleName, format, ...)
    #define DebugMsg(x) 
#endif


} //end of namespace zpt_frame


#endif
