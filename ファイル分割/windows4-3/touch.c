#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <directfb.h>
#include <directfb_util.h>
#include <direct/clock.h>

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

// DirectFBの初期化
extern void dfb_init(int , char** );

// 画像の表示
extern void show_img(char* );

// タッチ座標からボタンを特定する
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


//タッチしたボタンを特定
int touch(struct BUTTON* b, int n)
{
	DFBInputEvent evt;	//入力イベント

	int xcur, ycur;	//タッチした座標位置
	int x1, y1, x2, y2;	// スクリーン左上（x1 , y1), 右下(x2 , y2)　補正前座標

	xcur = 0;
	ycur = 0;

	// イベント待ち
	keybuffer->WaitForEvent(keybuffer);
		
	//イベント取得
	while (keybuffer->GetEvent(keybuffer, DFB_EVENT(&evt)) == DFB_OK) {
			
		//イベントの種類
		switch (evt.type) {
		
			case DIET_BUTTONPRESS:	/* タッチON */
				printf("\nPRESS !!   ");
				break;
				 
			case DIET_BUTTONRELEASE:	/* タッチOFF */
				printf("\nRELEASE !!  \n" );
				
				break;
			
			case DIET_AXISMOTION:	/* タッチしている期間 */
			
				switch (evt.axis) {
					case DIAI_X:	// X軸の値
						xcur = evt.axisabs;
						//printf("(%d , %d) ", xcur ,ycur );
						break;
					
					case DIAI_Y:	// Y軸の値
						ycur = evt.axisabs;
						//printf("(%d , %d) ", xcur ,ycur );
						
						// スクリーン座標補正
						// 左上（x1 , y1), 右下を(x2 , y2)とする
						x1=270; y1= 3617;
						x2=3832; y2= 362;
					
						cx = ((xcur -  x1) * xres) / (x2 - x1);
						cy = yres - ((ycur - y2) * yres) / (y1 - y2);
					
						// 上限値、下限値に合わせる
						if (cx < 0) cx = 0;
						if (cx > (xres-1)) cx = xres-1;
						if (cy < 0) cy = 0;
						if (cy > (yres-1)) cy = yres-1;
						
						printf("(%d , %d)\n", cx, cy);
												
						// ここでタッチ座標に応じた処理をする
						usleep(200000);		//仮想的な処理時間（チャタリング防止）
						
						keybuffer->Reset(keybuffer);	// バッファクリア（チャタリング防止）
						
						// バッファカラ読み（チャタリング防止）
						//while (keybuffer->GetEvent(keybuffer, DFB_EVENT(&evt)) == DFB_OK) ;
						
						return (chk_button( b, n) + 1);	// 押されたボタン番号（ボタン以外の座標は0）
						
						break;
					
					default:
						printf("? evt.axis=%d\n", evt.axis);
				}
			break;
				
			default:
				printf("? evt.type=%d\n", evt.type);
		}
	}
	return -1;  //タッチイベント以外
}

int sub(void)	
{

	int ret;	// 戻り値用
	int n;
	struct BUTTON b[2] ={{30, 180, 130-30, 250-180 },	// 戻るボタン
						{ 340, 180, 440-340, 250-180}};	// 次へボタン
	
	// メニュー画面用ファイル
	char* window_img[] = {"win1.png","win2.png","win3.png","win4.png","penguin.JPG"};

	
	if (win_no == 4)	// 最後の画面は戻るボタンのみ
		n = 1;	//ボタンの数							
	else
		n = 2;	//ボタンの数							

	//イベントループ
	while (1) {

		ret = touch(b, n);	// タッチしたボタンを調べる
		
		switch (ret){
		
		case 0:	// ボタンが特定できなかった場合
			break;
		
		case 1:	// b[1]登録ボタンが押された
			printf(" BUTTON NO=%d \n\n", ret);
			
			--win_no ;
			show_img( window_img[win_no] );
			
			return 0; // 前画面に戻る
			break;
			
		case 2: // b[0] 登録ボタンが押された
			printf(" BUTTON NO=%d \n\n", ret);
			
			// 次画像を表示
			++win_no ;
			show_img( window_img[win_no] );
			sub();	// 再帰

			break;
			
		
		default:
			break;
		}
		
		fflush(stdin);
	}

	return 0;
}
