 /***********************************************************************************
  * file   : virtualosc.c
  * brief  : draw points on the virtual OSC
  * author : zx
  * Data   : 2020/5/4-23:17
  * Ver    :
  *             -Ver.1.0 first add 
  * Note   : Developers don't need to care about implementation details, just learn how to use these the function "updateVirtualOSC()"
  * BBS    : http://www.openelec.cn/
  * 
  * Copyright (C) 2020-2021 zx. All rights reserved.
  ***********************************************************************************/ 
#include "virtualosc.h"
#include "usart.h"
/**
 * brief : sendbytes
 * parameter:
 *      events       : the result save to 'events'
 *      bytes        : bytes that need send
 *      len          : lenth of bytes
 * ret: none
 */
static void sendBytes(char * data, unsigned short datalen)
{
    /*you need input a function that send bytes with uart&TCP/IP*/
	sendUartBytes(data, datalen);
}

/**
 * brief    : draw one or more points on the virtual OSC
 * parameter:
 *      data        : buffer of new points
 *      datalen     : must be an integral multiple of sizeof(float)
 * ret      : none
 * 
 */
void updateVirtualOSC(char * data, unsigned short datalen)
{
    if(datalen<4 || datalen%4!=0)
    {
        return ;
    }
    sendBytes(data, datalen);
}
