#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int LCG(int prev){// function that generates a psuedorandom number, using the previous value in the sequence, which starts at a seed
    prev = prev % m;
    //https://www.ams.org/journals/mcom/1999-68-225/S0025-5718-99-00996-5/S0025-5718-99-00996-5.pdf has table of values
    long a = 530877178;
    long c = 0;//c = 0, since it is what the chart has
    long m = 536870909;// 2^29 -3
    return (a*prev+c)%m;
}
int main() {

  return 0;
}