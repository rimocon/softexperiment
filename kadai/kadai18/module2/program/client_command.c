/*****************************************************************
�ե�����̾	: client_command.c
��ǽ		: ���饤����ȤΥ��ޥ�ɽ���
*****************************************************************/

#include"common.h"
#include"client_func.h"

static void SetIntData2DataBlock(void *data,int intData,int *dataSize);
static void SetCharData2DataBlock(void *data,char charData,int *dataSize);
static void RecvCircleData(void);
static void RecvRectangleData(void);
static void RecvDiamondData(void);

/*****************************************************************
�ؿ�̾	: ExecuteCommand
��ǽ	: �����С����������Ƥ������ޥ�ɤ򸵤ˡ�
		  ����������������¹Ԥ���
����	: char	command		: ���ޥ��
����	: �ץ���ཪλ���ޥ�ɤ��������Ƥ������ˤ�0���֤���
		  ����ʳ���1���֤�
*****************************************************************/
int ExecuteCommand(char command)
{
    int	endFlag = 1;
#ifndef NDEBUG
    printf("#####\n");
    printf("ExecuteCommand()\n");
    printf("command = %c\n",command);
#endif
    switch(command){
		case END_COMMAND:
			endFlag = 0;
			break;
	    case CIRCLE_COMMAND:
			RecvCircleData();
			break;
		case RECT_COMMAND:
			RecvRectangleData();
			break;
		case DIAMOND_COMMAND:
			RecvDiamondData();
			break;
    }
    return endFlag;
}

/*****************************************************************
�ؿ�̾	: SendRectangleCommand
��ǽ	: ���饤����Ȥ˻ͳѤ�ɽ�������뤿��ˡ�
		  �����С��˥ǡ���������
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
void SendRectangleCommand(void)
{   
    unsigned char	data[MAX_DATA];
    int			dataSize;

#ifndef NDEBUG
    printf("#####\n");
    printf("SendRectangleCommand()\n");
#endif
    dataSize = 0;
    /* ���ޥ�ɤΥ��å� */
    SetCharData2DataBlock(data,RECT_COMMAND,&dataSize);

    /* �ǡ��������� */
    SendData(data,dataSize);
}

/*****************************************************************
�ؿ�̾	: SendCircleCommand
��ǽ	: ���饤����Ȥ˱ߤ�ɽ�������뤿��ˡ�
		  �����С��˥ǡ���������
����	: int		pos	    : �ߤ�ɽ�������륯�饤������ֹ�
����	: �ʤ�
*****************************************************************/
void SendCircleCommand(int pos)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    /* �����������å� */
    assert(0<=pos && pos<MAX_CLIENTS);

#ifndef NDEBUG
    printf("#####\n");
    printf("SendCircleCommand()\n");
    printf("Send Circle Command to %d\n",pos);
#endif

    dataSize = 0;
    /* ���ޥ�ɤΥ��å� */
    SetCharData2DataBlock(data,CIRCLE_COMMAND,&dataSize);
    /* ���饤������ֹ�Υ��å� */
    SetIntData2DataBlock(data,pos,&dataSize);

    /* �ǡ��������� */
    SendData(data,dataSize);
}

/*****************************************************************
�ؿ�̾	: SendEndCommand
��ǽ	: �ץ����ν�λ���Τ餻�뤿��ˡ�
		  �����С��˥ǡ���������
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
void SendEndCommand(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

#ifndef NDEBUG
    printf("#####\n");
    printf("SendEndCommand()\n");
#endif
    dataSize = 0;
    /* ���ޥ�ɤΥ��å� */
    SetCharData2DataBlock(data,END_COMMAND,&dataSize);

    /* �ǡ��������� */
    SendData(data,dataSize);
}

/*****
static
*****/
/*****************************************************************
�ؿ�̾	: SetIntData2DataBlock
��ǽ	: int ���Υǡ����������ѥǡ����κǸ�˥��åȤ���
����	: void		*data		: �����ѥǡ���
		  int		intData		: ���åȤ���ǡ���
		  int		*dataSize	: �����ѥǡ����θ��ߤΥ�����
����	: �ʤ�
*****************************************************************/
static void SetIntData2DataBlock(void *data,int intData,int *dataSize)
{
    int tmp;

    /* �����������å� */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    tmp = htonl(intData);

    /* int ���Υǡ����������ѥǡ����κǸ�˥��ԡ����� */
    memcpy(data + (*dataSize),&tmp,sizeof(int));
    /* �ǡ��������������䤹 */
    (*dataSize) += sizeof(int);
}

/*****************************************************************
�ؿ�̾	: SetCharData2DataBlock
��ǽ	: char ���Υǡ����������ѥǡ����κǸ�˥��åȤ���
����	: void		*data		: �����ѥǡ���
		  int		intData		: ���åȤ���ǡ���
		  int		*dataSize	: �����ѥǡ����θ��ߤΥ�����
����	: �ʤ�
*****************************************************************/
static void SetCharData2DataBlock(void *data,char charData,int *dataSize)
{
    /* �����������å� */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    /* char ���Υǡ����������ѥǡ����κǸ�˥��ԡ����� */
    *(char *)(data + (*dataSize)) = charData;
    /* �ǡ��������������䤹 */
    (*dataSize) += sizeof(char);
}

/*****************************************************************
�ؿ�̾	: RecvCircleData
��ǽ	: �ߤ�ɽ�����뤿��Υǡ������������ɽ������
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
static void RecvCircleData(void)
{
    int	x,y,r;

    /* �ߥ��ޥ�ɤ��Ф����������������� */
    RecvIntData(&x);
    RecvIntData(&y);
    RecvIntData(&r);

    /* �ߤ�ɽ������ */
    DrawCircle(x,y,r);
}

/*****************************************************************
�ؿ�̾	: RecvRectangleData
��ǽ	: �ͳѤ�ɽ�����뤿��Υǡ������������ɽ������
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
static void RecvRectangleData(void)
{
    int	x,y,width,height;

    /* �ͳѥ��ޥ�ɤ��Ф����������������� */
    RecvIntData(&x);
    RecvIntData(&y);
    RecvIntData(&width);
    RecvIntData(&height);

    /* �ͳѤ�ɽ������ */
    DrawRectangle(x,y,width,height);
}

/*****************************************************************
�ؿ�̾	: RecviDiamondData
��ǽ	: ɩ����ɽ�����뤿��Υǡ������������ɽ������
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
static void RecvDiamondData(void)
{
    int	x,y,height;

    /* ɩ�����ޥ�ɤ��Ф����������������� */
    RecvIntData(&x);
    RecvIntData(&y);
    RecvIntData(&height);

    /* ɩ����ɽ������ */
    DrawDiamond(x,y,height);
}
