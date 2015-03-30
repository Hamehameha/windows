#ifndef __WINDOW_H__

#define __WINDOW_H__

#define FIN_THRESHOLD 4

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

int xres, yres; // プライマリ・サーフェイス（解像度）

extern IDirectFBImageProvider *provider;	//　画像情報取得用（解像度、）
extern IDirectFBSurface *logo;	//サーフェイス


// キー入力（タッチパネル）イベントバッファ
extern IDirectFBEventBuffer *keybuffer;

extern int cx, cy; // 補正後のタッチ座標

// メニューボタンの範囲
struct BUTTON {
	int x, y;	// 左上座標
	int w, h;	// 縦横の大きさ
	};

extern int win_no;	// 表示画面番号

#endif // __WINDOW_H__
