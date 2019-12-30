for file in `ls`
do
 newfile=`echo $file | sed 's/frameBufferTest/frameBufferMirrorTest/g'`
 mv $file $newfile
done