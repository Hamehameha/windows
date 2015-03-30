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

extern IDirectFBImageProvider *provider;	//�@�摜���擾�p�i�𑜓x�A�j
extern IDirectFBSurface *logo;		//�T�[�t�F�C�X

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

