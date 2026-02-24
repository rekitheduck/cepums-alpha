project('Cepums-Alpha', 'cpp', default_options : ['cpp_std=c++20'])

fmt = subproject('fmt')
fmt_dep = fmt.get_variable('fmt_dep')
spdlog = subproject('spdlog')
spdlog_dep = spdlog.get_variable('spdlog_dep')

subdir('cepums')
