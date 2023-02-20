#include "thread.h"

#pragma execution_character_set("utf-8")

void Thread::run()
{// 线程执行函数
    // 显示配置信息
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
    // 发送下载/上传请求
    if (request() == enum_integer(TYPE::REQUEST_FAIL))
    {
        m_message.type = TYPE::REQUEST_FAIL;
        m_message.info = QString("REQUEST FAIL");
        emit send_message(m_message);
        quit();
        return;
    }
    up_down(); // 下载/上传任务主体代码
    quit();
}

void Thread::up_down()
{// 上传/下载任务执行函数
    clock_t time_start, time_end;
    int flag;
    time_start = clock(); // 记录执行时间
    if (m_request_type == enum_integer(OPC_REQUEST_DOWN))
    {// 下载
        flag = download();
    }
    else
    {// 上传
        flag = upload();
    }
    time_end = clock();
    double time_delta = (double)(time_end - time_start) / CLOCKS_PER_SEC;
    emit send_progress_max(1);
    emit send_process_value((flag == UP_DOWN_SUCCESS) ? 1 : 0); // 修改任务进度
    if (flag == UP_DOWN_SUCCESS)
    {// 显示任务成功信息
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
    {// 显示任务失败信息
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
    {// 关闭文件
        fclose(m_fp);
        m_fp = nullptr;
    }
}

int Thread::init(OPC request_type, int mode, Opt& opt, QString& server_ip, QString& server_file, QString& client_file)
{// 初始化线程参数
    #ifdef WIN32
    // 初始化WinSock
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
    // 设置线程各种属性的初始值
    m_request_type = request_type;
    m_mode = mode;
    m_opt = opt;
    m_server_file = server_file;
    m_client_file = client_file;
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_port = htons(SERVERPORT);
    InetPtonA(AF_INET, server_ip.toStdString().c_str(), (void*)&m_server_addr.sin_addr);
    m_server_addr_len = sizeof(m_server_addr);
    m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // 创建socket
    if (m_socket == INVALID_SOCKET)
    {
        m_message.type = TYPE::CONNECTION_ERROR;
        m_message.info = QString("Init Socket Fail.");
        emit send_message(m_message);
        return false;
    }
    if (m_mode == NETASCII)
    {// 修改文件编码
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
    {// 关闭文件
        fclose(m_fp);
        m_fp = nullptr;
    }
    return true;
}

int Thread::check_len()
{// 检查请求包长度
    size_t len = 2; // 操作码
    len += (m_server_file.size() + 1 + strlen((m_mode == NETASCII) ? "netascii" : "octet") + 1); // 文件名和模式
    for (auto & field : m_opt.fields)
    {// 选项
        len += (field.first.size() + 1 + QString(field.second).size() + 1);
    }
    if (len > MAX_REQUEST_SIZE)
    {
       return false;
    }
    return true;
}

int Thread::request()
{// 发送上传/下载请求
    if (m_opt.fields.find(QString("tsize")) != m_opt.fields.end())
    {// 修正tsize值
        m_opt.fields[QString("tsize")] = m_tsize;
    }
    update_opts(); // 更新选项值显示
    emit send_progress_max(m_tsize);

    if (!check_len())
    {// 检查长度
        m_message.type = TYPE::REQUEST_TOO_LONG;
        m_message.info = QString("Too Long Request");
        emit send_message(m_message);
        return enum_integer(TYPE::REQUEST_FAIL);
    }
    m_request_len = 0;
    m_pkt_send.opc = htons(m_request_type); // 填充操作码
    // 填充文件名
    sprintf_msg(m_pkt_send.request.reqmsg, MAX_REQUEST_SIZE - OPC_SIZE, m_request_len, "%s", m_server_file.toStdString().c_str());
    // 填充模式
    sprintf_msg(m_pkt_send.request.reqmsg, MAX_REQUEST_SIZE - OPC_SIZE, m_request_len, (m_mode == NETASCII) ? "netascii" : "octet");
    for (auto & field : m_opt.fields)
    {// 填充选项名和选项值
        sprintf_msg(m_pkt_send.request.reqmsg, MAX_REQUEST_SIZE - OPC_SIZE, m_request_len, "%s%c%d", field.first.toStdString().c_str(), '\0', field.second);
    }
    int ret = _send((char *) &m_pkt_send, OPC_SIZE + m_request_len);
    if (ret < 0)
    {// 请求包发送错误
        m_message.type = TYPE::REQUEST_FAIL;
        m_message.info = QString("REQUEST : Send request packet fail with code : <%1>.").arg(WSAGetLastError());
        emit send_message(m_message);
        return enum_integer(TYPE::REQUEST_FAIL);
    }
    else if (ret < (OPC_SIZE + m_request_len))
    {// 请求包部分发送
        m_message.type = TYPE::PART_PKT;
        m_message.info = QString("REQUEST : Send request packet partically with length : <%1>.").arg(ret);
        emit send_message(m_message);
    }
    else
    {// 请求包发送成功
        m_message.type = TYPE::REQUEST_SUCCESS;
        m_message.info = QString("REQUEST : Send request packet success with length : <%1>.").arg(ret);
        emit send_message(m_message);
        m_message.type = TYPE::REQUEST_OPTS;
        m_message.info.clear();
        for (auto & field : m_opt.fields)
        {// 显示发送的选项信息
            m_message.info += QString("%1 : %2\t").arg(field.first).arg(field.second);
        }
        emit send_message(m_message);
    }
    if (_open() == enum_integer(TYPE::OPEN_FILE_FAIL))
    {// 打开本地文件
        return enum_integer(TYPE::REQUEST_FAIL);
    }

    return enum_integer(TYPE::REQUEST_SUCCESS);
}

bool Thread::request_retransmit()
{// 重新发送请求包
    m_message.type = TYPE::RETRANSMIT_REQUEST;
    m_message.info = QString("Retransmit request packet.");
    emit send_message(m_message);
    if (_send((char *)&m_pkt_send, OPC_SIZE + m_request_len) < 0)
    {// 重发请求包
        m_message.type = TYPE::REQUEST_FAIL;
        m_message.info = QString("REQUEST : Send request packet fail.");
        emit send_message(m_message);
        return false;
    }
    return true;
}

int Thread::upload()
{// 上传文件
    block = 0;
    sum_retransmit = 0;
    filesize = 0;
    int retransmit = 0, size = 0, opc = 0, bytes = 0;
    double percent = 0;
    bool ext_opts = (m_opt.fields.size() ? true : false);
    bool ack_oack = false, finished = false;
    if (ext_opts)
    {// 客户端支持扩展选项
        while (retransmit <= MAX_RETRANSMIT_SIZE)
        {
            wait_pkt(-1, OPC_OACK, m_timeout * 1000, size, opc); // 等待OACK包
            if (opc == OPC_OACK)
            {// 收到OACK包
                m_message.type = TYPE::EXTENDED_OPTIONS_YES;
                m_message.info = QString("OACK : Support extended options.");
                emit send_message(m_message);
                oack(size);
                ack_oack = true;
                break;
            }
            else if (opc == OPC_ACK)
            {// 收到ACK包且序号为0
                m_message.type = TYPE::EXTENDED_OPTIONS_NO;
                m_message.info = QString("OACK : Not Support extended options.");
                emit send_message(m_message);
                ack_oack = true;
                break;
            }
            else if (opc == OPC_ERROR)
            {// 收到ERROR包
                error_pkt(size);
                return UP_DOWN_FAIL;
            }
            else
            {// 收到其他类型包或者超时
                retransmit++;
                sum_retransmit++;
                if (!request_retransmit())
                {
                    return UP_DOWN_FAIL;
                }
            }
        }
        if (exceed_retransmit(retransmit))
        {// 检查是否超过最大重发次数
            return UP_DOWN_FAIL;
        }
    }

    retransmit = 0;
    while (retransmit <= MAX_RETRANSMIT_SIZE)
    {
        if (ack_oack && (block == 0))
        {// 客户端支持扩展选项但是服务端不支持
            opc = OPC_ACK;
        }
        else
        {// 客户端不支持扩展选项或者客户端与服务端均支持扩展选项
            wait_pkt(block, OPC_ACK, m_timeout * 1000, size, opc); // 等待指定序号的ACK包
        }
        if (opc == OPC_ACK)
        {// 收到ACK包且序号为期望值
            if (!finished)
            {// 文件上传未结束
                m_pkt_send.opc = htons(OPC_DATA);
                m_pkt_send.data.block = htons(++block);
                bytes = (int)fread(m_pkt_send.data.data, 1, m_blksize, m_fp);
                if (bytes < m_blksize)
                {
                    finished = true;
                }
                // 发送下一个Data包
                if (_send((char *)&m_pkt_send, bytes + OPC_SIZE + BLOCK_SIZE) < 0)
                {
                    m_message.type = TYPE::DATA_FAIL;
                    m_message.info = QString("DATA <%1> : Send DATA packet fail.").arg(block - 1);
                    emit send_message(m_message);
                    return UP_DOWN_FAIL;
                }
                retransmit = 0;
                filesize += bytes; // 统计发送的数据字节数
                if ((double)filesize / m_tsize > percent)
                {// 更新文件上传进度
                    percent = (double)filesize / m_tsize + 0.01;
                    emit send_process_value(filesize);
                }
            }
            else
            {// 文件上传结束
                return UP_DOWN_SUCCESS;
            }
        }
        else if (opc == OPC_ERROR)
        {// 收到ERROR包
            error_pkt(size);
            return UP_DOWN_FAIL;
        }
        else
        {// 超时
            retransmit++;
            sum_retransmit++;
            if (!ext_opts && block == 0)
            {// 客户端不支持扩展选项且未收到过ACK包
                if (!request_retransmit())
                {
                    return UP_DOWN_FAIL;
                }
            }
            else
            {// 客户端支持扩展选项或者客户端不支持扩展选项但已经收到过ACK包
                m_message.type = TYPE::RETRANSMIT_DATA;
                m_message.info = QString("Retransmit DATA packet <%1>.").arg(block);
                emit send_message(m_message);
                if (_send((char *)&m_pkt_send, bytes + OPC_SIZE + BLOCK_SIZE) < 0)
                {// 重新发送Data包
                    m_message.type = TYPE::DATA_FAIL;
                    m_message.info = QString("DATA <%1> : Send DATA packet fail.").arg(block);
                    emit send_message(m_message);
                    return UP_DOWN_FAIL;
                }
            }
        }
    }
    exceed_retransmit(retransmit); // 超过最大重发次数限制处理
    return UP_DOWN_FAIL;
}

int Thread::download()
{// 下载文件
    block = m_opt.fields.size() ? 0 : 1;
    sum_retransmit = 0;
    filesize = 0;
    int retransmit = 0, size = 0, opc = 0, bytes = 0;
    double percent = 0;
    bool data = false;
    if (!block)
    {// 客户端支持扩展选项
        while (retransmit < MAX_RETRANSMIT_SIZE)
        {
            wait_pkt(-1, OPC_OACK, m_timeout * 1000, size, opc); // 等待OACK包
            if (opc == OPC_OACK)
            {// 收到OACK包
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
            {// 收到Data包且序号为1
                m_message.type = TYPE::EXTENDED_OPTIONS_NO;
                m_message.info = QString("OACK : Not Support extended options.");
                emit send_message(m_message);
                block = 1;
                data = true;
                break;
            }
            else if (opc == OPC_ERROR)
            {// 收到Error包
                error_pkt(size);
                return UP_DOWN_FAIL;
            }
            else
            {// 收到其他类型包或者超时
                retransmit++;
                sum_retransmit++;
                if (!request_retransmit())
                {
                    return UP_DOWN_FAIL;
                }
            }
        }
        if (exceed_retransmit(retransmit))
        {// 检查是否超过最大重发次数
            return UP_DOWN_FAIL;
        }
    }

    retransmit = 0;
    while (retransmit <= MAX_RETRANSMIT_SIZE)
    {
        if (data && (block == 1))
        {// 客户端支持扩展选项但是服务端不支持
            opc = OPC_DATA;
        }
        else
        {// 客户端不支持扩展选项或者客户端与服务端均支持扩展选项
            wait_pkt(block, OPC_DATA, m_timeout * 1000, size, opc); // 等待指定序号的Data包
        }
        if (opc == OPC_DATA)
        {// 收到Data包且序号为期望值
            bytes = (int)fwrite(m_pkt_recv.data.data, 1, size - OPC_SIZE - BLOCK_SIZE, m_fp); // 读出Data包数据内容到本地文件中
            if (bytes < ((size_t)size - OPC_SIZE - BLOCK_SIZE))
            {// 检查是否写入成功
                m_message.type = TYPE::WRITE_FAIL;
                m_message.info = QString("FWRITE : Fail.");
                emit send_message(m_message);
                return UP_DOWN_FAIL;
            }
            if (ack(block) < 0)
            {// 发送ACK包且序号为指定值
                return UP_DOWN_FAIL;
            }
            block++;
            retransmit = 0;
            filesize += size - OPC_SIZE - BLOCK_SIZE;
            if (size == m_blksize + OPC_SIZE + BLOCK_SIZE)
            {// 下载未结束
                if (m_opt.fields.find(QString("tsize")) != m_opt.fields.end())
                {
                    if ((double)filesize / m_tsize > percent)
                    {// 更新下载进度
                        percent = (double)filesize / m_tsize + 0.01;
                        emit send_process_value(filesize);
                    }
                }
            }
            else
            {// 下载结束
                return UP_DOWN_SUCCESS;
            }
        }
        else if (opc == OPC_ERROR)
        {// 收到Error包
            error_pkt(size);
            return UP_DOWN_FAIL;
        }
        else
        {// 超时
            retransmit++;
            sum_retransmit++;
            if (!data && (block == 1))
            {// 客户端不支持扩展选项且未收到过Data包
                if (!request_retransmit())
                {
                    return UP_DOWN_FAIL;
                }
            }
            else
            {// 客户端支持扩展选项或者客户端不支持扩展选项但已经收到过Data包
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
    exceed_retransmit(retransmit); // 超过最大重发次数限制处理
    return UP_DOWN_FAIL;
}

void Thread::error_pkt(int size)
{// Error包处理
    if (size < OPC_SIZE + ERRCODE_SIZE)
    {// Error包不完整
        m_message.type = TYPE::WRONG_PKT;
        m_message.info = QString("ERROR : Wrong error packet.");
        emit send_message(m_message);
    }
    else
    {// Error包完整
        m_message.type = TYPE::ERROR_PKT;
        m_message.info = QString("ERROR : errCode: %1 errMsg: %2").arg(ntohs(m_pkt_recv.error.errcode)).arg(m_pkt_recv.error.errmsg);
        emit send_message(m_message);
    }
}

bool Thread::exceed_retransmit(int retransmit)
{// 检查是否超过最大重发次数并处理
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
{// 更新选项值
    Opt opt = m_opt;
    // 更新tsize值
    if (opt.fields.find(QString("tsize")) != opt.fields.end())
    {
        m_tsize = opt.fields[QString("tsize")];
    }
    else
    {
        opt.fields[QString("tsize")] = m_tsize;
    }
    // 更新blksize值
    if (opt.fields.find(QString("blksize")) != opt.fields.end())
    {
        m_blksize = opt.fields[QString("blksize")];
    }
    else
    {
        opt.fields[QString("blksize")] = m_blksize;
    }
    // 更新timeout值
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
{// 处理接收到的OACK包
    char opt[MAX_REQUEST_SIZE] {}, val[MAX_REQUEST_SIZE] {};
    m_opt.fields.clear();
    for (int len = 0; len < size - 2; )
    {// 依次获取各个选项名及选项值
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
    emit send_message(m_message); // 显示所有选项内容
    update_opts(); // 更新已有选项值
    emit send_progress_max(m_tsize); // 更新进度条最大值
}

int Thread::ack(WORD block)
{// 发送ACK包
    m_pkt_send.opc = htons(OPC_ACK); // 编码转换
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
{// 获取数据包
    FD_SET socket;
    FD_ZERO(&socket);
    FD_SET(m_socket, &socket);
    TIMEVAL timeval = {timeout / 1000, timeout % 1000};
    // 在指定超时时间内接收一个数据包
    auto ret = select(m_socket + 1, &socket, nullptr, nullptr, &timeval);
    if (ret == SOCKET_ERROR)
    {// 套接字错误
        return PKT_SOCKET_ERROR;
    }
    else if (ret == 0)
    {// 超时
        return PKT_TIMEOUT;
    }
    else
    {// 成功接收
        size = _recv((char *)&m_pkt_recv, sizeof(PKT));
        // 根据接收数据包函数的返回值进行处理
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
    // 判断接收包是否完整
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
{// 获取指定操作码与指定序号的数据包
    int ret;
    for (bool flag = true; flag; )
    {
        auto start = clock();
        ret = pkt(timeout, size);
        timeout -= (clock() - start); // 更新剩余等待时间
        switch (type)
        {// 根据数据包操作码进行不同的处理
            case OPC_DATA:
            case OPC_ACK:
            // 获得指定序号的ACK/Data包或者Error包
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
            // 获得OACK包或者序号为1的Data包或者序号为0的ACK包或者Error包
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
    {// 等待超时处理
        m_message.type = TYPE::TIMEOUT;
        // 根据等待的数据包的类型显示不同的信息
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
    {// 返回成功接收的数据包的操作码
        opc = ntohs(m_pkt_recv.opc);
    }
    else
    {// 返回0(未成功接收数据包)
        opc = 0;
    }
}

int Thread::_open()
{// 打开本地文件
    if (fopen_s(&m_fp, m_client_file.toStdString().c_str(), (m_request_type == OPC_REQUEST_DOWN) ? "wb" : "rb") == 0)
    {// 打开本地文件成功
        m_message.type = TYPE::OPEN_FILE_SUCCESS;
        m_message.info = QString("Open file : Success : <%1>.").arg(m_client_file);
        emit send_message(m_message);
        return enum_integer(TYPE::OPEN_FILE_SUCCESS);
    }
    else
    {// 打开本地文件失败
        m_message.type = TYPE::OPEN_FILE_FAIL;
        m_message.info = QString("Open file : Fail : <%1>.").arg(m_client_file);
        emit send_message(m_message);
        return enum_integer(TYPE::OPEN_FILE_FAIL);
    }
}

int Thread::_send(const char* buf, int len)
{// 发送数据包
    int ret = sendto(m_socket, buf, len, 0, (SOCKADDR *)&m_server_addr, sizeof(m_server_addr));
    if (ret > 0)
    {
        bytes_up += ret; // 统计发送的字节数
    }
    return ret;
}

int Thread::_recv(char* buf, int len)
{// 接收数据包
    int ret = recvfrom(m_socket, buf, len, 0, (SOCKADDR *)&m_server_addr, &m_server_addr_len);
    if (ret > 0)
    {
        bytes_down += ret; // 统计接收的字节数
    }
    return ret;
}

void Thread::send_speed()
{// 更新上行与下行速度(实时吞吐量)
    emit send_speed_up(QString::number(((double)bytes_up - last_bytes_up) / REFRESH_INTERVAL * 1000 / 1024));
    emit send_speed_down(QString::number(((double)bytes_down - last_bytes_down) / REFRESH_INTERVAL * 1000 / 1024));
    last_bytes_down = bytes_down;
    last_bytes_up = bytes_up;
}

int Thread::sprintf_msg(char* buf, rsize_t bufferCount, int& offset, const char* format, ...)
{// 从buf起offset处开始打印格式化字符串
    va_list args;
    va_start(args, format);
    int ret = vsprintf_s(buf + offset, bufferCount - offset, format, args);
    va_end(args);
    offset += (ret + 1); // vsprintf_s函数返回值不包括终止的null字符
    return ret;
}

int Thread::sscanf_msg(char* buf, rsize_t bufferCount, char const* source, int& offset)
{// 从source起offset处读取字符串到buf中
    int ret = sprintf_s(buf, bufferCount, "%s", source + offset);
    offset += (ret + 1); // sprintf_s函数返回值不包括终止的null字符
    return ret;
}

int Thread::get_filesize(const char* fileName)
{// 获得文件大小
    struct stat statBuf;
    if (stat(fileName, &statBuf) != 0) { return -1; }
    return statBuf.st_size;
}

int Thread::local2netascii(const char *fileName)
{// 本地编码=>NETASCII编码
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
{// NETASCII编码=>本地编码
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
