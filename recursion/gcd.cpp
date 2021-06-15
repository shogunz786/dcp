#include <iostream>
using namespace std;
int gcd(int num1, int num2)
{
  if(num1==0)
     return num2;
  else if(num2==0)
     return num1;
  int big=num1;
  int small=num2;
  if(big<small){
    big=num2;
    small=num1;
  }
  int rem = big%small;
  return gcd(rem,small);
}

int gcd(int num1, int num2)
{
  if(num1 > num2)
  {
    gcd(num1-num2, num2);
  }

  else if(num2 > num1)
  {
    gcd(num1, num2-num1);
  }
  else
  {
    return num1;
  }
}
