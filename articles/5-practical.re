
= パート別MML打ち込み技法

この章では、mugene MMLの機能を活用して、さまざまなトラックを打ち込んでいく際に有用な技法を書き連ねていきます。打ち込み技法には、ある程度はセオリーもあると思いますが、個人差がそれなりにある分野であるはずです。筆者には打ち込みトレーニング等の知見は無く、ここは完全に筆者の個人的な経験と感覚に基づいて書いているので、そういうものであると思って読んでください。

楽器にも音色にもいろいろあると思いますが、ここでは典型的なベース、ギター、ドラム、キーボードのみを対象とします。

== ベース

ベースは単音で、打ち込みを始めやすいパートです。ベースの音色はMIDI楽器では@32〜@39に割り当てられていますが、アコースティック、フィンガリングベース、フレットレスベース、スラップベース、シンセベースなど、楽器の種類と奏法がごちゃごちゃになっています。しっかりしたMIDI音源であれば、バンク・セレクトで統一的に打ち込める音が選択できるでしょう。

DAWで打ち込む時はこれらのベースの種類は別々のトラックにすることが多そうですが、mugene MMLではFINGER とか SLAP といったマクロを定義して切り替えるようにすれば、1トラックで自然に表現することもできます@<fn>{frequent_progchg}。

//footnote[frequent_progchg][MIDI楽曲で音色を短時間で頻繁に変更すると、古いデバイスでは処理落ちすることもあって、推奨されなかったものですが、現代的な問題ではなかろうという前提で書いています。無いとは思いますが、環境によっては避けたほうがよいかもしれません。]

ベースはコードのルートになる音で、作りながら動かしたいこともあるでしょう。mugene MMLには、@<tt>{K}というコマンドがあり、もし必要になったらそれ以降のノート命令のキーを全てずらすことが可能です。

ベースは、ギターほど多くはありませんが、たまにスライド奏法で音階を上げ下げすることがあります。このような場合にはピッチベンドのspectra命令@<tt>{B_}が便利です。

== ギター

ギターは生演奏の躍動感が強く、MIDI打ち込みでもかなり難易度が高いパートです。あきらめるところは割り切って、表現できる範囲でがんばっていくとよいでしょう。

ギターの音色は@24〜@31にありますが、これもアコースティックギター、ジャズギター、ミュート、オーバードライブ、ディストーション、ハーモニクスと、楽器の種類と奏法がごちゃごちゃになったものです。もうひとつ、@120はギターのフレットノイズとなっています。アコースティックな打ち込みでたまに使うと効果的でしょう。

RolandやYamahaなどのハードウェア音源には、ギターにうまく適用できるようなコンプレッサー、オーバードライブ、ディストーションといったエフェクトをシステムエクスクルーシブで設定できるものがあります。これらのベンダーではインサーションエフェクトと呼ばれています。ギターチャンネルではこれらを活用したほうがよいでしょう。

マルチギター構成にしたい（ギタリストが2人いるようなバンドのサウンドにする）場合は、チャンネルを分けて、別々のギター音色を割り当てたり、エフェクトパラメータを変えて、音が重ならないようにしたほうがよいでしょう。バッキング担当のギターはコードを定義するマクロを活用するのがよいでしょう。特に、コードをカッティングで弾く場合にはゲートタイムを短めにしますが、カッティング用のコードマクロと通常のフィンガリング用のコードマクロを別々に定義しておくと、クールな演出が簡単になります。

ゲートタイムが調整されているかいないかで、バッキングのギターの印象はまるで変わります。また、音がちゃんとカットされていないとふにゃふにゃした印象になりますし、スパッと切れすぎてもスカスカになってしまうので、ゲートタイムを使ったり、@<tt>{DSD}などでディレイ・エフェクトを適用したり（ディレイはレベル以外にも遅延タイミングなどを調整できます）、場合によってはベロシティを小さくしたノートを手動で追加して表現します。

リードギターに出現しがちなスライド奏法の使い方は、ベースと同様です。ベースと異なりギターは音域が高いので、1オクターブくらいフルにスライドしないと弱々しい印象になることがあります。筆者が@<tt>{PITCH_BEND_SENSITIVITY}と@<tt>{BEND_CENT_MODE}を常に24に設定しているのは、ほぼギターのためです。

== ドラム

ドラムチャンネルは、まず音色をきちんと選択したほうが良いでしょう。General MIDI音源ではドラムの音色を選択するということ自体が観念されていませんが、ハードウェアMIDI音源などでは通常はプログラムチェンジでドラムセットを選択できます。@0にはロック・ポップス系の無難なドラムセットがあるのが一般的です。ジャズっぽい音楽にするならブラシなども含まれているジャズのセット、ハードロックやメタルなどのジャンルにするならパワーのあるドラムセット、EDMやハウスにするならテクノ系のセットなどが選択できることでしょう。

ドラムチャンネルで出す音を全て1トラックで記述するのは合理的ではありません。リズムパートの多くは繰り返しになります。繰り返しを効率よく使うためには、ばらばらになるパートはバラバラにしておいたほうがよいでしょう。具体的には、キックドラム、スネアドラムとタム、ハイハットとシンバルくらいで分けておくとよいです。両手と片足（場合によっては両足）の分だけトラックがあると考えるとよいでしょう。クラッシュシンバルは小節の始めやフィルインに使われますが、ハイハットと独立しているとたまに手が3本あることになってしまって格好悪いので、筆者はまとめたほうが良いと思っています@<fn>{drum_partitions}。

