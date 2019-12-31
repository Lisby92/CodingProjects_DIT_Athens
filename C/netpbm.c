// ######## PROJECT 2 ######## //
// ###### 1115201200106 ###### //
// ## Basileios Mavrommatis ## //

// ####### Error List ######## //
// #1 = Invalid picture format //
// #2 = Invalid width format //
// #3 = Invalid height format //
// #4 = Invalid colour channel max value //

#include <stdio.h>

int main (void)
{
	int ch, w, h, max_val, flag, i, r, g, b, power2, grey, counter, temp, temp1, extra;
	
	w = 0;
	h = 0;
	max_val = 0;
	flag = 0;
	temp = 0;
	temp1 = 0;
	
	ch = getchar();
	if(ch == 'P')			
	{
		putchar(ch);		
		ch = getchar();		
		if(ch == '3')		
		{
			ch -= 1;		
			putchar(ch);		
			ch = getchar();		
			if((ch != '\n') && (ch != ' ') && (ch != '\t'))			
			{
				printf("Invalid picture format\n");					
				return 1;											
			}
			else
			{
				putchar('\n');			
				ch = getchar();
				ch = getchar(); 
				while (ch != '\n')
				{
					ch = getchar();
				}
				ch = getchar();
				while (ch != '\n')
				{
					ch = getchar();
				}
				ch = getchar();
				while((((ch - '0') >= 0) && ((ch - '0') <= 9)) && (flag == 0))    
				{															
					w += (ch - '0');											
					w *= 10;
					putchar(ch);			
					ch = getchar();
					if((ch == '\n') || (ch == '\t') || (ch == ' '))
					{
						flag = 1;
						w /= 10;			
						putchar(ch);		
					}
					else if(((ch - '0') < 0) && ((ch - '0') > 9))
					{
						printf("Invalid width format\n");
						return 2;
					}
				}							
				ch = getchar();				
				flag = 0;					
				while((ch == '\n') || (ch == '\t') || (ch == ' '))           
				{
					putchar(ch);
					ch = getchar();
				}							
				if(((ch - '0') < 0) && ((ch - '0') > 9))					
				{
					printf("Invalid height format\n");					
					return 3;											
				}
				while((((ch - '0') >= 0) && ((ch - '0') <= 9)) && (flag == 0))    
				{															
					h += (ch - '0');											
					h *= 10;
					putchar(ch);		
					ch = getchar();
					if((ch == '\n') || (ch == '\t') || (ch == ' '))
					{
						flag = 1;
						h /= 10;			
						putchar(ch);		
					}
					else if(((ch - '0') < 0) && ((ch - '0') > 9))
					{
						printf("Invalid height format\n");
						return 2;
					}
				}							
				flag = 0;
				ch = getchar();
				if(((ch - '0') < 0) && ((ch - '0') > 9))
				{
					printf("Invalid max value of colour channels\n");    
					return 4;										     
				}
				while((((ch - '0') >= 0) && ((ch - '0') <= 9)) && (flag == 0))    
				{															 
					max_val += (ch - '0');											 
					max_val *= 10;
					putchar(ch);			
					ch = getchar();
					if((ch == '\n') || (ch == '\t') || (ch == ' '))
					{
						flag = 1;
						max_val /= 10;			
						putchar('\n');			
					}
					else if(((ch - '0') < 0) && ((ch - '0') > 9))
					{
						printf("Invalid max value of colour channels\n");	
						return 4;											
					}
				}
				counter = 0;
				while(counter < (h*w*12))
				{
					temp = 0;
					temp1 = 0;
					r = 0;
					b = 0;
					g = 0;
					grey = 0;
					ch = getchar();
					if((ch == '\n') || (ch == ' '))
					{
						ch = getchar();
						if((ch == '\n') || (ch == ' '))
						{
							ch = getchar();
							r = (ch - '0');
						}
						else
						{
							r = 10*(ch - '0');
							ch = getchar();
							r += (ch - '0');
						}
					}
					else
					{
						r = 100*(ch - '0');
						ch = getchar();
						r += 10*(ch - '0');
						ch = getchar();
						r += (ch - '0');
					}
					r *= 299;
					grey += r;
					counter += 4;
					ch = getchar();
					ch = getchar();
					if((ch == '\n') || (ch == ' '))
					{
						ch = getchar();
						if((ch == '\n') || (ch == ' '))
						{
							ch = getchar();
							g = (ch - '0');
						}
						else
						{
							g = 10*(ch - '0');
							ch = getchar();
							g += (ch - '0');
						}
					}
					else
					{
						g = 100*(ch - '0');
						ch = getchar();
						g += 10*(ch - '0');
						ch = getchar();
						g += (ch - '0');
					}
					g *= 587;
					grey += g;
					counter += 4;
					ch = getchar();
					ch = getchar();
					if((ch == '\n') || (ch == ' '))
					{
						ch = getchar();
						if((ch == '\n') || (ch == ' '))
						{
							ch = getchar();
							b = (ch - '0');
						}
						else
						{
							b = 10*(ch - '0');
							ch = getchar();
							b += (ch - '0');
						}
					}
					else
					{
						b = 100*(ch - '0');
						ch = getchar();
						b += 10*(ch - '0');
						ch = getchar();
						b += (ch - '0');
					}
					b *= 114;
					grey += b;
					counter += 4;
					grey /= 1000;
					ch = getchar();
					if(grey == 0)
					{
						putchar('0');
						putchar(' ');
					}
					else if((grey > 0) && (grey < 100))
					{
						temp = grey;
						temp = (temp % 10);
						grey -= temp;
						grey /= 10;
						printf("%d",grey);
						printf("%d",temp);
						putchar(' ');
					}
					else if(grey == 100)
					{
						putchar('1');
						putchar('0');
						putchar('0');
						putchar(' ');
					}
					else if((grey > 100) && (grey < 200))
					{
						temp = grey;
						temp = (temp % 100);
						temp1 = temp;
						temp1 = (temp1 %10);
						temp -= temp1;
						grey = (grey -temp1 -temp);
						grey /= 100;
						temp /= 10;
						printf("%d",grey);
						printf("%d",temp);
						printf("%d",temp1);
						putchar(' ');
					}
					else if(grey == 200)
					{
						putchar('2');
						putchar('0');
						putchar('0');
						putchar(' ');
					}
					else
					{
						temp = grey;
						temp = (temp % 100);
						temp1 = temp;
						temp1 = (temp1 %10);
						temp -= temp1;
						grey = (grey -temp1 -temp);
						grey /= 100;
						temp /= 10;
						printf("%d",grey);
						printf("%d",temp);
						printf("%d",temp1);
						putchar(' ');
					}
				}	
			}						
		}
		else if(ch == '2')
		{
			ch -= 1;		
			putchar(ch);		
			ch = getchar();		
			if((ch != '\n') && (ch != ' ') && (ch != '\t'))			
			{
				printf("Invalid picture format\n");					
				return 1;											
			}
			else
			{
				putchar('\n');
				ch = getchar();
				while((((ch - '0') >= 0) && ((ch - '0') <= 9)) && (flag == 0))    
				{															
					w += (ch - '0');											
					w *= 10;
					putchar(ch);			
					ch = getchar();
					if((ch == '\n') || (ch == '\t') || (ch == ' '))
					{
						flag = 1;
						w /= 10;			
						putchar(ch);		
					}
					else if(((ch - '0') < 0) && ((ch - '0') > 9))
					{
						printf("Invalid width format\n");
						return 2;
					}
				}							
				ch = getchar();				
				flag = 0;					
				while((ch == '\n') || (ch == '\t') || (ch == ' '))           
				{
					putchar(ch);
					ch = getchar();
				}							
				if(((ch - '0') < 0) && ((ch - '0') > 9))					
				{
					printf("Invalid height format\n");					
					return 3;											
				}
				while((((ch - '0') >= 0) && ((ch - '0') <= 9)) && (flag == 0))    
				{															
					h += (ch - '0');											
					h *= 10;
					putchar(ch);		
					ch = getchar();
					if((ch == '\n') || (ch == '\t') || (ch == ' '))
					{
						flag = 1;
						h /= 10;			
						putchar(ch);		
					}
					else if(((ch - '0') < 0) && ((ch - '0') > 9))
					{
						printf("Invalid height format\n");
						return 2;
					}
				}							
				flag = 0;
				ch = getchar();
				if(((ch - '0') < 0) && ((ch - '0') > 9))
				{
					printf("Invalid max value of colour channels\n");    
					return 4;										     
				}
				while((((ch - '0') >= 0) && ((ch - '0') <= 9)) && (flag == 0))    
				{															 
					max_val += (ch - '0');											 
					max_val *= 10;			
					ch = getchar();
					if((ch == '\n') || (ch == '\t') || (ch == ' '))
					{
						flag = 1;
						max_val /= 10;			
						putchar('\n');			
					}
					else if(((ch - '0') < 0) && ((ch - '0') > 9))
					{
						printf("Invalid max value of colour channels\n");	
						return 4;											
					}
				}
				counter = 0;
				while(ch != EOF)
				{
					counter = 0;
					temp = 0;
					temp1 = 0;
					ch = getchar();
					temp = ch;
					counter ++;
					if(((ch - '0') >= 0) && ((ch - '0') <= 9))
					{
						ch = getchar();
						temp1 = ch;
						counter ++;
						if(((ch - '0') >= 0) && ((ch - '0') <= 9))
						{
							ch = getchar();
							counter ++;
						}
					}
					if(counter == 3)
					{
						temp = 100*(temp - '0');
						temp1 = 10*(temp1 - '0');
						ch = (ch - '0');
						temp += (temp1 + ch);
					}
					else if(counter == 2)
					{
						temp = 10*(temp - '0');
						ch = (ch - '0');
						temp += ch;
					}
					else if(counter == 1)
					{
						ch = (ch - '0');
						temp = ch;
					}
					if(temp > (max_val + 1)/2)
					{
						putchar('0');
					}
					else
					{
						putchar('1');
					}
					ch = getchar();
					putchar(' ');
					counter ++;
				}
			}
		}
		else if(ch == '6')
		{
			ch -= 1;		
			putchar(ch);		
			ch = getchar();		
			if((ch != '\n') && (ch != ' ') && (ch != '\t'))			
			{
				printf("Invalid picture format\n");					
				return 1;											
			}
			else
			{
				putchar('\n');
				ch = getchar();
				while(ch!= '\n')
				{
					ch = getchar();
				}
				ch = getchar();
				while((((ch - '0') >= 0) && ((ch - '0') <= 9)) && (flag == 0))    
				{															
					w += (ch - '0');											
					w *= 10;
					putchar(ch);			
					ch = getchar();
					if((ch == '\n') || (ch == '\t') || (ch == ' '))
					{
						flag = 1;
						w /= 10;			
						putchar(ch);		
					}
					else if(((ch - '0') < 0) && ((ch - '0') > 9))
					{
						printf("Invalid width format\n");
						return 2;
					}
				}							
				ch = getchar();				
				flag = 0;					
				while((ch == '\n') || (ch == '\t') || (ch == ' '))           
				{
					putchar(ch);
					ch = getchar();
				}							
				if(((ch - '0') < 0) && ((ch - '0') > 9))					
				{
					printf("Invalid height format\n");					
					return 3;											
				}
				while((((ch - '0') >= 0) && ((ch - '0') <= 9)) && (flag == 0))    
				{															
					h += (ch - '0');											
					h *= 10;
					putchar(ch);		
					ch = getchar();
					if((ch == '\n') || (ch == '\t') || (ch == ' '))
					{
						flag = 1;
						h /= 10;			
						putchar(ch);		
					}
					else if(((ch - '0') < 0) && ((ch - '0') > 9))
					{
						printf("Invalid height format\n");
						return 2;
					}
				}							
				flag = 0;
				ch = getchar();
				if(((ch - '0') < 0) && ((ch - '0') > 9))
				{
					printf("Invalid max value of colour channels\n");    
					return 4;										     
				}
				while((((ch - '0') >= 0) && ((ch - '0') <= 9)) && (flag == 0))    
				{															 
					max_val += (ch - '0');											 
					max_val *= 10;
					putchar(ch);			
					ch = getchar();
					if((ch == '\n') || (ch == '\t') || (ch == ' '))
					{
						flag = 1;
						max_val /= 10;			
						putchar('\n');			
					}
					else if(((ch - '0') < 0) && ((ch - '0') > 9))
					{
						printf("Invalid max value of colour channels\n");	
						return 4;											
					}
				}
				counter = 0;
				while(counter < (h*w*3))
				{
					r = getchar();
					g = getchar();
					b = getchar();
					counter += 3;
					r *= 299;
					g *= 587;
					b *= 114;
					grey = r + g + b;
					grey /= 1000;
					putchar(grey);
				}
			}
		}
		else if(ch == '5')
		{
			ch -= 1;		
			putchar(ch);		
			ch = getchar();		
			if((ch != '\n') && (ch != ' ') && (ch != '\t'))			
			{
				printf("Invalid picture format\n");					
				return 1;											
			}
			else
			{
				putchar('\n');
				ch = getchar();
				while((((ch - '0') >= 0) && ((ch - '0') <= 9)) && (flag == 0))    
				{															
					w += (ch - '0');											
					w *= 10;
					putchar(ch);			
					ch = getchar();
					if((ch == '\n') || (ch == '\t') || (ch == ' '))
					{
						flag = 1;
						w /= 10;			
						putchar(ch);		
					}
					else if(((ch - '0') < 0) && ((ch - '0') > 9))
					{
						printf("Invalid width format\n");
						return 2;
					}
				}							
				ch = getchar();				
				flag = 0;									
				if(((ch - '0') < 0) && ((ch - '0') > 9))					
				{
					printf("Invalid height format\n");					
					return 3;											
				}
				while((((ch - '0') >= 0) && ((ch - '0') <= 9)) && (flag == 0))    
				{															
					h += (ch - '0');											
					h *= 10;
					putchar(ch);		
					ch = getchar();
					if((ch == '\n') || (ch == '\t') || (ch == ' '))
					{
						flag = 1;
						h /= 10;			
						putchar(ch);		
					}
					else if(((ch - '0') < 0) && ((ch - '0') > 9))
					{
						printf("Invalid height format\n");
						return 2;
					}
				}							
				flag = 0;
				ch = getchar();
				if(((ch - '0') < 0) && ((ch - '0') > 9))
				{
					printf("Invalid max value of colour channels\n");    
					return 4;										     
				}
				while((((ch - '0') >= 0) && ((ch - '0') <= 9)) && (flag == 0))    
				{															 
					max_val += (ch - '0');											 
					max_val *= 10;			
					ch = getchar();
					if((ch == '\n') || (ch == '\t') || (ch == ' '))
					{
						flag = 1;
						max_val /= 10;						
					}
					else if(((ch - '0') < 0) && ((ch - '0') > 9))
					{
						printf("Invalid max value of colour channels\n");	
						return 4;											
					}
				}
				counter = 0;
				if((w % 8) == 0)
				{
					while(counter < (h*w))
					{
						temp = 0;
						for(i = 0; i < 8 ; i++)
						{
							ch = getchar();
							if(ch == EOF)
							{
								return 0;
							}
							counter ++;
							if(ch > (max_val +1)/2)
							{
								temp *= 2;
							}
							else
							{
								temp *=2;
								temp += 1;
							}
						}
						putchar(temp);
					}
				}
				else
				{
					while(counter < h*w)
					{
						extra = w % 8;
						temp1 = w - extra;
						temp = 0;
						while(temp1 > 0)
						{
							temp = 0;
							for(i = 0; i < 8; i++)
							{
								ch = getchar();
								counter ++;
								if(ch > (max_val +1)/2)
								{
									temp *= 2;
								}
								else
								{
									temp *=2;
									temp += 1;
								}
							}
							putchar(temp);
							temp1 -= 8;
						}
						temp = 0;
						for(i = 0; i < extra; i++)
						{
							ch = getchar();
							counter ++;
							if(ch > (max_val +1)/2)
							{
								temp *= 2;
							}
							else
							{
								temp *=2;
								temp += 1;
							}
						}
						extra = 8 - extra;
						for(i = 0; i < extra; i++)
						{
							temp *= 2;
							temp += 1;
						}
						putchar(temp);
					}
				}
			}
		}
		else
		{
			printf("Invalid picture format\n");						
			return 1;											
		}
	}
	else
	{
		printf("Invalid picture format\n");						
		return 1;												
	}
return 0;
}
