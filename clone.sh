#!/bin/bash

echo "$0 enter"

src=$1
dst=$2

if [ ! -d ca/$src ] || [ -d ca/$dst ];then
	echo "Input paramaters error!!"
fi

dec2hex(){
	printf "%02x" $1
}

g_2() # generate two bytes
{
	shu=$(echo $RANDOM)
	shu=$(( $shu % 256 ))
	echo $(dec2hex $shu)
}
ta_binary_name="1"
ta_array_name="1"
ta_get_name()
{
	ta_binary_name=$(g_2)$(g_2)$(g_2)$(g_2)"-"$(g_2)$(g_2)"-"$(g_2)$(g_2)"-"$(g_2)$(g_2)"-"$(g_2)$(g_2)$(g_2)$(g_2)$(g_2)$(g_2)
	
	var=$ta_binary_name
	ta_array_name="{ 0x"${var:0:8}", 0x"${var:9:4}", 0x"${var:14:4}", { 0x"${var:19:2}", 0x"${var:21:2}", 0x"${var:24:2}", 0x"${var:26:2}", 0x"${var:28:2}", 0x"${var:30:2}", 0x"${var:32:2}", 0x"${var:34:2}" } }"
}

# 12345678-1234-1234-1234-123412341234
# { 0x12345678, 0x1234, 0x1234, { 0x12, 0x34, 0x12, 0x34, 0x12, 0x34, 0x12, 0x34 } }
ta_get_name
echo $ta_binary_name
echo $ta_array_name

for dir in ca ta
do
	if [ -d $dir/$dst ];then
		rm -rf $dir/$dst
	fi
	cp $dir/$src $dir/$dst -r
	cd $dir/$dst
	if [ -d Out ];then
		rm -rf Out
	fi
	for f in `find ./ -name "*$src*"`;do mv "$f" `echo "$f" | sed "s/$src/$dst/g" `; done
	sed -i "s/$src/$dst/g" `grep $src -rl ./`
	sed -i "s/${src^^}/${dst^^}/g" `grep ${src^^} -rl ./`
	cd -
done

if [ `grep -c "CA_DIRS += $dst" ca/Makefile` -ne '0' ];then
	echo "Not need to modify ca/Makefile because \"CA_DIRS += $dst\" is exist."
else
	sed -i "/$src/a\CA_DIRS += $dst" ca/Makefile
fi

if [ `grep -c "TA_DIRS += $dst" ta/makefile.optee` -ne '0' ];then
        echo "Not need to modify ta/makefile.optee because \"TA_DIRS += $dst\" is exist."
else
        sed -i "/$src/a\TA_DIRS += $dst" ta/makefile.optee
fi

sed -i "/TA_${dst^^}_UUID/c#define TA_${dst^^}_UUID $ta_array_name" ca/$dst/include/ta_$dst.h
sed -i "/TA_${dst^^}_UUID/c#define TA_${dst^^}_UUID $ta_array_name" ta/$dst/include/ta_$dst.h
sed -i "/BINARY/cBINARY =  $ta_binary_name" ta/$dst/makefile.optee
