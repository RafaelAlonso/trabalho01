// cabeçalho
  #include <stdio.h>
  #include <stdlib.h>
  #include <math.h>

  void initial_permutation(char **plain_text);
  void permutation(char **to_permute, int pm);
  void final_permutation(char **plain_text);
  void xor(char **first, char **second);
  void sbox(char **part, short round);
  void join_and_swap(char **left, char **right);

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
      14, 17, 11, 24,  1,  5,
       3, 28, 15,  6, 21, 10,
      23, 19, 12,  4, 26,  8,
      16,  7, 27, 20, 13,  2,
      41, 52, 31, 37, 47, 55,
      30, 40, 51, 45, 33, 48,
      44, 49, 39, 56, 34, 53,
      46, 42, 50, 36, 29, 32
  };

  const int SHIFTS[] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };
// fim cabeçalho

int main( int argc, char *argv[] ){
  // get the 64-bit plain text
  // pass it through the initial permutation (IP)   => 64-bit

  // get the 64-bit key
  // pass it through the permutation choice 1 (PC1) => 56-bit

  // =============================== ROUNDS ===============================
  // for each of the 16 rounds:
    // ============================= KEY PART =============================
    // divide the 56-key in two => 28-bit / 28-bit
    // shift the two parts to the left and join them  => 56-bit
    // permutate the shifted key with permutation choice 2 (PC2) => 48-bit
    // ====================================================================

    // ============================= TXT PART =============================
    // divide the text in two => 32-bit / 32-bit

    // ============= RIGHT PART =============
    // pass it through the expansion (E) => 48-bit
    // XOR the right side with the PC2'd key => 48-bit
    // with the result:
      // divide it into eight => 6-bit / 6-bit / 6-bit ...
      // pass each part through their respective Sbox (S1, S2, ...)
      // join them => 32-bit
    // pass it through the permutation (P) => 32-bit
    // XOR it with the left side
    // ======================================

    // join the left and right sides swaped => 64-bit
    // ====================================================================
  // ======================================================================

  // divide the result into two => 32-bit / 32-bit
  // join them swaped
  // pass it through with the final permutation (FP)

  char teste[9] = "hi world\0";
  char c;

  int value = 0;
  for (int i = 7; i >= 0; i--)
    value += (int) pow(2, i) * teste[7 - i];
  printf("%d", value);

  printf("\n%s\n", teste);

  return 0;
}

void initial_permutation(char **plain_text){
  // given the plain_text, get it's bits
  // rearrange them according to the position pointed out in IP

}

void final_permutation(char **plain_text){
  // given the plain_text, get it's bits
  // rearrange them according to the position pointed out in IP
}

void permutation(char **to_permute, int pm){
  // change the place of each bit of given part, placing it in the
  // position specified by the given permutation matrix
}

void xor(char **first, char **second){
  // given two sets of bits, store in the result their XOR
  // (or exclusive)
}

void sbox(char **part, short part_round){
  // given the 6-bit part and which one it is (1 - 8),
  // get the first and last bit of it => 2-bit
  // get all the others separately    => 4-bit
  // the 2-bit part indicate the SBox row
  // the 4-bit part indicate the SBox column
  // return the value indicated in the found position
}

void join_and_swap(char **left, char **right){
  // given two parts (left and right), return them joined
  // right part first, left part second
}
