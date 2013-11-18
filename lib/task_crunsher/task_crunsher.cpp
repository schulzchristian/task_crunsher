/******************************************************************************
 * task_crunsher 
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

#include <fstream>
#include <sstream>
#define BOOST_FILESYSTEM_VERSION 2 
#include <boost/process.hpp> 
#include <mpi.h>
#include "task_crunsher.h"

namespace bp = ::boost::process;

task_crunsher::task_crunsher() : MASTER(0), POISON(1), handle_output(false) {

}

task_crunsher::~task_crunsher() {

}

void task_crunsher::crunsh_tasks( string & task_filename ) {
        //now the master worker scheme
        int rank = MPI::COMM_WORLD.Get_rank();
        int size = MPI::COMM_WORLD.Get_size();

        if(size == 1) {
                // special case: we are alone
                vector<string> tasks;
                int ret_code = parse_taskfile( task_filename, tasks );
                if(ret_code){
                        return;
                }
                for( unsigned i = 0; i < tasks.size(); i++) {
                        stringstream command;
                        if( handle_output ) {
                                command << tasks[i] <<  " > " <<  i <<  ".out" <<  " 2>" <<  i <<  ".err" <<  endl;
                        } else {
                                command << tasks[i] <<  endl;
                        }
                        string commando = command.str();
                        run_task(commando);
                }
                return;
        }

        int counter = 0;
        if (rank == MASTER) {
                vector<string> tasks;
                int ret_code = parse_taskfile( task_filename, tasks );
                if(ret_code){
                        return;
                }
                //task are build up
                //first of all each slave gets a package of work 
                for( int target = 1; target < size; target++) {
                        send_work( tasks[counter], target, counter);
                        counter++;
                        if( counter == (int)tasks.size() ) 
                                break;
                }

                // master code
                while( counter != (int) tasks.size() ) {
                        bool work = false;
                        MPI::Status st;
                        MPI::COMM_WORLD.Recv(&work, 1, MPI::BOOL, MPI::ANY_SOURCE, MPI::ANY_TAG, st);

                        send_work(tasks[counter], st.Get_source(), counter);
                        counter++;
                }

                // send poison 
                for (int target = 1; target < size; ++target) {
                        MPI::COMM_WORLD.Send(0, 0, MPI::INTEGER, target, POISON);
                }


        } else {
                // slave code
                while( true ) {
                        string task;
                        int poison = recv_work(task);

                        if(poison) 
                                return;

                        run_task(task);

                        bool givemework = true;
                        MPI::COMM_WORLD.Send(&givemework, 1, MPI::BOOL, MASTER, 0);
                }

        }

}

void task_crunsher::send_work( string & task, int target, int counter) {
        stringstream command;
        if( handle_output ) {
                command << task <<  " > " <<  counter <<  ".out" <<  " 2>" <<  counter <<  ".err" <<  endl;
        } else {
                command << task <<  endl;
        }

        string commando = command.str();
        int string_size = commando.length()+1;

        MPI::COMM_WORLD.Send( &string_size, 1, MPI::INTEGER, target, 0);
        MPI::COMM_WORLD.Send( commando.c_str(), string_size, MPI::CHAR, target, 0);
}

void task_crunsher::run_task(string & task) {
        bp::context ctx; 
        ctx.stdout_behavior = bp::capture_stream(); 
        ctx.environment     = bp::self::get_environment();
        bp::child c         = bp::launch_shell(task, ctx); 
        bp::posix_status s  = c.wait();
}

int task_crunsher::recv_work( string & task ) {
        int string_size;
        MPI::Status st;
        MPI::COMM_WORLD.Recv( &string_size, 1, MPI::INTEGER, MASTER, MPI::ANY_TAG, st);

        if( st.Get_tag() == POISON ) {
                return POISON;
        }

        char * string_to_recv = new char[string_size];
        MPI::COMM_WORLD.Recv( string_to_recv , string_size, MPI::CHAR, MASTER, MPI::ANY_TAG);

        string buffer(string_to_recv);
        task = string_to_recv;

        return 0;
}

int task_crunsher::parse_taskfile( string & task_filename, vector<string> & tasks) {
        ifstream file;
        file.open(task_filename.c_str());
        if (!file){
                cout << "Error in openening file " <<  task_filename;
                return 1;
        }

        string line;
        while(std::getline(file, line)) {
                tasks.push_back(line);
        }

        file.close();

        return 0;

}
