def options(opt):
    opt.load('compiler_cxx')

def configure(conf):
    conf.load('compiler_cxx') 
    conf.env.LIB_MARISA = ['marisa']
    conf.env.LIB_EVENT = ['event']
    conf.env.CXXFLAGS = ['-O3', '-Wall']

def build(bld):
    bld.stlib(source=bld.path.ant_glob('src/*.cpp'), target='jsc', includes = ['src'])
    bld.program(source='tools/jsc-test.cpp', target='jsc-test', use=['jsc','MARISA'], includes = ['src'], install_path = None)
    bld.program(source='tools/jsc-build.cpp', target='jsc-build', use=['jsc','MARISA'], includes = ['src'])
    bld.program(source='tools/jsc-decode.cpp', target='jsc-decode', use=['jsc','MARISA'], includes = ['src'])
    bld.program(source='tools/jsc-server.cpp', target='jsc-server', use=['jsc','MARISA','EVENT'], includes = ['src'])

