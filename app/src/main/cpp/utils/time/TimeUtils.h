//
// Created by OuyangPeng on 2021/10/24.
//

#ifndef OPENGLESDEMO_TIMEUTILS_H
#define OPENGLESDEMO_TIMEUTILS_H

#include <ctime>

class TimeUtils{

public:
    static float currentTimeSeconds() {
        /*time_t now = time(NULL);
        struct tm *t = localtime(&now);
        return t->tm_sec;*/

        clock_t ct;
        ct = clock();
        return ((float)ct/CLOCKS_PER_SEC);
    }
};

#endif //OPENGLESDEMO_TIMEUTILS_H
