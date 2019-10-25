/*****************************************************************
�ե�����̾	: server_command.c
��ǽ		: �����С��Υ��ޥ�ɽ���
*****************************************************************/

#include"server_common.h"
#include"server_func.h"

static void SetIntData2DataBlock(void *data,int intData,int *dataSize);
static void SetCharData2DataBlock(void *data,char charData,int *dataSize);
static int GetRandomInt(int n);
static void judge(int command0,int command1);
static void waitclient(int pos,int command);
/*****************************************************************
�ؿ�̾	: ExecuteCommand
��ǽ	: ���饤����Ȥ��������Ƥ������ޥ�ɤ򸵤ˡ�
		  ����������������¹Ԥ���
����	: char	command		: ���ޥ��
		  int	pos			: ���ޥ�ɤ����ä����饤������ֹ�
����	: �ץ���ཪλ���ޥ�ɤ������Ƥ������ˤ�0���֤���
		  ����ʳ���1���֤�
*****************************************************************/
int ExecuteCommand(char command,int pos)
{
    unsigned char	data[MAX_DATA];
    int			dataSize,intData;
    int			endFlag = 1;

    /* �����������å� */
    assert(0<=pos && pos<MAX_CLIENTS);

#ifndef NDEBUG
    printf("#####\n");
    printf("ExecuteCommand()\n");
    printf("Get command %c\n",command);
#endif
    switch(command){
	    case END_COMMAND:
			dataSize = 0;
			/* ���ޥ�ɤΥ��å� */
			SetCharData2DataBlock(data,command,&dataSize);

			/* ���桼���������� */
			SendData(ALL_CLIENTS,data,dataSize);

			endFlag = 0;
			break;
	    case CIRCLE_COMMAND:
			/* �ߤ�ɽ�����륯�饤������ֹ��������� */
			RecvIntData(pos,&intData);

			dataSize = 0; //���㥹��
			/* ���ޥ�ɤΥ��å� */
			SetCharData2DataBlock(data,command,&dataSize);
			/* ����� x ��ɸ�Υ��å� */
			SetIntData2DataBlock(data,GetRandomInt(500),&dataSize);
			/* ����� y ��ɸ�Υ��å� */
			SetIntData2DataBlock(data,GetRandomInt(300),&dataSize);
			/* ľ�¤Υ��å� */
			SetIntData2DataBlock(data,GetRandomInt(100),&dataSize);

			/* ���ꤵ�줿���饤����Ȥ����� */
			SendData(intData,data,dataSize);
			break;
	    case ROCK_COMMAND: //�����ξ��
      ////������Ƚ���Ѵؿ�
        waitclient(pos,0); //¾�Υ��饤����Ȥ��Ԥ�
      break;
      case SCISSORS_COMMAND: //���祭�ξ��
        waitclient(pos,1); //¾�Υ��饤����Ȥ��Ԥ�
      break;
      case PAPER_COMMAND: //�ѡ��ξ��
        waitclient(pos,2);
      break;

	    default:
			/* ̤�ΤΥ��ޥ�ɤ������Ƥ��� */
			fprintf(stderr,"0x%02x is not command!\n",command);
    }
    return endFlag;
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

    /* int ���Υǡ����������ѥǡ����κǸ�˥��ԡ����� */
    *(char *)(data + (*dataSize)) = charData;
    /* �ǡ��������������䤹 */
    (*dataSize) += sizeof(char);
}

/*****************************************************************
�ؿ�̾	: GetRandomInt
��ǽ	: ���������������
����	: int		n	: ����κ�����
����	: �����
*****************************************************************/
static int GetRandomInt(int n)
{
    return rand()%n;
}

static void waitclient(int pos,int command)
{
  int flag0;
  int flag1;
  int command0;
  int command1;

  if(pos == 0) { //���饤�����0�������å�
    flag0 = 1;
    command0 = command;
    fprintf(stderr,"client0 set\n");
  }
  else if(pos == 1) { //���饤�����1�������å�
    flag1 = 1;
    command1 = command;
    fprintf(stderr,"client1 set\n");
  }
  if(flag0 == 1 && flag1 == 1) { //�ɤä�����������Ƥ�����Ƚ��
    
    fprintf(stderr,"judge start\n");
    judge(command0,command1);
    flag0 = 0;
    flag1 = 0;
  }
}
static void judge(int command0,int command1){
  int result;
  unsigned char	data1[MAX_DATA];
  unsigned char	data2[MAX_DATA];
	int dataSize = 0; //���㥹��
  result = (command0 - command1 +3) % 3; 
  switch(result){
  case 2: //1����2�餱
    SetCharData2DataBlock(data1,WIN,&dataSize);
	  SendData(0,data1,dataSize);
    SetCharData2DataBlock(data2,LOSE,&dataSize);
    SendData(1,data2,dataSize);
    fprintf(stderr,"1win2lose\n");
    break;
  case 1: //1�餱2����
    SetCharData2DataBlock(data1,LOSE,&dataSize);
	  SendData(0,data1,dataSize);
    SetCharData2DataBlock(data2,WIN,&dataSize);
    SendData(1,data2,dataSize);
    fprintf(stderr,"1lose2win\n");
    break;
  case 0://������
    SetCharData2DataBlock(data1,DRAW,&dataSize);
    SendData(ALL_CLIENTS,data1,dataSize);
    fprintf(stderr,"draw\n");
    break;
  }
  fprintf(stderr,"data1= %s \n",data1);
  fprintf(stderr,"data2= %s \n",data2);
}
