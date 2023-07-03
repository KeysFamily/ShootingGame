///-----------------------------------------------------------------------------
// 自己制作補助用ライブラリ
// 制　作　者：22CI0333 長谷川勇一朗
// 制作年月日：2023/4/13
// 概　　　要：制作用です。
//-----------------------------------------------------------------------------
#include "MyAssistLib.h"

namespace Assist {

    //サイズ
    ML::Box2D BoxCentering(int w_, int h_) {
        return ML::Box2D(-w_ / 2, -h_ / 2, w_, h_);
    }

    //(x,y)を中心としてサイズ(w,h)のオブジェクトを用意
    ML::Box2D BoxCentering(int x_, int y_, int w_, int h_) {
        return ML::Box2D(x_ - w_ / 2, y_ - h_ / 2, w_, h_);
    }

    //(pos)を中心としてサイズ(w_,h_のオブジェクトの用意)
    ML::Box2D BoxCentering(ML::Vec2 pos_, int w_, int h_) {
        return ML::Box2D(
            static_cast<int>(pos_.x - w_ / 2),
            static_cast<int>(pos_.y - h_ / 2),
            w_, h_);
    }
}
