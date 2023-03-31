#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAXTOKENS   128 
#define TOKENLENGTH 256 
#define N  1000
#define INT_BITS 64 // to use in left & right rotation

char tokens[MAXTOKENS][TOKENLENGTH] ;
char str[N] ; 
int  cur = 0 ; 
char dict_keys[MAXTOKENS][TOKENLENGTH] ;
long long int dict_vals[MAXTOKENS];
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
void evaluator (char *);

int main()
{
   char input_line[N];
   printf(">"); //input symbol
   //read the line & tokenize
    while (fgets(input_line, sizeof(input_line), stdin) != NULL){
      
      int     numtokens = 0 ; 
      char    str[N] = "" ; 
      char* tkn;
      char tokenized_line[N] = "";

      //reset the values
      strcpy(str,"") ; 
      cur = 0 ;

      input_line[strcspn(input_line,"\n")] = '\0'; //remove "\n"
      int len = strlen(input_line);
      for(int i = 0; i < len; i++) {
         if(input_line[i] == ' ') {
               for(int j = i; j < len; j++) {
                  input_line[j] = input_line[j+1];
               }
               len--;
               i--;
         }
      }
      for (int i = 0; i < len; i++) {
         if (isdigit(input_line[i])) {
               strncat(tokenized_line, &input_line[i],1);
               while (isdigit(input_line[i+1])) {
                  i++;
                  strncat(tokenized_line, &input_line[i],1);
               }
               strcat(tokenized_line," ");
         } else if (isalpha(input_line[i])) {
               strncat(tokenized_line, &input_line[i],1);
               while (isalpha(input_line[i+1])) {
                  i++;
                  strncat(tokenized_line, &input_line[i],1);
               }
               strcat(tokenized_line," ");
         } else if (ispunct(input_line[i])) {
               strncat(tokenized_line, &input_line[i],1);
               strcat(tokenized_line," ");
         } else if (isspace(input_line[i])) {
               continue;
         }
      }
      //if blank input or only comment, continue and scan the next line
      if (strlen(tokenized_line) == 0 || tokenized_line[0] == '%'){
         printf(">"); //input symbol 
         continue;
      }

      //put the tokens into array
      tkn = strtok(tokenized_line, " ");
      do {
         strcpy(tokens[numtokens], tkn);
         if ((strcmp(tokens[numtokens],"%") == 0)) { //rest is comments, dont read
            break;
         }
         numtokens++;
         tkn = strtok(NULL, " ");
      } while(tkn != NULL);
      sprintf(tokens[numtokens],"$") ;
      numtokens++ ;
      // parse the expression  
      if (begin(str) && (strcmp("$", tokens[cur]) == 0)) { //to check if the expression is fully parsed
         evaluator(str); //prints the result to the output
         }
      else {
         printf("Error!\n");
      }
      printf(">"); //input symbol    
   }
   return(0) ; 
}

