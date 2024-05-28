#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>  
#include <time.h>  

#define MAX 600
#define DISTANCE 2147483647
#define MPM 333

int final_count;
int passenger_count=0;
double array[MAX][MAX];
double rqtime[MAX];
double acctime[MAX];
int patrol[MAX][MAX][MAX];
int route[MAX];
int greedy_route[MAX];
double farthest_time[MAX];
int farthest_place[MAX];
double initial_diff[MAX];
int input[MAX];
double initial_cost=0;
double initial_rt=0;
double worst_time=0;
double initial_worst=0;
int bus_stops=0;
double total_rt=0;
double worst_ad=0;
double initial_worstad=0;
int count_error=0;

double calc_cost(int new_route[]){
    double total=0;
    int start, end;
    for (int i=1; i<passenger_count; i++){
        start=new_route[i-1];
        end=new_route[i];
        total+=array[start][end];
    }
    return total;
}

double calc_time(int new_route[]){
    double total=0;
    int start, end;
    double sq=0;
    double time=0;
    start=new_route[0];
    acctime[start]=rqtime[start];
    worst_time=0;
    total_rt=0;
    worst_ad=0;
    for (int i=1; i<passenger_count; i++){
        start=new_route[i-1];
        end=new_route[i];
        time=array[input[start]][input[end]]/MPM;
        acctime[end]=acctime[start]+time;
        if(acctime[end]<rqtime[end]){
            acctime[end]=rqtime[end];
        }
        else{
            if(acctime[end]-rqtime[end]>worst_ad){
                worst_ad=acctime[end]-rqtime[end];
            }
            total+=acctime[end]-rqtime[end];
        }
        //printf("%d %d %f %f\n", input[start], input[end], rqtime[end], acctime[end]);
    }
    for(int i=0; i<passenger_count-1; i+=2){
        int next;
        double best, current;
        start=new_route[i];
        next=new_route[i+1];
        best=array[input[start]][input[next]]/MPM;
        current=acctime[new_route[i+1]]-acctime[new_route[i]];
        if(current-best>worst_time){
            worst_time=current-best;
        }
        total_rt+=current-best;
        

    }
    total_rt/=passenger_count/2;
    //printf("\n");
    return total;
}

int minus_check(int new_route[]){
    int start=0, next=0;
    double best=0;
    double current=0;
    
    double temp=calc_time(new_route);
    for(int i=0; i<passenger_count-1; i+=2){
        current=acctime[i+1]-acctime[i];

        if(current<-0.00000001){
            return 1;
        }
    }
    return 0;
}

int count_patrols(int new_route[], int stops){
    int count=0;
    int start, next;

    for(int i=0; i<stops-1; i++){
        start=input[new_route[i]];
        next=input[new_route[i+1]];
        for(int j=0; j<stops; j++){
            if(patrol[start][next][j]!=1000){
                count++;
                
            }
            else{
                break;
            }
        }
    }
    printf("\033[1;31m%d\033[0m\n", count);
    return count;
}

void greedy(int first){
    int points[MAX];
    double nearest=0;

    for (int i=0; i<passenger_count; i++){
        points[i]=1;
    }

    int current=0;
    current=first;
    route[0]=first;

    for (int i=1; i<passenger_count; i++){
        nearest=DISTANCE;
        points[current]=0;

        for (int j=0; j<passenger_count; j++){
            if(array[current][j]<nearest&&points[j]==1){
                nearest=array[current][j];
                route[i]=j;
            }
        }
        current=route[i];
    }
    route[passenger_count]=first;

    for (int i=0; i<passenger_count; i++){
       printf("%d ", route[i]);
    }
    printf("\n");
    printf("cost= %f\n", calc_time(route));



}

