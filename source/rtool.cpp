//Preliminary tools for Turbo, all these functions are WITHOUT
//Direct X calls, that evil will be kept elsewhere.

#include <iostream.h>
//For the rand
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "rtool.h"
//For strings
#include <string.h>


//Takes hi/lo parms like dinkc
//To use either randoms here the below MUST be called at
//some point in the program.
//"srand( (unsigned)time( NULL ) );"

//can't use word small, DX thinks it's some kind of char???

int random(int big, int mysmall)
{
	return rand() % big + mysmall;
}

//Takes only biggest number and return between 1 & that
//ie. random(100); will return 1 - 100.
int random(int big)
{
	return rand() % big + 1;
}
