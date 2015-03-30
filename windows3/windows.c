#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <directfb.h>
#include <directfb_util.h>
#include <direct/clock.h>

#define FIN_THRESHOLD 4

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
static IDirectFBEventBuffer *keybuffer;

int cx, cy; // �␳��̃^�b�`���W

// ���j���[�{�^���͈̔�
struct BUTTON {
	int x, y;	// ������W
	int w, h;	// �c���̑傫��
	};

// ���j���[��ʗp�t�@�C��
char* windw_img[] = {"win1.png","win2.png","win3.png","win4.png","penguin.JPG"};

int win_no;	// �\����ʔԍ�

void dfb_init(int argc, char **argv)
{
	DFBResult err;

	 /*DirectFB�̏�����*/
	DFBCHECK(DirectFBInit(&argc, &argv));
	
	 /*���C���C���^�[�t�F�[�X�̎擾*/
	DFBCHECK(DirectFBCreate(&dfb));
	
	//���̓f�o�C�X�i�^�b�`�p�l���j�p�C�x���g�o�b�t�@
	DFBCHECK(dfb->CreateInputEventBuffer(dfb, DICAPS_ALL,
							 DFB_FALSE, &keybuffer));

	
	//�X�[�p�T�[�t�F�C�X�̃t���X�N���[���w��
	DFBCHECK(dfb->SetCooperativeLevel(dfb, DFSCL_FULLSCREEN));

	dsc.flags = DSDESC_CAPS;
	dsc.caps = DSCAPS_PRIMARY | DSCAPS_TRIPLE;
	
	 /*�`���ʂ̎擾*/
	DFBCHECK(dfb->CreateSurface(dfb, &dsc, &primary));
	
	 /*��ʃT�C�Y�̎擾*/
	DFBCHECK(primary->GetSize(primary, &xres, &yres));
	
}

void show_img(char* img)
{
	DFBResult err;
	
	//�摜�v���o�C�_�𐶐�
	DFBCHECK(dfb->CreateImageProvider(dfb, img, &provider));
		
	 //�摜�v���o�C�_�̐ݒ�l�擾�i�摜�t�@�C�����A�𑜓x�A�s�N�Z��������̃r�b�g�����j
	DFBCHECK(provider->GetSurfaceDescription(provider, &dsc));
		
	// �摜�v���o�C�_�̐ݒ�l�ŃT�[�t�F�C�X�𐶐�
	DFBCHECK(dfb->CreateSurface(dfb, &dsc, &logo));
		
	//�摜��Ǎ���
	DFBCHECK(provider->RenderTo(provider, logo, NULL));

	// �`�悷��摜��I�����ĕ\��
	DFBCHECK(primary->Blit(primary, logo, NULL, 0, 0));
	
	//�`��𔽉f�i�����A���ɓ������āj
	DFBCHECK(primary->Flip(primary, NULL, DSFLIP_ONSYNC));
	
	logo->Release(logo);
	provider->Release(provider);
}

// �^�b�`���W����{�^������肷��
int chk_button(struct BUTTON* b, int n)
{
	int i;
	
	for(i=0; i < n; i++)
	{
		if (( b[i].x < cx) && ((b[i].x + b[i].w) > cx) &&
			( b[i].y < cy) && ((b[i].y + b[i].h) > cy) )
			return i;
	}
	return -1;
}


