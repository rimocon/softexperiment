/*****************************************************************
�ե�����̾	: client_main.c
��ǽ		: ���饤����ȤΥᥤ��롼����
*****************************************************************/

#include<SDL/SDL.h>
#include"common.h"
#include"client_func.h"

static int NetworkEvent(void *data);

int main(int argc,char *argv[])
{
    int		num;
    char	name[MAX_CLIENTS][MAX_NAME_SIZE];
    int		endFlag=1;
    char	localHostName[]="localhost";
    char	*serverName;
    int		clientID;
	SDL_Thread	*thr;
	
    /* �����������å� */
    if(argc == 1){
    	serverName = localHostName;
    }
    else if(argc == 2){
    	serverName = argv[1];
    }
    else{
		fprintf(stderr, "Usage: %s, Cannot find a Server Name.\n", argv[0]);
		return -1;
    }

    /* �����С��Ȥ���³ */
    if(SetUpClient(serverName,&clientID,&num,name)==-1){
		fprintf(stderr,"setup failed : SetUpClient\n");
		return -1;
	}
    /* ������ɥ��ν���� */
	if(InitWindows(clientID,num,name)==-1){
		fprintf(stderr,"setup failed : InitWindows\n");
		return -1;
	}

    /* �ͥåȥ�����٥�Ƚ�������åɤκ��� */
	thr = SDL_CreateThread(NetworkEvent, &endFlag);
	
	/* �ᥤ�󥤥٥�ȥ롼�� */
    while(endFlag){
		WindowEvent(num);
    };

    /* ��λ���� */
	SDL_WaitThread(thr,NULL);
	DestroyWindow();
	CloseSoc();

    return 0;
}

/*****************************************************************
�ؿ�̾  : NetworkEvent
��ǽ    : �ͥåȥ���Υ��٥�ȥ롼��
����    : void		*data		: ��λȽ��ե饰
����    : ����åɽ�λ���ˣ����֤�
*****************************************************************/
static int NetworkEvent(void *data)
{
	int *endFlag;
	
	endFlag = (int*)data;
    while(*endFlag){
        *endFlag = SendRecvManager();
    }
	return 0;
}
