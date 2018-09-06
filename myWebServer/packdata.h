#ifndef PACKDATA_H
#define PACKDATA_H

//
typedef struct _THREAD_INFO{
    void*   pThis;
    int        nIndex;
}THREAD_INFO;

struct STU_TEMP{
int       nIndex;
int       nBufLen;
char *pBuf;
};

#endif // PACKDATA_H
