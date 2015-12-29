/* External definitions for single-server queueing system, fixed run length. */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include "lcgrand.h"  /* Header file for random-number generator. */
#include "simplerandom-c.h"

#define Q_LIMIT 100  /* Limit on queue length. */
#define BUSY      1  /* Mnemonics for server's being busy */
#define IDLE      0  /* and idle. */
#define LUNCHTIME -1 // server go eat lunch
#define TRUE 1
#define FALSE 0
#define EVENT_KIND 7

/* Simulation Clock */
float sim_time;

/* State variables */
// Arrival time for customer
float time_arrival[Q_LIMIT + 1];
// Number of customer in queue
int   num_in_q = 0;
// Server status
int   server_status = 0;

/* Variables for event list */
// Type number of events in the list
int   num_events = 0;
// Occur time for event I
float time_next_event[EVENT_KIND]; 
// Type of next event
int   next_event_type = 0;

/* Statistical variables */
// Total delay time
float total_of_delays;
// Number of customer processed
int   num_custs_delayed = 0;
// Number of customer balking
int   num_custs_balking = 0;

/* Input and control variables */
// Mean time to arrive
float mean_interarrival;
// Mean time to serve
float mean_service;
// Time to end simulation
float time_end;
// Total customers
int total_customers;

/* Output variables */
// Average delay
float avgdel;
// Average NIQ
float avgniq;
// Area under number-in-queue function
float area_num_in_q;
// Area under server-busy indicator function
float area_server_status;
// Compute time since last event to update area_num_in_q
float time_last_event;

/* Complex M/M/1 variables*/
// Time when the server start trying to take a lunch break
float time_server_try_eat;
// Time when the server forced to take a lunch break
float time_server_must_eat;
// Time when the server actually take a lunch break
float time_server_actual_eat;
// Flag to check if server has taken a lunch break
bool  server_eat_done;
FILE  *infile, *outfile, *outfile2;

int my_seed = 0;
clock_t start_simu, finish_simu;

SimpleRandomCong_t      cong_state;
SimpleRandomSHR3_t      shr3_state;
SimpleRandomMWC1_t      mwc1_state;
SimpleRandomMWC2_t      mwc2_state;
SimpleRandomMWC64_t     mwc64_state;
SimpleRandomKISS_t      kiss_state;
SimpleRandomKISS2_t     kiss2_state;
SimpleRandomLFSR113_t   lfsr113_state;
SimpleRandomLFSR88_t    lfsr88_state;


unsigned int cong_function(void);
unsigned int shr3_function(void);
unsigned int mwc1_function(void);
unsigned int mwc2_function(void);
unsigned int mwc64_function(void);
unsigned int kiss_function(void);
unsigned int kiss2_function(void);
unsigned int lfsr113_function(void);
unsigned int lfsr88_function(void);

void  initialize(void);
bool  timing(void);
void  arrive(void);
void  depart(void);
void  report(void);
void  close_server(void);
void  server_go_eat(void);
void  server_try_eat(void);
void  server_back(void);
void  update_time_avg_stats(void);
float expon(float mean);


