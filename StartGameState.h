#pragma once
#include "GameState.h"
#include "KeyReader.h"
#include "Graph.h"
#include "Beef.h"
#include "Onion.h"
#include "Radle.h"
#include "Rice.h"
#include "Steam.h"
#include <vector>

#define BEEF_NUM 120
#define ONION_NUM 75

//  提供までのモード分け
enum 
{
    PLAY,       //  盛るまで
    SLIDE_IN,   //  横にごはん移動
    SELECT,     //  選択画面
    SERVE,      //  提供
    RETURN,    //  戻す
    COMMENT,    //  客コメント
    END        //   終了
};


class StartGameState :public GameState
{
private:

    KeyReader key;
	GraphDraw bg;               //  背景
    GraphDraw pot;              //  鍋
    GraphDraw soup;             //  汁
    //  具材（牛肉・たまねぎ）
    Beef beef[BEEF_NUM];
    Onion onion[ONION_NUM];

    Radle cup;					//	お玉のカップのほう
    Radle handle;				//	お玉の柄

    Rice rice;                  //  ごはん

    GraphDraw window;           //  窓口
    GraphDraw explain;          //  説明書きの盤
    GraphDraw frame;            //  青のフレーム
    GraphDraw comment;          //  客コメント用ふきだし
    GraphDraw comment2;         //  客コメント用ふきだしエンド用
    GraphDraw customer;         //  客
    GraphDraw customer2;        //  客　リザルト用
    GraphDraw mButton;          //  ｍキーボタン画像
    GraphDraw rButton;          //  Rキーボタン画像
    GraphDraw countBoard;       //  牛肉とたまねぎの数を置く用の画像
    GraphDraw order_normal;     //  ノーマル牛丼のオーダーUI
    GraphDraw order_large;      //  大盛牛丼のオーダーUI
    GraphDraw manual;           //  マニュアル
   
    std::vector<Steam> steam;   //  湯気

    int beef_in_rice = 0;	    //	ごはんの上にある牛肉の数
    int onion_in_rice = 0;	    //	ごはんの上にある玉ねぎの数

    //  提供選択　変数
    int phase = PLAY;           //  モード　最初はプレイモード
    int phase_timer = 0;        //  タイマー　
    int yes_no = 0;			    //	0:はい 1:いいえ
    //  暗転
    int dark_alpha = 0;		    //	提供中の暗転
    int end_alpha = 0;          //  提供終了時の暗転
    bool dark_on = false;       //  暗転フラグ

    int order_size = 0;			//	0:並 1:大
    int serve_count = 0;		//	提供した回数
    //  ポイント
    int customer_point = 0;		    //	今回の点数
    int customer_point_sum = 0;		//	合計点
    std::string comment_text;		//	客のコメント
    
    int start_timer = 0;					//	開始からのフレーム数
    static const int START_MSG_TIME = 300;	//	案内を出す長さ(5秒)

    float manual_y = 720;					//	マニュアルのY位置(画面外の下)

    bool pushMKey=false;                    //  Mキー押したか

    //	ごはんの上にある具材だけ、ごはんと一緒に動かす
    void MoveServing(int dx, int dy)
    {
        rice.Move(dx, dy);
        for (int i = 0; i < BEEF_NUM; i++) {
            if (beef[i].in_rice) beef[i].Move(dx, dy);
        }
        for (int i = 0; i < ONION_NUM; i++) {
            if (onion[i].in_rice) onion[i].Move(dx, dy);
        }
    }

    //	採点
    void Judge();
   
public:

    StartGameState(GameManager* mgr) : GameState(mgr) {}

   
    void Init() override;
    void Input()override;
    void Update() override;
    void Draw() override;

   

};

