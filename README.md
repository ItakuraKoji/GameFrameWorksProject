# GameFrameWorksProject
this is my game frame works.

© 2018 Koji Itakura
Released under the MIT license
http://opensource.org/licenses/mit-license.php

[Version1.1]

リファレンス ： https://itakurakoji.github.io/GameFrameWorksProject/

このライブラリは、ゲーム制作に使う機能を個人的にまとめたフレームワークです。
もともと自分が使うためのものですが、MITライセンスでご自由に使用することができます。

※改良中のため大幅にヘッダの定義が変わるかもしれません
※例によって使用は自己責任でお願いします




・履歴

2018/4/7
不安定版Ver1.0 初公開

2018/4/12
入力のコンフィグができるようになった
FBXのテクスチャ読み込み先をFBXのあるファイルからの相対パスに変更

2018/4/26
仕様変更が多いのでひとまず、現行の段階を「不安定版」として位置づけ

2018/4/29
入力クラスの仕様を大きく変更
外に出しているクラスが 「InputGLFW → InputClass」 になり、AddGamePad()にてゲームパッドを初期化する仕様に変更
AddGamePad()によって追加されるのは、VirtualGamePadクラスであり、InputGLFWクラスに相当する機能を提供している

2018/5/01
pngの読み込みに対応
ただしグレースケールやインデックスカラーには未対応