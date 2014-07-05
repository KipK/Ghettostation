/*
 *  delay.h
 *
 *  Provides accurate delays for a given number of nanoseconds
 *  This version is part of the Arduino GLCD library.
 *
 *  This file is based on code by Copyright Hans-Juergen Heinrichs (c) 2005
 *
 *  the following comment is from his file
 *  The idea for the functions below was heavily inspired by the
 *  file <avr/delay.h> which is part of the excellent WinAVR
 *  distribution. Therefore, thanks to Marek Michalkiewicz and
 *  Joerg Wunsch.
 *
 *  The idea is to have the GCC preprocessor handle all calculations
 *  necessary for determining the exact implementation of a delay
 *  algorithm. The implementation itself is then inlined into the
 *  user code.
 *  In this way it is possible to always get the code size optimized
 *  delay implementation.
 *
 */
 
 /*
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef _ARDUINO_DELAY_H_
#define _ARDUINO_DELAY_H_

#include <inttypes.h>

#ifndef F_CPU
# warning "Macro F_CPU must be defined"
#endif

/*
 * Forward declaration for all functions with attribute
 * 'always_inline' enforces GCC to inline the code (even
 * if it would be better not to do so from optimization
 * perspective).
 * Without this attribute GCC is free to implement
 * inline code or not (using the keyword 'inline'
 * alone is not sufficient).
 *
 */
static __inline__ void _NOP1( void) __attribute__((always_inline));
static __inline__ void _NOP2( void) __attribute__((always_inline));
static __inline__ void _NOP3( void) __attribute__((always_inline));
static __inline__ void _NOP4( void) __attribute__((always_inline));
static __inline__ void _NOP5( void) __attribute__((always_inline));
static __inline__ void _NOP6( void) __attribute__((always_inline));
static __inline__ void _NOP7( void) __attribute__((always_inline));
static __inline__ void _NOP8( void) __attribute__((always_inline));
static __inline__ void _NOP9( void) __attribute__((always_inline));
static __inline__ void _NOP10(void) __attribute__((always_inline));
static __inline__ void _NOP11(void) __attribute__((always_inline));
static __inline__ void _NOP12(void) __attribute__((always_inline));

static __inline__ void _delay_loop_3(  uint32_t) __attribute__((always_inline));
static __inline__ void _delay_loop_1_x( uint8_t) __attribute__((always_inline));
static __inline__ void _delay_loop_2_x(uint16_t) __attribute__((always_inline));
static __inline__ void _delay_loop_3_x(uint32_t) __attribute__((always_inline));

static __inline__ void _delay_cycles(const double) __attribute__((always_inline));


/*
 * _ N O P x ( void )
 *
 * Code sized optimized NOPs - not using any registers
 *
 * These NOPs will be used for very short delays where
 * it is more code efficient than executing loops.
 *
 */
static __inline__ void _NOP1 (void) { __asm__ volatile ( "nop    " "\n\t" ); }
static __inline__ void _NOP2 (void) { __asm__ volatile ( "rjmp 1f" "\n\t"  "1:" "\n\t" ); }
static __inline__ void _NOP3 (void) { __asm__ volatile ( "lpm    " "\n\t" ); }
static __inline__ void _NOP4 (void) { _NOP3(); _NOP1(); }
static __inline__ void _NOP5 (void) { _NOP3(); _NOP2(); }
static __inline__ void _NOP6 (void) { _NOP3(); _NOP3(); }
static __inline__ void _NOP7 (void) { _NOP3(); _NOP3(); _NOP1(); }
static __inline__ void _NOP8 (void) { _NOP3(); _NOP3(); _NOP2(); }
static __inline__ void _NOP9 (void) { _NOP3(); _NOP3(); _NOP3(); }
static __inline__ void _NOP10(void) { _NOP3(); _NOP3(); _NOP3(); _NOP1(); }
static __inline__ void _NOP11(void) { _NOP3(); _NOP3(); _NOP3(); _NOP2(); }
static __inline__ void _NOP12(void) { _NOP3(); _NOP3(); _NOP3(); _NOP3(); }



/*
 *  _ d e l a y _ l o o p _ 3( uint32_t __count )
 *
 * This delay loop is not used in the code below: It is
 * a supplement to the _delay_loop_1() and _delay_loop_2()
 * within standard WinAVR <arv/delay.h> giving a wider
 * (32 bit) delay range.
 *
 */
static __inline__ void
_delay_loop_3( uint32_t __count )
{
    __asm__ volatile (
        "1: sbiw %A0,1" "\n\t"
        "sbc %C0,__zero_reg__" "\n\t"
        "sbc %D0,__zero_reg__" "\n\t"
        "brne 1b"
        : "=w" (__count)
        : "0" (__count)
    );
}


/*
 *  _ d e l a y _ l o o p _ 1 _ x( uint8_t __count )
 *  _ d e l a y _ l o o p _ 2 _ x( uint16_t  __count )
 *  _ d e l a y _ l o o p _ 4 _ x( uint32_t __count )
 *
 *  These delay loops always have exactly 4(8) cycles per loop.
 *  They use a 8/16/32 bit register counter respectively.
 *
 */
