#!/bin/bash
# usage : cat log_file1 log_file2 ... log_filen | ./get_stats.sh
bytesS=0
bytesR=0
filesS=0
filesR=0
maxID=-1         # clients have positive ids
minID=2147483647 # 32 bit max
clientsLeft=0
clientsConnected=0
i=0
j=0
leftArray=()
connectedArray=()
while IFS=' ' read path size sender; do
  if [ "$path" = "left_client" ]; then # find left clients and skip checks below
    leftArray[$i]=$size
    let "i += 1"
    continue
  fi
  declare -i size               # evaluate file size as number for checks below
  if [ "$size" -gt 0 ]; then    # skip directories
    let "filesS += 1"           # 1-1 situation 1 file sent by a client
    let "filesR += 1"           # = 1 file received by another client
    let "bytesS += $size"       # same goes for file data since logfiles
    let "bytesR += $size"       # have entries only for completed transfers
  fi
  declare -i sender             # evaluate as number for checks below
  connectedArray[$j]=$sender    # get all the client ids
  let "j += 1"
  if [ "$sender" -lt "$minID" ]; then # find the minimum client id
    let "minID = $sender"
  fi
  if [ "$sender" -gt "$maxID" ]; then # find the maximum client id
    let "maxID = $sender"
  fi
done
leftArray=($(echo ${leftArray[@]} | tr ' ' '\n' | sort -nu)) # keep unique(-u)
let "clientsLeft = ${#leftArray[@]}"           # count the left clients
connectedArray=($(echo ${connectedArray[@]} | tr ' ' '\n' | sort -nu))
let "clientsConnected = ${#connectedArray[@]}" # count total clients
echo "Connected clients IDs : ${connectedArray[@]}"
echo "Total clients connected : $clientsConnected"
echo "Minimum client ID : $minID"
echo "Maximum client ID : $maxID"
echo "Total bytes sent : $bytesS"
echo "Total bytes received : $bytesR"
echo "Total files sent : $filesS"
echo "Total files received : $filesR"
echo "Total clients left : $clientsLeft"
unset leftArray # remove the arrays
unset connectedArray