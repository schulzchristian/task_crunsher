/******************************************************************************
 * parse_parameters      
 *
 * Source of Task Crunsher 
 *
 ******************************************************************************
 * Copyright (C) 2013 Christian Schulz <christian.schulz@kit.edu>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/


#ifndef PARSE_PARAMETERS_GPJMGSM9
#define PARSE_PARAMETERS_GPJMGSM9


using namespace std;

int parse_parameters(int argn, char **argv, string & task_filename, bool handle_output) {
        const char *progname = argv[0];

        // Setup argtable parameters.
        struct arg_lit *help                                 = arg_lit0(NULL, "help","Print help.");
        struct arg_lit *arg_handle_output                    = arg_lit0(NULL, "handle_output","automatically create output files instead of putting everything to the console.");
        struct arg_str *filename                             = arg_strn(NULL, NULL, "FILE", 1, 1, 
                                                                        "Taskfile (each line is a task executable on the commandline).");
        struct arg_end *end                                  = arg_end(100);

        // Define argtable.
        void* argtable[] = {
                help, filename, arg_handle_output, end
        };

        // Parse arguments.
        int nerrors = arg_parse(argn, argv, argtable);

        // Catch case that help was requested.
        if (help->count > 0) {
                printf("Usage: %s", progname);
                arg_print_syntax(stdout, argtable, "\n");
                arg_print_glossary(stdout, argtable,"  %-40s %s\n");
                printf("This is task crunshing program.\n");
                arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
                return 1;
        }


        if (nerrors > 0) {
                arg_print_errors(stderr, end, progname);
                printf("Try '%s --help' for more information.\n",progname);
                arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
                return 1; 
        }

        if(filename->count > 0) {
                task_filename = filename->sval[0];
        }

        if( arg_handle_output->count > 0 ) {
                handle_output = true;
        }
        return 0;
}

#endif /* end of include guard: PARSE_PARAMETERS_GPJMGSM8 */
