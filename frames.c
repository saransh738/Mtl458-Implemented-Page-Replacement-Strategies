#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef long long ll;
int TOTAL_NO_OF_FRAMES = 0;
int TOTAL_ACCESSES=0,TOTAL_MISSES=0,TOTAL_WRITES=0,TOTAL_DROPS=0;
int VERBO = 0;
char tmp[7];

struct global
{
	int tot;
	int p;
	int* pt;
};

struct PTE
{
  int page_number;
  bool vb;
  bool pb;
  int marker;
  bool db;
  bool ub;
  
} ;


int popu = 0;
int TIMER = 0;

int get_pos_space(char* s)
{
	for(int i=0;i<strlen(s);i++)
	{
		if(s[i]!=' ')
		{
			return i;
		}
	}
	return 0;
}

char* help(int t,char* s)
{
    char* k=(char*)malloc(16*sizeof(char));
    int i = get_pos_space(s);
	int c=i;
    for(i=c;i<strlen(s);i++) 
	{
		if(s[i]!=' ')
		{			
		  k[i] = s[i];
		}
		else
		{
			break;
		}
	}
    k[i] = '\0';
    if(t) 
	{
		c=i;
        for(i=c;i<strlen(s);i++)
		{
			if(s[i]!=' ')
			{
				break;
			}
		}
        k[0] = s[i];  
		k[1] = '\0';
    }
    return k;
	
}

struct PTE list[1001]; 
struct global map[1050000];


ll convert(char* s)
{
	ll tot=0;
	for(int i=strlen(s)-1;i>=0;i--)
	{
		if(s[i]>='A' && s[i]<='F')
		{
			tot+=(s[i]-55)*pow(16,strlen(s)-1-i);
		}
		else if(s[i]>='0' && s[i]<='9')
		{
			tot+=(s[i]-48)*pow(16,strlen(s)-1-i);
		}
		else if(s[i]>='a' && s[i]<='f')
		{
			tot+=(s[i]-87)*pow(16,strlen(s)-1-i);
		}
	}
	return tot;
}


void Helpher(ll n,int num)
{
	char a[100];
	int k=0;
	while (n!=0) 
	{
		if ((n%16)>=10)
			a[k++] = 87+(n%16);
		else
			a[k++] = 48+(n%16);
		n/=16;
	}
    printf("Page %s was read from disk, page 0x",tmp);
    for(int i=k;i<5;i++)
	{
        printf("0");
	}
	for (int i=k-1;i>=0;i--) 
	{
	   printf("%c",a[i]);
    }
	if(list[num].db) 
	{ 
	    printf(" was written to the disk.\n");
	}
	else 
	{
	    printf(" was dropped (it was not dirty).\n");
	}
}

int FIRST_LEAST()
{
	int mn=1e9;
	int indi=0;
	for(int i=0;i<TOTAL_NO_OF_FRAMES;i++)
	{
		if(list[i].marker<mn)
		{
			mn=list[i].marker;
			indi=i;
		}
	}
	if(VERBO==1) 
	{
		Helpher(list[indi].page_number,indi);
	}
	list[indi].marker = popu++;
	return indi;
    
}

int RANDOM()
{ 
    int c = rand()%TOTAL_NO_OF_FRAMES;
	if(VERBO==1) 
	{
		Helpher(list[c].page_number,c);
	}
	return c;
}

int helper(int TIMER)
{
	return (TIMER+1)%TOTAL_NO_OF_FRAMES;
}

int CLOCK()
{
	while(list[TIMER].ub == true)
	{
        list[TIMER].ub = false;
        TIMER = helper(TIMER);
    }

	if(VERBO==1) 
	{
		Helpher(list[TIMER].page_number, TIMER);
	}
	
	list[TIMER].ub =  true;
	int ans = TIMER;
	TIMER = helper(TIMER);				
	return ans;
}

/*int helper_OPT(int arr[], int n, int k)
{
    for(int i=0;i<n;i++)
    {
    	if(arr[i]>k)
    	{
    		return i;
		}
	}
	return n;
}*/

int helper_OPT(int a[], int n ,int k)
{
    int left = 0;
    int right = n;
    while (left < right) 
	{
        int m = left + (right - left) / 2;
        if (k>= a[m]) 
		{
			left = m + 1;
		}
        else 
		{
			right = m;
		}
    }  
    return left;
}

int support()
{
	int mx =-1;
	int mxi=0;
	for(int i=0;i<TOTAL_NO_OF_FRAMES;i++)
	{
		if(list[i].marker>mx)
		{ 
	     	mx = list[i].marker;
			mxi = i;
		}
	}
	return mxi;
}

int OPT(int s, int a)
{
	for(int i=0;i<TOTAL_NO_OF_FRAMES;i++)
	{
		list[i].marker = a+1;
	}
    
	for(int i=0;i<TOTAL_NO_OF_FRAMES;i++)
	{
		int j = helper_OPT(map[list[i].page_number].pt, map[list[i].page_number].tot, s);
		if(j == map[list[i].page_number].tot) 
		{
		    list[i].marker = a+1;
		}
		else 
		{
		    list[i].marker = map[list[i].page_number].pt[j];
	    }
	}
	int c = support();
	if(VERBO==1) 
	{
	   Helpher(list[c].page_number,c);
    }
	return c;
}

