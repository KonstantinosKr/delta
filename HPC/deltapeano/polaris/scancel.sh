for job in {826500..826720}  
do 
echo -n "canceling job $job ... " 
scancel $job 
echo "done" 
done 
