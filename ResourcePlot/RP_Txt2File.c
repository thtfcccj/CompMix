/***************************************************************************

           资源文件型文本文件-在使用TMenuBuf时的实现
                         
****************************************************************************/

#include "RP_Txt2File.h"

#include "Rid.h"
#include <string.h>
  
//-------------------------------转换为文本流--------------------------------
//此函数将行文本输出至文本流中，每行以回车+换行结束
//返回插入的文本流长度,(非0时将通过pStartLine返回结束行位置以供下次使用)
unsigned long RP_Txt2File(unsigned char Rid,
                           unsigned short *pStartLine,//起始行,返回结束行
                           unsigned short CountLine,//行长度,0所有
                           char *pTxtStream,//文本流
                           unsigned long StreamLen)//文本流长度
{
  const unsigned char *pBase = RID_pGetData(Rid);
  if(pBase == RI_NULL) return 0;//异常
  unsigned short EndLine = RID_GetIdCount(pBase);
  if(EndLine == 0) return 0;//无字符异常
  unsigned short StartLine = *pStartLine;
  if(StartLine >= EndLine) return 0;//起始异常
  if(CountLine > 0){//指定了结束位置时
    CountLine += StartLine; //绝对结束位置了
    //if(CountLine > EndLine) EndLine = EndLine; //不能超结束位置
    if(CountLine < EndLine) EndLine = CountLine;//没到结束位置修正
  }
  //开始转换
  unsigned long FinalLen = 0; //已写入字符长度
  for(; StartLine < EndLine; StartLine++){
    const char *pCurStr = (const char *)RI_pGetData(pBase, StartLine);
    //检查文本流是否写满
    unsigned long CurLen = strlen(pCurStr);
    unsigned long NextFinalLen = FinalLen + CurLen + 2;//(2:回车换行)
    if(NextFinalLen > StreamLen) break;//超了
    //插入文本流尾部
    FinalLen = NextFinalLen; //下一行起始
    memcpy(pTxtStream, pCurStr, CurLen);
    pTxtStream += CurLen;
    *pTxtStream++ = 0x0D; //回车符
    *pTxtStream++ = 0x0A; //换行符
  }
  *pStartLine = StartLine; //结束位置
  return FinalLen;
}




