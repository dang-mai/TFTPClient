#include "thread.h"

#pragma execution_character_set("utf-8")

void Thread::run()
{// �߳�ִ�к���
    // ��ʾ������Ϣ
    if (m_request_type == OPC_REQUEST_DOWN)
    {
        m_message.type = TYPE::REQUEST_START;
        m_message.info = QString("DOWNLOAD START");
        emit send_message(m_message);
    }
    else
    {
        m_message.type = TYPE::REQUEST_START;
        m_message.info = QString("UPLOAD START");
        emit send_message(m_message);
    }
    m_message.type = TYPE::SERVER_FILE;
    m_message.info = QString("SERVER FILE : %1").arg(m_server_file);
    emit send_message(m_message);
    m_message.type = TYPE::CLIENT_FILE;
    m_message.info = QString("CLIENT FILE : %1").arg(m_client_file);
    emit send_message(m_message);
    char IP[16] {};
    InetNtopA(AF_INET, &m_server_addr.sin_addr, IP, 16);
    m_message.type = TYPE::SERVER_IP;
    m_message.info = QString("SERVER IP : %1").arg(IP);
    emit send_message(m_message);
    auto PORT = ntohs(m_server_addr.sin_port);
    m_message.type = TYPE::SERVER_PORT;
    m_message.info = QString("SERVER PORT : %1").arg(PORT);
    emit send_message(m_message);
    // ��������/�ϴ�����
    if (request() == enum_integer(TYPE::REQUEST_FAIL))
    {
        m_message.type = TYPE::REQUEST_FAIL;
        m_message.info = QString("REQUEST FAIL");
        emit send_message(m_message);
        quit();
        return;
    }
    up_down(); // ����/�ϴ������������
    quit();
}

void Thread::up_down()
{// �ϴ�/��������ִ�к���
    clock_t time_start, time_end;
    int flag;
    time_start = clock(); // ��¼ִ��ʱ��
    if (m_request_type == enum_integer(OPC_REQUEST_DOWN))
    {// ����
        flag = download();
    }
    else
    {// �ϴ�
        flag = upload();
    }
    time_end = clock();
    double time_delta = (double)(time_end - time_start) / CLOCKS_PER_SEC;
    emit send_progress_max(1);
    emit send_process_value((flag == UP_DOWN_SUCCESS) ? 1 : 0); // �޸��������
    if (flag == UP_DOWN_SUCCESS)
    {// ��ʾ����ɹ���Ϣ
        if (m_mode == NETASCII)
        {
            netascii2loacl(m_client_file.toStdString().c_str(), WINDOWS);
        }
        m_message.type = TYPE::SUCCESS;
        m_message.info.clear();
        if (m_request_type == OPC_REQUEST_DOWN)
        {
            m_message.info += QString("Download Success  : <%1> ==> <%2>").arg(m_server_file, m_client_file);
        }
        else
        {
            m_message.info += QString("Upload Success  : <%1> ==> <%2>").arg(m_client_file, m_server_file);
        }
        m_message.info += "\t";
        m_message.info += QString("Block:<%1 blks | %2 B/blk> ").arg(block - (m_request_type == OPC_REQUEST_DOWN ? 1 : 0)).arg(m_blksize);
        m_message.info += QString("Time:<%1 s> ").arg(time_delta);
        m_message.info += QString("FileSize:<%1 kB> ").arg((double)filesize / 1024);
        m_message.info += QString("Down:<%1 kB> ").arg((double)bytes_down / 1024);
        m_message.info += QString("DownSpeed:<%1 kB/s> ").arg(((double)bytes_down / 1024 / time_delta));
        m_message.info += QString("Up:<%1 kB> ").arg((double)bytes_up / 1024);
        m_message.info += QString("UpSpeed:<%1 kB/s> ").arg(((double)bytes_up / 1024 / time_delta));
        m_message.info += QString("Retransmit:<%1 times> ").arg(sum_retransmit);
        emit send_message(m_message);
    }
    else
    {// ��ʾ����ʧ����Ϣ
        m_message.type = TYPE::FAIL;
        m_message.info.clear();
        if (m_request_type == OPC_REQUEST_DOWN)
        {
            m_message.info += QString("Download FAIL  : <%1> ==> <%2>").arg(m_server_file, m_client_file);
        }
        else
        {
            m_message.info += QString("Upload FAIL  : <%1> ==> <%2>").arg(m_client_file, m_server_file);
        }
        emit send_message(m_message);
    }
    if (m_fp)
    {// �ر��ļ�
        fclose(m_fp);
        m_fp = nullptr;
    }
}

