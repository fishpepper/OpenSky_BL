#ifndef __DMA_H__
#define __DMA_H__

#include <stdint.h>

typedef struct {
  uint8_t srcAddrH;
  uint8_t srcAddrL;
  uint8_t dstAddrH;
  uint8_t dstAddrL;
  uint8_t xferLenV;
  uint8_t xferLenL;
  uint8_t ctrlA;
  uint8_t ctrlB;
} DMADesc_t;

#define DMA_SET_ADDR_DESC0(a) { DMA0CFGH = (uint8_t)((uint16_t)(a) >> 8); DMA0CFGL = (uint8_t)((uint16_t)(a)); }

#endif
