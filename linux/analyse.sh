cd usr/30analysis

find . -name "*.log" -type f | xargs cat | ../../bin/histogramme 1 > histogramme.txt
../../bin/InsertCoverage filelist.txt histogramme.txt
