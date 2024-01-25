project('app', 'c')

gtkdep = dependency('gtk4')
# vtedep = dependency('vte-2.91-gtk4')

sourcefiles=files('main.c','demowidget.h','demowidget.c')

executable('app', sourcefiles, dependencies: [gtkdep], install: false)
