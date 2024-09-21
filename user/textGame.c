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
int isValidBet(char *bet){
    if bet[0] == '-'{//no negative bets allowed
        return 0;
    }
    return 1;
}
int main() {// a reimplementation of a simple casino game I wrote in python a while ago
    int coins = 100;//starting Coins
    int stuff = 12;//starting stuff
    char *betInput = malloc(128*sizeof(char));// a var for holding the bet
    while (coins >= 0){//keep going while coins <= 0, not < because allows you to sell stuff at 0
        printf("how much to bet (type leave to leave the casino)");
        gets(betInput, 128);
        if (strcmp(bet, "leave")){//handle input of leave
            break;
        }
    }
  return 0;
}