#include	<stdio.h>
#include	<stdlib.h>
#include	<map>


#define	DEBUG	0


using	namespace	std;

typedef	map<int, int>	histogramme_t;


int		getHistogramme(histogramme_t&histogramme, const int fieldNum = 0)
{
	char	buf[102400];
	while(NULL != fgets(buf, 102400, stdin)){
		int		idx = 0;
		for(int i = 0; i < fieldNum; i++) {
			while('\t' != buf[idx++]){}
		}
		const	int		n = atoi(&buf[idx]);
		histogramme[n]++;
	}

	return	0;
}

int		outputHistogramme(histogramme_t&histogramme)
{
	for(histogramme_t::iterator itr = histogramme.begin(); itr != histogramme.end(); itr++) {
		printf("%d\t%d\n", itr->first, itr->second);
	}

	return	0;
}

#ifndef	_MSC_VER

int		main(const int argc, const char*argv[])
{
	const	int		fieldNum = atoi(argv[1]);
	histogramme_t	histogramme;
	getHistogramme(histogramme, fieldNum);
	outputHistogramme(histogramme);

	return	0;
}

#endif
