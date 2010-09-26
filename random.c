

/* generate random integer between min and max */
int RandomInt(int min, int max)
{
	if(min > max)
	{
		return 0;
	}
	else
	{
		return  min + rand() % ((max+1) - min);
	}
}

/* generate data points for OHLC charts */
void RandomOHLC(int min, int max, int *open, int *high, int *low, int *close)
{
		/* generate random values for this point */
		*low = RandomInt(min, max);
		*high = RandomInt(*low, max);
		/* get direction */
		*open = RandomInt(0, 1);

		if(*open > 0)
		{
			*open = RandomInt(*low, *high);
			*close = RandomInt(*low, *high);
		} else {
			*close = RandomInt(*low, *high);
			*open = RandomInt(*low, *high);
		}
}


