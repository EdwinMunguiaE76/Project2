// Author: Shikang Xu; ECE 353 TA



// List the full names of ALL group members at the top of your code.

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <math.h>

#include <assert.h>

#include <ctype.h>

//feel free to add here any additional library names you may need

#define SINGLE 1

#define BATCH 0

#define REG_NUM 32

char *progScanner(char *string);

    void deleteSpace(char *string);

    void deletePar(char *string);

    void deleteComma(char *string);

    void delChar(char *string, int index, int limit);

    void IF();
    void ID();
    void EX();
    void MEM();
    void WB();



char *regNumberConverter(char *string);
    char* findReg(char *reg);
    char* concat(const char *s1, const char *s2);

    char *mystrchr (const char *s, int c);
    char *mystrcpy (char *dest, const char *src);
    char *mystrncpy (char *dest, const char *src, size_t n);
    char *mystrncpy (char *dest, const char *src, size_t n);

struct inst parser(char *string);

struct inst findname(char *string);

static int i;
static int j;
static int money;
static int pc;
static int pclimit;
static int flag;
static int *datamem;
static struct inst *instmem;
static int *regmem;
//**************instruction object*****************

enum type {//R-type=even, I-type=odd including 0---> to find if it
  addi=8,//0///I or R typelater just do type%2==1 to see if R-type and if not its I-type.
  lw=23,//1
  add=0,//2
  sw=43,//3
  sub=0,//4
  beq=4,//5
  mul=0//6
};

struct inst{
  enum type opcode;

  int rs;
  int rt;
  int rd;
  int shamt;
  int funct;
  int immediate;
  //this is to tell what type of instruction it is Addi sub etc.
  //we need to define the variables R-Type, I-Type, or J-Type
      //we could this using   using designated  initializers
     //struct p = { .y = 2, .x = 1 }; so we can pick the variables based on the type
    //R-type: opcode         $rs     $rt      $rd     shamt          funct
    //I-type: opcode          $rs     $rt      imm
    //Hard part: how do we know the type??
};

static struct inst *instmem;


struct IFID
{
  struct inst data;
};
struct IFID ifid;


struct IDEX
{
  enum type opcode;
  int rs;
  int rt;
  int rd;
  int shamt;
  int funct;
  int immediate;
};
struct IDEX idex;

struct EXMEM {
  enum type opcode;
  int reg;
  int value;
};

struct EXMEM exmem;

struct MEMWB {
  enum type opcode;
  int reg;
  int value;
};

struct MEMWB memwb;

/**
** char *mystrcat (char *dest, const char *src)
** An implementation of strcat that works on Quark
*/
char *mystrcat(char *dest, const char *src){
    size_t i,j;
    for (i = 0; dest[i] != '\0'; i++)
        ;
    for (j = 0; src[j] != '\0'; j++)
        dest[i+j] = src[j];
    dest[i+j] = '\0';
    return dest;
}


/**
** mystrchr (const char *s, int c)
** An implementation of strchr that works on Quark
*/
char *mystrchr (const char *s, int c) {
  for (; *s != '\0'; ++s)
    if (*s == c)
      return (char *) s;

  return NULL;
}

/*
** char *mystrcpy (char *dest, const char *src)
** An implementation of strcpy that works on Quark
*/
char *mystrcpy (char *dest, const char *src) {
  size_t i;
  for (i = 0; src[i] != '\0'; ++i)
    dest[i] = src[i];

  dest[i] = src[i];
  return dest;
}

/*
** char *mystrncpy (char *dest, const char *src, size_t n)
** An implementation of strncpy that works on Quark
*/
char *mystrncpy (char *dest, const char *src, size_t n) {
  size_t i;
  for (i = 0; src[i] != '\0'; ++i)
    dest[i] = src[i];

  for (; i < n; ++i)
    dest[i] = '\0';

  return dest;
}
int mystrcmp (const char * s1, const char * s2){
    for(; *s1 == *s2; ++s1, ++s2)
        if(*s1 == 0)
            return 0;
    return *(unsigned char *)s1 < *(unsigned char *)s2 ? -1 : 1;
}