main()  /* Main function. */
{
    /* Open input and output files. */

    infile  = fopen("mm1alt.in",  "r");
    outfile = fopen("mm1alt.out", "w");
    outfile2 = fopen("mm1data.out", "w");
    
    /* Specify the number of events for the timing function. */

    num_events = EVENT_KIND - 1;
    
    /* Read input parameters. */

    fscanf(infile, "%f %f %f", &mean_interarrival, &mean_service, &time_end);


    /* Initialise all generators */
    simplerandom_cong_seed(&cong_state, UINT32_C(2051391225));
    simplerandom_shr3_seed(&shr3_state, UINT32_C(3360276411));
    simplerandom_mwc1_seed(&mwc1_state, UINT32_C(12345), UINT32_C(65437));
    simplerandom_mwc2_seed(&mwc2_state, UINT32_C(12345), UINT32_C(65437));
    simplerandom_mwc64_seed(&mwc64_state, UINT32_C(7654321), UINT32_C(521288629));
    simplerandom_kiss_seed(&kiss_state, UINT32_C(2247183469), UINT32_C(99545079), UINT32_C(3269400377), UINT32_C(3950144837));
    simplerandom_kiss2_seed(&kiss2_state, UINT32_C(2247183469), UINT32_C(99545079), UINT32_C(3269400377), UINT32_C(3950144837));
    simplerandom_lfsr113_seed(&lfsr113_state, 0, 0, 0, 0);
    simplerandom_lfsr88_seed(&lfsr88_state, 0, 0, 0);


    for (my_seed = 1; my_seed < 110; my_seed ++) {
        start_simu = clock();

        /* Write report heading and input parameters. */

        printf("output!!");

        fprintf(outfile, "Single-server queueing system with fixed run");
        fprintf(outfile, " length\n\n");
        fprintf(outfile, "Mean interarrival time%11.3f minutes\n\n",
                mean_interarrival);
        fprintf(outfile, "Mean service time%16.3f minutes\n\n", mean_service);
        fprintf(outfile, "Length of the simulation%9.3f minutes\n\n", time_end);

        /* Initialize the simulation. */

        initialize();

        // Write report content - start simulation
        //fprintf(outfile2, "Start simulation: %16.3f \n",
        //    sim_time);

        /* Run the simulation until it terminates after an end-simulation event
           (type 3) occurs. */
        do
        {
            /* Determine the next event. */

            if(!timing())
                break;

            /* Update time-average statistical accumulators. */

            update_time_avg_stats();

            /* Invoke the appropriate event function. */

            switch (next_event_type)
            {
                case 1:
                    arrive();
                    break;
                case 2:
                    depart();
                    break;
                case 3:
                    close_server();
                    break;
                case 4:
                    server_try_eat();
                    break;
                case 5:
                    server_go_eat();
                    break;
                case 6:
                    server_back();
                    break;
            }

        /* If the event just executed was not the end-simulation event (type 3),
           continue simulating.  Otherwise, end the simulation. */

        } while (next_event_type != 3);
    
        fprintf(outfile, "============================================\n\n");

        /* Compute and write estimates of desired measures of performance. */

        fprintf(outfile2, "101, %16.3f, %16.3f\n",
                sim_time, total_of_delays / num_custs_delayed);

        fprintf(outfile2, "102, %16.3f, %16.3f\n",
                area_num_in_q / sim_time, area_server_status / sim_time);

        fprintf(outfile2, "103, %16.3d, %16.3d\n",
                num_custs_delayed, num_custs_balking);

    }
    fclose(infile);
    fclose(outfile);
    fclose(outfile2);

    return 0;
}


void initialize(void)  /* Initialization function. */
{
    /* Initialize the simulation clock. */

    sim_time = 0.0;

    /* Initialize the state variables. */

    server_status   = IDLE;
    num_in_q        = 0;
    time_last_event = 0.0;

    /* Initialize the statistical counters. */

    num_custs_delayed  = 0;
    total_of_delays    = 0.0;
    area_num_in_q      = 0.0;
    area_server_status = 0.0;
    num_custs_balking  = 0;

    /* Initialize the complex M/M/1 variables. */
    time_server_try_eat     = 180;
    time_server_must_eat    = 240;
    time_server_actual_eat  = 0.0;
    server_eat_done         = FALSE;

    /* Initialize event list.  Since no customers are present, the departure
       (service completion) event is eliminated from consideration.  The end-
       simulation event (type 3) is scheduled for time time_end. */

    time_next_event[1] = sim_time + expon(mean_interarrival);
    time_next_event[2] = 1.0e+30;
    time_next_event[3] = time_end;
    // Add lunch event
    time_next_event[4] = time_server_try_eat;
    time_next_event[5] = time_server_must_eat;
    time_next_event[6] = 1.0e+30;
}


bool timing(void)  /* Timing function. */
{
    int   i;
    float min_time_next_event = 1.0e+29;

    next_event_type = 0;

    /* Determine the event type of the next event to occur. */

    for (i = 1; i <= num_events; ++i) {
        if (time_next_event[i] < min_time_next_event) {
            min_time_next_event = time_next_event[i];
            next_event_type     = i;
        }
    }

    /* Check to see whether the event list is empty. */

    if (next_event_type == 0)
    { 
    
        /* The event list is empty, so stop the simulation */

        fprintf(outfile, "\nEvent list empty at time %f", sim_time);

        /* Compute and write estimates of desired measures of performance. */

        fprintf(outfile, "\n\nAverage delay in queue%11.3f minutes\n\n",
                total_of_delays / num_custs_delayed);

        fprintf(outfile, "Average number in queue%10.3f\n\n",
                area_num_in_q / sim_time);

        fprintf(outfile, "Server utilization%15.3f\n\n",
                area_server_status / sim_time);

        fprintf(outfile, "Number of delays completed%7d\n\n",
                num_custs_delayed);
        
        fprintf(outfile, "Number of customer balking%7d\n\n",
            num_custs_balking);

        //exit(1);
        return FALSE;
    }

    /* Check to see whether RMIN < TIME */

    if (min_time_next_event < sim_time)
    {

        /* The closest next event time is before current simulation time
            , so stop the simulation */

        fprintf(outfile, "\n\nTime error! %f\n\n", sim_time);

        //exit(1);
        return FALSE;
    }

    /* The event list is not empty, so advance the simulation clock. */

    sim_time = min_time_next_event;
    return TRUE;
}


