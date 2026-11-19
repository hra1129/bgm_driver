# bgm_driver xeviyoke 再生成 差し替えファイル一式（2026-11-19）

## 内容
- `sample/bgm.asm` — 現行 `sample/xeviyoke.txt` から mc で再生成したもの
- `sample/BGMSMPL.BIN` — 新しい bgm.asm を include して sample.asm を zma で
  アセンブルし直したもの（bgm.asm の派生ファイル）

リポジトリ（hra1129/bgm_driver）の該当パスへそのまま差し替えてください。
GitHub への反映はお手元でお願いします。

## 背景
同梱 bgm.asm は現行 xeviyoke.txt とは異なる（古い）MML から生成された版ずれで、
ch1（音色@1）第1フレーズ末尾の59フレーム分だけが食い違っていました
（同梱 = B♭主体の6イベント 12バイト／現行 txt = タイ付き `a&a2` の1イベント 2バイト、
合計時間は同一）。2026-11-19 の裁定「bgm.asm を現行 txt で再生成して揃える」に
従って再生成しました。詳細な特定内容は bgm_driver_doc/mc_verify/XEVIYOKE_DIFF.md、
再生成の作業記録は bgm_driver_doc/regen/README.md にあります。

## 再生成の方法
- mc はリポジトリ同梱 `mml_compiler/source/mc.c` の Linux ネイティブビルド
  （検証済み。sample.txt → 同梱 sample.asm とバイト完全一致で動作を裏付け済み）
  で `mc xeviyoke.txt bgm.asm` を実行。
- BGMSMPL.BIN は sample/compile.bat と同じ手順（zma で sample.asm をアセンブル）。
  旧 bgm.asm では同梱 BGMSMPL.BIN がバイト完全一致で再現できることを確認した
  うえで、新 bgm.asm で組み直しています。

## 差分の範囲（検証済み）
- bgm.asm: 2046B → 1986B（データ 317B → 307B、−10B）。
  変わるのは上記59フレーム分と、それに伴うオフセットシフト
  （ヘッダの ch2 開始 234→224、SOUND_FONT 参照 235→225／276→266、
  JUMP 先 223→213）のみ。ch0 本体・ch2・音色データはバイト完全一致。
- BGMSMPL.BIN: 2162B → 2152B（−10B）。BIN 内の bgm データは新 bgm.asm と一致、
  後方の効果音データはバイト完全一致、前方コード部の相違はアドレス −10 調整の
  3ワードのみ（BSAVE 終了アドレス含む）。
