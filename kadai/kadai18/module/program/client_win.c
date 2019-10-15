/*****************************************************************
�ե�����̾	: client_win.c
��ǽ		: ���饤����ȤΥ桼�������󥿡��ե���������
*****************************************************************/

#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<SDL/SDL_gfxPrimitives.h>
#include"common.h"
#include"client_func.h"

static SDL_Surface *gMainWindow;
static SDL_Rect gButtonRect[MAX_CLIENTS+2];

static int CheckButtonNO(int x,int y,int num);

/*****************************************************************
�ؿ�̾	: InitWindows
��ǽ	: �ᥤ�󥦥���ɥ���ɽ���������Ԥ�
����	: int	clientID		: ���饤������ֹ�
		  int	num				: �����饤����ȿ�
����	: ���������Ǥ����Ȥ�0�����Ԥ����Ȥ�-1
*****************************************************************/
int InitWindows(int clientID,int num,char name[][MAX_NAME_SIZE])
{
	int i;
	SDL_Surface *image;
	char clientButton[4][6]={"0.jpg","1.jpg","2.jpg","3.jpg"};
	char endButton[]="END.jpg";
	char allButton[]="ALL.jpg";
	char *s,title[10];

    /* �����������å� */
    assert(0<num && num<=MAX_CLIENTS);
	
	/* SDL�ν���� */
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("failed to initialize SDL.\n");
		return -1;
	}
	
	/* �ᥤ��Υ�����ɥ���������� */
	if((gMainWindow = SDL_SetVideoMode(500,300, 32, SDL_SWSURFACE)) == NULL) {
		printf("failed to initialize videomode.\n");
		return -1;
	}
	/* ������ɥ��Υ����ȥ�򥻥å� */
	sprintf(title,"%d",clientID);
	SDL_WM_SetCaption(title,NULL);
	
	/* �طʤ���ˤ��� */
	SDL_FillRect(gMainWindow,NULL,0xffffff);

	/* �ܥ���κ��� */
	for(i=0;i<num+2;i++){
		gButtonRect[i].x = 20+80*i;
		gButtonRect[i].y=10;
		gButtonRect[i].w=70;
		gButtonRect[i].h=20;
      
		if(i==num){
			s=allButton;
		}
		else if(i==num+1){
			s=endButton;
		}
		else{
			s=clientButton[i];
		}
		image = IMG_Load(s);
		SDL_BlitSurface(image,NULL,gMainWindow,&(gButtonRect[i]));
		SDL_FreeSurface(image);
	}
	SDL_Flip(gMainWindow);
	
	return 0;
}

/*****************************************************************
�ؿ�̾	: DestroyWindow
��ǽ	: SDL��λ����
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
void DestroyWindow(void)
{
	SDL_Quit();
}

/*****************************************************************
�ؿ�̾	: WindowEvent
��ǽ	: �ᥤ�󥦥���ɥ����Ф��륤�٥�Ƚ�����Ԥ�
����	: int		num		: �����饤����ȿ�
����	: �ʤ�
*****************************************************************/
void WindowEvent(int num)
{
	SDL_Event event;
	SDL_MouseButtonEvent *mouse;
	int buttonNO;

    /* �����������å� */
    assert(0<num && num<=MAX_CLIENTS);

	if(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_QUIT:
				SendEndCommand();
				break;
			case SDL_MOUSEBUTTONUP:
				mouse = (SDL_MouseButtonEvent*)&event;
				if(mouse->button == SDL_BUTTON_LEFT){
					buttonNO = CheckButtonNO(mouse->x,mouse->y,num);
#ifndef NDEBUG
					printf("#####\n");
					printf("WindowEvent()\n");
					printf("Button %d is pressed\n",buttonNO);
#endif
					if(0<=buttonNO && buttonNO<num){
						/* ̾���ν񤫤줿�ܥ��󤬲����줿 */
						SendCircleCommand(buttonNO);
					}
					else if(buttonNO==num){
						/* ��All�פȽ񤫤줿�ܥ��󤬲����줿 */
						SendRectangleCommand();
					}
					else if(buttonNO==num+1){
						/* ��End�פȽ񤫤줿�ܥ��󤬲����줿 */
						SendEndCommand();
					}
				}
				break;
		}
	}
}

/*****************************************************************
�ؿ�̾	: DrawRectangle
��ǽ	: �ᥤ�󥦥���ɥ��˻ͳѤ�ɽ������
����	: int		x			: �ͳѤκ���� x ��ɸ
		  int		y			: �ͳѤκ���� y ��ɸ
		  int		width		: �ͳѤβ���
		  int		height		: �ͳѤι⤵
����	: �ʤ�
*****************************************************************/
void DrawRectangle(int x,int y,int width,int height)
{
#ifndef NDEBUG
    printf("#####\n");
    printf("DrawRectangle()\n");
    printf("x=%d,y=%d,width=%d,height=%d\n",x,y,width,height);
#endif

	rectangleColor(gMainWindow,x,y,x+width,y+height,0x000000ff);
	SDL_Flip(gMainWindow);
}

/*****************************************************************
�ؿ�̾	: DrawCircle
��ǽ	: �ᥤ�󥦥���ɥ��˱ߤ�ɽ������
����	: int		x		: �ߤ� x ��ɸ
		  int		y		: �ߤ� y ��ɸ
		  int		r		: �ߤ�Ⱦ��
����	: �ʤ�
*****************************************************************/
void DrawCircle(int x,int y,int r)
{
#ifndef NDEBUG
	printf("#####\n");
    printf("DrawCircle()\n");
    printf("x=%d,y=%d,tyokkei=%d\n",x,y,r);
#endif

    circleColor(gMainWindow,x,y,r,0x000000ff);
	SDL_Flip(gMainWindow);
}

/*****************************************************************
�ؿ�̾	: DrawDiamond
��ǽ	: �ᥤ�󥦥���ɥ���ɩ����ɽ������
����	: int		x		: ����� x ��ɸ
		  int		y		: ����� y ��ɸ
		  int		height		: �⤵
����	: �ʤ�
*****************************************************************/
void DrawDiamond(int x,int y,int height)
{
	Sint16	vx[5],vy[5];
	int	i;

#ifndef NDEBUG
    printf("#####\n");
    printf("DrawDiamond()\n");
    printf("x=%d,y=%d,height=%d\n",x,y,height);
#endif

    for(i=0;i<4;i++){
        vx[i] = x + height*((1-i)%2)/2;
        vy[i] = y + height*((2-i)%2);
    }
    vx[4]=vx[0];
    vy[4]=vy[0];
	
	polygonColor(gMainWindow, vx, vy, 5 , 0x000000ff);
	SDL_Flip(gMainWindow);
}

/*****
static
*****/
/*****************************************************************
�ؿ�̾	: CheckButtonNO
��ǽ	: ����å����줿�ܥ�����ֹ���֤�
����	: int	   x		: �ޥ����β����줿 x ��ɸ
		  int	   y		: �ޥ����β����줿 y ��ɸ
		  char	   num		: �����饤����ȿ�
����	: �����줿�ܥ�����ֹ���֤�
		  �ܥ��󤬲�����Ƥ��ʤ�����-1���֤�
*****************************************************************/
static int CheckButtonNO(int x,int y,int num)
{
	int i;

 	for(i=0;i<num+2;i++){
		if(gButtonRect[i].x < x &&
			gButtonRect[i].y < y &&
      		gButtonRect[i].x + gButtonRect[i].w > x &&
			gButtonRect[i].y + gButtonRect[i].h > y){
			return i;
		}
	}
 	return -1;
}
