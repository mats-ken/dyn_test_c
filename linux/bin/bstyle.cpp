// InsertTestPts.c
// This programme inserts test-points(hooks) like "TEST_PT_***".
// Copyright 2015 MATSUMOTO Kentaro
// You can use and distribute this programme under the GNU-GPL.
//
// Use this programme after applying:
// "astyle --indent=tab=4 --add-one-line-brackets --mode=c --style=kr --suffix=none",
// or some equivalent processors.
//
// Analyse coverage with macros shuch as:
// #define	TEST_PT_IF		printf(__FILE__ "\t" __FUNCTION__ "\t%d\tif\n"    , __LINE__)
// #define	TEST_PT_FOR		printf(__FILE__ "\t" __FUNCTION__ "\t%d\tfor\n"   , __LINE__)
// #define	TEST_PT_WHILE	printf(__FILE__ "\t" __FUNCTION__ "\t%d\twhile\n" , __LINE__)
// #define	TEST_PT_DO		printf(__FILE__ "\t" __FUNCTION__ "\t%d\tdo\n"    , __LINE__)
// #define	TEST_PT_SWITCH	printf(__FILE__ "\t" __FUNCTION__ "\t%d\tswitch\n", __LINE__)
// static	int		EXECNT = 0;
// #define	C1ZERO	0
//
// You can remove test-points regular expression replacement
// TEST_PT_[A-Z]+;


#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>

#include	"TestPt.h"


#define	DEBUG	1


#ifndef	_MSC_VER
#include	<stdbool.h>
#define		INT_MAX		0x7fffffff
#endif

#define	TEST_PT_PREFIX	"TEST_PT_"
#define	isalnum_(C)	('_' == (C) || isalnum(C))

#define	max2(A, B)	(((A) < (B)) ? (B) : (A))
#define	max3(A, B, C)	max2(max2((A), (B)), (C))
#define	max4(A, B, C, D)	max2(max2((A), (B)), max2((C), (D)))


#if	DEBUG
int		debugWriteFile(const char name[], const char buf[])
{
	FILE*	fp = fopen(name, "w");
	if (NULL == fp) {
		fprintf(stderr, "fopen(%s) failure.\n", name);
		return	-1;
	}

	int		idx = 0;
	while (1) {
		if ('\0' == buf[idx]) {
			break;
		}
		fputc(buf[idx], fp);
		idx++;
	}

	fclose(fp);

	return	0;
}
#endif

int		writeFile(const char name[], const int size, const int fileNum, char sourceBuf[], char testPointBuf[])
{
	static	int		totalPtNum = 0;

	FILE*	fp = fopen(name, "wb");
	if (NULL == fp) {
		fprintf(stderr, "fopen(%s) failure.\n", name);
		return	-1;
	}

	int		ptNum = 0;
	int		idx = 0;
	while (1) {
		fputc(sourceBuf[idx], fp);
		switch (testPointBuf[idx]) {
		case	'{':
		case	'}':
			fputc(testPointBuf[idx], fp);
			break;
		default:	// [DO NOTHING]
			break;
		}
		if (size <= ++idx) {
			break;
		}
	}

	fclose(fp);

	return	0;
}

void	replaceTabToSpace(char temporaryBuf[], int size)
{
	for(int i = 0; i < size; i++){
		if('\t' == temporaryBuf[i]){
			temporaryBuf[i] = ' ';
		}
	}
}

void	replaceCrToLf(char temporaryBuf[], int size)
{
	for(int i = 0; i < size; i++){
		if('\r' == temporaryBuf[i]){
			temporaryBuf[i] = '\n';
		}
	}
}

int		replaceCrLfToLfSpace(char temporaryBuf[])
{
	char	*ptr = strstr(temporaryBuf, "\r\n");
	if(NULL == ptr){
		return	0;	// finished
	}

	ptr[0] = '\n';
	ptr[1] = ' ';

	return	1;	// candidate(s) still can remain
}

int		cancelCommentAndString(char temporaryBuf[])
{
	static	const	char*beginMark[] = { "//", "/*", "\"", "#" };
	static	const	char*endMark  [] = { "\n", "*/", "\"", "\n" };
	char*const	ptrBegin[] = {
		strstr(temporaryBuf, beginMark[0]),
		strstr(temporaryBuf, beginMark[1]),
		strstr(temporaryBuf, beginMark[2]),
		strstr(temporaryBuf, beginMark[3]),
	};

	int		beginIdx = -1;
	char*	ptrBeginFirst = max4(ptrBegin[0], ptrBegin[1], ptrBegin[2], ptrBegin[3]);
	for (int i = 0; i < 4; i++) {
		if (NULL != ptrBegin[i]) {
			if (ptrBegin[i] <= ptrBeginFirst) {
				beginIdx = i;
				ptrBeginFirst = ptrBegin[i];
			}
		}
	}
	if (beginIdx < 0) {
		return	0;	// finished
	}
	char*ptrEnd = strstr(ptrBeginFirst + 1, endMark[beginIdx]);
	if (NULL == ptrEnd) {
		return	-1;
	}
	while(('\n' == ptrEnd[0]) && ('\\' == ptrEnd[-1])){
		ptrEnd++;
		ptrEnd = strstr(ptrEnd, endMark[beginIdx]);
		if (NULL == ptrEnd) {
			return	-1;
		}
	}
	int		idxBegin = ptrBeginFirst - temporaryBuf;
	int		idxEnd = ptrEnd - temporaryBuf + strlen(endMark[beginIdx]) - 1;
	for (int i = idxBegin; i <= idxEnd; i++) {
		temporaryBuf[i] = ' ';
	}
	return	1;	// candidate(s) still can remain
}

