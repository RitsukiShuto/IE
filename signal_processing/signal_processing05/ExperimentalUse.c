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
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// prototype宣言
int readWavHead(FILE *fp, int *dataLength, unsigned long *fs, unsigned short *chNum, int *sampSize);
int writeWavHead(FILE *fp, int dataLength, unsigned long fs, unsigned short chNum, int sampSize);
int CT_fft(double *x, double *y, int n, int fr);

void main(int argc, char *argv[])
{
    char ioFile[256];      // 出力ファイルの素
    char inFile[256];      // 入力ファイル
    char outFile[256];     // 出力ファイル
    char csvspctFile[256]; // CSVファイル
    char csvinfoFile[256];
    char csvvolFile[256];

    FILE *ifp, *ofp, *fp_info, *fp_spect, *fp_vol;

    int FRAMESIZE;

    int len, sampSize, outlen;
    unsigned long fs;
    unsigned short chNum;
    int fNum;

    double GAIN;
    double sfpf; // Sampling frequency per frame
    int cut_Hz_N1, cut_Hz_N2;
    int N1, N2;

    int yN;

    if (argc < 2)
    {
        puts("コマンドライン引数が足りません");
        exit(0);
    }

    strcpy(inFile, argv[1]); // 入力ファイルをコピー
    strcpy(ioFile, argv[1]);

    for (int i = 0; i < 256; i++)
    {
        if (ioFile[i] == '.')
        { // '.'を検索
            for (int j = i; j < 256; j++)
            {
                // 拡張子を'\0'で埋める
                ioFile[j] = '\0';
                if (ioFile[j + 1] == '\0')
                { // Null文字を検索
                    break;
                }
            }
        }
    }

    // 出力ファイル名を作成
    sprintf(outFile, "Out%s.wav", ioFile);
    sprintf(csvinfoFile, "info%s.csv", ioFile);
    sprintf(csvspctFile, "PowSpect%s.csv", ioFile);
    sprintf(csvvolFile, "Vol%s.csv", ioFile);

    printf("input file is %s.\n", inFile);
    printf("output file is %s.\n", outFile);

    ifp = fopen(inFile, "rb");
    ofp = fopen(outFile, "wb");

    // wavヘッダを読み込み
    if (readWavHead(ifp, &len, &fs, &chNum, &sampSize) < 0)
    {
        puts("入力ファイルのヘッダがうまく読めません");
        exit(-1);
    }

    fp_info = fopen(csvinfoFile, "w");
    fp_spect = fopen(csvspctFile, "w");
    fp_vol = fopen(csvvolFile, "w");

    // wavパラメータを出力
    printf("ファイルの長さは %d サンプル\n", len);
    printf("サンプリング周波数は %d Hz\n", fs);
    printf("チャネル数は %d\n", chNum);
    printf("1サンプルのビット数は %d ビット\n\n", sampSize);

    fprintf(fp_info, "ファイルの長さは %d サンプル\n", len);
    fprintf(fp_info, "サンプリング周波数は %d Hz\n", fs);
    fprintf(fp_info, "チャネル数は %d\n", chNum);
    fprintf(fp_info, "1サンプルのビット数は %d ビット\n\n", sampSize);

    puts("各種パラメータを入力");
    printf("FRAMESIZE = ");
    scanf("%d", &FRAMESIZE);
    printf("GAIN = ");
    scanf("%lf", &GAIN);
    printf("除去したい周波数帯域の最小値を入力[Hz]: ");
    scanf("%lf", &cut_Hz_N1);
    printf("除去したい周波数帯域の最大値を入力[Hz]: ");
    scanf("%lf", &cut_Hz_N2);

    sfpf = fs / FRAMESIZE;

    N1 = cut_Hz_N1 / sfpf;
    N2 = cut_Hz_N2 / sfpf;

    printf("FRAMESIZE: %d\n", FRAMESIZE);
    printf("GAIN: %d\n", GAIN);
    printf("%lf[Hz]から%lf[Hz]を除去\n", cut_Hz_N1, cut_Hz_N2);
    printf("操作対象の要素番号は[%d]から[%d]\n", N1, N2);

    fprintf(fp_info, "FRAMESIZE: %d\n", FRAMESIZE);
    fprintf(fp_info, "GAIN: %lf\n", GAIN);
    fprintf(fp_info, "%lf[Hz]から%lf[Hz]を除去\n", cut_Hz_N1, cut_Hz_N2);
    fprintf(fp_info, "操作対象の要素番号は[%d]から[%d]\n", N1, N2);

    short dataIn[FRAMESIZE];
    short dataOutX[FRAMESIZE], dataOutY[FRAMESIZE];
    double dDataInX[FRAMESIZE], dDataInY[FRAMESIZE];
    double dDataOutX[FRAMESIZE], dDataOutY[FRAMESIZE];

    fNum = (int)floor(len / FRAMESIZE);
    outlen = fNum * FRAMESIZE;

    /*
	書き込み用のwavファイルのヘッダを書き込む。データのサイズまで,
	書き込んでいるので、この後はかならずoutlenと同じだけfwriteすること
	*/
    writeWavHead(ofp, outlen, fs, chNum, sampSize);

    for (int i = 0; i < fNum; i++)
    {
        fread(dataIn, sizeof(short), FRAMESIZE, ifp);

        for (int j = 0; j < FRAMESIZE; j++)
        {
            dDataInX[j] = (double)dataIn[j]; // CT_fftはdoubleの変数を使うので、doubleの変数に代入してください。
            dDataInY[j] = 0.0;               // 虚数部は0.0を入れておきます。
        }
        CT_fft(dDataInX, dDataInY, FRAMESIZE, 1); //FFT
        for (int j = 0; j < FRAMESIZE; j++)
        {
            dDataOutX[j] = dDataInX[j];
            dDataOutY[j] = dDataInY[j];
            fprintf(fp_spect, "%lf\n", pow(dDataOutX[j], 2) + pow(dDataOutY[j], 2));
        }

        for (int j = N1; j < N2; j++)
        {
            dDataOutX[j] = GAIN * dDataOutX[j];
            dDataOutY[j] = GAIN * dDataOutY[j];
        }

        CT_fft(dDataOutX, dDataOutY, FRAMESIZE, -1); //逆FFT、４つ目の引数が-1の場合逆FFTとなる。

        for (int j = 0; j < FRAMESIZE; j++)
        {
            fprintf(fp_vol, "%lf,%lf\n", dDataOutX[j], dDataOutY[j]); // 逆FFTの結果を見たいなら、これを出力する
            dataOutX[j] = (short)dDataOutX[j];                        // Wavファイルに書き出すためにshortに型変換する
            dataOutY[j] = (short)dDataOutY[j];                        // Wavファイルに書き出すためにshortに型変換する
                                                                      //			fprintf(fp_vol, "%d,%d\n", dataOutX[j], dataOutY[j]); 	    // ファイルに書き出す数値を見たいなら、これを出力する
        }
        //		printf("\n");

        fwrite(dataOutX, sizeof(short), FRAMESIZE, ofp); // dataOutXのみを書き出しているが、Yの扱いは場合による
    }

    fclose(ifp);
    fclose(ofp);
    fclose(fp_info);
    fclose(fp_spect);
    fclose(fp_vol);
}