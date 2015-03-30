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
extern IDirectFB *dfb;

extern DFBSurfaceDescription dsc;	//�T�[�t�F�C�X���

// �v���C�}���E�T�[�t�F�C�X�i�t����ʂ̃v���C�}�����C���j
extern IDirectFBSurface *primary;

extern int xres, yres; // �v���C�}���E�T�[�t�F�C�X�i�𑜓x�j

// �L�[���́i�^�b�`�p�l���j�C�x���g�o�b�t�@
extern IDirectFBEventBuffer *keybuffer;

// DirectFB�̏�����
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






