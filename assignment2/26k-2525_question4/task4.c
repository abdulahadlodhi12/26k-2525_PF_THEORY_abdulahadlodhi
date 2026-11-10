
#include <stdio.h>
int check_festival_days(int n_festival) {
	printf("Enter number of days of festival ");
	do {
		if(scanf("%d",&n_festival)!=1) {
			printf("Enter correct days from 1 till 7 ");
		}
		else if(n_festival<1||n_festival>7) {
			printf("Enter correct days from 1 till 7 ");
		}
	} while(n_festival<1 || n_festival>7);
	
	return n_festival;

}

int daily_processing(int days, int n_orders[],int sales[7][100],int highest_per_day[]) {
	int i=0,j=0,k=0,amount=0,total=0,average=0,highest=0,total_whole_festival=0;
	for(i=0; i<days; i++) {
		printf("Enter number of order recieved on day%d ",i+1);
		scanf("%d",&n_orders[i]);
		for(j=0; j<n_orders[i]; j++) {
			do {
				printf("Enter amount for Order%d ",i+1);
				if(scanf("%d",&amount)!=1) {
					printf("Enter correct amount of the product%d ",j+1);
				}
			} while(amount<0);
			sales[i][j]=amount;



		}
	}

	for(i=0; i<days; i++) {
		total=0;
		highest=0;
		for(j=0; j<n_orders[i]; j++) {
			total+=sales[i][j];
			average = total/n_orders[i];
			if(sales[i][j]>highest) {
				highest=sales[i][j];
			}
			highest_per_day[i] = highest;

		}
		printf("The total sales of day%d is %d\n ",i+1,total);
		printf("The average sale of day%d is %d\n",i+1,average);
		printf("The highest amount of the day%d is %d\n",i+1,highest);

	}


	return total;
}

int highest_sales(int total_details, int days,int n_orders[],int sales[7][100]) {
    int max=0,best_day=1,i=0,j=0,total=0,highest=0,total_whole_festival=0;
	for(i=0; i<days; i++) {
		total=0;
		highest=0;
		for(j=0; j<n_orders[i]; j++) {
			// the total sales of the festival_days
			total+=sales[i][j];
			total_whole_festival+=sales[i][j];
			if(total>max) {
				max = total;
				best_day =i+1;

			}
		}
	}
	printf("The total sales of whole festival is %d\n",total_whole_festival);
	printf("Day%d has the highest sale among all the days\n",best_day);
	
	return total_whole_festival;
}

void display_bar_chart(int total_details, int days, int highest[]){
    int i =0,j=0;
    for(i =0;i<days;i++){
        printf("Day%d ",i+1);
        int asterisks = (highest[i] / 100);
    for(j=0;j<asterisks;j++){
        printf("*");
        
        
    }
    printf(" %d\n",highest[i]);
    }
}

int main()
{
	int n_festival=0,orders[100],i=0;
	int sales[7][100];
	int highest_per_day[7];
	int festival_days = check_festival_days(n_festival);
	int daily_sales = daily_processing(festival_days,orders,sales,highest_per_day);
	int highest_sale = highest_sales(daily_sales,festival_days,orders,sales);
    display_bar_chart(daily_sales,festival_days,highest_per_day);


    return 0;
}