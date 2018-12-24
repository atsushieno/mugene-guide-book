

= MMLと音楽演奏データ

本章では、mugeneが前提とするMIDIについて、MIDI以前の技術も振り返りつつ、音楽の演奏データの構造がどのようになっているかを説明します。mugeneそのものについては以降の章で説明していきます。

== 歴史的な背景

MMLという言語は特殊なジャンルですが、その特殊なジャンルの中には数多のMML文法が存在しています@<fn>{mml_variants}。MMLが隆盛した20世紀の日本には、PC-88/98、X68000、FM-TOWNS、MSXといった、現在のPCとは異なるPCが多数存在しており、動作しているOSもWindowsではない、MS-DOSなどシングルタスクで単一のアプリケーションが全画面を専有するものが主流でした。この時代の音源は（有る場合は）主にPCに付属するFM音源であり、ソフトウェアでPCM音声を合成して楽曲を演奏する処理速度はPCにはありませんでした（MicrosoftがWindows 98からMicrosoft GS Wavetable Synthを搭載しています@<fn>{fpd}）。

//footnote[mml_variants][N88-BASIC、MXDRV、Z-MUSIC、MUAP98、PMD、FMP、mml2mid、テキスト音楽サクラ、FIMML、TSS、SiON…]
//footnote[fpd][筆者の知識の範囲では、PC-98上で動作するFPDが、MIDIファイルからの変換をサポートした最初の多重音声PCM音源ドライバです。 http://www.vector.co.jp/soft/dos/art/se012450.html]

当時の日本のFM音源は、たいがいYamahaのYM-xxxxという品番のものでしたが@<fn>{soundblaster}、その種類もひとつではなかったので、環境によってさまざまな「音源ドライバ」が存在していました。音源ドライバは、演奏データを受け取って、音源の仕様に基づいて再生したり停止したりします。音源の性能（同時発音数やそもそもFMオシレーターの数など）は基板によってことなり、また当時のプログラミング環境ではタイマーの実装も環境や音源に依存しており@<fn>{timer_a_b}、高度なタイマーを処理するパワーも無かったので、音源ドライバはそれぞれが独自の発展を遂げており、データフォーマットはおろかデータの内容も多様でした。MMLは最初からバベルの塔が崩壊した世界で誕生したわけです。

//footnote[soundblaster][海外ではSoundBlasterが主流であったといえそうです。]
//footnote[timer_a_b][たとえばYamahaのYM-2203にはTimer-AやTimer-BというレジスタにハードウェアI/O命令でテンポを設定する必要があり、これはマイクロ秒のような単位で設定するものではありませんでした。]

このような状況の中で@<fn>{eras}、音源や音楽ソフト間の共通規格を定めたのがMIDIであり、楽器の定義はGM（General MIDI）音源、ファイルフォーマットにはSMF（standard MIDI format）など、いくつかの仕様が作成されました。このMIDIをターゲットとするMMLコンパイラもいくつか開発されましたが@<fn>{mml-midi}、MIDI音源やSMFフォーマットがFM音源やそのドライバの独自フォーマットに取って代わるまでは長い時間がかかり、その間にMMLによる楽曲制作はマイノリティとなっていきました。

//footnote[eras][ここで説明している時代背景は主に80年代後半〜90年代前半ですが、MIDI規格は1983年には成立しているものです。MIDIが解決しようとする課題そのものは、ここで説明している時代以前から存在していました。]
//footnote[mml-midi][筆者の知る限りでは、mml2midなどが最初期のもので、現在もっとも有名なのは テキスト音楽サクラ でしょうか。筆者はMUCというMMLコンパイラを使っていました。]

MIDIファイルをMMLコンパイラで作成するのは、主に過去にMMLによる楽曲制作を行っていて、それが最も手に馴染むという人や、制作と演奏でツールを使い分けたいといった嗜好の人が大半でしょう。


== MIDIと音楽データの構造

