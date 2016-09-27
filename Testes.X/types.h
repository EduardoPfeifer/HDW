/* 
 * File:   types.h
 * Author: bruno
 *
 * Created on September 8, 2016, 6:21 PM
 */

#ifndef TYPES_H
#define	TYPES_H

/**Redefinindo a constante NULL, caso ela ainda não tenha sido declarada*/
#ifndef NULL
#define NULL ((void*)0)
#endif

/**Indica quando o estado do bit está em HIGH ou SET*/
#ifndef HIGH
#define HIGH 1
#endif

/**Indica quando o estado do bit está em LOW ou CLEAR*/
#ifndef LOW
#define LOW 0
#endif

/**Indica se o pino será de entrada*/
#define INPUT HIGH
/**Indica se o pino será de saída*/
#define OUTPUT LOW

/**Tipo para representar um byte (8 bits)*/
typedef unsigned char byte;

/**Variante de byte, para inteiros*/
typedef unsigned char uint8_t;

/**Tipo para representar dois bytes (16 bits)*/
typedef unsigned int uint16_t;

/**Tipo para representar quatro bytes (32 bits)*/
typedef unsigned long uint32_t;

/**
 * Template para funções do tipo callback para rotinas de serviço de interrupção (ISR)
 */
typedef void (*callback_isr_t)(void);

#endif	/* TYPES_H */

