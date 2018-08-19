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

#include	<map>

#include	"TestPt.h"


using	namespace std;


#define	DEBUG	1


#ifndef	_MSC_VER
#include	<stdbool.h>
#define		INT_MAX		0x7fffffff
#endif

#define	TEST_PT_PREFIX	"TEST_PT(PT_"
#define	isalnum_(C)	('_' == (C) || isalnum(C))

#define	max2(A, B)	(((A) < (B)) ? (B) : (A))
#define	max3(A, B, C)	max2(max2((A), (B)), (C))
#define	max4(A, B, C, D)	max2(max2((A), (B)), max2((C), (D)))
#define	max5(A, B, C, D, E)	max2(max2((A), (B)), max3((C), (D), (E)))


typedef	map<int, int>	pt_t;


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

int		writeFile(const char name[], const int size, const int fileNum, char sourceBuf[], pt_t&testPointBuf)
{
	static	int		totalPtNum = 0;

	FILE*	fp = fopen(name, "wb");
	if (NULL == fp) {
		fprintf(stderr, "fopen(%s) failure.\n", name);
		return	-1;
	}

	int		ptNum = 0;
	int		idx = 0;
	testPointBuf[size - 1] = 'E';
	for(pt_t::iterator itr = testPointBuf.begin(); testPointBuf.end() != itr; itr++){
		int		n = itr->first;
		while(idx <= n){
			fputc(sourceBuf[idx++], fp);
		}
		switch (itr->second) {
		case	'F':
			fprintf(fp, " " TEST_PT_PREFIX "FUNC, %d, %d, %d, C1ZERO);", fileNum, ptNum++, totalPtNum++);
			break;
		case	'i':
			fprintf(fp, " " TEST_PT_PREFIX "IF, %d, %d, %d, C1ZERO);", fileNum, ptNum++, totalPtNum++);
			break;
		case	'f':
			fprintf(fp, " " TEST_PT_PREFIX "FOR, %d, %d, %d, C1ZERO);", fileNum, ptNum++, totalPtNum++);
			break;
		case	'w':
			fprintf(fp, " " TEST_PT_PREFIX "WHILE, %d, %d, %d, C1ZERO);", fileNum, ptNum++, totalPtNum++);
			break;
		case	'd':
			fprintf(fp, " " TEST_PT_PREFIX "DO, %d, %d, %d, C1ZERO);", fileNum, ptNum++, totalPtNum++);
			break;
		case	's':
			fprintf(fp, " " TEST_PT_PREFIX "CASE, %d, %d, %d, C1ZERO);", fileNum, ptNum++, totalPtNum++);
			break;
		case	'E':
		default:	// [DO NOTHING]
			break;
		}
	}

	fclose(fp);

	return	0;
}