void	cancelSwitch(char temporaryBuf[])
{
	while(1){
		char	*ptr = strstr(temporaryBuf, "switch");
		if(NULL == ptr){
			return;	// finished
		}
		ptr[5] = '*';
	}
}

int		findTestPoint(char temporaryBuf[], const char keyword1[], const char separator, const bool countParen = false)
{
	char*ptr = strstr(temporaryBuf, keyword1);
	if(NULL == ptr){
		return	INT_MAX;	// finished
	}
	int		idx  = ptr  - temporaryBuf;
	bool	chk1 = false;
	bool	chk2 = false;

	if((0 == idx)
	|| !isalnum_(temporaryBuf[idx - 1])){
		chk1 = true;
	}
	const	int		keyword1Len = strlen(keyword1);
	if(!isalnum_(temporaryBuf[idx + keyword1Len])){
		chk2 = true;
	}

	for(int i = 0; i < keyword1Len; i++){
		temporaryBuf[idx++] = ' ';
	}

	if(!chk1 || !chk2){
		return	0;	// go on
	}

	int		nParen = 0;
	while('\0' != temporaryBuf[idx]){
		if('(' == temporaryBuf[idx]){
			nParen++;
		}else if(')' == temporaryBuf[idx]){
			nParen--;
		}
		if(separator == temporaryBuf[idx]){
			if(countParen){
				if(0 == nParen){
					return	idx;
				}else{
					fprintf(stderr, "parens not match.\n");
				}
			}else{
				return	idx;
			}
		}
		idx++;
	}
	return	-2;	// error
}

int		setTestPoints(char temporaryBuf[], char testPointBuf[], const char keyword[], const int separator, const int flag)
{
	while(1){
		const	int		ret = findTestPoint(temporaryBuf, keyword, separator);
		if(ret < -1){
			fprintf(stderr, "separator('{' or ':') missing.\n");
			return	-7;
		}
		if(INT_MAX == ret){
			break;
		}
		if(0 < ret){
			testPointBuf[ret] = flag;
		}
	}
	return	0;
}

int		setTestPointsDoWhile(char temporaryBuf[], char testPointBuf[], const int flag)
{
	while(1){
		const	int		ret = findTestPoint(temporaryBuf, "do", '{');
		if(ret < 0){
			fprintf(stderr, "separator('{' or ':') missing.\n");
			return	-7;
		}
		if(INT_MAX == ret){
			break;
		}
		if(0 < ret){
			testPointBuf[ret] = flag;
			char*	ptr = strstr(temporaryBuf + ret, "while");
			if(NULL == ptr){
				fprintf(stderr, "while missing.\n");
				return	-8;
			}
			const	int		idx = ptr - temporaryBuf;
			for(int i = idx; i < idx + 5; i++){
				temporaryBuf[i] = ' ';
			}
		}
	}
	return	0;
}

int		readFile(const char name[], char**pSourceBuf, char**pTemproraryBuf)
{
	FILE*	fp = fopen(name, "rb");
	if (NULL == fp) {
		fprintf(stderr, "fopen(%s) failure.\n", name);
		return	-1;
	}

	fseek(fp, 0L, SEEK_END);
	const	int		sourceLen = ftell(fp) + 1;
	fseek(fp, 0L, SEEK_SET);

	char*	sourceBuf = (char*)malloc(sourceLen + 10);
	if (NULL == sourceBuf) {
		fprintf(stderr, "malloc(%d) failure.\n", sourceLen + 10);
		return	-2;
	}
	char*	temporaryBuf = (char*)malloc(sourceLen + 10);
	if (NULL == temporaryBuf) {
		fprintf(stderr, "malloc(%d) failure.\n", sourceLen + 10);
		return	-3;
	}

	int		idx = 0;
	while (1) {
		const	int		c = fgetc(fp);
		if (EOF == c) {
			temporaryBuf[idx] = sourceBuf[idx] = '\0';
			break;
		} else if ('\r' == c) {
			sourceBuf[idx] = c;
			temporaryBuf[idx] = '\n';
		} else {
			temporaryBuf[idx] = sourceBuf[idx] = c;
		}
		idx++;
	}

	fclose(fp);

	*pSourceBuf = sourceBuf;
	*pTemproraryBuf = temporaryBuf;

	return	idx;	//sourceLen;
}

