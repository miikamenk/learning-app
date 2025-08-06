#include "pinyin.h"
#include <cpp-pinyin/Pinyin.h>
#include <QDebug>

PinyinWrapper::PinyinWrapper() {
    g2p = std::make_unique<Pinyin::Pinyin>();
}

std::string PinyinWrapper::convert(const std::string& hanzi) {
    auto result = g2p->hanziToPinyin(
        hanzi,
        Pinyin::ManTone::Style::TONE,
        Pinyin::Error::Default,
        true,  // keep non-Hanzi
        true, // no whitespace split
        false   // enable polyphonic disambiguation
    );
    qDebug() << result.toStdStr(" ");
    return result.toStdStr(" ");
}
