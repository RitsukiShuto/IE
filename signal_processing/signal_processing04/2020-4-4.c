/*
2020-4-4.c

Wav�t�@�C����ǂݍ���ŁAFFT����B
FFT�̌��ʂ��tFFT���AWav�t�@�C���ɏo�͂���B

�������ŁB�ۑ�K08�͂�������ɉ��M����΂悢�B

�R���p�C�����@��
cl 2020-4-4.c readWavHead.c writeWavHead.c CT_fft.c

by mokam@cis
*/

#include<stdio.h>
#include<math.h>

#define FRAMESIZE 256

/*���������֐����g����A�Ƃ����錾*/
int readWavHead(FILE* fp, int* dataLength, unsigned long* fs, unsigned short* chNum, int* sampSize);
int writeWavHead(FILE* fp, int dataLength, unsigned long fs, unsigned short  chNum, int sampSize);

int CT_fft(double* x, double* y, int n, int fr);

void main(int argc, char* argv[])
{
	char inFile[256]; //�ǂݍ��݃t�@�C�������i�[����ϐ�
	char outFile[256]; //�������݃t�@�C�������i�[����ϐ�
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
		printf("�ǂݍ��ރt�@�C�����Əo�͂���t�@�C�������w�肵�Ă�������");
		exit(0);
	}
	/*�R�}���h���C������t�@�C�����̎擾*/
	strcpy(inFile, argv[1]);
	strcpy(outFile, argv[2]);
	/*�t�@�C�����̊m�F*/
	printf("input file is %s.\n", inFile);
	printf("output file is %s.\n", outFile);

	ifp = fopen(inFile, "rb");
	ofp = fopen(outFile, "wb");

	/*Wav�̃w�b�_��ǂ�*/
	if (readWavHead(ifp, &len, &fs, &chNum, &sampSize) < 0) {
		puts("���̓t�@�C���̃w�b�_�����܂��ǂ߂܂���");
		exit(-1);
	}
	/*�ǂݍ��񂾃p�����[�^���R���\�[���ɏo��*/
	printf("�t�@�C���̒����� %d �T���v��\n", len);
	printf("�T���v�����O���g���� %d Hz\n", fs);
	printf("�`���l������ %d\n", chNum);
	printf("1�T���v���̃r�b�g���� %d �r�b�g\n", sampSize);

	fNum = (int)floor(len / FRAMESIZE);
	outlen = fNum * FRAMESIZE;
	/*
	�������ݗp��wav�t�@�C���̃w�b�_���������ށB�f�[�^�̃T�C�Y�܂�,
	��������ł���̂ŁA���̌�͂��Ȃ炸outlen�Ɠ�������fwrite���邱��
	*/
	writeWavHead(ofp, outlen, fs, chNum, sampSize);

	for (int i = 0; i < fNum; i++) {
		fread(dataIn, sizeof(short), FRAMESIZE, ifp);

		for (int j = 0; j < FRAMESIZE; j++) {
			dDataInX[j] = (double)dataIn[j];  //CT_fft��double�̕ϐ����g���̂ŁAdouble�̕ϐ��ɑ�����Ă��������B
			dDataInY[j] = 0.0; //��������0.0�����Ă����܂��B

		}
		/*FFT����*/
		CT_fft(dDataInX, dDataInY, FRAMESIZE, 1); //FFT

		for (int j = 0; j < FRAMESIZE; j++) {
			dDataOutX[j] = dDataInX[j];
			dDataOutY[j] = dDataInY[j];

		}
		/*�tFFT����*/
		CT_fft(dataOutX, dataOutY, FRAMESIZE, -1);
		//�tFFT�̕�������Ȃ��̂ŒǋL���邱�ƁB
		//�tFFT�A�S�ڂ̈�����-1�̏ꍇ�tFFT�ƂȂ�B

		for (int j = 0; j < FRAMESIZE; j++) {
			//Wav�t�@�C���ɏ����o�����߂ɁAdDataOutX[j]��short�Ɍ^�ϊ����AdataOutX�ɑ�����镔���̒ǋL���K�v
			dataOutX[j] = (short)dataOutX[j];
		}

		fwrite(dataOutX, sizeof(short), FRAMESIZE, ofp); //�����dataOutX�݂̂������o���Ă��邪�AY�̈����͏ꍇ�ɂ��K�v�ɂȂ�ꍇ������B
		
	}

	fclose(ifp);
	fclose(ofp);

}