/*
** char *mystrstr (const char *haystack, const char *needle)
** An implementation of strstr that works on Quark
*/
char *mystrstr (const char *haystack, const char *needle) {
  char *p1 = (char *) haystack;
  int i, len = strlen(needle);

  while (*p1 != '\0') {
    for (i = 0; p1[i] == needle[i] && i < len; ++i) {  }
    if (i == len) return p1;
    ++p1;
  }

  return (char *) NULL;
}

void RegMemPrint(){
  for(i=0; i<32; i++){
    printf("Reg %d = %d \n",i , regmem[i]);
  }

}
//**************Programscanner method*****************

char *progScanner(char *string){


  deletePar(string);
  deleteComma(string);
  deleteSpace(string);
  //printf("%s\n", string );
  return string;
  //remove all duplicate spaces, all parentheses and all commas
  //returns the pointer to the edited string
  //error detection: mismatch parentheses in lw or sw then its a error that will
  //stop the program
  //double commas do not count as a error
  //  Ex: lw   $s0     8($t0)   ==> lw $s0 8 $t0
  //should give a error if lw $s0 8(($t0)
}
      void deleteSpace(char *string){
        int limit=strlen(string);
        for(i = 0; i<(limit-1); i++) //Overall loop, checking
          {
            //point= string[i];
            //point2=string[i+1];//limit-1 is so this term does not exceed the bounds
            if(string[i]==' ' && string[i+1]== ' ')
                {
                  delChar(string, i, limit);
                  i--;
                  limit--;//del charcter will shorten the string length
                }
          }
          if(string[0]==' ')
          delChar(string,0,limit);
      }
      void deletePar(char *string){
          char open = '(';
          char close= ')';
          int openCount = 0;
          int closeCount = 0;
          int limit=strlen(string);
          for(i = 0; i<(limit-1); i++) //Overall loop, checking
            {
              if(string[i]==open)
                {
                  string[i] = ' ';
                //  delChar(string, i, limit);
                //  i--;
                  openCount++;
                  //limit--;
                }

                else if(string[i]==close)
                  {
                      string[i] = ' ';
                  //  delChar(string, i, limit);
                  //  i--;
                    closeCount++;
                  //  limit--;
                  }

            }
            deleteSpace(string);
            if(closeCount==openCount)
            return;
            else if(string[0]=='l'||string[0]=='s')
            {
              printf("Non-coupled parenthesis detected");//only do this if it is lw and sw
              exit(0);
            }
        }

        void deleteComma(char *string)  {
            char comma= ',';
            int limit=strlen(string);
            for(i = 0; i<(limit); i++) //Overall loop, checking
              {
                if(string[i]==comma)
                  {
                    delChar(string, i, limit);
                    i--;
                    limit--;
                  }
              }
          }

                void delChar(char *string, int index , int limit)    {
                      for(j=index; j<(limit-1);j++)
                            {
                              string[j]=string[j+1];
                            }//shift
                          string[limit-1] = '\0';
                          //   strncpy(string,string,limit-3);
                  }
//*************regNumberConverter*****************
char *regNumberConverter(char *string){
//  printf("%s /n", string);
  int count=0;
  int count2=0;
  for(i=0;i<strlen(string);i++)
        if(string[i]=='$')
          count++;//Counts the number of $ inside instruction
          money=count;
  const char s[2]="$";
  char *token= string;
  char* spac=" ";
  char *strfin;
  strfin=strtok(string,s);
  while(count2!=count)
  {
    token =strtok(NULL,s);

    token=findReg(token);

    strfin=concat(strfin, token);
    strfin=concat(strfin,spac);
    deleteSpace(strfin);

      count2++;
    }

  //printf("%s\n", strfin);
  return strfin;
  //input= output of the progscanner
  //it will find all the $, it will replace everything from the $ to the next space with
  //the respective register. Ex: lw $s0 8 $t0 ==>  lw 16 8 8 because s0 is reg 16 and t0 is reg 8
  //another example is lw $16 8 $8 ==> lw 16 8 8 it should be able to handle these 2 cases
  //
  //returns the pointer to the edited string
  //Error: if a illegal register is detected then it should give a error message and halt simulation, this could also be a register too by or something
  //like $y0
}