void time_greedy(int first, int rq_count){
    int points[MAX];
    double next=0;
    int current=0;
    current=first;
    route[0]=first;

    for (int i=first; i<rq_count; i++){
        points[i]=1;
    }

    for (int i=first+1; i<rq_count; i++){
        next=DISTANCE;
        points[current]=0;

        for (int j=first; j<rq_count; j++){
            if(rqtime[j]<next&&points[j]==1){
                next=rqtime[j];
                route[i]=j;
                current=j;
            }
        }
        
    }

    for (int i=first; i<rq_count; i++){
        greedy_route[i]=route[i];
    }


    // for (int i=0; i<passenger_count; i++){
    //     printf("%d --> %f\n", input[route[i]], rqtime[route[i]]);
    // }
    // // }
    // printf("\n");
    // printf("cost= %f\n", calc_time(route));
    initial_cost=calc_time(greedy_route);
    initial_worst=worst_time;
    initial_rt=total_rt;
    initial_worstad=worst_ad;
    count_patrols(greedy_route, rq_count);

    if(initial_cost<-0.000001||initial_rt<-0.000001||initial_worst<-0.000001||initial_worstad<-0.000001){
        printf("%f %f %f %f\n", initial_cost, initial_rt, initial_worst, initial_worstad);
        exit(0);
    }

}

void my_init() {
    srand(time(NULL));
}

int get_random() {
    int random=rand() % passenger_count;
    while(random==0){
        random=rand() % passenger_count;
    }
    
    return random;
    
}

double get_random_perc() {
    return (double)rand() / (double)((unsigned)RAND_MAX + 1);
}

int *connect(int first, int second, int *new_route){
    int marker=0;
    int temp=0;
    int passenger_countup=0;
    
    for (int i=0; i<passenger_count; i++){
        new_route[i]=route[i];
        marker=i;
        if(new_route[i]==first){
            break;
        }
    }
    

    for (int i=0; i<passenger_count; i++){
        if(route[i]==second){
            temp=i;
            break;
        }
    }

    passenger_countup=marker+1;


    for (int i=temp; i>marker; i--){
        new_route[passenger_countup]=route[i];
        passenger_countup++;
    }

    for(int i=temp+1; i<passenger_count; i++){
        new_route[i]=route[i];
    }

    return new_route;

    
}

int *rand_route(int *new_route){
    int first=0;
    int second=0;

    first=get_random();
    second=get_random();
    while(second==first){
        second=get_random();
    }

    //printf("first=%d, second=%d\n", first, second);

    connect(first, second, new_route);

    return new_route;

}

void simulated_annealing(){
    double best=calc_time(greedy_route);
    double temp=100;
    int new[passenger_count];
    double new_cost=0;
    double p=0;
    double rand=0;
    int flag=0;
    printf("ogworst=%f\n", initial_worst);
    int temp_count=0;
    for(;;){
        if(temp<1){
            break;
        }
        rand_route(new);


        new_cost=calc_time(new);
        flag=minus_check(new);


        if(new_cost<=best&&flag==0){
            best=new_cost;
            for (int i = 0; i < passenger_count; i++) {     
                route[i] = new[i];     
            }
        }
        else if (flag==0){
            p=exp(-(new_cost-best)/temp);
            rand=get_random_perc();
            if(p>=rand){
                best=calc_time(new);
                for (int i = 0; i < passenger_count; i++) {     
                    route[i] = new[i];     
                }
            }
        }
        temp_count++;
        if(temp_count==15000){
            temp=temp*0.98;
            temp_count=0;
        }

    }

    if(total_rt>initial_worst+20&&count_error<50){
        printf("REDO, worst=%f\n", worst_time);
        count_error+=1;
        simulated_annealing();
    }
    else if(total_rt>initial_worst||count_error==50){
        for (int i = 0; i < passenger_count; i++) {     
            route[i] = greedy_route[i];   
            printf("%d ", input[route[i]]);  
        }
        best=calc_time(route);
        printf("GREEDY\n");
        printf("worst=%f\n", worst_time);

    }
    else{
        for (int i = 0; i < passenger_count; i++) {
            printf("%d ", input[route[i]]);  
        }
        best=calc_time(route);
        printf("worst=%f\n", worst_time);
    }
    
}

void find_farthest(){

     for (int i=0; i<bus_stops; i++){
        farthest_time[i]=0;
        farthest_place[i]=0;
     }

    for (int i=0; i<bus_stops; i++){
        for(int j=0; j<bus_stops; j++){
            if(array[i][j]>farthest_time[i]){
                farthest_time[i]=array[i][j];
                farthest_place[i]=j;
            }
        }
        printf("farthest of %d is %d\n", i, farthest_place[i]);

    }
    printf("\n\n");

}

