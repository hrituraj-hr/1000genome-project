#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "kmeans.h"

float final_matrix[30000][3000];
float transpose_matrix[3000][30000];

//Function to display output
void display(int n, int d, int **points, int clusters, float **centroids, int *array1, char data3[3000][30])
{   
    for(int i = 0; i < n; i ++) {
		printf("%s %d ",data3[i] ,array1[i]);
        for(int j = 0; j < d; j++) {
            printf("%.3f,", centroids[array1[i]][j+1]);
        }
        printf("\n");
    }
    
}

//Function to break the row into token
int getToken(int *index, char *data, char *temp)
{
	int i = 0;
	while (data[*index] == '\t') {
		*index = *index + 1;
	}
	
	while (data[*index] != '\t' && data[*index] != '\n') {
		temp[i] = data[*index];
		*index = *index + 1;
		i++;
	}

	temp[i] = '\0';

	return (data[*index] != '\n');
}

//function to convert char into int
int charArrayToInt(char *temp)
{
	int res = 0;
	int i = 0;
   
	while (temp[i] != '\0') {
		res = res * 10 + temp[i] - '0';
		i++;
	}

	return res;
}

//finction to find the index of A,C,G,T
int get_char_val(char c) {
	if(c == 'A')
		return 0;
	if(c == 'C')
                return 1;
	if(c == 'G')
                return 2;
	if(c == 'T')
                return 3;
}