int Thread::init(OPC request_type, int mode, Opt& opt, QString& server_ip, QString& server_file, QString& client_file)
{// ��ʼ���̲߳���
    #ifdef WIN32
    // ��ʼ��WinSock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        m_message.type = TYPE::CONNECTION_ERROR;
        m_message.info = QString("Init Socket Fail.");
        emit send_message(m_message);
        return false;
    }
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) { WSACleanup(); exit(-1); }
    #endif
    // �����̸߳������Եĳ�ʼֵ
    m_request_type = request_type;
    m_mode = mode;
    m_opt = opt;
    m_server_file = server_file;
    m_client_file = client_file;
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_port = htons(SERVERPORT);
    InetPtonA(AF_INET, server_ip.toStdString().c_str(), (void*)&m_server_addr.sin_addr);
    m_server_addr_len = sizeof(m_server_addr);
    m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // ����socket
    if (m_socket == INVALID_SOCKET)
    {
        m_message.type = TYPE::CONNECTION_ERROR;
        m_message.info = QString("Init Socket Fail.");
        emit send_message(m_message);
        return false;
    }
    if (m_mode == NETASCII)
    {// �޸��ļ�����
        local2netascii(m_client_file.toStdString().c_str());
    }
    m_request_len = 0;
    m_tsize = (m_request_type == OPC_REQUEST_UP) ? get_filesize(m_client_file.toStdString().c_str()) : 0;
    m_blksize = DEFAULT_BLOCK_SIZE;
    m_timeout = DEFAULT_TIMEOUT;
    bytes_down = bytes_up = last_bytes_down = last_bytes_up = 0;
    sum_retransmit = 0;
    filesize = 0;
    block = 0;
    if (m_fp)
    {// �ر��ļ�
        fclose(m_fp);
        m_fp = nullptr;
    }
    return true;
}

int Thread::check_len()
{// ������������
    size_t len = 2; // ������
    len += (m_server_file.size() + 1 + strlen((m_mode == NETASCII) ? "netascii" : "octet") + 1); // �ļ�����ģʽ
    for (auto & field : m_opt.fields)
    {// ѡ��
        len += (field.first.size() + 1 + QString(field.second).size() + 1);
    }
    if (len > MAX_REQUEST_SIZE)
    {
       return false;
    }
    return true;
}

