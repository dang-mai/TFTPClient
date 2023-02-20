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

// �ͻ���ƽ̨����
enum PLATFORM {WINDOWS = 1, LINUX = 2};
// �ļ��ָ�������
enum DELIMITER {CR = '\r', LF = '\n', END = '\0'};
// �ֶδ�С
enum FIELDSIZE {OPC_SIZE = 2, MODE_SIZE = 2, BLOCK_SIZE = 2, ERRCODE_SIZE = 2};
// ���ֵ����
enum MAXSIZE {MAX_RETRANSMIT_SIZE = 10, MAX_REQUEST_SIZE = 512, MAX_BLOCK_SIZE = 16384, MAX_ERROR_SIZE = 512};
// ���ݰ������붨��
enum OPC {OPC_REQUEST_DOWN = 1, OPC_REQUEST_UP = 2, OPC_DATA = 3, OPC_ACK = 4, OPC_ERROR = 5, OPC_OACK = 6};
// ѡ��Ĭ��ֵ����
enum DEFAULT_OPTS {DEFAULT_TIMEOUT = 2, DEFAULT_BLOCK_SIZE = 512};
// ���ݰ�����������
enum PACKET {PKT_SOCKET_ERROR, PKT_TIMEOUT, PKT_CON_CLOSE, PKT_SUCCESS, PKT_FAIL};
// ����ģʽ����
enum MODE {NETASCII = 1, OCTET = 2};
// �������������
enum CHECK {INVALID_ARGS = -1, VALID_ARGS};
// ��ʾ��Ϣ���Ͷ���
enum class TYPE {INIT_FAIL, RETRANSMIT_DATA, DATA_FAIL, RETRANSMIT_ACK, WRITE_FAIL, RETRANSMIT_TOO_MUCH, EXTENDED_OPTIONS_YES, EXTENDED_OPTIONS_NO, WRONG_PKT, ERROR_PKT, RETRANSMIT_REQUEST, ACK_FAIL, EXTENDED_OPTIONS, TIMEOUT, CONNECTION_ERROR, CONNECTION_CLOSE, FAIL, SUCCESS, REQUEST_TOO_LONG, SERVER_IP, SERVER_PORT, SERVER_FILE, CLIENT_FILE, OPEN_FILE_SUCCESS, OPEN_FILE_FAIL, REQUEST_START, REQUEST_OPTS, RESPONSE_OPTS, REQUEST_SUCCESS, REQUEST_FAIL, PART_PKT};
// �ϴ�/��������������
enum UP_DOWN {UP_DOWN_FAIL, UP_DOWN_SUCCESS};
// ��ʱ��ˢ�¼������
enum INTERVAL {REFRESH_INTERVAL = 1000};
// ����˶˿ڶ���
enum PORT {SERVERPORT = 69};


struct Field
{// ѡ���ֶζ���
    QString name;
    int value;
};
struct Opt
{// ��չѡ���
    unordered_map<QString, int> fields {};
};
struct Request
{// Request������
    char reqmsg[MAX_REQUEST_SIZE];
};
struct OACK
{// OACK������
    char opt2val[MAX_REQUEST_SIZE];
};
struct Data
{// Data������
    WORD block;
    char data[MAX_BLOCK_SIZE];
};
struct ACK
{// ACK������
    WORD block;
};
struct Error
{// Error������
    WORD errcode;
    char errmsg[MAX_ERROR_SIZE];
};
struct PKT
{// ���ݰ�����
    WORD opc; // ������
    union
    {// ���������ݰ�������
        Request request;
        OACK oack;
        Data data;
        ACK ack;
        Error error;
    };
};
struct Message
{// ���������Ϣ����
    TYPE type;
    QString info;
};

class Thread : public QThread
{
    Q_OBJECT

public:
    // ��ʼ���̵߳ĺ���API
    int init(OPC request_type, int mode, Opt& opt, QString& server_ip, QString& server_file, QString& client_file);

public slots:
    void send_speed(); // ��ʾʵʱ����/�����ٶȵ��ź�

signals:
    void send_message(Message& message); // ��ʾ��Ϣ���ź�
    void send_opts(Opt& opt); // ��ʾѡ����Ϣ���ź�
    void send_progress_max(int size); // ���ý��������ֵ���ź�
    void send_process_value(int filesize); // ���ý�������ǰֵ���ź�
    void send_speed_up(QString speed); // ��ʾʵʱ�����ٶȵ��ź�
    void send_speed_down(QString speed); // ��ʾʵʱ�����ٶȵ��ź�

private:
    PKT m_pkt_recv {}, m_pkt_send {}; // ���͡��������ݰ�����
    Message m_message; // ��ʾ��Ϣ����
    FILE * m_fp = nullptr; // �ͻ����ļ�ָ��
    Opt m_opt {}; // ��չѡ��
    OPC m_request_type; // ��������:����/�ϴ�
    SOCKET m_socket = INVALID_SOCKET; // �ͻ���SOCKET
    SOCKADDR_IN m_server_addr {}; // SOCKADDR�ṹ��
    int m_server_addr_len = sizeof(m_server_addr);
    QString m_server_file, m_client_file; // �ͻ����ļ���������ļ�
    int m_mode = NETASCII; // ����ģʽ
    int m_request_len = 0; // ���������
    int m_tsize = 0; // tsizeѡ��ֵ
    unsigned short m_blksize = DEFAULT_BLOCK_SIZE; // blksizeѡ��ֵ
    unsigned short m_timeout = DEFAULT_TIMEOUT; // timeoutѡ��ֵ
    int bytes_down, bytes_up, last_bytes_down, last_bytes_up; // ����/�����ֽ���ͳ��
    int sum_retransmit; // ���ط�����
    int filesize; // �����ļ���Сͳ��
    int block; // Data/ACK�����

    void up_down(); // �ϴ�/���غ���
    bool exceed_retransmit(int retransmit); // ��������ط���������
    void error_pkt(int size); // Error������
    int request(); // ����Request��
    bool request_retransmit(); // ���·���Request��
    int upload(); // �ϴ�����
    int download(); // ��������
    int _send(const char* buf, int len); // �������ݰ�
    int _recv(char* buf, int len); // �������ݰ�
    int _open(); // �򿪿ͻ����ļ�
    int check_len(); // ������������
    int pkt(int timeout, int& size); // ��ȡ���ݰ�
    void wait_pkt(WORD block, int type, int timeout, int& size, int& opc); // ��ȡָ����������ָ����ŵ����ݰ�
    void oack(int size); // OACK������
    int ack(WORD block); // ����ACK��
    void update_opts(); // ����ѡ��ֵ

    // ���ߺ���
    int sprintf_msg(char* buf, rsize_t bufferCount, int& offset, const char* format, ...);
    int sscanf_msg(char* buf, rsize_t bufferCount, char const* source, int& offset);
    int get_filesize(const char* fileName);
    int local2netascii(const char *fileName);
    int netascii2loacl(const char *fileName, int platform);

protected:
    void run() override;
};
#endif // THREAD_H
