/*
 * File:   main.c
 * Author: bruno
 *
 * Created on September 1, 2016, 4:44 PM
 */

#define HIGH 1
#define LOW 0
#define STARTTIMER T1CONbits.TMR1ON = 1;
#define STOPTIMER stop_timer();
#include <xc.h>



void setup_timer();
void stop_timer();
void setup_adc();
void setup_interrupts();
void interrupt ISR(void);
void enter_sleep();
void clockSpeed();
int read_adc();
bit speed;

void main(void) {
    //// Clock 31Khz para funcionamento normal.
    OSCCONbits.IRCF = 0;
    //// Subir clock para 8Mhz para efetuar transmissao com 9600 baud rate
    //// Recomendavel utilizar 2Mhz e com 2400 Bauds
    //// ou ate 1Mhz com 1200 bauds
    setup_interrupts();
    setup_timer();
    setup_adc();
    
    
    while(1) {
        
    }
    return;
}

void setup_interrupts()
{
    INTCONbits.PEIE = 1; /// priority interrupt
    PIE1bits.TMR1IE = 1; /// habilitar interrupcao timer
    INTCONbits.INTE = 1; /// habilitar interrupcao externa
    OPTION_REGbits.INTEDG = 1;
    INTCONbits.GIE = 0;/// global interrupt
    
}

void setup_timer()
{
    /// Timer1
    T1CONbits.TMR1CS = 0b10; // clock source frequencia do sistema.
    T1CONbits.T1OSCEN = 1;// usar oscilador proprio
    T1GCONbits.TMR1GE = 0;// disable gate ? (contador?)
    T1GCONbits.T1GPOL = 0;// polaridade do gate.
    T1CONbits.nT1SYNC = 1;
    //T1CONbits.TMR1ON = 1; ligar so quando precisar?
    
}

void setup_adc()
{
    //// referencial de voltagem fixo
    FVRCONbits.FVREN = 1;
    FVRCONbits.ADFVR = 1; /// ligar Referencial de voltagem fixo em 1v para AD
    //// portas
    TRISA0 = 0; /// entrada a0 input
    ANSELAbits.ANSA0 = 0;
    ADCON0bits.ADRMD = 0; ///  12 bits?
    ADCON0bits.CHS = 0; /// an0
    ADCON1bits.ADPREF = 0b11; /// referencial positivo do conversor AD no FVR
    ADCON1bits.ADNREF = 0; /// referencial nevativo do conversor AD
    ADCON1bits.ADCS = 000; //// fonte do clock de sistema
    ADCON1bits.ADFM = 1; /// modo de posicionar bits melhor.
    /// ligar
    ADCON0bits.ADON = 1; /// ligar AD
}

int read_adc()
{
    int resultado;
    ADCON0bits.GO = 1;
    while(ADCON0bits.GO == 1); /// espera converter.
    resultado = (ADRESH<<8) | ADRESL; // colar junto valores
    /// consome mais de 100 ciclos de instrucao
    /// Pesquisar metodo mais eficiente.
    return resultado;
    

}

void interrupt ISR(void)
{
    if(INTCONbits.INTF == 1)/// interrupcao externa e tava dormindo
    {
        INTCONbits.INTF = 0; /// limpa flag
        
    }else if(PIR1bits.TMR1IF == 1)//// interrupcao timer passou um tempo
    {
        STOPTIMER;
        PIR1bits.TMR1IF = 0; /// limpa flag
        
    
    }

}



void enter_sleep()
{
    clockSpeed(LOW); ///underclock
    STARTTIMER;
    SLEEP();
    asm("nop"); /// prox instrucao ao acordar NOP.


}

void clockSpeed()
{
    if(speed == 1)
    {
    
    
    }else
    {
        /// setup clock alto
    
    }

}

void stop_timer()
{
    T1CONbits.TMR1ON = 0;
    TMR1H = 0;
    TMR1L = 0;

}


/**
 ?
?????
??????
???????
???????
???????DOLLYNHO DEFORMADO EM ASCII ABENCOANDO SEU CODIGO
???????
???????
???????
??????
??????
??????
??????
??????
???????
 * TRANSAR QUE EH BOM NADA AMIGUINHO
 */