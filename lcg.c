/****************************/
/* Hector Castillo Martinez */
/* CS 241 lab 5             */
/* 3/26/2018                */
/****************************/

#include <stdio.h>
#include "lcg.h"
#define DEBUG 0 /*change to 1 to see prime factors of m and product*/

struct helper
{
  unsigned long arr[15];/*prime factors of m*/
  unsigned long p;/*product of prime factors*/
  int count;/*number of prime factors, starting from 0*/
};

/*Update LCG and return next value of sequence*/
unsigned long getNextRandomValue(struct LinearCongruentialGenerator *lcg)
{
  unsigned long a=1;
  a=lcg->x;
  lcg->x=(lcg->a * lcg->x+ lcg->c) % lcg->m;
  return a;
}

/**********************************************/
/* Takes in two arguments, first being pointer*/
/* to a struct helper, and second (n) being   */
/* number from which the prime factors will be*/
/* saved. Array of factors is returned and    */
/* product is saved as well.                  */
/**********************************************/
unsigned long* prime(struct helper *x,unsigned long n)
{
  unsigned long p=1;
  unsigned long i=2;
  int index=0;
  x->count=0;
  if(n<2) /*if n is too small just save and return*/
    {
      x->arr[index]=n;
      return x->arr;
    }
  while(n>1)
    {
      if((i*i)>n)/*means n is prime so is saved and product found*/
        {
          x->arr[index]=n;
          x->count++;
          p=p*n;
	  x->p=p;
          return x->arr;
        }
      else if((i*i)==n)/* if divisor is root of n*/
        {
          x->arr[index]=i;
          x->count++;
          p=p*i;
	  x->p=p;
          return x->arr;
        }
      else if(n%i==0)     /*if divisible, means it is factor so it is saved*/
	{                 /*is divided to find next divisible number until */
          x->arr[index]=i;/*no longer divisible*/
          index++;
          x->count++;
          p=p*i;
	  x->p=p;
          while(n%i==0)
            {
              n=n/i;
            }
        }
      else i++; /*if divisor not a prime factor then increment to next*/
    }
  return x->arr;
}

/***************************************************************/
/* Initialize an LCG with modulus m and increment c.           */
/* Calculate multiplier a such that:                           */
/*        a = 1+2p, if 4 is a factor of m, otherwise, a = 1+p. */
/*        p = (product of mâ€™s unique prime factors).           */
/*        a < m                                                */
/* Seed value x is same as increment c.                        */
/* If values are invalid for LCG, set all fields to zero.      */
/***************************************************************/
struct LinearCongruentialGenerator makeLCG(unsigned long m, unsigned long c)
{
  struct LinearCongruentialGenerator a;
  struct helper b;
  unsigned long* arr;
  arr=prime(&b,m);
  /* prints out the prime factors of m and what the*/ 
  /* the product of said primes are*/
  if(DEBUG==1)
    {
      int count=0;
      printf("prime factors of %lu\n", m);
      while(count<b.count)
        {
          printf("%d. %lu\n",count+1,arr[count]);
          count++;
        }
      printf("p = %lu\n",b.p);
    }
  if((m>0) && (c<m) && (c>0))/*if numbers are valid then initialize normally*/
    {
      a.m=m;
      a.c=c;
      a.x=c;
      if(m%4==0)a.a=1+(2*b.p);
      else a.a=1+b.p;
      
      if(a.a>m || a.a<0)/*if a is not valid set values to zero*/
        {
          a.m=a.c=a.a=a.x=0;
          return a;
        }
    }
  else a.m=a.c=a.x=a.a=0;/* m and c were not valid, set values to zero*/
  
  return a;
}
