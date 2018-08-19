cd usr
rm -rf 20testPts_
mkdir -p 20testPts_
rsync -au 20testPts/ 20testPts_/

cd 20testPts_

for F in `cat filelist.txt`
do
	sed -i -e 's/TEST_PT([^)]*)\;/TEST_PT();/g' $F
done