//�^�b�`�����{�^�������
int touch(struct BUTTON* b, int n)
{
	DFBInputEvent evt;	//���̓C�x���g

	int xcur, ycur;	//�^�b�`�������W�ʒu
	int x1, y1, x2, y2;	// �X�N���[������ix1 , y1), �E��(x2 , y2)�@�␳�O���W

	xcur = 0;
	ycur = 0;

	// �C�x���g�҂�
	keybuffer->WaitForEvent(keybuffer);
		
	//�C�x���g�擾
	while (keybuffer->GetEvent(keybuffer, DFB_EVENT(&evt)) == DFB_OK) {
			
		//�C�x���g�̎��
		switch (evt.type) {
		
			case DIET_BUTTONPRESS:	/* �^�b�`ON */
				printf("\nPRESS !!   ");
				break;
				 
			case DIET_BUTTONRELEASE:	/* �^�b�`OFF */
				printf("\nRELEASE !!  \n" );
				
				break;
			
			case DIET_AXISMOTION:	/* �^�b�`���Ă������ */
			
				switch (evt.axis) {
					case DIAI_X:	// X���̒l
						xcur = evt.axisabs;
						//printf("(%d , %d) ", xcur ,ycur );
						break;
					
					case DIAI_Y:	// Y���̒l
						ycur = evt.axisabs;
						//printf("(%d , %d) ", xcur ,ycur );
						
						// �X�N���[�����W�␳
						// ����ix1 , y1), �E����(x2 , y2)�Ƃ���
						x1=270; y1= 3617;
						x2=3832; y2= 362;
					
						cx = ((xcur -  x1) * xres) / (x2 - x1);
						cy = yres - ((ycur - y2) * yres) / (y1 - y2);
					
						// ����l�A�����l�ɍ��킹��
						if (cx < 0) cx = 0;
						if (cx > (xres-1)) cx = xres-1;
						if (cy < 0) cy = 0;
						if (cy > (yres-1)) cy = yres-1;
						
						printf("(%d , %d)\n", cx, cy);
												
						// �����Ń^�b�`���W�ɉ���������������
						usleep(200000);		//���z�I�ȏ������ԁi�`���^�����O�h�~�j
						
						keybuffer->Reset(keybuffer);	// �o�b�t�@�N���A�i�`���^�����O�h�~�j
						
						// �o�b�t�@�J���ǂ݁i�`���^�����O�h�~�j
						//while (keybuffer->GetEvent(keybuffer, DFB_EVENT(&evt)) == DFB_OK) ;
						
						return (chk_button( b, n) + 1);	// �����ꂽ�{�^���ԍ��i�{�^���ȊO�̍��W��0�j
						
						break;
					
					default:
						printf("? evt.axis=%d\n", evt.axis);
				}
			break;
				
			default:
				printf("? evt.type=%d\n", evt.type);
		}
	}
	return -1;  //�^�b�`�C�x���g�ȊO
}

int sub(void)	
{

	int ret;	// �߂�l�p
	int n;
	struct BUTTON b[2] ={{30, 180, 130-30, 250-180 },	// �߂�{�^��
						{ 340, 180, 440-340, 250-180}};	// ���փ{�^��
						
	
	if (win_no == 4)	// �Ō�̉�ʂ͖߂�{�^���̂�
		n = 1;	//�{�^���̐�							
	else
		n = 2;	//�{�^���̐�							

	//�C�x���g���[�v
	while (1) {

		ret = touch(b, n);	// �^�b�`�����{�^���𒲂ׂ�
		
		switch (ret){
		
		case 0:	// �{�^��������ł��Ȃ������ꍇ
			break;
		
		case 1:	// b[1]�o�^�{�^���������ꂽ
			printf(" BUTTON NO=%d \n\n", ret);
			
			--win_no ;
			show_img( windw_img[win_no] );
			
			return 0; // �O��ʂɖ߂�
			break;
			
		case 2: // b[0] �o�^�{�^���������ꂽ
			printf(" BUTTON NO=%d \n\n", ret);
			
			// ���摜��\��
			++win_no ;
			show_img( windw_img[win_no] );
			sub();	// �ċA

			break;
			
		
		default:
			break;
		}
		
		fflush(stdin);
	}

	return 0;
}



// �g�b�v��ʂ̏���
int main(int argc, char **argv)
{
	int ret;	// �߂�l�p
	
	struct BUTTON b[2] ={{ 340, 180, 440-340, 250-180},	// ���փ{�^��
						{320, 33, 430-320, 100-33 }};	// �I���{�^��
						
	int n = 2;	//�{�^���̐�

	 /*DirectFB�̏�����*/
	dfb_init(argc, argv);
	
	win_no = 0;
	
	// �摜��\��
	show_img( windw_img[win_no] );
	
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
			show_img( windw_img[win_no] );
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
