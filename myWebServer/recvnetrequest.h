#ifndef RECVNETREQUEST_H
#define RECVNETREQUEST_H

#include <string>
#include <stdio.h>
#include <list>
#include <atomic>
#include "soapH.h"

class RecvNetRequest
{
public:
    RecvNetRequest();
    ~RecvNetRequest();
    int startSvs();
    int enqueue(SOAP_SOCKET sock);
    SOAP_SOCKET dequeue();

    static void netRecvEntry(void* lpParam);
    static void netProcessEntry(void* lpParam);

    int                           m_nPort;
    struct soap           m_Fun_soap;
    SOAP_SOCKET  m_Master;

private:
#ifdef WIN32
    HANDLE    h_recvRequestThread;
    HANDLE    h_reqProcessThread[2];
    HANDLE    hEvent;
#else
    pthread_t   thread_recvRequest;
    pthread_t   thread_processQueue;
    sem_t          hsem;
#endif
    CRITICAL_SECTION          m_csQueue;
    std::list<SOAP_SOCKET> m_lstSoapSocket;
    struct soap                           *m_soap_thread;
    std::atomic_bool                  m_bQuit;
    bool                                          m_bWithSSL;
};

#endif // RECVNETREQUEST_H
