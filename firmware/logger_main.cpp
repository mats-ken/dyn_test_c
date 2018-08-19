#include	<stdio.h>
#include	<stdlib.h>
#include	<stdint.h>
#include	<unistd.h>

#include	<wiringPi.h>

#include	"gpioPorts.h"

#define	DEBUG	1


#define	CLOCK_LIMIT	(1000000UL * 3600)
#if UINT32_MAX < CLOCK_LIMIT
  #error
#endif // UINT32_MAX < CLOCK_LIMIT

#define	BUF_SIZE	(100 * 1000 * 1000)


int	i = 0;
uint32_t	gClock = 0;

typedef	struct {
	uint32_t	time;
	uint32_t	data;
}	buf_t;

typedef	struct {
	int			writeIdx;
	int			readIdx;
	int			phase;
	int			errors;
}	bufferingParam_t;

buf_t*	gBuf;
bufferingParam_t	bufferingParam = { 0, 0, 0, 0 };



void	bufferLog4by4_0(void)
{
	int		data = 0;

	data  = digitalRead(OUT1);	data <<= 1;
	data |= digitalRead(OUT2);	data <<= 1;
	data |= digitalRead(OUT3);	data <<= 1;
	data |= digitalRead(OUT4);

	gBuf[bufferingParam.writeIdx].time = gClock;
	gBuf[bufferingParam.writeIdx].data |= (data << 12);
}

void	bufferLog4by4_1(void)
{
	int		data = 0;

	data  = digitalRead(OUT1);	data <<= 1;
	data |= digitalRead(OUT2);	data <<= 1;
	data |= digitalRead(OUT3);	data <<= 1;
	data |= digitalRead(OUT4);

	gBuf[bufferingParam.writeIdx].data |= (data << 8);
}

void	bufferLog4by4_2(void)
{
	int		data = 0;

	data  = digitalRead(OUT1);	data <<= 1;
	data |= digitalRead(OUT2);	data <<= 1;
	data |= digitalRead(OUT3);	data <<= 1;
	data |= digitalRead(OUT4);

	gBuf[bufferingParam.writeIdx].data |= (data << 4);
}

