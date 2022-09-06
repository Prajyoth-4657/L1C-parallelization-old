#!/bin/bash

start_slot=$1
end_slot=$2
data_dir=$3
data_directory=$4

if [ $data_dir == 'DL' ]; then
	for ((i=start_slot;i<=end_slot;i++))
	do
		./testmake_DL $data_directory/Slot$i/message.bin $data_directory/configs_DL_slot$i.txt $data_directory/configs_DL_HWpacketsONLY.txt
	done
elif [ $data_dir == 'UL' ]; then
	for ((i=start_slot;i<=end_slot;i++))
	do
		./testmake_UL $data_directory/Slot$i/message.bin $data_directory/Slot$i/configs_UL_slot$i.txt $data_directory/configs_UL_HWpacketsONLY.txt
	done
fi
