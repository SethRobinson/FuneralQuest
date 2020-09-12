
/*
 * A simple high-precision timer package (version 1.0)
 *
 * Before using the functions from this package, you 
 * should call init_timer(). From then on you may call
 * get_time() to get the number of elapsed milliseconds 
 * since your call to init_timer(). Note that it's okay
 * to call init_timer() more than once; it will only be
 * initialized the first time it's called.
 *
 * Note that high-resolution timing is not necessarily 
 * supported on all platforms. When the hires timer is 
 * not available, get_time() still works, but it uses 
 * less accurate timer functions, typically with 10 ms 
 * resolution.
 *
 * Written by Matthijs Hollemans <matthijs@shakeyourass.org>
 *
 * Public domain. No warranties expressed or implied.
 */

#ifndef TIMER_H
#define TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

void init_timer(void);

unsigned int get_time(void);

#ifdef __cplusplus
}
#endif

#endif /* TIMER_H */
