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

extern IDirectFBImageProvider *provider;	//　画像情報取得用（解像度、）
extern IDirectFBSurface *logo;		//サーフェイス

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

