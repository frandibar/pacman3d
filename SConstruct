includeDir = '#lib'

env = Environment(CPPPATH=includeDir)
Export('env')

script_files = [
    'lib/SConscript',
    'editor/SConscript'
]
#   'server/SConscript'
#   'client/SConscript'

build_dir = 'build'

# avoid duplicating source files when building to build_dir
#env.SConscript(script_files, variant_dir=build_dir, duplicate=0)
env.SConscript(script_files)


