#include "leak.h"

#include <stdio.h>
#include <unistd.h>

enum leakage_type leakage;
FILE *leakage_file;
int is_leaking = 0;

void leak(unsigned char l){
   if(!is_leaking) return;
   fputc(l, leakage_file);
}

/**
 * opening a new file and starting to leak into it.
 * binary format: unsigned char (i.e. 8 bit per sample)
 */
void leak_start(enum leakage_type l_type, const char *filename){
   leakage = l_type;
   leakage_file = fopen(filename, "w");
   is_leaking = 1;
}

/**
 * pausing the leakge, but doesn't close the leakage file
 */
void leak_pause(){
  is_leaking = 0;
}

/**
 * resuming lekage after leakge has been paused
 */
void leak_resume(){
  is_leaking = 1;
}

/**
 * stoping leakage and closing leakage file
 */
void leak_end(){
  if(leakage_file != NULL) fclose(leakage_file);
  leakage_file = NULL;
  is_leaking = 0;
}

unsigned char hw(unsigned int a){
  unsigned char result =0;
  do{
    result += a&0x1;
    a>>=1;
  } while(a);
  return result;
}
void general_leak(unsigned int left, unsigned int right, unsigned int result){
  switch(leakage){
    case HW:
      leak(hw(result));
      break;
    case HD_L:
      leak(hw(left^result));
      break;
    case HD_R:
      leak(hw(right^result));
      break;
    case HW_BYTE:
      leak(hw(result&0xFF));
      leak(hw((result>>8)&0xFF));
      leak(hw((result>>16)&0xFF));
      leak(hw((result>>24)&0xFF));
      break;
  }
}

/**
 * leaking initializion operation
 * result = left + 0;
 */
unsigned int leak_uint_init(unsigned int left){
  general_leak(0,0,left);
  return left;
}

/**
 * leaking right shift operation
 * result = left >> right;
 */
unsigned int leak_uint_rightshift(unsigned int left, unsigned int right){
  unsigned int result = left >> right;
  general_leak(left,right,result);
  return result;
}

/**
 * leaking left shift operation
 * result = left << right;
 */
unsigned int leak_uint_leftshift(unsigned int left, unsigned int right){
  unsigned int result = left << right;
  general_leak(left,right,result);
  return result;
}

/**
 * leaking binary OR operation
 * result = left | right;
 */
unsigned int leak_uint_or(unsigned int left, unsigned int right){
  unsigned int result = left | right;
  general_leak(left, right, result);
  return result;
}

/**
 * leaking binary AND operation
 * result = left & right;
 */
unsigned int leak_uint_and(unsigned int left, unsigned int right){
  unsigned int result = left&right;
  general_leak(left, right, result);
  return result;
}

/**
 * leaking XOR operation
 * result = left ^ right;
 */
unsigned int leak_uint_xor(unsigned int left, unsigned int right){
  unsigned int result = left^right;
  general_leak(left,right,result);
  return result;
}

/**
 * leaking binary not operation
 * result = ~left;
 */
unsigned int leak_uint_not(unsigned int left){
  unsigned int result = ~left;
  general_leak(left,left,result);
  return result;
}

/**
 * leaking minus operation (32 bit modular subtraction)
 * result = left - right;
 */
unsigned int leak_uint_minus(unsigned int left, unsigned int right){
  unsigned int result = left-right;
  general_leak(left,right,result);
  return result;
}

/**
 * leaking plus operation (32 bit modular addition)
 * result = left + right;
 */
unsigned int leak_uint_plus(unsigned int left, unsigned int right){
  unsigned int result = left+right;
  general_leak(left,right,result);
  return result;
}

/**
 * leaking division operation (32 bit integer division)
 * result = left / right;
 */
unsigned int leak_uint_div(unsigned int left, unsigned int right){
  unsigned int result = left/right;
  general_leak(left,right,result);
  return result;
}