char* concat(const char *s1, const char *s2){
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    mystrcpy(result, s1);
    mystrcat(result, s2);
    return result;
}

      char* findReg(char *reg)  {
        int temp=0;
        int flag =0;
        int flag2 =0;
        int offset=0;
        char val;
        char *longgg;
        int im=0;
        int scount=0;

        for(i=0;reg[i]!=' ';i++)
            {scount++;}//number of char till space
              if(isdigit(reg[scount+1])||reg[scount+1]=='-')
              im=1;



        if(im==1&&money!=3)
        {
          longgg = malloc((strlen(reg)-(scount))*sizeof(char));
          mystrncpy(longgg, reg+scount, strlen(reg)-(scount-1));
          mystrncpy(reg, reg, scount);

          flag = 1;

        }

        if(((!isdigit(reg[0]))&&isdigit(reg[2]))||(isalpha(reg[2])&&reg[0]!='z'))
        {

          printf("Is not a valid register\n");
          exit(0);
        }

        if(reg[0]=='z'){//these if statements will find the number of the register
            temp=0;
            flag2 = 1;
          }

        else if(reg[0]=='a'&&reg[1]=='t')
            temp=1;

        else if(reg[0]=='v'&&isdigit(reg[1]))
        {
          offset= reg[1]-'0';
            temp=2+offset;
          }

        else if(reg[0]=='a'&&isdigit(reg[1]))
          {
            offset= reg[1]-'0';
              temp=4+offset;
            }

       if(reg[0]=='t'&&isdigit(reg[1]))
            {

              offset= reg[1]-'0';
                temp=8+offset;
              }

        else if(reg[0]=='s'&&isdigit(reg[1]))
          {
            offset= reg[1]-'0';
            temp=16+offset;
                }

                if(reg[0]=='t'&&(reg[1]=='8'))
                  temp=24;

                if(reg[0]=='t'&&(reg[1]=='9'))
                  temp=25;

        else if(reg[0]=='k'&&isdigit(reg[1]))
          {
            offset= reg[1]-'0';
            temp=26+offset;
                }

        else if(reg[0]=='g'&&reg[1]=='p')
            temp=28;

        else if(reg[0]=='s'&&reg[1]=='p')
            temp=29;

        else if(reg[0]=='f'&&reg[1]=='p')
            temp=30;

        else if(reg[0]=='r'&&reg[1]=='a')
            temp=31;

          else if(temp==0 && flag2==0)
          {
            if(isalpha(reg[0]))
            {

              printf("Is not a valid register\n");
              exit(0);
            }

            else if(isdigit(reg[0]))
            {
              int digcount=0;
              for(i=0;isdigit(reg[i]);i++)
                  digcount++;
                  //printf("%d", digcount);
            char *check;
            int dec=0;
            check = malloc(10*sizeof(char));
            mystrncpy(check, reg, scount);
	           for(i=0; i<digcount; i++)
		             dec = dec * 10 + ( check[i] - '0' );
              if(dec>31)
              {
                printf("Is not a valid register\n");
                exit(0);
              }

              else
              temp=dec;
              //free(check);

          }

          }

          //printf("%s\n", longgg);
            if(temp>31)
            {
              printf("Is not a valid register\n");
              exit(0);
            }

          sprintf(reg, "%d", temp); //converts temp from int to string
            if(flag==1){

            reg=mystrcat(reg, longgg);
          }
          //free(longgg);
            return reg;
      }

//**************Struct inst parser*****************

