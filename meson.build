project('Cepums-Alpha', 'cpp', default_options : ['cpp_std=c++20'])

fmt = subproject('fmt')
fmt_dep = fmt.get_variable('fmt_dep')
spdlog = subproject('spdlog')
spdlog_dep = spdlog.get_variable('spdlog_dep')

# Idk why but msvc spits out a bunch of warnings in fmt so these should suppress those
if host_machine.system() == 'windows'
    flags = ['-wd4251', '-wd4275']
    add_project_arguments(flags, language: ['cpp'])
endif

subdir('cepums')
