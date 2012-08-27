JSC: Joint Souce Channel Model and Decoder
===

**JSC** is an implementation of joint source channel or joint n-gram model with monotonic decoder.

    A Joint Source-Channel Model for Machine Transliteration, Li Haizhou, Zhang Min, Su Jian.
    http://acl.ldc.upenn.edu/acl2004/main/pdf/121_pdf_2-col.pdf

It can be used for machine transliteration, Japanese kana-kanji conversion, Chinese pinyin input or pronunciation inference.

Requirement
---

    marisa-trie version 0.2.0
    http://code.google.com/p/marisa-trie/

Install
---

    ./waf configure [--prefix=INSTALL_DIRECTORY]
    ./waf build
    sudo waf install

Usage
---

### jsc-decode

**jsc-decode** command convert source string from standard input to target string via joint source channel model.

    -d directory: specify data directory or prefix
    -f format: specify format (plain [default] or debug)
    -l: turn off sentence-beginning/ending label

### jsc-build

**jsc-build** command build model files in binary format from n-gram file in text format. The file name of n-gram should be "ngram".

    -d directory: specify data directory or prefix


Japanese Kana Kanji Conversion
---

For Japanese Kana Kanji conversion, a default model is provided at data/ directory 

    jsc-decode
    Now loading model...
    Input:
    わたしのなまえはなかのです。
    わたしの名前は中野です。
    きょうはよいてんきですね。
    今日は良い天気ですね。
    あらゆるげんじつをすべてじぶんのほうへねじまげたのだ。
    あらゆる現実をすべて自分のほうへネジ曲げたのだ。

Chinese Pinyin Input
---

For Chinese Pinyin input, a default model is provided at chinese/ directory 

    jsc-decode -d chinese/
    Now loading model...
    Input:
    nihao
    你好
    woaini
    我爱你
    ziranyuyanchuli
    自然语言处理

File format
---

N-gram file should be SRILM format.
http://www.speech.sri.com/projects/srilm/

Target string and source string should be coupled with character '/'; e.g. "私/わたし"