static __inline__ void      /* exactly 4 cycles/loop, max 2**8 loops */
_delay_loop_1_x( uint8_t __n )
{                                               /* cycles per loop      */
    __asm__ volatile (                          /* __n..one        zero */
        "1: dec  %0"   "\n\t"                   /*    1             1   */
        "   breq 2f"   "\n\t"                   /*    1             2   */
        "2: brne 1b"   "\n\t"                   /*    2             1   */
        : "=r" (__n)                            /*  -----         ----- */
        : "0" (__n)                             /*    4             4   */
    );
}

static __inline__ void      /* exactly 4 cycles/loop, max 2**16 loops */
_delay_loop_2_x( uint16_t __n )
{                                               /* cycles per loop      */
    __asm__ volatile (                          /* __n..one        zero */
        "1: sbiw %0,1"   "\n\t"                 /*    2             2   */
        "   brne 1b  "   "\n\t"                 /*    2             1   */
        "   nop      "   "\n\t"                 /*                  1   */
        : "=w" (__n)                            /*  -----         ----- */
        : "0" (__n)                             /*    4             4   */
    );
}

static __inline__ void      /* exactly 8 cycles/loop, max 2**32 loops */
_delay_loop_3_x( uint32_t __n )
{                                               /* cycles per loop      */
    __asm__ volatile (                          /* __n..one        zero */
        "1: sbiw %A0,1           "  "\n\t"      /*    2             2   */
        "   sbc  %C0,__zero_reg__"  "\n\t"      /*    1             1   */
        "   sbc  %D0,__zero_reg__"  "\n\t"      /*    1             1   */
        "   nop                  "  "\n\t"      /*    1             1   */
        "   breq 2f              "  "\n\t"      /*    1             2   */
        "2: brne 1b              "  "\n\t"      /*    2             1   */
        : "=w" (__n)                            /*  -----         ----- */
        : "0" (__n)                             /*    8             8   */
    );
}


/*
 *
 *  _ d e l a y _ c y c l e s (double __ticks_d)
 *
 *  Perform an accurate delay of a given number of processor cycles.
 *
 *  All the floating point arithmetic will be handled by the
 *  GCC Preprocessor and no floating point code will be generated.
 *  Allthough the parameter __ticks_d is of type 'double' this
 *  function can be called with any constant integer value, too.
 *  GCC will handle the casting appropriately.
 *
 *  With an 8 MHz clock e.g., delays ranging from 125 nanoseconds
 *  up to (2**32-1) * 125ns ~= 536,87 seconds are feasible.
 *
 */
static __inline__ void
_delay_cycles(const double __ticks_d)
{
    uint32_t __ticks = (uint32_t)(__ticks_d);
    uint32_t __padding;
    uint32_t __loops;

    /*
     * check for rounding/truncation 
     * that truncated fractional cycle count down to 0
     *
     * If you ask for more than 0 but less than 1
     * you get 1 cycle.
     */

    if(__ticks_d && !__ticks)
        __ticks = 1;

    /*
     * Special optimization for very
     * small delays - not using any register.
     */
    if( __ticks <= 12 )  {              /* this can be done with 4 opcodes      */
        __padding = __ticks;

    /* create a single byte counter */
    } else if( __ticks <= 0x400 )  {
        __ticks -= 1;                   /* caller needs 1 cycle to init counter */
        __loops = __ticks / 4;
        __padding = __ticks % 4;
        if( __loops != 0 )
            _delay_loop_1_x( (uint8_t)__loops );

    /* create a two byte counter */
    } else if( __ticks <= 0x40001 )  {
        __ticks -= 2;                   /* caller needs 2 cycles to init counter */
        __loops = __ticks / 4;
        __padding = __ticks % 4;
        if( __loops != 0 )
            _delay_loop_2_x( (uint16_t)__loops );

    /* create a four byte counter */
    } else  {
        __ticks -= 4;                   /* caller needs 4 cycles to init counter */
        __loops = __ticks / 8;
        __padding = __ticks % 8;
        if( __loops != 0 )
            _delay_loop_3_x( (uint32_t)__loops );
    }

    if( __padding ==  1 )  _NOP1();
    if( __padding ==  2 )  _NOP2();
    if( __padding ==  3 )  _NOP3();
    if( __padding ==  4 )  _NOP4();
    if( __padding ==  5 )  _NOP5();
    if( __padding ==  6 )  _NOP6();
    if( __padding ==  7 )  _NOP7();
    if( __padding ==  8 )  _NOP8();
    if( __padding ==  9 )  _NOP9();
    if( __padding == 10 ) _NOP10();
    if( __padding == 11 ) _NOP11();
    if( __padding == 12 ) _NOP12();
}


/*
 *   _ d e l a y _ n s (double __ns)
 *   _ d e l a y _ u s (double __us)
 *
 *   Perform a very exact delay with a resolution as accurate as a
 *   single CPU clock (the macro F_CPU is supposed to be defined to a
 *   constant defining the CPU clock frequency in Hertz).
 *
 */
#define _delayNanoseconds(__ns)     _delay_cycles( (double)(F_CPU)*((double)__ns)/1.0e9 + 0.5 )

#endif /* _ARDUINO_DELAY_H_ */
