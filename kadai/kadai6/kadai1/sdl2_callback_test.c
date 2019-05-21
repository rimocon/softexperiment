/*************************************************
sdl2_callback_test.c
Programmed by H. Mitsuhara (May, 2019)

�R���p�C���I�v�V�����i�����N�Ώہj�F-lSDL2
����
�@�E�B���h�E���Ń}�E�X�𓮂����ƁC�^�[�~�i���ɍ��W�\��
�@5�b���ƂɁC10�b�ҋ@����R�[���o�b�N���Ăяo�����
�@Enter�L�[�������ƁC5�b�ҋ@
�@Esc�L�[�������ƏI��
���l�F
�@�R�[���o�b�N�֐������C���X���b�h�Ƃ͈قȂ�X���b�h�ł��邱�Ƃ��m�F���邽�߂̃v���O����
*************************************************/

#include <stdio.h>
#include <SDL2/SDL.h> // SDL��p���邽�߂ɕK�v�ȃw�b�_�t�@�C�����C���N���[�h

// �^�C�}�[�ŌĂяo�����R�[���o�b�N�֐��i���C���X���b�h�ƈقȂ�X���b�h�ɂȂ�j
Uint32 TestCallback(Uint32 interval, void *param){
    printf("10-sec Sleep in Callback Func.\n");
    SDL_Delay(10000); // 10�b�ҋ@
    // ���C���X���b�h�Ƃ͈قȂ�X���b�h�ł̑ҋ@�Ȃ̂ŁC���C���X���b�h�̏����͌p��
    // �i�}�E�X�𓮂����ƁC���W�͕\�������j

    return interval; // �R�[���o�b�N���Ăяo�����Ԋu��Ԃ��i�K�{�j
}

// ���̊֐��̓��C���X���b�h���̊֐��i���C���X���b�h�Ɠ���j
void Sleep5(){
    printf("5-sec Sleep in Main Thread.\n");
    SDL_Delay(5000); // ������5�b�ҋ@
    // ���C���X���b�h�̗��ꂪ�����Ŏ~�܂�̂ŁC�}�E�X���W�͕\������Ȃ�
}

// ���C���֐�
int main(int argc, char* argv[]) {
    SDL_Window* window; // �E�B���h�E�f�[�^
    SDL_Event event; // �C�x���g�f�[�^���i�[����\����
    SDL_TimerID timer_id; // �^�C�}���荞�݂��s�����߂̃^�C�}��ID

    // SDL������
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("failed to initialize SDL.\n");
        exit(-1);
    }

    // �E�B���h�E�����E�\��
    window = SDL_CreateWindow("Callback Func. Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 120, 120, 0);

    // �^�C�}�[�쐬
    timer_id=SDL_AddTimer(5000, TestCallback, NULL); // 5�b���ƂɃR�[���o�b�N�֐����Ăяo��
    // �^�C�}�[�ݒ�ł�5�b���ƂɃR�[���o�b�N�֐����Ăяo����邪�C�R�[���o�b�N�֐�����10�b�ҋ@�����D
    // ���ʓI��10�b��ɃR�[���o�b�N�֐����Ăяo����邱�ƂƓ����ɂȂ�
    // �i�C�x���g�L���[�ɂ͏�Ƀ^�C�}�[�����̃L���[�����܂��Ă���j

    // �������[�v
    while(1){
        // �C�x���g���m
        if(SDL_PollEvent(&event)){
            switch (event.type) {
            case SDL_MOUSEMOTION: // �}�E�X���ړ�������
                printf("Mouse moved by %d,%d to (%d,%d)\n", event.motion.xrel, event.motion.yrel, event.motion.x, event.motion.y); // �}�E�X�̍��W��\��
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                case SDLK_RETURN:
                    Sleep5(); // 5�b�ҋ@����֐����Ăяo��
                    break;
                case SDLK_ESCAPE:
                    printf("Terminated.\n");
                    // �I������
                    SDL_RemoveTimer(timer_id); // �^�C�}�̍폜
                    SDL_DestroyWindow(window); // �E�B���h�E�̔j��
                    SDL_Quit(); // SDL�̏I��
                    return 0;
                    break;
                }
                break;
            }
        }
    }
}