//Function to calculate the mean
void mean_calc(int n, int d ,int clusters, float **centroids, int **points, int *array1)
{
    int array2[n];
    for(int i = 0; i < n; i++) {
            array2[i] = 0;
    }
    
    for(int i = 0; i < clusters; i++) {
        for(int j = 0; j < d; j++){
            centroids[i][j] = 0;
        }
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < d; j++) {
            centroids[array1[i]][j] += points[i][j];
         }
         array2[array1[i]]++;
    }

    for(int i = 0; i < clusters; i++) {
        for(int j = 0; j < d; j++) {
            centroids[i][j] /= array2[i];
         }
    }
    
}

//Function to calculate to distance
int get_distance(int *point1, float *point2, int d) 
{
    int distance;
    distance = 0;
    for(int i = 0; i < d; i++) {
        distance = distance + (point1[i] - point2[i])*(point1[i] - point2[i]);
    }
    return distance;
}

//Function to calculate cluster numbers
int cluster_no(float **centroids, int d, int clusters, int *point)
{   int distance;
    int min_distance = 1000000;
    int index = 0;
    
    for(int i = 0; i < clusters; i++) {
        distance = get_distance(point, centroids[i], d);
       
        if(distance < min_distance) {
            min_distance = distance;
            index = i;
        }
    }
    return index;
}

//Function to compute K means

int* kmeans(int n, int d ,int clusters, float **centroids, int **points, int *array1)
{   
    for(int i = 0; i < n; i++) {
        array1[i] = cluster_no(centroids, d, clusters, points[i]);
    }

    mean_calc(n, d, clusters, centroids, points, array1);
    
    return array1;
}