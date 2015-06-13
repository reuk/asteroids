#include "program.h"

#include "logger.h"

#include <vector>
#include <map>

using namespace std;

Program::Program():
    index(glCreateProgram())
{
    if (index == 0) {
        throw runtime_error("failed to create shader program");
    }
}

Program::~Program() {
    glDeleteProgram(index);
}

void Program::attach(const Shader & s) const {
    glAttachShader(index, s.get_index());
}

void Program::detach(const Shader & s) const {
    glDetachShader(index, s.get_index());
}

void Program::link() const {
    glLinkProgram(index);
}

void Program::use() const {
    glUseProgram(index);
}

GLuint Program::get_index() const {
    return index;
}

GLint Program::get_attrib_location(const std::string & name) const {
    auto ret = glGetAttribLocation(index, name.c_str());
    if (ret == -1)
        Logger::log_err("no such attrib: ", name);
    return ret;
}

GLint Program::get_uniform_location(const std::string & name) const {
    auto ret = glGetUniformLocation(index, name.c_str());
    if (ret == -1)
        Logger::log_err("no such uniform: ", name);
    return ret;
}

void simple_print(GLuint item, const pair<GLenum, string> & i, int & params) {
    glGetProgramiv(item, i.first, &params);
    Logger::log(i.second, ": ", params);
}

void log_params() {
    vector<pair<GLenum, string>> int_params = {
        {GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS    , "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS"},
        {GL_MAX_CUBE_MAP_TEXTURE_SIZE           , "GL_MAX_CUBE_MAP_TEXTURE_SIZE"},
        {GL_MAX_DRAW_BUFFERS                    , "GL_MAX_DRAW_BUFFERS"},
        {GL_MAX_FRAGMENT_UNIFORM_COMPONENTS     , "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS"},
        {GL_MAX_TEXTURE_IMAGE_UNITS             , "GL_MAX_TEXTURE_IMAGE_UNITS"},
        {GL_MAX_TEXTURE_SIZE                    , "GL_MAX_TEXTURE_SIZE"},
        {GL_MAX_VARYING_FLOATS                  , "GL_MAX_VARYING_FLOATS"},
        {GL_MAX_VERTEX_ATTRIBS                  , "GL_MAX_VERTEX_ATTRIBS"},
        {GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS      , "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS"},
        {GL_MAX_VERTEX_UNIFORM_COMPONENTS       , "GL_MAX_VERTEX_UNIFORM_COMPONENTS"},
    };

    vector<pair<GLenum, string>> two_int_params = {
        {GL_MAX_VIEWPORT_DIMS                   , "GL_MAX_VIEWPORT_DIMS"},
    };

    vector<pair<GLenum, string>> bool_params = {
        {GL_STEREO                              , "GL_STEREO"},
    };

    Logger::log("GL Context Parameters:");

    for (const auto & i : int_params) {
        GLint v = 0;
        glGetIntegerv(i.first, &v);
        Logger::log(i.second, ": ", v);
    }

    for (const auto & i : two_int_params) {
        GLint v[2] = {0, 0};
        glGetIntegerv(i.first, v);
        Logger::log(i.second, ": ", v[0], ", ", v[1]);
    }

    for (const auto & i : bool_params) {
        GLboolean v = 0;
        glGetBooleanv(i.first, &v);
        Logger::log(i.second, ": ", (bool)v);
    }

    Logger::log("--------------------------------");
}

template<typename T>
void print_info_log(GLuint shader, T func) {
    vector<char> log(2048);
    auto length = 0;
    func(shader, log.size() * sizeof(decltype(log)::value_type), &length, log.data());
    if (length) {
        Logger::log("info log for index ", shader, ":");
        Logger::log(log.data());
    }
}

template<typename T, typename U>
auto check_gl(GLuint item, GLenum flag, T a, U b) {
    auto params = -1;
    a(item, flag, &params);
    if (params != GL_TRUE) {
        Logger::log_err("ERROR: GL index ", item);
        print_info_log(item, b);
        return false;
    }
    return true;
}

auto check_shader(GLuint item) {
    return check_gl(item, GL_COMPILE_STATUS, glGetShaderiv, glGetShaderInfoLog);
}

auto check_program(GLuint item) {
    return check_gl(item, GL_LINK_STATUS, glGetProgramiv, glGetProgramInfoLog);
}

bool Program::check() const {
    return check_program(index);
}

template<typename T, typename U>
void print_params(GLuint item, int params, T a, U b) {
    const map<GLenum, string> type_strings = {
        {GL_BOOL                    , "GL_BOOL"},
        {GL_INT                     , "GL_INT"},
        {GL_FLOAT                   , "GL_FLOAT"},
        {GL_FLOAT_VEC2              , "GL_FLOAT_VEC2"},
        {GL_FLOAT_VEC3              , "GL_FLOAT_VEC3"},
        {GL_FLOAT_VEC4              , "GL_FLOAT_VEC4"},
        {GL_FLOAT_MAT2              , "GL_FLOAT_MAT2"},
        {GL_FLOAT_MAT3              , "GL_FLOAT_MAT3"},
        {GL_FLOAT_MAT4              , "GL_FLOAT_MAT4"},
        {GL_SAMPLER_2D              , "GL_SAMPLER_2D"},
        {GL_SAMPLER_3D              , "GL_SAMPLER_3D"},
        {GL_SAMPLER_CUBE            , "GL_SAMPLER_CUBE"},
        {GL_SAMPLER_2D_SHADOW       , "GL_SAMPLER_2D_SHADOW"},
    };

    for (auto i = 0; i != params; ++i) {
        vector<char> name(64);
        auto length = 0;
        auto size = 0;
        GLenum type;
        a(item, i, name.size() * sizeof(decltype(name)::value_type), &length, &size, &type, name.data());

        if (size > 1) {
            for (auto j = 0; j != size; ++j) {
                stringstream ss;
                ss << name.data() << "[" << j << "]";
                int location = b(item, ss.str().c_str());
                Logger::log(i, " type: ", type_strings.at(type), " name: ", ss.str(), " location: ", location);
            }
        } else {
            int location = b(item, name.data());
            Logger::log(i, " type: ", type_strings.at(type), " name: ", name.data(), " location: ", location);
        }
    }
}

void print_all(GLuint item) {
    Logger::log("--- shader program ", item, " info ---");
    auto params = -1;

    simple_print(item, {GL_LINK_STATUS         , "GL_LINK_STATUS"},         params);
    simple_print(item, {GL_ATTACHED_SHADERS    , "GL_ATTACHED_SHADERS"},    params);

    simple_print(item, {GL_ACTIVE_ATTRIBUTES   , "GL_ACTIVE_ATTRIBUTES"},   params);
    print_params(item, params, glGetActiveAttrib, glGetAttribLocation);

    simple_print(item, {GL_ACTIVE_UNIFORMS     , "GL_ACTIVE_UNIFORMS"},     params);
    print_params(item, params, glGetActiveUniform, glGetUniformLocation);

    print_info_log(item, glGetProgramInfoLog);
}

bool Program::verify() const {
    glValidateProgram(index);

    auto params = -1;
    simple_print(index, {GL_VALIDATE_STATUS, "GL_VALIDATE_STATUS"}, params);

    if (params != GL_TRUE) {
        print_info_log(index, glGetProgramInfoLog);
        return false;
    }
    return true;
}
