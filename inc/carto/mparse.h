
#ifndef _MPARSE_H
#define _MPARSE_H

#define MPARSE_FALSE 0
#define Get_Arg_ERROR -1
#define Get_Arg_OK 0
#define MPARSE_TRUE 1



typedef struct
{
  char      *com_str;
  int       optional;
  int       nparams;
  int       exists;
  char      **arg_str;  /* NOTE: This is an important change from command_parse.h */
                        /*   arg_str is an array of ptrs rather than a single ptr */
                        /*   Each array element is a pointer to char, which points */
                        /*   to a param associated with the command line flag. */
                        /*   Indices of this array go from 1 to nparams. */
  char      *doc_str;
} arg, *arg_ptr;


int Get_Args(int argc, char **argv, arg *args, int nargs);
void print_usage(char *com_name, arg *args, int nargs);

#define  Init_Arg(args,i,name,optflag,na,doc)    \
  args[i].com_str = name; \
  args[i].optional = optflag; \
  args[i].nparams = na; \
  args[i].doc_str = doc;


#define  Set_Arg(args,i,var)  if (args[i].exists) var = args[i].arg_str 
  

#endif /* _MPARSE_H */


/* Eof. */