int Thread::request()
{// �����ϴ�/��������
    if (m_opt.fields.find(QString("tsize")) != m_opt.fields.end())
    {// ����tsizeֵ
        m_opt.fields[QString("tsize")] = m_tsize;
    }
    update_opts(); // ����ѡ��ֵ��ʾ
    emit send_progress_max(m_tsize);

    if (!check_len())
    {// ��鳤��
        m_message.type = TYPE::REQUEST_TOO_LONG;
        m_message.info = QString("Too Long Request");
        emit send_message(m_message);
        return enum_integer(TYPE::REQUEST_FAIL);
    }
    m_request_len = 0;
    m_pkt_send.opc = htons(m_request_type); // ��������
    // ����ļ���
    sprintf_msg(m_pkt_send.request.reqmsg, MAX_REQUEST_SIZE - OPC_SIZE, m_request_len, "%s", m_server_file.toStdString().c_str());
    // ���ģʽ
    sprintf_msg(m_pkt_send.request.reqmsg, MAX_REQUEST_SIZE - OPC_SIZE, m_request_len, (m_mode == NETASCII) ? "netascii" : "octet");
    for (auto & field : m_opt.fields)
    {// ���ѡ������ѡ��ֵ
        sprintf_msg(m_pkt_send.request.reqmsg, MAX_REQUEST_SIZE - OPC_SIZE, m_request_len, "%s%c%d", field.first.toStdString().c_str(), '\0', field.second);
    }
    int ret = _send((char *) &m_pkt_send, OPC_SIZE + m_request_len);
    if (ret < 0)
    {// ��������ʹ���
        m_message.type = TYPE::REQUEST_FAIL;
        m_message.info = QString("REQUEST : Send request packet fail with code : <%1>.").arg(WSAGetLastError());
        emit send_message(m_message);
        return enum_integer(TYPE::REQUEST_FAIL);
    }
    else if (ret < (OPC_SIZE + m_request_len))
    {// ��������ַ���
        m_message.type = TYPE::PART_PKT;
        m_message.info = QString("REQUEST : Send request packet partically with length : <%1>.").arg(ret);
        emit send_message(m_message);
    }
    else
    {// ��������ͳɹ�
        m_message.type = TYPE::REQUEST_SUCCESS;
        m_message.info = QString("REQUEST : Send request packet success with length : <%1>.").arg(ret);
        emit send_message(m_message);
        m_message.type = TYPE::REQUEST_OPTS;
        m_message.info.clear();
        for (auto & field : m_opt.fields)
        {// ��ʾ���͵�ѡ����Ϣ
            m_message.info += QString("%1 : %2\t").arg(field.first).arg(field.second);
        }
        emit send_message(m_message);
    }
    if (_open() == enum_integer(TYPE::OPEN_FILE_FAIL))
    {// �򿪱����ļ�
        return enum_integer(TYPE::REQUEST_FAIL);
    }

    return enum_integer(TYPE::REQUEST_SUCCESS);
}

bool Thread::request_retransmit()
{// ���·��������
    m_message.type = TYPE::RETRANSMIT_REQUEST;
    m_message.info = QString("Retransmit request packet.");
    emit send_message(m_message);
    if (_send((char *)&m_pkt_send, OPC_SIZE + m_request_len) < 0)
    {// �ط������
        m_message.type = TYPE::REQUEST_FAIL;
        m_message.info = QString("REQUEST : Send request packet fail.");
        emit send_message(m_message);
        return false;
    }
    return true;
}

