#include<stdio.h>
#define LEN (20)
int getSt(int a, int input[])
{
    int i = 0;
    int count = 0;
    for(i = 0; i < LEN; i ++) {
        if(a == input[i])
            count ++;
    }
    return count;
}
int main()
{
    int input[LEN] = { 0 };
    int output[LEN] = { 0 };

    int i = 0;
    int count = 0;
    int retry = 0;

    for(i = 0; i < LEN; i ++)
        input[i] = i;

    while(1)
    {
        retry = 0;
        for(i = 0; i < LEN; i ++)
        {
           count = getSt(i, output);
           if(output[i] != count) {
               output[i] = count;
               retry = 1;
           }
        }
        if(retry == 0)
            break;
    }
    for(i = 0; i < LEN; i ++)
        printf("%d ", output[i]);
    printf("\n");

}
