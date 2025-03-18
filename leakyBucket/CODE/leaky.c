#include <stdio.h>
int main()
{
int bsize, bucket, __in, out_rate, n;
bucket = 0;
printf("Bucket size:\t");
scanf("%d", &bsize);
printf("Outflow rate:\t");
scanf("%d", &out_rate);
printf("Number of incoming packets:\t");
scanf("%d", &n);
for (int i = 0; i < n; i++)
{
printf("Incoming packet:\t");
scanf("%d", &__in);
if (__in < (bsize - bucket))
{
bucket = bucket + __in;
printf("Adding packet: %d/%d\n", bucket, bsize);
}
else
{
printf("Dropped %d\n", __in - (bsize - bucket));
bucket = bsize;
}
if(bucket - out_rate > 0)
bucket = bucket - out_rate;
else
bucket = 0;
printf("STATUS\n======\n");
printf("Bucket filled:\t%d/%d\n", bucket, bsize);
}
}