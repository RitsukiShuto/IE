/*
2020-2-6.c
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

void main()
{
	//変数の宣言
	char inFile[] = "VoiceMono.wav";	  //読み込みファイル名
	char outFile[] = "VoiceMono_out.wav"; //読み込みファイル名
	FILE *ifp, *ofp;
	short dataIn[FRAMESIZE];
	short dataOut[FRAMESIZE];
	int len, sampSize, outlen;
	unsigned long fs;
	unsigned short chNum;
	int fNum;

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

	/*
	書き込み用のwavファイルのヘッダを書き込む。データのサイズまで,
	書き込んでいるので、この後はかならずoutlenと同じだけfwriteすること
	*/
	writeWavHead(ofp, outlen, fs, chNum, sampSize);

	for (int i = 0; i < fNum; i++)
	{
		fread(&dataIn[i], sizeof(short), 1, ifp);
		for (int j = 0; j < FRAMESIZE; j++)
		{
			dataOut[j] = dataIn[j];
		}
		outFile[i] = fwrite(dataOut, sizeof(short), 256, ofp);
	}

	fclose(ifp);
	fclose(ofp);
}
