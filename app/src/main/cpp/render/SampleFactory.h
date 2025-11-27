//
// Created by tzh on 2025/11/26.
//

#pragma once
#include <unordered_map>
#include <functional>

// 前向声明
class GLBaseSample;

class SampleFactory {
public:
    static SampleFactory& Instance() {
        static SampleFactory instance;
        return instance;
    }

    void Register(float type, std::function<GLBaseSample*()> creator) {
        creators[type] = creator;
    }

    GLBaseSample* Create(float type) {
        auto it = creators.find(type);
        if (it != creators.end()) return it->second();
        return nullptr;
    }
private:
    std::unordered_map<float, std::function<GLBaseSample*()>> creators;
};

// 注册宏可以放在 GLBaseSample.h 里

