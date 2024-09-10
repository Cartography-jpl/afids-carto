/*******************************************************************************

  Title:    mparse
  Author:   Mike Burl 
  Date:     Dec 13, 1993
  Function: An improved version of the Fayyad command_parse funtion. Gets
              command line arguments. This version allows command flags with 
              no parameters and also flags with a variable number of parameters.
  MODIFIED: A significant change from command parse is that the arg_str field
              of the arg structure (defined in mparse.h) is an array of pointers
              to chars rather than simply a pointer to char. This change means
              mparse and command_parse are not directly interchangeable. See the
              program mparse_test.c to see what effect this change has on the
              calling program. Also, changed nargs field to nparams to make
              code more understandable.

  History:  2005/02/09 (MCB) - Changed mallocs to qmallocs and included qmalloc.h.

            2004/10/19 (MCB) - Change PARAM to MPARSE_PARAM. Incorporated
              changes below that happened after 20010601

            20030611 (MCB) - Added an implicit -help argument so that any program
              can specify a single command line flag -help and have the usage
              printed. Get_Args will return Get_Arg_ERROR in this case, but
              nothing especially bad has happened (this allows caller to terminate
              rather than proceeding as if it had gotten everything it needed
              from the command line).

            20010817 (MCB) - Modified mparse.h to change FALSE and TRUE
              to MPARSE_FALSE and MPARSE_TRUE. Also, eliminated #define of
              BOOLEAN.

            20010725 (MCB) - Added function prototypes and changed
              to modern style of declaring types of function arguments.

            19961226 (MCB) - Found a bug in get_arg_match. Basically, we were
              assuming first char of comstr was a minus sign and skipping over
              it without actually checking that it was a minus sign. As a result
              command line such as doit -list ilist was interpreting ilist as a
              command flag.


*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "carto/mparse.h"
#include "carto/qmalloc.h"
#include "carto/utils_return_values.h"

#define MPARSE_PARAM -1

/*******************************/
/* GLOBAL DECLARATIONS         */
/*******************************/
/* These functions don't get exposed outside of here, so 
   didn't put their prototypes in mparse.h */

int  get_arg_match(char *comstr, arg *args, int nargs);
void Print_Args(arg *args, int nargs);
void print_usage(char *com_name, arg *args, int nargs);

/**************************************/
/* get_arg_match                      */
/**************************************/
int get_arg_match(char *comstr, arg *args, int nargs)

{
  int  n, match = MPARSE_PARAM;
  /*  char infunc[] = "get_arg_match"; */
  
  /* BUG: 961226 MCB - Must check that first char of comstr is 
          indeed a minus sign before skipping it. */
  if (*comstr == '-') {
    comstr++;					  /* skip '-' sign  */
    for (n = 1; n <= nargs; n++) {
      if (strcmp(comstr,args[n].com_str) == 0) {
        match = n;
        args[n].exists = MPARSE_TRUE;
        break;
      }
    }
  }
  else {
    match = MPARSE_PARAM;
  }
  return(match);
}


/**************************************/
/* Print_Args                         */
/**************************************/
void Print_Args(arg *args, int nargs)

{
  int n, k;
  /*  char infunc[] = "Print_Args"; */
  
  printf("\nList of args:\n");
  for(n = 1; n <= nargs; n++) {
    if (args[n].exists) {
      if (args[n].nparams == 0) {
        printf("%d:  %s\n", n, args[n].com_str);
      }
      else {
        for (k = 1; k <= args[n].nparams; k++) {
          printf("%d:  %s %d '%s'\n", n, args[n].com_str, k, args[n].arg_str[k]);
        }
      }
    }
  }

  return;
}



/**************************************/
/* Get_Args                           */
/**************************************/
int Get_Args(int argc, char **argv, arg *args, int nargs)

{
  int  i, j, k, n, *flag, nparams;
  char infunc[] = "Get_Args";
  
  /* 20030611 (MCB): Before doing anything else, check for -help */
  if ((argc == 1) && (strcmp(argv[1], "-help") == 0)) {
    print_usage(argv[0], args, nargs);
    return(Get_Arg_ERROR);
  }

  /* Initialize */
  for (n = 1; n <= nargs; n++) {
    args[n].exists = MPARSE_FALSE;
  }
  
  /* Check whether each command line word is a flag or a parameter */
  flag = (int *) qmalloc((1+argc), sizeof(int), 0, infunc, "flag");
  for (i = 1; i <= argc; i++) {
    flag[i] = get_arg_match(argv[i], args, nargs);
  }

  i = 1;
  while (i <= argc) {
    if (flag[i] != MPARSE_PARAM) {
      /* We have a command line flag; determine number of associated parameters */
      nparams = 0;
      j = i+1;
      while ((j <= argc) && (flag[j] == MPARSE_PARAM)) {
        nparams++;
        j++;
      }

      /* For command flags having a specified number of params, check that */
      /*   we have found the right number */
      if ((args[flag[i]].nparams >= 0) && (nparams != args[flag[i]].nparams)) {
        fprintf(stderr, "ERROR (%s): %s improper number of params for command '-%s'.\n", 
            infunc, argv[0], args[flag[i]].com_str);
        return(Get_Arg_ERROR);
      } 

      /* For command flags with a variable number of params, set the */
      /*   nparams field to match the number we found. */
      if (args[flag[i]].nparams < 0) {
        args[flag[i]].nparams = nparams;
      }

      /* Put the params in arg_str array */
      if (nparams == 0) {
        /* If no params, don't need arg_str array */
        args[flag[i]].arg_str = NULL;
      }
      else {
        args[flag[i]].arg_str = (char **) qmalloc((nparams+1), sizeof(char *), 0, infunc, "args[flag[i]].arg_str");
        for (k = 1; k <= nparams; k++) {
          args[flag[i]].arg_str[k] = argv[i+k];
        }
      }

      i = j;    
    }
    else {
      printf("%s: -arg specifier expected, '%s' found.\n", argv[0], argv[i]);
      return(Get_Arg_ERROR);
    }
  }

  for (n = 1; n <= nargs; n++) {
    if (!args[n].optional && !args[n].exists) {
      fprintf(stderr, "ERROR (%s): %s '-%s' flag missing.\n", infunc, argv[0], args[n].com_str);
      fprintf(stderr, "ERROR (%s): %s insufficient arguments, aborting.\n", infunc, argv[0]);
      print_usage(argv[0], args, nargs);
      return(Get_Arg_ERROR);
    }
  }

  free((unsigned char *) flag);
  return(Get_Arg_OK);
}


/**************************************/
/* print_usage                        */
/**************************************/

void print_usage(char *com_name, arg *args, int nargs)

{
  int n;
  /*  char infunc[] = "print_usage"; */

  printf("\nUSAGE: %s\n", com_name);
  for(n = 1; n <= nargs; n++) {
    printf("     ");
    if (args[n].optional) {
      printf("[");
    }
    printf("-%s '%s'", args[n].com_str, args[n].doc_str);
    if (args[n].optional) {
      printf("]");
    }
    printf("\n");
  }
  printf("\n");
}


/* Eof. */