void add_patrol(){
    double to=0, back=0;
    int start=0, next=0;
    int patrol_route_passenger_count=passenger_count;
    int patrol_route[MAX];
    double patrol_time[MAX];
    int j=0;
    double trash=0;

    for(int i=0; i<passenger_count; i++){
        patrol_route[i]=route[i];
        patrol_time[i]=acctime[route[i]];
        printf("%d --> %f\n", route[i], acctime[route[i]]);
    }
    
    for(int i=0; i<passenger_count-1; i++){
        start=route[i];
        next=route[i+1];
        to=farthest_time[start]/MPM;
        back=array[farthest_place[start]][next]/MPM;
        if(acctime[next]-acctime[start]>(to+back+2)&&farthest_place[start]!=next){
            printf("can add %d between %d and %d, to=%f, back=%f, sum=%f\n", farthest_place[start], start, next, to, back, to+back);
            patrol_route_passenger_count++;
 
            for (j = patrol_route_passenger_count-1; j>i+(patrol_route_passenger_count-passenger_count); j--){
                patrol_route[j] = patrol_route[j-1];
                patrol_time[j] = patrol_time[j-1];
            }
            patrol_route[j] = farthest_place[start];
            patrol_time[j] = acctime[start]+to;
        }
    }


    for(int i=0; i<patrol_route_passenger_count; i++){
        printf("%d ", patrol_route[i]);
    }
    printf("\n");
    for(int i=0; i<patrol_route_passenger_count; i++){
        printf("%d --> %f\n", patrol_route[i], patrol_time[i]);
    }
    
    printf("\n\n");

}

void add_patrol_time(){
    double to=0, back=0;
    int start=0, next=0;
    int patrol_route_count=passenger_count;
    int patrol_route[MAX];
    int patrol_queue[MAX];
    double patrol_time[MAX];
    int potential=0;
    int temp;
    int include_pass=0;
    int j=0;
    

    for(int i=0; i<passenger_count; i++){
        patrol_route[i]=input[route[i]];
        patrol_time[i]=acctime[route[i]];
    }

    for (int i=0; i<bus_stops; i++){
        patrol_queue[i]=i;
    }

    
    for(int i=0; i<passenger_count-1; i++){
        start=route[i];
        next=route[i+1];
        for(j=0; j<bus_stops; j++){
            if(patrol_queue[j]==input[start]){
                for(int x=j+1; x<bus_stops; x++){
                    patrol_queue[x-1]=patrol_queue[x];
                }
                patrol_queue[bus_stops-1]=input[start];
                break;
            }
        }
        
        for(j=0; j<bus_stops; j++){
            if(acctime[next]-rqtime[next]>0){
                for(int m=0; m<bus_stops; m++){
                    temp=patrol[input[start]][input[next]][m];
                    if(temp!=1000){
                        include_pass++;
                        for(j=0; j<bus_stops; j++){
                            if(patrol_queue[j]==temp){
                                for(int x=j+1; x<bus_stops; x++){
                                    patrol_queue[x-1]=patrol_queue[x];
                                }
                                patrol_queue[bus_stops-1]=temp;
                                break;
                            }
                        }
                    }
                    else{
                        break;
                    }
                }
                break;
            }
            potential=patrol_queue[j];
            to=array[input[start]][potential]/MPM;
            back=array[potential][input[next]]/MPM;
            if(acctime[next]-acctime[start]>(to+back+2)&&potential!=input[next]&&potential!=input[start]){
                for(int m=0; m<bus_stops; m++){
                    temp=patrol[input[start]][potential][m];
                    if(temp!=1000){
                        include_pass++;
                        for(j=0; j<bus_stops; j++){
                            if(patrol_queue[j]==temp){
                                for(int x=j+1; x<bus_stops; x++){
                                    patrol_queue[x-1]=patrol_queue[x];
                                }
                                patrol_queue[bus_stops-1]=temp;
                                break;
                            }
                        }
                    }
                    else{
                        include_pass++;
                        break;
                    }
                }
                for(int m=0; m<bus_stops; m++){
                    temp=patrol[potential][input[next]][m];
                    if(temp!=1000){
                        include_pass++;
                        for(int n=0; n<bus_stops; n++){
                            if(patrol_queue[n]==temp){
                                for(int x=j+1; x<bus_stops; x++){
                                    patrol_queue[x-1]=patrol_queue[x];
                                }
                                patrol_queue[bus_stops-1]=temp;
                                break;
                            }
                        }
                    }
                    else{
                        break;
                    }
                }
                patrol_route_count++;
                int m=0;
 
                for (m = patrol_route_count-1; m>i+(patrol_route_count-passenger_count); m--){
                    patrol_route[m] = patrol_route[m-1];
                    patrol_time[m] = patrol_time[m-1];
                }
                patrol_route[m] = potential;
                patrol_time[m] = acctime[start]+to;

                break;
            }
            if(j==(bus_stops-1)){
                for(int m=0; m<bus_stops; m++){
                    temp=patrol[input[start]][input[next]][m];
                    if(temp!=1000){
                        include_pass++;
                        for(j=0; j<bus_stops; j++){
                            if(patrol_queue[j]==temp){
                                for(int x=j+1; x<bus_stops; x++){
                                    patrol_queue[x-1]=patrol_queue[x];
                                }
                                patrol_queue[bus_stops-1]=temp;
                                break;
                            }
                        }
                    }
                    else{
                        break;
                    }
                }
                break;
            }
            

        }

        
    }

    
    j=0;
    for(int i=0; i<patrol_route_count; i++){
        if(patrol_route[i]!=input[route[i-j]]){
            printf("\033[1;31m%d --> %f\033[0m\n", patrol_route[i], patrol_time[i]);
            j++;
        }
        else{
            printf("%d --> %f, diff=%f\n", patrol_route[i], patrol_time[i], patrol_time[i]-rqtime[route[i-j]]);
        }
        
    }

    // count_patrols(patrol_route, patrol_route_count);

    printf("\t\033[1;31m%d\033[0m\n", include_pass);


}

