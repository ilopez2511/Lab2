#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/* Return true (non-zero) if c is a whitespace characer
   ('\t' or ' ').
   Zero terminators are not printable (therefore false) */
bool delim_character(char c){
   if (c == '\t' || c == ' ' || c == '\n' || c == '(' || c == ')') {
      return true;
   }
   return false;
}

/* Return true (non-zero) if c is a non-whitespace
   character (not tab or space).
   Zero terminators are not printable (therefore false) */
bool non_delim_character(char c){
   if (c != '\t' && c != ' ' && c != '\0' && c != '\n' && c != '(' && c != ')') {
       return true;
   }
   return false;

}

/* Returns a pointer to the first character of the next
   space-separated word*/
char *word_start(char* str){
  char* ptr = str;
  while(*ptr != '\0'){
     if(delim_character(*ptr)) {
        ptr++;
     }
     else {
        return ptr;
     }
  }    
  return ptr;
}

/* Returns a pointer to the first space character of the zero
terminated string*/
char *end_word(char* str){
   char* s = str;
   while(*s != '\0') {
      if(non_delim_character(*s)) {
         s++;
      }
      else {
         return s;
      }   
   }
   return s;
}
// counts the number of words or tokens
int count_tokens(char* str){
   char* tmp = str;
   int count = 0;
   tmp = word_start(tmp);
   while(*tmp != '\0') {
      tmp = end_word(tmp);
      count++;
      tmp = word_start(tmp);
   }
   return count;
}
/* Returns a freshly allocated zero-terminated vector of freshly allocated
   space-separated tokens from zero-terminated str.
   For example, tokenize("hello world string") would result in:
     tokens[0] = "hello"
     tokens[1] = "world"
     tokens[2] = "string"
     tokens[3] = 0
*/
char *copy_str(char* inStr, short len){
   char* token = (char*) malloc(len*sizeof(char*));
   for(int i = 0; i < len; i++) {
      token[i] = inStr[i];
   }
   token[len] = '\0';
   return token;
}

char** tokenize(char* str){
   int size = count_tokens(str);
   char** tokens = (char**) malloc(size*sizeof(char**)+1);
   char* start = word_start(str);
   char* end;
   short length;
   for(int i = 0; i < size; i++) {
      end = end_word(start);
      length = end-start;
      tokens[i] = copy_str(start,length);
      start = word_start(end);
   }
   return tokens;
}

void print_all_tokens(char** tokens){
   char** tmp = tokens;
   int count = 0;
   while(*tmp != NULL) {
      printf("Token[%d]: %s\n",count,*tmp);
      count++;
      tmp++;
   }
}

// int main() {
//    char str[50];
//    printf("Please enter an input string: ");
//    fgets(str,50,stdin);
//    char** tokens = tokenize(str);
//    char* tok = tokens[0];
//    print_all_tokens(tokens);
// }
