#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>

#include "cppjieba/Jieba.hpp"

class Tokenizer
{
    cppjieba::Jieba jieba;
public:
    Tokenizer()
        : jieba("dict/jieba.dict.utf8",
                "dict/hmm_model.utf8",
                "dict/user.dict.utf8",
                "dict/idf.utf8",
                "dict/stop_words.utf8")
    {};
    std::vector<std::string> tokenize(const std::string &text);
};

#endif // TOKENIZER_H
