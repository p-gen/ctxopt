**ctxopt**: yet another command line options manager.
#####################################################

|

For many uses, the traditional getopt function is enough to parse the
options in command lines.

However, cases exist where getopt shows its limits.
**ctxopt** is able to manage complex configurations of command line
options and excels when they appear in structured or independent blocs.

In **ctxopt**, each option has the possibility of starting a new context
in which the following command line options will be taken into account
and analyzed.
With this concept, it becomes easy, for example, to have repetitions
of identical options with each their independent sub-options.

Features:
---------

**ctxopt** has many features, its main ones are:

- Options are organized in a hierarchy of contexts.
- Options are easily declared using a syntax similar to BNF notation in
  each context.
- Any number of parameters can be assigned to each option.
- Parameters are not limited to just one character.
- The parameters associated with an option can be abbreviated as long as
  they do not conflict with another option in the same context.
- Parameters can be aggregated, even the long ones.
- Options evaluations trigger user-defined functions.
- Options can appear multiple times in one or more contexts.
- Options can be optional or mandatory.
- Option arguments can be optional or mandatory.
- Arguments can be multiple and their number controlled by simple rules.
- Arguments can have user-defined or built-in constraints.
- Options marked as mutually incompatibles are automatically detected.
- Error functions can be customized.
- Automatic detection of missing|incompatible|unknown|... options or
  parameters.
- Command lines options are evaluated in order but some options can be
  forced to be evaluated first though.

Context notion illustrated by a tiny example:
---------------------------------------------

Imagine a situation where you want an option to be allowed only if
another option was previously given.

For example, you want the *group* option to be allowed only after
a *user* option.

With **ctxopt** its easy, you just have to define two contexts (at least one
is mandatory), tell the *user* option to switch to the second context
(named ``ctx1`` here) and define the *group* option in the second context.

+------------------+-----------------+--------------+-------------------+
| Defined contexts | Allowed options | Next context | Option parameters |
+==================+=================+==============+===================+
| ``main``         | user            | ``ctx1``     | ``-u`` ``-user``  |
+------------------+-----------------+--------------+-------------------+
| ``ctx1``         | group           |              | ``-g`` ``-group`` |
+------------------+-----------------+--------------+-------------------+

According to the situation summarized in this table, the following
command line (the context changes in brackets have been added only for
understanding and not part of the command line)

.. parsed-literal::
  prog[main] -u[ctx1] u1 -g g1 g2 -user[ctx1] u2 -group g3

will be understood as:

.. parsed-literal::
  Context main:  prog -u u1          -user u2
  Context ctx1:             -g g1 g2          -group g3

In this example, you can see that the previous context (``main`` here) is
automatically re-entered after the analysis of the *group* option because
the *user* option is unknown in the ``ctx1`` context.

See the file **example1.c** in the **examples** directory for details.

Usage:
------

To use **ctxopt**, the users must at least:

- include **ctxopt.h** and **ctxopt.c** in his code.
- define at least one context and describe its options.
- set at least one parameter's name for the options described in the contexts.
- write and attach an action callback function to each options.
- possibly register constraint and other things (optional).
- call ``ctxopt_init``.
- call ``ctxopt_analyze``.
- call ``ctxopt_evaluate``.

Optional steps include:

- register entering and/or exiting function for contexts.
- register arguments constraint checking functions.
- redefine non internal error functions.

For more, please read the provided man page.

Enough theory, here is a basic Hello World example:
---------------------------------------------------

.. code:: c
  :number-lines:

  #include "ctxopt.h"
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>

  /* Callback functions */
  /* ****************** */

  void name_action(char * ctx_name, char * opt_name, char * param,
                   int nb_values, char ** values, int nb_opt_data,
                   void ** opt_data, int nb_ctx_data, void** ctx_data)
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

  int main(int argc, char * argv[])
  {
    int     nb_rem_args = 0;    /* Nb of remaining unprocessed arguments. */
    char ** rem_args    = NULL; /* Remaining arguments string array.      */

    ctxopt_init(argv[0]);
    ctxopt_new_ctx("main", "[name... #<string>...]");
    ctxopt_add_opt_settings(parameters, "name", "-n -name");
    ctxopt_add_opt_settings(actions, "name", name_action, NULL);
    ctxopt_analyze(argc - 1, argv + 1, &nb_rem_args, &rem_args);

    if (nb_rem_args > 0)
    {
      printf("Non-arguments are not allowed.\n");
      exit(EXIT_FAILURE);
    }

    ctxopt_evaluate();

    if (argc == 1)
      printf("Hello world.\n");

    exit(EXIT_SUCCESS);
  }

Code explanations:
..................

Line 1:

  This ``#include`` gives access to the API necessary to use **ctxopt**.

Line 9:

  This function is the callback function call each time a parameter
  associated with the option **name** is seen in the command line.

Line 32:

  The init function is mandatory and must be called first.

Line 33:

  Here the first (and unique here) context called **main** here is
  created with the description of an option called **name**.

  The **name** option is defined as an optional possible multiple option
  taking mandatory possibly multiple arguments.
  It is the ``#`` which indicates the presence of an argument,
  ``<string>`` is just a decaration to clarify the meaning of this
  argument.

Line 34:

  It's now time to introduce the two parameters of the option **name**.
  These are the parameters looked for in the command line.

Line 35:

  Here the callback function defined line 9 is associated with the option
  **name**.

Line 36:

  Here the command line is parsed and errors like unknown parameter, not
  enough arguments... are detected. All errors detected during this phase
  are fatal.

Line 38:

  The remaining non-arguments, if any, are managed here.

Line 44:

  All the internal representation of the command line built during the
  analysis phase (line 36) is finally evaluated and the callback
  registered functions (here **name_action**) called.

Line 46:

  The special case where the command line only contains the program name
  is treated here.

Examples of running session:
............................

.. parsed-literal::

  **$ ./hello -n Alice Bob -name Carol**
  Hello Alice, Bob.
  Hello Carol.

  **$ ./hello -n**
  -n requires argument(s).

  Synopsis:
  hello \
    [-n|-name... #<string>...]

  Syntactic explanations:
  Only the parameters (prefixed by -) and the arguments, if any, must be entered.
  The following is just there to explain the other symbols displayed.

  #tag         : argument tag giving a clue to its meaning.
  [...]        : the object between square brackets is optional.
  ...          : the previous object can be repeated more than one time.

  **$ ./hello**
  Hello world.
