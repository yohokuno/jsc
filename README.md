JSC Decoder: Joint Souce Channel Decoder
===

**JSC Decoder** is a monotonic decoder for joint source channel or joint n-gram model.

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
--

Default model is provided at data/ directory for Japanese Kana Kanji conversion.

    jsc-decode
    Now loading model...
    Input:
    わたしのなまえはなかのです。
    わたしの名前は中野です。
    きょうはよいてんきですね。
    今日は良い天気ですね。

File format
---

N-gram file should be SRILM format.
http://www.speech.sri.com/projects/srilm/

Target string and source string should be coupled with character '/'.

    head test-data/ngram

    \data\
    ngram 1=15
    ngram 2=19

    \1-grams:
    -0.845098       </s>
    -99     <s>     -0.5351132
    -1.322219       、/、   -0.2575643
    -1.322219       あり/あり       -0.2798407