//footnote[drum_partitions][実のところ筆者はクラッシュシンバルは別のパートに分けがちなのですが、これは単に筆者のバックグラウンドがポリフォニックなMIDIではなくFMやPCMチャンネルで打ち込んでいた経験を引きずっているためです…]

クラッシュシンバルは、しっかりした音源だと複数用意されていて、デフォルトで左と右にパンが分かれていたりしますが、そのような音源では、ドラムパートのパンは（通常は）ノートごとに自分で調整することが可能です。

ドラムトラックで表現力に差が現れるのは、グルーヴ感です。音楽のジャンルによっては、リズムが「走る」ものがあったり「モタる」ものがあったりします。少しドラムをモタらせたいな、と思ったら、r96などをトラックの先頭に置きましょう。逆に走らせたいと思ったら、r-96などを置きます（rは休符の命令ですが、前章で説明したとおり、マイナス値を指定すると時間を巻き戻すことができます）。そしてノートによってたまに早めたり遅めたりすると、機械的ではないリズム感が出てきます@<fn>{automated_groove}。

//footnote[automated_groove][本当はこういうグルーヴ感を自動生成できるspectraがあると良いのですが、今のところ実装できていません。筆者はあまりコンパイル時にランダムで変化を付けたがらない打ち込み屋なので消極的だということもあります。]

グルーヴ感を演出できるもうひとつの要素はベロシティです。全てのノートを同じベロシティで演奏すると、ドラムは機械的で味気ないものになります。位置によってベロシティを10とか20とか下げてみるとこれが緩和されます。筆者の場合、そもそも音量指定を伴うマクロを使用しています。

//emlist{
#macro 11       HA {c0,,100r}
#macro 11       HB {c0,,90r}
#macro 11       HC {c0,,100r}
#macro 11       HD {c0,,70r}
#macro 11       HE {c0,,60r}
A 11    CH10 v80 l16
C 11    [ HB HE HC HE HA HD HB HE HB HE HC HE HA HE HC o ]3
//}

v60まで下がると、v100の音に比べてだいぶ小さくなりますが、聞こえてはいます。実のところ、ハイハットを叩くときは、普通に叩いているときと、小さく触れているときがあるはずで（ゴーストノート）、後者をきちんと打ち込んでやることで、より自然なドラムに聞こえるようになります。

ちなみに、オーケストラサウンドを作っている場合はティンパニ、EDMなどであればオーケストラ・ヒットなどがよく使われますが、これらはドラムパートではなく@47や@56を使って普通のチャンネルに打ち込めます。

== キーボード

キーボードのパートは、生演奏でもMIDIシンセサイザーなどが使われているので、特段難しい事情は何もないでしょう。さまざまな音があるので、趣味にあったものを選ぶとよいでしょう。もちろん入れなくてもいいし、複数パートあっても全く問題ないでしょう。ここではいくつかの楽器を挙げて補足説明します。

(1) ピアノ - @0〜@7にあります。ピアノにはさまざまな種類があるので、好きなものを選ぶと良いと思います。ピアノで特に有用なのはホールド（ダンパーペダル）のCCで、これをオンにしたりオフにしたりすると、アルペジオ奏法を簡単に実現できます。音源によって再現の仕方が全く異なるので、さまざまなMIDI楽器で再生することを意図しているのであれば、0と64だけにする、といったやり方もありです。

少し凝った作曲を目指している人は、もしかしたら平均律ではなく純正律などを使っているかもしれませんが、MIDI楽器によっては純正律に最適化された倍音調整をNRPNやSysExのパラメーターで与えることが出来るものも存在します。筆者は一度も使ったことがありませんが、場合によっては活用できるでしょう。

(2) オルガン - @16〜@23はオルガンというカテゴリーになっています。実際にはハーモニカやアコーディオンまで含められています。オルガンはロックのコードなどで便利です。楽器によってはロータリーエンコーダーやフェイザーなどのエフェクトが用意されており、オルガンには非常に適しています。もちろんディストーションなどをかけるのもありです。

(3) ストリングアンサンブル - @48〜54はストリングスやクワイアなど、和声に適した音色が揃っており、アタックが弱いので他のパートが構築するリズム感を邪魔せずにコードを構成できます。ピアノなどで打ち込むと打鍵が強くて邪魔だ、という場合に有用でしょう。

これらの音色に限らず、どの音色でも適用できる音の強弱の時間的変化として、<b>{エンベロープ}があります。エンベロープはNRPNで実装している音源でのみ適用できるパラメーターですが、RolandのGSとYamahaのXGでは規定されており、mugene MMLでも@<tt>{nrpn-gs-xg.mml}で@<tt>{ENV_ATTACK}、@<tt>{ENV_DECAY},
@<tt>{ENV_RELEASE}として定義されています。アタック、ディケイ、リリースといった変化ポイントがあり、発声時の強さを控えめにしたり、音の減衰をゆるやかにしたりといった調整が可能です。




