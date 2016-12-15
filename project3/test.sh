#!/bin/bash 

for file in sample_test_and_result/*.cm ;
do 
echo $file
./parser $file
done
