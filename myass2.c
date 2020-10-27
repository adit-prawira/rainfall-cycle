/* Solution to comp20005 Assignment 2, 2019 semester 1.

   Authorship Declaration:

   (1) I certify that the program contained in this submission is completely
   my own individual work, except where explicitly noted by comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students,
   or by non-students as a result of request, solicitation, or payment,
   may not be submitted for assessment in this subject.  I understand that
   submitting for assessment work developed by or in collaboration with
   other students or non-students constitutes Academic Misconduct, and
   may be penalized by mark deductions, or by other penalties determined
   via the University of Melbourne Academic Honesty Policy, as described
   at https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will
   I do so until after the marks are released. I understand that providing
   my work to other students, regardless of my intention or any undertakings
   made to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring
   service, or drawing the attention of others to such services and code
   that may have been made available via such a service, may be regarded
   as Student General Misconduct (interfering with the teaching activities
   of the University and/or inciting others to commit Academic Misconduct).
   I understand that an allegation of Student General Misconduct may arise
   regardless of whether or not I personally make use of such solutions
   or sought benefit from such actions.

   Signed by: [Aditya Prawira, Student_ID:874615]
   Dated:     [May 11th, 2019]

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* numerical constant */
#define MAXMONTH 12   /* months in a year */
#define MAXDATA 3000  /* 3000 months for 250 years worth data */  
#define MAXROWS 24    /* max rows in diagram */

/* character constant */
#define NOTVALID 'N'  /* invalidate data */
#define VALIDATE 'Y'  /* validate data */
#define STAR '*'      /* identification for invalidate data */
#define NODATA '?'    /* identification for non-existing data */
#define NEWLINE '\n'  


/* string constant for the x-axis of the plot */
#define LINE \
    "+-----+----+----+----+----+----+----+----+----+----+----+----+\n\
       Jan  Feb  Mar  Apr  May  Jun  Jul  Aug  Sep  Oct  Nov  Dec "