void evaluator (char *str) //calculates the parsed input stored in str
{
   int cur_eval = 0;
   long long int op1, op2;
   char variable[TOKENLENGTH];
   char stack1[MAXTOKENS][TOKENLENGTH], stack2[MAXTOKENS][TOKENLENGTH] ;
   int top1, top2 = -1;
   char str_c[N];
   char elm[TOKENLENGTH];
   char operand;

   //add to the stack
   strcpy(str_c,str);
   char* token_str = strtok(str_c, " ");
   do {
      strcpy(stack1[cur_eval], token_str);
      cur_eval++;
      token_str = strtok(NULL, " ");
   } while(token_str != NULL);
   top1 = cur_eval-1; //last index of the first queue
   
   //reverse the stack1, store in stack2. Reverse order used to get from the first to last token when popping 
   while (top1 >= 0) {
      strcpy(stack2[++top2],stack1[top1--]);     
   }

   //start evaluating: pop from stack2, make the operation, put the result to stack1 until there is no element left
   cur_eval = 0;
   while (true) {
      strcpy(elm,stack2[top2--]); //pop the element
      if((strcmp(elm, "not")) == 0){
         op1 = strtoll(stack1[top1--], NULL, 10);
         sprintf(stack1[++top1], "%lli", ~op1);
         continue;
      }
      
      if (strcmp(elm, "=") == 0) { //if the operation is variable assignment (stored in this postfix format: 2 x =)
            strcpy(variable, stack1[top1--]);
            op1 = strtoll(stack1[top1--], NULL, 10);
            add_item_to_dict(variable, op1);
            break;
         }
      if ((strcmp(elm, "+") == 0)|| (strcmp(elm, "*") == 0) || (strcmp(elm, "-") == 0) ||(strcmp(elm, "&") == 0) || (strcmp(elm, "|") == 0) || (strcmp(elm,"xor") == 0) || (strcmp(elm,"ls") == 0) || (strcmp(elm,"rs") == 0) || (strcmp(elm,"lr") == 0) || (strcmp(elm,"rr") == 0)) {//if elm is operand
         op2 = strtoll(stack1[top1--], NULL, 10);
         op1 = strtoll(stack1[top1--], NULL, 10);
         if (strcmp(elm, "+") == 0) {
            sprintf(stack1[++top1], "%lli", op1 + op2);
         }
         else if (strcmp(elm, "*") == 0) {
            sprintf(stack1[++top1], "%lli", op1 * op2);
         }
         else if (strcmp(elm, "-") == 0) {
            sprintf(stack1[++top1], "%lli", op1 - op2);
         }
         else if (strcmp(elm, "&") == 0) {
            sprintf(stack1[++top1], "%lli", op1 & op2);
         }
         else if (strcmp(elm, "|") == 0) {
            sprintf(stack1[++top1], "%lli", op1 | op2);
         }
         else if (strcmp(elm, "xor") == 0) {
            sprintf(stack1[++top1], "%lli", op1 ^ op2);
         }
         else if (strcmp(elm, "ls") == 0) {
            sprintf(stack1[++top1], "%lli", op1 << op2);
         }
         else if (strcmp(elm, "rs") == 0) {
            sprintf(stack1[++top1], "%lli", op1 >> op2);
         }
         else if (strcmp(elm, "lr") == 0) {
            sprintf(stack1[++top1], "%lli", (op1 << op2) | (op1 >> (INT_BITS - op2)));
         }
         else if (strcmp(elm, "rr") == 0) {
            sprintf(stack1[++top1], "%lli", (op1 >> op2) | (op1 << (INT_BITS - op2)));
         }
         else {
            ; //blank statement
         }
      }
      else { //elm is a number, keep pushing to stack1 until seeing an operator in stack2
         strcpy(stack1[++top1],elm);
      }
      if(top2 < 0){ //no elm left, finished
         printf("%s\n", stack1[0]);
         break;
      }  
   } 
}

void add_item_to_dict (char var[], int val)  //case 1 - variable initialized | case 2 - variable reassigned to a different value
{
   int in_dict = 0;
   for (int i = 0; i < nof_dict_items; i++) {
      if (strcmp(dict_keys[i], var) == 0) { //variable already exists in keys, change the value
         in_dict = 1;
         dict_vals[i] = val;
         break;
      }
   }
   if (! in_dict) { //variable initialized for the first time
      strcpy(dict_keys[nof_dict_items], var);
      dict_vals[nof_dict_items] = val;
      nof_dict_items++; 
   }

   //print the values in dictionary
   /*
   for (int i = 0; i < nof_dict_items; i++) {
      printf("%s: %lli", dict_keys[i], dict_vals[i]);
   }
   */
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
   if (is_variable(tokens[cur])) { 
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
            sprintf(tokens[cur], "%lli", dict_vals[i]); // if in dict, use value instead
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
          return(0) ;
       }
       cur++ ; 
       strcpy(str,str1) ; 
       return(1) ; 

    }
    return(0) ; 
}

//Helper functions
int is_integer(char *token) {
    int isnumber = 1 ;
    char *q ; 

    for(q = token ; *q != '\0' ; q++) {
        isnumber = isnumber && isdigit(*q) ;
    }
    return(isnumber) ; 
}    

int is_variable(char *token) {
    int isvariable = 1 ;
    char *q ; 

    for(q = token ; *q != '\0' ; q++) {
        isvariable = isvariable && (isupper(*q) || islower(*q)) ;
    }
    return(isvariable) ; 
}    