struct inst parser(char *string){
//It takes in the output of regNumberConverter
//this should take the instruction we have and turn it into mips instruction form
//Ex:  lw 16 8 8 ==> opcode      $rs     $rt      imm
//all this info will get store into inst which is a object we made up top
//This is by far the hardest method so far
//Errors: Illegal opcode or if Imm field cannot be represented by 16 bits(too big),
// or Missing $ in front of a register name (e.g., a program which says s0 instead of $s0).
//always stop a program after a error
              struct inst instruction;
              const char s[2]=" ";
              char *opcode=malloc(10*sizeof(char));
              char *first=malloc(10*sizeof(char));
              char *second=malloc(10*sizeof(char));
              char *third=malloc(10*sizeof(char));
              int r1=0;
              int r2=0;
              int r3=0;
              int neg1=1;
              int neg2=1;

              instruction.immediate =0;
              instruction.rt =0;
              instruction.rs =0;
              instruction.rd =0;
              instruction.shamt =0;
              instruction.funct =0;


              opcode=strtok(string,s);
              first=strtok(NULL,s);
            //  printf("%s first ", first);
              for(i=0; i<strlen(first); i++)
 		             r1= r1*10 + ( first[i] - '0' );


              second=strtok(NULL,s);
            //  printf("%s second ", second);
              if(!second)
              {printf("not enough registers");
              exit(0);
            }

                        for(i=0; i<strlen(second); i++)
                                      if(second[i]=='-')
                                        {
                                        second[i]=' ';
                                          deleteSpace(second);
                                          neg1=-1;
                                        }

              for(i=0; i<strlen(second); i++)
 		              r2= r2 * 10 + ( second[i] - '0' );
                  r2=r2*neg1;



              third=strtok(NULL,s);
            //  printf("%s third ", third);
              if(!third && money!= 2)
              {printf("not enough registers");
              exit(0);
            }

            if(!third && money == 2)
            {
              r3 = r2;
             r2 = 0;
            }
            else{
                        for(i=0; i<strlen(third); i++)
                                      if(third[i]=='-')
                                        {
                                        third[i]=' ';
                                          deleteSpace(third);
                                          neg2=-1;
                                        }

              for(i=0; i<strlen(third)&&third[i]!='\n'; i++)
              //if(isdigit(third[i]))
 		              r3= r3 * 10 + ( third[i] - '0' );
                  r3=r3*neg2;
                }

                    if(mystrcmp(opcode,"addi")==0)
                    {
                      assert(money==2);

                    instruction.opcode=addi;
                    instruction.rt=r1;
                    instruction.rs=r2;
                    instruction.immediate=r3;

                  /*printf(" addi \n");
                    printf("%d\n", instruction.opcode);
                    printf("%d\n", instruction.rt);
                    printf("%d\n", instruction.rs);
                    printf("%d\n", instruction.immediate);
                      printf("-----------------");*/
                  }

                   else if(mystrcmp(opcode,"lw")==0)
                      {
                        assert(money==2);
                        instruction.opcode=lw;

                        instruction.rt=r1;
                        instruction.immediate=r2;
                        instruction.rs=r3;

                      /*  printf(" lw \n");
                        printf("%d\n", instruction.opcode);
                        printf("%d\n", instruction.rt);
                        printf("%d\n", instruction.immediate);
                        printf("%d\n", instruction.rs);
                        printf("-----------------");*/
                      }

                    else if(mystrcmp(opcode,"add")==0)
                    {
                      assert(money==3);
                      instruction.opcode=add;
                      instruction.rd=r1;
                      instruction.rs=r2;
                      instruction.rt=r3;
                      instruction.shamt=0;
                      instruction.funct=32;

                    /*  printf(" add \n");
                      printf("%d\n", instruction.opcode);
                      printf("%d\n", instruction.rd);
                      printf("%d\n", instruction.rs);
                      printf("%d\n", instruction.rt);
                      printf("%d\n", instruction.shamt);
                      printf("%d\n", instruction.funct);
                        printf("-----------------");*/
                    }

