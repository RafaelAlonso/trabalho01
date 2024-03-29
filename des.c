/*
  Gustavo Eugênio de Souza Moraes - RA 620238
  Rafael Pereira Alonso           - RA 620084
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

char bitAt(char *string, int position);

void stringToBits(char *string);
unsigned long stringToLong(char *string);
char* longToString(unsigned long textInLong);
char* longToHex(unsigned long textInLong);

unsigned long initial_permutation(char *plain_text);
unsigned long permutation(char *to_permute, const int pm[], int pmSize, int permutateSize);
unsigned long expansion(char *plain_text);
unsigned long final_permutation(char *plain_text);

unsigned long circularLeftShift(unsigned long part, int nShifts);

char sbox(char part, short round);
unsigned long joinSplits(char *splits);

// Initial Permutation Matrix
const int IP[] = {
  58, 50, 42, 34, 26, 18, 10, 2,
  60, 52, 44, 36, 28, 20, 12, 4,
  62, 54, 46, 38, 30, 22, 14, 6,
  64, 56, 48, 40, 32, 24, 16, 8,
  57, 49, 41, 33, 25, 17,  9, 1,
  59, 51, 43, 35, 27, 19, 11, 3,
  61, 53, 45, 37, 29, 21, 13, 5,
  63, 55, 47, 39, 31, 23, 15, 7
};

// Expansion Matrix
const int E[]  = {
  32,  1,  2,  3,  4,  5,
   4,  5,  6,  7,  8,  9,
   8,  9, 10, 11, 12, 13,
  12, 13, 14, 15, 16, 17,
  16, 17, 18, 19, 20, 21,
  20, 21, 22, 23, 24, 25,
  24, 25, 26, 27, 28, 29,
  28, 29, 30, 31, 32,  1
};

// Permutation Matrix
const int P[]  = {
  16,  7, 20, 21,
  29, 12, 28, 17,
   1, 15, 23, 26,
   5, 18, 31, 10,
   2,  8, 24, 14,
  32, 27,  3,  9,
  19, 13, 30,  6,
  22, 11,  4, 25
};

// Final Permutation Matrix
const int FP[]  = {
  40, 8, 48, 16, 56, 24, 64, 32,
  39, 7, 47, 15, 55, 23, 63, 31,
  38, 6, 46, 14, 54, 22, 62, 30,
  37, 5, 45, 13, 53, 21, 61, 29,
  36, 4, 44, 12, 52, 20, 60, 28,
  35, 3, 43, 11, 51, 19, 59, 27,
  34, 2, 42, 10, 50, 18, 58, 26,
  33, 1, 41,  9, 49, 17, 57, 25
};

// ================================ SBOXES ================================
  const int S1[4][16]  = {
     14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
      0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
      4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
     15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
  };

  const int S2[4][16]  = {
     15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
      3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
      0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
     13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
  };

  const int S3[4][16]  = {
     10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
     13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
     13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
      1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
  };

  const int S4[4][16]  = {
      7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
     13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
     10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
      3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
  };

  const int S5[4][16]  = {
      2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
     14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
      4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
     11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
  };

  const int S6[4][16]  = {
     12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
     10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
      9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
      4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
  };

  const int S7[4][16] = {
      4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
     13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
      1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
      6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
  };

  const int S8[4][16] = {
     13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
      1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
      7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
      2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
  };
// ========================================================================

// Permutation Choice 1 Matrix
const int PC1[]  = {
    57, 49, 41, 33, 25, 17,  9,
     1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
     7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4
};

// Permutation Choice 2 Matrix
const int PC2[]  = {
    14, 17, 11, 24,  1,  5,  3, 28,
    15,  6, 21, 10, 23, 19, 12,  4,
    26,  8, 16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55, 30, 40,
    51, 45, 33, 48, 44, 49, 39, 56,
    34, 53, 46, 42, 50, 36, 29, 32
};

const int SHIFTS[] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

int main( int argc, char *argv[] ){
  unsigned long entradaManipulada;
  unsigned long entradaEsq;
  unsigned long entradaDir;

  unsigned long chaveManipulada;
  unsigned long chaveEsq;
  unsigned long chaveDir;

  char entrada[9];
  char chave[9];
  char eightSplit[8] = "";

  // get the 64-bit plain text
  strcpy(entrada, argv[1]);
  entrada[8] = '\0';
  printf("PLAIN TEXT\n");
  printf("%lX\n\n", stringToLong(entrada));

  // pass it through the initial permutation (IP)  => 64-bit
  entradaManipulada = initial_permutation(entrada);
  printf("IP\n");
  printf("%lX\n\n", entradaManipulada);

  // get the 64-bit key
  strcpy(chave, argv[2]);
  chave[8] = '\0';
  printf("CHAVE\n");
  printf("%lX\n\n", stringToLong(chave));

  // pass it through the permutation choice 1 (PC1) => 56-bit
  chaveManipulada = permutation(chave, PC1, 56, 64);

  // divide the 56-key in two => 28-bit / 28-bit
  chaveEsq = chaveManipulada >> 28;

  chaveDir = chaveManipulada << 36 >> 36;

  // =============================== ROUNDS ===============================
  // for each of the 16 rounds:
  for(char i = 0; i < 16; i++){
    // ============================= KEY PART =============================

      // shift the two parts to the left and join them  => 56-bit
      chaveEsq = circularLeftShift(chaveEsq, SHIFTS[i]);
      chaveDir = circularLeftShift(chaveDir, SHIFTS[i]);
      chaveManipulada = (chaveEsq << 28) + chaveDir;

      // permutate the shifted key with permutation choice 2 (PC2) => 48-bit
      chaveManipulada = permutation(longToString(chaveManipulada), PC2, 48, 56);

      printf("CHAVE DE ROUND %d\n", i+1);
      printf("%lX\n\n", chaveManipulada);

    // ====================================================================

    // ============================= TXT PART =============================
      // divide the text in two => 32-bit / 32-bit
      entradaEsq = entradaManipulada >> 32;
      entradaDir = entradaManipulada << 32 >> 32;

      // ============= RIGHT PART =============
        // pass it through the expansion (E) => 48-bit
        entradaDir = expansion(longToString(entradaDir));

        // XOR the right side with the PC2'd key => 48-bit
        entradaDir = entradaDir ^ chaveManipulada;

        // with the result:
        for(char j = 0; j < 8; j++){
          // divide it into eight => 6-bit / 6-bit / 6-bit ...
          eightSplit[j] = (char) (entradaDir << (16 + 6 * j) >> 58);

          // pass each part through their respective Sbox (S1, S2, ...)
          eightSplit[j] = sbox(eightSplit[j], j + 1);
        }

        // join them => 32-bit
        entradaDir = joinSplits(eightSplit);

        // pass it through the permutation (P) => 32-bit
        entradaDir = permutation(longToString(entradaDir), P, 32, 32);

        // XOR it with the left side
        entradaDir = entradaDir ^ entradaEsq;

      // ======================================

      // join the left and right sides swaped => 64-bit
      entradaManipulada = (entradaManipulada << 32) | entradaDir;

      printf("ROUND %d\n", i+1);
      printf("%lX\n\n", entradaManipulada);

    // ====================================================================
  }
  // ======================================================================

  // final part
    // divide the result into two => 32-bit / 32-bit
    entradaEsq = entradaManipulada >> 32;
    entradaDir = entradaManipulada << 32 >> 32;

    // join them swaped
    entradaManipulada = (unsigned long) (entradaDir << 32) | entradaEsq;

    printf("Swap: ");
    printf("%lX\n\n", entradaManipulada);

    // pass it through with the final permutation (FP)
    entradaManipulada = final_permutation(longToString(entradaManipulada));

    printf("IP Inverso: ");
    printf("%lX\n\n", entradaManipulada);

    return 0;
}

// get the bit At a given position, reading the bits right to left
char bitAt(char *string, int position){
  // logic to get left to right
  position = 63 - position;

  // which char to get
  int a = position / 8;

  // which bit from the char to get
  int b = 7 - (position % 8);

  return (string[a] & (1 << b)) ? 1 : 0;
}

// get the equivalent string to a given unsigned long (always 64-bit)
char* longToString(unsigned long textInLong){
  char* res = (char *) malloc(8 * sizeof(char));
  for(int i = 7; i >= 0; i--){
    res[7-i] = (char) ( (unsigned long) (textInLong << (8 * (7 - i))) >> 56);
  }
  return res;
}

unsigned long initial_permutation(char *plain_text){
  // given the plain_text, get it's bits and
  // rearrange them according to the position pointed out in IP matrix
  unsigned long res = 0;
  for(int i = 63; i >= 0; i--){
    res += (unsigned long) bitAt(plain_text, IP[i] - 1) << i;
  }

  return res;
}

// FP
unsigned long final_permutation(char *plain_text){
  // given the plain_text, get it's bits
  // rearrange them according to the position pointed out in FP matrix
  unsigned long res = 0;
  for(int i = 63; i >= 0; i--){
    res += (unsigned long) bitAt(plain_text, FP[i] - 1) << i;
  }

  return res;
}

// PC1, PC2, P
unsigned long permutation(char *to_permute, const int pm[], int pmSize, int permutateSize){
  // change the place of each bit of given part, placing it in the
  // position specified by the given permutation matrix
  unsigned long res = 0;
  for(int i = 0; i < pmSize; i++){
    /* "63 - (pm[i] + (64 - permutateSize) - 1)" logic:
      we're gonna read these ones right to left, because the string to permute
      might not have 64 bits TO CONSIDER, but the string itself WILL ALWAYS BE
      64 bits. This logic is to get the right bit according to it's relative size
      (permutateSize)
    */
    res += (unsigned long) bitAt(to_permute, 63 - (pm[i] + (64 - permutateSize) - 1)) << (pmSize - i - 1);
  }

  return res;
}

