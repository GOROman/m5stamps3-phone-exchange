# Mini-pbx実例からの設計反映

確認日：2026-09-21。参考：[takao-t/Mini-pbx](https://github.com/takao-t/Mini-pbx)。
確認資料はREADMEと `config.mcc/hal_pbx.c`。PDF回路図・Gerberの実データ照合は未実施。
READMEの確認時blob SHA：`0aace26dc9c61da13e27c5a956b6f4a98e59d479`。
著者の製作・動作報告と、こちらの実機検証は区別する。

## 採否

| 参考元の知見 | Rev.Bでの対応 |
|---|---|
| Ag1171×2、アナログスイッチで音声接続 | 既存SLIC方式を維持。オペアンプを除いた受動音声経路へ変更 |
| 4066で通話を開閉 | Rev.Bはトーン選択とミュートも必要なためHCT4053を維持 |
| 音声バイアス修正のためC1/C2短絡 | 修正の理由を反映。こちらはAC結合後の再バイアスを明示し、コンデンサを残す |
| Ag1171の短い端子と低背ソケット | 低背ソケットを実装要件に追加。接触深さの現物確認を残す |
| ダイヤルパルスとホットライン | 10/20ppsの認識器を独立実装。ルーティング・実GPIO接続は未実装 |
| PIC内部CLCによるデバウンス | ESP32向け参照ロジックで10msのデバウンスを実装 |
| KS0835Fの動作報告 | 代替候補として記録のみ。本設計の互換保証・採用にはしない |
| UART経由の逆給電報告 | USB／制御線を含む電源停止時の逆給電検証を製造前条件として維持 |

## 呼出処理をそのまま移さない理由

確認したHALでは呼出中の休止区間もRMがHighで、FRのみを固定している。
停止処理はRMをLowにした後FRをLowにする構成だった。
こちらはAg1171のメーカー資料を基準に、FR=High固定後20msを待ってRMをLowにする。
休止中はRM=Low。1秒鳴動に停止待ち20msを加えても4秒周期で25.5%となる設計を維持する。
この比較は実コードの読解結果で、参考元の実機故障を確認したものではない。

## 今回の検証と限界

- `tests/test_pulse_dial.cpp`：10/20ppsそれぞれ1〜0の全数字、バウンス、11パルス拒否、切断、長いフラッシュ、32bit時刻周回。
- `tests/test_ag1171_ring.cpp`：既存の呼出停止順序・周期テストを再実行。
- `scripts/check_audio_passive.py`：部品定数と負荷を含む受動音声経路の公称利得計算。
- 電話実機、音量、波形、温度、ソケットの嵌合、ERC/DRCは未検証。

DP認識の受付窓は設計初期値。黒電話の個体差、パルス比、遅延、フック操作による誤認識は実機で確認する。
参照ロジックは1ms程度の周期で動作させる前提。CPU停止・大きな周期遅延への安全機構ではない。
DPの内線番号A=11/B=12と既定の7秒自動呼出は設計仕様であり、現行アプリで選択できる機能ではない。

## 出典・利用条件

参考元READMEには改変時の継承元明示と商用利用禁止の条件がある。
本変更では参考元のソースコード・回路図・画像・Gerberをリポジトリへコピーしていない。
パルス認識器、テスト、小信号計算スクリプトは本プロジェクトで新規に記述したもの。
参考元の成果物を将来取り込む場合は、既存MITライセンスへ無条件に含めず、その利用条件を個別に確認する。

一次資料：

- [Mini-pbx README](https://github.com/takao-t/Mini-pbx/blob/main/README.md)
- [Mini-pbx HAL](https://github.com/takao-t/Mini-pbx/blob/main/config.mcc/hal_pbx.c)
- [Ag1171データシート](https://silvertel.com/images/datasheets/Ag1171-datasheet-Low-cost-ringing-SLIC-with-single-supply.pdf)
- [CD74HCT4053データシート](https://www.ti.com/lit/ds/symlink/cd74hct4053.pdf)
- [CD74HCT4066データシート](https://www.ti.com/lit/ds/symlink/cd74hct4066.pdf)
- [低背ソケット候補：秋月103138](https://akizukidenshi.com/catalog/g/g103138/)
