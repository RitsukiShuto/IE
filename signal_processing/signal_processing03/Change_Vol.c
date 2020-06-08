/*
Edited by RitsukiShuto on 2020/06/08.
2020-3-4.c a.k.a. Change_Vol.c

Coding "Shift JIS"

wavファイルを読み、関数でデータを操作し、wavファイルに出力する
未完成なので、資料を見て完成させると良い

コンパイル方法は
cl 2020-3-4.c readWavHead.c writeWavHead.c

by mokam@cis
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FRAMESIZE 256

/*こういう関数を使うよ、という宣言*/
int readWavHead(FILE *fp, int *dataLength, unsigned long *fs, unsigned short *chNum, int *sampSize);
int writeWavHead(FILE *fp, int dataLength, unsigned long fs, unsigned short chNum, int sampSize);
int volChange(short *in, short *out, float val, int len);

void main(int argc, char *argv[])
{
	char inFile[256];  //読み込みファイル名を格納する変数
	char outFile[256]; //出力ファイル名を格納する変数
	FILE *ifp, *ofp;
	short dataIn[FRAMESIZE];
	short dataOut[FRAMESIZE];
	int len, sampSize, outlen;
	unsigned long fs;
	unsigned short chNum;
	int fNum;

	/*コマンドラインの引数が足りないと動かないので、数をチェックする*/
	if (argc < 3)
	{
		printf("引数に二つ以上の文字列を書いてください\n");
		exit(0);
	}
	/*コマンドラインからファイル名の取得*/
	strcpy(inFile, argv[1]);
	strcpy(outFile, argv[2]);
	/*ファイル名の確認*/
	printf("input file is %s.\n", inFile);
	printf("output file is %s.\n", outFile);

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

	fNum = (int)floor(len / FRAMESIZE);
	outlen = fNum * FRAMESIZE;

	writeWavHead(ofp, outlen, fs, chNum, sampSize);

	for (int i = 0; i < fNum; i++)
	{
		fread(dataIn, sizeof(short), FRAMESIZE, ifp);
		volChange(dataIn, dataOut, 0.5, FRAMESIZE);
		fwrite(dataOut, sizeof(short), FRAMESIZE, ofp);
	}

	fclose(ifp);
	fclose(ofp);
}

int volChange(short *in, short *out, float val, int len)
{
	float ff;
	int i;

	for (i = 0; i < len; i++)
	{
		ff = *in * val;
		*out = (short)floor(ff);
		in++;
		out++;
	}

	return i;
}