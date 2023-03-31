#ifndef __SerialInterface_H_
#define __SerialInterface_H_
#include <Windows.h>
#include <string>
using namespace std;

class SerialInterface
{
private:
    /* data */
    HANDLE hCom;
    string last_error;
public:
    SerialInterface();
    ~SerialInterface();
public:
    //同步方式打开串口,并配置默认信息
    bool openSyn(string serial_name, unsigned char baud_rate, unsigned char parity, unsigned char byte_size, unsigned char stop_bits);
    //同步方式打开串口(需自己配置串口参数)
    bool openSyn(string serial_name);

    //设置推荐的缓冲大小
    bool setBufferSize(DWORD inputBuff, DWORD outBuffer);
    //设置超时
    bool setTimeouts(COMMTIMEOUTS& timeouts);
    //设置串口信息
    bool setDCB(DCB& dcb);

    //刷新缓冲区
    bool purgeBuff(DWORD flags);
    //刷新缓冲区
    bool flushBuff();
    //写数据
    DWORD writeData(char* buffer, int length);
    //读数据
    DWORD readData(char* buffer, int length);
    //写字符串
    DWORD writeStr(string str);
    //关闭串口
    void closeComm();
    //判断串口是否打开
    bool isOpened();
    //得到最后一次失败的错误信息
    string getSerialLastError();
private:
    //设置最后一次的错误信息
    void setSerialLastError(string error_msg);
    //清chu最后一次的错误信息
    void clearSerialLastError();
};

#endif /*__SerialInterface_H_*/
#pragma once
