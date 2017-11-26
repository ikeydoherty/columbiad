# We'll generate resources in this build directory, so we'll need the include path
data_include = include_directories('.')

# Sort our our resources
columbiad_resources = gnome.compile_resources(
    'columbiad-resources',
    'app.gresource.xml',
    source_dir: meson.current_source_dir(),
    c_name: 'columbiad',
)
