#include <stdio.h>

int main()
{
    int i = 0;
    while(i < 10)
    {
        if(i == 3)
        {
            continue;
        }

        printf("%d", i);
        i++;
    }
}