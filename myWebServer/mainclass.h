#ifndef MAINCLASS_H
#define MAINCLASS_H

class MainClass
{
public:
    MainClass();
    ~MainClass();
    int readSysCfg();
    RecvRequest *m_pRecvRequest;
private:
    int m_nWebListenPort;
    int m_nTcpListenPort;
    int m_nWebMaxThreadNum;
    int m_nTcpMaxThreadNum;
    int m_nWebReplyClientDalay;
    int m_nTaskWaitAnsTime;
    int m_nRecvRequestType;
    int m_nThreshold;
    int m_nMinHeight;
    int m_nMinWidth;
    int m_nTaskServerPort;
    int m_nFeatureServerPort;
    std::string m_strTaskServerIP;
    std::string m_strFeatureServerIP;
    std::string m_strSaveFileDir;
    std::string m_strSaveCmpFileDir;
};

#endif // MAINCLASS_H
