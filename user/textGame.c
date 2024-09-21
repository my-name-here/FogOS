#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int LCG(int prev){// function that generates a psuedorandom number, using the previous value in the sequence, which starts at a seed

    //https://www.ams.org/journals/mcom/1999-68-225/S0025-5718-99-00996-5/S0025-5718-99-00996-5.pdf has table of values
    long a = 530877178;
    long c = 0;//c = 0, since it is what the chart has
    long m = 536870909;// 2^29 -3
    prev = prev % m;
    return (a*prev+c)%m;
}
int isValidBet(char *bet){
    if (bet[0] == '-'){//no negative bets allowed
        return 0;
    }
    int i = 0;
    while (bet[i] != '\0') {//loop over every char in string
        if (bet[i] < '0' || bet[i] > '9'){// not a digit
            return 0;
        }
        i++;
    }
    return 1;

}
void removeTrailingNewline(char *string){
    int stringLength = strlen(string);
}
int main() {// a reimplementation of a simple casino game I wrote in python a while ago
    int coins = 100;//starting Coins
    int stuff = 12;//starting stuff
    char *betInput = malloc(128*sizeof(char));// a var for holding the bet
    char *responseInput = malloc(128*sizeof(char));//a var for holding various responses
    int randVal = uptime();// set seed for lcg as uptime
    int roll = 0;//set roll var to 0;
    while (coins >= 0){//keep going while coins <= 0, not < because allows you to sell stuff at 0
        printf("how much to bet (type leave to leave the casino)\n");
        gets(betInput, 128);
        if (strcmp(betInput, "leave") == 0){//handle input of leave
            break;
        }
        //not leave, so now check if it is a valid bet
        while (!isValidBet(betInput)){
            printf("invalid bet\n");
            printf("how much to bet\n");
            gets(betInput, 128);
        }
        randVal = LCG(randVal);// get a new random val with LCG
        roll = (randVal % 97) + 1; // get a roll from 1-98
        if (roll == 1){// win more coins if roll is one
            coins = coins + atoi(betInput)*48;//win 48 times your bet
            printf("you won\n");
            printf("would you like to sell some of your stuff?\n");
            printf("yes/no\n");
            gets(responseInput, 128);
            if (strcmp(responseInput, "yes") == 0){//said yes to selling
                printf("you have %d things\n", stuff);// print out how much stuff we have, so the user knows
                printf("how many things would you like to sell?\n");
                gets(responseInput, 128);// get amount to sell in response var
                while (!isValidBet(responseInput) || atoi(responseInput)>stuff){//reuse bet handling to check if we can convert, and check we have enough stuff
                    printf("invalid input. Make sure you input an int, and you have enough stuff.\n");
                    printf("how many things would you like to sell?\n");
                    gets(responseInput, 128);// get amount to sell in response var
                }
                stuff = stuff - atoi(responseInput);
                randVal = LCG(randVal);// get a new random val with LCG
                int salePrice = randVal%99+1;//random int from 1-100
                printf("your stuff sold for $%d each\n", salePrice);
                printf("you have %d things\n", stuff);
                coins = coins + salePrice * atoi(responseInput);
                printf("$%d", coins);
            }
            printf("would you like to leave yes/no\n");
            gets(responseInput, 128);
            if (strcmp(responseInput, "yes") == 0){//said yes to leaving
                break;
            }
        }
        else if (roll == 2){//win stuff if roll is 2
            stuff=stuff+(atoi(betInput));// win your bet in stuff
            printf("you have %d things\n", stuff);
            printf("would you like to buy some things yes/no\n");
            gets(responseInput, 128);
            if (strcmp(responseInput, "yes") == 0){//said yes to buying
                printf("how many things would you like to buy for $50 each?\n");
                gets(responseInput, 128);// get amount to buy in response var
                while (!isValidBet(responseInput) || atoi(responseInput)<0){//reuse bet handling to check if we can convert, and check we are buying positive amount
                    printf("invalid input. Make sure you input a positive int.\n");
                    printf("how many things would you like to buy for $50 each?\n");
                    gets(responseInput, 128);// get amount to buy in response var
                }
                stuff = stuff + atoi(responseInput);//increase stuff
                coins = coins - atoi(responseInput)*50;//decrease money

            }
        }
        else{//you lose
            coins=coins-atoi(betInput);// lose your bet
            printf("you lose\n");
            
        }
        printf("you have $%d\n", coins);//print money after bet
        //now allow the user to sell stuff if they are out of money.
        if (coins <= 0){//not enough money
            printf("would you like to sell some of your stuff?\n");
            printf("yes/no\n");
            gets(responseInput, 128);
            if (strcmp(responseInput, "yes") == 0){//said yes to selling
                if (stuff<=0){//out of money and stuff
                    printf("not enough stuff\n");
                    break;
                }
                else{//have at least some stuff
                    printf("you have %d things\n", stuff);// print out how much stuff we have, so the user knows
                    printf("how many things would you like to sell?\n");
                    gets(responseInput, 128);// get amount to sell in response var
                    while (!isValidBet(responseInput) || atoi(responseInput)>stuff){//reuse bet handling to check if we can convert, and check we have enough stuff
                        printf("invalid input. Make sure you input an int, and you have enough stuff.\n");
                        printf("how many things would you like to sell?\n");
                        gets(responseInput, 128);// get amount to sell in response var
                    }
                    stuff = stuff - atoi(responseInput);
                    randVal = LCG(randVal);// get a new random val with LCG
                    int salePrice = randVal%20+1;//random int from 1-20
                    printf("your stuff sold for $%d each\n", salePrice);
                    printf("you have %d things\n", stuff);
                    coins = coins + salePrice * atoi(responseInput);
                    printf("$%d\n", coins);
                }
            }
        }
        // allow user to buy stuff
        printf("would you like to buy some things yes/no\n");
        gets(responseInput, 128);
        if (strcmp(responseInput, "yes") == 0){//said yes to buying
            printf("how many things would you like to buy for $50 each?\n");
            gets(responseInput, 128);// get amount to buy in response var
            while (!isValidBet(responseInput) || atoi(responseInput)<0){//reuse bet handling to check if we can convert, and check we are buying positive amount
                printf("invalid input. Make sure you input a positive int.\n");
                printf("how many things would you like to buy for $50 each?\n");
                gets(responseInput, 128);// get amount to buy in response var
            }
            stuff = stuff + atoi(responseInput);//increase stuff
            coins = coins - atoi(responseInput)*50;//decrease money
        }
    }
    printf("profit was $%d\n",coins-100);
    free(betInput);
    free(responseInput);
    return 0;
}