int Frame_finder(int num, char* s, int st, int l)
{
	int f;
	if(s == "OPT") 
	{
		f = OPT(st,l);
	}
	if(s == "FIFO" || s == "LRU") 
	{
		f = FIRST_LEAST();
	} 
	else if(s=="RANDOM") 
	{
		f = RANDOM();
	}
	else if(s=="CLOCK") 
	{
		f = CLOCK();
	}
	if(list[f].db) 
	{
		TOTAL_WRITES++;
	}
	else 
	{
		TOTAL_DROPS++;
	}		
	list[f].page_number =  num;
	list[f].db = false;   
    return f;
}

int get_VPN(int VA)
{
	return (VA & (0xfffff000UL)) >> 12;
}
int get_OFFSET(int VA)
{
	return 	VA & (0x00000fffUL);
}
int get_address(int VA,char *s, int st,int n, char* ty)
{
	
	int PA;
	int i=0;
	for(i=0;i<TOTAL_NO_OF_FRAMES ;i++)
	{
		if(list[i].page_number == get_VPN(VA))
		{
			if(s!="FIFO") 
			{
			   list[i].marker = popu++;
		    }
			if(strcmp(ty, "W")==0) 
			{
			   list[i].db = true;    
		    }
			list[i].ub = true;
			return PA = (i << 12) | get_OFFSET(VA); 
		}
		else if(list[i].page_number == -1)
		{
		   break;
	    }
	}

	
	struct PTE page = list[i];
	TOTAL_MISSES++;
	if(i<TOTAL_NO_OF_FRAMES)
	{
		list[i].marker = popu++;
		list[i].page_number = get_VPN(VA);
		list[i].ub = true;
		if(strcmp(ty, "W")==0) 
		{
			list[i].db = true;
		}
		PA= (i << 12) | get_OFFSET(VA);
	}
	
	else
	{
		int c = Frame_finder(get_VPN(VA),s,st,n);
		if(strcmp(ty, "W")==0) 
		{
			list[c].db = true;
		}			
		PA = (c << 12) | get_OFFSET(VA);
	} 
	return PA;
	
}

void set1()
{
	
	for(int i=0;i<1050000;i++)
	{							
		if(map[i].tot)
		{
			map[i].pt = (int*)malloc(map[i].tot * sizeof(int));
			map[i].p = 0;
		}
	}

}
void set2()
{
	for(int i=0;i<1050000;i++)
	{
		if(map[i].tot)
		{
			map[i].p = 0;
		}
	}
	
}

int get_pn(char* s)
{
	return (convert(help(0,s)) & (0xfffff000UL)) >> 12;
}

int main(int argc, char *argv[] ) 
{  
    if(argc == 5)
	{
		VERBO=1;
	}
	srand(5635);
	char* STATEGY;
	if(strcmp(argv[3],"OPT")==0)
	{		
		STATEGY = "OPT";
	}
	else if(strcmp(argv[3],"FIFO")==0) 
	{
		STATEGY = "FIFO";
	}
	else if(strcmp(argv[3],"CLOCK")==0)
	{		
		STATEGY = "CLOCK";
	}
	else if(strcmp(argv[3],"LRU")==0) 
	{
		STATEGY = "LRU";
	}
	else if(strcmp(argv[3],"RANDOM")==0) 
	{
		STATEGY = "RANDOM";
	}
	
	
	FILE * trace;
	trace = fopen(argv[1], "r");
    
	char x[16] = {0};
	int n = 0;

	
	while(fgets(x, 16, trace))
	{
		n+=1;		
	}		

    for(int i=0;i<1050000;i++)
	{
		map[i].tot=0;
		map[i].p = -1;
	}
	
	TOTAL_NO_OF_FRAMES = atoi(argv[2]); 
	
	for(int i=0;i<TOTAL_NO_OF_FRAMES;i++)
	{
		list[i].page_number = -1;
		list[i].vb = true;
		list[i].pb = false;
		list[i].ub = false;
		list[i].db = false;
		list[i].marker = n+1;
	}
	
	char **init = (char **)malloc(n * sizeof(char *));
    for (int i=0; i<n; i++)
	{
         init[i] = (char *)malloc(16 * sizeof(char));
	}
    fseek(trace, 0, SEEK_SET);
	
	int q = 0;

	if(STATEGY == "OPT")
	{
		int page_number;
		q = 0;
		while(fgets(x, 16, trace))
		{ 
			strcpy(init[q], x); 
	        page_number = get_pn(init[q]);
	        map[page_number].tot++;
	        q++;
	        
	    }
        set1();
        fseek(trace, 0, SEEK_SET);
	    q = 0;
	    while(fgets(x, 16, trace))
		{
	        page_number = get_pn(init[q]);
	        (map[page_number].pt)[ map[page_number].p] = q++;
	        map[page_number].p ++;
	    }
        set2();
	}
    else
	{	q = 0;
		while(fgets(x, 16, trace))
		{ 
			strcpy(init[q++], x); 
		}
	}

    q = 0;
	fseek(trace, 0, SEEK_SET);
	TOTAL_ACCESSES = n;   

	for (int i=0;i<n;i++)
	{
		strncpy(tmp, &(help(0,init[i]))[0], 7);
		int physical_address = get_address(convert(help(0,init[i])), STATEGY, i, n,help(1,init[i]));
	}
    printf("Number of memory accesses: %d\nNumber of misses: %d\nNumber of writes: %d\nNumber of drops: %d\n",TOTAL_ACCESSES,TOTAL_MISSES,TOTAL_WRITES,TOTAL_DROPS);	
    return 0;

}