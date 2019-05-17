/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012, 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_main.c
* Version      : Code Generator for RL78/G10 V1.05.02.03 [08 May 2018]
* Device(s)    : R5F10Y47
* Tool-Chain   : CCRL
* Description  : This file implements main function.
* Creation Date: 16-05-2019
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_it.h"
#include "r_cg_adc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"
static void mode_1(void);
static void mode_2(void);
static void start_op(void);
int fall_edge=0;
unsigned long start_counter=0;
static void trans_op(void);
static void tik_tak(void);
static void tak_tik(void);
 enum Machine_state {
	Start=0,
	Transition,
	Run,
};

int _inloop=0;
enum Machine_state _state=Start;
void start_count_check(void);
unsigned long Run_delay_table[10]={8267,8158,8052,7949,7848,7750,7654,7561,7470,7381};
long run_counter=0;
unsigned table_index=0;
unsigned long Run_delay=8267;
void run_count_check(void);
unsigned int Min_Run_Delay=6000;//the value of time period at 150Hz
/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

static void R_MAIN_UserInit(void);
/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    //_state=Start;
    /* Start user code. Do not edit comment generated here */
    while (1U)
    {
	    start_count_check();
	    
	    switch(_state){
		    case Start:
		               start_op();
			       start_counter++;
		                break;
	            case Transition:
		               trans_op();
			       
			       break;
	            case Run:
		          if(fall_edge==1)
			  {
				  tik_tak();
			  }
		         else
			 {
				 tak_tik();
			 }
	                
			break;
    }
    /* End user code. Do not edit comment generated here */
}
}
/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    EI();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
void DelayUs(unsigned long i)
{
	char c;
	do
	{
		c=1;
		do
		c=c-1;
		while(c>0);
	}while(--i>0);
}
void DelayMs(unsigned int i)
{
	unsigned int c;
	do
	{
		c=1500;
		do
		c=c-1;
		while(c>0);
	}while(--i>0);
	
}
static void mode_1(void)
{
	PWM_L1 = 0;          PWM_H2 = 0;
        PWM_H1 = 1;          PWM_L2 = 1;
}
static void mode_2(void)
{
	PWM_L1 = 1;          PWM_H2 = 1;
        PWM_H1 = 0;          PWM_L2 = 0	;
}
static void start_op(void)
{
	if(HALL_FORWARD_PHASE)
	    {
		    mode_1();
		    while(HALL_FORWARD_PHASE);
	    }
	    
        else
	{
		mode_2();
	        while(!HALL_FORWARD_PHASE);
	}
	
}
static void trans_op(void)
{
	if(HALL_FORWARD_PHASE)
	    {
		    mode_1();
		    fall_edge=1;
	    }
	else
	{
		mode_2();
	        fall_edge=0;
	}
	_state=Run;
	//_inloop=0;
}
static void tik_tak(void)
{
	while (1U)
	{
		
		//if(_inloop==0)
		{
		run_count_check();
		DelayUs(Run_delay);//some need to be defined
		mode_2();
		DelayUs(Run_delay);
		mode_1();
		}
		//else
		//break;
		
		
	run_counter++;	
	}
}
static void tak_tik(void)
{
	while(1U)
	{
		//if (_inloop==0)
		{
		run_count_check();
		DelayUs(Run_delay);//some need to be defined
		mode_1();
		DelayUs(Run_delay);
		mode_2();
		run_counter++;
		}
		//else
		//break;
			
	}
}
void start_count_check(void)
{
	if(start_counter>4000)//time on start seqence
	{
		_state=Transition;
		start_counter=0;
	}
	
}
void run_count_check(void)
{ 
	if(run_counter>200)
	{
		if(Run_delay>Min_Run_Delay)
		{
			table_index++;
			//Run_delay=Run_delay_table[table_index];
			Run_delay=Run_delay-5;
			//_inloop=1;
			//_state=Transition;
		}
		run_counter=0;
		
}
}