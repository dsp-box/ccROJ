#ifndef _roj_misc_
#define _roj_misc_

/**
* @type: module
* @brief: In this module, various simple independent (separated) and useful functions and structures are introduced. It is intended to use them in other classes of this project as well as in stand alone programs.
*/

/* **************************************************************************************************************************/
/* headers */

#include "roj-external.hh"

/**
* @type: define
* @brief: This is a flag, which switch on debug printing. Comment this line if you do not want to print the debug communicates.
*/
#define ROJ_DEBUG_ON

/**
* @type: define
* @brief: This is a flag, which switch on a progress. Comment this line if you do not want to print the progress.
*/
#define ROJ_PROGRESS_ON

/**
* @type: define
* @brief: This is a const string, which defines the version number. Please do not change it manually. In order to change modify VERSION file and run 'make version'
*/
#define ROJ_VERSION "0-41"

/**
* @type: define
* @brief: This is the alias to 2 pi value.
*/
#define TWO_PI 6.2831853071795864769252866

/**
* @type: define
* @brief: This is Euler number.
*/
#define E_NUMBER 2.7182818284590452353602975

/**
* @type: define
* @brief: Blackman-Harris window code.
*/
#define ROJ_BH_WINDOW 0

/**
 * @type: define
 * @brief: ODE filter type code
 */
#define ROJ_ODE_FILTER 1

/**
 * @type: define
 * @brief: please comment it if your consol does not support color text
 */
#define ROJ_COLOR_TEXT 

/**
 * @type: define
 * @brief: debug prompt
 */
#define ROJ_DEBUG_PROMPT "\033[1;31m(debug)\033[0m"

/* Blackman Harris window coefficents*/
#define WIN_BH_0  0.3232153788877343
#define WIN_BH_1 -0.4714921439576260
#define WIN_BH_2  0.175534129901972
#define WIN_BH_3 -0.02849699010614994
#define WIN_BH_4  0.001261357088292677

/* ************************************************************************************************************************* */
/* structure definitions */

/**
* @type: struct
* @brief: This is a structure for roj_complex_signal configuration (as well as roj_fourier_spectrum and others).
*/
struct roj_signal_config{

  unsigned int length;
  double start;
  double rate;
};

/**
* @type: struct
* @brief: This is a structure for roj_real_array configuration.
*/
struct roj_array_config{

  unsigned int length;
  double min;
  double max;
};

/**
 * @type: struct
 * @brief: This is a structure for roj_real_matrix configuration.
 */
struct roj_image_config{

  roj_array_config frequency;
  roj_array_config time;
};

/* ************************************************************************************************************************* */
/* function signatures */

/* for diagnostic */
void print_roj_info();
void require_roj_version (int, int);
void print_progress(int, int, const char *);
void print_marker (const char* ="I am here!");

void call_error(const char*);
void call_warning(const char*);
void call_info(const char*, double);
void call_info(const char*, char*);
void call_info(const char*);

void verify_config(roj_signal_config);
void verify_config(roj_array_config);
void verify_config(roj_image_config);

/* for calculations */
double rand_gauss(double =1.0);
double calc_binominal (int, int);
double calc_eulerian (int, int);
int calc_factorial(int);

/* windows */
double calc_blackman_harris(int, int, int =0);

/* others */
int value_comparer (const void*, const void*);
double calc_4x4_det (double [4][4]);

#endif
