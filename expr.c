#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAXTOKENS   100 
#define TOKENLENGTH 20 
#define N  1000 

char tokens[MAXTOKENS][TOKENLENGTH] ;
char str[N] ; 
int  cur = 0 ; 
char dict_keys[MAXTOKENS][TOKENLENGTH] ;
int dict_vals[MAXTOKENS];
int nof_dict_items = 0;

int  begin(char *) ;
int  assignment(char *) ;
int  hyperexpr(char *) ;
int  moresuperexprs(char *) ;
int  superexpr(char *) ;
int  moreexprs(char *) ;
int  expr(char *) ;
int  term(char *) ;
int  moreterms(char *) ;
int  factor(char *) ;
int  morefactors(char *) ;
int  is_integer(char *) ;
int  is_variable(char *) ;
void add_item_to_dict(char var[], int val) ;

int main()
{
   int     numtokens ; 
   char    str[N]   ; 
   
   // read the tokens 
   numtokens = 0 ; 
   while (scanf("%s",tokens[numtokens]) != EOF) {
      numtokens++ ; 
   } 
   sprintf(tokens[numtokens],"$") ;
   numtokens++ ;

   add_item_to_dict("x", 1);
   add_item_to_dict("y", 2);

   //print the tokens
   /*
   for(int i = 0; i < numtokens; i++) {
      printf("%s ", tokens[i]);
   }
   printf("\n");
   */
   
   

   // parse the expression  
   if (begin(str) && (strcmp("$", tokens[cur]) == 0)) { //to check if the expression is fully parsed
         printf("%s\n",str) ;
      }
   else {
      //printf("%s\n",str) ;
      printf("Error!");
   }
  
   return(0) ; 
}


void add_item_to_dict (char var[], int val) 
{
   sprintf(dict_keys[nof_dict_items], var);
   dict_vals[nof_dict_items] = val;
   nof_dict_items++; 
}

int begin(char *str)
{
   char str1[N];
   str1[0] = '\0';
   if (! assignment(str1)) {
      cur = 0; // reset cursor since it might have moved due to token starting with a variable
      if (! hyperexpr(str1)) {
         return 0;
      }
   }
   strcpy(str,str1) ; 
   return(1) ; 
}

int assignment(char *str)
{
   char str1[N], str2[N] ; 
   str1[0] = str2[0] = '\0' ;
   if (is_variable(tokens[cur])) { //case 1 - variable initialized | case 2 - variable reassigned to a different value
      strcpy(str1, tokens[cur]);
      strcat(str1, " ");
      cur++;
      if (strcmp("=", tokens[cur]) == 0) {
         strcat(str1, tokens[cur]);
         strcat(str1, " ");
         cur++;
         if (! hyperexpr(str2)){
            return(0) ; 
         }
         strcat(str2,str1) ; 
         strcpy(str,str2) ; 
         return(1) ; 
      }
      return 0;
   }
   return 0;
}

int hyperexpr(char *str)
{
   char str1[N], str2[N] ; 
   
   str1[0] = str2[0] = '\0' ; 
   if (!  superexpr(str1)) {
      return(0) ; 
   } 
   if (!  moresuperexprs(str2)) {
      return(0) ; 
   }

   strcat(str1,str2) ; 
   strcpy(str,str1) ; 
   return(1) ;  
}

int moresuperexprs(char *str)
{
   char str1[N], str2[N], str3[N] ; 
   
   str1[0] = str2[0] = str3[0] = '\0' ; 
   
   if (strcmp(tokens[cur],"|") == 0 ) {
       strcpy(str1,tokens[cur]) ; 
       strcat(str1," ") ; 
       cur++ ;  
       if (!  superexpr(str2)) {
          return(0) ; 
       } 
       if (!  moresuperexprs(str3)) {
         return(0) ; 
       }
   } 
   strcat(str2,str3) ; 
   strcat(str2,str1) ; 
   strcpy(str,str2) ; 
   return(1) ;  
}

int superexpr(char *str)
{
   char str1[N], str2[N] ; 
   
   str1[0] = str2[0] = '\0' ; 
   if (!  expr(str1)) {
      return(0) ; 
   } 
   if (!  moreexprs(str2)) {
      return(0) ; 
   }

   strcat(str1,str2) ; 
   strcpy(str,str1) ; 
   return(1) ;  
}

int moreexprs(char *str)
{
   char str1[N], str2[N], str3[N] ; 
   
   str1[0] = str2[0] = str3[0] = '\0' ; 
   
   if (strcmp(tokens[cur],"&") == 0 ) {
       strcpy(str1,tokens[cur]) ; 
       strcat(str1," ") ; 
       cur++ ;  
       if (!  expr(str2)) {
          return(0) ; 
       } 
       if (!  moreexprs(str3)) {
         return(0) ; 
       }
   } 
   strcat(str2,str3) ; 
   strcat(str2,str1) ; 
   strcpy(str,str2) ; 
   return(1) ;  
}

int expr(char *str)
{
   char str1[N], str2[N] ; 
   
   str1[0] = str2[0] = '\0' ; 
   if (!  term(str1)) {
      return(0) ; 
   } 
   if (!  moreterms(str2)) {
      return(0) ; 
   }
   strcat(str1,str2) ; 
   strcpy(str,str1) ; 
   return(1) ;  
}