void	bufferLog4by4_3(void)
{
	int		data = 0;

	data  = digitalRead(OUT1);	data <<= 1;
	data |= digitalRead(OUT2);	data <<= 1;
	data |= digitalRead(OUT3);	data <<= 1;
	data |= digitalRead(OUT4);

	gBuf[bufferingParam.writeIdx].data |= data;
}
#if	0
void	bufferLog4by4(const int edge)
{
	int		data = 0;

	data  = digitalRead(OUT1);	data <<= 1;
	data |= digitalRead(OUT2);	data <<= 1;
	data |= digitalRead(OUT3);	data <<= 1;
	data |= digitalRead(OUT4);
	switch (bufferingParam.phase) {
	case	0:
		if ((0 == edge) || (0 == data & 8)){
			bufferingParam.errors++;
			bufferingParam.phase = 0;
			return;
		}
		gBuf[bufferingParam.writeIdx].time = gClock;
		gBuf[bufferingParam.writeIdx].data = data;
		gBuf[bufferingParam.writeIdx].data <<= 4;
		bufferingParam.phase++;
		break;
	case	1:
		if (1 == edge){
			bufferingParam.errors++;
			bufferingParam.phase = 0;
			return;
		}
		gBuf[bufferingParam.writeIdx].data |= data;
		gBuf[bufferingParam.writeIdx].data <<= 4;
		bufferingParam.phase++;
		break;
	case	2:
		if ((0 == edge) || (0 != data & 8)){
			bufferingParam.errors++;
			bufferingParam.phase = 0;
			return;
		}
		gBuf[bufferingParam.writeIdx].data |= data;
		gBuf[bufferingParam.writeIdx].data <<= 4;
		bufferingParam.phase++;
		break;
	case	3:
		if (1 == edge){
			bufferingParam.errors++;
			bufferingParam.phase = 0;
			return;
		}
		gBuf[bufferingParam.writeIdx].data |= data;
		bufferingParam.phase = 0;
		bufferingParam.writeIdx++;
		break;
	default:
		bufferingParam.phase = 0;
		bufferingParam.errors++;
		break;
	
	}
}
#endif
int main(const int argc, const char*const argv[])
{
	int		wp = wiringPiSetupGpio();
	if (wp < 0) {
		fprintf(stderr, "wp setup failure.\n");
		return	EXIT_FAILURE;
	}

	gBuf = (buf_t*)calloc(sizeof(buf_t), BUF_SIZE);
	if (NULL == gBuf) {
		fprintf(stderr, "calloc() failure.\n");
		return	EXIT_FAILURE;
	}

	pinMode(CLK , INPUT);
	pinMode(OUT1, INPUT);
	pinMode(OUT2, INPUT);
	pinMode(OUT3, INPUT);
	pinMode(OUT4, INPUT);
	//wiringPiISR(0, INT_EDGE_BOTH, &foo);
	for(i = 0; i < 7200*2; i++){
		while (0 == digitalRead(17)) { usleep(1); }	bufferLog4by4_0();
		while (0 != digitalRead(17)) { usleep(1); }	bufferLog4by4_1();
		while (0 == digitalRead(17)) { usleep(1); }	bufferLog4by4_2();
		while (0 != digitalRead(17)) { usleep(1); }	bufferLog4by4_3();
		bufferingParam.writeIdx++;
	}
	uint32_t		n;
	uint32_t	data, dataPrev = 0;
	for(n = 0; n < bufferingParam.writeIdx; n++){
		dataPrev = data;
		data  =  gBuf[n].data       &  7;
		data |= (gBuf[n].data >> 1) & (7 << 3);
		data |= (gBuf[n].data >> 2) & (7 << 6);
		data |= (gBuf[n].data >> 3) & (7 << 9);
		int		answer = n;
			printf("er %6d must be %6d ", data, answer);
		data  =  gBuf[n].data;
		//printf("[%x %d]", data, data);
		if(1 || answer != data){
			int		i;
			for(i = 0; i < 8; i++){
				printf("%d", (data >> (15 - i)) & 1);
			}
			putchar(' ');
			for(i = 0; i < 8; i++){
				printf("%d", (data >> (7 - i)) & 1);
			}
			putchar(' ');

			for(i = 0; i < 8; i++){
				printf("%d", (answer >> (15 - i)) & 1);
			}
			putchar(' ');
			for(i = 0; i < 8; i++){
				printf("%d", (answer >> (7 - i)) & 1);
			}
			puts("");
		}else{
			//printf("ok %d must be %d ", data, n);
		}
		
	}
	puts("");
	printf("n: %d, errors:%d\n", bufferingParam.writeIdx, bufferingParam.errors);

return 0;
	while (1) {
		if (CLOCK_LIMIT <= ++gClock) {
			gClock = 0;
		}
		usleep(1);
#if	DEBUG
		if (0 == gClock % 1000) {
			printf("%d %d %d %u\n", i, data, bufferingParam.phase, gBuf[bufferingParam.writeIdx].data);
			int	n;
			for(n = 0; n < bufferingParam.writeIdx; n++)
				printf("[%d]", gBuf[n].data);
		}
#endif	// DEBUG
	}
	return	EXIT_SUCCESS;
}

int main2(void)
{
#if	1
#else
	int*data = (int*)malloc(sizeof(int) * 153000000);
	printf("%d\n", sizeof(int) * 153);
	if (NULL == data) {
		fprintf(stderr, "malloc() failure\n");
		return 1;
	}

	puts("started");
	while (1) {
		printf("%d", digitalRead(GPIO17));

		//sleep(1);
	}

	for (i = 0; i < 15300000; i++) {
		data[i] = digitalRead(GPIO17);
	}

	for (i = 0; i < 100; i++) {
		putchar('0' + data[i]);
	}
#endif
	return	EXIT_SUCCESS;
}
