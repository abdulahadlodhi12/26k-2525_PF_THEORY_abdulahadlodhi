#include<stdio.h>
int main(){
	// Checking how many reservation does the user wants:
	int reservations;
	int booked_count=0;
	int spare_count=0;
	int customers =0;
		// initialize seats cinema have:
	int rows,seat_per_row;
	printf("\nStaff: Enter How many rows does the cinema have ");
	scanf("%d",&rows);
	printf("Staff: Enter number of seats per row ");
	scanf("%d",&seat_per_row);
	int cinema[rows][seat_per_row];
		
	for(int i=0;i<rows;i++){
		for(int j=0;j<seat_per_row;j++){
			cinema[i][j] = 0;
		}
	}
	
	printf("Enter 0 if you are single person || Enter 1 if you are complete gathering");
	scanf("%d",&reservations);
	int start =0;
	
	
	if(reservations==0){
	// initialize which seat does the user want:
	printf("Booking %d ",start+1);
	int user_seat_row, user_seat_column;
	printf("\nEnter User Prefered Seat row ");
	scanf("%d",&user_seat_row);
	printf("Enter user prefered column number ");
	scanf("%d",&user_seat_column);
	
	// Now turning extra seat to user booked seat and counting the books and spare seats:
	for(int i=0;i<rows;i++){
		for(int j=0;j<seat_per_row;j++){
			if(cinema[user_seat_row][user_seat_column] == 0){
				cinema[user_seat_row][user_seat_column]=1;
			}
		
		}
	}
	
	
	

}else if(reservations == 1){
	printf("How many tickets you want to buy for cinema in one row");
	scanf("%d",&customers);
	if(customers == 1){
		printf("If you are alone so please select 1st option");
	}
	else{
 int row_found = 0;
            int mid = rows / 2;
           // findind lower and upper half of mid martix
            for(int d = 0; d < rows; d++){
                int area_to_check[2];
                int count_to_check = 0;
                
                int r1 = mid - d;
                if(r1 >= 0 && r1 < rows){
                    area_to_check[count_to_check++] = r1;
                }
                
                int r2 = mid + d;
                if(d > 0 && r2 >= 0 && r2 < rows){
                    area_to_check[count_to_check++] = r2;
                }
                
                for(int idx = 0; idx < count_to_check; idx++) {
                    int i = area_to_check[idx];
                    int free_in_this_row = 0;
                    
                    // Count free seats in this specific row
                    for(int j = 0; j < seat_per_row; j++) {
                        if(cinema[i][j] == 0) {
                            free_in_this_row++;
                        }
                    }
                    
                    // If this row has enough space for the entire matrc
                    if(free_in_this_row >= customers) {
                        int assigned = 0;
                        for(int j = 0; j < seat_per_row && assigned < customers; j++) {
                            if(cinema[i][j] == 0) {
                                cinema[i][j] = 1;
                                assigned++;
                            }
                        }
                        
                        printf("\nSuccessfully booked %d seats together in Row %d!\n", customers, i);
                        row_found = 1;
                        break; 
                    }
                }
                
                if(row_found) {
                    break;
                }
            }
            
            if(!row_found) {
                printf("\nSorry! No single row has %d spare seats available together.\n", customers);
            }
}
// end reservation loop



			for(int i=0;i<rows;i++){
			printf("\n");
			for(int j=0;j<seat_per_row;j++){
				printf("%d",cinema[i][j]);
			}	
		}
	
	
	

	for(int i=0;i<rows;i++){
		int spare_in_this_row = 0;
		int booked_in_this_row = 0;
			for(int j=0;j<seat_per_row;j++){
				if(cinema[i][j] == 1){
				booked_count++;
				booked_in_this_row++;			
				}
				else if(cinema[i][j] == 0){
					spare_count++;
					spare_in_this_row++;
				}	
		}
//		printf("\nNo Of Seats Spare in Row %d are: %d ",i+1,spare_in_this_row);
//		printf("\nNo Of Seats Occupied in Row %d are: %d ",i+1,booked_in_this_row);	
}
	printf("\nTotal Booked Seats Till Now are %d ",booked_count);
	printf("\nTotal Spare Seats Till Now are %d ",spare_count);	
	if(start == 0){
		printf("\nBooking Completed");
	}
}

}
