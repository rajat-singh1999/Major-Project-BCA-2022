#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<math.h>
#define size 2500
#define no_ants 10
#define max_itr 10000
int graph[size][size]={{0}};
int fset[size]={0},sset[size]={0},tset[size]={0};
int color[size]={0};
int tempcolor[size]={0};
int pher[size][size]={{0}};
int position[size];
int rand_array[size];
int antcolorcount[size];
int visited[size]={0};
int usedcolor[size]={0};
void initialcoloration(int);  //Generates Initial valid coloration of vertices
void generateantposition(int,int);  //Randomly place the ants in different nodes
int color_count(int); //Count the number colors used in coloration 
int countantcolor(int,int);  //Count the number of color made by each ant
int findmin(int[],int);  //Find minimum value in a list
int findmax(int[],int);  //Find maximum value in a list
int min(int,int); //Find minimum of two numbers
void updatelocalpher(int);  //Generates coloration by each ant
void updateglobalpher(int,int);  //Updates the global coloration of the colony
int member(int[],int);
void antmove(int);
int node,edges,mincol,row,col;
int tempmin=100000,chrnum,counter=0;
int main ()
{
	int i,j,k;
	FILE *fs;
	int ch,itr=0;
	char faltu;
	system("clear");
	printf("Coloration by Ant Colony Optimization...\n");
	if((fs=fopen("queen6_6.col","r"))==NULL)
	{
		printf("\nCan't open the file.\n");
		exit(1);
	}
	while(!feof(fs))
	{
		fscanf(fs,"%d %d %d\n",&mincol,&node,&edges);
	    for(j=1;j<=edges;j++)
	    {
			fscanf(fs,"%c %d %d\n",&faltu,&row,&col);
		    graph[row][col] = 1;
		    graph[col][row] = 1;
	    }
	}
	fclose(fs);
	srand((unsigned)time(NULL));
	initialcoloration(node);
	printf("\nInitial Coloration:");
	for(i=1;i<=node;i++)
		printf(" %d ",color[i]);
	chrnum=color_count(node);
	printf("\n\nInitial Chromatic Number: %d",chrnum);
	getchar();
	for(i=1;i<=node;i++)
	{
		fset[i]=0;
		sset[i]=0;
		tset[i]=0;
	}
	while(chrnum>mincol)
	{
		itr++;
		printf("\n\nIteration: %d",itr);
		//getchar();
		generateantposition(node,no_ants);
		/*for(i=1;i<=no_ants;i++)
			printf("\nAnt[%d]: %d",i,position[i]);
		getchar();*/
		updatelocalpher(no_ants);
		/*printf("\nLocal Pheromone:\n");
		for(i=1;i<=no_ants;i++)
		{
			for(j=1;j<=node;j++)
			{
				printf(" %d ",pher[i][j]);
			}
			printf("\n\n");
		}
		getchar();*/
		antmove(no_ants);
		printf("\t\tChromatic Number:%d",chrnum);
		/*if(itr%10000==0)
		{
			printf("\nChromatic Number after %d Iteration: %d",itr,min(chrnum,tempmin));
			getchar();
		}*/
		if(itr==max_itr)
			break;
	}
	printf("\n\nThe Final Coloration is:\n\n");
	if(tempmin>=chrnum)
	{
		for(i=1;i<=node;i++)
			printf(" %d ",color[i]);
		printf("\n\nChromatic Number: %d\n\n",color_count(node));
	}
	else
	{
		for(i=1;i<=node;i++)
			printf(" %d ",tempcolor[i]);
		printf("\n\nChromatic Number: %d\n\n",tempmin);
	}
	printf("Number of Iterations: %d\n\n",itr);
	return 0;
}
//function initialcoloration..it generates initial vertex color sequentially
void initialcoloration(int n)
{
	int i,j,k;
	int pos,ncolor=1,chkcolor=0;
	for(i=1;i<=n;i++)
		tset[i]=i;
	do
	{
		/*printf("\nfset:\t");
		for(i=1;i<=n;i++)
			printf(" %d ",fset[i]);
		getchar();
		printf("\nsset:\t");
		for(i=1;i<=n;i++)
			printf(" %d ",sset[i]);
		getchar();
		printf("\ntset:\t");
		for(i=1;i<=n;i++)
			printf(" %d ",tset[i]);
		getchar();*/
		j=1;
		pos=1;
		for(i=1;i<=n;i++)
		{
			if(tset[i]>0)
			{
				fset[pos]=tset[i];
				tset[i]=-9999;
				for(k=1;k<=n;k++)
				{
					if(tset[k]>0 && graph[fset[pos]][tset[k]]==1)
					{
						if(!member(sset,tset[k]))
						{
							sset[j++]=tset[k];
							tset[k]=-9999;
						}
					}
				}
				pos++;
			}
		}
		for(i=1;i<=n;i++)
		{
			if(fset[i]!=0)
			{
				color[fset[i]]=ncolor;
				fset[i]=0;
			}
		}
		for(i=1;i<=n;i++)
		{
			if(sset[i]!=0)
			{
				tset[i]=sset[i];
				sset[i]=0;
			}
		}
		ncolor++;
		/*printf("\ncolor:\n");
		for(i=1;i<=n;i++)
			printf(" %d ",color[i]);
		getchar();*/
	}while(member(color,chkcolor)==1);
}//End of initialcoloration
//function returns 1 or 0 depending on whether x belongs to set[] or not
int member(int set[],int x)
{
	int i;
    for(i=1;i<=node;i++)
    	if(x==set[i])
    		return 1;
    return 0;
}//End of member
//Function generateantposition..this function place ants in different vertices
void generateantposition(int n,int a)
{
	int i,rnd,j=1;
	for(i=1;i<=n;i++)
		rand_array[i]=0;
	while(j<=a)
	{
		rnd=rand()%n+1;
		if(rand_array[rnd]==0)
		{
			position[j++]=rnd;
			rand_array[rnd]=1;
		}
	}
}//End of generateantposition()
//Function updatelocalpher()..this function makes coloration by each ant
void updatelocalpher(int n)
{
	int i,j;
	for(i=1;i<=n;i++)
	{
		for(j=1;j<=node;j++)
		{
			pher[i][j]=color[j];
		}
	}
}//end of updatelocalpher
//Function color_count..this function counts number of colors in a coloration
int color_count(int n)
{
	int i,j;
	int temp_color[size];
	int count=0;
	for(i=1;i<=n;i++)
	{
		temp_color[i]=color[i];
	}
	for(i=1;i<n;i++)
	{
		for(j=i+1;j<=n;j++)
		{
			if(temp_color[i]==temp_color[j])
			temp_color[j]=0;
		}
	}
	for(i=1;i<=n;i++)
		if(temp_color[i]!=0)
			count++;
	return count;				
}// end of color_count()
//Function antcolorcount..this function counts number of colors made by each ant locally
int countantcolor(int r,int n)
{
	int i,j;
	int temp_color[size];
	int count=0;
	for(i=1;i<=n;i++)
	{
		temp_color[i]=pher[r][i];
	}
	for(i=1;i<n;i++)
	{
		for(j=i+1;j<=n;j++)
		{
			if(temp_color[i]==temp_color[j])
			temp_color[j]=0;
		}
	}
	for(i=1;i<=n;i++)
		if(temp_color[i]!=0)
			count++;
	return count;				
}// end of antcolorcount()
//Function findmin().. this function returns the minimum value in a list
int findmin(int arr[],int n)
{
	int min,i,pos=1;
	min=arr[pos];
	for(i=2;i<=n;i++)
	{
		if(arr[i]<min)
		{
			pos=i;
			min=arr[i];
		}
	}
	return pos;
}//end of findmin
//Function findmax().. this function returns the maximum value in a list
int findmax(int arr[],int n)
{
	int max,i,pos=1;
	max=arr[pos];
	for(i=2;i<=n;i++)
	{
		if(arr[i]>max)
		{
			pos=i;
			max=arr[i];
		}
	}
	return pos;
}//end of findmin
//Function min().. this function returns the minimum of two numbers
int min(int x,int y)
{
	if(x<y)
		return x;
	else
		return y;
}//end of min
//Function updateglobalpher()..this function updates the global coloration of the colony
void updateglobalpher(int k,int n)
{
	int i;
	for(i=1;i<=n;i++)
		color[i]=pher[k][i];
}//end of updateglobalpher
void antmove(int x)
{
	int i,j,k,mvmnt,itrn,index;
	int pos,state,chk,complete;
	int min,max,val,clr,rnd,cl=1,choice;
	for(mvmnt=1;mvmnt<=x;mvmnt++)
	{
		//printf("\nMovement of Ant [%d]:",mvmnt);
		//printf("***********************************");
		//getchar();
		for(i=1;i<=node;i++)
			tset[i]=i;
		j=1;
		pos=1;
		itrn=1;
		do
		{
			if(itrn==1)
			{
				fset[pos]=position[mvmnt];
				tset[position[mvmnt]]=-9999;
			}
			else
			{
				do
				{
					rnd=rand()%node+1;
					if(tset[rnd]<=0)
						chk=1;
					else
						chk=0;
				}while(chk==1);
				fset[pos]=tset[rnd];
				tset[rnd]=-9999;
			}
			do
			{
				for(k=1;k<=node;k++)
				{
					if(tset[k]>0 && graph[fset[pos]][tset[k]]==1)
					{
						if(visited[tset[k]]==0 && !member(sset,tset[k]))
						{
							sset[j++]=tset[k];
							tset[k]=-9999;
						}
					}
				}
				/*printf("\nfset:");
				for(i=1;i<=node;i++)
					printf(" %d ",fset[i]);
				getchar();
				printf("\nsset:");
				for(i=1;i<=node;i++)
					printf(" %d ",sset[i]);
				getchar();
				printf("\ntset:");
				for(i=1;i<=node;i++)
					printf(" %d ",tset[i]);
				getchar();*/
				for(i=1;i<=node;i++)
				{
					if(tset[i]>0)
					{
						state=1;
						break;
					}
					else
						state=0;
				}
				if(state==1)
				{
					do
					{
						index=rand()%node+1;
						if(tset[index]<=0)
							chk=1;
						else
							chk=0;
					}while(chk==1);
					pos++;
					//printf("\nindex= %d",index);
					//getchar();	
					fset[pos]=tset[index];
					tset[index]=-9999;
					//printf("\nnext node= %d",fset[pos]);
					//getchar();	
				}
			}while(state==1);
			for(i=1;i<=node;i++)
			{
				if(fset[i]!=0 && !member(usedcolor,color[fset[i]]))
				{
					clr=color[fset[i]];
					usedcolor[cl++]=clr;
					choice=0;
					break;	
				}
				else
					choice=1;
			}
			if(choice==1)
			{
				do
				{
					clr=rand()%node+1;
					if(member(usedcolor,clr)==1)
						chk=1;
					else
					{
						usedcolor[cl++]=clr;
						choice=0;
						chk=0;
					}
				}while(chk==1);
			}
			//printf("\nclr= %d",clr);
			//getchar();
			for(i=1;i<=node;i++)
			{
				if(fset[i]!=0)
				{
					pher[mvmnt][fset[i]]=clr;
					visited[fset[i]]=1;
					fset[i]=0;
				}
			}
			/*printf("\nPheromone of Ant[%d]:",mvmnt);
			for(i=1;i<=node;i++)
				printf(" %d ",pher[mvmnt][i]);
			getchar();	
			printf("\nvisited list:");
			for(i=1;i<=node;i++)
				printf(" %d ",visited[i]);
			getchar();
			printf("\nusedcolor list:");
			for(i=1;i<=node;i++)
				printf(" %d ",usedcolor[i]);
			getchar();*/
			for(i=1;i<=node;i++)
			{
				if(sset[i]!=0)
				{
					tset[i]=sset[i];
					sset[i]=0;
				}
			}
			/*printf("\nfset:");
			for(i=1;i<=node;i++)
				printf(" %d ",fset[i]);
			getchar();
			printf("\nsset:");
			for(i=1;i<=node;i++)
				printf(" %d ",sset[i]);
			getchar();
			printf("\ntset:");
			for(i=1;i<=node;i++)
				printf(" %d ",tset[i]);
			getchar();*/
			for(i=1;i<=node;i++)
			{
				if(visited[i]==1)
					complete=1;
				else
				{
					complete=0;
					break;
				}
			}
			itrn++;
			pos=1;
			j=1;
		}while(complete==0);
		/*printf("\nColoration of Ant[%d]:",mvmnt);
		for(i=1;i<=node;i++)
			printf(" %d ",pher[mvmnt][i]);
		getchar();*/
		antcolorcount[mvmnt]=countantcolor(mvmnt,node);
		for(i=1;i<=node;i++)
		{
			fset[i]=0;
			sset[i]=0;
			visited[i]=0;
			usedcolor[i]=0;
		}
		cl=1;
	}
	/*printf("\nAnt Coloration:\n");
	for(i=1;i<=no_ants;i++)
	{
		for(j=1;j<=node;j++)
		{
			printf(" %d ",pher[i][j]);
		}
		printf("\n");
	}*/
	//getchar();
	min=findmin(antcolorcount,no_ants);
	max=findmax(antcolorcount,no_ants);
	if(countantcolor(min,node)<chrnum)
	{
		//printf("\t\tCh. Num Changed");
		updateglobalpher(min,node);
		chrnum=countantcolor(min,node);
		counter=0;
		//getchar();
	}
	else
	{	
		if(counter==10)
		{
			//printf("\t\tWorst Ch. Num");
			if(chrnum<tempmin)
			{
				tempmin=chrnum;
				//printf("\t\tempmin = %d",tempmin);
				//getchar();
				for(i=1;i<=node;i++)
					tempcolor[i]=pher[min][i];
			}
			updateglobalpher(max,node);
			chrnum=countantcolor(max,node);
			counter=0;
			//getchar();
		}
		counter++;
	}
	/*printf("\nGlobal Pheromone:");
	for(i=1;i<=node;i++)
		printf(" %d ",color[i]);
	getchar();*/
}

