project('t10', 'c')

gtkdep = dependency('gtk4')
vtedep = dependency('vte-2.91-gtk4')

# gnome=import('gnome')
# resources = gnome.compile_resources('resources','app.gresource.xml')

# sourcefiles=files('first.c', 'second.c')
sourcefiles=files('main.c')

executable('app', sourcefiles, dependencies: [gtkdep, vtedep], install: false)
