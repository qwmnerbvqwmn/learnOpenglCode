for file in `ls`
do
 newfile=`echo $file | sed 's/coordSystem/coordSystemReal/g'`
 mv $file $newfile
done