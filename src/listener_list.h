#pragma once

#include <set>
#include <memory>

template<typename Listener, typename Ptr = Listener *, typename Collection = std::set<Ptr>>
class ListenerList {
public:
    template<typename... Parameters, typename... Values>
    inline void call(void (Listener::*callback)(Parameters...), Values && ... values) const {
        for (auto && i : listener)
            (i->*callback)(std::forward<Values>(values)...);
    }

    inline void add(Ptr ptr) { listener.insert(ptr); }
    inline void remove(Ptr ptr) { listener.erase(ptr); }

private:
    Collection listener;
};
