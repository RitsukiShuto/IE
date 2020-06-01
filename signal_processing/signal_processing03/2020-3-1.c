/*
2020-3-1.c

コマンドラインから変数名を取得するプログラム例


コンパイル方法は
cl 2020-3-1.c

by mokam@cis
*/

#include<stdio.h>
#include<stdlib.h>

#define SAMPLESIZE 8000  //読み込むデータの数

void main(int argc, char* argv[]) {
	char inFile[256]; //読み込みファイル名を格納する変数
	char outFile[256]; //出力ファイル名を格納する変数

	/*コマンドラインの引数が足りないと動かないので、数をチェックする*/
	if (argc < 3) {
		printf("引数に二つ以上の文字列を書いてください\n");
		exit(0);
	}
	/*コマンドラインからファイル名の取得*/
	strcpy(inFile, argv[1]);
	strcpy(outFile, argv[2]);
	/*ファイル名の確認*/
	printf("input file is %s.\n", inFile);
	printf("output file is %s.\n", outFile);



}