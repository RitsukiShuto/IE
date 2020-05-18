/*
* IE Sound Processing 02
* 2020/05/18. 16:03:45

�M����������@�ۑ�K02�̂��߂̎Q�l�t�@�C���B
����������ǂ݁A����Ȃ�������ǋL�����������邱�ƁB


Wav�t�@�C�����J���ăt�@�C���̃t�H�[�}�b�g���R���\�[���ɏo�͂���

�R���p�C����Visual Studio��Developer Command Prompt��
cl 2019_3-1-5.c readWavHead.c

by mokam@cis
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/*���������֐����g����A�Ƃ����錾*/
int readWavHead(FILE *fp, int *dataLength, unsigned long *fs, unsigned short *chNum, int *sampSize);

void main()
{
	char inFile[] = "VoiceMono.wav"; //�ǂݍ��݃t�@�C����
	FILE *ifp;
	int len, sampSize;
	unsigned long fs;
	unsigned short chNum;
	short dataln;

	ifp = fopen(inFile, "rb");

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

	for (int i = 0; i < 8000; i++)
	{
		fread(&dataln, sizeof(short), 1, ifp); // �f�[�^�̓ǂݍ���
		printf("%d\n", dataln);				   // �f�[�^���o��
	}

	fclose(ifp);
}
