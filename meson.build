project('main', 'cpp')

srcs = [
    'src/main.cpp'
]

include_dirs = [
    'src'
]

common_args = [
    '-std=c++20',
    '-Wextra',
    '-W',
    '-Wall',
    '-ffreestanding',
    '-nostdlib',
    '-fno-rtti',
    '-fno-exceptions'
]

if get_option('native')
    common_args += '-march=native -mtune=native'
endif

largs = []
largs = ['-L' + meson.source_root() + '/ldscripts', '-Tshellcode.ld']
largs += common_args

ldeps = 'ldscripts/shellcode.ld'

executable('main.out', sources : srcs, cpp_pch : 'src/pch.h', include_directories : include_dirs, cpp_args : common_args, pie : true, link_args : largs, link_depends : ldeps)
