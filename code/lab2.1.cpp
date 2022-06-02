#include <bits/stdc++.h>
#include "stdio.h"
#include "sys/types.h"
#include "unistd.h"

using namespace std;


int main ()
{   
    int x = 5;
    if(fork())
	{
		x += 30;
		printf("%d\n", x);
	}
	else
    { 
		printf("%d\n", x);
    }
	printf("%d\n", x);
    return 0;
}