int Thread::upload()
{// �ϴ��ļ�
    block = 0;
    sum_retransmit = 0;
    filesize = 0;
    int retransmit = 0, size = 0, opc = 0, bytes = 0;
    double percent = 0;
    bool ext_opts = (m_opt.fields.size() ? true : false);
    bool ack_oack = false, finished = false;
    if (ext_opts)
    {// �ͻ���֧����չѡ��
        while (retransmit <= MAX_RETRANSMIT_SIZE)
        {
            wait_pkt(-1, OPC_OACK, m_timeout * 1000, size, opc); // �ȴ�OACK��
            if (opc == OPC_OACK)
            {// �յ�OACK��
                m_message.type = TYPE::EXTENDED_OPTIONS_YES;
                m_message.info = QString("OACK : Support extended options.");
                emit send_message(m_message);
                oack(size);
                ack_oack = true;
                break;
            }
            else if (opc == OPC_ACK)
            {// �յ�ACK�������Ϊ0
                m_message.type = TYPE::EXTENDED_OPTIONS_NO;
                m_message.info = QString("OACK : Not Support extended options.");
                emit send_message(m_message);
                ack_oack = true;
                break;
            }
            else if (opc == OPC_ERROR)
            {// �յ�ERROR��
                error_pkt(size);
                return UP_DOWN_FAIL;
            }
            else
            {// �յ��������Ͱ����߳�ʱ
                retransmit++;
                sum_retransmit++;
                if (!request_retransmit())
                {
                    return UP_DOWN_FAIL;
                }
            }
        }
        if (exceed_retransmit(retransmit))
        {// ����Ƿ񳬹�����ط�����
            return UP_DOWN_FAIL;
        }
    }

    retransmit = 0;
    while (retransmit <= MAX_RETRANSMIT_SIZE)
    {
        if (ack_oack && (block == 0))
        {// �ͻ���֧����չѡ��Ƿ���˲�֧��
            opc = OPC_ACK;
        }
        else
        {// �ͻ��˲�֧����չѡ����߿ͻ��������˾�֧����չѡ��
            wait_pkt(block, OPC_ACK, m_timeout * 1000, size, opc); // �ȴ�ָ����ŵ�ACK��
        }
        if (opc == OPC_ACK)
        {// �յ�ACK�������Ϊ����ֵ
            if (!finished)
            {// �ļ��ϴ�δ����
                m_pkt_send.opc = htons(OPC_DATA);
                m_pkt_send.data.block = htons(++block);
                bytes = (int)fread(m_pkt_send.data.data, 1, m_blksize, m_fp);
                if (bytes < m_blksize)
                {
                    finished = true;
                }
                // ������һ��Data��
                if (_send((char *)&m_pkt_send, bytes + OPC_SIZE + BLOCK_SIZE) < 0)
                {
                    m_message.type = TYPE::DATA_FAIL;
                    m_message.info = QString("DATA <%1> : Send DATA packet fail.").arg(block - 1);
                    emit send_message(m_message);
                    return UP_DOWN_FAIL;
                }
                retransmit = 0;
                filesize += bytes; // ͳ�Ʒ��͵������ֽ���
                if ((double)filesize / m_tsize > percent)
                {// �����ļ��ϴ�����
                    percent = (double)filesize / m_tsize + 0.01;
                    emit send_process_value(filesize);
                }
            }
            else
            {// �ļ��ϴ�����
                return UP_DOWN_SUCCESS;
            }
        }
        else if (opc == OPC_ERROR)
        {// �յ�ERROR��
            error_pkt(size);
            return UP_DOWN_FAIL;
        }
        else
        {// ��ʱ
            retransmit++;
            sum_retransmit++;
            if (!ext_opts && block == 0)
            {// �ͻ��˲�֧����չѡ����δ�յ���ACK��
                if (!request_retransmit())
                {
                    return UP_DOWN_FAIL;
                }
            }
            else
            {// �ͻ���֧����չѡ����߿ͻ��˲�֧����չѡ��Ѿ��յ���ACK��
                m_message.type = TYPE::RETRANSMIT_DATA;
                m_message.info = QString("Retransmit DATA packet <%1>.").arg(block);
                emit send_message(m_message);
                if (_send((char *)&m_pkt_send, bytes + OPC_SIZE + BLOCK_SIZE) < 0)
                {// ���·���Data��
                    m_message.type = TYPE::DATA_FAIL;
                    m_message.info = QString("DATA <%1> : Send DATA packet fail.").arg(block);
                    emit send_message(m_message);
                    return UP_DOWN_FAIL;
                }
            }
        }
    }
    exceed_retransmit(retransmit); // ��������ط��������ƴ���
    return UP_DOWN_FAIL;
}

