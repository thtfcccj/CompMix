/***************************************************************************

           资源文件型文本-转换为txt文件函数
                         
****************************************************************************/
#ifndef __RP_TXT_2_FILE_H
#define __RP_TXT_2_FILE_H

//-------------------------------转换为文本流-------------------------------
//此函数将行文本输出至文本流中，每行以回车+换行结束
//返回插入的文本流长度,(非0时将通过pStartLine返回结束行位置以供下次使用)
unsigned long RP_Txt2File(unsigned char Rid,
                           unsigned short *pStartLine,//起始行,返回结束行
                           unsigned short CountLine,//行长度,0所有
                           char *pTxtStream,//文本流
                           unsigned long StreamLen);//文本流长度


#endif
