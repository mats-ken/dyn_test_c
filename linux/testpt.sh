BASE=`pwd`
BIN=bin
SRC=00source
STY=10astyle
PTS=20testPts
ANA=30analysis
BAK=80backup
SET=90settings


#for debug
cd $BIN
make

cd ../usr
mkdir -p $SRC
mkdir -p $ANA
mkdir -p $BAK

tar cfz $BAK/$SRC_`date +%Y%m%d_%H%M%S`.tar.gz $SRC/
#tar cfz $BAK/$PTS_`date +%Y%m%d_%H%M%S`.tar.gz $PTS/
tar cfz $BAK/$ANA`date +%Y%m%d_%H%M%S`.tar.gz $ANA/

mkdir -p $SRC
rm -rf $STY ; mkdir -p $STY
rm -rf $PTS ; mkdir -p $PTS
rm -rf $ANA ; mkdir -p $ANA
mkdir -p $BAK
mkdir -p $SET

# debug
#rm -rf input
#tar xfz input.tar.gz

rsync -aur --delete $SRC/ $STY/

cd $STY
rm -f filelist.txt
../$SET/filelist.sh > filelist.txt
xargs `cat ../$SET/astyle.sh` < filelist.txt
#xargs ../../bin/bstyle < filelist.txt
cd ..

rsync -aur --delete $STY/ $PTS/

cd $PTS
xargs ../../bin/InsertTestPts < filelist.txt
cd ..

rsync -aur --delete $PTS/ $ANA/
