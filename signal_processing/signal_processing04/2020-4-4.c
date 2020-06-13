/*
2020-4-4.c

Wavファイルを読み込んで、FFTする。
FFTの結果を逆FFTし、Wavファイルに出力する。

未完成版。課題K08はこれを元に加筆すればよい。

コンパイル方法は
cl 2020-4-4.c readWavHead.c writeWavHead.c CT_fft.c

by mokam@cis
*/

#include<stdio.h>
#include<math.h>

#define FRAMESIZE 256

/*こういう関数を使うよ、という宣言*/
int readWavHead(FILE* fp, int* dataLength, unsigned long* fs, unsigned short* chNum, int* sampSize);
int writeWavHead(FILE* fp, int dataLength, unsigned long fs, unsigned short  chNum, int sampSize);

int CT_fft(double* x, double* y, int n, int fr);

void main(int argc, char* argv[])
{
	char inFile[256]; //読み込みファイル名を格納する変数
	char outFile[256]; //書き込みファイル名を格納する変数
	FILE *ifp, *ofp;
	short dataIn[FRAMESIZE];
	short dataOutX[FRAMESIZE], dataOutY[FRAMESIZE];
	double dDataInX[FRAMESIZE], dDataInY[FRAMESIZE];
	double dDataOutX[FRAMESIZE], dDataOutY[FRAMESIZE];

	int len, sampSize, outlen;
	unsigned long fs;
	unsigned short chNum;
	int fNum;

	if (argc < 3) {
		printf("読み込むファイル名と出力するファイル名を指定してください");
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
	if (readWavHead(ifp, &len, &fs, &chNum, &sampSize) < 0) {
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
	/*
	書き込み用のwavファイルのヘッダを書き込む。データのサイズまで,
	書き込んでいるので、この後はかならずoutlenと同じだけfwriteすること
	*/
	writeWavHead(ofp, outlen, fs, chNum, sampSize);

	for (int i = 0; i < fNum; i++) {
		fread(dataIn, sizeof(short), FRAMESIZE, ifp);

		for (int j = 0; j < FRAMESIZE; j++) {
			dDataInX[j] = (double)dataIn[j];  //CT_fftはdoubleの変数を使うので、doubleの変数に代入してください。
			dDataInY[j] = 0.0; //虚数部は0.0を入れておきます。

		}
		/*FFTする*/
		CT_fft(dDataInX, dDataInY, FRAMESIZE, 1); //FFT

		for (int j = 0; j < FRAMESIZE; j++) {
			dDataOutX[j] = dDataInX[j];
			dDataOutY[j] = dDataInY[j];

		}
		/*逆FFTする*/
		CT_fft(dataOutX, dataOutY, FRAMESIZE, -1);
		//逆FFTの文が足りないので追記すること。
		//逆FFT、４つ目の引数が-1の場合逆FFTとなる。

		for (int j = 0; j < FRAMESIZE; j++) {
			//Wavファイルに書き出すために、dDataOutX[j]をshortに型変換し、dataOutXに代入する部分の追記が必要
			dataOutX[j] = (short)dataOutX[j];
		}

		fwrite(dataOutX, sizeof(short), FRAMESIZE, ofp); //今回はdataOutXのみを書き出しているが、Yの扱いは場合により必要になる場合もある。
		
	}

	fclose(ifp);
	fclose(ofp);

}