                    else if(mystrcmp(opcode,"sw")==0)
                    {
                      assert(money==2);
                      instruction.opcode=sw;
                      instruction.rt=r1;
                      instruction.immediate=r2;
                      instruction.rs=r3;

                    /*  printf(" sw \n");
                      printf("%d\n", instruction.opcode);
                      printf("%d\n", instruction.rt);
                      printf("%d\n", instruction.immediate);
                      printf("%d\n", instruction.rs);
                        printf("-----------------");*/
                    }

                    else if(mystrcmp(opcode,"sub")==0)
                    {
                        assert(money==3);
                      instruction.opcode=sub;
                      instruction.rd=r1;
                      instruction.rs=r2;
                      instruction.rt=r3;
                      instruction.shamt=0;
                      instruction.funct=34;

                    /*  printf(" sub \n");
                      printf("%d\n", instruction.opcode);
                      printf("%d\n", instruction.rd);
                      printf("%d\n", instruction.rs);
                      printf("%d\n", instruction.rt);
                      printf("%d\n", instruction.shamt);
                      printf("%d\n", instruction.funct);
                      printf("-----------------");*/
                    }

                    else if(mystrcmp(opcode,"beq")==0)
                    {
                      assert(money==2);
                      instruction.opcode=beq;
                      instruction.rs=r1;
                      instruction.rt=r2;
                      instruction.immediate=r3;

                    /*  printf(" beq \n");
                      printf("%d\n", instruction.opcode);
                      printf("%d\n", instruction.rs);
                      printf("%d\n", instruction.rt);
                      printf("%d\n", instruction.immediate);
                        printf("-----------------");*/
                    }

                    else if(mystrcmp(opcode,"mul")==0)
                    {
                        assert(money==3);
                      instruction.opcode=mul;
                      instruction.rd=r1;
                      instruction.rs=r2;
                      instruction.rt=r3;
                      instruction.shamt=0;
                      instruction.funct=24;

                    /*  printf(" mul \n");
                      printf("%d\n", instruction.opcode);
                      printf("%d\n", instruction.rd);
                      printf("%d\n", instruction.rs);
                      printf("%d\n", instruction.rt);
                      printf("%d\n", instruction.shamt);
                      printf("%d\n", instruction.funct);
                      printf("-----------------");*/

                    }

                    else{
                        printf("illegal OPCODE");
                      exit(0);
                    }

                    assert(instruction.rt<32);
                    assert(instruction.rs<32);
                    assert(instruction.rd<32);
                    if(instruction.immediate > 65535){
                        printf("immediate overflow (Greater than 16 bits)");
                      exit(0);
                    }

                    //free(opcode);
                    //free(first);
                  //free(second);
                  //free(third);

          //    printf("%d\n", instruction.opcode);
                return instruction;

            }

//**************Pipeline methods*****************

//***************************BEGINS PIPELINE / DATAPATH METHODS************************************

//NOTE WE NEED TO INITIALIZE TWO SEPARATE MEMORY ARRAYS
/*
INSTRUCTION MEMORY: Addressed by the PC, 2k bytes
DATA MEMORY: Accessible by the program, 2k bytes

*/

void IF()
{
  //   RegMemPrint();

if(flag==0)
{
 ifid.data=instmem[pc];
 pc++;

 //printf("%d immediate\n", ifid.data.immediate);
}
//   printf("%d in\n", ifid.data.opcode);
 }
/*
For IF we are going to need a PC variable that holds the current instruction. "The Program Counter,
or PC, is a register that holds the address of the current instruction." It's important to remember that
the stages themselves are not pipelined, so if an add instruction takes "4 cycles", the next instructions can't enter
the EX stage until after the 4 cycles. In this step we should read the address from the PC.(Using our instruction memory array)
Basically in this stage we are "grabbing" the address from the instruction memory. Should be the easiest stage coding wise.
*/