int Thread::download()
{// �����ļ�
    block = m_opt.fields.size() ? 0 : 1;
    sum_retransmit = 0;
    filesize = 0;
    int retransmit = 0, size = 0, opc = 0, bytes = 0;
    double percent = 0;
    bool data = false;
    if (!block)
    {// �ͻ���֧����չѡ��
        while (retransmit < MAX_RETRANSMIT_SIZE)
        {
            wait_pkt(-1, OPC_OACK, m_timeout * 1000, size, opc); // �ȴ�OACK��
            if (opc == OPC_OACK)
            {// �յ�OACK��
                m_message.type = TYPE::EXTENDED_OPTIONS_YES;
                m_message.info = QString("OACK : Support extended options.");
                emit send_message(m_message);
                oack(size);
                if (ack(block++) < 0)
                {
                    return UP_DOWN_FAIL;
                }
                break;
            }
            else if (opc == OPC_DATA)
            {// �յ�Data�������Ϊ1
                m_message.type = TYPE::EXTENDED_OPTIONS_NO;
                m_message.info = QString("OACK : Not Support extended options.");
                emit send_message(m_message);
                block = 1;
                data = true;
                break;
            }
            else if (opc == OPC_ERROR)
            {// �յ�Error��
                error_pkt(size);
                return UP_DOWN_FAIL;
            }
            else
            {// �յ��������Ͱ����߳�ʱ
                retransmit++;
                sum_retransmit++;
                if (!request_retransmit())
                {
                    return UP_DOWN_FAIL;
                }
            }
        }
        if (exceed_retransmit(retransmit))
        {// ����Ƿ񳬹�����ط�����
            return UP_DOWN_FAIL;
        }
    }

    retransmit = 0;
    while (retransmit <= MAX_RETRANSMIT_SIZE)
    {
        if (data && (block == 1))
        {// �ͻ���֧����չѡ��Ƿ���˲�֧��
            opc = OPC_DATA;
        }
        else
        {// �ͻ��˲�֧����չѡ����߿ͻ��������˾�֧����չѡ��
            wait_pkt(block, OPC_DATA, m_timeout * 1000, size, opc); // �ȴ�ָ����ŵ�Data��
        }
        if (opc == OPC_DATA)
        {// �յ�Data�������Ϊ����ֵ
            bytes = (int)fwrite(m_pkt_recv.data.data, 1, size - OPC_SIZE - BLOCK_SIZE, m_fp); // ����Data���������ݵ������ļ���
            if (bytes < ((size_t)size - OPC_SIZE - BLOCK_SIZE))
            {// ����Ƿ�д��ɹ�
                m_message.type = TYPE::WRITE_FAIL;
                m_message.info = QString("FWRITE : Fail.");
                emit send_message(m_message);
                return UP_DOWN_FAIL;
            }
            if (ack(block) < 0)
            {// ����ACK�������Ϊָ��ֵ
                return UP_DOWN_FAIL;
            }
            block++;
            retransmit = 0;
            filesize += size - OPC_SIZE - BLOCK_SIZE;
            if (size == m_blksize + OPC_SIZE + BLOCK_SIZE)
            {// ����δ����
                if (m_opt.fields.find(QString("tsize")) != m_opt.fields.end())
                {
                    if ((double)filesize / m_tsize > percent)
                    {// �������ؽ���
                        percent = (double)filesize / m_tsize + 0.01;
                        emit send_process_value(filesize);
                    }
                }
            }
            else
            {// ���ؽ���
                return UP_DOWN_SUCCESS;
            }
        }
        else if (opc == OPC_ERROR)
        {// �յ�Error��
            error_pkt(size);
            return UP_DOWN_FAIL;
        }
        else
        {// ��ʱ
            retransmit++;
            sum_retransmit++;
            if (!data && (block == 1))
            {// �ͻ��˲�֧����չѡ����δ�յ���Data��
                if (!request_retransmit())
                {
                    return UP_DOWN_FAIL;
                }
            }
            else
            {// �ͻ���֧����չѡ����߿ͻ��˲�֧����չѡ��Ѿ��յ���Data��
                m_message.type = TYPE::RETRANSMIT_ACK;
                m_message.info = QString("Retransmit ACK packet <%1>.").arg(block - 1);
                emit send_message(m_message);
                if (ack(block - 1) < 0)
                {
                    return UP_DOWN_FAIL;
                }
            }
        }
    }
    exceed_retransmit(retransmit); // ��������ط��������ƴ���
    return UP_DOWN_FAIL;
}

void Thread::error_pkt(int size)
{// Error������
    if (size < OPC_SIZE + ERRCODE_SIZE)
    {// Error��������
        m_message.type = TYPE::WRONG_PKT;
        m_message.info = QString("ERROR : Wrong error packet.");
        emit send_message(m_message);
    }
    else
    {// Error������
        m_message.type = TYPE::ERROR_PKT;
        m_message.info = QString("ERROR : errCode: %1 errMsg: %2").arg(ntohs(m_pkt_recv.error.errcode)).arg(m_pkt_recv.error.errmsg);
        emit send_message(m_message);
    }
}

bool Thread::exceed_retransmit(int retransmit)
{// ����Ƿ񳬹�����ط�����������
    if (retransmit > MAX_RETRANSMIT_SIZE)
    {
        m_message.type = TYPE::RETRANSMIT_TOO_MUCH;
        m_message.info = QString("Retransmission times too much.");
        emit send_message(m_message);
        return true;
    }
    return false;
}

