#ifndef __WINDOW_H__

#define __WINDOW_H__

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
extern IDirectFB *dfb;

extern DFBSurfaceDescription dsc;	//�T�[�t�F�C�X���

// �v���C�}���E�T�[�t�F�C�X�i�t����ʂ̃v���C�}�����C���j
extern IDirectFBSurface *primary;

int xres, yres; // �v���C�}���E�T�[�t�F�C�X�i�𑜓x�j

extern IDirectFBImageProvider *provider;	//�@�摜���擾�p�i�𑜓x�A�j
extern IDirectFBSurface *logo;	//�T�[�t�F�C�X


// �L�[���́i�^�b�`�p�l���j�C�x���g�o�b�t�@
extern IDirectFBEventBuffer *keybuffer;

extern int cx, cy; // �␳��̃^�b�`���W

// ���j���[�{�^���͈̔�
struct BUTTON {
	int x, y;	// ������W
	int w, h;	// �c���̑傫��
	};

extern int win_no;	// �\����ʔԍ�

#endif // __WINDOW_H__
