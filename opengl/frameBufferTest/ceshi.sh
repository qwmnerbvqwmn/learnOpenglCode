for file in `ls`
do
 newfile=`echo $file | sed 's/cullFaceTest/frameBufferTest/g'`
 mv $file $newfile
done