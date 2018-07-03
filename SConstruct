env = Environment(CPPPATH = 'include')

genlev = 'genlev'
genlevinput = ['generate_levels.c']

outname = 'snake'
input = ['main.c']
libs = ['ncurses']

env.Program(target = outname, LIBS = libs, source = input)
env.Program(target = genlev, source = genlevinput)