//
// Created by OuyangPeng on 2021/10/24.
//

#ifndef OPENGLESDEMO_TIMEUTILS_H
#define OPENGLESDEMO_TIMEUTILS_H

#include <ctime>

class TimeUtils{

public:
    static int currentTimeSeconds() {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        return t->tm_sec;
    }
};

#endif //OPENGLESDEMO_TIMEUTILS_H
