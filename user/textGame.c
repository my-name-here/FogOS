#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int LCG(int prev){// function that generates a psuedorandom number, using the previous value in the sequence, which starts at a seed

    //https://www.ams.org/journals/mcom/1999-68-225/S0025-5718-99-00996-5/S0025-5718-99-00996-5.pdf has table of values
    const long a = 530877178;
    const long c = 0;//c = 0, since it is what the chart has
    const long m = 536870909;// 2^29 -3
    prev = prev % m;
    return (a*prev+c)%m;
}
int isNonNegativeInt(char *inString){
    if (inString[0] == '-'){//no negative bets allowed
        return 0;
    }
    int i = 0;
    while (inString[i] != '\0') {//loop over every char in string
        if (inString[i] < '0' || inString[i] > '9'){// not a digit
            return 0;
        }
        i++;
    }
    return 1;

}
void removeTrailingNewline(char *string){
    int stringLength = strlen(string);
    if (stringLength > 0 && string[stringLength-1] == '\n'){//if string has at least one character, and last char is newline
        string[stringLength-1] = '\0';
    }
}

int getInputAndCompare(char *userInput, char *comparisonString){
    gets(userInput, 128);
    removeTrailingNewline(userInput);
    return (strcmp(userInput, comparisonString) == 0);//1 if they match, 0 otherwise
}

int shouldBuyOrSellStuff(char *responseInput, int isSelling){//isSelling is 1 if selling, 0 if buying
    printf("would you like to %s\n", (isSelling ? "sell some of your stuff yes/no" : "buy some things yes/no"));//use ternary operator to change the print depending on if buying or selling
    return getInputAndCompare(responseInput, "yes");
}

void performBuying(int *coinAmount, int *stuffAmount, char *responseInput){
    printf("how many things would you like to buy for $50 each?\n");
    gets(responseInput, 128);// get amount to buy in response var
    removeTrailingNewline(responseInput);
    while (!isNonNegativeInt(responseInput) || atoi(responseInput)<0 || 50*atoi(responseInput)>*coinAmount){//reuse bet handling to check if we can convert, and check we are buying positive amount, and that we have enough money
        printf("invalid input. Make sure you input a positive int, and have enough money to buy that many.\n");
        printf("how many things would you like to buy for $50 each?\n");
        gets(responseInput, 128);// get amount to buy in response var
        removeTrailingNewline(responseInput);
    }
    *stuffAmount = *stuffAmount + atoi(responseInput);//increase stuff
    *coinAmount = *coinAmount - atoi(responseInput)*50;//decrease money

}

void performSelling(int *coinAmount, int *stuffAmount, int *randomNumber, int maxSalePrice, char *responseInput){
    printf("how many things would you like to sell?\n");
    gets(responseInput, 128);// get amount to sell in response var
    removeTrailingNewline(responseInput);

    while (!isNonNegativeInt(responseInput) || atoi(responseInput)>*stuffAmount){//reuse bet handling to check if we can convert, and check we have enough stuff
        printf("invalid input. Make sure you input a non-negative integer, and you have enough stuff.\n");
        printf("how many things would you like to sell?\n");
        gets(responseInput, 128);// get amount to sell in response var
        removeTrailingNewline(responseInput);

    }
    *stuffAmount = *stuffAmount - atoi(responseInput);
    *randomNumber = LCG(*randomNumber);// get a new random val with LCG
    int salePrice = *randomNumber%(maxSalePrice-1)+1;//random int from 1-maxSalePrice
    printf("your stuff sold for $%d each\n", salePrice);
    printf("you have %d things\n", *stuffAmount);
    *coinAmount = *coinAmount + salePrice * atoi(responseInput);
    printf("You now have $%d\n", *coinAmount);
}

int main() {// a reimplementation of a simple casino game I wrote in python a while ago
    const int initialStuff = 12;// starting stuff amount
    const int initialCoins = 100;// starting coins amount
    int coins = initialCoins;//set coins var
    int stuff = initialStuff;//set stuff var
    char *betInput = malloc(128*sizeof(char));// a var for holding the bet
    char *responseInput = malloc(128*sizeof(char));//a var for holding various responses
    int randVal = uptime();// set seed for lcg as uptime
    int roll = 0;//set roll var to 0;
    while (coins >= 0){//keep going while coins <= 0, not < because allows you to sell stuff at 0
        if (coins <= 0 && stuff <= 0){//stop if you are out of money and stuff(changed from equals zero, since if negative, also want to stop)
            break;
        }
        printf("how much to bet (type leave to leave the casino)\n");
        if (getInputAndCompare(betInput, "leave")){//handle input of leave
            break;
        }
        //not leave, so now check if it is a valid bet
        while (!isNonNegativeInt(betInput) || atoi(betInput)>coins){//make sure it is valid int input, if it is, also make sure we have enough coins
            printf("invalid bet. make sure you give a positive integer, and have enough money.\n");
            printf("how much to bet\n");
            gets(betInput, 128);
            removeTrailingNewline(betInput);
        }
        randVal = LCG(randVal);// get a new random val with LCG
        roll = (randVal % 97) + 1; // get a roll from 1-98
        if (roll == 1){// win more coins if roll is one
            coins = coins + atoi(betInput)*48;//win 48 times your bet
            printf("you won\n");
            if (shouldBuyOrSellStuff(responseInput, 1)){//does the user want to sell
                printf("you have %d things\n", stuff);// print out how much stuff we have, so the user knows
                performSelling(&coins, &stuff, &randVal, 100, responseInput);
            }
            printf("would you like to leave yes/no\n");
            if (getInputAndCompare(responseInput, "yes")){//said yes to leaving
                break;
            }
        }
        else if (roll == 2){//win stuff if roll is 2
            stuff=stuff+(atoi(betInput));// win your bet in stuff
            printf("you won %d things\n", atoi(betInput));
            printf("you have %d things\n", stuff);
            if (shouldBuyOrSellStuff(responseInput, 0)){//user wants to buy things
                performBuying(&coins, &stuff, responseInput);

            }
        }
        else{//you lose
            coins=coins-atoi(betInput);// lose your bet
            printf("you lose\n");
            
        }
        printf("you have $%d\n", coins);//print money after bet
        //now allow the user to sell stuff if they are out of money.
        if (coins <= 0){//not enough money
            if (shouldBuyOrSellStuff(responseInput, 1)){//user wants to sell
                if (stuff<=0){//out of money and stuff
                    printf("not enough stuff\n");
                    break;
                }
                else{//have at least some stuff
                    printf("you have %d things\n", stuff);// print out how much stuff we have, so the user knows
                    performSelling(&coins, &stuff, &randVal, 20, responseInput);
                }
            }
        }
        // allow user to buy stuff
        if (shouldBuyOrSellStuff(responseInput, 0)){//user wants to buy things
            performBuying(&coins, &stuff, responseInput);
        }
    }
    printf("profit was $%d\n",coins-initialCoins);
    printf("you gained %d things\n",stuff-initialStuff);

    free(betInput);
    free(responseInput);
    return 0;
}