/* File:     A1.c
 * Author:   Anran Zhang   B00747547
 * Date:     2020/05/14
 * Version:  1.0
 *
 * Purpose:  This program is a simulation of Bob’s Barber Boutique
 *           and return the wait times of customers.
 *           It can take in commandline arguments '-randomize' and/or the number
 *           of days to simulate.
 * Notes:    rand() does not guarantee randomness so the chances may deviate.
 *           Floating points are rounded to 2 decimals in the output.
 *           Should not have a upper limit for the number of days unless out of
 *           memory.
 *           Command used to compile: gcc A1.c -o A1.out -lm
 */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//define the min and max time for a haircut.
#define  MIN_TIME 10
#define  MAX_TIME 20

struct Customer
{
    int arrive_time;
    int haircut_time;
    int wait_time;
};
float result[4];

// function declarations
int getRand(int min, int max);
struct Customer getCustomer(int time);
float * simulation();
int isInt(char arg[]);
void printResults(int n,float * output);
float * multiday(int n);


int main(int argc,char* argv[])
{
    //check the arguments and do something if they do exist
    if(argc>=2 && strcmp(argv[1],"-randomize")==0)
    {
        printf("You choose to randomize the output.\n");
        srandom(time(NULL));
    }
    else
    {
        if (argc>=2 && isInt(argv[1])!=1)
        {
            printf("%s%s%s","Invalid commandline argument '",argv[1],"'.\n ");
            return 1;
        }
        if (argc>=2 && isInt(argv[1])==1)
        {
            printf("%s%s%s", "You choose to simulate ", argv[1], " days.\n");
            int n = atoi(argv[2]);
            multiday(n);
            return 0;
        }
    }
    if(argc>2)
    {
        if (isInt(argv[2])!=1)
        {
            printf("%s%s%s","Invalid commandline argument '",argv[2],"'.\n ");
            return 1;
        }
        else
        {
            printf("%s%s%s", "You choose to simulate ", argv[2], " days.\n");
            //run simulation for the specified # of days.
            int n = atoi(argv[2]);
            multiday(n);

            return 0;
        }
    }
    //run for one day.
    printf("Simulating one day by default.\n\n");
    float * output;
    output = simulation();
    printResults(1,output);

    return 0;
}
//This function runs a simulation for the nth day.
float * simulation()
{
    int time = 0;
    int customers_inside = 0;
    int total_waiting_time = 0;
    int customers_left = 0;

    struct Customer customers[40];
    //Generate the initial 3 customers and add them to array
    customers[0] = getCustomer(time);
    customers[1] = getCustomer(time);
    customers[2] = getCustomer(time);
    customers_inside=3;
    int count = 3;
    //the first customer is served immediately
    int current = 0;
    int current_serve = 0;
    customers[current].wait_time = 0;
    /*
    printf("-------Log-------\n");
    printf("Time 0:\t\tCustomer 0, cut time %d\n",customers[0].haircut_time);
    printf("Time 0:\t\tCustomer 1, cut time %d\n",customers[1].haircut_time);
    printf("Time 0:\t\tCustomer 2, cut time %d\n",customers[2].haircut_time);
    printf("Time %d:\t\tCurrent customer: %d starts,waited %d "
           "minutes\n",time,current,customers[current].wait_time);
    printf("Time %d:\t\tcurrent finishes at time %d\n",time,
           current_serve+customers[current].haircut_time);
    */
    while (count <40 && time <480)
    {
        if(time > 0)
        {
            if(rand()%20==0)//a new customer will come in
            {
                /*printf("Time %d:\tCustomer came in, %d inside.\n",time,
                        customers_inside);
                */
                float chance_to_leave = 20*customers_inside;
                bool leave = (rand() % 100) < chance_to_leave;
                if(leave)
                {
                    customers_left++;
                    //printf("\t\t\tCustomer left.\n");
                }
                else
                {
                    count++;
                    customers[count-1] = getCustomer(time);
                    customers_inside++;
                    /*
                    printf("\t\t\tCustomer %d stays, cut time: %d, now %d"
                           " waiting\n",count-1,customers[count].haircut_time,
                           customers_inside-1);
                    */
                    if(customers_inside==1)
                    {
                        current++;
                        current_serve = time;
                        total_waiting_time += time - customers[current]
                                .arrive_time;
                        customers[current].wait_time = time - customers[current]
                                .arrive_time;
                        /*printf("Time %d:\tCurrent customer: %d starts,came "
                               "in at time %d, waited %d minutes\n",time,current,
                               customers[current].arrive_time,
                               customers[current].wait_time);
                        printf("Time %d:\thair cut time %d, current'll finish at "
                               "time %d\n",time,customers[current].haircut_time,
                               current_serve+customers[current].haircut_time);
                        */
                    }
                }
            }
            //if a customer is finishing or current does not exist

            if(current_serve+customers[current].haircut_time==time) //done
            {
                /*
                printf("Time %d:\tCurrent customer: %d finishes\n",time,
                       current);
                */
                customers_inside--;
                //next customer is served immediately if any
                if(count-1!=current)
                {
                    current++;
                    current_serve = time;
                    total_waiting_time += time - customers[current]
                            .arrive_time;
                    customers[current].wait_time = time - customers[current]
                            .arrive_time;
                    /*
                    printf("Time %d:\tCurrent customer: %d starts,came "
                           "in at time %d, waited %d minutes\n",time,current,
                           customers[current].arrive_time,
                           customers[current].wait_time);
                    printf("Time %d:\thair cut time %d, current'll finish at "
                           "time %d\n",time,customers[current].haircut_time,
                           current_serve+customers[current].haircut_time);
                    */
                }
            }

        }
        time++;
    }
    float avg_waiting = (float)total_waiting_time/(float)(current+1);
    //calculate sample standard deviation

    float sdv=0;
    for (int i = 0; i < current; ++i)
    {
        float tmp = 0;
        tmp+=pow(customers[i].wait_time-avg_waiting,2);
        sdv+=tmp;
    }
    if(current<=count-1)
        sdv=sqrt(sdv / current);
    else
        sdv=sqrt(sdv / (count-1));

    float not_served = count-current-1;//-1->none,0->no waiting 1 in chair
    if(not_served==-1)
        not_served = 0;

    //clear and repopulate output


    result[0] = customers_left;
    result[1] = avg_waiting;
    result[2]= sdv;
    result[3]= not_served;
    //printf("Total number of customers served: %d\n",current+1);
    return result;
}
//generates a random customer
struct Customer getCustomer(int time)
{
    int haircut_time = getRand(MIN_TIME,MAX_TIME);
    struct Customer c;
    c.arrive_time = time;
    c.haircut_time = haircut_time;
    return c;
}
//This function generates a random number from min to max inclusive.
int getRand(int min, int max)
{
    return min + rand() % (max+1 - min);
}
//this functions checks if the input is a number
int isInt(char arg[])
{
    int ret;
    int n_days;
    char c;
    ret = sscanf(arg, "%d%c", &n_days, &c);
    return ret;
}
//prints the result for the nth day.
void printResults(int n, float output[4])
{
    printf("Results of day %d:\n# of customers walked away: %.f\nAvg"
           " waiting time: %.2f minutes\nSample standard derivation: %.2f\n",n,
           output[0],output[1],output[2]);
    printf("# of those who waited but didn't get served: %.f.\n",
           output[3]);
}
//run the simulation for n days and process the results
float * multiday(int n)
{
    /*  min average wait time                               -> tmp[0]
     *  max average wait time                               -> tmp[1]
     *  min standard deviation of wait time                 -> tmp[2]
     *  max standard deviation of wait time                 -> tmp[3]
     *  avg daily average wait time                         -> tmp[4]
     *  standard deviation of daily avg                     -> tmp[5]
     *  avg daily standard deviation                        -> tmp[6]
     *  standard deviation of daily standard deviation      -> tmp[7]
     */
    float * temp[n];//hold the results for simulations
    float tmp[8]={0,0,0,0,0,0,0,0};//stores processed data, default: 0
    float * output = &tmp;
    for (int i = 0; i < n; ++i)
    {
        temp[i] = simulation();
        //populate the results of daily min and max
        if (tmp[0]==0)
        {
            tmp[0] = temp[i][1];
            tmp[1] = temp[i][1];
            tmp[2] = temp[i][2];
            tmp[3] = temp[i][2];
        }
        else
        {
            if (temp[i][1]<tmp[0])
                tmp[0] = temp[i][1];
            if (temp[i][1]>tmp[1])
                tmp[1] = temp[1][1];
            if (temp[i][2]<tmp[2])
                tmp[2] = temp[i][2];
            if (temp[i][2]>tmp[3])
                tmp[3] = temp[i][2];
        }
        //standard derivation can't be calculated until all simulation is done
        tmp[4] +=temp[i][1]/(float)n;
        tmp[6] +=temp[i][2]/(float)n;

        //print results for each day
        printResults(i+1,temp[i]);
    }
    float sdv_avg = 0;
    float sdv_sdv = 0;
    float avg_wait = tmp[4];
    float avg_sdv = tmp[6];
    //calculate standard deviation of daily avg
    for (int i = 0; i < n; ++i)
    {
        float x = 0;
        x+=pow(temp[i][1]-avg_wait,2);
        sdv_avg += x;
    }
    sdv_avg=sqrt(sdv_avg / n);

    //calculate standard deviation of daily standard deviation
    for (int i = 0; i < n; ++i)
    {
        float x = 0;
        x+=pow(temp[i][2]-avg_sdv,2);
        sdv_sdv += x;
    }
    sdv_sdv=sqrt(sdv_sdv / n);
    tmp[5] = sdv_avg;
    tmp[7] = sdv_sdv;

    printf("\nSummary:\n");
    printf("Daily avg waiting time min: %.2f,max: %.2f\n"
           "Daily standard deviation of wait time min: %.2f,max: %.2f\n"
           "Average daily waiting time: %.2f,standard deviation: %.2f\n"
           "Average of daily standard deviations: %.2f\n"
           "Standard deviation of daily standard deviations: %.2f\n",
           tmp[0],tmp[1],tmp[2],tmp[3],tmp[4],tmp[5],tmp[6],tmp[7]);
    return output;
}