void ID()
{
  //   RegMemPrint();
 if( ifid.data.opcode == beq){
  {
  const struct IFID reset;
   ifid.data = reset.data;
   flag=1;
 }

 }

  if(ifid.data.opcode==0)
  {
    idex.opcode=0;
    idex.rs=regmem[ifid.data.rs];
    idex.rt=regmem[ifid.data.rt];
    idex.rd=ifid.data.rd;
    idex.funct=ifid.data.funct;
    idex.immediate=0;
  }
  else if(ifid.data.opcode == lw || ifid.data.opcode == addi )
{
  idex.opcode=ifid.data.opcode;
  idex.rs=regmem[ifid.data.rs];
  idex.rt=ifid.data.rt;
  idex.rd=0;
  idex.funct=0;
  idex.immediate=ifid.data.immediate;
}

  else if(ifid.data.opcode == sw||ifid.data.opcode == addi)
  {
    idex.opcode=ifid.data.opcode;
    idex.rs=regmem[ifid.data.rs];
    idex.rt=regmem[ifid.data.rt];
    idex.rd=0;
    idex.funct=0;
    idex.immediate=ifid.data.immediate;

  }
/*
This is the stage in which we "decode" our instruction. It takes one cycle to decode the instruction.
That means we should separate the instruction into operands, operation, etc.
This is the stage in which we check for all hazards. (I will outline those below)
When a branch is detected in this stage (ie beq), all instruction decoding stops until it resolved.
Branches are resolved in the EX stage where a subtraction operation is carried out and the result
used to determine if the branch is taken or not. Branches are resolved in the EX stage where a subtraction
operation is carried out and the result used to determine if the branch is taken or not.
*/
}

void EX()
{
  //   RegMemPrint();
  if(idex.opcode == 0){
    exmem.opcode=0;
    ////////////////////ADD//////////////
    if(idex.funct== 32){
     exmem.value = idex.rs + idex.rt;
     exmem.reg = idex.rd;
    }

    ////////////////////SUB//////////////
    else if(idex.funct== 34){
      exmem.value = idex.rs - idex.rt;
      exmem.reg = idex.rd;
    }

    ////////////////////MUL//////////////
    else if(idex.funct== 24){
      exmem.value = idex.rs * idex.rt;
      exmem.reg = idex.rd;
    }

  }

  if(idex.opcode== addi){
  exmem.opcode=addi;
   exmem.value = idex.rs + idex.immediate;
   exmem.reg = idex.rt;
  }

  if(idex.opcode== sw){
  exmem.opcode=sw;
  exmem.value = idex.rt;
  exmem.reg = (idex.rs + (idex.immediate))/4;   ////datamem address
  }

  if(idex.opcode== lw){
  exmem.opcode = lw;
  exmem.reg = idex.rt;
  exmem.value = (idex.rs + (idex.immediate))/4;   ////datamem address
  }

  if(idex.opcode== beq){
   flag=0;
   exmem.opcode = beq;
   if(idex.rs == idex.rs)
   pc = pc + 1 + idex.immediate;
  }

/*
At this stage we take our operands from the ID stage, and execute the operation (also determined from the ID stage). Maybe
we should create helper methods for "Add", "subtract", "multiplication", etc. We should use a counter to keep track of
ongoing operations.

We are also using data memory array for the data memory address.

ONLY ONE instruction can be at EX at a time.
*/

}
void MEM()
{
    // RegMemPrint();
 if(exmem.opcode== beq){
   memwb.opcode = beq;
 }

  if(exmem.opcode== sw){
    datamem[exmem.reg]=exmem.value;
    memwb.opcode = sw;
  }
  if(exmem.opcode== lw){
  memwb.value =  datamem[exmem.value];
  memwb.reg = exmem.reg;
  memwb.opcode = 0;
  }

  if(exmem.opcode == 0 ||exmem.opcode == addi){
    memwb.value = exmem.value;
    memwb.reg = exmem.reg;
    memwb.opcode = 0;
  }

}
/*
This stage is only used for LW and SW instructions. It receives the data memory address from the EX stage and carries out the data
read/write operation. This takes "c" amount of cycles to do.
*/
void WB()
{
     //RegMemPrint();
  if(memwb.opcode == 0){
    //printf("inside");
      regmem[memwb.reg] = memwb.value;
    //  printf("%d reg\n", memwb.reg);
    //  printf("%d value\n", memwb.value);
    }
}
/*
Writes back into the register file.
*/

