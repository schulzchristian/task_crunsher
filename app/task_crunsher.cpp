#include <stdio.h>
#include <iostream>
#include <string.h>
#include <argtable2.h>
#include <mpi.h>
#include "parse_parameters.h"
#include "../lib/task_crunsher/task_crunsher.h"

using namespace std;

int main(int argn, char **argv)
{
        MPI::Init(argn, argv);

        string task_filename;
        bool handle_output = false;
        int ret_code = parse_parameters(argn, argv, task_filename, handle_output );
        if( ret_code ) {
                MPI::Finalize();
                return 0;
        }

        task_crunsher tc;
        tc.crunsh_tasks(task_filename);

        MPI::Finalize();

        return 0;
}

