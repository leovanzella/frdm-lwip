#ifndef __BSP_TYPES_H_
#define __BSP_TYPES_H_

#include <stdint.h>

/**
 * @brief Boolean Type definition
 */
typedef enum {FALSE = 0, TRUE = !FALSE} Bool;

/**
 * @brief Flag Status and Interrupt Flag Status type definition
 */
typedef enum {RESET = 0, SET = !RESET} FlagStatus, IntStatus, SetState;
#define PARAM_SETSTATE(State) ((State == RESET) || (State == SET))

/**
 * @brief Functional State Definition
 */
typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define PARAM_FUNCTIONALSTATE(State) ((State == DISABLE) || (State == ENABLE))

/**
 * @ Status type definition
 */
typedef enum {ERROR = 0, SUCCESS = !ERROR} Status;

/* NULL pointer */
#ifndef NULL
#define NULL ((void*) 0)
#endif

/* _BIT(n) sets the bit at position "n"
 * _BIT(n) is intended to be used in "OR" and "AND" expressions:
 * e.g., "(_BIT(3) | _BIT(7))".
 */
#undef _BIT
/* Set bit macro */
#define _BIT(n) (1<<n)

/* _SBF(f,v) sets the bit field starting at position "f" to value "v".
 * _SBF(f,v) is intended to be used in "OR" and "AND" expressions:
 * e.g., "((_SBF(5,7) | _SBF(12,0xF)) & 0xFFFF)"
 */
#undef _SBF
/* Set bit field macro */
#define _SBF(f,v) (v<<f)

#endif /* __BSP_TYPES_H_ */
