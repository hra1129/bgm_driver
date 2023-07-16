SPLAY.COM
--------------------------------------------------------------------------------

簡易再生ツールです。

1. 準備
	MML を作ります。MMLテキストファイルを music.txt という名前で保存したとします。
	PC上で、MMLをコンパイルします。
	  mc.exe music.txt music.asm
	これにより生成された music.asm を zma でアセンブルします。
	  zma music.asm music.bin
	この music.bin が BGMファイルになります。

2. 使い方
	SPLAY.COM, BGMDRV.BIN を MSX-DOS起動ディスクの中にコピーします。
	上記で準備した music.bin もコピーします。
	  splay music.bin
	これで再生されます。

3. OpenMSX で使う場合
	ディレクトリマウントしたディレクトリに MSXDOS.SYS, COMMAND.COM, SPLAY.COM, BGMDRV.BIN, music.bin
	をコピーして OpenMSX を起動します。
	  splay music.bin
	これで再生されます。
	music.bin を更新したら、L を押すことでリロードして P で再生開始です。
	MML の確認作業にご利用下さいませ。

-------------------------------------------------------------------------------
2023年7月17日  HRA!