/* numerical integer and character of months in a year array */
char *month_char[] = {"...", "Jan", "Feb", "Mar", "Apr", "May", 
                     "Jun","Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

int month_int[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

/* structs declaration */
typedef struct{
   int initial_year;
   int final_year;
}year_range_t;

typedef struct{
   double precip;
   int month;
   int year;
   char valid;
}data_t;

typedef struct{
   double r_precip;
   char r_valid;
   int r_month;
}resultant_data_t;

/* function prototypes */
int mygetchar();
int get_char();
int check_year(data_t DATA[], int n);
int not_in_year(int A[], int m, int n);
int round_to_scale(int prcp, int scales);

double average_bigger(double MEAN[], double max);
double find_max(resultant_data_t R_DAT[], int maxmonth);
double tau(double R_PRCP[], char V_PM[], int initial_year, 
   int final_year, int i_year, int ndata);

void years(int Y[], int i_year, int n);
void print_stage1(resultant_data_t R_DAT[][MAXMONTH], int Y[], int nyear);
void stage3(resultant_data_t R_DAT[][MAXMONTH], year_range_t Y_R[], 
   int D_EXST[], int i_year, int nyear);
void stage2(resultant_data_t R_DAT[][MAXMONTH], int D_INT[][MAXMONTH], 
   year_range_t Y_R[], int D_EXST[], int Y[], double MEAN[], int nyear);
void round_array(int ROUND_DATA[], int ROUND_MEAN[], 
   resultant_data_t R_DAT[], double MEAN[], int scale);
void data_in_year(int DATE[][MAXMONTH], double PRCP[][MAXMONTH], 
   char VALID[][MAXMONTH], int MPY[], int Y2[], 
   data_t DATA[], int data, int nyear);
void resultant_data(int D[][MAXMONTH], resultant_data_t R_DAT[][MAXMONTH], 
   double P_DAT[][MAXMONTH], char V_DAT[][MAXMONTH], 
   int MPY[], int m[], int nyear);
void draw_plot(resultant_data_t R_DAT[][MAXMONTH], int ROUND_DATA[], 
   int ROUND_MEAN[], double MEAN[], int scale, int yyear, 
   int last_digit, int max_prcp);
void stage4(resultant_data_t R_DAT[][MAXMONTH], int ROUND_DATA[], 
   int ROUND_MEAN[], double MEAN[], int last_digit, int index, 
   int year, int maxmonth);

/* main program computes and calling 
function protoypes to compute STAGE1 - STAGE4 */
int
main(int argc, char *argv[]){
   year_range_t YEAR_RANGE[MAXDATA];
   data_t DATA[MAXDATA];
   resultant_data_t RESULTANT_DATA[MAXDATA][MAXMONTH];

   char valid;
   char VALID_DATA[MAXDATA][MAXMONTH];

   int i, last_digit, n = 0;
   int stat_num, year, month, post;
   int data = 0, chars, nyear, i_year, index_year;
   int ROUND_DATA[MAXDATA], ROUND_MEAN[MAXDATA];
   int DATE_INTERVAL[MAXDATA][MAXMONTH], DATE[MAXDATA][MAXMONTH];
   int DATA_EXIST[MAXDATA], YEARS[MAXDATA], MONTHS_PER_YEAR[MAXDATA];

   double precip;
   double MEAN[MAXDATA], PRECIP[MAXDATA][MAXMONTH];

   /*STAGE1 output*/
   /* main loop to read data line and store in months, 
   years, and monthly rain precipitation in an struct array */
   while((chars = get_char()) != EOF){
      while(scanf("%d,%d,%d,%d,%lf,%c", &post, &stat_num, &year, 
         &month, &precip, &valid) == 6){
         data += 1;
         DATA[n].year = year; DATA[n].month = month;
         DATA[n].precip = precip; DATA[n].valid = valid;
         n += 1;
      }
   }
   printf("S1, site number %06d, %d datalines in input\n", 
      stat_num, data);

   nyear = check_year(DATA, data);
   i_year = year - nyear;

   /* computes interval of year in csv data */
   years(YEARS, i_year, nyear);
   data_in_year(DATE, PRECIP, 
      VALID_DATA, MONTHS_PER_YEAR, YEARS, 
      DATA, data, nyear);
   resultant_data(DATE, RESULTANT_DATA, PRECIP, 
      VALID_DATA, MONTHS_PER_YEAR, 
      month_int, nyear);
   print_stage1(RESULTANT_DATA, YEARS, nyear);
   /*END OF STAGE1*/

   /* STAGE2 output*/
   stage2(RESULTANT_DATA, DATE_INTERVAL, YEAR_RANGE, DATA_EXIST, YEARS, 
      MEAN, nyear);
   /* END OF STAGE2 */

   /* STAGE3 output*/
   stage3(RESULTANT_DATA, YEAR_RANGE, 
      DATA_EXIST, i_year, nyear);
   /* END OF STAGE3 */

   /* STAGE4 output*/
   /* print plot depending on how many years and what 
   year the program should show */
   for(i= 1; i<argc; i++){
      index_year = atoi(argv[i]) - i_year;
      /* taking last 2 digits of a year */
      last_digit = atoi(argv[i]) % 100;
      /* find maximum value in a month of the year */
      stage4(RESULTANT_DATA, ROUND_DATA, 
         ROUND_MEAN, MEAN, last_digit, index_year, 
         atoi(argv[i]), MAXMONTH);
   }
   /* END OF STAGE4 */
   printf("Ta daa!\n");

   /* and Done */
   return 0;
}
 
int
mygetchar() {
   int c;
   while ((c=getchar())=='\r') {
   }
   return c;
}

int
get_char(){
   int ch;
   while((ch = mygetchar())== NEWLINE){
   }
   return ch;
}

/* compute how many years occur in the data */
int
check_year(data_t DATA[], int n){
   int i;
   int nyear = 0;
   for(i = 0; i< n-1; i++){
      if(DATA[i].month >= DATA[i+1].month){
         nyear += 1;
      }
   }
   return nyear;
}
/* compute array of years in data */
void
years(int Y[], int i_year, int n){
   int i;
   int year = i_year;
   for(i = 0; i<=n; i++){
      Y[i] = year;
      year += 1;
   }
}

/* compute array of current month, monthly precipitation 
and character type of data validation */
void
data_in_year(int DATE[][MAXMONTH], double PRCP[][MAXMONTH], 
   char VALID[][MAXMONTH], int MPY[], int Y2[], 
   data_t DATA[], int data, int nyear){
   int i, n = 0, cols = 0, rows = 0;
   while(rows<nyear){
      for(i = 0; i<data; i++){
         if(DATA[i].year == Y2[rows]){
            DATE[rows][cols] = DATA[i].month;
            PRCP[rows][cols] = DATA[i].precip;
            VALID[rows][cols] = DATA[i].valid;
            cols += 1;
         }else{
            MPY[n] = cols;
            cols = 0;
            i -= 1; n+=1; rows += 1;
         }
      }
   }
   MPY[n] = cols;
}

/* check of what month does not exist in the data per year */
int
not_in_year(int A[], int m, int n){
   int i;
   int in_year = 0;
   for(i = 0; i<n; i++){
      if(m == A[i]){
         in_year += 1;
      }
   }
   if(in_year){
      return 0;
   }
   return 1;
}

/* Fill struct of resultant data */
void
resultant_data(int D[][MAXMONTH], resultant_data_t R_DAT[][MAXMONTH], 
   double P_DAT[][MAXMONTH], char V_DAT[][MAXMONTH], 
   int MPY[], int m[], int nyear){

   int i, j, rows = 0, cols = 0;
   while(rows <= nyear){
      j = i = 0;
      /* Indicates non-existence data as 0 and type of validation as question mark */
      while(i < MAXMONTH){
         if(not_in_year(D[rows], m[i], MPY[rows])){
            R_DAT[rows][cols].r_month = 0;
            R_DAT[rows][cols].r_precip = 0;
            R_DAT[rows][cols].r_valid = NODATA;
            cols += 1;
         }else{
            R_DAT[rows][cols].r_month = month_int[i];
            R_DAT[rows][cols].r_precip = P_DAT[rows][j];
            R_DAT[rows][cols].r_valid = V_DAT[rows][j];
            j += 1;
            cols += 1;
         }
         i+=1;
      }
      rows += 1;
      cols = 0;
   }
}

/* printing stage 1 */
void
print_stage1(resultant_data_t R_DAT[][MAXMONTH], int Y[], int nyear){
   int i, j;
   for(i = 0; i<=nyear; i++){
      printf("S1, %d:", Y[i]);
      for(j = 0; j< MAXMONTH; j++){
         if(R_DAT[i][j].r_valid==NOTVALID){

            /* print name of month with star for invalidate data */
            printf("%4s%1c", month_char[ R_DAT[i][j].r_month ], STAR);
         }else{
            printf("%4s ", month_char[ R_DAT[i][j].r_month ]);
         }
      }
      printf("\n");
   }
   printf("\n");
}

/* Compute Mean value of validated and 
existed monthly precipitation */
void
stage2(resultant_data_t R_DAT[][MAXMONTH], int D_INT[][MAXMONTH], 
   year_range_t Y_R[], int D_EXST[], int Y[], double MEAN[], int nyear){

   int i, m, n;
   double data_exist = 0.0, tot_precip = 0.0;
   m = n = 0;
   while(n < MAXMONTH){
      for(i= 0; i<=nyear; i++){
         if(R_DAT[i][n].r_month != 0 || R_DAT[i][n].r_valid != NODATA){
            if(R_DAT[i][n].r_valid == NOTVALID){
               tot_precip += 0;
            }
            tot_precip += R_DAT[i][n].r_precip;
            data_exist += 1;
            D_INT[n][m] = Y[i];
            m+=1;
         }
      }
      /* value of mean in array is zero for 
      non_existing and invalidated data */
      if(data_exist == 0){
         MEAN[n] = 0;
         D_EXST[n] = 0;
      }else{
         D_EXST[n] = data_exist;
         MEAN[n] = tot_precip/data_exist;
      }

      /* compute interval of years to where 
      monthly precipitation start and end in overall*/
      Y_R[n].initial_year = D_INT[n][0];
      Y_R[n].final_year = D_INT[n][m-1];
      printf("S2, %s,%4d values", month_char[n+1], D_EXST[n]);
      if(D_EXST[n] != 0){
         printf(", %d-%d, mean of %5.1lfmm\n", Y_R[n].initial_year, 
            Y_R[n].final_year, MEAN[n]);
      }else{
         printf("\n");
      }
      n+=1;

      /* start new indexing for the 
      next year's monthly data */
      m = 0;
      tot_precip = data_exist = 0;
   }
   printf("\n");
}

/* compute Kendall's tau of existed 
and validated monthly precipitation */
double
tau(double R_PRCP[], char V_PM[], int initial_year, 
   int final_year, int i_year, int ndata){

   int i,j, start, end;
   double sum, resultant_sum;
   double coef, tau;
   coef = 1/((ndata*(ndata-1.0))/2.0);
   sum = resultant_sum = 0.0;
   start = initial_year - i_year;
   end = final_year - i_year;
   for(i = start; i<end; i++){
      if(R_PRCP[i]==0 && V_PM[i] != VALIDATE){
         i+=1;
      }
      for(j = i+1; j<=end; j++){
         if(R_PRCP[j] == 0&&V_PM[j]!=VALIDATE){
            j +=1;
         }
         if(R_PRCP[i]>R_PRCP[j]){
            sum -= 1.0;
         }else if(R_PRCP[i]==R_PRCP[j]){
            sum += 0.0;
         }else{
            sum += 1.0;
         }
      }  
      resultant_sum += sum;
      sum = 0.0;
   }
   tau = coef*resultant_sum;
   return tau;
}

/* print STAGE3's ouput calling Kendall's tau function */
void
stage3(resultant_data_t R_DAT[][MAXMONTH], year_range_t Y_R[], 
   int D_EXST[], int i_year, int nyear){
   int i,j;
   double RAIN_PERMONTH[MAXDATA][MAXMONTH];
   double TAU[MAXDATA];
   char VALID_PERMONTH[MAXDATA][MAXMONTH];
   for(i = 0; i< MAXMONTH; i++){
      for(j = 0; j<=nyear; j++){
         RAIN_PERMONTH[i][j] = R_DAT[j][i].r_precip;
         VALID_PERMONTH[i][j] = R_DAT[j][i].r_valid;
         TAU[i] = tau(RAIN_PERMONTH[i], VALID_PERMONTH[i], Y_R[i].initial_year, 
            Y_R[i].final_year, i_year, D_EXST[i]);
      }
      printf("S3, %s,%4d values", month_char[i+1], D_EXST[i]);
      if(D_EXST[i]>=2){
         printf(", %d-%d, tau of %5.2lf\n", Y_R[i].initial_year, 
            Y_R[i].final_year, TAU[i]);
      }else{
         printf("\n");
      }
   }
   printf("\n");
}

/* Find the highest monthly precipitation in a chosen year */
double
find_max(resultant_data_t R_DAT[], int maxmonth){
   int i;
   double max;
   max = R_DAT[0].r_precip;
   for(i = 1; i< MAXMONTH; i++){
      if(max<R_DAT[i].r_precip){
         max = R_DAT[i].r_precip;
      }
   }
   return max; 
}

/* Round to the nearest scale computed */
int
round_to_scale(int prcp, int scales){
   return ((prcp + (scales -1)) / scales )* scales;
}

/* comparing max precipitation to each element in MEAN array. */
/* return mean value if there exist a mean value greater than 
max precipitation, else return the original max precipitation*/
double
average_bigger(double MEAN[], double max){
   int i; 
   for(i = 0; i< MAXMONTH; i++){
      if(MEAN[i]>max){
         max = MEAN[i];
      }
   }
   return max;
}
/* compute array of rounded precipitation data 
and mean data to the nearest scale calculated */
void
round_array(int ROUND_DATA[], int ROUND_MEAN[], 
   resultant_data_t R_DAT[], double MEAN[], int scale){
   int i; 
   for(i = 0; i<MAXMONTH; i++){

      /* if monthly precipitation is grater than 0 
      and less than 1, scaled the value as the lowest 
      value in the graph as data is exist */
      if(R_DAT[i].r_precip>0 && R_DAT[i].r_precip<1 && round(R_DAT[i].r_precip)==0){
         ROUND_DATA[i] = scale;
      }else{
         ROUND_DATA[i] = round_to_scale(round(R_DAT[i].r_precip), scale);
      }
      /* same for mean value */
      if(MEAN[i]>0 && MEAN[i]<1 && round(MEAN[i])==0){
         ROUND_MEAN[i] = scale;
      }else{
         ROUND_MEAN[i] = round_to_scale(round(MEAN[i]), scale);
      }
   }
}
/* compute plot */
void
draw_plot(resultant_data_t R_DAT[][MAXMONTH], int ROUND_DATA[], 
   int ROUND_MEAN[], double MEAN[], int scale, int yyear, 
   int last_digit, int max_prcp){
   int i, j;
   for(i = 0; i < MAXROWS; i++){
      printf("%4d |", max_prcp);
      for(j = 0; j<MAXMONTH; j++){
         /* if the rounded to the nearest scale value is less 
         than the original value add the scale value in order to match 
         scaled values in y-axis */
         if(ROUND_DATA[j] < R_DAT[yyear][j].r_precip){
            ROUND_DATA[j] += scale;
         }
         if(ROUND_MEAN[j] < MEAN[j]){
            ROUND_MEAN[j] += scale;
         }
         if(ROUND_DATA[j] >= max_prcp){
            if(ROUND_DATA[j] >= ROUND_MEAN[j]){

               /* printf both average and data value if 
               the monthly data exceeds average */
               if(ROUND_MEAN[j] == max_prcp){
                  printf(" *%02d*", last_digit);
               }else{
                  printf("  %02d ", last_digit);
               }
               ROUND_DATA[j]-=scale;
            }else{
               printf("  %02d ", last_digit);
            } 
         /* Plot average even though data is 0 */  
         }else if(ROUND_DATA[j] == 0){
            if(ROUND_MEAN[j] == max_prcp){
               printf(" ****");
            }else{
               printf("     ");
            }
         }else{
            if(ROUND_MEAN[j] == max_prcp){
               printf(" ****");
            }else{
               printf("     ");
            }
         }
      }
      printf("\n");
      /* stop plotting y-axis if it reaches 0 */
      max_prcp -= scale;
      if(max_prcp == 0){
         break;
      }
   }
   printf("%4d %s\n\n", max_prcp, LINE);
}
void 
stage4(resultant_data_t R_DAT[][MAXMONTH], int ROUND_DATA[], 
   int ROUND_MEAN[], double MEAN[], int last_digit, int index, 
   int year, int maxmonth){

   int scale_fac_int, max_prcp;
   double max, scale_fac_d;
   max = find_max(R_DAT[index], maxmonth);
   max = average_bigger(MEAN, max);
   /* round double value into integer */
   max_prcp = round(max);

   /* compute scale */
   scale_fac_d = max/24;
   scale_fac_int = round(scale_fac_d);

   /* add value of scale by 1 if rounded scale 
   value is smaller than actual scale */
   if(scale_fac_int < scale_fac_d){
      scale_fac_int += 1;
   }
   /* compute the highest y-axis value */
   max_prcp = round_to_scale(max_prcp, scale_fac_int);
   printf("S4, %d max is %5.1lf, scale is %d\n", year, 
      max, scale_fac_int);
   if(max_prcp < max){
      max_prcp += scale_fac_int;
   }
   round_array(ROUND_DATA, ROUND_MEAN, 
      R_DAT[index], MEAN, scale_fac_int);

   /* plot y-axis of plot in descending order by value of scale up to 0 */
   draw_plot(R_DAT, ROUND_DATA, 
      ROUND_MEAN, MEAN, scale_fac_int, index, 
      last_digit, max_prcp);
}
/* programming is fun */

