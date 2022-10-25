/***************************************************************************

           ��Դ�ļ����ı��ļ�-��ʹ��TMenuBufʱ��ʵ��
                         
****************************************************************************/

#include "RP_Txt2File.h"

#include "Rid.h"
#include <string.h>
  
//-------------------------------ת��Ϊ�ı���--------------------------------
//�˺��������ı�������ı����У�ÿ���Իس�+���н���
//���ز�����ı�������,(��0ʱ��ͨ��pStartLine���ؽ�����λ���Թ��´�ʹ��)
unsigned long RP_Txt2File(unsigned char Rid,
                           unsigned short *pStartLine,//��ʼ��,���ؽ�����
                           unsigned short CountLine,//�г���,0����
                           char *pTxtStream,//�ı���
                           unsigned long StreamLen)//�ı�������
{
  const unsigned char *pBase = RID_pGetData(Rid);
  if(pBase == RI_NULL) return 0;//�쳣
  unsigned short EndLine = RID_GetIdCount(pBase);
  if(EndLine == 0) return 0;//���ַ��쳣
  unsigned short StartLine = *pStartLine;
  if(StartLine >= EndLine) return 0;//��ʼ�쳣
  if(CountLine > 0){//ָ���˽���λ��ʱ
    CountLine += StartLine; //���Խ���λ����
    //if(CountLine > EndLine) EndLine = EndLine; //���ܳ�����λ��
    if(CountLine < EndLine) EndLine = CountLine;//û������λ������
  }
  //��ʼת��
  unsigned long FinalLen = 0; //��д���ַ�����
  for(; StartLine < EndLine; StartLine++){
    const char *pCurStr = (const char *)RI_pGetData(pBase, StartLine);
    //����ı����Ƿ�д��
    unsigned long CurLen = strlen(pCurStr);
    unsigned long NextFinalLen = FinalLen + CurLen + 2;//(2:�س�����)
    if(NextFinalLen > StreamLen) break;//����
    //�����ı���β��
    FinalLen = NextFinalLen; //��һ����ʼ
    memcpy(pTxtStream, pCurStr, CurLen);
    pTxtStream += CurLen;
    *pTxtStream++ = 0x0D; //�س���
    *pTxtStream++ = 0x0A; //���з�
  }
  *pStartLine = StartLine; //����λ��
  return FinalLen;
}