void arrive(void)  /* Arrival event function. */
{
    float delay;

    /* Schedule next arrival. */

    time_next_event[1] = sim_time + expon(mean_interarrival);

    /* Check to see whether server is busy. */

    if (server_status == BUSY) {

        /* Server is busy, so increment number of customers in queue. */

        ++num_in_q;

        /* Check to see whether an overflow condition exists. */

        if (num_in_q > Q_LIMIT) {

            /* The queue has overflowed, so stop the simulation. */

            fprintf(outfile, "\nOverflow of the array time_arrival at");
            fprintf(outfile, " time %f", sim_time);
            //exit(2);
        }

        /* There is still room in the queue, so store the time of arrival of the
           arriving customer at the (new) end of time_arrival. */

        time_arrival[num_in_q] = sim_time;
        fprintf(outfile2, "1, %16.3f, %14d \n",
            sim_time, num_in_q);
    }

    else if (server_status == LUNCHTIME) {
        if ((sim_time - time_server_actual_eat < 10 && lcgrand(2) < 0.75) ||
            (sim_time - time_server_actual_eat < 20 && lcgrand(2) < 0.5) ||
            (sim_time - time_server_actual_eat < 30 && lcgrand(2) < 0.25))
        {
            fprintf(outfile2, "7, %16.3f, %14d \n",
                sim_time, num_in_q);

            num_custs_balking++;
        }
        else {
            /* Server is eating lunch, so increment number of customers in queue. */

            ++num_in_q;

            /* Check to see whether an overflow condition exists. */

            if (num_in_q > Q_LIMIT) {

                /* The queue has overflowed, so stop the simulation. */

                fprintf(outfile, "\nOverflow of the array time_arrival at");
                fprintf(outfile, " time %f", sim_time);
                //exit(2);
            }

            /* There is still room in the queue, so store the time of arrival of the
               arriving customer at the (new) end of time_arrival. */

            time_arrival[num_in_q] = sim_time;
            fprintf(outfile2, "1, %16.3f, %14d \n",
                sim_time, num_in_q);
        }
    }

    else {

        /* Server is idle, so arriving customer has a delay of zero.  (The
           following two statements are for program clarity and do not affect
           the results of the simulation.) */

        delay            = 0.0;
        total_of_delays += delay;

        /* Increment the number of customers delayed, and make server busy. */

        ++num_custs_delayed;
        server_status = BUSY;

        /* Schedule a departure (service completion). */

        time_next_event[2] = sim_time + expon(mean_service);
        fprintf(outfile2, "1, %16.3f, %14d \n",
            sim_time, num_in_q);
    }
}


void depart(void)  /* Departure event function. */
{
    int   i;
    float delay;

    /* Check to see whether the queue is empty. */

    if (num_in_q == 0) {

        /* The queue is empty so make the server idle and eliminate the
           departure (service completion) event from consideration. */

        server_status      = IDLE;
        time_next_event[2] = 1.0e+30;
        if (!server_eat_done && sim_time <= time_server_must_eat 
            && sim_time >= time_server_try_eat) {
            server_go_eat();
        }
    }

    else {

        /* The queue is nonempty, so decrement the number of customers in
           queue. */

        --num_in_q;

        /* Compute the delay of the customer who is beginning service and update
           the total delay accumulator. */

        delay            = sim_time - time_arrival[1];
        total_of_delays += delay;

        /* Increment the number of customers delayed, and schedule departure. */

        ++num_custs_delayed;
        time_next_event[2] = sim_time + expon(mean_service);

        /* Check to see whether the queue is empty. */
        if (num_in_q <= 0) {

            return;
        }

        /* Move each customer in queue (if any) up one place. */

        for (i = 1; i <= num_in_q; ++i)
            time_arrival[i] = time_arrival[i + 1];
    }
    fprintf(outfile2, "2, %16.3f, %14d \n",
        sim_time, num_in_q);
}


void server_go_eat(void) // server go to eat lunch
{
    // set server status to go to eat lunch
    server_status = LUNCHTIME;

    // the server_go_eat event is eliminated from consideration
    time_next_event[5] = 1.0e+30;

    if (num_in_q != 0) {
        // the server will go after finish the current customer
        --num_in_q;

        fprintf(outfile2, "5, %16.3f, %14d \n",
            time_next_event[2], num_in_q);

        time_server_actual_eat = time_next_event[2];
        time_next_event[2] += 30;

    } else {
        fprintf(outfile2, "5, %16.3f, %14d \n",
            sim_time, num_in_q);

        time_server_actual_eat = sim_time;
        time_next_event[2] = sim_time + expon(mean_service) + 30;
    }

    // server will come back after 30 minutes
    time_next_event[6] = time_server_actual_eat + 30;
}


