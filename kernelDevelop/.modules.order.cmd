cmd_/home/rusekon/kernelDevelop/modules.order := {   echo /home/rusekon/kernelDevelop/hello-1.ko; :; } | awk '!x[$$0]++' - > /home/rusekon/kernelDevelop/modules.order
