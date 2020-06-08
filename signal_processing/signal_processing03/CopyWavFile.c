/*
Edited by RitsukiShuto on 2020/06/08.
CopyWavFile.c

Cding "Shift JIS"

Wavファイルを開いてファイルのフォーマットをコンソールに出力する
データはwavファイルに出力する

出力ファイルはヘッダを書いて終了してるので、このままだと正しいwavファイルはできない
中身を追記すること

コンパイル方法は
cl 2020-2-6.c readWavHead.c writeWavHead.c

by mokam@cis
*/

#include <stdio.h>
#include <math.h>

#define FRAMESIZE 256

/*こういう関数を使うよ、という宣言*/
int readWavHead(FILE *fp, int *dataLength, unsigned long *fs, unsigned short *chNum, int *sampSize);
int writeWavHead(FILE *fp, int dataLength, unsigned long fs, unsigned short chNum, int sampSize);

void main(int argc, char *argv[])
{
	//変数の宣言
	char inFile[256];  //読み込みファイル名
	char outFile[256]; //読み込みファイル名
	FILE *ifp, *ofp;
	short dataIn[FRAMESIZE];
	short dataOut[FRAMESIZE];
	int len, sampSize, outlen;
	unsigned long fs;
	unsigned short chNum;
	int fNum;

	// コマンドライン引数をチェック
	if (argc < 3)
	{
		puts("コマンドライン引数が足りません。");
		exit(0);
	}

	// コマンドラインからファイル名を取得
	strcpy(inFile, argv[1]);
	strcpy(outFile, argv[2]);

	//ファイルを開く
	ifp = fopen(inFile, "rb");
	ofp = fopen(outFile, "wb");

	/*Wavのヘッダを読む*/
	if (readWavHead(ifp, &len, &fs, &chNum, &sampSize) < 0)
	{
		puts("入力ファイルのヘッダがうまく読めません");
		exit(-1);
	}
	/*読み込んだパラメータをコンソールに出力*/
	printf("ファイルの長さは %d サンプル\n", len);
	printf("サンプリング周波数は %d Hz\n", fs);
	printf("チャネル数は %d\n", chNum);
	printf("1サンプルのビット数は %d ビット\n", sampSize);

	//データ長からループの回数を計算。
	fNum = (int)floor(len / FRAMESIZE);

	//fNumから書き込みファイルのサイズを計算
	outlen = fNum * FRAMESIZE;

	writeWavHead(ofp, outlen, fs, chNum, sampSize);

	for (int i = 0; i < fNum; i++)
	{
		fread(dataIn, sizeof(short), FRAMESIZE, ifp);
		for (int j = 0; j < FRAMESIZE; j++)
		{
			dataOut[j] = dataIn[j];
		}
		fwrite(dataOut, sizeof(short), FRAMESIZE, ofp);
	}

	fclose(ifp);
	fclose(ofp);
}
