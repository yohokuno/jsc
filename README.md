JSC: Joint Souce Channel Model and Decoder
===

**JSC** is an implementation of joint source channel or joint n-gram model with monotonic decoder.

    A Joint Source-Channel Model for Machine Transliteration, Li Haizhou, Zhang Min, Su Jian.
    http://acl.ldc.upenn.edu/acl2004/main/pdf/121_pdf_2-col.pdf

It can be used for machine transliteration, Japanese kana-kanji conversion, Chinese pinyin input, English word segmentation or pronunciation inference.

Requirement
---

    marisa-trie version 0.2.0
    http://code.google.com/p/marisa-trie/

Install
---

    $ ./waf configure [--prefix=INSTALL_DIRECTORY]
    $ ./waf build
    $ sudo ./waf install

Usage
---

### jsc-decode

**jsc-decode** command convert source string from standard input to target string via joint source channel model.

    options:
    -d directory: specify data directory or prefix (default: ./)
    -f format: specify format (segment [default], plain, debug)
    -r romaji: specify romaji mode (both [default], on, off)
    -l: turn off sentence-beginning/ending label

### jsc-build

**jsc-build** command build model files in binary format from n-gram file in text format.

    options:
    -d directory: specify data directory or prefix (default: ./)
    -m model: specify model file name (default: ngram)
    -r: build reverse model

Japanese Kana Kanji Conversion
---

For Japanese Kana Kanji conversion, a model is provided at data/japanese directory. By default, both romaji and hiragana input are allowed.

    $ jsc-decode -d data/japanese/
    わたしのなまえはなかのです。
    わたし の 名前 は 中野 です 。
    arayurugenjitsuwosubetejibunnnohouhenejimagetanoda
    あらゆる 現実 を 全て 自分 の ほう へ ネジ 曲げ た の だ

Chinese Pinyin Input
---

For Chinese Pinyin input, a model is provided at data/chinese/ directory.

    $ jsc-decode -d data/chinese/
    woaiziranyuyanchuli
    我 爱 自然 语言 处理
    zhejianshitagegehaibuzhidaone
    这 件 事 她 哥哥 海部 知道 呢

English word segmentation / automatic capitalization
---

For English input, a model is provided at data/english/ directory.

    $ jsc-decode -d data/english/
    alicewasbeginningtogetverytiredofsittingbyhersisteronthebank
    Alice was beginning to get very tired of sitting by her sister on the bank
    istandheretodayhumbledbythetaskbeforeusgratefulforthetrustyouhavebestowedmindfulofthesacrificesbornebyourancestors
    I Stand here today humbled by the task before us grateful for the trust you have bestowed mindful of the sacrifices borne by our ancestors 

Mixed Input
---

For English/Japanese/Chinese mixed input, a model is provided at data/mixed/ directory. The language is detected automatically.

    $ jsc-decode -d data/mixed/
    thisisapen
    This is a pen
    kyouhayoitenkidesune
    今日 は 良い 天気 です ね
    woshizhongguoren
    我 是 中国 人
    thisishaochi!
    This is 好吃 !

File format
---

N-gram file should be SRILM format.

http://www.speech.sri.com/projects/srilm/

Target string and source string should be coupled with character '/'; e.g. "私/わたし"