#if	0
void	outputTestPoints(FILE*const fp, const pt_t&testPointBuf)
{
	static	const	struct{
		int		flag;
		char*	str;
	}	table[] = {
		{'i', TEST_PT_PREFIX "IF"},
		{'f', TEST_PT_PREFIX "FOR"},
		{'w', TEST_PT_PREFIX "WHILE"},
		{'d', TEST_PT_PREFIX "DO"},
		{'s', TEST_PT_PREFIX "SWITCH"},
		{'\0', NULL}
	};

	int		idx = 0;
	int		offset = 0;

	while(1){
		if('\0' == testPointBuf[idx]){
			break;
		}
		if(' ' != testPointBuf[idx]){
			for(int i = 0; '\0' != table[i].flag; i++){
				if(table[i].flag == testPointBuf[idx]){
					fseek(fp, (long)(idx + offset), SEEK_SET);
					fputs(table[i].str, fp);
					offset += strlen(table[i].str);
				}
			}
		}
		idx++;
	}
}
#endif

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
	static	const	char*beginMark[] = { "//", "/*", "\"", "'", "#"  };
	static	const	char*endMark  [] = { "\n", "*/", "\"", "'", "\n" };
	char*const	ptrBegin[] = {
		strstr(temporaryBuf, beginMark[0]),
		strstr(temporaryBuf, beginMark[1]),
		strstr(temporaryBuf, beginMark[2]),
		strstr(temporaryBuf, beginMark[3]),
		strstr(temporaryBuf, beginMark[4]),
	};

	int		beginIdx = -1;
	char*	ptrBeginFirst = max5(ptrBegin[0], ptrBegin[1], ptrBegin[2], ptrBegin[3], ptrBegin[4]);
	for (int i = 0; i < 5; i++) {
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

int		setTestPoints(char temporaryBuf[], pt_t&testPointBuf, const char keyword[], const int separator, const int flag)
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

int		setTestPointsDoWhile(char temporaryBuf[], pt_t&testPointBuf, const int flag)
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

int		setTestPointsFunc(char temporaryBuf[], pt_t&testPointBuf, const int flag)
{
	// find '[ \*][a-zA-Z0-9_]+[ \n]*\(.*\)[ \n]*\{'(regular expression)

	char*	ptr = strchr(temporaryBuf, '{');
	if(NULL == ptr){
		return	0;	// finished
	}

	const	int		idxBracket = ptr - temporaryBuf;
	int		idx = idxBracket;

	ptr[0] = ' ';

	int		nParen = 0;
	bool	error = false;
	while(0 <= idx){
		switch(temporaryBuf[idx]){
		case ')':
			nParen++;
			break;
		case ' ':
		case '\n':
			break;
		default:
			return	1;
			break;
		}
		idx--;
		if(0 < nParen){
			break;
		}
	}
	if(idx < 0){
		return	1;
	}

	while(0 <= idx){
		switch(temporaryBuf[idx]){
		case ')':
			nParen++;
			break;
		case '(':
			nParen--;
			break;
		}
		idx--;
		if(0 == nParen){
			break;
		}
	}
	if(idx < 0){
		return	1;
	}

	while(0 <= idx){
		switch(temporaryBuf[idx]){
		case ' ':
		case '\n':
			idx--;
			continue;
		default:
			if(!isalnum_(temporaryBuf[idx])){
				return	1;
			}
			break;
		}
		break;
	}
	if(idx < 0){
		return	1;
	}

	while(0 <= idx){
		if(!isalnum_(temporaryBuf[idx])){
			break;
		}
		idx--;
	}
	switch(temporaryBuf[idx]){
	case ' ':
	case '*':
		break;
	default:
		return	1;
	}

	testPointBuf[idxBracket] = 'F';

	return	1;	// candidate(s) still can remain
}

#if	0
int		setTestPointsFunc(char temporaryBuf[], pt_t&testPointBuf, const int flag)
{
	char*	ptr = strchr(temporaryBuf, '{');
	if(NULL == ptr){
		return	0;	// finished
	}

	ptr[0] = ' ';

	int		idx = ptr - temporaryBuf - 1;
	const	int		idxBracket = idx;
	int		nParen = 0;
	bool	atLeastOneParen = false;
	while(0 <= idx){
		switch(temporaryBuf[idx]){
		case ')':
			nParen++;
			atLeastOneParen = true;
			break;
		case '(':
			atLeastOneParen = true;
			nParen--;
			break;
		case '[':
		case ']':
			if(!atLeastOneParen){
				return	1;	// this can be an array initialisation
			}
		}
		idx--;
		if(atLeastOneParen && (0 == nParen)){
			break;
		}
	}

	while(0 <= idx){
		if(' ' != temporaryBuf[idx] && ('\n' != temporaryBuf[idx])){
			idx--;
			break;
		}
		idx--;
	}
	while(0 <= idx){
		if(!isalnum_(temporaryBuf[idx])){
			break;
		}
		idx--;
	}

	if(' ' == temporaryBuf[idx]){
		testPointBuf[idxBracket + 1] = 'F';
	}

	return	1;	// candidate(s) still can remain
}
#endif

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

int		insertTestPt(const char name[], const int fileNum)
{
#if	DEBUG
	fprintf(stderr, "Processing: %s ... ", name);
#endif

	char*	temporaryBuf;
	char*	sourceBuf;
	const	int		sourceLen = readFile(name, &sourceBuf, &temporaryBuf);
	if (sourceLen < 0) {
		return	-1;
	}

	pt_t	testPointBuf;

	while(replaceCrLfToLfSpace(temporaryBuf)){}
	replaceCrToLf(temporaryBuf, sourceLen);
	replaceTabToSpace(temporaryBuf, sourceLen);
	while(cancelCommentAndString(temporaryBuf)){}
	cancelSwitch(temporaryBuf);

	setTestPoints(temporaryBuf, testPointBuf, "if"     , '{', 'i');
	setTestPoints(temporaryBuf, testPointBuf, "else"   , '{', 'i');
	setTestPoints(temporaryBuf, testPointBuf, "for"    , '{', 'f');
	setTestPointsDoWhile(temporaryBuf, testPointBuf, 'd');
	setTestPoints(temporaryBuf, testPointBuf, "while"  , '{', 'w');
	setTestPoints(temporaryBuf, testPointBuf, "case"   , ':', 's');
	setTestPoints(temporaryBuf, testPointBuf, "default", ':', 's');
	while(setTestPointsFunc(temporaryBuf, testPointBuf, 'F')){}

	free(temporaryBuf);
	const	int		nTestPts = testPointBuf.size();

#if	DEBUG
	//debugWriteFile("temporaryBuf.c", temporaryBuf);
	//debugWriteFile("testPointBuf.c", testPointBuf);
#endif

	writeFile(name, sourceLen, fileNum, sourceBuf, testPointBuf);

	free(sourceBuf);
	testPointBuf.clear();

#if	DEBUG
	fputs("Finished.\n", stderr);
#endif

	return	nTestPts;
}

#ifndef	_MSC_VER

int	main(const int argc, const char*argv[])
{
	int		totalPts = 0;
	int		maxNPts = 0;
	int		nFileMax = 0;

	for(int i = 1; i < argc; i++){
		int		ret = insertTestPt(argv[i], i - 1);
		if (0 <= ret) {
			totalPts += ret;
			if (maxNPts < ret) {
				maxNPts = ret;
				nFileMax = i;
			}
		}
	}
	printf("Files: %d\n"
		"Max test pts in a file: %d (%s)\n"
		"Total test pts: %d\n",
		argc - 1, maxNPts, argv[nFileMax], totalPts);
	return	EXIT_SUCCESS;
}

#endif
