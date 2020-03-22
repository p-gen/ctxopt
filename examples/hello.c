#include "../ctxopt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Callback functions */
/* ****************** */

void
name_action(char * ctx_name, char * opt_name, char * param, int nb_values,
            char ** values, int nb_opt_data, void ** opt_data, int nb_ctx_data,
            void ** ctx_data)
{
  int v;

  printf("Hello %s", values[0]); /* First command line argument after name *
                                  | (-n or -name).                         */

  for (v = 1; v < nb_values; v++) /* Other command line arguments.         */
    printf(", %s", values[v]);

  printf(".\n");
}

/* Program entry */
/* ************* */

int
main(int argc, char * argv[])
{
  int     nb_rem_args = 0;    /* Nb of remaining unprocessed arguments. */
  char ** rem_args    = NULL; /* Remaining arguments string array.      */

  /* initialize cop */
  /* """""""""""""" */
  ctxopt_init(argv[0]);

  /* Create a new context with its  allowed options. */
  /* """"""""""""""""""""""""""""""""""""""""""""""" */
  ctxopt_new_ctx("main", "[name... #<string>...]");

  /* Attach parameters to the name option */
  /* """""""""""""""""""""""""""""""""""" */
  ctxopt_add_opt_settings(parameters, "name", "-n -name");

  /* Attach a callback action to the name option */
  /* """"""""""""""""""""""""""""""""""""""""""" */
  ctxopt_add_opt_settings(actions, "name", name_action, NULL);

  /* Parse and check the command line options */
  /* """""""""""""""""""""""""""""""""""""""" */
  ctxopt_analyze(argc - 1, argv + 1, &nb_rem_args, &rem_args);

  /* Manage the remaining non options */
  /* """""""""""""""""""""""""""""""" */
  if (nb_rem_args > 0)
  {
    printf("Non-arguments are not allowed.\n");
    exit(EXIT_FAILURE);
  }

  /* Execute the call back actions in sequence */
  /* """"""""""""""""""""""""""""""""""""""""" */
  ctxopt_evaluate();

  /* prints the default messages when there is no command line argument. */
  /* """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""" */
  if (argc == 1)
    printf("Hello world.\n");

  exit(EXIT_SUCCESS);
}
