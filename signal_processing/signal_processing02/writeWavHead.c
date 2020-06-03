/*
readWavHead.c
RIFF-Wavのヘッダを読み込む
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

  /*   ファイルサイズの計算 */
  fileSize = dataLength * chNum * sampSize / 8;         /* データ部分のサイズ */
  fileSize = fileSize + sizeof(ckHead);                 /* data chunkのヘッダ部分のサイズ */
  fileSize = fileSize + sizeof(comCk) + sizeof(ckHead); /*fmtチャンクのサイズ*/
  fileSize = fileSize + sizeof(ID);                     /* WAVE IDのサイズ */
  fileSize = fileSize + sizeof(ckHead);                 /* RIFF chunkのサイズ */

  printf("write Sample Number %d sample\n", dataLength);
  printf("write file length %d byte\n", fileSize);
  /* 先頭のRIFFヘッダを書き出す。 */
  ckHead.ckID = RIFF;
  ckHead.ckSize = fileSize;
  if (fwrite(&ckHead, sizeof(ckHead), 1, fp) == 1)
  {
    //    printf("write RIFFHead\n");
  }

  /* WAVE IDの書き出し */
  iD = WAVE;
  if (fwrite(&iD, sizeof(iD), 1, fp) == 1)
  {
    //    printf("write WavID\n");
  }

  /* fmt Chunkのヘッダ書き出し   */
  ckHead.ckID = fmt_;
  ckHead.ckSize = sizeof(comCk);
  if (fwrite(&ckHead, sizeof(ckHead), 1, fp) == 1)
  {
    //   printf("write fmt Head\n");
  }

  /* fmt Chunkのボディの書き出し。  */
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

  /* data chunkヘッダの書き出し */
  ckHead.ckID = data;
  ckHead.ckSize = dataLength * chNum * sampSize / 8;
  if (fwrite(&ckHead, sizeof(ckHead), 1, fp) == 1)
  {
    //    printf("data head\n");
  }
  return (fileSize);
}
