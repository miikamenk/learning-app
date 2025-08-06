#include "tokenizer.h"

std::vector<std::string> Tokenizer::tokenize(const std::string &text) {
    std::vector<std::string> words;
    jieba.Cut(text, words, true);
    return words;
}