void Thread::update_opts()
{// ����ѡ��ֵ
    Opt opt = m_opt;
    // ����tsizeֵ
    if (opt.fields.find(QString("tsize")) != opt.fields.end())
    {
        m_tsize = opt.fields[QString("tsize")];
    }
    else
    {
        opt.fields[QString("tsize")] = m_tsize;
    }
    // ����blksizeֵ
    if (opt.fields.find(QString("blksize")) != opt.fields.end())
    {
        m_blksize = opt.fields[QString("blksize")];
    }
    else
    {
        opt.fields[QString("blksize")] = m_blksize;
    }
    // ����timeoutֵ
    if (opt.fields.find(QString("timeout")) != opt.fields.end())
    {
        m_timeout = opt.fields[QString("timeout")];
    }
    else
    {
        opt.fields[QString("timeout")] = m_timeout;
    }
    emit send_opts(opt);
}

void Thread::oack(int size)
{// ������յ���OACK��
    char opt[MAX_REQUEST_SIZE] {}, val[MAX_REQUEST_SIZE] {};
    m_opt.fields.clear();
    for (int len = 0; len < size - 2; )
    {// ���λ�ȡ����ѡ������ѡ��ֵ
        sscanf_msg(opt, MAX_REQUEST_SIZE, m_pkt_recv.oack.opt2val, len);
        sscanf_msg(val, MAX_REQUEST_SIZE, m_pkt_recv.oack.opt2val, len);
        m_opt.fields[QString(opt)] = atoi(val);
    }
    m_message.type = TYPE::RESPONSE_OPTS;
    m_message.info.clear();
    for (auto & field : m_opt.fields)
    {
        m_message.info += QString("%1 : %2\t").arg(field.first).arg(field.second);
    }
    emit send_message(m_message); // ��ʾ����ѡ������
    update_opts(); // ��������ѡ��ֵ
    emit send_progress_max(m_tsize); // ���½��������ֵ
}

int Thread::ack(WORD block)
{// ����ACK��
    m_pkt_send.opc = htons(OPC_ACK); // ����ת��
    m_pkt_send.ack.block = htons(block);
    int ret = _send((char *) &m_pkt_send, OPC_SIZE + BLOCK_SIZE);
    if (ret < 0)
    {
        m_message.type = TYPE::ACK_FAIL;
        m_message.info = QString("ACK  <%1> : Send ACK packet fail.").arg(block);
        emit send_message(m_message);
    }
    return ret;
}

int Thread::pkt(int timeout, int& size)
{// ��ȡ���ݰ�
    FD_SET socket;
    FD_ZERO(&socket);
    FD_SET(m_socket, &socket);
    TIMEVAL timeval = {timeout / 1000, timeout % 1000};
    // ��ָ����ʱʱ���ڽ���һ�����ݰ�
    auto ret = select(m_socket + 1, &socket, nullptr, nullptr, &timeval);
    if (ret == SOCKET_ERROR)
    {// �׽��ִ���
        return PKT_SOCKET_ERROR;
    }
    else if (ret == 0)
    {// ��ʱ
        return PKT_TIMEOUT;
    }
    else
    {// �ɹ�����
        size = _recv((char *)&m_pkt_recv, sizeof(PKT));
        // ���ݽ������ݰ������ķ���ֵ���д���
        if (size == SOCKET_ERROR)
        {
            m_message.type = TYPE::CONNECTION_ERROR;
            m_message.info = QString("RECV : SOCKET_ERROR.");
            emit send_message(m_message);
            return PKT_FAIL;
        }
        else if (size == 0)
        {
            m_message.type = TYPE::CONNECTION_CLOSE;
            m_message.info = QString("RECV : The connection has been closed.");
            emit send_message(m_message);
            return PKT_FAIL;
        }
    }
    auto opc = ntohs(m_pkt_recv.opc);
    // �жϽ��հ��Ƿ�����
    if (opc == OPC_DATA || opc == OPC_ACK || opc == OPC_ERROR)
    {
        if (size < OPC_SIZE + MODE_SIZE)
        {
            return PKT_FAIL;
        }
    }
    else if (opc == OPC_OACK)
    {
        if (size < OPC_SIZE)
        {
            return PKT_FAIL;
        }
    }
    return PKT_SUCCESS;
}

