Task Crunsher v1.0
=============

Task crunsher is a simple tool to use MPI to execute multiple command line tasks in parallel. It is based on the master worker scheme. To compile the program you need it install boost (www.boost.org) and boost process by Boris Schaeling (see for example http://www.highscore.de/boost/process/). 


Usage
=======
To start with task crunsher, you need to compile the project using the ./compile.sh script. 

The next step is to create a task_file in which each line stands for a separate task to be executed. 
This can also be more compilicated collections of task, i.e. 

mkdir test; cd test; yourcommandhere > output_of_your_command; cd .. .

Usually you can generate the task file for lots of experiments like this:

for seed in `seq 1 100`; do echo "./youprogram --random_seed=$seed"; done > task_file

An example task_file with simple tasks can be found in examples/task_file. 
It contains the following lines:

echo "a" > a1.txt
echo "b" > a2.txt
echo "c" > a3.txt
echo "d" > a4.txt

To run the tasks using the task_crunsher, for example using two mpi working processes, type the following into your command line:

mpirun -n 3 ./deploy/task_crunsher examples/task_file

The third MPI task is the scheduling processes that sends the tasks to the MPI processes.
This will then execute the task in parallel yielding the four files a*.txt.

The program is most useful if you have lots of experiments to run and want to collect the results afterwards by parsing the output of your program using awk, sed, etc...