void time_check(){
    int i=0;
    int check=0;
    int temp=0;

    // for(i=0; i<passenger_count; i++){
    //     printf("%f-%f=%f\n", acctime[route[i]], rqtime[route[i]], acctime[route[i]]-rqtime[route[i]]);
    // }
    for(i=0; i<passenger_count; i++){
        check=route[i];
        //printf("initial=%f ", initial_diff[check]);
        if(initial_diff[check]==DISTANCE){
            initial_diff[check]=acctime[check]-rqtime[check];
        }
        else{
            initial_diff[check]+=(acctime[check]-rqtime[check])-initial_diff[check];
        }
        // if(initial_diff[check]!=acctime[check]-rqtime[check]){
        //     printf("%d diff= %f\n", check, initial_diff[check]);
        // }
        // printf("\n");
    }
    
}




void fix_route(){
    int check=0;
    int temp=0;
    calc_time(route);
    double worst_marker=total_rt;
    int passenger_check=0;
    int flag=0;

    int test[passenger_count];
    if(worst_marker==0){
        return;
    }

    printf("fixing...\n");
    

    while(worst_marker>5){
        for(int i=0; i<passenger_count; i++){
            test[i]=route[i];
        }
        for(int i=0; i<passenger_count; i++){
            check=route[i];
            if(initial_diff[check]>5){
                temp=test[i-1];
                test[i-1]=check;
                test[i]=temp;
                temp=calc_time(test);
                flag=minus_check(test);

                if(total_rt<worst_marker&&flag==0){
                    worst_marker=total_rt;
                    for(int i=0; i<passenger_count; i++){
                        route[i]=test[i];
                    }
                    printf("WORST=%f\n\n", worst_marker);
                }
            }
        }
        passenger_check++;
        if(passenger_check==passenger_count){
            printf("muri lol\n");
            break;
        }
    }

    printf("\n");
    for(int i=0; i<passenger_count-1; i++){
        if(input[route[i]]==input[route[i+1]]&&fabs(acctime[i+1]-acctime[i])<5){
            printf("same%d %d\n", input[route[i]], input[route[i+1]]);
            printf("\t%d %d %f %f\n", input[route[i]], input[route[i+1]], acctime[i], acctime[i+1]);
            acctime[i]=acctime[i+1];
        }
    }
    
    temp=calc_time(route);
    // for(int i=0; i<passenger_count; i++){
    //     printf("%d--> %f\n", input[route[i]], acctime[route[i]]);
    // }
    

    printf("fixed\n");

    count_patrols(route, passenger_count);

}

