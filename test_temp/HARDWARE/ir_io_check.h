#ifndef _IR_IO_CHECK_H
#define _IR_IO_CHECK_H

#include "stm32f10x.h"

typedef struct ir_io
{
  unsigned  left;
  unsigned  mleft;
  unsigned  mright;
  unsigned  right;
}IR_IO;

#define ir_left    ir_io_result.left
#define ir_mleft   ir_io_result.mleft
#define ir_mright  ir_io_result.mright
#define ir_right   ir_io_result.right

void ir_io_init();
void ir_io_check();
#endif