int main(int arg, char **args)
{
	FILE *file;
	FILE *ptr1;
	FILE *ptr2;

	int chromNumber= charArrayToInt(args[1]);
	int countLines = 0;
	double min= atof(args[3]);
	double max= atof(args[4]);
	int clusters = atoi(args[7]);
	int max_iteration = atoi(args[8]);
	int threshold = atof(args[9]);
	double array[4][4];

	file = fopen(args[2], "r");
	ptr1 = fopen(args[5], "r");
	ptr2 = fopen(args[6], "r");

	char data[100000];
	char data1[1000];
	char data2[3000][30];
	char data3[3000][30];
	int arr[2504] = {0};
	int line = 0;
	char ptr[1000];
	int line_number = 0;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			fscanf(ptr2, "%lf", &array[i][j]);
		}
	}
	// Finding the coloumns
	while(fgets(data1, 100, ptr1)) {
		int j = 0;
		while(data1[j] != '\n'|| data1[j] == '\0') {
			data2[line_number][j] = data1[j];
			j++;
		}
		data2[line_number][j] = '\0';
		line_number++;
	}

	int gid_count = 0;

	//main data reading
	while (fgets(data, 100000, file)) {
		char ch[100000];
		line++;
		ch[0] = '\0';
		if (data[0] == '#' && data[1] == '#') {
			continue;
		}

		int index = 0;
		if (data[0] == '#') {
			//Chrome
			getToken(&index, data, ptr);
			
			//POS
			getToken(&index, data, ptr);

			//ID
			getToken(&index, data, ptr);
			
			//REF
			getToken(&index, data, ptr);
			
			//ALT
			getToken(&index, data, ptr);

			//Qual
			getToken(&index, data, ptr);

			//filter
			getToken(&index, data, ptr);
			
			//into
			getToken(&index, data, ptr);
			
			//FORMAT
			getToken(&index, data, ptr);
			// printf("%s ",ptr);

			int j = 0;
			int t = 0;
			while (getToken(&index, data, ptr)) {
				 for(int i = 0; i < line_number; i++) {
					
					 if(strcmp(ptr, data2[i]) == 0) {
							strcpy(data3[t], ptr);
			      			arr[j] = 1;
							gid_count++;
							t++;
					 }
				 } 
					j++;
            }

			for(int i = 0; i < line_number; i++) {
				 if(strcmp(ptr, data2[i]) == 0) {
						gid_count++;
			      		arr[j] = 1;
				 }
			}
			continue;
		}

		// CHROM
		getToken(&index, data, ptr);
		if (charArrayToInt(ptr) != chromNumber) {
			continue;
		}

		// POS
		getToken(&index, data, ptr);

		// ID
		getToken(&index, data, ptr);

		// REF
		getToken(&index, data, ptr);
		
		int i = 0;
		int fail = 0;
		
		while (ptr[i] != '\0')
		{
			if (i == 1)
				fail = 1;
			if (ptr[i] != 'A' && ptr[i] != 'T' && ptr[i] != 'G' && ptr[i] != 'C')
				fail = 1;
			i++;
		}
		if (fail)
			continue;
		char c[5];
		c[0] = ptr[0];
		int counter = 1;

		// ALT
		getToken(&index, data, ptr);
		
		i = 0;
		fail = 0;
		
		while (ptr[i] != '\0') {
			if (i % 2 == 1)
			{
				if (ptr[i] != ',')
				{
					fail = 1;
				}
			}
			else
			{
				if (ptr[i] != 'A' && ptr[i] != 'T' && ptr[i] != 'G' && ptr[i] != 'C')
					fail = 1;
				c[counter++] = ptr[i];
			}
			i++;
		}
		if (fail == 1)

			continue;

		// QUAL
		getToken(&index, data, ptr);

		// FILTER
		getToken(&index, data, ptr);
	
		if (strcmp(ptr, "PASS") != 0)
			continue;

		// INFO
		getToken(&index, data, ptr);
		
		//FORMAT
		getToken(&index, data, ptr);
		
		int countZero = 0; 
		int countNonZero = 0;
		int l = 0;
		char ch1[1000];
		ch1[0] = '\0';

		while (getToken(&index, data, ptr)) {
			if(arr[l] == 1) {
				int A = get_char_val(c[ptr[0]-'0']);
				int B = get_char_val(c[ptr[2]-'0']);
				sprintf(ch, "%.1f ", array[A][B]);
				strcat(ch1, ch);
			}
			l++;

			if(ptr[0] == '0' && ptr[2] == '0') {
				countZero++;
			} else {
				countNonZero++;
			}
		}
		int A, B;
		if(arr[l++] == 1) {
    	  	A = get_char_val(c[ptr[0]-'0']);
            B = get_char_val(c[ptr[2]-'0']);
			sprintf(ch, "%.1f ", array[A][B]);
			strcat(ch1, ch);
        }

		if (strcmp(ptr, "0|0") == 0) {
			countZero++;
		} else {
				countNonZero++;
		}

		fail = 0;
		double totalCount = countZero + countNonZero;
		double mutationPercentage = ((countNonZero) /totalCount)*100;
	
		if (mutationPercentage < min || mutationPercentage > max)
		  fail = 1;
		if (fail == 1) {
			continue;
		}
		countLines++;

		char* token = strtok(ch1, " ");
		for(int i = 0; i < gid_count; i++) {
			final_matrix[countLines][i] = atof(token);
			token = strtok(NULL, " ");
		}	
	}

	for(int i = 0; i < gid_count; i++) {
		for(int j = 0; j < countLines; j++) {
			transpose_matrix[i][j] = final_matrix[j][i];
		}
	}
	int number;
    int n, d;
	int **points;
    float **centroids;
    int *array1;
	n = gid_count;
	d = countLines;

    points = malloc(n*sizeof(int*));
    centroids = malloc(clusters*sizeof(int*));
    array1 = malloc(n*sizeof(int));

	 for(int i = 0; i <n; i++){
        points[i] = malloc(d*sizeof(int));
    }
    
    for(int i = 0; i <clusters; i++){
        centroids[i] = malloc(d*sizeof(int));
    }
	
	for(int i = 0; i < clusters; i++) {
        for(int j = 0; j < d; j++) {
           centroids[i][j] = transpose_matrix[i][j];
        }
    }

	for(int i = 0; i < n; i++) {
        for(int j = 0; j < d; j++) {
           points[i][j] = transpose_matrix[i][j];
        }
    }

	int *ans1 = kmeans(n, d, clusters, centroids, points,array1);
    int *ans2 = kmeans(n, d, clusters, centroids, points,array1);
    while(1) {
        int temp = 0;
        for(int i=0; i< n; i++) {
            if(ans1[i] != ans2[i])
            temp = 1;
        }
        if(temp == 1) {
            ans1 = ans2;
            ans2 = kmeans(n, d, clusters, centroids, points,array1);
        } else {
            display(n, d, points, clusters, centroids, ans1, data3);
            break;
        }
      
    }
	fclose(file);
	//printf("\n%d\n", countLines);
	return 0;
}
