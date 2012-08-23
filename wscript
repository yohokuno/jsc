def options(opt):
    opt.load('compiler_cxx')

def configure(conf):
    conf.load('compiler_cxx') 
    conf.env.LIB_MARISA = ['marisa']
    conf.env.CXXFLAGS = ['-O3', '-Wall']

def build(bld):
    bld.stlib(source=bld.path.ant_glob('src/*.cpp'), target='jsc', includes = ['src'])
    bld.program(source='tools/marisa-test.cpp', target='marisa-test', use=['MARISA'])
    bld.program(source='tools/jsc-test.cpp', target='jsc-test', use=['jsc','MARISA'], includes = ['src'])
    bld.program(source='tools/jsc-builder.cpp', target='jsc-builder', use=['jsc','MARISA'], includes = ['src'])
    bld.program(source='tools/jsc-decode.cpp', target='jsc-decode', use=['jsc','MARISA'], includes = ['src'])

