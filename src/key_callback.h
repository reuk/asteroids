#pragma once

#include <map>

template <typename T, typename Obj>
auto key_dispatch(Obj obj, const std::map<int, T> &triggers, int key) {
    auto in = triggers.find(key);
    if (in != triggers.end()) {
        (obj->*(in->second))();
    }
}

template <typename T, typename Obj, typename... Ts>
auto key_dispatch(Obj obj, const std::map<int, T> &triggers, int key, Ts&&... ts) {
    auto in = triggers.find(key);
    if (in != triggers.end()) {
        (obj->*(in->second))(ts...);
    }
}
