columbiad_sources = [
    'carousel.c',
    'image.c',
    'main.c',
    'settings.c',
    'window.c',
]

columbiad = executable(
    'columbiad',
    sources: columbiad_sources + columbiad_resources,
    install: true,
    dependencies: [
        dep_gio_unix,
        dep_gtk3,
    ],
)
