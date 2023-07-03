///-----------------------------------------------------------------------------
// ���Ȑ���⏕�p���C�u����
// ���@��@�ҁF22CI0333 ���J��E��N
// ����N�����F2023/4/13
// �T�@�@�@�v�F����p�ł��B
//-----------------------------------------------------------------------------
#include "MyAssistLib.h"

namespace Assist {

    //�T�C�Y
    ML::Box2D BoxCentering(int w_, int h_) {
        return ML::Box2D(-w_ / 2, -h_ / 2, w_, h_);
    }

    //(x,y)�𒆐S�Ƃ��ăT�C�Y(w,h)�̃I�u�W�F�N�g��p��
    ML::Box2D BoxCentering(int x_, int y_, int w_, int h_) {
        return ML::Box2D(x_ - w_ / 2, y_ - h_ / 2, w_, h_);
    }

    //(pos)�𒆐S�Ƃ��ăT�C�Y(w_,h_�̃I�u�W�F�N�g�̗p��)
    ML::Box2D BoxCentering(ML::Vec2 pos_, int w_, int h_) {
        return ML::Box2D(
            static_cast<int>(pos_.x - w_ / 2),
            static_cast<int>(pos_.y - h_ / 2),
            w_, h_);
    }
}