// E
unsigned long expansion(char *to_permute){
  unsigned long res = 0;
  for(int i = 48; i >= 0; i--){
    res += (unsigned long) bitAt(to_permute, E[i] - 1) << i;
  }

  return res;
}

// SB's 1 to 8
char sbox(char part, short part_round){
  // given the 6-bit part and which one it is (1 - 8),

  // get the first and last bit of it (SBox row) => 2-bit
  char external = ((part & (1 << 5)) >> 4) + (part & 1);

  // get all the others separately (SBox column) => 4-bit
  char internal = (part << 3 >> 4) & 15;

  // return the value indicated in the found position
  char res = 0;
  switch(part_round){
    case 1:
      res = S1[external][internal];
      break;
    case 2:
      res = S2[external][internal];
      break;
    case 3:
      res = S3[external][internal];
      break;
    case 4:
      res = S4[external][internal];
      break;
    case 5:
      res = S5[external][internal];
      break;
    case 6:
      res = S6[external][internal];
      break;
    case 7:
      res = S7[external][internal];
      break;
    case 8:
      res = S8[external][internal];
      break;
  }
  return res;
}

unsigned long circularLeftShift(unsigned long part, int nShifts){
  part = part << nShifts;

  if (nShifts == 1){
    part += bitAt(longToString(part), 28);
  } else{
    part += bitAt(longToString(part), 28) + (2 * bitAt(longToString(part), 29));
  }
  part -= (unsigned long) (bitAt(longToString(part), 28)) << 28;
  part -= (unsigned long) (bitAt(longToString(part), 29)) << 29;
  return part;
}

unsigned long joinSplits(char *splits){
  unsigned long res = 0;
  for(int i = 7; i >= 0; i--){
    res += (unsigned long) ( (unsigned long) splits[i] << (4 * (7 - i)));
  }
  return res;
}

// write the 64-bit string as chunks of 8-bits, writing left to right
void stringToBits(char *string){
  char c;
  // for each of the 8 chars in the string
  for(short i = 0; i < 8; i++){
    c = string[i];
    // for each bit of the given char
    for (short j = 7; 0 <= j; j--){
      // print 1 or 0, according to the bit parity
      printf("%c", (c & (1 << j)) ? '1' : '0');
    }
    // space to split the chars
    printf(" ");
  }
  printf("\n");
}

// get the unsigned long value of a given string
unsigned long stringToLong(char *string){
  unsigned long res = 0;
  for(int i = 0; i < 64; i++){
    res += bitAt(string, i) * (unsigned long) pow(2, i);
  }
  return res;
}
