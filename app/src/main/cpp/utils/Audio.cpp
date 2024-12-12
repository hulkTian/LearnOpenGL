//
// Created by ts on 2024/12/12.
//

#include "Audio.h"

AAudioStreamBuilder *builder;

void Audio::Init() {
   aaudio_result_t  result = AAudio_createStreamBuilder(&builder);
}
