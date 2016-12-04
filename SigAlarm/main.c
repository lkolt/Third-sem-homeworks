#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <sched.h>

static int g_is_done;
int timer_getcounter() {
	struct itimerval cur_it;
	getitimer(ITIMER_REAL, &cur_it);
	return cur_it.it_value.tv_usec;
}

void timer_handler(void);

void signal_handler(int signal, siginfo_t *info, void *ctx) {
	printf("%s: in\n", __func__);
// CHECK begin
	timer_handler();
// CHECK begin
	printf("%s: out\n", __func__);
}

// TODO start

#include "que.h"

int time_from_start = 0;

typedef struct {
	long int interval;
	long int expires;
	void(*handler)(void);
} TIMER;

void timer_handler(void) {
	time_from_start++;

	TIMER* next = first();

	if (next != NULL) {
		next->expires--;

		if (next->expires <= 0) {
			while (1) {
                next = first();
                if (next == NULL){
                    break;
                }
                pop();
				next->handler();

				next->expires = time_from_start + next->interval;
				push(next->expires, next);

				TIMER* timer = first();
				if (timer != NULL) {
					timer->expires -= time_from_start;

					if (timer->expires > 0) {
						break;
					}
				}
			}
		}
	}
}

int timer_init(int sec, void(*hnd)(void)) {
    TIMER* new_timer = (TIMER*)malloc(sizeof(TIMER));
	new_timer->interval = sec;
	new_timer->expires = sec;
	new_timer->handler = hnd;

	push(sec, new_timer);
	return 0;
}

int timer_gettime() {
	return (1e6) * (time_from_start + 1) - timer_getcounter();
}

// TODO end

void hnd3(void) {
	printf("%s: called every 3 secs\n", __func__);
}

void hnd5(void) {
	printf("%s: called every 5 secs\n", __func__);
}

int main(int argc, char *argv[]) {

	struct sigaction sa = {
		.sa_sigaction = signal_handler,
	};
	sigemptyset(&sa.sa_mask);
	sigaction(SIGALRM, &sa, NULL);


	timer_init(3, hnd3);
	timer_init(5, hnd5);

	const struct itimerval setup_it = {
		.it_interval = { 1 /*sec*/, 0 /*usec*/},
		.it_value    = { 1 /*sec*/, 0 /*usec*/},
	};
	setitimer(ITIMER_REAL, &setup_it, NULL);

	int count = 0;
	while (!g_is_done) {
		if (!count--) {
			printf("cur_it = %d\n", timer_gettime());
			count = 1000000;
		}

		sched_yield();
	}
	return 0;
}
