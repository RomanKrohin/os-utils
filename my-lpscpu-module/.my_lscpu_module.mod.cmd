savedcmd_/home/roman/os-utils/my-lpscpu-module/my_lscpu_module.mod := printf '%s\n'   my_lscpu_module.o | awk '!x[$$0]++ { print("/home/roman/os-utils/my-lpscpu-module/"$$0) }' > /home/roman/os-utils/my-lpscpu-module/my_lscpu_module.mod
