#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <directfb.h>
#include <directfb_util.h>
#include <direct/clock.h>
#include "window.h"


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






