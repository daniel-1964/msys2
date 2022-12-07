/*
 * random.h
 *
 * BSD random function to support the TclX random command on SysV based
 * systems.
 *-----------------------------------------------------------------------------
 * $Id: random.h,v 1.1 2001/10/24 23:31:47 hobbs Exp $
 *-----------------------------------------------------------------------------
 */


/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _RANDOM_H
#define _RANDOM_H

/*
 * random.h:
 *
 * An improved random number generation package.  In addition to the standard
 * rand()/srand() like interface, this package also has a special state info
 * interface.  The initstate() routine is called with a seed, an array of
 * bytes, and a count of how many bytes are being passed in; this array is
 * then initialized to contain information for random number generation with
 * that much state information.  Good sizes for the amount of state
 * information are 32, 64, 128, and 256 bytes.  The state can be switched by
 * calling the setstate() routine with the same array as was initiallized
 * with initstate().  By default, the package runs with 128 bytes of state
 * information and generates far better random numbers than a linear
 * congruential generator.  If the amount of state information is less than
 * 32 bytes, a simple linear congruential R.N.G. is used.
 *
 * Internally, the state information is treated as an array of longs; the
 * zeroeth element of the array is the type of R.N.G. being used (small
 * integer); the remainder of the array is the state information for the
 * R.N.G.  Thus, 32 bytes of state information will give 7 longs worth of
 * state information, which will allow a degree seven polynomial.  (Note:
 * the zeroeth word of state information also has some other information
 * stored in it -- see setstate() for details).
 *
 * The random number generation technique is a linear feedback shift register
 * approach, employing trinomials (since there are fewer terms to sum up that
 * way).  In this approach, the least significant bit of all the numbers in
 * the state table will act as a linear feedback shift register, and will
 * have period 2^deg - 1 (where deg is the degree of the polynomial being
 * used, assuming that the polynomial is irreducible and primitive).  The
 * higher order bits will have longer periods, since their values are also
 * influenced by pseudo-random carries out of the lower bits.  The total
 * period of the generator is approximately deg*(2**deg - 1); thus doubling
 * the amount of state information has a vast influence on the period of the
 * generator.  Note: the deg*(2**deg - 1) is an approximation only good for
 * large deg, when the period of the shift register is the dominant factor.
 * With deg equal to seven, the period is actually much longer than the
 * 7*(2**7 - 1) predicted by this formula.
 */

/*
 * srandom:
 *
 * Initialize the random number generator based on the given seed.  If the
 * type is the trivial no-state-information type, just remember the seed.
 * Otherwise, initializes state[] based on the given "seed" via a linear
 * congruential generator.  Then, the pointers are set to known locations
 * that are exactly rand_sep places apart.  Lastly, it cycles the state
 * information a given number of times to get rid of any initial dependencies
 * introduced by the L.C.R.N.G.  Note that the initialization of randtbl[]
 * for default usage relies on values produced by this routine.
 */
void srandom(unsigned int x);

/*
 * initstate:
 *
 * Initialize the state information in the given array of n bytes for future
 * random number generation.  Based on the number of bytes we are given, and
 * the break values for the different R.N.G.'s, we choose the best (largest)
 * one we can and set things up for it.  srandom() is then called to
 * initialize the state information.
 *
 * Note that on return from srandom(), we set state[-1] to be the type
 * multiplexed with the current value of the rear pointer; this is so
 * successive calls to initstate() won't lose this information and will be
 * able to restart with setstate().
 *
 * Note: the first thing we do is save the current state, if any, just like
 * setstate() so that it doesn't matter when initstate is called.
 *
 * Returns a pointer to the old state.
 */
const char *initstate(unsigned int seed, const char *arg_state, int n);


/*
 * setstate:
 *
 * Restore the state from the given state array.
 *
 * Note: it is important that we also remember the locations of the pointers
 * in the current state information, and restore the locations of the pointers
 * from the old state information.  This is done by multiplexing the pointer
 * location into the zeroeth word of the state information.
 *
 * Note that due to the order in which things are done, it is OK to call
 * setstate() with the same state as the current state.
 *
 * Returns a pointer to the old state information.
 */
const char *setstate(const char *arg_state);

/*
 * random:
 *
 * If we are using the trivial TYPE_0 R.N.G., just do the old linear
 * congruential bit.  Otherwise, we do our fancy trinomial stuff, which is
 * the same in all the other cases due to all the global variables that have
 * been set up.  The basic operation is to add the number at the rear pointer
 * into the one at the front pointer.  Then both pointers are advanced to
 * the next location cyclically in the table.  The value returned is the sum
 * generated, reduced to 31 bits by throwing away the "least random" low bit.
 *
 * Note: the code takes advantage of the fact that both the front and
 * rear pointers can't wrap on the same call by not testing the rear
 * pointer if the front one has wrapped.
 *
 * Returns a 31-bit random number.
 */
long random();

#endif /* _RANDOM_H */
