hspcmp proxy dll
HSP のスクリプトファイルに #compiler "DLLのファイル名" と書くだけで hspcmp.dll を切り替えられる DLL
--------------------------------------------------------------------------------
HSP の実行ランタイム は #runtime を使うことで切り替えられます。
バージョンの違う HSP のランタイム同士の動作を確認したいときにはこの #runtime の行を書き換えるだけなのでとっても楽です。
しかし、 コンパイラの場合はそうもいきません。
hspcmp.dll に上書きコピーするのも面倒だし、それに標準エディタの場合はコンパイルしていないときでも hspcmp.dll を読み込んでいてロックがかかっていて上書きできないし。
そこでこの hspcmp proxy dll の登場です！
これを使うとスクリプトファイルに #compiler "DLLのファイル名" と書くだけでコンパイラの DLL を切り替えられます。

--------------------------------------------------------------------------------
	準備
--------------------------------------------------------------------------------
HSP のインストールディレクトリの hspcmp.dll を hspcmp-default.dll に名前変更します。
（hspcmp-default.dll は #compiler 指定がないときに使われます）
そして、 hspcmp proxy dll の hspcmp.dll を HSP のインストールディレクトリにコピーします。

あと、hspcmp-default.dll 以外にいろんなバージョンの hspcmp.dll を入れとかないと意味ないですね。
	hspcmp-31.dll
	hspcmp-32b1.dll
	hspcmp-32b2.dll
	hspcmp-rXXX.dll
	hspcmp-work.dll
	    :
	    :
	みたいなかんじで。

--------------------------------------------------------------------------------
	注意
--------------------------------------------------------------------------------
スクリプトファイルから #compiler と書かれた行を探すだけですから、 #include 先は見ない上に、
- #if 0 ～ #endif 内
- 複数行コメント内 /* ～ */
- 複数行文字列内 {"～～"}
でも #compiler と書いてあればそれを拾ってしまいます。



HSPTV! : <http://hsp.tv/>
HSPオフィシャルホームページ : <http://www.onionsoft.net/hsp/>
OpenHSP : <http://www.onionsoft.net/hsp/openhsp/>
作者 ( fujidig ) web ページ : <http://www.fujidig.com/>
配布ページ : <http://www.fujidig.com/archives/#hspcmp-proxy>

fujidig 2009/5/8
fuji.rosen@gmail.com
