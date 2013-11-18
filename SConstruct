# scons build file for the sequential graph coarsening.
#
# You can build it in the following variants:
#
#   debug         debug symbols, assertions, no optimization.
#   optimized     no debug symbols, no assertions, optimization.
#   poptimized    debug symbols, no assertions, optimization.
#   doptimized    debug symbols, assertions, optimization.
#
#   scons variant=${variant}

import os
import platform
import sys

# Get the current platform.
SYSTEM = platform.uname()[0]
HOST = platform.uname()[1]

# Get shortcut to $HOME.
HOME = os.environ['HOME']

def GetEnvironment():
  """Get environment variables from command line and environment.

  Exits on errors.

  Returns
    Environment with the configuration from the command line.
  """
  opts = Variables()
  opts.Add('variant', 'the variant to build, debug or optimized', 'debug')
  opts.Add('program', 'program or interface to compile', 'task_crunsher')

  env = Environment(options=opts, ENV=os.environ)
  if not env['variant'] in ['debug', 'optimized', 'pdebug', 'profilingoptimized']:
    print 'Illegal value for variant: %s' % env['variant']
    sys.exit(1)
  
  if not env['program'] in ['task_crunsher']:
    print 'Illegal value for program: %s' % env['program']
    sys.exit(1)

  # Special configuration for 64 bit machines.
  if platform.architecture()[0] == '64bit':
     env.Append(CPPFLAGS=['-DPOINTER64=1'])

  return env

# Get the common environment.
env = GetEnvironment()
env.Append(CPPPATH=['../extern/argtable-2.10/include'])
env.Append(CPPPATH=['/home/c_schulz/local/opt/boost/boost_1_48_0']) #add path to boost
env.Append(LIBPATH=['../extern/argtable-2.10/lib'])
env.Append(LIBPATH=['/home/c_schulz/local/opt/boost/boost_1_48_0/stage/lib']) #add path to boost

if SYSTEM == 'Darwin':
        env.Append(CPPPATH=['/opt/local/include/','../include'])
        env.Append(LIBPATH=['/opt/local/lib/'])
        env.Append(LIBPATH=['/opt/local/lib/openmpi/'])

env.Append(CXXFLAGS = '-fopenmp')
# Apply variant specific settings.
if env['variant'] == 'optimized':
  env.Append(CXXFLAGS = ' -DNDEBUG -funroll-loops -Wall -fno-stack-limit -O3')
  env.Append(CCFLAGS = '-g -O3  -DNDEBUG')
  env.Append(LDFLAGS = '-lboost_filesystem')
elif env['variant'] == 'pdebug':
  # Optimized with debug symbols for profiling.
  env.Append(CXXFLAGS = '-g -pg  -Wall -Winline')
  env.Append(CCFLAGS = '-g -pg ')
elif env['variant'] == 'profilingoptimized':
  # Optimized with debug symbols and assertions.
  env.Append(CXXFLAGS = '-g -pg  -O0 -DSPEEDPROFILING -fno-stack-limit -Wall -Winline')
  env.Append(CCFLAGS = '-g -pg  -O0 -DSPEEDPROFILING -Winline')
else:
  # Debug settings.
  env.Append(CXXFLAGS = '-g -O0 -Wall -fno-stack-limit -Winline')
  env.Append(CCFLAGS = '-g -O0 -Winline')
 

# Execute the SConscript.
SConscript('SConscript', exports=['env'],variant_dir=env['variant'], duplicate=False)