前述の通り、音楽演奏データはドライバによって異なるものです。最もプリミティブな例を挙げると、任天堂のファミリーコンピューターで使用されているFc音源やそのエミュレーション環境で演奏するデータを作成するツールとしてMCKというMMLコンパイラがありますが、MCKが生成するのはファミリーコンピューターのCPUである6502上で動作する「プログラム」です。コンピューターに接続された音源は、CPUにとっては外部デバイスであり、外部デバイスへのI/O命令を通じて制御されるので、演奏データがプログラムそのものである、というのはある意味もっとも柔軟な（プログラマー向けにいえば「低レベルな」）データフォーマットであるといえます。

こういうフォーマットで生成されたデータは、コンピューターの命令セットやデバイスが変わってしまったときに、全く使えないものになってしまいます。そこまで極端ではなくても、FM音源ではドライバが乱立し命令セットもファイルフォーマットも環境依存であったため、ある程度統一して複数の環境で利用できるよう、標準音源の提案と楽器とやり取りする命令、演奏データのフォーマットなどを規格として定めたのが、先にも言及したMIDIです。MIDIによって、音楽データの構造がある程度シンプルになりました。

音楽演奏データにはどのような情報があるでしょうか。ここではデジタル化以前の典型的なフォーマットである「楽譜」を引き合いに出して、いくつかの重要な要素を取り出していきます。

(1) @<b>{パート}: ひとつの完成した楽曲は、通常は複数のパートで構成されます（ソリスト用の曲もあります）。これはMIDIにおいては「チャンネル」や「トラック」という概念が割り当てられます。楽譜におけるパートには通常はひとつの楽器が割り当てられますが、MIDIのチャンネルは途中で楽器を切り替えることができます。

(2) @<b>{楽器}: ピアノ、ギター、ドラム、ヴァイオリン、人間の声など、さまざまな楽器があります。MIDI規格では、128種類の音源が規定されています。楽器のことをプログラムと呼び（一般的な意味での実行プログラムとは意味が違います）、チャンネルで使用する楽器を指定する命令を@<b>{プログラム・チェンジ}といいます。

(3) @<b>{音符}: どんな楽譜にも必ずあるのが、音階と音の長さを示す音符です。音符は、MIDIではノート命令、具体的には@<b>{ノートオン}（音を鳴らす）と@<b>{ノートオフ}（音を止める）という命令で表します。音階は0〜127で表し、大きい数値であれば高い音となります。音の「長さ」（以降は「音長」と書きます）は、命令と次の命令の間の待機時間として表します（全てのMIDI命令のメッセージには「次の命令までの待機時間」が付きます）。ちなみに「休符」に対応する「命令」はありません。ノートオフから次のノートオンまでの待機時間が、休符の分だけ加算されるだけです。

(4) @<b>{テンポ}: 楽譜ではあいまいにlargo, andante, allegroなどと指定されるのをよく見たと思いますが、コンピューターは具体的な数値でないと理解できません。MIDIではBPM（beat per minutes、1分あたりの4分音符の数）と4分音符ひとつにかかるマイクロ秒単位の時間の組み合わせでテンポを表現します。

(5) その他の装飾記号: フォルテやピアニッシモといった強弱、スラーによるなめらかな音符の接続など、楽譜にはさまざまな表現があります。MIDI規格では、ノートの強弱にはベロシティ、音程のなめらかな移動にはピッチベンドなど、いくつかのパラメーターが用意されています。ベロシティとピッチベンド以外は、だいたいエフェクトであり、これらは@<b>{コントロール・チェンジ}と呼ばれる命令の引数で、さらに細かく種類を指定して制御します。

=== チャンネルとトラック

1つのチャンネルには1つの時点では1つの「楽器」を演奏することしかできません。MIDIでは16チャンネルが利用可能となっています。16チャンネル以上で楽器を同時に使用したい場合は、SMF（標準MIDIフォーマット）のデータ構造では扱わず、複数のMIDIデバイスを同時に使うことで対応します（とはいっても、そこまでの規模の楽曲を、無理に標準MIDIの範囲で制作する必要はないでしょう）。

また、16チャンネルのうち、10番目のチャンネルはドラムパート用に予約されており、このチャンネルのノート命令では、ノートごとに異なるリズム楽器の音が割り当てられています。例えば、ドの音を鳴らしたらキックドラムが出たとしても、レの音はそのキックドラムの音階が違うものが鳴るわけではなく、スネアドラムの音が出ることになります。

