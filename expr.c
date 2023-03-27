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
bool in_dict ;
char dict_keys[MAXTOKENS][TOKENLENGTH] ;
int dict_vals[MAXTOKENS];
int nof_dict_items = 0;

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

   /*
   for(int i = 0; i < numtokens; i++) {
      printf("%c ", *tokens[i]);
   }
   printf("\n");
   */

   // parse the expression 
   hyperexpr(str) ; 
   printf("%s\n",str) ; 
  
   return(0) ; 
}


void add_item_to_dict (char var[], int val) 
{
   sprintf(dict_keys[nof_dict_items], var);
   dict_vals[nof_dict_items] = val;
   nof_dict_items++; 
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
   
   if ( (strcmp(tokens[cur],"*") == 0 ) || (strcmp(tokens[cur],"/") == 0 ) ) {
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
   char str1[N] ; 
   
   str1[0] = '\0' ; 

    //printf("%s\n",tokens[cur]) ; 
    if ( is_integer(tokens[cur])) {
       strcpy(str,tokens[cur]) ; 
       strcat(str," ") ; 
       cur++ ; 
       return(1) ; 
    } 
    if ( is_variable(tokens[cur])) { // replace variable with the value stored, 0 if not declared
      // Check if the "brand" key exists in the dictionary
      in_dict = 0;
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
    
    if ( strcmp(tokens[cur],"(") == 0 ) {
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
 
