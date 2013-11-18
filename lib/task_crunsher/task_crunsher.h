/******************************************************************************
 * task_crunsher         
 *
 * Source of  Task Crunsher
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


#ifndef TASK_CRUNSHER_M7FHIYK2
#define TASK_CRUNSHER_M7FHIYK2
#include <iostream>
#include <vector>

using namespace std;

class task_crunsher {
public:
        task_crunsher();
        virtual ~task_crunsher();
        void crunsh_tasks( string & task_filename );

private:
        int parse_taskfile( string & task_filename, vector<string> & tasks); 
        void send_work( string & task, int target, int counter);
        int recv_work( string & task);
        void run_task( string & task);

        int MASTER;
        int POISON;

        bool handle_output;
};


#endif /* end of include guard: TASK_CRUNSHER_M7FHIYK2 */
