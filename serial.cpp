#include "Serial.h"

/*******************************************************************
* 名称： openSyn
* 功能： 同步方式打开串口,并配置默认信息
* 参数：
    serial_name：串口名称
    baud_rate  ：波特率，取值如下
        ......
        CBR_9600    9600bps
        CBR_14400   14400bps
        ......
    parity     ：校验方式
        EVENPARITY  偶校验
        MARKPARITY  标号校验
        NOPARITY    无校验
        ODDPARITY   奇校验
        SPACEPARITY 空格校验
    byte_size  ：数据位大小
        4，5，6，7，8
    stop_bits  ：停止位
        ONESTOPBIT      1个停止位
        ONE5STOPBITS    1.5个停止位
        TWOSTOPBITS     2个停止位
* 返回： 正确返回为ture，错误返回为false
*******************************************************************/
bool SerialInterface::openSyn(string serial_name, unsigned char baud_rate, unsigned char parity, unsigned char byte_size, unsigned char stop_bits)
{
    if (!openSyn(serial_name))
        return false;
    DCB dcb;

    if (false == GetCommState(hCom, &dcb))//获得当前串口的配置信息
    {
        setSerialLastError("SerialInterface::open() : GetCommState Error");
        return false;
    }

    dcb.DCBlength = sizeof(DCB);
    dcb.BaudRate = baud_rate;
    dcb.Parity = parity;
    dcb.ByteSize = byte_size;
    dcb.StopBits = stop_bits;

    if (false == SetCommState(hCom, &dcb))//用DCB结构重新配置串行端口信息
    {
        setSerialLastError("SerialInterface::open() : SetCommState Error");
        return false;
    }

    //超时处理
    COMMTIMEOUTS timeouts;
    timeouts.ReadIntervalTimeout = MAXDWORD; //读间隔超时
    // 把间隔超时设为最大，把总超时设为0将导致ReadFile立即返回并完成操作 

    timeouts.ReadTotalTimeoutMultiplier = 0; //读时间系数
    timeouts.ReadTotalTimeoutConstant = 0; //读时间常量
    timeouts.WriteTotalTimeoutMultiplier = 50; // 写时间系数
    timeouts.WriteTotalTimeoutConstant = 2000; //写时间常量
    //总的读/写超时时间 = Read(Write)TotalTimeoutMultiplier x 要读/写的字节数 + Read(Write)TotalTimeoutConstant. 
    if (false == SetCommTimeouts(hCom, &timeouts))
    {
        setSerialLastError("SerialInterface::open() : SetCommTimeouts Error");
        return false;
    }



    //清空缓冲区，为读写串口做准备
    if (false == PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_RXABORT))
    {
        setSerialLastError("SerialInterface::open() : PurgeComm Error");
        return false;
    }
    return true;
}
/*******************************************************************
* 名称： openSyn
* 功能： 同步方式打开串口(需自己配置串口参数)
* 参数：
    serial_name：串口名称
* 返回： 正确返回为ture，错误返回为false
*******************************************************************/
bool SerialInterface::openSyn(string serial_name)
{
    hCom = CreateFileA(
        serial_name.data(),//普通文件名或者设备文件名，这里是串口名
        GENERIC_READ | GENERIC_WRITE,//访问模式,读和写
        0,//共享模式，独占模式
        NULL,//指向安全属性的指针,不使用，传NULL
        OPEN_EXISTING,//如何创建,在串口中必须设置为OPEN_EXISTING。表示不能创建新端口只能打开已有的端口。
        FILE_ATTRIBUTE_NORMAL,//文件属性,使用默认属性FILE_ATTRIBUTE_NORMAL。
        NULL//用于复制文件句柄,通常这个参数设置为NULL，为空表示不使用模板
    );


    if (INVALID_HANDLE_VALUE == hCom)//出错判断
    {
        hCom = NULL;
        setSerialLastError("open():CreateFileA Error!");
        return false;
    }
    return true;
}
/*******************************************************************
* 名称： closeComm
* 功能： 关闭串口
* 参数： 无
* 返回： 正确返回为ture，错误返回为false
*******************************************************************/
void SerialInterface::closeComm()
{
    if (NULL == hCom)
        return;
    CloseHandle(hCom);
    hCom = NULL;
}
/*******************************************************************
* 名称： closeComm
* 功能： 判断串口是否打开
* 参数： 无
* 返回： 正确返回为ture，错误返回为false
*******************************************************************/
bool SerialInterface::isOpened()
{
    return NULL == hCom ? false : true;
}

