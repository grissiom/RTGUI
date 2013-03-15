import os
import sys

if os.getenv('RTT_ROOT'):
    RTT_ROOT = os.getenv('RTT_ROOT')
else:
    print "error! Please set the environment var 'RTT_ROOT'"

path_cwd = os.path.normpath(os.getcwd())
sys.path = sys.path + [os.path.join(RTT_ROOT, 'tools')]
sys.path.insert(0, os.path.join(path_cwd, 'components', 'rtgui', 'utils'))

import rtconfig
from building import *

env = Environment(TARGET_ARCH='x86')

Export('RTT_ROOT')
Export('rtconfig')

if rtconfig.PLATFORM == 'cl':
    TARGET = 'rtthread-win32.' + rtconfig.TARGET_EXT

    libs = Split('''
    winmm
    gdi32
    winspool
    comdlg32
    advapi32
    shell32
    ole32
    oleaut32
    uuid
    odbc32
    msvcrt
    odbccp32
    SDL
    SDLmain
    ''')
    definitions = Split('''
    WIN32
    _DEBUG
    _CONSOLE
    MSVC
    ''')
    env.Append(CCFLAGS=rtconfig.CFLAGS)
    env.Append(LINKFLAGS=rtconfig.LFLAGS)
    env['LIBS']=libs
    env['CPPDEFINES']=definitions
elif rtconfig.PLATFORM == 'mingw':
    libs = Split('''
        winmm
        gdi32
        winspool
        comdlg32
        advapi32
        shell32
        ole32
        oleaut32
        uuid
        odbc32
        odbccp32
        ''')
    TARGET = 'rtthread-win32.' + rtconfig.TARGET_EXT
    env = Environment(tools = ['mingw'],
        AS = rtconfig.AS, ASFLAGS = rtconfig.AFLAGS,
        CC = rtconfig.CC, CCFLAGS = rtconfig.CFLAGS,
        AR = rtconfig.AR, ARFLAGS = '-rc',
        LINK = rtconfig.LINK, LINKFLAGS = rtconfig.LFLAGS)
    env['LIBS']=libs
    env.PrependENVPath('PATH', rtconfig.EXEC_PATH)
else:
    TARGET = 'rtthread'
    env.Append(CCFLAGS=rtconfig.CFLAGS)
    env.Append(LINKFLAGS=rtconfig.LFLAGS)
    env.Append(LIBS=['m'])

# list of targets, list item format in:
#     ['executable_name', 'path_to_SConscript']
TARGETS = [['demo', 'demo'], ['realtouch', 'realtouch']]

exe_dir = 'executables'

# prepare building environment

base_objs = PrepareBuilding(env, RTT_ROOT, has_libcpu=False, remove_components=['rtgui'])

path_jn = os.path.join

BSP_ROOT = path_jn(RTT_ROOT, 'bsp/simulator')

# make use of bsp/simulator
sim_objs = SConscript(GetCurrentDir() + '/win32-sim/SConscript',
					   variant_dir='build/simulator/sim-com',
					   duplicate=0)

sim_objs.extend(SConscript(path_jn(BSP_ROOT, 'drivers/SConscript'),
					   variant_dir='build/simulator/sim-drv',
					   duplicate=0))
base_objs.extend(sim_objs)

sdl_lib_path = [os.path.abspath(path_jn(BSP_ROOT, 'SDL/lib/x86'))]
sdl_include_path = [os.path.abspath(path_jn(BSP_ROOT, 'SDL/include'))]
env.Append(LIBPATH=sdl_lib_path)
env.Append(CPPPATH=sdl_include_path)

for exe_name, src_path in TARGETS:
    work_objs = base_objs + SConscript(dirs=[src_path],
                                       variant_dir=os.path.join('build', src_path),
                                       duplicate=0)
    # build program
    program = env.Program(os.path.join(exe_dir, exe_name), work_objs)

    # end building
    EndBuilding(exe_name, program)

if ARGUMENTS.get('build_tests') == '1':
    # build for testcases
    list = os.listdir(os.path.join(str(Dir('#')), 'test_cases'))
    for d in list:
        src_path = os.path.join(str(Dir('#')), 'test_cases', d)
        if os.path.isfile(os.path.join(src_path, 'SConscript')):
            exe_name = os.path.basename(src_path)
            objs = base_objs + SConscript(dirs=[src_path],
                                           variant_dir=os.path.join('build', src_path),
                                           duplicate=0)
            # build program
            env.Program(os.path.join(exe_dir, exe_name), objs)

            # end building
            EndBuilding(exe_name)

