cmd_drivers/firmware/built-in.o :=  rm -f drivers/firmware/built-in.o; arm-ostl-linux-gnueabi-ar cDPrsT drivers/firmware/built-in.o drivers/firmware/firmware-uclass.o drivers/firmware/psci.o drivers/firmware/scmi/built-in.o