DWORD SerialInterface::readData(char* buffer, int length)
{
    DWORD writeSize = 0;

    bool ret = false;

    ret = ReadFile(hCom, buffer, length, &writeSize, NULL);

    if (false == ret)
        return 0;

    return writeSize;
}


DWORD SerialInterface::writeData(char* buffer, int length)
{
    DWORD writeSize = 0;

    bool ret = false;

    ret = WriteFile(hCom, buffer, length, &writeSize, NULL);

    if (false == ret)
        return 0;

    return writeSize;

}


DWORD SerialInterface::writeStr(string str)
{
    bool ret = false;

    DWORD writeSize = 0;


    ret = WriteFile(hCom, str.data(), str.length(), &writeSize, NULL);

    if (0 == ret)
    {
        last_error = "Error By writeStr(string str)";
        return 0;
    }


    last_error = "";
    return writeSize;

}
/*******************************************************************
* 名称： setTimeouts
* 功能： 设置超时
* 参数：
    timeouts：超时配置的COMMTIMEOUTS结构体
* 返回： 正确返回为ture，错误返回为false
*******************************************************************/
bool SerialInterface::setTimeouts(COMMTIMEOUTS& timeouts)
{

    if (false == SetCommTimeouts(hCom, &timeouts))
    {
        setSerialLastError("SerialInterface::setTimeouts() : SetCommTimeouts Error");
        return false;
    }
    return true;
}
/*******************************************************************
* 名称： setDCB
* 功能： 设置串口信息
* 参数：
    dcb：串口信息配置的DCB结构体
* 返回： 正确返回为ture，错误返回为false
*******************************************************************/
bool SerialInterface::setDCB(DCB& dcb)
{
    if (false == SetCommState(hCom, &dcb))
    {
        setSerialLastError("SerialInterface::setDCB() : SetCommState Error");
        return false;
    }
    return true;
}
/*******************************************************************
* 名称： purgeBuff
* 功能： 刷新缓冲区
* 参数：
    flags：需要完成的操作，取值如下
        PURGE_RXABORT 终止所有未完成的重叠读取操作并立即返回，即使读取操作尚未完成。
        PURGE_RXCLEAR 清除输入缓冲区（如果设备驱动程序有一个）。
        PURGE_TXABORT 终止所有未完成的重叠写操作并立即返回，即使写操作尚未完成。
        PURGE_TXCLEAR 清除输出缓冲区（如果设备驱动程序有一个）。
* 返回： 正确返回为ture，错误返回为false
* 提示：PurgeComm()函数可以在读写操作的同时，清空缓冲区。当应用程序在读写操作时
调用PurgeComm()函数，不能保证缓冲区内的所有字符都被发送。
*******************************************************************/
bool SerialInterface::purgeBuff(DWORD flags)
{

    if (false == PurgeComm(hCom, flags))
    {
        setSerialLastError("SerialInterface::purgeBuff() : PurgeComm Error");
        return false;
    }
    return true;
}
/*******************************************************************
* 名称：flushBuff
* 功能：刷新缓冲区
* 参数：无
* 返回：正确返回为ture，错误返回为false
* 提示：该函数只受流量控制的支配，不受超时控制的支配，它在所有的写操作完成后才返回。
*******************************************************************/
bool SerialInterface::flushBuff()
{
    if (FlushFileBuffers(hCom))
    {
        setSerialLastError("SerialInterface::flushBuff() : FlushFileBuffers Error");
        return false;
    }
    return true;
}
/*******************************************************************
* 名称： setBufferSize
* 功能： 设置推荐的缓冲大小
* 参数：
    inputBuff：输入缓冲大小
    outBuffer：输出缓冲大小
* 返回： 正确返回为ture，错误返回为false
*******************************************************************/
bool SerialInterface::setBufferSize(DWORD inputBuff, DWORD outBuffer)
{
    if (inputBuff <= 0 || outBuffer <= 0)
        return false;

    return SetupComm(hCom, inputBuff, outBuffer);
}
/*******************************************************************
* 名称： getSerialLastError
* 功能： 得到最后一次失败的错误信息
* 参数： 无
* 返回： 数据类型:string,错误信息
*******************************************************************/
string SerialInterface::getSerialLastError()
{
    return last_error;
}

void SerialInterface::setSerialLastError(string error_msg)
{
    last_error = error_msg;
}
void SerialInterface::clearSerialLastError()
{
    last_error = "";
}
SerialInterface::SerialInterface()
{
    hCom = NULL;
}
SerialInterface::~SerialInterface()
{

}
