project(
    'columbiad',
    ['c'],
    version: '1',
    license: [
        'GPL-2.0',
    ],
    default_options: [
        'c_std=c11',
        'prefix=/usr',
        'sysconfdir=/etc',
    ],
)

am_cflags = [
    '-fstack-protector',
    '-Wall',
    '-pedantic',
    '-Wstrict-prototypes',
    '-Wundef',
    '-fno-common',
    '-Werror-implicit-function-declaration',
    '-Wformat',
    '-Wformat-security',
    '-Werror=format-security',
    '-Wconversion',
    '-Wunused-variable',
    '-Wunreachable-code',
    '-W',
    '-D_FORTIFY_SOURCE=2',
]


# Add our main flags
add_global_arguments(am_cflags, language: 'c')

# Need GNOME module throughout our build
gnome = import('gnome')

# Required minimum versions
gtk_min_version = '>= 3.12.0'
glib_min_version = '>= 2.40.0'

# GTK/UI deps
dep_gtk3 = dependency('gtk+-3.0', version: gtk_min_version)
dep_gio_unix = dependency('gio-unix-2.0', version: glib_min_version)

# Get configuration bits together
path_prefix = get_option('prefix')
path_bindir = join_paths(path_prefix, get_option('bindir'))
path_datadir = join_paths(path_prefix, get_option('datadir'))

# Sort out config.h now
cdata = configuration_data()

# General options..
cdata.set_quoted('PACKAGE_VERSION', meson.project_version())

# Write config.h now
config_h = configure_file(
     configuration: cdata,
     output: 'config.h',
)
config_h_dir = include_directories('.')

# Handle data before anything else
subdir('data')

# Now go build the source
subdir('src')


report = [
    '    Build configuration:',
    '    ====================',
    '',
    '    prefix:                                 @0@'.format(path_prefix),
    '    datadir:                                @0@'.format(path_datadir),
    '    bindir:                                 @0@'.format(path_bindir),
]

# Output some stuff to validate the build config
message('\n\n\n' + '\n'.join(report) + '\n\n')
