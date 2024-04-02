//
// Created by TS on 2024/2/29.
//
#pragma once

#include <GLBaseSample.h>

class NativeTriangle4 : public GLBaseSample{

public:
    NativeTriangle4() = default;

    virtual ~NativeTriangle4() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();
};