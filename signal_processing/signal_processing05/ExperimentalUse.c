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
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// prototype�錾
int readWavHead(FILE *fp, int *dataLength, unsigned long *fs, unsigned short *chNum, int *sampSize);
int writeWavHead(FILE *fp, int dataLength, unsigned long fs, unsigned short chNum, int sampSize);
int CT_fft(double *x, double *y, int n, int fr);

void main(int argc, char *argv[])
{
    char ioFile[256];      // �o�̓t�@�C���̑f
    char inFile[256];      // ���̓t�@�C��
    char outFile[256];     // �o�̓t�@�C��
    char csvspctFile[256]; // CSV�t�@�C��
    char csvinfoFile[256];
    char csvvolFile[256];

    FILE *ifp, *ofp, *fp_info, *fp_spect, *fp_vol;

    int FRAMESIZE;

    int len, sampSize, outlen;
    unsigned long fs;
    unsigned short chNum;
    int fNum;

    double GAIN;
    double sfpf; // Sampling frequency per frame
    int cut_Hz_N1, cut_Hz_N2;
    int N1, N2;

    int yN;

    if (argc < 2)
    {
        puts("�R�}���h���C������������܂���");
        exit(0);
    }

    strcpy(inFile, argv[1]); // ���̓t�@�C�����R�s�[
    strcpy(ioFile, argv[1]);

    for (int i = 0; i < 256; i++)
    {
        if (ioFile[i] == '.')
        { // '.'������
            for (int j = i; j < 256; j++)
            {
                // �g���q��'\0'�Ŗ��߂�
                ioFile[j] = '\0';
                if (ioFile[j + 1] == '\0')
                { // Null����������
                    break;
                }
            }
        }
    }

    // �o�̓t�@�C�������쐬
    sprintf(outFile, "Out%s.wav", ioFile);
    sprintf(csvinfoFile, "info%s.csv", ioFile);
    sprintf(csvspctFile, "PowSpect%s.csv", ioFile);
    sprintf(csvvolFile, "Vol%s.csv", ioFile);

    printf("input file is %s.\n", inFile);
    printf("output file is %s.\n", outFile);

    ifp = fopen(inFile, "rb");
    ofp = fopen(outFile, "wb");

    // wav�w�b�_��ǂݍ���
    if (readWavHead(ifp, &len, &fs, &chNum, &sampSize) < 0)
    {
        puts("���̓t�@�C���̃w�b�_�����܂��ǂ߂܂���");
        exit(-1);
    }

    fp_info = fopen(csvinfoFile, "w");
    fp_spect = fopen(csvspctFile, "w");
    fp_vol = fopen(csvvolFile, "w");

    // wav�p�����[�^���o��
    printf("�t�@�C���̒����� %d �T���v��\n", len);
    printf("�T���v�����O���g���� %d Hz\n", fs);
    printf("�`���l������ %d\n", chNum);
    printf("1�T���v���̃r�b�g���� %d �r�b�g\n\n", sampSize);

    fprintf(fp_info, "�t�@�C���̒����� %d �T���v��\n", len);
    fprintf(fp_info, "�T���v�����O���g���� %d Hz\n", fs);
    fprintf(fp_info, "�`���l������ %d\n", chNum);
    fprintf(fp_info, "1�T���v���̃r�b�g���� %d �r�b�g\n\n", sampSize);

    puts("�e��p�����[�^�����");
    printf("FRAMESIZE = ");
    scanf("%d", &FRAMESIZE);
    printf("GAIN = ");
    scanf("%lf", &GAIN);
    printf("�������������g���ш�̍ŏ��l�����[Hz]: ");
    scanf("%lf", &cut_Hz_N1);
    printf("�������������g���ш�̍ő�l�����[Hz]: ");
    scanf("%lf", &cut_Hz_N2);

    sfpf = fs / FRAMESIZE;

    N1 = cut_Hz_N1 / sfpf;
    N2 = cut_Hz_N2 / sfpf;

    printf("FRAMESIZE: %d\n", FRAMESIZE);
    printf("GAIN: %d\n", GAIN);
    printf("%lf[Hz]����%lf[Hz]������\n", cut_Hz_N1, cut_Hz_N2);
    printf("����Ώۂ̗v�f�ԍ���[%d]����[%d]\n", N1, N2);

    fprintf(fp_info, "FRAMESIZE: %d\n", FRAMESIZE);
    fprintf(fp_info, "GAIN: %lf\n", GAIN);
    fprintf(fp_info, "%lf[Hz]����%lf[Hz]������\n", cut_Hz_N1, cut_Hz_N2);
    fprintf(fp_info, "����Ώۂ̗v�f�ԍ���[%d]����[%d]\n", N1, N2);

    short dataIn[FRAMESIZE];
    short dataOutX[FRAMESIZE], dataOutY[FRAMESIZE];
    double dDataInX[FRAMESIZE], dDataInY[FRAMESIZE];
    double dDataOutX[FRAMESIZE], dDataOutY[FRAMESIZE];

    fNum = (int)floor(len / FRAMESIZE);
    outlen = fNum * FRAMESIZE;

    /*
	�������ݗp��wav�t�@�C���̃w�b�_���������ށB�f�[�^�̃T�C�Y�܂�,
	��������ł���̂ŁA���̌�͂��Ȃ炸outlen�Ɠ�������fwrite���邱��
	*/
    writeWavHead(ofp, outlen, fs, chNum, sampSize);

    for (int i = 0; i < fNum; i++)
    {
        fread(dataIn, sizeof(short), FRAMESIZE, ifp);

        for (int j = 0; j < FRAMESIZE; j++)
        {
            dDataInX[j] = (double)dataIn[j]; // CT_fft��double�̕ϐ����g���̂ŁAdouble�̕ϐ��ɑ�����Ă��������B
            dDataInY[j] = 0.0;               // ��������0.0�����Ă����܂��B
        }
        CT_fft(dDataInX, dDataInY, FRAMESIZE, 1); //FFT
        for (int j = 0; j < FRAMESIZE; j++)
        {
            dDataOutX[j] = dDataInX[j];
            dDataOutY[j] = dDataInY[j];
            fprintf(fp_spect, "%lf\n", pow(dDataOutX[j], 2) + pow(dDataOutY[j], 2));
        }

        for (int j = N1; j < N2; j++)
        {
            dDataOutX[j] = GAIN * dDataOutX[j];
            dDataOutY[j] = GAIN * dDataOutY[j];
        }

        CT_fft(dDataOutX, dDataOutY, FRAMESIZE, -1); //�tFFT�A�S�ڂ̈�����-1�̏ꍇ�tFFT�ƂȂ�B

        for (int j = 0; j < FRAMESIZE; j++)
        {
            fprintf(fp_vol, "%lf,%lf\n", dDataOutX[j], dDataOutY[j]); // �tFFT�̌��ʂ��������Ȃ�A������o�͂���
            dataOutX[j] = (short)dDataOutX[j];                        // Wav�t�@�C���ɏ����o�����߂�short�Ɍ^�ϊ�����
            dataOutY[j] = (short)dDataOutY[j];                        // Wav�t�@�C���ɏ����o�����߂�short�Ɍ^�ϊ�����
                                                                      //			fprintf(fp_vol, "%d,%d\n", dataOutX[j], dataOutY[j]); 	    // �t�@�C���ɏ����o�����l���������Ȃ�A������o�͂���
        }
        //		printf("\n");

        fwrite(dataOutX, sizeof(short), FRAMESIZE, ofp); // dataOutX�݂̂������o���Ă��邪�AY�̈����͏ꍇ�ɂ��
    }

    fclose(ifp);
    fclose(ofp);
    fclose(fp_info);
    fclose(fp_spect);
    fclose(fp_vol);
}