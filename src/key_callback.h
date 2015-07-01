#pragma once

#include <map>

template <typename T, typename Obj, typename Fun>
auto key_dispatch(Obj obj, const std::map<int, T> &triggers, int key, Fun fun) {
  auto in = triggers.find(key);
  if (in != triggers.end()) {
    fun((obj->*(in->second))());
  }
}

template <typename T, typename Obj>
auto key_dispatch(Obj obj, const std::map<int, T> &triggers, int key) {
  auto in = triggers.find(key);
  if (in != triggers.end()) {
    (obj->*(in->second))();
  }
}
