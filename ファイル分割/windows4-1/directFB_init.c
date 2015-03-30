#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <directfb.h>
#include <directfb_util.h>
#include <direct/clock.h>

// DirectFBから呼び出されるエラーチェック用マクロ
#define DFBCHECK(x...)						\
	{								\
		err = x;							\
		if (err != DFB_OK) {					\
			fprintf( stderr, "%s <%d>:\n\t", __FILE__, __LINE__ );	\
			DirectFBErrorFatal( #x, err );				\
		}								\
	}
	
// DirectFBの機能を実現するためのスーパインターフェイス（最上位のインターフェース）
extern IDirectFB *dfb;

extern DFBSurfaceDescription dsc;	//サーフェイス情報

// プライマリ・サーフェイス（フル画面のプライマリレイヤ）
extern IDirectFBSurface *primary;

extern int xres, yres; // プライマリ・サーフェイス（解像度）

// キー入力（タッチパネル）イベントバッファ
extern IDirectFBEventBuffer *keybuffer;

// DirectFBの初期化
void dfb_init(int argc, char **argv)
{
	DFBResult err;

	 /*DirectFBの初期化*/
	DFBCHECK(DirectFBInit(&argc, &argv));
	
	 /*メインインターフェースの取得*/
	DFBCHECK(DirectFBCreate(&dfb));
	
	//入力デバイス（タッチパネル）用イベントバッファ
	DFBCHECK(dfb->CreateInputEventBuffer(dfb, DICAPS_ALL,
							 DFB_FALSE, &keybuffer));

	
	//スーパサーフェイスのフルスクリーン指定
	DFBCHECK(dfb->SetCooperativeLevel(dfb, DFSCL_FULLSCREEN));

	dsc.flags = DSDESC_CAPS;
	dsc.caps = DSCAPS_PRIMARY | DSCAPS_TRIPLE;
	
	 /*描画画面の取得*/
	DFBCHECK(dfb->CreateSurface(dfb, &dsc, &primary));
	
	 /*画面サイズの取得*/
	DFBCHECK(primary->GetSize(primary, &xres, &yres));
	
}






