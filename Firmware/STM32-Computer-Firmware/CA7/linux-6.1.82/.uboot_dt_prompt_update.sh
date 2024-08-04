#!/bin/bash
#set -x

# $1: Device Tree name to be added
#     typically  : stm32mp157c-ev1_mx-mx
# $2: .conf file to be modified
#      typically : extlinux.conf

LOG="/dev/null"
LOG="/tmp/`basename $0`"
> $LOG
echo "Log available in $LOG"

echo2() {
    echo $@
    echo "$@" >> $LOG
}


# Argument check
if [ $# -lt 2 ]; then
    echo2 "Usage: $0 <Device Tree name> <*.conf file to modify>"
    exit 1
fi

DT_NAME="$1"						# Device Tree name to be added
CONF_FILE="$2"						# .conf file to be modified
CONF_FILE_SAVE="$2.save"					# .conf file saved

GAWK_SCRIPT_NODEFAULT='
/TIMEOUT/ {
   print $0
   print "DEFAULT " newDTName ; next;
}

{
print $0
}

/KERNEL/ {
   kernel= $0
 };

/INITRD/ {
   initrd= $0
 };

/APPEND/ {
   append= $0
 };


END {
    print "LABEL " newDTName;
    print kernel;
    print "\tFDT /" newDTName ".dtb";
    print initrd;
    print append;
};
'



GAWK_SCRIPT_DEFAULT='
/DEFAULT/ {
   print "DEFAULT " newDTName ; next;
}

{
print $0
}

/KERNEL/ {
   kernel= $0
 };

/INITRD/ {
   initrd= $0
 };

/APPEND/ {
   append= $0
 };


END {
    print "LABEL " newDTName;
    print kernel;
    print "\tFDT /" newDTName ".dtb";
    print initrd;
    print append;
};
'

# Check if Device Tree is already supported

grep "DEFAULT" "$CONF_FILE" | grep "$DT_NAME"
RET=$?

if [ $RET -eq 0 ]; then
	echo2 "$DT_NAME is already the default Device Tree in $CONF_FILE"
	exit 1
fi

grep "LABEL" "$CONF_FILE" | grep "$DT_NAME"
RET=$?

if [ $RET -eq 0 ]; then
	echo2 "$DT_NAME is already a choice in $CONF_FILE"
	exit 2
fi

/bin/cp "$CONF_FILE" "$CONF_FILE.save"

grep "DEFAULT" "$CONF_FILE"
RET=$?

if [ $RET -eq 0 ]; then
    /usr/bin/gawk -v newDTName="$DT_NAME" "$GAWK_SCRIPT_DEFAULT" "$CONF_FILE_SAVE" > $CONF_FILE
else
    /usr/bin/gawk -v newDTName="$DT_NAME" "$GAWK_SCRIPT_NODEFAULT" "$CONF_FILE_SAVE" > $CONF_FILE
fi

exit 0





