======
ctxopt
======

-----------------------------------------
An advanced command line options manager.
-----------------------------------------

:Author: Pierre Gentile p.gen.progs@gmail.com
:Date: 2020
:Copyright: MPLv2.0
:Manual section: 3

DESCRIPTION
===========

**ctxopt** provides an advanced way to parse and evaluate complex command
line parameters and arguments.

Only two files **ctxopt.h** and **ctxopt.c** are needed to use **ctxopt**
in your projects.

CONCEPTS
--------

The command line can be considered as a tree of contexts.
This offers the possibility of having a hierarchy of option groups.

Each option has a set of parameters which must start with a single dash
and may appear in the command line.

In order not to change habits, the double dash notation popularized by
the GNU **getopt_long** function for long options is also accepted and
automatically and silently converted to notation with a single dash.

These parameters are not limited to a dash followed by single letter.
In what follows, I will consider for simplicity that these are the options
which are visible in the command line and not one of their parameters.

Unless explicitly stated, the options are processed in the same order
as they appear in the command line, from left to right.

Any option may have the ability to modify the current context.
If such an option is seen in the command line, then the next option will
be looked for in this new context.

Initially the default context is the first created one in the program.

Exiting a context can be explicit, see below, or implicit if the current
option is not supported in this context.
The previous context in the hierarchy then becomes the current context
if it is not already the highest one.

An option which is unknown in the current context will be automatically
reevaluated in the previous one in the hierarchy.
If there is no such context, then a fatal error occurs.

A context can be explicitly delimited by a pair of symbols ``{`` and
``}``.
The symbol ``{`` car appear just before or after the option responsible
of the context change.

The symbol ``}`` forces the end of a context.

The presence of ``{`` and ``}`` is purely optional and rarely necessary
but can help readability.

FUNCTIONS
---------

The API consists in the following functions:

* **void ctxopt_init(char *** \
  *prog_name*\
  **, char *** \
  *flags*\
  **);**

  This function initializes the internal structures uses by **ctxopt**.
  It is mandatory and must me called first.

  Its first argument (*prog_name*) is typically the content of ``argv[0]``
  which nearly always contains the name of the program.

  *prog_name* is used by the error system.

  Its second argument (*flags*) will be read as a set of parameters
  using the syntax ``flag=value``, each flag being separated from
  its neighbor by spaces.

  Flags may be missing in this argument, in this case the missing flags
  will be given their default value which is given below.
  An empty string is of course allowed but must be given anyway.

  For now, only three flags are understood: **stop_if_non_option**,
  **allow_abbreviations** and **display_usage_on_error**.

  Their value can be **yes** or **no**, **1** and **0** and also accepted.

  stop_if_non_option
    Instructs **ctxopt** to stop processing arguments as soon as it
    encounters a non-option word in the command line, even if other
    parameters remain to be processed. The default value of this flag
    is **0** or **no**.

  allow_abbreviations
    Tells **ctxopt** to try to guess a parameter name even if only its
    beginning is given. The default value of this flag is **1** or
    **yes**.

  display_usage_on_error
    If the setting is set to yes (default), the usage text for the
    relevant contexts is displayed in case of a fatal error.
    This may be useful to reduce the length of the error message and
    allow the user to see the error more easily.

  Example of content of the *flags* parameter:

  .. code-block:: c

    "stop_if_non_option=1 allow_abbreviations=No"

|

