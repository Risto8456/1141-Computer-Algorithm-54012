#include<stdio.h>
#include<stdlib.h>

int main(int argc,char *argv[])
{
    int i,j;

    FILE *fp;
    char newline[10];
    int nodenum,edgenum,cost,begin,end;

	fp=fopen("cost239","r");
	fgets(newline,10,fp);

	sscanf(newline,"%i %i",&nodenum,&edgenum);

    int A[nodenum][nodenum];//儲存連線的點
    for(i=0;i<nodenum;i++)
        for(j=0;j<nodenum;j++)
            A[i][j]=0;
    while(fgets(newline,10,fp) != NULL)
	{
	   sscanf(newline,"%i %i %i",&begin,&end,&cost);
	   A[begin][end]=1;
	   A[end][begin]=1;
	}
	fclose(fp);

    int r,f;

    for(r=0;r<nodenum;r++){
        for(f=0;f<nodenum;f++)
            printf("%d ",A[r][f]);
        printf("\n");
    }
    printf("\n");


}