void Thread::wait_pkt(WORD block, int type, int timeout, int& size, int& opc)
{// ��ȡָ����������ָ����ŵ����ݰ�
    int ret;
    for (bool flag = true; flag; )
    {
        auto start = clock();
        ret = pkt(timeout, size);
        timeout -= (clock() - start); // ����ʣ��ȴ�ʱ��
        switch (type)
        {// �������ݰ���������в�ͬ�Ĵ���
            case OPC_DATA:
            case OPC_ACK:
            // ���ָ����ŵ�ACK/Data������Error��
                if ((ret == PKT_SUCCESS) && (ntohs(m_pkt_recv.opc) != OPC_ERROR) &&
                        ((ntohs(m_pkt_recv.opc) != type) || (ntohs(m_pkt_recv.data.block) != block)
                         || (ntohs(m_pkt_recv.ack.block) != block)))
                {
                    continue;
                }
                else
                {
                    flag = false;
                    continue;
                }
                break;
            case OPC_OACK:
            // ���OACK���������Ϊ1��Data���������Ϊ0��ACK������Error��
                if ((ret == PKT_SUCCESS) &&
                        (m_request_type == OPC_REQUEST_DOWN ?
                         (ntohs(m_pkt_recv.opc) == OPC_DATA && ntohs(m_pkt_recv.data.block) != 1) :
                         (ntohs(m_pkt_recv.opc) == OPC_ACK && ntohs(m_pkt_recv.ack.block) != 0)))
                {
                    continue;
                }
                else
                {
                    flag = false;
                    continue;
                }
                break;
        }
    }
    if (timeout <= 0)
    {// �ȴ���ʱ����
        m_message.type = TYPE::TIMEOUT;
        // ���ݵȴ������ݰ���������ʾ��ͬ����Ϣ
        if (type == OPC_ACK)
        {
            m_message.info = QString("ACK <%1>: Timeout.").arg(block);
        }
        else if (type == OPC_DATA)
        {
            m_message.info = QString("DATA <%1>: Timeout.").arg(block);
        }
        else if (type == OPC_OACK)
        {
            m_message.info = QString("OACK : Timeout.");
        }
        emit send_message(m_message);
    }
    if (ret == PKT_SUCCESS)
    {// ���سɹ����յ����ݰ��Ĳ�����
        opc = ntohs(m_pkt_recv.opc);
    }
    else
    {// ����0(δ�ɹ��������ݰ�)
        opc = 0;
    }
}

int Thread::_open()
{// �򿪱����ļ�
    if (fopen_s(&m_fp, m_client_file.toStdString().c_str(), (m_request_type == OPC_REQUEST_DOWN) ? "wb" : "rb") == 0)
    {// �򿪱����ļ��ɹ�
        m_message.type = TYPE::OPEN_FILE_SUCCESS;
        m_message.info = QString("Open file : Success : <%1>.").arg(m_client_file);
        emit send_message(m_message);
        return enum_integer(TYPE::OPEN_FILE_SUCCESS);
    }
    else
    {// �򿪱����ļ�ʧ��
        m_message.type = TYPE::OPEN_FILE_FAIL;
        m_message.info = QString("Open file : Fail : <%1>.").arg(m_client_file);
        emit send_message(m_message);
        return enum_integer(TYPE::OPEN_FILE_FAIL);
    }
}

int Thread::_send(const char* buf, int len)
{// �������ݰ�
    int ret = sendto(m_socket, buf, len, 0, (SOCKADDR *)&m_server_addr, sizeof(m_server_addr));
    if (ret > 0)
    {
        bytes_up += ret; // ͳ�Ʒ��͵��ֽ���
    }
    return ret;
}

