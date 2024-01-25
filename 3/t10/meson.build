project('t10', 'c')

gtkdep = dependency('gtk4')

gnome=import('gnome')
resources = gnome.compile_resources('resources','app.gresource.xml')

# sourcefiles=files('first.c', 'second.c')
sourcefiles=files('main.c')

executable('app', sourcefiles, resources, dependencies: gtkdep, install: false)
