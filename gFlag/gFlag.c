/*******************************************************************************

					            APP�����־ģ��ʵ��

********************************************************************************/
#include  "gFlag.h"
#include  <String.h>

struct _gFlag gFlag;

static const struct _gFlag _Default = {
  GFLAG_BIT_DEFAULT,
  GFLAG_BYTE_DEFAULT,
};

/****************************************************************************
                             ��غ���ʵ��
****************************************************************************/
//--------------------------------��ʼ������------------------------------
void gFlag_Init(void)
{
  memcpy(&gFlag, &_Default , sizeof(struct _gFlag));
}