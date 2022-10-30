#include "STD_types.h"
#include "LCD_interface.h"
#include "KYP_interface.h"
#define F_CPU 8000000UL
#include "util/delay.h"

//global array that holds the values of pow(10,i)
uint16 Power_arr[]={1,10,100,1000,10000,100000};

int main(void)
{
	//Initializations
	LCD_vInit();
	KYP_vInit();
	
	sint16 Num1=0 , Num2=0 ;  //operation numbers
	uint8 Oper;				  //operation sign
	uint8 Check_press=0,flag=0;
	sint32 Result;			  // operation result
	uint8 ARR_NUM[8];		  //array to hold the numbers from keypad
	sint8 counter_num=0 , counter_pow =0; //counter_num for the ARR_NUM & counter_pow for Power_arr
	
    while(1)
    {
		//an outer array that breaks when the user enters a sign operation
		while(Check_press != '+' && Check_press != '-' && Check_press != '*' && Check_press != '/' && Check_press != '=')
		{
			//take a value from the keypad
			do{
				Check_press = KYP_u8CheckPress();
			}
			while(Check_press==NOT_PRESSED);
			while(KYP_u8CheckPress() != NOT_PRESSED);
			
			//flag = 1 if the operation is not the first one
			if(flag == 1)
			{
				LCD_vClearScreen();
				flag = 0;
			}
			
			//display the number entered and save it in the array
			LCD_vSendData(Check_press);
			ARR_NUM[counter_num]=Check_press-48;
			
			//check negative number
			if(Check_press == '-' && counter_num == 0)
			{
				ARR_NUM[counter_num]=Check_press;
				Check_press='0';
			}
			counter_num++;
		}
		counter_num--;
		//last entered is sign operator that broke the loop
		Oper = Check_press;
		
		
		//get the whole number from the array by the equation
		while(counter_num > 0)
		{
			counter_num--;
			//check if the first number is negative
			if(counter_num == 0 && ARR_NUM[0]=='-')
			{
				Num1 = ~Num1+1;
			}
			else
			{
				Num1 = Num1 + ARR_NUM[counter_num]*Power_arr[counter_pow];
			}
			counter_pow++;
		}
		
		counter_pow=0; // 0 for the second number
		//if the operation entered = 0 , then the default value for num2 =0
		if(Oper == '=')
		{
			Num2=0;
		}	
		else
		{
			//an external loop that breaks when operation=0
			while(Check_press != '=')
			{
				do{
					Check_press = KYP_u8CheckPress();
				}
				while(Check_press==NOT_PRESSED);
				while(KYP_u8CheckPress() != NOT_PRESSED);
				LCD_vSendData(Check_press);
				ARR_NUM[counter_num]=Check_press-48;
				counter_num++;
			}
			counter_num--;
			
			while(counter_num > 0)
			{
				counter_num--;
				Num2 = Num2 + ARR_NUM[counter_num]*Power_arr[counter_pow];
				counter_pow++;
			}
		}	
		
		//switch case to check the operation
		switch(Oper)
		{
			case '+':
			Result = (sint16)Num1 + (sint16)Num2;
			break;
			
			case '-':
			Result = (sint16)Num1 - (sint16)Num2;
			break;
			
			case '*':
			Result = (sint16)Num1 * (sint16)Num2;
			break;
			
			case '/':
			Result = (sint16)Num1 / (sint16)Num2;
			break;
			
			case '=':
			Result = Num1;
			break;	
		}
		//check the negative numbers
		if(Result < 0)
		{
			//negative sign
			LCD_vSendData('-');
			//2's complement to get the negative number
			Result = ~Result + 1;
		}
		//display the result
		LCD_vSendNumber(Result);
		
		//returning the variables to their default values for the next operation
		Num1=Num2=Check_press=0;
		Result=0;
		Oper=0;
		flag = 1;	
		counter_pow = counter_num =0;								
    }
}