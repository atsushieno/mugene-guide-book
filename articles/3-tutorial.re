
= mugeneチュートリアル

== mugeneコンパイラの使い方

mugene MMLコンパイラはC#で書かれた.NETのコンソール・ツール（mugene.exe）です。筆者は主にUbuntu Linux上で開発しているので、monoを使って実行しています。Windowsではそのままmugene.exeを実行すればよいでしょう。

mugeneは（2018年4月時点では）githubとnugetでのみ公開しています。いずれ整備しますが、現時点では次のように必要なファイルを全て集めて実行するのが楽でしょう。

//emlist{
$ nuget install mugene
(コンソール出力略)
$ cp managed-midi.1.1.0/lib/net45/* .
$ cp -R mugene.1.1.4/lib/net45/* .
$ echo "1  t100 @0 l4 cdefedc2  efgagfe2  crcrcrcr  l8ccddeefferdrc2" > frog.mugene
$ mono mugene.exe frog.mugene
Written SMF file ... frog.mid
$ ls frog*
frog.mid  frog.mml
//}

このMMLは冒頭に出した かえるのうた です。コンパイル結果として@<tt>{frog.mid}が生成されていることがわかります。内容を見るとこうなっています。（全部載せるまでもないのですが、これくらいのサイズであることを示す意図で載せています。）

//emlist{
$ hexdump frog.mid
0000000 544d 6468 0000 0600 0100 0100 3000 544d
0000010 6b72 0000 fd00 ff00 0351 2709 00c0 00b0
0000020 0000 0020 c000 0000 3090 3064 3080 0000
0000030 3290 3064 3280 0000 3490 3064 3480 0000
0000040 3590 3064 3580 0000 3490 3064 3480 0000
0000050 3290 3064 3280 0000 3090 6064 3080 0000
0000060 3290 3064 3280 0000 3490 3064 3480 0000
0000070 3590 3064 3580 0000 3790 3064 3780 0000
0000080 3590 3064 3580 0000 3490 3064 3480 0000
0000090 3290 6064 3280 0000 3090 3064 3080 3000
00000a0 3090 3064 3080 3000 3090 3064 3080 3000
00000b0 3090 3064 3080 3000 3090 1864 3080 0000
00000c0 3090 1864 3080 0000 3290 1864 3280 0000
00000d0 3290 1864 3280 0000 3490 1864 3480 0000
00000e0 3490 1864 3480 0000 3590 1864 3580 0000
00000f0 3590 1864 3580 0000 3490 1864 3480 1800
0000100 3290 1864 3280 1800 3090 6064 3080 0000
0000110 2fff 0000
//}

冒頭の@<tt>{MThd}やその後ろにある@<tt>{MTrk}といったチャンクを含み、0x1Fバイト目からの@<tt>{00 90 32 64}はノートオン、その直後の@<tt>{30 80 32 00}は30tick後にノートオフ…といった、標準的なMIDIファイルのフォーマットになっています。

生成された@<tt>{.mid}ファイルは、（おそらくどのOS環境でも）選択して関連付けられたアクションを実行すると、相応するメディアプレイヤーが開いて再生が始まるでしょう。筆者が作りかけ@<fn>{xmdsp_ongoing}のヴィジュアルMIDIプレイヤーxmdsp@<fn>{xmdsp-repo}も雑に宣伝しておきます。

//image[xmdsp][Xmdsp]
//footnote[xmdsp_ongoing][筆者がMMDSPみたいなプレイヤーを作ってみたかっただけなのですが、対象がMIDIだとスペアナなどを表示してもほぼ意味がなく、空白を埋めるアイディアも特に無いので、中途半端な状態のままになっています。]
//footnote[xmdsp-repo][https://github.com/atsushieno/xmdsp]


== パート構成をトラックにマッピングする

ここまでのサンプルでは かえるのうた しか扱っていませんでした。本書は かえるのうた をMMLで打ち込めるようになるためのものではありません。まともな楽曲を制作できるようになるためには、あとどのような知識が必要なのでしょうか?　この疑問に対してどう答えるのが正しいのか、あるいは妥当なのかは難しいところですが、まず意識しておくべきことは、われわれが扱っているのはデジタル音楽であるということと、MIDIはさまざまな音源の最大公約数であり、その表現力には制約が（それも、かなり大きな制約が）あるということです。期待値を下げて臨みましょう。

MMLでMIDI楽曲を制作するには、次に挙げる作業が必要になるでしょう。

 * 楽曲のパートをトラックに割り当てる
 * 各トラックごとに音符を載せていく
 * タイトルや作曲者情報を記述する

=== パート構成をトラックにマッピングする

多くの音楽は、複数のパートから構成される、と前章で説明しました。これをもう少し掘り下げてみましょう。たとえば、ロックのパート構成は、典型的には次のようなものが考えられるでしょう（これらが全て揃っていないこともよくあります）。

 * ヴォーカル
 * ギター
 * ベース
 * ドラム
 * キーボード

GM音源でヴォーカルを打ち込むのは無理で、この部分は（もし本当に必要なら）Vocaloidのようなソフトウェアを使って解決することになります。その他のパートはGM音源で実現可能です。ギターはプログラムNo.24〜31、ベースはNo.32〜39、ドラムは10チャンネルのリズムトラック、キーボードはさまざまな音色で実現可能です。No.0@<fn>{program_index}のピアノやNo.16のオルガン、No.48のストリングス、No.61のブラスセクションなどは典型的な音色で使いやすいでしょう。これらのそれぞれにトラックを割り当てます。ギターとキーボードはポリフォニック（複数のノートを同時に発声するモード）であり、ベースはモノフォニック（単音）です。（ドラムの打ち込みは特殊なのでここでは例示しません。）

//footnote[program_index][この説明で分かる読者もいるかもしれませんが、本書ではプログラム・チェンジの番号を1〜128ではなく、0〜127で記述します。これはmugeneにおける記法と合致するものです。]

この構成はモダンな音楽の大半で通用するでしょう。オーケストラ楽曲ではもっと多くのパートが必要になります。テクノではもっとクリエイティブな音色を使うことが多いでしょう。

多くの場合は、1パートにつき、1トラックが存在していれば大丈夫ですが、ドラムに関しては、キックドラム、スネアドラム、ハイハット、クラッシュシンバルなど、パートを複数使用したほうが効率的な打ち込みが可能でしょう。また、トラックをひとつ使用して、どのチャンネルにも属さない命令（たとえばテンポ変更など）をまとめておくと、後から読みやすいでしょう@<fn>{tempo_change_bugs}。

//footnote[tempo_change_bugs][テンポ変更や拍子変更を通常トラックに分散して記述していると、後で思い通りの音楽演奏になっていない時に、原因箇所の特定に時間がかかります。]

=== トラックに演奏命令を記述する

トラックが決まったら、それぞれに必要なノート命令などを、他の命令（ボリューム、パン、ピッチベンド、リバーブなど）を使って表現を調整しながら打ち込んでいくことになります。mugene MMLのトラックの内容は、行頭にトラック番号を書いて、1文字空白（spaceかtab）を入れて、あとはMML命令を書くだけです。ここからは、さまざまな命令を使いながら学んでいく必要があるでしょう。

打ち込みの進め方は制作者次第です。メロディから書き始めるのもよし、キーボードでコードを作り始めるもよし@<fn>{chord_driven}、リズムパターンから入っても良いでしょう。特に最初のうちは「出来ること」を増やすのも重要です。また全パートを少しずつ進めるのか、1パートだけ先行するのか、といった部分も人それぞれです。

//footnote[chord_driven][楽曲のコード進行を決めるのは、たいがいはキーボードではなくベースになるでしょうから、実際のコードはベースを打ち込み始めてから決まることになるでしょうが、頭に浮かんだ音楽を実装する意味ではどこから始めても良いでしょう。]

mugeneで制作していくのであれば、その命令を知っていく必要もあるでしょうが、読者にあまり深入りしてもらうほど息の長いツールにするつもりもないので、どちらかといえばそれぞれのMML命令がMIDIのどのような機能に対応するかを理解しながら読み解いてもらいたいと考えています。そうすれば、他のMMLツールやDAWなどを使った打ち込みにシフトしていっても役に立つ知識となることでしょう。

=== MIDIメッセージ

mugeneの命令は、最終的には何らかのかたちで出力されるMIDIメッセージに反映されることになります。MIDIメッセージには次の8種類があります。（@<table>{typical_midi_messages}）

//table[typical_midi_messages][MIDIメッセージの種類]{
コード	引数	意味	mugene MML
----------------------------------
80H	[キー]、[ベロシティ]	ノートオフ
90H	[キー]、[ベロシティ]	ノートオン	n, cdefgab
A0H	[キー]、[値]	PAf	PAF
B0H	[No.]、[値]	CC	CC
C0H	[No.]	プログラムチェンジ	@
D0H	[値]	CAf	CAF
E0H	[値2バイト]	ピッチベンドチェンジ	B
F0H	[可変長]	システムメッセージ
//}

コードは16進数です。0となっている部分は、実際のメッセージでは対象チャンネルが0〜Fで指定されます。この基本構造のため、標準MIDIのチャンネルは必ず16までとなります。

mugene MMLでは、トラックごとに対象チャンネルを@<tt>{CH}命令で指定する必要があります。チャンネルが指定されなければ、全てチャンネル No.0に出力されます@<fn>{sample_mml_ch}。トラックとチャンネルは違うものを指定できますし、実際複数トラックで操作するチャンネルは0〜15以外で割り当てたほうが無難でしょう。

//footnote[sample_mml_ch][サンプルで何度か登場している かえるのうた は、CH命令を使っていないので、全てチャンネルNo.0に出力されています。]

ノートオンとノートオフは既に何度か登場しています。ノート命令については、単純なMIDI命令へのマッピングではなく、「ドレミファソラシ」に対応する@<tt>{c d e f g a b}で記述するのが一般的です（これらはドイツ語由来の記法です）。シャープは@<tt>{+}、フラットは@<tt>{-}を付加します。音階名だけではキーを特定できないので、MML上では「現在のオクターブ」をトラックごとに記憶しておいて、@<tt>{c d e f g a b}のいずれかが登場するたびに、オクターブと音階名から引数となるキー（0〜127）を計算します。オクターブは@<tt>{o}命令で指定し、@<tt>{>}や@<tt>{<}で1つずつ上げ下げできます。ちなみに@<tt>{n}でこれらを無視して80H/90Hに即値を渡すこともできます。

MIDIメッセージ上はノートオンとノートオフは別々ですが、MML上ではノート命令に音長を指定するだけです。SMFファイル上は、ノートオンが終わったら音長のぶんだけ待機してからノートオフ命令を送るかたちです。

音長は@<tt>{l}命令でデフォルト値を指定でき、デフォルト音長のノート命令では音長を省略できます。また、@<tt>{.}（ドット）を音長の後ろにつけると付点となります。また、@<tt>{+}を使ってc2+8のような表記も可能です。ちなみに全ての音長は（デフォルトで）192の約数として指定される必要があります。192分音符、96分音符、64分音符は表現できますが、例えば100分音符を指定すると仕上がりが予測不能な（ずれた）曲になります。また、@<tt>{#}を使って、c192と書く代わりにc#1と書くことも可能です。

音長にはひとつ特殊ルールがあって、音長に0が指定された場合は、その次のノート命令で指定される音長が適用されます。これは和音を演奏する時に便利で、たとえば@<tt>{c0e0g1}と記述すると、ド・ミ・ソの和音が全音符で展開されます@<fn>{gate_and_step}

//footnote[gate_and_step][ノートの長さと次の命令までの間隔は、ノート命令の引数で別々に指定できるのですが、これは高度なトピックなので以降の章で扱います。]

また、ノート命令には、ベロシティという音量指定を追加することもできます。MIDIメッセージ80H@<tt>{noteoff-velocity}と90Hではベロシティが必ず指定されますが、ノート命令に音量を指定しなかった場合は、デフォルトベロシティとして@<tt>{v}命令で指定してあった値が使われます。

//footnote[noteoff-velocity][ノートオフ＝消音になぜ音量パラメーターがあるのかというと、音が「どのように」消えるかを制御するパラメーターとして機能しているのです。0を指定すると唐突に消えて、大きな値を指定すると減衰音を残しつつ消える、といった制御が可能になります。これは音色依存、楽器依存の機能です。]

ここまでわかったところで、（しつこいですが）かえるのうた を見てみましょう。だいぶ読めるようになったのではないでしょうか。

//emlist{
1  t100 @0 l4 cdefedc2  efgagfe2  crcrcrcr  l8ccddeefferdrc2
//}

プログラムチェンジも既に説明した音色指定命令です。ピッチベンドは音程を半音よりもはるかに小さい単位で変化させる命令です。典型的なシンセサイザーでは、鍵盤の横にあるスライダーでこの命令が使われています。PAf（ポリフォニックキープレッシャー）とCAf（チャンネルプレッシャー）は鍵盤を押した時の深さをデジタル化したようなもの@<fn>{caf_dumper}で、楽器以外ではAppleのMagic MouseやiPhoneのタッチ操作における深さセンサーのようなものに近いです。PAfは鍵盤単位、CAfはチャンネル単位で、強弱のパラメーターを伴います。システムメッセージには、音源固有のメッセージをやり取りするためのSysEx（システムエクスクルーシブ）メッセージなどが含まれます。

//footnote[caf_dumper][CAfはピアノのダンパーペダルのようなものとも言えますが、これに相当するのはコントロールチェンジのダンパーペダルなので、話をややこしくしないために例としては用いません。]

=== コントロール・チェンジ

最後に残っているのはCC（コントロールチェンジ）ですが、ここにはさまざまなチャンネル別パラメータ制御命令があります。これは大量にあるので、いくつかよく使われるものだけ取り上げておきます。コードを覚える意味はほぼありません。mugeneの@<tt>{default-macro.mml}では、これらを簡単に指定できる命令を用意しています。（@<table>{control_changes}）

//table[control_changes][コントロールチェンジ一覧（抄）]{
コード	意味	mugene MML
----------------------------------
00H	バンク・セレクト		@
01H	モジュレーション	M
06H	DTE MSB	DTE, DTEM
07H	ボリューム	V
0AH	パン	P
0BH	エクスプレッション	E
26H	DTE LSB	DTE, DTEL
40H	ダンパーペダル（ホールド）	H
5BH	リバーブ（など）	RSD
5DH	コーラス（など）	CSD
5EH	ディレイ（など）	DSD
62H	NRPN LSB	NRPN, NRPNL
63H	NRPN MSB	NRPN, NRPNM
64H	RPN LSB	RPN, RPNL
65H	RPN MSB	RPN, RPNM
//}

モジュレーションはLFOなど時間に伴って周波数などを反復的に上下させるものです。パンは音の左右振り分けです。ダンパーペダルとはピアノのペダルの効果を表現するものです（音が延びたりします）。リバーブ、コーラス、ディレイは音の響き方、広がり方を変えるものです。

ボリュームとエクスプレッションはそれぞれ音量に影響するパラメーターです。これらの他にもノート命令のベロシティパラメーターやPAfがあるのですが、ボリュームは音量、エクスプレッションは発音後の強弱の変化を、それぞれチャンネル単位で指定するものです。ノートやPAfのパラメーターはキー単位で指定します（ここは学習したての時はそれほど気にしなくても良いでしょう）。

RPN、NRPN、DTEは、コントロールチェンジでもさらに足りない部分の制御を行います。RPNはregistered parameter number、NRPNはnon-RPN、すなわち標準化の有無です。RPNはチューニングのパラメーターなど、わずかな定義しかありません。NRPNはMIDIデバイス固有のパラメーターとなります。LSBは下位バイト、MSBは上位バイトで、計14ビット@<fn>{midi_msg_encoding}のデータを送信できます。使うときは、まずRPNやNRPNのメッセージ（LSB/MSBのペア）で対象パラメータ番号を指定して、次にDTE（LSB/MSBのペア）でその値を送信します。過去バージョンの話ですが、Vocaloid2では、このNRPNを活用して歌唱合成を制御していました。

//footnote[midi_msg_encoding][MIDIメッセージは1バイト = 8ビットのうち、sign flagなどで使う1ビットを除いた7ビットのみで数値を表すため、2バイトで14ビットとなります。]

これらはいずれも、知っておくと表現力が増すものなので、探究心をもっていろいろ実験してみると面白いでしょう。シンセサイザー固有の表現力もここに現れてきますし、GSやXGといったベンダー固有の共通規格のパラメータも活用するとよいでしょう。mugeneでは、@<tt>{nrpn-gs-xg.mml}というMMLがデフォルトで取り込まれており、この中では@<tt>{TVF_CUTOFF}（カットオフ）、@<tt>{TVF_RESO}（レゾナンス）などもサポートされています。

=== ドラムパートの打ち込み

ドラムパートは音色の扱いが特殊で、キーごとに全く異なる音が出る、というのは前述したとおりですが、mugeneではドラムパートも違和感なく打ち込めるように、ドラムパート用の命令を定義してあります@<fn>{drum-part-mml}。（@<table>{drum_mml}）

//footnote[drum-part-mml][ドラムパート用のMML命令は、@<tt>{default-macro.mml}ではなく@<tt>{drum-part.mml}に含まれています。]

//table[drum_mml][ドラムパート用命令]{
MML命令	意味
----------------------------------
b	bass drum (n36)
B	bass drum 2 (n35)
s	snare drum (n38)
S	snare drum 2 (n40)
TL	low tom (n43)
TM	mid tom (n47)
TH	high tom (n50)
c	closed hi-hat (n42)
o	open hi-hat (n46)
RC	ride cymbal (n51)
CR	crash cymbal (n49)
R	side stick (rimshot) (n37)
MT	muted triangle (n80)
OT	open triangle (n81)
//}

これらは全てノート命令@<tt>{n}を呼び出しているだけなので、ノート命令の他の引数は全て同じように指定できますし、ベロシティも@<tt>{v}命令の値が適用されます。実のところ、この程度の内容であれば、打ち込み時に自分でマクロ定義してしまったほうが良いと思います。ドラムパートで使える楽器は他にもたくさんあります。

典型的な4つ打ちのドラムフレーズの例を挙げると、こんな感じになります。

//emlist{
#define DRUMTRACKS 21,22
21  CH10 l4   cccc   ccoc  ccoc   l8 c4c4ococ
22  CH10 l4  bbsb  bbsb  bbsb  l8 b4b4sbs4
//}

mugene MMLでは、特定のトラックに限定したマクロを定義することが可能で、これらのリズム命令では、この機能を活用しています。@<tt>{o}命令を他のトラックで使用したら、オクターブ指定になります（ドラムパートでオクターブ指定は無用の長物なので、使い潰しているというわけです）。@<tt>{c}命令も他のパートではドの発音になります。どのトラックがドラムとして扱われるかは、@<tt>{#define DRUMTRACKS}の行で定義できます。

=== 楽曲メタ情報を記述する

最後に、SMFの楽曲にはメタデータとして、タイトルや作曲者の情報を埋め込むことができます。mugene MMLにも、それらを指定する方法があります。これらは@<tt>{#}から始まる1行で記述します（ディレクティブ行というもので、これについては以降の章で説明します）。

 * #meta title 楽曲タイトル
 * #meta copyright 著作権者
 * #meta text  その他のテキスト

ここまでの作業がこなせたら、まずはベタ打ちでもMIDIデータとして配布するのにはふさわしいMIDIデータになるでしょう。

=== ループを活用する

MMLで音楽を打ち込んでいると、同じフレーズや同じ命令列を何度も書くことがあります。コピーしてしまえば良いのですが、全部まとめて変更したくなると面倒です。mugeneでは、反復的な処理をまとめるやり方が２つあります。ループとマクロです。

まずループ処理です。@<tt>{[}〜@<tt>{]}@<i>{n}で囲まれた部分は繰り返しになります。@<i>{n}には繰り返したい数を指定します。

ループ命令は、実際にはもう少し複雑なことができます。ループの途中で@<tt>{:}@<i>{s}や@<tt>{/}@<i>{s}を指定すると、@<i>{s}回目のループの時のみ、その次の@<tt>{:}・@<tt>{/}あるいは@<tt>{]}までの範囲を処理します。これは主として楽譜の中で使われる反復記号を実現するためにあります。例を示しましょう。

//emlist{
10	[ b4b4 / s4b4 ]4  TH8TM8TL4
//}

これは、キックドラムとスネアドラムの4つ打ちですが、3小節同じフレーズで叩いた後、タムのフィルインに続いています。

次の例はもう少しややこしいです。

//emlist{
10	[ b4s4 :4 b8b8s4 : b4s4 ]8  TH8TM8TL4
//}

4小節目のときだけフレーズの後半が@<tt>{b8b8b4}になり、8小節目ではタムのフィルインに繋がります。

@<tt>{/}と@<tt>{:}で指定できる数値は、実はリストになっていて、「2回目と6回目だけこのフレーズ」みたいな指定も可能なのですが、使いこなすのは難しい（自分で書いたMMLを読み解くのが困難になる）ので、あまりオススメはしません。

=== マクロを活用する

単純な繰り返し以外の場面で、同じセットの命令を何度も指定したいことがあります。たとえばキーボードが単純な和音でコードを演奏しているとき、コードに対応する音符の集合を何度も記述するのは面倒です。こういうときは、マクロを活用します。

//emlist{
#macro CHORD_A c0e0g
#macro CHORD_B d0f0a
#macro CHORD_C c0e0g0b
1	@48 o5 l4 CHORD_A1  CHORD_B1  CHORD_B2 CHORD_B2  CHORD_C1
//}

注意すべきは、@<b>{マクロ名には数字を使えない}ということです。mugene MMLの命令はほぼ全てマクロで構成されています。命令には引数が伴います（マクロ定義では引数を定義することが可能です）。マクロ名に数字が使えてしまうと、命令の引数とマクロを区別できなくなってしまうのです。コードマクロだからといって、C_MAJOR、C_MAJOR_7などと命名しようとすると失敗します。

この辺りの事情は、一般的なプログラミング言語とは大きく異なります。一般的なプログラミング言語では@<tt>{(}〜@<tt>{)}などで関数名と引数を区別できますが、MMLでは誰も@<tt>{c(4)d(4)e(4)}などと書きたくないので、命令語の後は直ちに引数となります。

マクロ定義の内容は、命令語までは完全に展開できることが想定されていますが、引数までマクロ定義に含まれる必要はありません。これらのコード定義は@<tt>{g}、@<tt>{a}、@<tt>{b}といった命令の途中で終わっているので、@<tt>{CHORD_B2}のように続けて引数を指定することができます。これは2分音符の和音になっています。





