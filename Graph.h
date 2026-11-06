#pragma once
#include <string>
#include "Base.h"

// 画像描画用クラス ベース継承
class GraphDraw : public Base
{
protected:
    int graph_hnd = -1;          // 画像ハンドル(-1 = 未読み込み)
    std::string file_name;       // ファイル名

public:
    //  デフォルトコンストラクタ
    GraphDraw() = default;

    GraphDraw(std::string arg_file_name, int x, int y)
    {
        Load(arg_file_name);
        SetPos(x, y);
    }
    // 画像読み込み(成功したら true)
    bool Load(const std::string& arg_file_name)
    {
        Release();  // 既に読み込み済みなら先に解放
        file_name = arg_file_name;
        graph_hnd = LoadGraph(file_name.c_str());
        return graph_hnd != -1;
    }
    virtual void Draw() const
    {
        if (graph_hnd != -1)
        {
            DrawGraph(pos_x, pos_y, graph_hnd, TRUE);
        }
    }

    void Release()
    {
        if (graph_hnd != -1)
        {
            DeleteGraph(graph_hnd);
            graph_hnd = -1;
        }
    }
};