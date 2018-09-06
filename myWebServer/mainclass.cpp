#include "mainclass.h"
#include "recvnetrequest.h"

MainClass::MainClass()
{
    m_pRecvNetRequest = new RecvNetRequest;
    m_pRecvNetRequest->startSvs();
}

MainClass::~MainClass()
{
    delete m_pRecvNetRequest;
}

int MainClass::readSysCfg()
{

    return 0;
}
