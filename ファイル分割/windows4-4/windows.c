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
IDirectFB *dfb;

DFBSurfaceDescription dsc;	//サーフェイス情報

// プライマリ・サーフェイス（フル画面のプライマリレイヤ）
IDirectFBSurface *primary;

int xres, yres; // プライマリ・サーフェイス（解像度）

IDirectFBImageProvider *provider;	//　画像情報取得用（解像度、）
IDirectFBSurface *logo;	//サーフェイス


// キー入力（タッチパネル）イベントバッファ
IDirectFBEventBuffer *keybuffer;

int cx, cy; // 補正後のタッチ座標

// メニューボタンの範囲
struct BUTTON {
	int x, y;	// 左上座標
	int w, h;	// 縦横の大きさ
	};

int win_no;	// 表示画面番号

// DirectFBの初期化
extern void dfb_init(int , char** );

// 画像の表示
extern void show_img(char* );

// タッチ座標からボタンを特定する
extern int chk_button(struct BUTTON* , int );

//タッチしたボタンを特定
extern int touch(struct BUTTON* , int );

extern int sub(void);

// トップ画面の処理
int main(int argc, char **argv)
{
	int ret;	// 戻り値用
	
	// メニュー画面用ファイル
	char* window_img[] = {"win1.png","win2.png","win3.png","win4.png","penguin.JPG"};
	
	struct BUTTON b[2] ={{ 340, 180, 440-340, 250-180},	// 次へボタン
						{320, 33, 430-320, 100-33 }};	// 終了ボタン
						
	int n = 2;	//ボタンの数

	 /*DirectFBの初期化*/
	dfb_init(argc, argv);
	
	win_no = 0;
	
	// 画像を表示
	show_img( window_img[win_no] );
	
	//イベントループ
	while (1) {

		ret = touch(b, n);	// タッチしたボタンを調べる
		
		switch (ret){
		
		case 0:	// ボタンが特定できなかった場合
			break;
		
		case 1: // b[0] 登録ボタンが押された
			printf(" BUTTON NO=%d \n\n", ret);
			
			// next画像を表示
			++win_no ;
			show_img( window_img[win_no] );
			sub();
			
			break;
			
		case 2:	// b[1]登録ボタンが押された
			printf(" BUTTON NO=%d \n\n", ret);
			
			keybuffer->Release(keybuffer);
			primary->Release(primary);
			dfb->Release(dfb);

			exit(0);	// プログラムを終了
			break;
		
		default:
			break;
		}
		
		fflush(stdin);
	}

	return 0;
}