* **void ctxopt_new_ctx(char *** \
  *name*\
  **, char *** \
  *opts_specs*\
  **);**

  The next thing to do after having called **ctxopt_new_ctx** is to
  define the contexts and its associated options.

  Using this function, you can name the new context and its authorized
  options and determine their main characteristics with a syntax easy
  to understand.
  If they already exist in another context, then they must have the same
  signature (described below) as in their previous appearance.

  *name* is the name of the new context and *opts_specs* must contain a
  description of the options allowed in this context.

  *opts_specs* is a string containing various components separated by
  spaces according to the syntax below:

  opt
    A mandatory option named opt without parameter usable only one type in
    the context

  opt1 opt2
      Two mandatory option named **opt1** and **opt2** without argument.

  opt...
      Same as above but can appear multiple time in the context.

  [opt...]
      Same as above but the option is optional

  opt1 [opt2...]
      One mandatory and one optional option named **opt1** and **opt2**
      without argument. **opt2** can appear multiple times in the context.

  opt #
      One mandatory option named **opt*** taking one mandatory argument.

  opt #tag
      One mandatory option named **opt** taking one mandatory argument.
      **tag** is ignored but can be used to improve the readability.

  opt [#]
      One mandatory option named **opt** taking one optional argument.

  opt #...
      One mandatory option named **opt** taking one or more mandatory
      arguments.

  opt>ctx... [#<value>]
      The mandatory multiple option named **opt** will switch to the
      context named **cxt** which will become the new current context.

      It takes an optional argument with a tag named **<value>**.

  [opt... [#...]]
      One optional option named **opt** taking multiple optional
      arguments.

  The number of options/arguments allowed can be restricted by adding
  an operator and an integer just after the dots like in the following
  example:

    opt...<3 #...=3
        Here, the mandatory option **opt** is restricted to only appear
        one or two times in the context.
        The number of its mandatory arguments must be exactly three.

        Valid operators are **<**, **=** and **>**.

  The multiplicity or not of the options and argument, their mandatory or
  optional characteristics constitutes their signatures.s

  As said above, an option can appear in more than one context but must
  have the same signature.

  Example:

  .. code-block:: c

    ctxopt_new_ctx("context1",
                   "[opt1>context2...] #arg1... [opt3]");

    ctxopt_new_ctx("context2",
                   "[opt2 [#arg2]] [opt3]");

  In the previous example, three options **opt1**, **opt2** and **opt3**
  are defined.

    :opt1:
      is mandatory and can appear more than one time and take multiple
      mandatory arguments.

    :opt2:
      is optional and take an optional argument.

    :opt3:
      is optional and take no argument.
      Note that **opt3** is legal in both contexts.

    |

    **opt2**, if present in the command line, will be evaluated in the
    context **context2**.
    Note that, in this example, the **context2** can only be entered if
    **opt1** has previously been seen in the command line.
    Hence, **opt2** is only legal if **opt1** is present first.

    **opt3** does not have this limitation.
    In fact, as **opt3** is optional in **context2** and if its action
    function is not interested in the name of the current context,
    then it could have been omitted from the second setting thanks to
    the backtracking: an option which is illegal in a context is retried
    in the previous context in the hierarchy.

|

* **void ctxopt_ctx_disp_usage(char *** \
  *ctx_name*\
  **, usage_behaviour** \
  *action*\
  **);**

  This function builds and prints an usage help text for the
  specific context *ctx_name*.
  The symbols used in this text are the same as those used when defining
  options in **ctxopt_new_ctx**.

  The parameter *action* can take the following values:

  continue_after
    The program is not stopped when this function returns.

  exit_after
    The program is stopped with a non zero return code (typically 1)
    when this function returns.

  The usage text is followed by a legend explaining the symbols meanings.
  This function is useful when associated with a **help** or **usage**
  option.

|

* **void ctxopt_disp_usage(usage_behaviour** \
  *action*\
  **);**

  This function is similar to the preceding one but displays the usage
  help text for all the defined contexts.
  It is useful when associated with a general **help** or **usage**
  option.

  The parameter *action* can take the following values:

  continue_after
    The program is not stopped when this function returns.

  exit_after
    The program is stopped with a non zero return code (typically 1)
    when this function returns.

|

* **void ctxopt_add_global_settings(settings** \
  *s*\
  **,** \
  *...*\
  **);**

  This function allows to set general **ctxopt** settings.
  As for now, the only possible setting for *s* is **error_functions**.

  This setting tells **ctxopt_add_global_settings** to use the rest of
  its arguments in order to replace the built-in error functions with
  custom ones.

  When the value of the first parameter is **error_functions**,
  then the second one must be one of the following constants:

  :CTXOPTMISPAR:
    A mandatory parameter is missing.

  :CTXOPTUNKPAR:
    A given parameter is unknown in the current context.

  :CTXOPTDUPOPT:
     An option has been seen more than once but has not been declared as
     multiple in the context.

  :CTXOPTINCOPT:
    An option is incompatible with an option already given in the context.

  :CTXOPTMISARG:
    A mandatory argument is missing.

  :CTXOPTCNTEOPT, CTXOPTCNTLOPT and CTXOPTCNTGOPT:
    The number of occurrences is not equal, lower or greater than a
    given value.

  :CTXOPTCNTEARG, CTXOPTCNTLARG and CTXOPTCNTGARG:
    The number of arguments of an option is not equal, lower or greater
    than a given value.

  and the third parameter is a function pointer with the following
  prototype:

  .. code-block:: c

    void (*) (errors err, state_t * state);

  *state* will point to the publicly available analysis state structure.
  This structure contains a snapshot of variables related to the command
  line analysis so far.
  They and can be used to give the user clues about errors.

  This structure available in **ctxopt.h** is:

  .. code-block:: c

    typedef struct
    {
      char * prog_name;        /* base name of the program name.         */
      char * ctx_name;         /* current context name.                  */
      char * ctx_par_name;     /* parameter which led to this context.   */
      char * opt_name;         /* current option name.                   */
      char * opt_params;       /* all parameters of the current option.  */
      int    opts_count;       /* limit of the number of occurrences of  *
                               |  the current option.                    */
      int opt_args_count;      /* limit of the number of parameters of   *
                               |  the current option.                    */
      char * pre_opt_par_name; /* parameter just before the current one. */
      char * cur_opt_par_name; /* current parameter.                     */
    } state_t;

  All these pointers can be equal to the **NULL** pointer.

  Example:

  .. code-block:: c

    ctxopt_add_global_settings(error_functions, CTXOPTMISPAR, error);

|

* **void ctxopt_add_ctx_settings(settings** \
  *s*\
  **,** \
  *...*\
  **);**

  This function manages some settings for a given context.
  Its first parameter *s* determines the setting and the signification
  of the remaining arguments.

  Its possible values are:

  incompatibilities:
    This setting allows to declare a set of options incompatible with
    each other.

    In this case the second argument must be a context name and the
    third argument must be a string containing option names separated
    by a space.

    Example of **incompatibilities** setting:

    .. code-block:: c

      void ctxopt_add_ctx_settings(incompatibilities,
                                   "context1",
                                   "opt1 opt2 opt3");

    The three options named **opt1**, **opt2** and **opt3** will be
    marked as mutually incompatibles in each instance of the context
    **context1**.

  requirements:
    This setting allows options in a context to require the presence of
    sets of other options of which at least one must be present.
    Using this setting, the user can impose dependencies between options.

    The option that imposes the requirement must be the first in the
    list of options listed in the third arguments.

    Example of **requirements** setting:

    .. code-block:: c

      void ctxopt_add_ctx_settings(requirements;
                                   "context1",
                                   "opt1 opt2 opt3");

    At least one of the two options named **opt2** and **opt3** must
    be present in the same context instance as **opt1** which is
    **context1** in this case

    There may be multiple requirements via multiple calls to
    **ctxopt_add_ctx_settings** for the same first option (**opt1**
    in the previous example) and the same context.
    Each of them is considered in order.

  actions:
    This setting allows to associate a function to the context.

    The second argument (called *f* below) will be called as soon as the
    context is entered or exited during the evaluation phase.

    Note that *f* will NOT be called if the context is empty
    (does not contain any option).

    The next parameters must be pointers to arbitrary data which may
    be used by *f*.

    In this setting, the last parameter must be **NULL**.

    *f* must have the following prototype:

    .. code-block:: c

      int (*) (char     * name1,   /* Context name */
               direction  status,  /* entering or exiting */
               char     * name2,   /* previous or next context */
               int        nb_data, /* Number of data */
               void    ** data     /* Data */);

    This function *f* will be called when entering **AND** exiting
    the context.
    Its arguments will then be set to:

    *name1*
      the name of the context.

    *status*
      will be **entering** when entering the context and **exiting**
      when exiting the context.

    *name2*
      according to the content of *status*, the name of the context we
      are coming from or the name of the context we are returning to.

      *name2* can be **NULL** if we are entering in the main context or
      are leaving it.

    *nb_data*
      The number of data pointers passed to the **ctxopt_add_ctx_settings**
      function after the *s* parameter.

    *data*
      The data pointers passed to the **ctxopt_add_ctx_settings** function
      after the *s* parameter and arranged in an array of *nb_data*

    Example of **actions** setting:

    .. code-block:: c

      void ctxopt_add_ctx_settings(actions,
                                   "context1",
                                   action,
                                   &data_1, &data_2, &data_3,
                                   NULL);

    This function call registers the **action** function to the context
    named **context1**.

    The action function will be called **after** entering to and
    **before** exiting from each instance of the context
    named **context1**.

    The optional *data_X* pointers will be passed to **action** through
    its data pointer to allow it to manipulate them if needed.
    The count of these pointers (3 here) will also be passed to action
    through its *nb_data* parameter.

    The ending **NULL** is mandatory.

|

* **void ctxopt_add_opt_settings(settings** \
  *s*\
  **, char *** \
  *opt*\
  **,** \
  *...*\
  **);**

  This function manages some settings for an option whose name is given in
  *opt*.

  The first parameter *s* determines the exact setting and the
  signification of the remaining arguments.
  Its possible values are:

  parameters
    This setting allows to associate command line parameters with *opt*.
    The set of parameters must be given in the third argument as a string
    containing words separated by blanks.

    Each appearance of one of these parameters in the command line will
    trigger the action associated with the named option.

    Each of these words must start with one and exactly one dash.

    Example of **parameters** setting:

    .. code-block:: c

      ctxopt_add_opt_settings(parameters,
                              "opt1",
                              "-p -parm -p1");

    In this example, **opt1** is the name of a previously defined option and
    **-p**, **-parm** and **-p1** will be three valid command line
    parameters for the option **opt1**.

  actions
    This setting allows to associate a function to this options.
    As said above, this function will be called each time the option will be
    recognized when evaluating the command line.

    The function pointer must be given as the third argument.

    Following the function pointer, it is possible to add a bunch of
    other parameters which must be pointers to some pre-allocated arbitrary
    data.

    These pointers will be passed to the function when called.
    The last parameter must be **NULL** to end the sequence.

    The function needs to be given as the third argument and must
    match the following prototype:

    .. code-block:: c

      void (*) (char  * ctx_name,     /* Context name */
                char  * opt_name,     /* Option name  */
                char  * param,        /* Parameter name */
                int     nb_values,    /* Number of arguments */
                char ** values,       /* Arguments */
                int     nb_opt_data,  /* Number of option data passed */
                void ** opt_data,     /* Array of option data passed */
                int     nb_ctx_data,  /* Number of context data passed */
                void ** ctx_data      /* Array of context data passed */)

    *ctx_name*
      is the name of the current context.

    *opt_name*
      is the name of the option.

    *param*
      is the name of the parameter that triggered the option *opt_name*.

    *nb_values*
      is the number of arguments immediately following this option in
      the command line.

    *values*
      is an array of stings containing the arguments following this
      option in the command line.

    *nb_opt_data*
      is the number of data pointers which were given after the third
      arguments of **ctxopt_add_opt_settings**.

    *opt_data*
      The data pointers passed after the third arguments of
      **ctxopt_add_opt_settings** and reorganized as an array of
      *nb_opt_data* elements.

      The aim is to be able to consult/alter options specific data.

    *nb_ctx_data*
       Same as *nb_opt_data* but referencing to the number of data
       pointers given to **ctxopt_add_ctx_settings** for the current
       context after its third argument.

    *ctx_data*
      are the data pointers given to **ctxopt_add_ctx_settings** for the
      current context after its third argument.

      The aim is to be able to consult/alter contexts specific data.

    Example of **actions** setting:

    .. code-block:: c

      void action(char * ctx_name,
                  char * opt_name,
                  char * param,
                  int    nb_values,   char ** values,
                  int    nb_opt_data, void ** opt_data,
                  int    nb_ctx_data, void ** ctx_data)
      {
        ...
      }

      ...

      void ctxopt_add_opt_settings(actions, "opt1", action,
                                   &data_1, &data_2, &data_3,
                                   NULL);

    This example associates the function *action* to the option **opt1**.

    Here, the *data_** pointers will be accessible to the function
    *action* when called through its argument *opt_data* and their number
    (3 here) through its argument *nb_opt_data* as mentioned above.

    *action* will also have access to the current context data in the
    same way through its arguments *ctx_data* and *nb_ctx_data*.

    The *action* argument *param* will receive the value of the specific
    parameter which triggered it - one of the parameters registered with
    **ctxopt_add_opt_settings**.

  constraints
    This setting registers a function whose responsibility is to validate
    that the arguments of the option respect some constraints.

    To do that the third argument must be a function pointer and the fourth
    argument must be some arbitrary parameter to this function needed
    to validate the constraint.

    The constraint function must match the following prototype:

    .. code-block:: c

       int (*) (int nb_args, char ** args, char * value, char * parameter);

    Where:

      *nb_args*
        is the number which will be set to the number of arguments fol-
        lowing the command line parameter.

      *args*
        is an array of nb_args strings containing theses arguments.

      *value*
        is an arbitrary string containing the constraints which must be
        respected by args.

      *parameter*
        is the parameter of which *value* is an argument.

    Three constraint functions are built-in and are described below.
    They give examples on how to build them.

    Example of constraint function using the built-it regular expression
    constraint checker function:

    .. code-block:: c

      ctxopt_add_opt_settings(constraints,
                              "opt1",
                              ctxopt_re_constraint,
                              "[^:]+:.+");


    In this example all the arguments of the option **opt1** must match
    the extended regular expression::

      [^:]+:.+

    See below for details about the function **ctxopt_re_constraint**.

  before or after
    These settings allow to tell ctxopt than some options must be
    evaluated **before** or **after** a given option in a context.
    This can be useful, for example, if an action triggered by the
    evaluation of a option is required to be executed before the action
    of another option.

    Example of **before** setting:

    .. code-block:: c

      ctxopt_add_opt_settings(before,
                              "opt1",
                              "opt2 opt3");

    In this example, **opt2** and **opt3** will be evaluated *before*
    **opt1**.
    The relative order of **opt2** and **opt3** evaluations will still
    follow their order of appearance in the command line.

    Example of **after** setting:

    .. code-block:: c

      ctxopt_add_opt_settings(after,
                              "opt2",
                              "opt3 opt4");

    In this example, **opt3** and **opt4** will be evaluated *after*
    **opt2**.
    This example shows than we can combine multiple settings reusing
    options previously mentioned.

    Incompatible setting combinations are not checked and will be ignored
    or lead to undefined behaviors.

  visible_in_help
    These settings allow you to indicate that certain options should
    not be visible in the help messages. A use case could be to keep
    certain options hidden.

    A third parameter whose value (case insensitive) is **yes** prevents
    the option from being visible and a value set to **no** (the default)
    forces if to be visible in the auto-generated help messages.

|

* **int ctxopt_format_constraint(int** \
  *nb_args*\
  **, char **** \
  *args*\
  **, char *** \
  *value*\
  **, char *** \
  *parameter*\
  **);**

  This pre-defined constraint function checks whether the arguments
  in *args* respect a C printf format given in value, `%2d` by e.g.
  It returns 1 if the checking is successful and 0 if not.

|

* **int ctxopt_re_constraint(int** \
  *nb_args*\
  **, char **** \
  *args*\
  **, char *** \
  *value*\
  **, char *** \
  *parameter*\
  **);**

  Another pre-defined constraint function which checks if the arguments
  of an option respects the extended regular expression given in *value*.

  It returns 1 if the arguments respects the constraint and 0 if this
  is not the case.

|

* **int ctxopt_range_constraint(int** \
  *nb_args*\
  **, char **** \
  *args*\
  **, char *** \
  *value*\
  **, char *** \
  *parameter*\
  **);**

  Yet another pre-defined constraint function. This one checks if the
  arguments of an option are in in a specified ranges.

  *value* must contain a string made of a maximum of 2 long integers
  separated by spaces.

  The first or the second of these numbers can be replaced with the
  character '`.`'. In this case only the minimum or maximum is checked
  and the '`.`' equals to plus or minus infinity depending of this
  place in the string.

  It returns 1 if the arguments respects the constraint and 0 if this
  is not the case.

|

* **void ctxopt_analyze(int** \
  *nb_words*\
  **, char **** \
  *words*\
  **, int *** \
  *rem_count*\
  **, char ***** \
  *rem_args*\
  **);**

  This function processes the registered contexts instances tree, detects
  errors and possibly reorganizes the options order according
  to given priorities.

  The first two arguments are similar to the *argc* and *argv* arguments
  of the main function but without counting `argv[0]`.
  Therefore, in many cases, *nb_words* will have the value of `argc-1`
  and *words* will have the value of `argv+1`.

  The last two will receive the number of remaining (non analyzed)
  command line words and the array of these remaining words.
  Remaining words can be words appearing after ``--`` per example.

  All errors are fatal and terminates the program with a return code
  greater then 0.

  Example:

  .. code-block:: c

    int     res_argc;
    char ** res_argv;
    ...
    ctxopt_analyze(argc-1, argv+1, &res_argc, &res_argv);

|

* **void ctxopt_evaluate(void);**

  This function walks through the tree of context instances previously
  built by **ctxopt_analyze** and launches the action attached to
  each options, if any, one after the other.

* **ctxopt_free_memory(void)**

  This function frees the memory used internally by **ctxopt**.

ENVIRONMENT
===========

**ctxopt** is able to switch to debug mode if the variable CTXOPT_DEBUG
is set to any not-empty value.

If this is the case, informational messages about how **ctxopt**
analyses the command line are printed on the error output.

Each of them are prefixed with "CTXOPT_DEBUG: ".
