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
#include<stdio.h>
#include<stdio.h>
#include<stdlib.h>

// prototype宣言
int readWavHead(FILE* fp, int* dataLength, unsigned long* fs, unsigned short* chNum, int* sampSize);
int writeWavHead(FILE* fp, int dataLength, unsigned long fs, unsigned short  chNum, int sampSize);
int CT_fft(double* x, double* y, int n, int fr);

void main(int argc, char *argv[]){
    char ioFile[256];   // 出力ファイルの素
    char inFile[256];   // 入力ファイル
    char outFile[256];  // 出力ファイル
    char csvFile[256];  // CSVファイル
    FILE *ifp, *ofp;

    int FRAMESIZE;

    int len, sampSize, outlen;
    unsigned long fs;
    unsigned short chNum;
    int fNum;

    double GAIN;
    double sfpf;    // Sampling frequency per frame
    int cut_Hz_N1, cut_Hz_N2;
    int N1, N2;

    char yN = 'n';

    if(argc < 2){
        puts("コマンドライン引数が足りません");
        exit(0);
    }

	strcpy(inFile, argv[1]);    // 入力ファイルをコピー
    strcpy(ioFile, argv[1]);

    for(int i = 0;i < 256;i++){
        if(ioFile[i] == '.'){  // '.'を検索
            for(int j = i;j < length(inFile);j++){
                // 拡張子を'\0'で埋める
                ioFile[j] = '\0';
                if(ioFile[j + 1] == '\0'){  // Null文字を検索
                    break;
                }
            }
        }
    }

    // 出力ファイル名を作成
    sprintf(csvFile, "PowSpectorum", ioFile, ".csv");
    sprintf(outFile, "Out", ioFile, ".wav");

	printf("input file is %s.\n", inFile);
	printf("output file is %s.\n", outFile);
    printf("csv file is %s.\n", csvFile);

	ifp = fopen(inFile, "rb");
	ofp = fopen(outFile, "wb");

    // wavヘッダを読み込み
	if (readWavHead(ifp, &len, &fs, &chNum, &sampSize) < 0) {
		puts("入力ファイルのヘッダがうまく読めません");
		exit(-1);
	}

	// wavパラメータを出力
	printf("ファイルの長さは %d サンプル\n", len);
	printf("サンプリング周波数は %d Hz\n", fs);
	printf("チャネル数は %d\n", chNum);
	printf("1サンプルのビット数は %d ビット\n", sampSize);

    while(yN == 'N' || 'n'){
        puts("各種パラメータを入力");
        printf("FRAMESIZE = ");     scanf("%d", &FRAMESIZE);
        printf("GAIN = ");          scanf("%lf", &GAIN);
        printf("除去したい周波数帯域の最小値を入力[Hz]: ");
        scanf_s("%lf", &cut_Hz_N1);
        printf("除去したい周波数帯域の最大値を入力[Hz]: ");
	    scanf_s("%lf", &cut_Hz_N2);

        sfpf = fs / FRAMESIZE;

        N1 = cut_Hz_N1 / sfpf;
	    N2 = cut_Hz_N2 / sfpf;

        printf("FRAMESIZE: %d\n", FRAMESIZE);
        printf("GAIN: %d\n", GAIN);
    	printf("%lf[Hz]から%lf[Hz]を除去\n", cut_Hz_N1, cut_Hz_N2);
	    printf("操作対象の要素番号は[%d]から[%d]\n", N1, N2);

        puts("以上のパラメータで実行してよろしいですか(y/N):");
        scanf("%c", &yN);
    }
}
