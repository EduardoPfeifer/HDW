/*
 * File:   sleep.c
 * Author: victor
 *
 * Created on 29 de Setembro de 2016, 17:08
 */

#include "sleep.h" 
#include <xc.h>
#include <pic16lf1786.h>

void enter_deep_sleep();

void enter_deep_sleep()
{
    SLEEP();
    asm("nop");


}