int		searchIdentifier(const char buf[], const char id[])
{
	const	char*ptr = buf;
	const	int		idLen = strlen(id);
	while(1){
		ptr = strstr(ptr, id);
		if(NULL == ptr){
			return	-1;	// not found
		}

		if(isalnum_(ptr[-1]) || !isalnum_(ptr[idLen])){
			ptr += idLen;
			continue;
		}
		return	ptr - buf;
	}
}

int		cancelDoWhile(char temporaryBuf[])
{
	int		idx = searchIdentifier(temporaryBuf, "do");
	if(idx < 0){
		return	0;	// finished
	}

	temporaryBuf[idx] = ' ';

	int		nBracket = 0;
	bool	atLeastOneBracket = false;
	bool	atLeastOneSemiColon = false;
	while(1){
		switch(temporaryBuf[idx]){
		case '{':
			atLeastOneBracket = true;
			nBracket++;
			break;
		case '}':
			nBracket--;
			break;
		case ';':
			atLeastOneSemiColon = true;
			break;
		case '\0':
			return	1;
		}
		idx++;
		if((atLeastOneBracket || atLeastOneSemiColon) && (0 == nBracket)){
			break;
		}
	}
	idx = searchIdentifier(temporaryBuf, "while");
	temporaryBuf[idx] = ' ';

	return	1;
}

int		test(char temporaryBuf[], char testPointBuf[], const int sourceLen, const char keyword[])
{
	char*ptr = temporaryBuf;
	const	int		keywordLen = strlen(keyword);
	int		nParen = 0;
	while(1){
		ptr = strstr(ptr, keyword);
		if(NULL == ptr){
			return	0;	// finished
		}
		memset(ptr, ' ', keywordLen);
		if(isalnum_(ptr[-1])){
			return	1;	// candidate(s) can remain
		}
		if(isalnum_(ptr[keywordLen])){
			return	1;	// candidate(s) can remain
		}
		ptr += keywordLen;
		int		idx = ptr - temporaryBuf;
		if(sourceLen < idx){
			return	1;	// candidate(s) can remain
		}
		bool	atLeastOneParen = false;
		while(1){
			switch(temporaryBuf[idx]){
			case ' ':
				break;
			case '(':
				atLeastOneParen = true;
				nParen++;
				break;
			case ')':
				nParen--;
				break;
			}
			idx++;
			if(sourceLen < idx){
				return	1;	// candidate(s) can remain
			}
			if(atLeastOneParen && (0 == nParen)){
				break;
			}
		}

		while(1){
			switch(temporaryBuf[idx]){
			case '{':
				return	1;	// candidate(s) can remain
				break;
			case ' ':
				break;
			case ';':
				testPointBuf[idx - 1] = '{';
				testPointBuf[idx    ] = '}';
				return	1;	// candidate(s) can remain
			}
			idx++;
			if(sourceLen < idx){
				return	1;	// candidate(s) can remain
			}
		}

	}
	return	1;	// candidate(s) can remain
}

int		BStyle(const char name[], const int fileNum = 0)
{
#if	DEBUG
	fprintf(stderr, "bstyle: processing %s ... ", name);
#endif

	char*	temporaryBuf;
	char*	sourceBuf;
	const	int		sourceLen = readFile(name, &sourceBuf, &temporaryBuf);
	if (sourceLen < 0) {
		return	-1;
	}

	char*	testPointBuf = (char*)malloc(sourceLen + 10);
	if(NULL == testPointBuf){
		fprintf(stderr, "malloc(%d) failure.\n", sourceLen + 10);
		return	-5;
	}
	memset(testPointBuf, ' ', sourceLen);
	testPointBuf[sourceLen] = '\0';

	while(replaceCrLfToLfSpace(temporaryBuf)){}
	replaceCrToLf(temporaryBuf, sourceLen);
	replaceTabToSpace(temporaryBuf, sourceLen);
	while(cancelCommentAndString(temporaryBuf)){}
	cancelSwitch(temporaryBuf);

	while(cancelDoWhile(temporaryBuf));
	while(test(temporaryBuf, testPointBuf, sourceLen, "while"));

	//setTestPoints(temporaryBuf, testPointBuf, "if"     , '{', 'i');
	//setTestPoints(temporaryBuf, testPointBuf, "else"   , '{', 'i');
	//setTestPoints(temporaryBuf, testPointBuf, "for"    , '{', 'f');
	//setTestPoints(temporaryBuf, testPointBuf, "while"  , '{', 'w');
	//setTestPoints(temporaryBuf, testPointBuf, "case"   , ':', 's');
	//setTestPoints(temporaryBuf, testPointBuf, "default", ':', 's');

#if	DEBUG
	//debugWriteFile("temporaryBuf.c", temporaryBuf);
	//debugWriteFile("testPointBuf.c", testPointBuf);
#endif

	writeFile(name, sourceLen, fileNum, sourceBuf, testPointBuf);

	free(sourceBuf);
	free(temporaryBuf);
	free(testPointBuf);

#if	DEBUG
	fputs("done.\n", stderr);
#endif

	return	0;
}

#ifndef	_MSC_VER

int	main(const int argc, const char*argv[])
{
	for(int i = 1; i < argc; i++){
		BStyle(argv[i]);
	}
	return	EXIT_SUCCESS;
}

#endif
