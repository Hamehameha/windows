#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <directfb.h>
#include <directfb_util.h>
#include <direct/clock.h>
#include "window.h"


void show_img(char* img)
{
	DFBResult err;
	
	//画像プロバイダを生成
	DFBCHECK(dfb->CreateImageProvider(dfb, img, &provider));
		
	 //画像プロバイダの設定値取得（画像ファイル情報、解像度、ピクセル当たりのビット数等）
	DFBCHECK(provider->GetSurfaceDescription(provider, &dsc));
		
	// 画像プロバイダの設定値でサーフェイスを生成
	DFBCHECK(dfb->CreateSurface(dfb, &dsc, &logo));
		
	//画像を読込み
	DFBCHECK(provider->RenderTo(provider, logo, NULL));

	// 描画する画像を選択して表示
	DFBCHECK(primary->Blit(primary, logo, NULL, 0, 0));
	
	//描画を反映（垂直帰線に同期して）
	DFBCHECK(primary->Flip(primary, NULL, DSFLIP_ONSYNC));
	
	logo->Release(logo);
	provider->Release(provider);
}

