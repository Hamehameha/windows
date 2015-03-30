#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <directfb.h>
#include <directfb_util.h>
#include <direct/clock.h>

// DirectFB����Ăяo�����G���[�`�F�b�N�p�}�N��
#define DFBCHECK(x...)						\
	{								\
		err = x;							\
		if (err != DFB_OK) {					\
			fprintf( stderr, "%s <%d>:\n\t", __FILE__, __LINE__ );	\
			DirectFBErrorFatal( #x, err );				\
		}								\
	}

// DirectFB�̋@�\���������邽�߂̃X�[�p�C���^�[�t�F�C�X�i�ŏ�ʂ̃C���^�[�t�F�[�X�j
IDirectFB *dfb;

DFBSurfaceDescription dsc;	//�T�[�t�F�C�X���

// �v���C�}���E�T�[�t�F�C�X�i�t����ʂ̃v���C�}�����C���j
IDirectFBSurface *primary;

int xres, yres; // �v���C�}���E�T�[�t�F�C�X�i�𑜓x�j

IDirectFBImageProvider *provider;	//�@�摜���擾�p�i�𑜓x�A�j
IDirectFBSurface *logo;	//�T�[�t�F�C�X


// �L�[���́i�^�b�`�p�l���j�C�x���g�o�b�t�@
IDirectFBEventBuffer *keybuffer;

int cx, cy; // �␳��̃^�b�`���W

// ���j���[�{�^���͈̔�
struct BUTTON {
	int x, y;	// ������W
	int w, h;	// �c���̑傫��
	};

int win_no;	// �\����ʔԍ�

// DirectFB�̏�����
extern void dfb_init(int , char** );

// �摜�̕\��
extern void show_img(char* );

// �^�b�`���W����{�^������肷��
extern int chk_button(struct BUTTON* , int );

//�^�b�`�����{�^�������
extern int touch(struct BUTTON* , int );

extern int sub(void);

// �g�b�v��ʂ̏���
int main(int argc, char **argv)
{
	int ret;	// �߂�l�p
	
	// ���j���[��ʗp�t�@�C��
	char* window_img[] = {"win1.png","win2.png","win3.png","win4.png","penguin.JPG"};
	
	struct BUTTON b[2] ={{ 340, 180, 440-340, 250-180},	// ���փ{�^��
						{320, 33, 430-320, 100-33 }};	// �I���{�^��
						
	int n = 2;	//�{�^���̐�

	 /*DirectFB�̏�����*/
	dfb_init(argc, argv);
	
	win_no = 0;
	
	// �摜��\��
	show_img( window_img[win_no] );
	
	//�C�x���g���[�v
	while (1) {

		ret = touch(b, n);	// �^�b�`�����{�^���𒲂ׂ�
		
		switch (ret){
		
		case 0:	// �{�^��������ł��Ȃ������ꍇ
			break;
		
		case 1: // b[0] �o�^�{�^���������ꂽ
			printf(" BUTTON NO=%d \n\n", ret);
			
			// next�摜��\��
			++win_no ;
			show_img( window_img[win_no] );
			sub();
			
			break;
			
		case 2:	// b[1]�o�^�{�^���������ꂽ
			printf(" BUTTON NO=%d \n\n", ret);
			
			keybuffer->Release(keybuffer);
			primary->Release(primary);
			dfb->Release(dfb);

			exit(0);	// �v���O�������I��
			break;
		
		default:
			break;
		}
		
		fflush(stdin);
	}

	return 0;
}
