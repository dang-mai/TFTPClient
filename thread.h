#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QTimer>
#include <QTextCodec>
#include <QDebug>
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <unordered_map>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <sys/stat.h>
#include <io.h>

#include "magic_enum.hpp"

using namespace std;

using namespace magic_enum;

// 客户端平台定义
enum PLATFORM {WINDOWS = 1, LINUX = 2};
// 文件分隔符定义
enum DELIMITER {CR = '\r', LF = '\n', END = '\0'};
// 字段大小
enum FIELDSIZE {OPC_SIZE = 2, MODE_SIZE = 2, BLOCK_SIZE = 2, ERRCODE_SIZE = 2};
// 最大值定义
enum MAXSIZE {MAX_RETRANSMIT_SIZE = 10, MAX_REQUEST_SIZE = 512, MAX_BLOCK_SIZE = 16384, MAX_ERROR_SIZE = 512};
// 数据包操作码定义
enum OPC {OPC_REQUEST_DOWN = 1, OPC_REQUEST_UP = 2, OPC_DATA = 3, OPC_ACK = 4, OPC_ERROR = 5, OPC_OACK = 6};
// 选项默认值定义
enum DEFAULT_OPTS {DEFAULT_TIMEOUT = 2, DEFAULT_BLOCK_SIZE = 512};
// 数据包处理结果定义
enum PACKET {PKT_SOCKET_ERROR, PKT_TIMEOUT, PKT_CON_CLOSE, PKT_SUCCESS, PKT_FAIL};
// 传输模式定义
enum MODE {NETASCII = 1, OCTET = 2};
// 参数检查结果定义
enum CHECK {INVALID_ARGS = -1, VALID_ARGS};
// 显示信息类型定义
enum class TYPE {INIT_FAIL, RETRANSMIT_DATA, DATA_FAIL, RETRANSMIT_ACK, WRITE_FAIL, RETRANSMIT_TOO_MUCH, EXTENDED_OPTIONS_YES, EXTENDED_OPTIONS_NO, WRONG_PKT, ERROR_PKT, RETRANSMIT_REQUEST, ACK_FAIL, EXTENDED_OPTIONS, TIMEOUT, CONNECTION_ERROR, CONNECTION_CLOSE, FAIL, SUCCESS, REQUEST_TOO_LONG, SERVER_IP, SERVER_PORT, SERVER_FILE, CLIENT_FILE, OPEN_FILE_SUCCESS, OPEN_FILE_FAIL, REQUEST_START, REQUEST_OPTS, RESPONSE_OPTS, REQUEST_SUCCESS, REQUEST_FAIL, PART_PKT};
// 上传/下载任务结果定义
enum UP_DOWN {UP_DOWN_FAIL, UP_DOWN_SUCCESS};
// 定时器刷新间隔定义
enum INTERVAL {REFRESH_INTERVAL = 1000};
// 服务端端口定义
enum PORT {SERVERPORT = 69};


struct Field
{// 选项字段定义
    QString name;
    int value;
};
struct Opt
{// 扩展选项定义
    unordered_map<QString, int> fields {};
};
struct Request
{// Request包定义
    char reqmsg[MAX_REQUEST_SIZE];
};
struct OACK
{// OACK包定义
    char opt2val[MAX_REQUEST_SIZE];
};
struct Data
{// Data包定义
    WORD block;
    char data[MAX_BLOCK_SIZE];
};
struct ACK
{// ACK包定义
    WORD block;
};
struct Error
{// Error包定义
    WORD errcode;
    char errmsg[MAX_ERROR_SIZE];
};
struct PKT
{// 数据包定义
    WORD opc; // 操作码
    union
    {// 各类型数据包联合体
        Request request;
        OACK oack;
        Data data;
        ACK ack;
        Error error;
    };
};
struct Message
{// 任务过程信息定义
    TYPE type;
    QString info;
};

class Thread : public QThread
{
    Q_OBJECT

public:
    // 初始化线程的函数API
    int init(OPC request_type, int mode, Opt& opt, QString& server_ip, QString& server_file, QString& client_file);

public slots:
    void send_speed(); // 显示实时上行/下行速度的信号

signals:
    void send_message(Message& message); // 显示信息的信号
    void send_opts(Opt& opt); // 显示选项信息的信号
    void send_progress_max(int size); // 设置进度条最大值的信号
    void send_process_value(int filesize); // 设置进度条当前值的信号
    void send_speed_up(QString speed); // 显示实时上行速度的信号
    void send_speed_down(QString speed); // 显示实时下行速度的信号

private:
    PKT m_pkt_recv {}, m_pkt_send {}; // 发送、接收数据包定义
    Message m_message; // 显示信息定义
    FILE * m_fp = nullptr; // 客户端文件指针
    Opt m_opt {}; // 扩展选项
    OPC m_request_type; // 请求类型:下载/上传
    SOCKET m_socket = INVALID_SOCKET; // 客户端SOCKET
    SOCKADDR_IN m_server_addr {}; // SOCKADDR结构体
    int m_server_addr_len = sizeof(m_server_addr);
    QString m_server_file, m_client_file; // 客户端文件、服务端文件
    int m_mode = NETASCII; // 传输模式
    int m_request_len = 0; // 请求包长度
    int m_tsize = 0; // tsize选项值
    unsigned short m_blksize = DEFAULT_BLOCK_SIZE; // blksize选项值
    unsigned short m_timeout = DEFAULT_TIMEOUT; // timeout选项值
    int bytes_down, bytes_up, last_bytes_down, last_bytes_up; // 上行/下行字节数统计
    int sum_retransmit; // 总重发次数
    int filesize; // 传输文件大小统计
    int block; // Data/ACK包序号

    void up_down(); // 上传/下载函数
    bool exceed_retransmit(int retransmit); // 超过最大重发次数处理
    void error_pkt(int size); // Error包处理
    int request(); // 发起Request包
    bool request_retransmit(); // 重新发送Request包
    int upload(); // 上传任务
    int download(); // 下载任务
    int _send(const char* buf, int len); // 发送数据包
    int _recv(char* buf, int len); // 接收数据包
    int _open(); // 打开客户端文件
    int check_len(); // 检查请求包长度
    int pkt(int timeout, int& size); // 获取数据包
    void wait_pkt(WORD block, int type, int timeout, int& size, int& opc); // 获取指定操作码与指定序号的数据包
    void oack(int size); // OACK包处理
    int ack(WORD block); // 发送ACK包
    void update_opts(); // 更新选项值

    // 工具函数
    int sprintf_msg(char* buf, rsize_t bufferCount, int& offset, const char* format, ...);
    int sscanf_msg(char* buf, rsize_t bufferCount, char const* source, int& offset);
    int get_filesize(const char* fileName);
    int local2netascii(const char *fileName);
    int netascii2loacl(const char *fileName, int platform);

protected:
    void run() override;
};
#endif // THREAD_H
