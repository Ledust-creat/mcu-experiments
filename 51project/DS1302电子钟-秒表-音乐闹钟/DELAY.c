void delay(unsigned int x)	//@12.000MHz
{
	unsigned char data i, j;

	
	while(x)
	{
		i = 2;
		j = 239;
			do
			{
				while (--j);
			} while (--i);
	x--;
	}
}
