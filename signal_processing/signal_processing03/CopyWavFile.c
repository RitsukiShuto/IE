/*
Edited by RitsukiShuto on 2020/06/08.
CopyWavFile.c

Cding "Shift JIS"

Wav�t�@�C�����J���ăt�@�C���̃t�H�[�}�b�g���R���\�[���ɏo�͂���
�f�[�^��wav�t�@�C���ɏo�͂���

�o�̓t�@�C���̓w�b�_�������ďI�����Ă�̂ŁA���̂܂܂��Ɛ�����wav�t�@�C���͂ł��Ȃ�
���g��ǋL���邱��

�R���p�C�����@��
cl 2020-2-6.c readWavHead.c writeWavHead.c

by mokam@cis
*/

#include <stdio.h>
#include <math.h>

#define FRAMESIZE 256

/*���������֐����g����A�Ƃ����錾*/
int readWavHead(FILE *fp, int *dataLength, unsigned long *fs, unsigned short *chNum, int *sampSize);
int writeWavHead(FILE *fp, int dataLength, unsigned long fs, unsigned short chNum, int sampSize);

void main(int argc, char *argv[])
{
	//�ϐ��̐錾
	char inFile[256];  //�ǂݍ��݃t�@�C����
	char outFile[256]; //�ǂݍ��݃t�@�C����
	FILE *ifp, *ofp;
	short dataIn[FRAMESIZE];
	short dataOut[FRAMESIZE];
	int len, sampSize, outlen;
	unsigned long fs;
	unsigned short chNum;
	int fNum;

	// �R�}���h���C���������`�F�b�N
	if (argc < 3)
	{
		puts("�R�}���h���C������������܂���B");
		exit(0);
	}

	// �R�}���h���C������t�@�C�������擾
	strcpy(inFile, argv[1]);
	strcpy(outFile, argv[2]);

	//�t�@�C�����J��
	ifp = fopen(inFile, "rb");
	ofp = fopen(outFile, "wb");

	/*Wav�̃w�b�_��ǂ�*/
	if (readWavHead(ifp, &len, &fs, &chNum, &sampSize) < 0)
	{
		puts("���̓t�@�C���̃w�b�_�����܂��ǂ߂܂���");
		exit(-1);
	}
	/*�ǂݍ��񂾃p�����[�^���R���\�[���ɏo��*/
	printf("�t�@�C���̒����� %d �T���v��\n", len);
	printf("�T���v�����O���g���� %d Hz\n", fs);
	printf("�`���l������ %d\n", chNum);
	printf("1�T���v���̃r�b�g���� %d �r�b�g\n", sampSize);

	//�f�[�^�����烋�[�v�̉񐔂��v�Z�B
	fNum = (int)floor(len / FRAMESIZE);

	//fNum���珑�����݃t�@�C���̃T�C�Y���v�Z
	outlen = fNum * FRAMESIZE;

	writeWavHead(ofp, outlen, fs, chNum, sampSize);

	for (int i = 0; i < fNum; i++)
	{
		fread(dataIn, sizeof(short), FRAMESIZE, ifp);
		for (int j = 0; j < FRAMESIZE; j++)
		{
			dataOut[j] = dataIn[j];
		}
		fwrite(dataOut, sizeof(short), FRAMESIZE, ofp);
	}

	fclose(ifp);
	fclose(ofp);
}
