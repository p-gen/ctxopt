#include "../ctxopt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Callback functions */
/* ****************** */

void
user_action(char * ctx_name, char * opt_name, char * param, int nb_values,
            char ** values, int nb_opt_data, void ** opt_data, int nb_ctx_data,
            void ** ctx_data)
{
  printf("User %s\n", values[0]);
}

void
groups_action(char * ctx_name, char * opt_name, char * param, int nb_values,
              char ** values, int nb_opt_data, void ** opt_data,
              int nb_ctx_data, void ** ctx_data)
{
  int v;

  printf("Groups %s", values[0]);

  for (v = 1; v < nb_values; v++)
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

  /* initialize ctxopt */
  /* """"""""""""""""" */
  ctxopt_init(argv[0]);

  /* Create new contexts with their allowed options */
  /* """""""""""""""""""""""""""""""""""""""""""""" */
  ctxopt_new_ctx("main", "[user>ctx1... #<string>]");
  ctxopt_new_ctx("ctx1", "group #<string>...");

  /* Attach parameters to options */
  /* """""""""""""""""""""""""""" */
  ctxopt_add_opt_settings(parameters, "user", "-u -user");
  ctxopt_add_opt_settings(parameters, "group", "-g -groups");

  /* Attach a callback action to options */
  /* """"""""""""""""""""""""""""""""""" */
  ctxopt_add_opt_settings(actions, "user", user_action, NULL);
  ctxopt_add_opt_settings(actions, "group", groups_action, NULL);

  /* Parse and check the command line options */
  /* """""""""""""""""""""""""""""""""""""""" */
  ctxopt_analyze(argc - 1, argv + 1, &nb_rem_args, &rem_args);

  /* Execute registered actions in sequence */
  /* """""""""""""""""""""""""""""""""""""" */
  ctxopt_evaluate();

  exit(EXIT_SUCCESS);
}
