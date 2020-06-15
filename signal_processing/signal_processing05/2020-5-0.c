/*
2020-5-0.c

Coding 'Shift JIS'

Wavファイルを読み込んで、FFTする。
FFTの結果を逆FFTし、Wavファイルに出力する。

コンパイル方法は
cl 2020-5-0.c readWavHead.c writeWavHead.c CT_fft.c

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
	char inFile[256]; // 読み込みファイル名を格納する変数
	char outFile[256]; // 書き込みファイル名を格納する変数
	FILE *ifp, *ofp;
	short dataIn[FRAMESIZE];
	short dataOutX[FRAMESIZE], dataOutY[FRAMESIZE];
	double dDataInX[FRAMESIZE], dDataInY[FRAMESIZE];
	double dDataOutX[FRAMESIZE], dDataOutY[FRAMESIZE];

	int len, sampSize, outlen;
	unsigned long fs;
	unsigned short chNum;
	int fNum;

	double GAIN;
	double N1, N2;

	if (argc < 3) {
		printf("引数が足りません。読み込むファイル名と書き込むファイル名を指定してください");
		exit(0);
	}

	/* コマンドラインからファイル名の取得 */
	strcpy(inFile, argv[1]);
	strcpy(outFile, argv[2]);
	/* ファイル名の確認 */
	printf("input file is %s.\n", inFile);
	printf("output file is %s.\n", outFile);

	ifp = fopen(inFile, "rb");
	ofp = fopen(outFile, "wb");

	/* Wavのヘッダを読む */
	if (readWavHead(ifp, &len, &fs, &chNum, &sampSize) < 0) {
		puts("入力ファイルのヘッダがうまく読めません");
		exit(-1);
	}
	/* 読み込んだパラメータをコンソールに出力 */
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
			dDataInX[j] = (double)dataIn[j];  // CT_fftはdoubleの変数を使うので、doubleの変数に代入してください。
			dDataInY[j] = 0.0; // 虚数部は0.0を入れておきます。

		}
		CT_fft(dDataInX, dDataInY, FRAMESIZE, 1); //FFT
		for (int j = 0; j < FRAMESIZE; j++) {
			dDataOutX[j] = dDataInX[j];
			dDataOutY[j] = dDataInY[j];

		}
		/* dDataOutXとdDataOutYへの代入は範囲を分けて行うべきだが、ある範囲以外は、すべて
		そのままコピーしてよいので、上記のfor文はそのまま残して、この後に、ある範囲だけを小さくする
		forを書いた方がプログラムとしては楽。*/
		/* なので、この下にforの範囲が異なるforループを作り資料を見て記載することを勧める */
//		for(int j = N1; j < N2; j++){
//			dDataOutX[j] = GAIN * dDataOutX[j];
//			dDataOutY[j] = GAIN * dDataOutY[j];
//		}

		CT_fft(dDataOutX, dDataOutY, FRAMESIZE, -1); //逆FFT、４つ目の引数が-1の場合逆FFTとなる。




		for (int j = 0; j < FRAMESIZE; j++) {
			printf("%lf,%lf\n", dDataOutX[j], dDataOutY[j]);// 逆FFTの結果を見たいなら、これを出力する
			dataOutX[j] = (short)dDataOutX[j]; 				// Wavファイルに書き出すためにshortに型変換する
			dataOutY[j] = (short)dDataOutY[j]; 				// Wavファイルに書き出すためにshortに型変換する
			printf("%d,%d\n", dataOutX[j], dataOutY[j]); 	// ファイルに書き出す数値を見たいなら、これを出力する
		}
		printf("\n");

		fwrite(dataOutX, sizeof(short), FRAMESIZE, ofp); // dataOutXのみを書き出しているが、Yの扱いは場合による
	}

	fclose(ifp);
	fclose(ofp);

}