void best_time_comp(){
   int start=0, next=0;
    double best=0;
    double current=0;
    int check_here=0;

    double temp=calc_time(route);
    for(int i=0; i<passenger_count-1; i+=2){
        start=route[i];
        next=route[i+1];
        best=array[input[start]][input[next]]/MPM;
        current=acctime[route[i+1]]-acctime[route[i]];
        if((current-best)>0.00000001){
            printf("from %d to %d: best=%f, current=%f-%f=%f, request=%f-%f, overtime=%f\n", start, next, best, acctime[i+1], acctime[i],current, rqtime[i+1], rqtime[i], current-best);
        }

    }
}

int main(void){
    int i=0;
    my_init();
    int first=0;
    int file_name=1;
    char temp[100];

    for (file_name=1; file_name<=50; file_name++){
        FILE *fptr;
        count_error=0;
        
        char path[100]="/Users/satokamimura/Desktop/INCOS_tests/two/tests/test";
        sprintf(temp, "%d.txt", file_name);
        strcat(path, temp);
        printf("%s", path);
        if ((fptr = fopen(path,"r")) == NULL){
            printf("Error! opening file");

            // Program exits if the file pointer returns NULL.
            exit(1);
        }

        fscanf(fptr, "%d ", &bus_stops);
        int new[bus_stops];
    

        for(i=0; i<bus_stops; i++){
            for(int j=0; j<bus_stops; j++){
                fscanf(fptr," %lf ", &array[i][j]);
            }
            initial_diff[i]=DISTANCE;
        }

        // for(i=0; i<bus_stops; i++){
        //     printf("\t");
        //     for(int j=0; j<bus_stops; j++){
        //         printf("%f ", array[i][j]);
        //     }
        //     printf("\n");
        // }

        for(i=0; i<bus_stops; i++){
            for(int j=0; j<bus_stops; j++){
                for(int m=0; m<bus_stops; m++){
                    fscanf(fptr," %d ", &patrol[i][j][m]);
                    if(patrol[i][j][m]==1000){
                        break;
                    }
                }
            }
            
        }


        
    clock_t t;
        t = clock();

        // find_farthest();

        fscanf(fptr, "%d ", &passenger_count);
        printf("%d\n", passenger_count);

        passenger_count*=2;
        for(int j=0; j<passenger_count; j+=2){
            fscanf(fptr,"%d %d %lf ", &input[j], &input[j+1], &rqtime[j+1]);
            if(rqtime[j+1]<(array[input[j]][input[j+1]]/MPM)){
                rqtime[j]=0;
                rqtime[j+1]=array[input[j]][input[j+1]]/MPM;
            }
            else{
                rqtime[j]=rqtime[j+1]-(array[input[j]][input[j+1]]/MPM);
            }
        }

        
        // printf("input ");
        for(int j=0; j<passenger_count; j++){
            // printf("%d ", input[j]);
            if(rqtime[j]<rqtime[first]){
                first=j;
            }
        }

        printf("\n");

        time_greedy(first, passenger_count);

        simulated_annealing();

        time_check();
        printf("tcdone\n");

        fix_route();

        printf("frdone\n");

        add_patrol_time();
        printf("aptdone\n");

        calc_time(route);

        while(worst_time<-0.000001||calc_time(route)<-0.000001||total_rt<-0.000001||worst_ad<-0.000001){
            time_greedy(first, passenger_count);
            simulated_annealing();
            time_check();
            printf("tcdone\n");
            fix_route();
            printf("frdone\n");
            add_patrol_time();
            printf("aptdone\n");
            calc_time(route);
        }
        printf("worst=%f\n", worst_time);
        printf("cost=%f\n", calc_time(route));
        printf("rt=%f\n", total_rt);
        printf("worstad=%f\n", worst_ad);

        printf("initial worst=%f\n", initial_worst);
        printf("initial cost=%f\n", initial_cost);
        printf("initial rt=%f\n", initial_rt);
        printf("initial worstad=%f\n", initial_worstad);

        // printf("%d done\n\n\n", i/6);



        best_time_comp();




        t = clock() - t;

        double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
        FILE *fp;
        fp = fopen("rt_ave.txt", "a");
        fprintf(fp, "%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", worst_time, calc_time(route), total_rt, worst_ad, initial_worst, initial_cost, initial_rt, initial_worstad);

    
        fclose(fp); //Don't forget to close the file when finished
    
        printf("runtime= %f \n", time_taken);


    }

    return 0;

}
