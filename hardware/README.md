# ハードウェア

設計CADはKiCadを使用し、JLCPCBでの2層FR-4基板製造を想定しています。必要に応じてEasyEDA Proへインポートします。

Rev.AはM5StampS3を交換可能な2.54mm DIP方式で搭載する制御キャリア基板です。公式M5Stackフットプリントのピン配置に合わせます。

## 設計ブロック

1. 5Vロジック電源と入力保護
2. StampS3キャリアとプログラミングアクセス
3. 電話機Aの回線給電とオフフック検出
4. 電話機Bの回線給電とオフフック検出
5. 音声経路と回線切り替え
6. 昇圧リンガ、Hブリッジ、電話機選択

## Rev.A PCB

`m5stamps3_phone_exchange.kicad_pcb`は、80mm×60mmの初期レイアウトです。StampS3、5Vロジック電源、RJ11×2、リンガ／音声拡張ヘッダを配置しています。

## 製造データ

- Gerber／NC Drill ZIP
- LCSC品番付きBOM CSV
- CPL（部品座標）CSV
- 実装図
- 回路図PDF

RJ11端子は`PRIVATE PHONE A`と`PRIVATE PHONE B`です。公衆電話網へ接続する端子は設けません。