int moreterms(char *str)
{
   char str1[N], str2[N], str3[N] ; 
   
   str1[0] = str2[0] = str3[0] = '\0' ; 
   
   if ( (strcmp(tokens[cur],"+") == 0 ) || (strcmp(tokens[cur],"-") == 0 ) ) {
       strcpy(str1,tokens[cur]) ; 
       strcat(str1," ") ; 
       cur++ ;  
       if (!  term(str2)) {
          return(0) ; 
       } 
       if (!  moreterms(str3)) {
         return(0) ; 
       }
   } 
   strcat(str2,str3) ; 
   strcat(str2,str1) ; 
   strcpy(str,str2) ; 
   return(1) ;  
}

int term(char *str)
{
   char str1[N], str2[N] ; 
   
   str1[0] = str2[0] = '\0' ; 
   if (!  factor(str1)) {
      return(0) ; 
   } 
   if (!  morefactors(str2)) {
      return(0) ; 
   }

   strcat(str1,str2) ; 
   strcpy(str,str1) ; 
   return(1) ;  
}

int morefactors(char *str)
{
   char str1[N], str2[N], str3[N] ; 
   
   str1[0] = str2[0] = str3[0] = '\0' ; 
   
   if (strcmp(tokens[cur],"*") == 0 ) {
       strcpy(str1,tokens[cur]) ; 
       strcat(str1," ") ; 
       cur++ ;  
       if (!  factor(str2)) {
          return(0) ; 
       } 
       if (!  morefactors(str3)) {
         return(0) ; 
       }
   } 
   strcat(str2,str3) ; 
   strcat(str2,str1) ; 
   strcpy(str,str2) ; 
   return(1) ;  
}

int factor(char *str)
{
   char str1[N], str2[N], str3[N] ; 
   str1[0] = str2[0] = str3[0] = '\0' ; 

    if ( is_integer(tokens[cur])) { //factor -> int
       strcpy(str,tokens[cur]) ; 
       strcat(str," ") ; 
       cur++ ; 
       return(1) ; 
    } 

    //factor -> function
   if ( strcmp(tokens[cur],"not") == 0 ) { //function -> not(hyperexpr)
      strcpy(str1,tokens[cur]) ; 
      strcat(str1," ") ; 
      cur++ ; 
      if ( strcmp(tokens[cur],"(") != 0 ) {
         return (0);
      }
      cur++ ;
      if ( ! hyperexpr(str2) ) {
         return(0) ; 
      }
      if ( strcmp(tokens[cur],")") != 0 ) { 
         return(0) ;
      }
      cur++ ;
      strcat(str2, str1);
      strcpy(str,str2) ; 
      return(1) ; 
   }

   if ( (strcmp(tokens[cur],"xor") == 0) || (strcmp(tokens[cur],"ls") == 0) || (strcmp(tokens[cur],"rs") == 0) || (strcmp(tokens[cur],"lr") == 0) || (strcmp(tokens[cur],"rr") == 0) ) { //function -> xor|ls|rs|lr|rr(hyperexpr,hyperexpr)
      strcpy(str1,tokens[cur]) ; 
      strcat(str1," ") ; 
      cur++ ; 
      if ( strcmp(tokens[cur],"(") != 0 ) {
         return (0);
      }
      cur++ ;
      if ( ! hyperexpr(str2) ) {
         return(0) ; 
      }
      if ( strcmp(tokens[cur],",") != 0 ) {
         return (0);
      }
      cur++;
      if ( ! hyperexpr(str3) ) {
         return(0) ; 
      }
      if ( strcmp(tokens[cur],")") != 0 ) { 
         return(0) ;
      }
      cur++ ;
      strcat(str2,str3);
      strcat(str2, str1);
      strcpy(str,str2) ; 
      return(1) ; 
   }

    

    if ( is_variable(tokens[cur])) { // factor -> var: replace variable with the value stored, replace with 0 if not declared
      int in_dict = 0;
      for (int i = 0; i < nof_dict_items; i++) {
         if (strcmp(dict_keys[i], tokens[cur]) == 0) {
            in_dict = 1;
            sprintf(tokens[cur], "%d", dict_vals[i]); // if in dict, use value instead
            break;
         }
      }
      if (! in_dict) {
         sprintf(tokens[cur], "0"); ; // if not in dict, use value 0 instead
      }   
      strcpy(str,tokens[cur]) ; 
      strcat(str," ") ; 
      cur++ ; 
      return(1) ; 
    } 
    
    if ( strcmp(tokens[cur],"(") == 0 ) { //factor -> (hyperexpr)
       cur++ ; 
       if ( ! hyperexpr(str1) ) {
          return(0) ;    
       }
       if ( strcmp(tokens[cur],")") != 0 ) { 
          printf("Error: expecting paranthesis\n") ; 
          return(0) ;
       }
       cur++ ; 
       strcpy(str,str1) ; 
       return(1) ; 

    }
    printf("Error: expecting factor\n") ; 
    return(0) ; 
}

int is_integer(char *token) 
{
    int isnumber = 1 ;
    char *q ; 

    for(q = token ; *q != '\0' ; q++) {
        isnumber = isnumber && isdigit(*q) ;
    }

    return(isnumber) ; 
}    

int is_variable(char *token) 
{
    int isvariable = 1 ;
    char *q ; 

    for(q = token ; *q != '\0' ; q++) {
        isvariable = isvariable && (isupper(*q) || islower(*q)) ;
    }

    return(isvariable) ; 
}    
 
