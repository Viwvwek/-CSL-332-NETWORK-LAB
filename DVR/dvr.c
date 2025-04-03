#include <stdio.h>

struct node {
    unsigned dist[20];
    unsigned from[20];
} rt[10];

int main() {
    int costmat[20][20];
    int nodes, i, j, k, count = 0;

    printf("Enter the number of nodes: ");
    scanf("%d", &nodes);

    printf("Enter the cost matrix: \n");
    for (i = 0; i < nodes; i++) {
        for (j = 0; j < nodes; j++) {
            printf("costmat[%d][%d]: ", i, j);
            scanf("%d", &costmat[i][j]);
        }
        costmat[i][i] = 0;  // Ensure diagonal elements are 0
    }

    // Initialize distance table
    for (i = 0; i < nodes; i++) {
        for (j = 0; j < nodes; j++) {
            rt[i].dist[j] = costmat[i][j];
            rt[i].from[j] = j;
        }
    }

    // Distance Vector Algorithm
    do {
        count = 0;
        for (i = 0; i < nodes; i++) {
            for (j = 0; j < nodes; j++) {
                for (k = 0; k < nodes; k++) {
                    if (rt[i].dist[j] > costmat[i][k] + rt[k].dist[j]) {
                        rt[i].dist[j] = costmat[i][k] + rt[k].dist[j];
                        rt[i].from[j] = k;
                        count++;
                    }
                }
            }
        }
    } while (count != 0);

    // Print Routing Tables
    for (i = 0; i < nodes; i++) {
        printf("\nRouting Table for Router %d\n", i + 1);
        printf("Destination\tNext Hop\tDistance\n");
        for (j = 0; j < nodes; j++) {
            printf("%d\t\t%d\t\t%d\n", j + 1, rt[i].from[j] + 1, rt[i].dist[j]);
        }
    }

    return 0;
}
