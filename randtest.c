
int main()
{
	int dice[6]={0,0,0,0,0,0};
	int i;

	for (i=0;i<3000000;i++)
	{
		dice[rand()%6]++;
	}

	for (i=0; i<6; i++)
	{
		printf("%d came up %d times\n",i,dice[i]);
	}
	return 0;
}