なお、MIDI規格では、1つのチャネルで複数の音階のノート（音）を発声できることになっています。FM音源では、1つのチャネルでは1音しか発声できず@<fn>{fm_poly}、和音を鳴らそうと思ったら、それだけで複数チャネルを占有することになります。FM音源はたいがい3チャンネルとか6チャンネルとか8チャンネルくらいしかないので、本格的なポップスなどの楽曲を制作するのは無理があるでしょう（FM音源で本当にそこまでやりたい場合は、いったんPCMで録音してからミックスします）。

//footnote[fm_poly][これは論理的な必然ではありませんが、通常はFM音源の「チャンネル」に割り当てられる音色はオシレーターを組み合わせて表現され、これを分解して別々の音階を同時に鳴らすことは想定されません。FM音源によっては、一部チャネルのオシレーターを分解して独立した音を出せたものもありますが、特殊な例です。]

=== 標準楽器

MIDIでは、その仕様の一部であるGeneral MIDI楽器として（GM音源ともいいます）、128種類の楽器とドラム用の楽器がいくつか規定されています。楽器というのはクリエイティブなものであり、アーティストが独自の楽器を作ることもよくありますが、GMはそういった表現には向いていません@<fn>{midi_ctrl}。また同じ「ギター」でも、メーカーや演奏する人によって出る音は大きく変わるのが一般的でしょう。GMは「標準規格」であり、標準規格で表現できる音楽データは最大公約数的なものでしかありません。細かい表現を追求するなら、標準規格を外れたところで表現するのが良いでしょうし、本格的なデジタル楽曲制作ではそれが一般的です。

RolandはGMを拡張した音源規格としてGSを、Yamahaは同様にXGを、それぞれ定めていますが、ほぼ同様の議論が当てはまります。

//footnote[midi_ctrl][楽器の制御をMIDIメッセージで行うということは、今でも一般的に行われています。]

さいわい、MIDI仕様の策定者も、ひとつのGM音色には複数のバリエーションがあるべきであることを理解していたので、MIDIで音色を指定する際には、プログラムとは別に<b>{バンク}とよばれるバリエーションの指定も可能になっています。MIDI音源、とくにハードウェア音源では、音色はプログラム・チェンジとバンク・セレクトを組み合わせた128音以上の選択肢から選ぶのが一般的です。筆者が大枚をはたいて購入したRoland Integra-7というハードウェアMIDI音源には6000音色分のデータが搭載されています@<fn>{num-tones}。

//footnote[num-tones][実際には、ドラムパート用の音色はリズム音色セット1つにつき20音以上含まれていることになるので、それを考慮した上で数字を見るべきです。]

また、MIDIの範囲を越える楽器でも、VSTi（Virtual Studio Technology - instrument）などの技術に基づくソフトウェアシンセであれば、DAWなどを使えば、音色以外の演奏情報はMIDIやそれに準ずる命令集合を用いてMIDIのような感覚で音楽制作できるでしょう。


== 音楽制作・再生環境

本書で解説するmugeneはMMLコンパイラです。MMLコンパイラは単なるコンソールツールで完結するので、MS-DOSなどの環境の頃からフリーソフトなどで比較的お手軽に開発されてきたものです（ただし、通常は音源ドライバを伴うもので、その開発がメインであったともいえるでしょう）。この意味では、mugeneは古典的なツールです。もっとも、実装言語がC#であり実行環境がMonoあるいは.NET Frameworkであるため、古いPC環境では動作しないでしょう。

使い方も簡単で、コンパイラツールの後にMMLファイル名を指定して実行すると演奏データファイルが出力されるだけ、というのが一般的です。（例: @<img>{mugene-console-use}）

//image[mugene-console-use][mugeneをコンソールで実行]

MIDIが音源として使われるようになった時代にはDAWによる制作が一般的になってきて、MIDIをサポートするMMLコンパイラというのはやや時代のギャップがありますが、プログラマブルに音楽制作を行いたい人にとっては、MMLコンパイラはなお有力な選択肢であったといえるでしょう。ただ、さすがに2018年現在もMMLで楽曲を制作する人は多くありません。

mugeneはSMFを生成するコンパイラですが、FM音源とMIDI音源以外の世界にも音楽データのフォーマットは存在しています。有名なところではMODというPCMデータを用いる音楽データがあります。

