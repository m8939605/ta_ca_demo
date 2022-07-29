#!/bin/bash
  
target_dir=$1

echo "111111"
echo $target_dir

for i in $(ls $target_dir)
do
        echo "ln -sf $target_dir/$i $i"
        ln -sf $target_dir/$i $i
done
