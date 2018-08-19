#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>
#include	<map>

#include	"TestPt.h"


#define	DEBUG	0


using	namespace	std;

typedef	map<int, int>	histogramme_t;


#define	isNewLine(C)	(('\r' == (C)) || ('\n' == (C)))


int		readHistogramme(const char name[], histogramme_t&histogramme)
{
	FILE*	fp = fopen(name, "rt");
	if(NULL == fp){
		fprintf(stderr, "fopen(%s) failure.\n", name);
		return	-1;
	}

	char	buf[102400];
	while(NULL != fgets(buf, 102400, fp)){
		int		idx = 0;
		const	int		first = atoi(buf);
		while('\t' != buf[idx++]){}
		const	int		second = atoi(&buf[idx]);
		if(0 != histogramme[first]){
			fprintf(stderr, "*** invalid histogramme(%d) ***\n", first);
		}
		histogramme[first] = second;
	}

	fclose(fp);

	return	0;
}

int		readFile(const char name[], char**buf)
{
	FILE*fp = fopen(name, "rt");
	if(NULL == fp){
		fprintf(stderr, "fopen(%s, r) failure.\n", name);
		return	-1;
	}

	fseek(fp, 0L, SEEK_END);
	const	int		size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	*buf = (char*)malloc(size + 10);
	if(NULL == *buf){
		fprintf(stderr, "malloc() failure.\n");
		return	-1;
	}

	int		idx = 0;
	while(1){
		const	int		c = fgetc(fp);
		if(EOF == c){
			(*buf)[idx] = '\0';
			break;
		}
		(*buf)[idx] = (char)c;
		idx++;
	}

	fclose(fp);

	return	size;
}

int		writeFile(const char name[], const char sourceBuf[], const int frequencyBuf[])
{
	FILE*fp = fopen(name, "wt");
	if(NULL == fp){
		fprintf(stderr, "fopen(%s, w) failure.\n", name);
		return	-1;
	}

	int		i = 0;
	while('\0' != sourceBuf[i]){
		if(0 != frequencyBuf[i]){
			fprintf(fp, "%d", frequencyBuf[i]);
			while(')' != sourceBuf[i] && '\0' != sourceBuf[i]){
				i++;
			}
		}
		if('\0' == sourceBuf[i]){
			break;
		}
		fputc(sourceBuf[i], fp);
		i++;
	}

	fclose(fp);

	return	0;
}

int		findTestPt(const char sourceBuf[], const int fileNum, const int ptNum)
{
	const	char*	ptr = sourceBuf;
	while(1){
		ptr = strstr(ptr, "TEST_PT(PT_");
		if(NULL == ptr){
			fprintf(stderr, "*** no test point ***\n");
			return	-1;
		}

		while(!isdigit(*ptr)){
			ptr++;
		}
		if(fileNum != atoi(ptr)){
			fprintf(stderr, "*** internal error(1). ***\n");
			return	-2;
		}

		while(',' != *ptr++){}
		while(!isdigit(*ptr)){
			ptr++;
		}
		if(ptNum == atoi(ptr)){
			break;
		}
	}
	while (',' != *ptr++) {}
	while (',' != *ptr++) {}
	while(' ' == *ptr){
		ptr++;
	}
	//if(0 != strncmp(ptr, "EXECNT=", 7)){
	//	fprintf(stderr, "*** internal error(2). ***\n");
	//	return	-3;
	//}
	//ptr += 7;
	return	(int)(ptr - sourceBuf);

	return	0;
}

int		insertFrequency(char**filelist, histogramme_t&histogramme)
{
	FILE*	fp = NULL;
	int		lastFileNum = -1;
	int		idx = 0;
	char*	sourceBuf;
	int*	frequencyBuf;
	int		lastPtNum;
	int		fileSize;

	for(histogramme_t::iterator itr = histogramme.begin();
		itr != histogramme.end();
		++itr) {
		const	int		fileNum = itr->first / 65536;
		const	int		ptNum = itr->first % 65536;
		const	int		frequency = itr->second;

		if(fileNum != lastFileNum){
			if(0 <= lastFileNum){
				writeFile(filelist[lastFileNum], sourceBuf, frequencyBuf);
				free(sourceBuf);
				free(frequencyBuf);
			}
			fileSize = readFile(filelist[fileNum], &sourceBuf);
			frequencyBuf = (int*)calloc(fileSize, sizeof(int));
			lastPtNum = 0;
			idx = 0;
		}

		int		idx = findTestPt(sourceBuf, fileNum, ptNum);

		frequencyBuf[idx] = frequency;

		lastFileNum = fileNum;
		lastPtNum = ptNum;

#if	DEBUG
		fprintf(stderr, "%d\t%d\t%d\n", itr->first / 65536, itr->first % 65536, itr->second);
#endif
	}

	if(0 <= lastFileNum){
		writeFile(filelist[lastFileNum], sourceBuf, frequencyBuf);
		free(sourceBuf);
		free(frequencyBuf);
	}

	return	0;
}

char**	getFilelist(const char name[])
{
	FILE*	fp = fopen(name, "rt");
	if(NULL == fp){
		fprintf(stderr, "fopen(%s) failure.\n", name);
		return	NULL;
	}


	char	buf[102400];
	int		lines = 0;
	while(NULL != fgets(buf, 102400, fp)){
		lines++;
	}

	char**	filelist = (char**)malloc(sizeof(char*) * (lines + 10));
	if(NULL == filelist){
		fprintf(stderr, "malloc() failure.\n");
		return	NULL;
	}

	fseek(fp, 0L, SEEK_SET);
	for(int i = 0; i < lines; i++){
		if(NULL == fgets(buf, 102400, fp)){
			fprintf(stderr, "fgets() failure.\n");
			return	NULL;
		}
		int		len = strlen(buf);
		if(('\r' == buf[len - 1])
		|| ('\n' == buf[len - 1])){
			buf[len - 1] = '\0';
			len--;
		}
		if(('\r' == buf[len - 1])
		|| ('\n' == buf[len - 1])){
			buf[len - 1] = '\0';
			len--;
		}

		filelist[i] = (char*)malloc(len + 10);
		if(NULL == filelist[i]){
			fprintf(stderr, "malloc() failure.\n");
			return	NULL;
		}
		strcpy(filelist[i], buf);
	}
	filelist[lines] = NULL;

	fclose(fp);

	return	filelist;
}

#ifndef	_MSC_VER

int		main(const int argc, const char*argv[])
{
	if(argc < 3){
		fprintf(stderr, "too few arguments.\n");
		return	1;
	}
	char**	filelist = getFilelist(argv[1]);

#if	DEBUG
	int		i = 0;
	while(1){
		if(NULL == filelist[i]){
			break;
		}
		printf("%d\t", i);
		puts(filelist[i]);
		i++;
	}
#endif
	histogramme_t	histogramme;
	readHistogramme(argv[2], histogramme);
	insertFrequency(filelist, histogramme);

	return	0;
}

#endif
