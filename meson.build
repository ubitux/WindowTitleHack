project(
  'WindowTitleHack',
  'c',
  default_options: ['c_std=c99'],
  version : '0.1',
)

cc = meson.get_compiler('c')
project_args = cc.get_supported_arguments([
  '-Werror=missing-prototypes',
  '-Werror=pointer-arith',
  '-Werror=vla',
  '-Wformat=2',
  '-Wignored-qualifiers',
  '-Wimplicit-fallthrough',
  '-Wlogical-op',
  '-Wconversion',
  '-Wno-sign-conversion',
  '-fno-math-errno',
])
add_project_arguments(project_args, language: 'c')

dep_x11 = dependency('x11')
dep_xcb = dependency('xcb-atom')
deps = [dep_x11, dep_xcb]

src = files(
  'wth.c',
  'xcb_hook.c',
  'x11_hook.c',
)

lib = library(
  'wth',
  src,
  dependencies: deps,
  install: true
)
