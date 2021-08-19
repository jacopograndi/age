#ifndef CAM_H
#define CAM_H

#include <vector>


class Cam {
    public:
    Cam () { pos = vec2(); scale = 1; }
    vec2 pos;
    float scale;
};

#endif