/*
* IE Sound Processing 02
* 2020/05/18. 16:03:45

信号処理第一回　課題K02のための参考ファイル。
説明資料を読み、足りない部分を追記し完成させること。


Wavファイルを開いてファイルのフォーマットをコンソールに出力する

コンパイルはVisual StudioのDeveloper Command Promptで
cl 2019_3-1-5.c readWavHead.c

by mokam@cis
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/*こういう関数を使うよ、という宣言*/
int readWavHead(FILE *fp, int *dataLength, unsigned long *fs, unsigned short *chNum, int *sampSize);

void main()
{
	char inFile[] = "VoiceMono.wav"; //読み込みファイル名
	FILE *ifp;
	int len, sampSize;
	unsigned long fs;
	unsigned short chNum;
	short dataln;

	ifp = fopen(inFile, "rb");

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

	for (int i = 0; i < 8000; i++)
	{
		fread(&dataln, sizeof(short), 1, ifp); // データの読み込み
		printf("%d\n", dataln);				   // データを出力
	}

	fclose(ifp);
}
