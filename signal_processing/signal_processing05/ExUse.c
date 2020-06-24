/*
Created by RitsukiShuto on 2020/06/21-20:50:56.
Coding 'Shift JIS'

--使い方--
    *実験に使うwavファイル、ヘッダファイル、その他の依存ファイルをこのコードと同じディレクトリに置く
    *実行時にもろもろのパラメータを逐次定義する
    *実行結果はすべてcsvファイルで帰ってくる

--コンパイル--
    gccを使用する。clではコンパイルが通らない可能性あり

*/
#include<stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

// prototype宣言
int readWavHead(FILE* fp, int* dataLength, unsigned long* fs, unsigned short* chNum, int* sampSize);
int writeWavHead(FILE* fp, int dataLength, unsigned long fs, unsigned short  chNum, int sampSize);
int CT_fft(double* x, double* y, int n, int fr);

void main(int argc, char *argv[]){
    // 入出力ファイル
    char ioFile[256];  // 出力ファイルの素
    char inFile[256];   // 入力ファイル
    char outFile[256];  // 出力ファイル
    char csvspctFile[256];  // CSVファイル
    char csvinfoFile[256];
    char csvvolFile[256];

    FILE *ifp, *ofp, *fp_info, *fp_spect, *fp_vol;

    // 信号処理用
    int FRAMESIZE;
    int len, sampSize, outlen;
    unsigned long fs;
    unsigned short chNum;
    int fNum;

    // 周波数除去用 
    double GAIN;
    double cutOff;
    int cutHz, cutHzL1, cutHzL2;
    int cut_Hz_N1, cut_Hz_N2;
    int N1, N2;

    // コマンドライン引数を確認
    if(argc < 2){
        puts("コマンドライン引数が足りません");
        exit(0);
    }

	strcpy(inFile, argv[1]);    // 入力ファイルをコピー
    strcpy(ioFile, argv[1]);

    for(int i = 0;i < 256;i++){
        // '.'を検索
        if(ioFile[i] == '.'){
            // 拡張子以降を'\0'で埋める
            for(int j = i;j < 256;j++){
                ioFile[j] = '\0';
                // Null文字が見つかったら終了
                if(ioFile[j + 1] == '\0'){
                    break;
                }
            }
        }
    }

    // 出力ファイル名を作成
    sprintf(outFile, "Out%s.wav", ioFile);      // sprintf:文字列連結用関数
    sprintf(csvinfoFile, "info%s.csv", ioFile);
    sprintf(csvspctFile, "PowSpect%s.csv", ioFile);
    sprintf(csvvolFile, "Vol%s.csv", ioFile);

    // 出力ファイルを確認
	puts("以下のファイルが作成されます");
    printf("%s\n", outFile);
    printf("%s\n", csvinfoFile);
    printf("%s\n", csvspctFile);
    printf("%s\n", csvvolFile);

	ifp = fopen(inFile, "rb");

	ofp = fopen(outFile, "wb");
    fp_info = fopen(csvinfoFile, "w");
    fp_spect = fopen(csvspctFile, "w");
    fp_vol = fopen(csvvolFile, "w");

    // wavヘッダを読み込み
	if (readWavHead(ifp, &len, &fs, &chNum, &sampSize) < 0) {
		puts("入力ファイルのヘッダがうまく読めません");
		exit(-1);
	}

	// wavパラメータを出力
	printf("ファイルの長さは %d サンプル\n", len);
	printf("サンプリング周波数は %d Hz\n", fs);
	printf("チャネル数は %d\n", chNum);
	printf("1サンプルのビット数は %d ビット\n\n", sampSize);

    // wavパラメータを書き込み
    fprintf(fp_info, "ファイルの長さは %d サンプル\n", len);
	fprintf(fp_info, "サンプリング周波数は %d Hz\n", fs);
	fprintf(fp_info, "チャネル数は %d\n", chNum);
	fprintf(fp_info, "1サンプルのビット数は %d ビット\n\n", sampSize);

    // 入力ファイルパラメータ入力部
    puts("各種パラメータを入力");
    printf("FRAMESIZE = ");     scanf("%d", &FRAMESIZE);
    printf("GAIN = ");          scanf("%lf", &GAIN);
    printf("除去したい周波数帯域の最小値を入力[Hz]: ");
    scanf("%d", &cutHzL1);
    printf("除去したい周波数帯域の最大値を入力[Hz]: ");
    scanf("%d", &cutHzL2);

    // 処理領域を計算
    cutOff = cutHzL1 * (FRAMESIZE / (double)fs);
    N1 = (int)cutOff + 1;       // 始点
    cutOff = cutHzL2 * (FRAMESIZE /(double)fs);
    N2 = (int)cutOff + 1;       // 終点

    // 入力ファイルパラメータを確認
    printf("FRAMESIZE: %d\n", FRAMESIZE);
    printf("GAIN: %lf\n", GAIN);
	printf("%d[Hz]から%d[Hz]を除去\n", cutHzL1, cutHzL2);
    printf("操作対象の要素番号は[%d]から[%d]\n", N1, N2);

    // 書き込み info
    fprintf(fp_info, "FRAMESIZE: %d\n", FRAMESIZE);
    fprintf(fp_info, "GAIN: %lf\n", GAIN);
	fprintf(fp_info, "%d[Hz]から%d[Hz]を除去\n", cutHzL1, cutHzL2);
    fprintf(fp_info, "操作対象の要素番号は[%d]から[%d]\n", N1, N2);

    //// BUG:clコマンドでコンパイルが通らない場所 => gccコンパイラで通るのでヨシ!
    short dataIn[FRAMESIZE];
	short dataOutX[FRAMESIZE], dataOutY[FRAMESIZE];
	double dDataInX[FRAMESIZE], dDataInY[FRAMESIZE];
	double dDataOutX[FRAMESIZE], dDataOutY[FRAMESIZE];

    fNum = (int)floor(len / FRAMESIZE);
	outlen = fNum * FRAMESIZE;

	writeWavHead(ofp, outlen, fs, chNum, sampSize);

    // 以下音声処理
	for (int i = 0; i < fNum; i++) {
		fread(dataIn, sizeof(short), FRAMESIZE, ifp);

        // FFT用のデータを準備
		for (int j = 0; j < FRAMESIZE; j++) {
			dDataInX[j] = (double)dataIn[j];
			dDataInY[j] = 0.0;
		}

		CT_fft(dDataInX, dDataInY, FRAMESIZE, 1); //FFT

        // FFT結果をパワースペクトルにして格納
		for (int j = 0; j < FRAMESIZE; j++) {
			dDataOutX[j] = dDataInX[j];
			dDataOutY[j] = dDataInY[j];
		}
        // 対象の周波数を処理()
        for(int j = 0;j < FRAMESIZE; j++){
            if(j >= N1 && j < N2){
                dDataOutX[j] = GAIN * dDataOutX[j];     // 波形処理
			    dDataOutY[j] = GAIN * dDataOutY[j];
                fprintf(fp_spect, "%lf\n", sqrt(pow(dDataOutX[j], 2) + pow(dDataOutY[j], 2)));  // PowSpect
            }else{
                dDataOutX[j] = dDataInX[j];             // 処理が不要な信号なので何もしない
			    dDataOutY[j] = dDataInY[j];
                fprintf(fp_spect, "%lf\n", sqrt(pow(dDataOutX[j], 2) + pow(dDataOutY[j], 2)));  // PowSpect
            }
        }

		CT_fft(dDataOutX, dDataOutY, FRAMESIZE, -1);    // IFFT(arg4 = -1)

		for (int j = 0; j < FRAMESIZE; j++) {
//			fprintf(fp_vol, "%lf,%lf\n", dDataOutX[j], dDataOutY[j]);   // 逆FFTの結果
			dataOutX[j] = (short)dDataOutX[j];
			dataOutY[j] = (short)dDataOutY[j];
			fprintf(fp_vol, "%d,%d\n", dataOutX[j], dataOutY[j]); 	    // Vol
		}

		fprintf(fp_spect, "\n");
        fprintf(fp_vol, "\n");

		fwrite(dataOutX, sizeof(short), FRAMESIZE, ofp);    // dataOutXのみを書き出しているが、Yの扱いは場合による
	}

	fclose(ifp);
	fclose(ofp);
    fclose(fp_info);
    fclose(fp_spect);
    fclose(fp_vol);
}