#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define p 10000
#define s 3
#define ir 5
#define sp 300
int matrix[p][p],result[3];
int providers[sp],infected[ir],testing[p];
int interpro;
int interper;

int transitive[p][p];

void init()
{
	int i,j;
	for(i=0;i<p;i++)
	{
		for(j=0;j<p;j++)
		matrix[i][j]=0;
	}
	for(i=0;i<sp;i++)
	providers[i]=-1;
	for(i=0;i<ir;i++)
	infected[i]=-1;
	for(i=0;i<p;i++)
	testing[i]=-1;
	
	printf("Initialised\n");
}
int checkPrime(int val)
{
	int i;
	for(i=2;i<val;i++)
	{
		if(val%i==0)
		return 0;
	}
	return 1;
}
int randomYes()
{
	int val=1+rand()%1000;
	if(val==1000)
	return 1;
	else
	return 0;
}
int countPrimeFactors(int x,int y)
{
	int min,i,count=0;
	if(x>y)
	min=y;
	else
	min=x;
	for(i=2;i<=min;i++)
	{
		if(x%i==0&&y%i==0)
		{
			int check=checkPrime(i);
			if(check==1)
			count++;
		}
	}
	return count;
}
int biasedYes(int x,int y)
{
	int common=countPrimeFactors(x+1001,y+1001);
	while(common!=0)
	{
	   if(randomYes()==1)
	   return 1;
	   common--;
    }
	return 0;
}
void markinfected()
{
	int count=0,i=0;
	while(count!=ir)
	{
		int val,j;
		val=randomYes();
		if(val==1)
		{
			int count1=0;
			for(j=0;j<ir;j++)
			if(infected[j]!=i)
			count1++;
			if(count1==ir)
			{
				infected[count]=i;
				count++;
			}
		}
		if(i==p-1)
		i=0;
		else
		i++;
	}
	
	printf("Marked1\n");
}
void markprovider()
{
	int count=0,i=0;
	while(count!=sp)
	{
		int val,j;
		val=randomYes();
		if(val==1)
		{
			int count1=0;
			for(j=0;j<sp;j++)
			if(providers[j]!=i)
			count1++;
			if(count1==sp)
			{
				providers[count]=i;
				count++;
			}
		}
		if(i==p-1)
		i=0;
		else
		i++;
	}
	
	printf("Marked2\n");
}

int checkinfected(int x)
{
	int i;
	for(i=0;i<ir;i++)
	if(infected[i]==x)
	return 1;
	return 0;
}
int persontype(int x)
{
	int i;
	for(i=0;i<sp;i++)
	{
		if(providers[i]==x)
		return 1;
	}
	return 0;
}

int checktest(int x)
{
	int i=0;
	while(testing[i]!=-1)
	{
		if(testing[i]==x)
		return 0;
		i++;
	}
	return 1;
}

int doExperiment()
{
	init();
	markprovider();
	markinfected();
	int countper=0,countpro=0,i,j,countin=0;
	interpro=5*p/100;
    interper=6;
    int z1=interper*(p-sp);
	while(countper<z1)
	{
		for(i=0;i<p;i++)
		{
			if(persontype(i)==0)
			{
				for(j=i+1;j<p;j++)
				{
					if(biasedYes(i,j)==1)
					{
						matrix[i][j]=1;
						matrix[j][i]=1;
						countper=countper+2;
					}
				}
			}
		}
	}
	
	printf("Person Interacted\n");
	
	
	int z2=interpro*sp;
	while(countpro<z2)
	{
		for(i=0;i<p;i++)
		{
			if(persontype(i)==1)
			{
				for(j=i+1;j<p;j++)
				{
					if(biasedYes(i,j)==1)
					{
						matrix[i][j]=1;
						matrix[j][i]=1;
						countpro++;
					}
				}
			}
		}
	}
	
	printf("Provider Interacted\n");
	
	int k;
	for(i=0;i<p;i++)
	{
		for(j=0;j<p;j++)
		transitive[i][j]=matrix[i][j];
	}
	for(k=0;k<p;k++)
	{
		for(i=0;i<p;i++)
		{
			for(j=0;j<p;j++)
			transitive[i][j]=transitive[i][j]||(transitive[i][k]&&transitive[k][j]);
		}
	}
	for(i=0;i<p;i++)
	{
		if(checkinfected(i)==1)
		{
			for(j=0;j<p;j++)
			{
				if(transitive[i][j]==1)
				{
					if(checktest(j)==1)
					{
						testing[countin]=j;
						countin++;
					}
				}
			}
		}
	}
	return countin;
}
int main()
{
	int repeats;
	printf("Started\n");
	for(repeats=0;repeats<3;repeats++)
	{
    
	result[repeats]=doExperiment();
	printf("%d\n",result[repeats]);
	
    }
	float mean=0,std=0;
	for(repeats=0;repeats<3;repeats++)
	mean=mean+result[repeats];
	mean=mean/3;
	for(repeats=0;repeats<3;repeats++)
	std=std+(result[repeats]-mean)*(result[repeats]-mean);
	std=std/3;
	std=sqrt(std);
	printf("The mean is %f and standard deviation is %f",mean,std);
}

