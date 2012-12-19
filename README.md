JSC: Joint Souce Channel Model and Decoder
===

**JSC** is an implementation of joint source channel or joint n-gram model with monotonic decoder.

    A Joint Source-Channel Model for Machine Transliteration, Li Haizhou, Zhang Min, Su Jian.
    http://acl.ldc.upenn.edu/acl2004/main/pdf/121_pdf_2-col.pdf

It can be used for machine transliteration, Japanese kana-kanji conversion, Chinese pinyin input, English word segmentation or pronunciation inference.

Requirement
---

JSC requires Unix, gcc, python, and marisa-trie. If you want to use RPC server, you also need libevent.

    marisa-trie 0.2.0 or later
    http://code.google.com/p/marisa-trie/

    libevent 2.0 or later
    http://libevent.org/

Install
---

To install JSC, type these commands into your console.

    $ ./waf configure [--prefix=INSTALL_DIRECTORY]
    $ ./waf build
    $ sudo ./waf install

Usage
---

### jsc-decode

**jsc-decode** command convert source string into target string via joint source channel model.
You can provide queries through standard input line by line.

    options:
    -d directory: specify data directory or prefix (default: ./)
    -f format: specify format (segment [default], plain, debug)
    -t table: specify table [romaji] mode (both [default], on, off)
    -l: turn off sentence-beginning/ending label

### jsc-build

**jsc-build** command build model files in binary format from n-gram file in text format.

    options:
    -d directory: specify data directory or prefix (default: ./)
    -m model: specify model file name (default: ngram)
    -t trie_num: specify trie number in marisa-trie (default: 3)
    -r: build reverse model

### jsc-server

**jsc-server** command provides RPC server via simple TCP protocol.
You can provide queries through telnet command line by line.

    options:
    -d directory: specify data directory or prefix (default: ./)
    -f format: specify format (segment [default], plain, debug)
    -t table: specify table [romaji] mode (both [default], on, off)
    -p port: specify port number (default: 40714)
    -l: turn off sentence-beginning/ending label

Sample Applications
---

### Japanese Kana Kanji Conversion

For Japanese Kana Kanji conversion, a model is provided at data/japanese directory. By default, both romaji and hiragana input are allowed.

    $ ./build/jsc-decode -d data/japanese/
    わたしのなまえはなかのです。
    わたし の 名前 は 中野 です 。
    arayurugenjitsuwosubetejibunnnohouhenejimagetanoda
    あらゆる 現実 を 全て 自分 の ほう へ ネジ 曲げ た の だ

### Japanese Pronunciation Inference

For Japanese pronunciation inference, a model is provided at data/japanese-reverse directory.

    $ ./build/jsc-decode -d data/japanese-reverse/
    魔理沙は大変なものを盗んでいきました
    ま りさ は たいへん な もの を ぬす ん で い き ま し た

### Chinese Pinyin Input

For Chinese Pinyin input, a model is provided at data/chinese/ directory.

    $ ./build/jsc-decode -d data/chinese/
    woaiziranyuyanchuli
    我 爱 自然 语言 处理
    zhejianshitagegehaibuzhidaone
    这 件 事 她 哥哥 海部 知道 呢

### Chinese Hanzi-to-Pinyin Conversion

For Chinese Hanzi-to-Pinyin Conversion, a model is provided at data/chinese-reverse/ directory.

    $ ./build/jsc-decode -d data/chinese-reverse/
    汉字拼音转换
    hanzi pinyin zhuanhuan 

### English word segmentation / automatic capitalization

For English input, a model is provided at data/english/ directory.

    $ ./build/jsc-decode -d data/english/
    alicewasbeginningtogetverytiredofsittingbyhersisteronthebank
    Alice was beginning to get very tired of sitting by her sister on the bank
    istandheretodayhumbledbythetaskbeforeusgratefulforthetrustyouhavebestowedmindfulofthesacrificesbornebyourancestors
    I Stand here today humbled by the task before us grateful for the trust you have bestowed mindful of the sacrifices borne by our ancestors 

### Mixed Input

For English/Japanese/Chinese mixed input, a model is provided at data/mixed/ directory. The language is detected automatically.

    $ ./build/jsc-decode -d data/mixed/
    thisisapencil
    This is a pencil
    kyouhayoitenkidesune
    今日 は 良い 天気 です ね
    woshizhongguoren
    我 是 中国 人
    thisistotemohaochi!
    This is とても 好吃 ！

Data Structure
---

### Directories

Top directory contains these files and directories:

    README.md   this file
    build/      built by waf automatically
    data/       model files
    src/        source and header files for C++
    tools/      command tools by C++
    waf         waf build script
    wscript     waf settings

### File format

N-gram file should be SRILM format.

    http://www.speech.sri.com/projects/srilm/

Target string and source string should be coupled with character '/'; e.g. "私/わたし"

Reference
---

### Accuracy

<table>
<tr><th>Language</th><th>F-score</th><th>Size</th></tr>
<tr><td>Japanese</td><td>0.937</td><td>10MB</td></tr>
<tr><td>Chinese</td><td>0.895</td><td>9MB</td></tr>
<tr><td>English</td><td>not ready</td><td>9MB</td></tr>
<tr><td>Mixed</td><td>not ready</td><td>27MB</td></tr>
</table>

### Paper

Please refer this paper if you need.

    Yoh Okuno and Shinsuke Mori, An Ensemble Model of Word-based and Character-based Models for Japanese and Chinese Input Method, Workshop on Advances in Text Input Methods, 2012.
    http://yoh.okuno.name/pdf/wtim2012.pdf

