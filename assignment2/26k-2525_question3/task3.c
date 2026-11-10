#include <stdio.h>

void input_temp(int n_sensors, int n_temp, float init_temp[n_sensors][n_temp]) {
  int i = 0, j = 0;
  
  for(i = 0; i < n_sensors; i++) {
    printf(" ");
    for(j = 0; j < n_temp; j++) {
      printf("Enter sensor %d hourly readings ", i + 1);
      scanf("%f", &init_temp[i][j]);
    }
  }
}

int find_highest_avg_sensor(int n_sensors, int n_temp, float init_temp[n_sensors][n_temp], float *highest_avg) {
    int best_sensor = 1;
    float max_avg = -999999.0;

    for (int i = 0; i < n_sensors; i++) {
        float total = 0.0;
        for (int j = 0; j < n_temp; j++) {
            total += init_temp[i][j];
        }
        float avg = total / n_temp;

        if (i == 0 || avg > max_avg) {
            max_avg = avg;
            best_sensor = i + 1;
        }
    }

    *highest_avg = max_avg;
    return best_sensor;
}

float other_chores(int n_sensors, int n_temp, float init_temp[n_sensors][n_temp]) {
    int i = 0, j = 0;

    for(i = 0; i < n_sensors; i++) {
        int critical = 0, warning = 0, current_streak = 0, max_streak = 0;
        float total = 0.00, average = 0.00;
        float max = init_temp[i][0];
        float min = init_temp[i][0];

        for(j = 0; j < n_temp; j++) {
            float sensor_reading = init_temp[i][j];
            total += sensor_reading;
            
            if(sensor_reading > max) {
                max = sensor_reading;
            }
            
            if(sensor_reading < min) {
                min = sensor_reading;
            }
            
            if(sensor_reading >= 40.00) {
                critical++;
            }
            else if(sensor_reading >= 35.00 && sensor_reading < 40.00) {
                warning++;
            }

            if(sensor_reading >= 35.00) {
                current_streak++;
                if(current_streak > max_streak) {
                    max_streak = current_streak;
                }
            } else {
                current_streak = 0;
            }
        }
        
        average = total / n_temp;
        printf("\nSensor %d Report:\n", i + 1);
        printf("Total Sum: %.2f\n", total);
        printf("Average: %.2f\n", average);
        printf("Maximum: %.2f\n", max);
        printf("Minimum: %.2f\n", min);
        printf("Number of critical readings: %d\n", critical);
        printf("Number of warning readings: %d\n", warning);
        printf("Longest streak will be: %d\n", max_streak);
    }
    return 0.0;
}

int main() {
  int h_temp = 0, sensors = 0;
  printf("Enter how many sensors your system have: ");
  scanf("%d", &sensors);
  printf("Enter how many hourly readings you want: ");
  scanf("%d", &h_temp);
  
  float init_temp[sensors][h_temp];
  input_temp(sensors, h_temp, init_temp);
  other_chores(sensors, h_temp, init_temp);

  float highest_avg = 0.00;
  int top_sensor = find_highest_avg_sensor(sensors, h_temp, init_temp, &highest_avg);
  printf("Sensor with highest average: Sensor %d with average %.2f\n", top_sensor, highest_avg);
}