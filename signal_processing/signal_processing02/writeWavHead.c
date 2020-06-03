/*
readWavHead.c
RIFF-Wav�̃w�b�_��ǂݍ���
 */

#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

int writeWavHead(FILE *fp, int dataLength, unsigned long fs, unsigned short chNum, int sampSize)
{
  int i;
  char c;
  RIFF_HDR ckHead;
  COMMON_FIELD comCk;
  ID iD;
  long fileSize, ckLength;

  /*   �t�@�C���T�C�Y�̌v�Z */
  fileSize = dataLength * chNum * sampSize / 8;         /* �f�[�^�����̃T�C�Y */
  fileSize = fileSize + sizeof(ckHead);                 /* data chunk�̃w�b�_�����̃T�C�Y */
  fileSize = fileSize + sizeof(comCk) + sizeof(ckHead); /*fmt�`�����N�̃T�C�Y*/
  fileSize = fileSize + sizeof(ID);                     /* WAVE ID�̃T�C�Y */
  fileSize = fileSize + sizeof(ckHead);                 /* RIFF chunk�̃T�C�Y */

  printf("write Sample Number %d sample\n", dataLength);
  printf("write file length %d byte\n", fileSize);
  /* �擪��RIFF�w�b�_�������o���B */
  ckHead.ckID = RIFF;
  ckHead.ckSize = fileSize;
  if (fwrite(&ckHead, sizeof(ckHead), 1, fp) == 1)
  {
    //    printf("write RIFFHead\n");
  }

  /* WAVE ID�̏����o�� */
  iD = WAVE;
  if (fwrite(&iD, sizeof(iD), 1, fp) == 1)
  {
    //    printf("write WavID\n");
  }

  /* fmt Chunk�̃w�b�_�����o��   */
  ckHead.ckID = fmt_;
  ckHead.ckSize = sizeof(comCk);
  if (fwrite(&ckHead, sizeof(ckHead), 1, fp) == 1)
  {
    //   printf("write fmt Head\n");
  }

  /* fmt Chunk�̃{�f�B�̏����o���B  */
  comCk.wFormatTag = WAVE_FORMAT_PCM;
  comCk.wChannels = chNum;
  comCk.wSamplesPerSec = fs;
  comCk.wAvgBytesPerSec = fs * chNum * sampSize / 8;
  comCk.wBlockAlign = chNum * sampSize / 8;
  comCk.wBitsPerSample = sampSize;
  if (fwrite(&comCk, sizeof(comCk), 1, fp) == 1)
  {
    //   printf("write fmt Chunk\n");
  }

  /* data chunk�w�b�_�̏����o�� */
  ckHead.ckID = data;
  ckHead.ckSize = dataLength * chNum * sampSize / 8;
  if (fwrite(&ckHead, sizeof(ckHead), 1, fp) == 1)
  {
    //    printf("data head\n");
  }
  return (fileSize);
}
