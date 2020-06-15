/*
2020-5-0.c

Coding 'Shift JIS'

Wav�t�@�C����ǂݍ���ŁAFFT����B
FFT�̌��ʂ��tFFT���AWav�t�@�C���ɏo�͂���B

�R���p�C�����@��
cl 2020-5-0.c readWavHead.c writeWavHead.c CT_fft.c

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
	char inFile[256]; // �ǂݍ��݃t�@�C�������i�[����ϐ�
	char outFile[256]; // �������݃t�@�C�������i�[����ϐ�
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
		printf("����������܂���B�ǂݍ��ރt�@�C�����Ə������ރt�@�C�������w�肵�Ă�������");
		exit(0);
	}

	/* �R�}���h���C������t�@�C�����̎擾 */
	strcpy(inFile, argv[1]);
	strcpy(outFile, argv[2]);
	/* �t�@�C�����̊m�F */
	printf("input file is %s.\n", inFile);
	printf("output file is %s.\n", outFile);

	ifp = fopen(inFile, "rb");
	ofp = fopen(outFile, "wb");

	/* Wav�̃w�b�_��ǂ� */
	if (readWavHead(ifp, &len, &fs, &chNum, &sampSize) < 0) {
		puts("���̓t�@�C���̃w�b�_�����܂��ǂ߂܂���");
		exit(-1);
	}
	/* �ǂݍ��񂾃p�����[�^���R���\�[���ɏo�� */
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
			dDataInX[j] = (double)dataIn[j];  // CT_fft��double�̕ϐ����g���̂ŁAdouble�̕ϐ��ɑ�����Ă��������B
			dDataInY[j] = 0.0; // ��������0.0�����Ă����܂��B

		}
		CT_fft(dDataInX, dDataInY, FRAMESIZE, 1); //FFT
		for (int j = 0; j < FRAMESIZE; j++) {
			dDataOutX[j] = dDataInX[j];
			dDataOutY[j] = dDataInY[j];

		}
		/* dDataOutX��dDataOutY�ւ̑���͔͈͂𕪂��čs���ׂ������A����͈͈ȊO�́A���ׂ�
		���̂܂܃R�s�[���Ă悢�̂ŁA��L��for���͂��̂܂܎c���āA���̌�ɁA����͈͂���������������
		for�������������v���O�����Ƃ��Ă͊y�B*/
		/* �Ȃ̂ŁA���̉���for�͈̔͂��قȂ�for���[�v����莑�������ċL�ڂ��邱�Ƃ����߂� */
//		for(int j = N1; j < N2; j++){
//			dDataOutX[j] = GAIN * dDataOutX[j];
//			dDataOutY[j] = GAIN * dDataOutY[j];
//		}

		CT_fft(dDataOutX, dDataOutY, FRAMESIZE, -1); //�tFFT�A�S�ڂ̈�����-1�̏ꍇ�tFFT�ƂȂ�B




		for (int j = 0; j < FRAMESIZE; j++) {
			printf("%lf,%lf\n", dDataOutX[j], dDataOutY[j]);// �tFFT�̌��ʂ��������Ȃ�A������o�͂���
			dataOutX[j] = (short)dDataOutX[j]; 				// Wav�t�@�C���ɏ����o�����߂�short�Ɍ^�ϊ�����
			dataOutY[j] = (short)dDataOutY[j]; 				// Wav�t�@�C���ɏ����o�����߂�short�Ɍ^�ϊ�����
			printf("%d,%d\n", dataOutX[j], dataOutY[j]); 	// �t�@�C���ɏ����o�����l���������Ȃ�A������o�͂���
		}
		printf("\n");

		fwrite(dataOutX, sizeof(short), FRAMESIZE, ofp); // dataOutX�݂̂������o���Ă��邪�AY�̈����͏ꍇ�ɂ��
	}

	fclose(ifp);
	fclose(ofp);

}