/*
* IE Signal Processing #2
* 2020/05/18. 15:44:56

信号処理第一回　課題K01のための参考ファイル。
説明資料を読み、足りない部分を追記し完成させること。

by mokam@cis
*/
#include <stdio.h>

void main()
{
	char inFile[] = "1000Sine-8k.raw"; // 読み込みファイル名
	FILE *ifp;						   // ファイルに紐づけるポインタ. どこまで読んだかも記録されている
	short dataln;					   // 読み込んむデータの領域(1要素)

	ifp = fopen(inFile, "rb"); //ファイルを読み込みでモードで開く。これ以降はifpでファイルを操作する

	for (int i = 0; i < 8000; i++)
	{
		fread(&dataln, sizeof(short), 1, ifp); // ファイルを読み出し => 'dataln'に格納される
		printf("%d\n", dataln);				   // データを出力
	}

	fclose(ifp); //ファイルを閉じる
}