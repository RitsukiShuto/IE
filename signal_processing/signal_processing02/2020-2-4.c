/*
2020-2-4.c
Wavファイルを開いてファイルのフォーマットをコンソールに出力する
データを読み込んでコンソールに出力をする

一部を取り除いてるのでこのままでは動作しない。授業資料を読み、
完成させること

コンパイル方法は
cl 2020-2-4.c readWavHead.c 

by mokam@cis
*/

#include<stdio.h>
#include<math.h>

//フレーム長を決めておく。以下FRAMESIZEと書くと256に置き換わるようになる。
#define FRAMESIZE 256

/*こういう関数を使うよ、という宣言*/
int readWavHead(FILE* fp, int* dataLength, unsigned long* fs, unsigned short* chNum, int* sampSize);

void main()
{
	/*変数の宣言*/
	char inFile[] = "VoiceMono.wav"; //読み込みファイル名
	FILE *ifp, *ofp;
	short dataIn[FRAMESIZE]; //読み込んだデータを入れる配列
	//各種変数
	int len, sampSize;
	unsigned long fs;
	unsigned short chNum;
	int fNum;

	/*ファイルを開く*/
	ifp = fopen(inFile, "rb");

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

	/*データ長から繰り返しの数を計算する*/
	/*ここにfNumを計算する式を入れると良い*/

	
	/*あらかじめ計算したループの回数だけ繰り返す*/
	for (int i = 0; i < fNum; i++) {

		/*フレーム単位でまとめてデータを読み込む*/
		//ここにfread文をフレーム単位で読み込むように追記する		
		
		/*読み込んだデータをコンソールに出力*/
		for (int j = 0; j < FRAMESIZE; j++) {
			printf("%d\n", dataIn[j]);
		}
	}
	/*ファイルを閉じる*/
	fclose(ifp);

}
