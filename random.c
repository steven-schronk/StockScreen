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
