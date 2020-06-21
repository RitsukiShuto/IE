/*
2020-4-1.c a.k.a. FFT.c
Edited by RitsukiShuto on 2020/06/18-22:18:51.

Coding 'UTF-8'

Wavファイルを読み込んで、FFTする。
結果をそのまま複素数として、コンソールに出力する。


コンパイル方法は
cl 2020-4-1.c readWavHead.c CT_fft.c

by mokam@cis
*/

#include<stdio.h>
#include<math.h>

#define FRAMESIZE 256 //フレーム長、2のべき乗であること

/*こういう関数を使うよ、という宣言*/
int readWavHead(FILE* fp, int* dataLength, unsigned long* fs, unsigned short* chNum, int* sampSize);
int CT_fft(double* x, double* y, int n, int fr);

void main(int argc, char* argv[]) 
{
	char inFile[256]; //読み込みファイル名を格納する変数
	FILE* ifp;
	short dataIn[FRAMESIZE];//wavファイルから読み込むデータを入れる配列
	double dDataInX[FRAMESIZE], dDataInY[FRAMESIZE];//FFTの計算に使う配列はdobuleで宣言

	/*各種変数*/
	int len, sampSize, outlen;
	unsigned long fs;
	unsigned short chNum;
	int fNum;

	if (argc < 2) {
		printf("読み込むファイル名を指定してください");
		exit(0);
	}
	/*コマンドラインからファイル名の取得*/
	strcpy(inFile, argv[1]);
	/*ファイル名の確認*/
	printf("input file is %s.\n", inFile);

	ifp = fopen(inFile, "rb");//ファイルを開きポインタをifpに入れる

	/*Wavのヘッダを読む*/
	if (readWavHead(ifp, &len, &fs, &chNum, &sampSize) < 0) {
		puts("入力ファイルのヘッダがうまく読めません");
		exit(-1);
	}
	/*読み込んだパラメータをコンソールに出力*/
	printf("ファイルの長さは %d サンプル\n", len);
	printf("サンプリング周波数は %d Hz\n", fs);
	printf("チャネル数は %d\n", chNum);
	printf("1サンプルのビット数は %d ビット\n", sampSize);

	fNum = (int)floor(len / FRAMESIZE); //実行する回数(フレーム数)の計算

	for (int i = 0; i < fNum; i++) {
		fread(dataIn, sizeof(short), FRAMESIZE, ifp); //ifpからデータをフレーム長分読み込む

		for (int j = 0; j < FRAMESIZE; j++) {
			dDataInX[j] = (double)dataIn[j];  //CT_fftはdoubleの変数を使うので、doubleの変数に代入。
			dDataInY[j] = 0.0; //虚数部は0.0を入れる。

		}
		CT_fft(dDataInX, dDataInY, FRAMESIZE, 1); //FFTの実行
		for (int j = 0; j < FRAMESIZE; j++) {
			printf("%lf\n", sqrt(pow(dDataInX[j], 2) + pow(dDataInY[j], 2))); //FFT結果の表示。dDataはdoubleなの%lfで表示する。
		}
		printf("\n");
	}

	fclose(ifp);

}