void server_try_eat(void) // server try to go to eat lunch
{
    fprintf(outfile2, "4, %16.3f, %14d \n",
        sim_time, num_in_q);

    // server first try to go to eat at 12 o'clock (180min)
    if (num_in_q == 0)
        server_go_eat();

    // the server_try_eat event is eliminated from consideration
    time_next_event[4] = 1.0e+30;
}

void server_back(void)
{
    // change server status
    if (num_in_q == 0) {
        server_status = IDLE;
    } else {
        server_status = BUSY;
    }
    server_eat_done = TRUE;

    fprintf(outfile2, "6, %16.3f, %14d \n",
        sim_time, num_in_q);

    // the server_back event is eliminated from consideration
    time_next_event[6] = 1.0e+30;
}

void close_server(void)
{
    // finish all the customer wait in queue and close
    time_next_event[3] = 1.0e+30;
    time_next_event[1] = 1.0e+30;
    do
    {
        /* Determine the next event. */

        timing();

        /* Update time-average statistical accumulators. */

        update_time_avg_stats();

        /* Invoke the appropriate event function. */

        depart();

    /* If the event just executed was not the end-simulation event (type 3),
       continue simulating.  Otherwise, end the simulation. */

    } while (num_in_q != 0);
}

void report(void)  /* Report generator function. */
{   
    /* Compute and write estimates of desired measures of performance. */

    fprintf(outfile, "\n\nAverage delay in queue%11.3f minutes\n\n",
            total_of_delays / num_custs_delayed);

    fprintf(outfile, "Average number in queue%10.3f\n\n",
            area_num_in_q / sim_time);

    fprintf(outfile, "Server utilization%15.3f\n\n",
            area_server_status / sim_time);

    fprintf(outfile, "Number of delays completed%7d\n\n",
            num_custs_delayed);

    fprintf(outfile, "Number of customer balking%7d\n\n",
            num_custs_balking);
}


void update_time_avg_stats(void)  /* Update area accumulators for time-average
                                     statistics. */
{
    float time_since_last_event;

    /* Compute time since last event, and update last-event-time marker. */

    time_since_last_event = sim_time - time_last_event;
    time_last_event       = sim_time;

    /* Update area under number-in-queue function. */

    area_num_in_q      += num_in_q * time_since_last_event;

    /* Update area under server-busy indicator function. */

    area_server_status += server_status * time_since_last_event;
}


float expon(float mean)  /* Exponential variate generation function. */
{
    /* Return an exponential random variate with mean "mean". */
    if (my_seed == 101)
        return -mean * log((cong_function() >> 8 | 1) / 16777216.0);
    else if (my_seed == 102)
        return -mean * log((shr3_function() >> 8 | 1) / 16777216.0);
    else if (my_seed == 103)
        return -mean * log((mwc1_function() >> 8 | 1) / 16777216.0);
    else if (my_seed == 104)
        return -mean * log((mwc2_function() >> 8 | 1) / 16777216.0);
    else if (my_seed == 105)
        return -mean * log((mwc64_function() >> 8 | 1) / 16777216.0);
    else if (my_seed == 106)
        return -mean * log((kiss_function() >> 8 | 1) / 16777216.0);
    else if (my_seed == 107)
        return -mean * log((kiss2_function() >> 8 | 1) / 16777216.0);
    else if (my_seed == 108)
        return -mean * log((lfsr113_function() >> 8 | 1) / 16777216.0);
    else if (my_seed == 109)
        return -mean * log((lfsr88_function() >> 8 | 1) / 16777216.0);
    else
        return -mean * log(lcgrand(my_seed));
}


unsigned int cong_function(void)
{
    return simplerandom_cong_next(&cong_state);
}

unsigned int shr3_function(void)
{
    return simplerandom_shr3_next(&shr3_state);
}

unsigned int mwc1_function(void)
{
    return simplerandom_mwc1_next(&mwc1_state);
}

unsigned int mwc2_function(void)
{
    return simplerandom_mwc2_next(&mwc2_state);
}

unsigned int mwc64_function(void)
{
    return simplerandom_mwc64_next(&mwc64_state);
}

unsigned int kiss_function(void)
{
    return simplerandom_kiss_next(&kiss_state);
}

unsigned int kiss2_function(void)
{
    return simplerandom_kiss2_next(&kiss2_state);
}

unsigned int lfsr113_function(void)
{
    return simplerandom_lfsr113_next(&lfsr113_state);
}

unsigned int lfsr88_function(void)
{
    return simplerandom_lfsr88_next(&lfsr88_state);
}

