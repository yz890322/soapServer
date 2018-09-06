#include <process.h>
#include <chrono>
#include "recvnetrequest.h"
#include "stdsoap2.h"
#include "packdata.h"
#include "mySoapServer.nsmap"


int http_get(struct soap* soap)
{
    FILE* fd=NULL;
    fd = fopen("faceRecon.wsdl","rb");
    if(!fd)
    {
        return 404;
    }
    soap->http_content="text/xml";
    soap_response(soap,SOAP_FILE);
    for(;;)
    {
        size_t r=fread(soap->tmpbuf,1,sizeof(soap->tmpbuf),fd);
        if(!r)
        {
            break;
        }
        if(soap_send_raw(soap,soap->tmpbuf,r))
        {
            break;
        }
    }
    fclose(fd);
    soap_end_send(soap);
    return SOAP_OK;
}

RecvNetRequest::RecvNetRequest()
{
    m_bQuit = false;
    m_nPort = 8181;

    InitializeCriticalSection(&m_csQueue);
    hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
}

RecvNetRequest::~RecvNetRequest()
{
    CloseHandle(hEvent);
    DeleteCriticalSection(&m_csQueue);
}

int RecvNetRequest::startSvs()
{
    soap_init(&m_Fun_soap);
    soap_set_mode(&m_Fun_soap, SOAP_C_UTFSTRING);
    m_Fun_soap.fget = http_get;
    m_Fun_soap.send_timeout = 1000;
    m_Master = soap_bind(&m_Fun_soap,NULL,m_nPort,500);

    if(!(soap_valid_socket(m_Master)))
    {
        soap_print_fault(&m_Fun_soap,stderr);
        exit(1);
    }
    printf("WebService listen port : %d\n",m_nPort);
    printf("Webserver socket created successful: master socket = %d\n",m_Master);

    //网络请求接收线程
    h_recvRequestThread = (HANDLE)_beginthread(netRecvEntry, 0, this);

    //网络请求处理线程
    THREAD_INFO temp;
    temp.pThis = this;
    for(int i = 0; i < 2; i++)
    {
        temp.nIndex = i;
        h_reqProcessThread[i] = (HANDLE)_beginthread(netProcessEntry, 0, &temp);
        Sleep(50);
    }

    return 0;
}

void RecvNetRequest::netRecvEntry(void* lpParam)
{
    RecvNetRequest *pThis = (RecvNetRequest*)lpParam;

    printf("Receive request thread.\n");
    while(!pThis->m_bQuit)
    {
        SOAP_SOCKET nSlave = soap_accept(&pThis->m_Fun_soap);
        if(!soap_valid_socket(nSlave))
        {
            if(pThis->m_Fun_soap.errnum)
            {
                soap_print_fault(&pThis->m_Fun_soap,stderr);
                continue;
            }
            else
            {
                printf("Server timed out ");
                break;
            }
        }
        printf("Accepted connection from IP : %d.%d.%d.%d socket = %d\n",(pThis->m_Fun_soap.ip>>24)&0xFF,(pThis->m_Fun_soap.ip>>16)&0xFF,
                (pThis->m_Fun_soap.ip>>8)&0xFF,(pThis->m_Fun_soap.ip)&0xFF,(pThis->m_Fun_soap.socket));
        pThis->enqueue(nSlave);
    }
    return;
}

void  RecvNetRequest::netProcessEntry(void* lpParam)
{
    THREAD_INFO *tempInfo = (THREAD_INFO*)lpParam;
    RecvNetRequest *pThis = (RecvNetRequest*)tempInfo->pThis;
    int index  = tempInfo->nIndex;

    struct soap *mysoap = soap_copy(&pThis->m_Fun_soap);
    mysoap->user = (void*)(new STU_TEMP);
    ((STU_TEMP*)mysoap->user)->nIndex = index;
    ((STU_TEMP*)mysoap->user)->nBufLen = 10*1024*1024;
    ((STU_TEMP*)mysoap->user)->pBuf = new char[10*1024*1024];

    printf("Process request thread %d\n", ((STU_TEMP*)mysoap->user)->nIndex);
    while(!pThis->m_bQuit)
    {
        mysoap->socket = pThis->dequeue();
        if(!soap_valid_socket(mysoap->socket))
        {
            continue;
        }
        if(soap_serve(mysoap)!=SOAP_OK)
        {
            soap_print_fault(mysoap, stderr);
            fprintf(stderr,"request served\n");
        }
        soap_destroy(mysoap);
        soap_end(mysoap);
    }

    delete[] ((STU_TEMP*)mysoap->user)->pBuf;
    ((STU_TEMP*)mysoap->user)->pBuf = NULL;
    delete mysoap->user;
    mysoap->user = NULL;
    return;
}

int RecvNetRequest::enqueue(SOAP_SOCKET sock)
{
    if(soap_valid_socket(sock))
    {
        EnterCriticalSection(&m_csQueue);
        m_lstSoapSocket.push_back(sock);
        LeaveCriticalSection(&m_csQueue);
        SetEvent(hEvent);
    }
    return 0;
}

SOAP_SOCKET RecvNetRequest::dequeue()
{
    SOAP_SOCKET sock = -1;
    if(m_lstSoapSocket.empty())
    {
        ResetEvent(hEvent);
        WaitForSingleObject(hEvent,INFINITE);
    }else
    {
        EnterCriticalSection(&m_csQueue);
        sock = *m_lstSoapSocket.begin();
        m_lstSoapSocket.pop_front();
        LeaveCriticalSection(&m_csQueue);
    }
    return sock;
}

int ns__FaceDetect(struct soap *soap,std::string inXmlData,std::string &result)
{
    std::chrono::system_clock::time_point t1= std::chrono::system_clock::now();
    Sleep(500);
    std::chrono::system_clock::time_point t2= std::chrono::system_clock::now();
    printf("------FaceDetect:%d ms------\n",std::chrono::duration_cast<std::chrono::milliseconds> (t2-t1).count());
    return SOAP_OK;
}