mugeneは今のところSMFをターゲットにしており、これ以外のチャンネルを対象にすることは今のところ考えていません。全く新しい音源やドライバをターゲットにする場合は、新しいコンパイラを書いたほうが良いだろうと考えています。形式上SMFではないがSMF互換の命令セットやデータを出力すれば良いというものに関しては、この限りではありません。たとえば、Vocaloid2でサポートしていたフォーマット.vsqは、その実態は単なるSMFに則ってメタデータなどを拡張したものであって、mugeneを少し拡張するだけで生成できるものでした。そのため、mugeneではVocaloid2をサポートしています@<fn>{vocaloid4}。

//footnote[vocaloid4][Vocaloidは既にバージョン4であり、.vsqサポートは古いものです。]


== 典型的なMMLの特徴

MMLは、PC上のCPU、メモリ、ディスクなどのリソースが限られていた時代に、その強い制約を受けながら発展してきた言語生態系です。21世紀になるとXMLのような冗長な記述が盛んになりましたが@<fn>{music_xml}、MMLは1行のテキストにノートなどの各種命令を記述していくスタイルです。もちろん、数小節書いてテキストエディタの横幅いっぱいになってきたら、行を改めて書くことになります（可能です）。（例: @<img>{mml_sshot}）

//image[mml_sshot][mugene MMLの編集画面(vim)]

また、一般的なMMLは楽曲を制作するために用いられる言語であり、ノートひとつにつき1行、のような記述には馴染みません。GUIベースで音楽制作する環境のいち類型としてTrackerというジャンルのツールがありますが、Trackerは1つのノート命令で1行が消費されます（複数トラックが横に並ぶ構成でもあり、また演奏中は自動スクロールで追尾するのが一般的なので、MMLとは根本的にスタイルが違います）。

//footnote[music_xml][MusicXMLやMusic Markup Languageといった言語はXMLを採用しています。これで作曲したいか、という観点で見ておくのも良いでしょう…]

音楽を構成するために記述する命令が簡潔に書けるのは、命令がほとんど1文字の識別子で提供されているからであり、すなわち一見すると何が記述されているのか把握しづらい、という問題があります。一方で、MMLに慣れていると、命令語の短さは、テキストエディタのビューで大まかに楽曲の流れがある程度把握できるという利点になります。

MML文法にも、筆者が記憶している中には、Cに近いinclude構文をサポートしたり、パートごとに@<tt>{{}〜@<tt>{\}}で囲ったりするものもありましたが、これくらいの冗長さであれば現代的には十分に受け容れられるものでしょう。

mugeneのように、楽曲を記述できる程度の機能をサポートするMML文法では、「マクロ」を定義して、あるパターンに沿ったMMLを簡潔に記述できるようになっているのが一般的です（ごく簡単なMMLコンパイラには存在しない機能です）。プログラミング言語における関数のようなものですが、そこまで高度なものではないのが一般的です。引数もサポートしたりしなかったりです。

MML文法によっては、大文字・小文字を区別しないものもあります。もっとも、MMLにおける命令は通常はかなり短いものであり、ある程度の規模の音楽制作に耐えることを想定しているツールでは、これらを区別する文法のほうが一般的でしょう。文法によっては、「大文字識別子のマクロは2文字以上」のようなルールを導入しているものもあったりします。

=== 典型的なMML命令

典型的なMML命令には、次のようなものがあります。（@<table>{typical_mml_operators}）

//table[typical_mml_operators][典型的なMML表]{
命令	意味	命令	意味
----------------------------------
cdefgab	ノート	r	休符
l	デフォルト音長指定	q	発音時間（ゲートタイム）指定
o	オクターブ指定	< >	オクターブ相対指定
v	音量指定	( )	音量相対指定
@	音色指定	[ 〜 ]n	繰り返し（n回）
t	テンポ指定	B	ピッチベンド指定
//}

これらはあくまで例であって、実際の文法次第です。

オクターブ相対指定で@<tt>{<}と@<tt>{>}のどちらが上げるか・下げるかはMML方言によって大きく二分するところです。mugeneのデフォルトは@<tt>{<}が1オクターブ下げるほうです。




