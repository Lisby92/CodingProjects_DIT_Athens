#!/bin/bash
#Usage : ./create_infiles.sh dir_name num_of_files num_of_dirs levels
params=$#
if [ "$params" -ne 4 ]; then
  echo "error: script arguments must be 4"
  exit 1
fi
Program_Name=${0}
dir_name=${1}
num_of_files=${2}
num_of_dirs=${3}
levels=${4}
# used to check if argument is a number (excludes float too)
re='^[+-]?[0-9]+$' 
if ! [[ $num_of_files =~ $re ]]; then
   echo "error: num_of_files must be a number"
   exit 1
fi
# check if num_of_files to be created are more than 0
if [ "$num_of_files" -lt 0 ]; then
  echo "error: file number must be >= 0"
  exit 1
fi

if ! [[ $num_of_dirs =~ $re ]]; then
   echo "error: num_of_dirs must be a number"
   exit 1
fi
# check if num_of_dirs to be created are more than 0
if [ $num_of_dirs -lt 0 ]; then
  echo "error: directory number must be >= 0"
  exit 1
fi

if ! [[ $levels =~ $re ]]; then
   echo "error: levels must be a number"
   exit 1
fi
# check if levels to be created are more than 0
if [ $levels -lt 0 ]; then
  echo "error: levels number must be >= 0"
  exit 1
fi
# check if directory exists if not create it
if [ ! -d "$dir_name" ]; then
  echo "creating main directory $dir_name"
  mkdir $dir_name
else
  echo "directory $dir_name already exists"
fi
# enter the parent directory
cd $dir_name
# create the directories level-wise
dirCounter=0
fileCounter=0
echo "creating $num_of_dirs directories at $levels depth"
while [ "$dirCounter" -lt "$num_of_dirs" ]; do
  curLvl=0
  while [ "$curLvl" -lt "$levels" ]; do
    if [ "$dirCounter" -eq "$num_of_dirs" ]; then
      break
    fi
    let "dirCounter += 1"
    let "curLvl += 1"
    #generate a random number == #chars in new directoryName
    nD=$(cat /dev/urandom | tr -dc '1-8' | fold -w 256 | head --bytes 1)
    #generate a random string with size nD characters
    nS=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w $nD | head -n 1)
    mkdir $nS # create it
    cd $nS # jump in for next level
  done
  while [ "$curLvl" -gt 0 ]; do
    cd ../ # jump back
    let "curLvl -= 1"
  done
done
# add the files RR-wise
workingDir=`pwd`
fileCounter=0
echo "creating $num_of_files files"
while [ "$fileCounter" -lt "$num_of_files" ]; do
  # traverse the directory tree
  for d in $(find $workingDir -maxdepth $levels -type d); do
    # check if we can create more files
    if [ "$fileCounter" -eq "$num_of_files" ]; then
      break
    fi
    cd $d #jump in the directory
    #generate a random number == #chars in new fileName
    nF=$(cat /dev/urandom | tr -dc '1-8' | fold -w 256 | head --bytes 1)
    #generate a random string with size nF characters
    nfS=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w $nF | head -n 1)
    let "fileCounter += 1"
    touch $nfS # create the file and fill with data
    #generate a random big enough number
    nT=$(cat /dev/urandom | tr -dc '1-9' | fold -w 256 | head --bytes 8)
    #get it in range 1024-131072 bytes long = 1KB ~ 128KB
    let "nT %= 130049"
    let "nT += 1024"
    #generate a random string with size nT characters
    ntS=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w $nT | head -n 1)
    #write it out to the newly created file
    echo "$ntS" > $nfS
  done
done
exit 0
# End of script