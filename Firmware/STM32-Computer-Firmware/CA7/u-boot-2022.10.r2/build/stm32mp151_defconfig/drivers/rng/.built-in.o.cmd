cmd_drivers/rng/built-in.o :=  rm -f drivers/rng/built-in.o; arm-ostl-linux-gnueabi-ar cDPrsT drivers/rng/built-in.o drivers/rng/rng-uclass.o drivers/rng/optee_rng.o drivers/rng/stm32_rng.o
