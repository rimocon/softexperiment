#include "headers.h"

int main(int argc, char *argv[])
{
startUp();
while(quits()!=0)
	{
	backGraund();	//�o�b�N�n
	inputEngine();//�L�[����
	switch(games)
		{
		case -1://�E�B���h�E���[�h�I��
			bos_test();
			winMode();
			break;
			
		case 0://�^�C�g�����
			dr_Title(100,100);
			startGame();//�Q�[����
			break;
		
		case 1://�Q�[��
			chara();
			break;
		
		case 2://�Q�[���I�[�o�[
			dr_end(100,100);
			Mix_HaltMusic();//BGM�~�߂�
			backTitle();//�^�C�g����ʂ�
			break;
		
		default: break;
		}
	score_main();//���_
	frames();
	}
SDL_Quit();
return 0;
}