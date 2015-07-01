#pragma once

#include "program.h"

class ShaderProgram : public Program {
public:
    ShaderProgram(const std::string &vs, const std::string &fs) {
        FragmentShader f;
        f.source(fs);
        f.compile();

        VertexShader v;
        v.source(vs);
        v.compile();

        attach(f);
        attach(v);
        link();
    }
};
