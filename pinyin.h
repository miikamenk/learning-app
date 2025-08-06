#ifndef PINYIN_H
#define PINYIN_H

#include <memory>
#include <string>
#include <vector>
#include <cpp-pinyin/Pinyin.h>

class PinyinWrapper {
public:
    explicit PinyinWrapper();
    std::string convert(const std::string& hanzi);

private:
    std::unique_ptr<Pinyin::Pinyin> g2p;
};

#endif // PINYIN_H
