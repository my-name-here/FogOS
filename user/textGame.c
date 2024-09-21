#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int LCG(int prev){// function that generates a psuedorandom number, using the previous value in the sequence, which starts at a seed
    long a = 0;
    long c = 0;
    long m = 0;
  return (a*prev+c)%m;
}
int main() {

  return 0;
}