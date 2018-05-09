/*****************************/
/* Hector Castillo Martinezz */
/* CS 241                    */
/* 3/26/2018                 */
/*****************************/

#include <stdio.h>
#include "lcg.h"

#define FILL_M 0  /*when to convert m to number*/
#define FILL_C 1  /*when to convert c to number*/
#define FILL_DATA 2  /*when to work with the data*/
#define ENCRYPT 1  /*when to encrypt*/
#define DECRYPT 2  /*when to decrypt*/


int encryptData(unsigned long m,unsigned long c,int ch,unsigned long x);
int decryptData(unsigned long m, unsigned long c, int ch,unsigned long x);

/*************************************************************/
/* Takes in a text file and goes through it char by char     */
/* with \n being signal to move to next line and ',' being   */
/* signal to convert next string into number or when at data.*/
/* Expected input to be action lcg_m, lcg_c, Data \n. Error  */
/* is thrown if format is not followed or if lcg_m or lcg_c  */
/* are not valid.                                            */
/*************************************************************/
int main()
{
  int c,currentPos,action,linenum,error;
  unsigned long m,ci,random;
  currentPos=action=error=0;
  m=ci=random=0; 
  linenum=1;
  while((c=getchar())!=EOF)
    {
      /*if c is e then encrypt, else if d then decrypt
	else then action left at nothing and error caught*/ 
      if(currentPos==FILL_M && c =='e')action=ENCRYPT;
      else if(currentPos==FILL_M && c=='d')action=DECRYPT;
      if(c!=',' && c!='\n')                               
        {
	  if(currentPos==FILL_M && c>='0' && c<='9')
            {
              m=10*m+(c-'0'); /*m converted to a number*/
            }
          else if(currentPos==FILL_C && c>='0' && c<='9')
            {
              ci=10*ci+(c-'0'); /*ci (lcg_c) converted to a number*/
            }
          else if(currentPos==FILL_DATA)
            {
              if(action == ENCRYPT)
                {
                  struct LinearCongruentialGenerator a;
                  a=makeLCG(m,ci);
                  printf("%5d) ",linenum);
                  linenum++;
                  if(a.m==0)/* if zero than m and ci were not valid*/
                    {
                      printf("Error");
                      while((c=getchar())!='\n');
                    }
                  else
                    {
                      random=getNextRandomValue(&a);
		      /* feeds in char by char into function
                         until data is done or error is found*/
                      while(c!='\n' && error==0)
                        {
                          error= encryptData(m,ci,c,random);
                          c=getchar();
                          random=getNextRandomValue(&a);
                        }
                    }
                  printf("\n");
                }
              else if(action == DECRYPT)
                {
                  struct LinearCongruentialGenerator b;
                  b=makeLCG(m,ci);
                  printf("%5d) ",linenum);
                  linenum++;
                  if(b.m==0)/* if m is zero, then m and ci were invalid*/
                    {
                      printf("Error");
                      while((c=getchar())!='\n');
                    }
                  else
                    {
                      random=getNextRandomValue(&b);
		      /* data fed in char by char into function
                         until data is done or error is found*/
                      while(c!='\n' && error==0)
                        {
                          error=decryptData(m,ci,c,random);
                          c=getchar();
                          random=getNextRandomValue(&b); 
                        }
                    }
                  printf("\n");
                }
              else /* goes into this if action never declared*/
                {
                  printf("%5d) ",linenum);
                  linenum++;
                  printf("Error\n");
                }
              m=ci=0;  /*reset everything to zero when done with data */
              currentPos=FILL_M;
              action=error=0;
            }
        }
      if(c == ',') /*tracks what place currently in*/
        {
          if(currentPos==FILL_M)currentPos=FILL_C;
          else if(currentPos==FILL_C)currentPos=FILL_DATA;
        }
      /* not enough commas detected as it did not 
         go into FILL_DATA, therefore print error 
         and reset everything to zero*/
      if(c=='\n' && currentPos!=FILL_M)
        {
          printf("%5d) ",linenum);
          linenum++;
          printf("Error\n");
          m=ci=0;
          currentPos=FILL_M;
          action=0;
        }
    }
  
  return 0;
}
/********************************************************/
/* Expects unsigned long m,c,and x, and int ch          */
/* m and c being lcg_m and lcg_c and x being the        */
/* random number generated from generateNextRandomvalue */
/* and ch being the char that needs to be encrypted.    */
/* if m and c passed in as 0 then print error and return*/
/* 1 meaning flag that error found to stop passing in   */
/* data. If operation leads to invalid byte then pass   */
/* through series of if statements until true.Return 0  */
/* if no error seen.                                    */
/********************************************************/
int encryptData(unsigned long m, unsigned long c, int ch,unsigned long x)
{
  int out=ch^(x%128);
  if(m ==0 || c== 0)
    {
      printf("Error");
      return 1;
    }
  if(out<32) printf("*%c",('?' + out));
  else if(out == 127) printf("*!");
  else if(out == '*') printf("**");
  else printf("%c",out);
  return 0;
}
/****************************************************************/
/* Expects 4 arguments, m being lcg_m, c being lcg_c, and       */
/* x being random value generated from generateNextRandomValue  */
/* and ch being the data that needs to be decrypted. It returns */
/* 0 if not error was detected or 1 if error is caught and data */
/* is not being fed in anymore (done in main).Does the operation*/
/* to char, then checks to see what it is. If it is * then it   */
/* will check char adjactent to it and based on what it is do   */
/* the appropriate steps to convert to orignal char prior to    */
/* encryption. Else it just prints out the initial generated    */
/* byte and returns 0.                                          */
/****************************************************************/
int decryptData(unsigned long m, unsigned long c, int ch,unsigned long x)
{
  int out=ch^(x%128);
  if(m==0 || c==0)
    {
      printf("Error");
      return 1;
    }
  if(ch == '*')
    {
      ch=getchar();
      if(ch == '*')
        {
          out=ch^(x%128);
	  printf("%c",out);
        }
      else if(ch == '!')
        {
          out=127^(x%128);
          printf("%c",out);
        }
      else if(ch>32 && ch<127)
        {
          out=(ch-'?')^(x%128);
          printf("%c",out);
        }
      else if(ch==32)
       {
         printf("Error");
         return 1;
       }
      
    }
  else printf("%c",out);
  
  return 0;
}