int Thread::_recv(char* buf, int len)
{// �������ݰ�
    int ret = recvfrom(m_socket, buf, len, 0, (SOCKADDR *)&m_server_addr, &m_server_addr_len);
    if (ret > 0)
    {
        bytes_down += ret; // ͳ�ƽ��յ��ֽ���
    }
    return ret;
}

void Thread::send_speed()
{// ���������������ٶ�(ʵʱ������)
    emit send_speed_up(QString::number(((double)bytes_up - last_bytes_up) / REFRESH_INTERVAL * 1000 / 1024));
    emit send_speed_down(QString::number(((double)bytes_down - last_bytes_down) / REFRESH_INTERVAL * 1000 / 1024));
    last_bytes_down = bytes_down;
    last_bytes_up = bytes_up;
}

int Thread::sprintf_msg(char* buf, rsize_t bufferCount, int& offset, const char* format, ...)
{// ��buf��offset����ʼ��ӡ��ʽ���ַ���
    va_list args;
    va_start(args, format);
    int ret = vsprintf_s(buf + offset, bufferCount - offset, format, args);
    va_end(args);
    offset += (ret + 1); // vsprintf_s��������ֵ��������ֹ��null�ַ�
    return ret;
}

int Thread::sscanf_msg(char* buf, rsize_t bufferCount, char const* source, int& offset)
{// ��source��offset����ȡ�ַ�����buf��
    int ret = sprintf_s(buf, bufferCount, "%s", source + offset);
    offset += (ret + 1); // sprintf_s��������ֵ��������ֹ��null�ַ�
    return ret;
}

int Thread::get_filesize(const char* fileName)
{// ����ļ���С
    struct stat statBuf;
    if (stat(fileName, &statBuf) != 0) { return -1; }
    return statBuf.st_size;
}

int Thread::local2netascii(const char *fileName)
{// ���ر���=>NETASCII����
    FILE *fp = NULL, *tfp = NULL;
    char tempFileName[1024];
    strcpy_s(tempFileName, 1023, fileName);
    strcat_s(tempFileName, 1023, ".netascii");
    fopen_s(&fp, fileName, "rb");
    fopen_s(&tfp, tempFileName, "wb");
    if (fp == NULL || tfp == NULL)
    {
        if (fp)
        {
            fclose(fp);
        }
        if (fp)
        {
            fclose(tfp);
        }
        return -1;
    }
    int ch = fgetc(fp), lch = END;
    // LF => CRLF
    // CR => CRNUL
    while (true)
    {
        if (ch == LF && lch != CR)
        {// LF => CRLF
            fputc(CR, tfp);
        }
        else if (lch == CR && ch != LF && ch != END)
        {// CR => CRNUL
            fputc(END, tfp);
        }
        else if (ch == EOF)
        {
            break;
        }
        fputc(ch, tfp);
        lch = ch;
        ch = fgetc(fp);
    }
    fclose(fp);
    fclose(tfp);
    return 0;
}

int Thread::netascii2loacl(const char *fileName, int platform)
{// NETASCII����=>���ر���
    FILE *fp = NULL, *tfp = NULL;
    char temp[1024];
    strcpy_s(temp, 1023, fileName);
    strcat_s(temp, 1023, ".local");
    fopen_s(&fp, fileName, "rb");
    fopen_s(&tfp, temp, "wb");
    if (fp == NULL || tfp == NULL)
    {
        if (fp)
        {
            fclose(fp);
        }
        if (fp)
        {
            fclose(tfp);
        }
        return -1;
    }
    int ch = 0, nch = 0;
    // Windows: CRLF => CRLF CRNUL => CR
    // Linux:   CRLF => LF   CRNUL => CR
    while (true)
    {
        ch = fgetc(fp);
        if (ch == CR)
        {
            nch = fgetc(fp);
            if ( platform != WINDOWS && nch == LF)
            {
                ch = LF;
            }
            else if (nch == END)
            {
                ch = CR;
            }
            else
            {
                fseek(fp, -1L, SEEK_CUR);
            }
        }
        else if (ch == EOF)
        {
            break;
        }
        fputc(ch, tfp);
    }
    fclose(fp);
    fclose(tfp);
    return 0;
}
