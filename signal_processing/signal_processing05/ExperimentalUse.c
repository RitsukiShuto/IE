/*
Created by RitsukiShuto on 2020/06/21-20:50:56.
Coding 'Shift JIS'

--�g����--
    *�����Ɏg��wav�t�@�C���A�w�b�_�t�@�C���A���̑��̈ˑ��t�@�C�������̃R�[�h�Ɠ����f�B���N�g���ɒu��
    *���s���ɂ������̃p�����[�^�𒀎���`����
    *���s���ʂ͂��ׂ�csv�t�@�C���ŋA���Ă���

--�R���p�C��--
    cl ExperimentalUse.c readWavHead.c writeWavHead.c CT_fft.c
    link \out:ExUse.exe ExperimentalUse.obj readWavHead.obj writeWavHead.obj CT_fft.obj
    ExUse.exe <����wav�t�@�C��>

*/
#include<stdio.h>
#include<stdio.h>
#include<stdlib.h>

// prototype�錾
int readWavHead(FILE* fp, int* dataLength, unsigned long* fs, unsigned short* chNum, int* sampSize);
int writeWavHead(FILE* fp, int dataLength, unsigned long fs, unsigned short  chNum, int sampSize);
int CT_fft(double* x, double* y, int n, int fr);

void main(int argc, char *argv[]){
    char ioFile[256];   // �o�̓t�@�C���̑f
    char inFile[256];   // ���̓t�@�C��
    char outFile[256];  // �o�̓t�@�C��
    char csvFile[256];  // CSV�t�@�C��
    FILE *ifp, *ofp;

    int FRAMESIZE;

    int len, sampSize, outlen;
    unsigned long fs;
    unsigned short chNum;
    int fNum;

    double GAIN;
    double sfpf;    // Sampling frequency per frame
    int cut_Hz_N1, cut_Hz_N2;
    int N1, N2;

    char yN = 'n';

    if(argc < 2){
        puts("�R�}���h���C������������܂���");
        exit(0);
    }

	strcpy(inFile, argv[1]);    // ���̓t�@�C�����R�s�[
    strcpy(ioFile, argv[1]);

    for(int i = 0;i < 256;i++){
        if(ioFile[i] == '.'){  // '.'������
            for(int j = i;j < length(inFile);j++){
                // �g���q��'\0'�Ŗ��߂�
                ioFile[j] = '\0';
                if(ioFile[j + 1] == '\0'){  // Null����������
                    break;
                }
            }
        }
    }

    // �o�̓t�@�C�������쐬
    sprintf(csvFile, "PowSpectorum", ioFile, ".csv");
    sprintf(outFile, "Out", ioFile, ".wav");

	printf("input file is %s.\n", inFile);
	printf("output file is %s.\n", outFile);
    printf("csv file is %s.\n", csvFile);

	ifp = fopen(inFile, "rb");
	ofp = fopen(outFile, "wb");

    // wav�w�b�_��ǂݍ���
	if (readWavHead(ifp, &len, &fs, &chNum, &sampSize) < 0) {
		puts("���̓t�@�C���̃w�b�_�����܂��ǂ߂܂���");
		exit(-1);
	}

	// wav�p�����[�^���o��
	printf("�t�@�C���̒����� %d �T���v��\n", len);
	printf("�T���v�����O���g���� %d Hz\n", fs);
	printf("�`���l������ %d\n", chNum);
	printf("1�T���v���̃r�b�g���� %d �r�b�g\n", sampSize);

    while(yN == 'N' || 'n'){
        puts("�e��p�����[�^�����");
        printf("FRAMESIZE = ");     scanf("%d", &FRAMESIZE);
        printf("GAIN = ");          scanf("%lf", &GAIN);
        printf("�������������g���ш�̍ŏ��l�����[Hz]: ");
        scanf_s("%lf", &cut_Hz_N1);
        printf("�������������g���ш�̍ő�l�����[Hz]: ");
	    scanf_s("%lf", &cut_Hz_N2);

        sfpf = fs / FRAMESIZE;

        N1 = cut_Hz_N1 / sfpf;
	    N2 = cut_Hz_N2 / sfpf;

        printf("FRAMESIZE: %d\n", FRAMESIZE);
        printf("GAIN: %d\n", GAIN);
    	printf("%lf[Hz]����%lf[Hz]������\n", cut_Hz_N1, cut_Hz_N2);
	    printf("����Ώۂ̗v�f�ԍ���[%d]����[%d]\n", N1, N2);

        puts("�ȏ�̃p�����[�^�Ŏ��s���Ă�낵���ł���(y/N):");
        scanf("%c", &yN);
    }
}
