/*
Created by RitsukiShuto on 2020/06/21-20:50:56.
Coding 'Shift JIS'

--使い方--
    *実験に使うwavファイル、ヘッダファイル、その他の依存ファイルをこのコードと同じディレクトリに置く
    *実行時にもろもろのパラメータを逐次定義する
    *実行結果はすべてcsvファイルで帰ってくる

--コンパイル--
    cl ExperimentalUse.c readWavHead.c writeWavHead.c CT_fft.c
    link \out:ExUse.exe ExperimentalUse.obj readWavHead.obj writeWavHead.obj CT_fft.obj
    ExUse.exe <入力wavファイル>

*/
