import platform
import sys

# Get the current platform.
SYSTEM = platform.uname()[0]

Import('env')

# Build a library from the code in lib/.
libcrunsher_files = [ 'lib/task_crunsher/task_crunsher.cpp'
                     ]

if env['program'] == 'task_crunsher':
        if SYSTEM == 'Darwin':
                env['CXX'] = 'openmpicxx'
        else:
                env['CXX'] = 'mpicxx'

        env.Program('task_crunsher', ['app/task_crunsher.cpp']+libcrunsher_files, LIBS=['libargtable2','boost_system','boost_filesystem'] )
