# Script to clean up the shared memory

flag=0;

for i in $(ipcs | grep $USER | awk '{ print $2 }');
do
  if (( $flag == 0 )); then
    echo "ipcrm -m $i"
    ipcrm -m $i
    flag=1;
  else
    echo "ipcrm -s $i"
    ipcrm -s $i
    flag=0;
  fi
done


ps | grep amazing_client | awk {'print $4'} | xargs kill -9

exit 0;

