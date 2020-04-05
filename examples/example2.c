#include "../ctxopt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

/* Utility functions */
/* ***************** */

static void
hexdump(void * mem, int len)
{
  uint8_t * data = (uint8_t *)mem;
  int       pos  = 0;
  while (pos < len)
  {
    /* Print address */
    /* """"""""""""" */
    printf("%08x", pos);

    /* Print hex bytes */
    /* """"""""""""""" */
    for (int i = 0; i < 16; i++)
    {
      if ((i & 7) == 0)
        printf(" ");
      if (pos + i < len)
        printf(" %02x", data[pos + i]);
      else
        printf("   ");
    }

    /* Print ASCII bytes */
    /* """"""""""""""""" */
    printf("  |");
    for (int i = 0; i < 16; i++)
    {
      /* Abort if at end */
      if (pos + i >= len)
        break;

      /* Print if printable */
      if (isprint((int)data[pos + i]))
        printf("%c", data[pos + i]);
      else
        printf(".");
    }
    printf("|\n");
    pos += 16;
  }
}

/* Callback functions */
/* ****************** */

int
second_action(char * ctx_name, direction status, char * prev_ctx_name,
              int nb_data, void ** data)

{
  puts("Context action");
  puts("==============");
  if (ctx_name != NULL)
    printf("ctx_name: %s\n", ctx_name);

  printf("status:   ");
  if (status == entering)
  {
    puts("entering");
    (*(int **)data)[1]++;
  }
  else
  {
    puts("exiting");
    (*(int **)data)[1]++;
  }

  if (nb_data > 0)
    hexdump(*(int **)data, 3 * sizeof(int));

  puts("");
}

void
opt_action(char * ctx_name, char * opt_name, char * param, int nb_values,
           char ** values, int nb_opt_data, void ** opt_data, int nb_ctx_data,
           void ** ctx_data)
{
  int v;

  puts("Option action");
  puts("=============");
  if (ctx_name != NULL)
    printf("ctx_name:      %s\n", ctx_name);

  if (opt_name != NULL)
    printf("opt_name:      %s\n", opt_name);

  if (param != NULL)
    printf("param:         %s\n", param);

  /* Display arguments */
  /* """"""""""""""""" */
  printf("Values   (%2d): ", nb_values);
  for (v = 0; v < nb_values; v++)
    printf("%s ", values[v]);
  puts("");

  /* Decrement the second member of the first context argument */
  /* """"""""""""""""""""""""""""""""""""""""""""""""""""""""""*/
  if (nb_ctx_data > 0)
    (*(int **)ctx_data)[1]--;

  /* Display option data */
  /* """"""""""""""""""" */
  if (nb_opt_data > 0)
  {
    printf("opt_data (%2d): ", nb_opt_data);
    hexdump(*(char **)opt_data, strlen(*(char **)opt_data) + 1);
    puts("");
  }

  /* Display context data */
  /* """""""""""""""""""" */
  if (nb_ctx_data > 0)
  {
    printf("ctx_data (%2d):\n", nb_ctx_data);
    hexdump(*(int **)ctx_data, 3 * sizeof(int));
    puts("");
  }
}

/* Program entry */
/* ************* */

int
main(int argc, char * argv[])
{
  int     nb_rem_args = 0;    /* Nb of remaining unprocessed arguments. */
  char ** rem_args    = NULL; /* Remaining arguments string array.      */

  int  ctx_data[] = { 1, 2, 3 };
  char opt_data[] = "test";

  /* initialize cop */
  /* """""""""""""" */
  ctxopt_init(argv[0]);

  /* Create new contexts with their allowed options */
  /* """""""""""""""""""""""""""""""""""""""""""""" */
  ctxopt_new_ctx("first", "[a>second... #<string>...]");
  ctxopt_new_ctx("second", "[b>third [#<string>]]");
  ctxopt_new_ctx("third", "[c #<string>]");

  /* Attach parameters to the options */
  /* """""""""""""""""""""""""""""""" */
  ctxopt_add_opt_settings(parameters, "a", "-a");
  ctxopt_add_opt_settings(parameters, "b", "-b");
  ctxopt_add_opt_settings(parameters, "c", "-c");

  /* Attach a callback action to the second context */
  /* """""""""""""""""""""""""""""""""""""""""""""" */
  ctxopt_add_ctx_settings(actions, "second", second_action, &ctx_data, NULL);

  /* Attach a callback action to the options */
  /* """"""""""""""""""""""""""""""""""""""" */
  ctxopt_add_opt_settings(actions, "a", opt_action, NULL);
  ctxopt_add_opt_settings(actions, "b", opt_action, &opt_data, NULL);
  ctxopt_add_opt_settings(actions, "c", opt_action, NULL);

  /* Parse and check the command line options */
  /* """""""""""""""""""""""""""""""""""""""" */
  ctxopt_analyze(argc - 1, argv + 1, &nb_rem_args, &rem_args);

  /* Execute the callback actions in sequence */
  /* """""""""""""""""""""""""""""""""""""""" */
  ctxopt_evaluate();

  /* Print remaining non-arguments if any */
  /* """""""""""""""""""""""""""""""""""" */
  if (nb_rem_args > 0)
  {
    int a;

    puts("Remaining unparsed command line words");
    puts("=====================================");
    for (a = 0; a < nb_rem_args; a++)
      printf("%s ", rem_args[a]);
    puts("");
  }

  exit(EXIT_SUCCESS);
}
