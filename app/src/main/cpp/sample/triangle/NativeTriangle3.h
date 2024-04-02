//
// Created by TS on 2024/2/29.
//

#pragma once

#include <GLBaseSample.h>

class NativeTriangle3 : public GLBaseSample {

public:
    NativeTriangle3() = default;

    virtual ~NativeTriangle3() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();
};