//***************************DATA HAZARDS***************************
/*
A stage has to make sure that the
Latch it is updating is empty before it updates it.
Latch data it is consuming are valid before it reads it.
For a multi-cycle operation in any stage, the latch it is consuming from only empties out in the final cycle of that operation.
A latch can be filled (by its producing stage) in the same cycle in which its consuming stage empties out.
*/

//**************************read from file************************
//this part of the code until fclose should read the instruction from the FILE
//and then give it to the program scanner where it gets converted to the instruction/opcode


//*************************read from file************************
//this part of the code until fclose should read the instruction from the FILE
//and then give it to the program scanner where it gets converted to the instruction/opcode

//We will start this after parser works
int main (int argc, char *argv[]){

           int sim_mode=0;//mode flag, 1 for single-cycle, 0 for batch
           int c,m,n;
           //int i;//for loop counter
           long mips_reg[REG_NUM];
           long pgm_c=0;//program counter
           long sim_cycle=0;//simulation cycle counter
           //define your own counter for the usage of each pipeline stage here
           int test_counter=0;
           FILE *input=NULL;
           FILE *output=NULL;
           printf("The arguments are:");
           for(i=1;i<argc;i++){
                      printf("%s ",argv[i]);
           }
           printf("\n");
           if(argc==7){
                      if(mystrcmp("-s",argv[1])==0){
                                 sim_mode=SINGLE;
                      }

                      else if(mystrcmp("-b",argv[1])==0){
                                 sim_mode=BATCH;
                      }
                      else{
                                 printf("Wrong sim mode chosen\n");
                                 exit(0);
                      }
                      m=atoi(argv[2]);
                      n=atoi(argv[3]);
                      c=atoi(argv[4]);

                      input=fopen(argv[5],"r");

                      output=fopen(argv[6],"w");

           }

           else{
                      printf("Usage: ./sim-mips -s m n c input_name output_name (single-sysle mode)\n or \n ./sim-mips -b m n c input_name  output_name(batch mode)\n");
                      printf("m,n,c stand for number of cycles needed by multiplication, other operation, and memory access, respectively\n");
                      exit(0);
           }

           if(input==NULL){

                      printf("Unable to open input or output file\n");
                      exit(0);
           }

           if(output==NULL){
                      printf("Cannot create output file\n");
                      exit(0);
           }

           //initialize registers and program counter

           if(sim_mode==1){
                      for (i=0;i<REG_NUM;i++){
                                 mips_reg[i]=0;
                      }
           }



  flag=0;
  char *instruction;
  instmem = (struct inst*) malloc(512*sizeof(struct inst));//instruction memory
  datamem = malloc(512*sizeof(int));//data memory
  regmem= malloc(32*sizeof(int));

  for(i=0; i<32; i++){
    regmem[i] =0;
  }


  instruction = malloc(50*sizeof(char));
 pclimit=0;
  pc=0;
      while (!feof(input))
        {
        fgets(instruction, 75, input);
        if(mystrstr(instruction,"haltSimulation")!=NULL)
        break;
        instmem[pclimit]=parser(regNumberConverter(progScanner(instruction)));
          pclimit++;
        }

        while(pc<=pclimit+5)//pipeline
        {
        WB();
        MEM();
        EX();
        ID();
        IF();
       }

       RegMemPrint();
      //DONT DELETE ANY OF THESE
      fclose(input);
      fclose(output);

//Program is loaded
  //________________________________________________________________
}
