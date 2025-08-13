#pragma once
#include <cstddef>
#include <atomic>
#include <mutex>

template <typename T, uint32_t N>
struct TS_RB {
public:

    uint32_t GetElemNum(){
        return nElem.load();
    }
    void AddElem(const T& elem){
        std::lock_guard<std::mutex> lock(mutex);
        objs[(idx.load() + nElem.load()) % N].store(elem);
        nElem.fetch_add(1);
    }
    void PopElem(T& elem){
        std::lock_guard<std::mutex> lock(mutex);
        if(!GetElemNum.load()) return;
        elem = objs[idx.load()].load();
        idx.store((idx.load() + 1) % N);
        nElem.fetch_sub(1);
    }
private:
    std::atomic<T> objs[N];
    std::atomic<uint32_t> nElem = 0;
    std::atomic<uint32_t> idx = 0;
    std::mutex mutex;
};