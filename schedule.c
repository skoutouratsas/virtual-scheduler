/* schedule.c
 * This file contains the primary logic for the 
 * scheduler.
 */
#include "schedule.h"
#include "macros.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

#define NEWTASKSLICE (NS_TO_JIFFIES(100000000))
#define exp_burst_on 0




/* Local Globals
 * rq - This is a pointer to the runqueue that the scheduler uses.
 * current - A pointer to the current running task.
 */
struct runqueue *rq;
struct task_struct *current;


/* External Globals
 * jiffies - A discrete unit of time used for scheduling.
 *			 There are HZ jiffies in a second, (HZ is 
 *			 declared in macros.h), and is usually
 *			 1 or 10 milliseconds.
 */
extern long long jiffies;
extern struct task_struct *idle;

/*-----------------Initilization/Shutdown Code-------------------*/
/* This code is not used by the scheduler, but by the virtual machine
 * to setup and destroy the scheduler cleanly.
 */
 
 /* initscheduler
  * Sets up and allocates memory for the scheduler, as well
  * as sets initial values. This function should also
  * set the initial effective priority for the "seed" task 
  * and enqueu it in the scheduler.
  * INPUT:
  * newrq - A pointer to an allocated rq to assign to your
  *			local rq.
  * seedTask - A pointer to a task to seed the scheduler and start
  * the simulation.
  */
void initschedule(struct runqueue *newrq, struct task_struct *seedTask)
{
	seedTask->next = seedTask->prev = seedTask;
	newrq->head = seedTask;
	newrq->nr_running++;
	rq = newrq;
}

/* killschedule
 * This function should free any memory that 
 * was allocated when setting up the runqueu.
 * It SHOULD NOT free the runqueue itself.
 */
void killschedule()
{
	//free(rq);             den desmeusame
	return;
}


void print_rq () {
	struct task_struct *curr;
	
	printf("Rq: \n");
	curr = rq->head;
	if (curr)
		printf("%p", curr);
	while(curr->next != rq->head) {
		curr = curr->next;
		printf(", %p", curr);
	};
	printf("\n");
}

/*-------------Scheduler Code Goes Below------------*/
/* This is the beginning of the actual scheduling logic */

/* schedule
 * Gets the next task in the queue
 */
void schedule()
{
#ifdef exp_burst_on
	struct task_struct *curr;
	long long temp_waiting_in_rq,temp_burst,temp_exp_burst;
	



	
	current->need_reschedule = 0;  /*Always make sure to reset that, in case *
								   * we entered the scheduler because current*
								   * had requested so by setting this flag   */
	
	if (rq->nr_running == 1) {
		context_switch(rq->head);
	}
	else {	
		
		
		temp_waiting_in_rq = sched_clock();
		
		temp_burst = sched_clock() - current->got_cpu;
		
		temp_exp_burst = current->exp_burst;
		
		current->exp_burst = (temp_burst + 0.5 * current->exp_burst) / ( 1 + 0.5 );
		
		
		
		curr = min_burst ();
		
		if ( curr != current ) { 
			current->waiting_in_rq = temp_waiting_in_rq;
			current->burst = temp_burst;
			curr->got_cpu = sched_clock();
		}
		else {
		
			current->exp_burst = temp_exp_burst;
		}
		
		context_switch(curr);
	} 
#endif
#ifndef exp_burst_on
	struct task_struct *good;
	long long temp_waiting_in_rq,temp_burst;
	double temp_exp_burst;

	
	current->need_reschedule = 0; /* Always make sure to reset that, in case *
								   * we entered the scheduler because current*
								   * had requested so by setting this flag   */
	
	if (rq->nr_running == 1) {
		context_switch(rq->head);
	}
	else {	
		
		
		temp_waiting_in_rq = sched_clock();
		
		temp_burst = sched_clock() - current->got_cpu;
		
		temp_exp_burst = current->exp_burst;
		
		current->exp_burst = ((double)(temp_burst )+ 0.5 * current->exp_burst) / ( 1.0 + 0.5 );
		
		good = min_goodness();
		
		if (good!=current) { 
			
			current->waiting_in_rq = temp_waiting_in_rq;
			
			current->burst = temp_burst;
			
			good->got_cpu =sched_clock();
			
			
		}
		else {
			current->exp_burst = temp_exp_burst;
			
		}
		
		context_switch(good);
	}
#endif
}


