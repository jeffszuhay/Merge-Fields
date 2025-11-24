#!/bin/zsh

# get dataFile & textFile
# or show usage

dataFile="$1"
if [ "$dataFile" = ? ] ; then
  echo "Usage $0 <dataFile> <bodyFile>"
  exit
fi

while [ ! -f "$dataFile" ] ; do
  echo "Please provide a valid data file:"
  read dataFile
done

textFile="$2"
while [ ! -f "$textFile" ] ; do
  echo "Please provide a valid text file:"
  read textFile
done

echo "Using [$dataFile] for members and [$textFile] to merge."

lineCount=0
while IFS=":" read -r line; do
  ((lineCount++))
  echo "$lineCount"
  if [ "$lineCount" -eq 1 ] ; then   # field names
    # get variable names
    declare -a varname=($f1 $f2 $f3 $f4 $f5)
  else
    # get values for this line (can contain spaces)
    declare -a value=("$f1" "$f2" "$f3" "$f4" "$f5")
  fi
  for ((i=0; i<${#value[*]}; i++)) ; do
    if [ ! -z "${value[$i]}" ] ; then
      echo "s/#${varname[$i]}#/${value[$i]}/g"
    fi
  done
done < $dataFile