/* sched_fork
 * Sets up schedule info for a newly forked task
 */
void sched_fork(struct task_struct *p)
{
	p->time_slice = 100;
	p->burst = 0;
	p->exp_burst = 0;
	p->waiting_in_rq = 0;
	
 	p->goodness=0;
}

/* scheduler_tick
 * Updates information and priority
 * for the task that is currently running.
 */
void scheduler_tick(struct task_struct *p)	//gia dieukolinsi mas ananeonoume tis times mesa stin schedule
{
	
	schedule();
}

/* wake_up_new_task
 * Prepares information for a task
 * that is waking up for the first time
 * (being created).
 */
void wake_up_new_task(struct task_struct *p)
{	
	p->next = rq->head->next;
	p->prev = rq->head;
	p->next->prev = p;
	p->prev->next = p;
	p->waiting_in_rq = sched_clock();
	
	
	 
		
	
	rq->nr_running++;
}

/* activate_task
 * Activates a task that is being woken-up
 * from sleeping.
 */
void activate_task(struct task_struct *p)
{
	p->next = rq->head->next;
	p->prev = rq->head;
	p->next->prev = p;
	p->prev->next = p;
	
	p->waiting_in_rq = sched_clock();
	
	rq->nr_running++;
}

/* deactivate_task
 * Removes a running task from the scheduler to
 * put it to sleep.
 */
void deactivate_task(struct task_struct *p)
{
	p->prev->next = p->next;
	p->next->prev = p->prev;
	p->next = p->prev = NULL; /* Make sure to set them to NULL *
							   * next is checked in cpu.c      */
	
	
	
	rq->nr_running--;
}


struct task_struct* min_burst() {
	
	struct task_struct *curr,*min_task;
	curr = rq->head;
	curr = curr->next;
	double min = curr->exp_burst ;
	min_task = curr;
	curr = curr->next;
	
	while(curr != rq->head) {
		if (curr->exp_burst < min){
			min = curr->exp_burst;
			min_task = curr;
		}
		curr = curr->next;
	}

	return min_task;
}

struct task_struct* max_waiting() {
	long long now;
	struct task_struct *curr,*max_task;
	curr = rq->head;
	curr = curr->next;

	now = sched_clock();

	max_task = curr;
	
	curr = curr->next;
	
	while(curr != rq->head) {
		
		if (now - curr->waiting_in_rq > now - max_task->waiting_in_rq){
			max_task = curr;
		}
		curr = curr->next;
	}
	
	
	return max_task;
}

struct task_struct* min_goodness() {		//upologizei kai briskei to task struct me to mikrotero goodness
	long long min_goodness;
	struct task_struct *curr,*min_task;
	struct task_struct *min,*max;
	
	curr = rq->head;
	curr = curr->next;
	
	min = min_burst ();
		
	max = max_waiting();
	curr->goodness = ((1.0 + curr->exp_burst) / (min->exp_burst+1.0)) * ((double)((sched_clock() - max->waiting_in_rq + 1.0)) / (double)((1.0 + sched_clock() - curr->waiting_in_rq)));
	printf("[Name:%s] [Goodness:%.20lf] [Exp_burst:%.20lf] [Waiting_in_rq:%lld]\n", curr->thread_info->processName, curr->goodness,curr->exp_burst,curr->waiting_in_rq);
	min_goodness = curr->goodness;
	min_task = curr;
	curr = curr->next;
	
	while(curr != rq->head) {
		curr->goodness = ((1.0 + curr->exp_burst) / (min->exp_burst+1.0)) * ((double)((sched_clock() - max->waiting_in_rq + 1.0)) / (double)((1.0 + sched_clock() - curr->waiting_in_rq)));
		printf("[Name:%s] [Goodness:%.20lf] [Exp_burst:%.20lf] [Waiting_in_rq:%lld]\n", curr->thread_info->processName, curr->goodness,curr->exp_burst,curr->waiting_in_rq);
		if (curr->goodness < min_goodness) {
			min_goodness = curr->goodness;
			min_task = curr;
		}
		curr = curr->next;
	}
	
	return min_task;
	
}


