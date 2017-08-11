/* *************************************************** *
 * This file is a part of ccROJ project (version 0-44) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#include "roj-misc.hh"

/* ************************************************************************************************************************* */
/**
* @type: function
* @brief: The routine prints info about the project.
*/
void print_roj_info (){

#ifdef ROJ_DEBUG_ON
#ifdef ROJ_COLOR_TEXT 
  fprintf(stderr, "\033[1;35m");
#endif

  fprintf(stderr, "         ▗▄▄  ▄▄  ▗▄ \n");
  fprintf(stderr, "  ▄▖  ▄▖ ▐ ▝▌▗▘▝▖  ▐ \n");
  fprintf(stderr, " ▐▘▝ ▐▘▝ ▐▄▄▘▐  ▌  ▐ \n");
  fprintf(stderr, " ▐   ▐   ▐ ▝▖▐  ▌  ▐ \n");
  fprintf(stderr, " ▝▙▞ ▝▙▞ ▐  ▌ ▙▟ ▝▄▞  v. %s\n", ROJ_VERSION);
  fprintf(stderr, "▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\n");

#ifdef ROJ_COLOR_TEXT 
  fprintf(stderr, "\033[0m\n");
#else
  fprintf(stderr, "\n");
#endif
#endif
}

/**
* @type: function
* @brief: The routine checks that a given version is the same as the current (installed) version.
*
* @param [in] a_major: A major part of the version.
* @param [in] a_minor: A minor part of the version
*/
void require_roj_version (int a_major, int a_minor){
  
  int major = -1;
  int minor = -1;

  sscanf(ROJ_VERSION, " %d-%d ", &major, &minor);
  
  if (major!=a_major or minor!=a_minor)
    call_error("wrong ccROJ version");  
}

/**
* @type: function
* @brief: The routine checks that the current (installed) version is the same or newer than a given version.
*
* @param [in] a_major: A major part of the version.
* @param [in] a_minor: A minor part of the version
*/
void require_equal_or_newer_roj_version (int a_major, int a_minor){
  
  int major = -1;
  int minor = -1;

  sscanf(ROJ_VERSION, " %d-%d ", &major, &minor);
  
  if (major<a_major)
    call_error("wrong ccROJ version");  
  if (minor<a_minor)
    call_error("wrong ccROJ version");  
}

/* ************************************************************************************************************************* */
/**
* @type: function
* @brief: This function shoud be called if an error is detected.
*
* @param [in] a_info: A message, statement for printing in stderr.
*/
void call_error (const char* a_info){
  
#ifdef ROJ_DEBUG_ON
#ifdef ROJ_COLOR_TEXT 
  fprintf(stderr, "\033[1;31m(error)\033[0m %s\n", a_info);
#else
  fprintf(stderr, "(error) %s\n", a_info);
#endif
  
  /* TODO: save this msg to a logfile */
  
#endif
  
  exit(EXIT_FAILURE);
}

/**
* @type: function
* @brief: This function shoud be called if a warning is detected.
*
* @param [in] a_info: A message, statement for printing in stderr.
*/
void call_warning (const char* a_info){

#ifdef ROJ_DEBUG_ON
#ifdef ROJ_COLOR_TEXT 
  fprintf(stderr, "\033[1;33m(warning)\033[0m %s\n", a_info);
#else
  fprintf(stderr, "(warning) %s\n", a_info);
#endif

  /* TODO: save this msg to a logfile */

#endif
}

/**
* @type: function
* @brief: This function shoud be called if any info is detected.
*
* @param [in] a_info: A message, statement for printing in stderr.
*/
void call_info (const char* a_info){

#ifdef ROJ_DEBUG_ON
#ifdef ROJ_COLOR_TEXT 
  fprintf(stderr, "\033[1;32m(info)\033[0m %s\n", a_info);
#else
  fprintf(stderr, "(info) %s\n", a_info);
#endif

  /* TODO: save this msg to a logfile */

#endif
}

/**
* @type: function
* @brief: This function shoud be called if any info is detected.
*
* @param [in] a_info: A message (const), statement for printing in stderr.
* @param [in] a_string: A message (dynamic), statement for printing in stderr.
*/
void call_info (const char* a_info, char *a_string){

#ifdef ROJ_DEBUG_ON

#ifdef ROJ_COLOR_TEXT 
  fprintf(stderr, "\033[1;32m(info)\033[0m %s%s\n", a_info, a_string);
#else
  fprintf(stderr, "(info) %s%s\n", a_info, a_string);
#endif

  /* TODO: save this msg to a logfile */

#endif
}

/**
* @type: function
* @brief: This function shoud be called if any info is detected.
*
* @param [in] a_info: A message (const), statement for printing in stderr.
* @param [in] a_number: A number for printing in stderr.
*/
void call_info (const char* a_info, double a_number){

#ifdef ROJ_DEBUG_ON
#ifdef ROJ_COLOR_TEXT 
  fprintf(stderr, "\033[1;32m(info)\033[0m %s%g\n", a_info, a_number);
#else
  fprintf(stderr, "(info) %s%g\n", a_info, a_number);
#endif
 
  /* TODO: save this msg to a logfile */

#endif
}

/**
* @type: function
* @brief: This function shoud be called for marker printing.
*
* @param [in] a_info: A message, statement for printing in stderr.
*/
void print_marker (const char* a_info){
  
#ifdef ROJ_DEBUG_ON
#ifdef ROJ_COLOR_TEXT 
  fprintf(stderr, "\033[1;31m(marker)\033[0m %s\n", a_info);
#else
  fprintf(stderr, "(marker) %s\n", a_info);
#endif
#endif
}

/* ************************************************************************************************************************* */
/**
* @type: function
* @brief: The function prints a progress in the text mode. It can be useful for long calculation for monitoring its progress.
*
* @param [in] a_curr: The current position of a progress.
* @param [in] a_len: A length of the bar.
* @param [in] a_info: A printing label.
*/
void print_progress (int a_curr, int a_len, const char *a_info){

#ifdef ROJ_DEBUG_ON
#ifdef ROJ_PROGRESS_ON

  if(a_len>0){
    fprintf(stderr,"\r%s %.3f", a_info, (float)a_curr/a_len);
    fflush(stderr);
  }
  else
#ifdef ROJ_COLOR_TEXT 
    fprintf(stderr,"\r\033[1;34m(done)\033[0m %s\n", a_info);
#else
    fprintf(stderr,"\r(done) %s\n", a_info);
#endif

#endif
#endif
}

/* ************************************************************************************************************************* */
/**
* @type: function
* @brief: This function verify the correctness of a signal config. It call error if not.
*
* @param [in] a_conf: configuration to check.
*/
void verify_config(roj_signal_config a_conf){

  if (a_conf.length<1) call_error("signal length is not valid");
  if (a_conf.rate<=0.0)  call_error("signal rate is not valid");
}

/**
* @type: function
* @brief: This function verify the correctness of an array config. It call error if not.
*
* @param [in] a_conf: configuration to check.
*/
void verify_config(roj_array_config a_conf){

  if (a_conf.length<1) call_error("array length is not valid");
  if (a_conf.min>=a_conf.max) call_error("array config is not valid");
}

/**
* @type: function
* @brief: This function verify the correctness of an image config. It call error if not.
*
* @param [in] a_conf: configuration to check.
*/
void verify_config(roj_image_config a_conf){
  
  verify_config(a_conf.y);
  verify_config(a_conf.x);
}

/* ************************************************************************************************************************* */
/**
* @type: function
* @brief: This function returns random number with normal probability using Box-Muller transform (G. E. P. Box and Mervin E. Muller, A Note on the Generation of Random Normal Deviates, The Annals of Mathematical Statistics (1958), Vol. 29, No. 2).
*
* @param [in] a_sigma: Standard deviation.
*
* @return: Random number.
*/
double rand_gauss (double a_sigma){
  
  double u1 = (double)rand() / RAND_MAX;
  if (u1 > 0.99999999999)
    u1 = 0.99999999999;
  double u2 = a_sigma * sqrt( 2.0 * log( 1.0 / (1.0 - u1) ) );
  
  u1 = (double)rand() / RAND_MAX;
  if (u1 > 0.99999999999) 
    u1 = 0.99999999999;
  
  return (double) (u2 * cos(2 * M_PI * u1));
}

/**
* @type: function
* @brief: This function returns factorial of the argument.
*
* @param [in] a_number: The number for which factorial is roj_calculated.
*
* @return: Factorial value.
*/
int calc_factorial (int a_number){

  if(a_number<0) 
    call_error("factorial < 0");
  if (a_number == 1 || a_number == 0) return 1;
  else return calc_factorial(a_number - 1) * a_number;
}

/* ************************************************************************************************************************* */
/**
* @type: function
* @brief: This function returns binominal coefficient of two arguments.
*
* @param [in] a_kval: A first argument.
* @param [in] a_nval: A second argument.
*
* @return: Binominal coefficient.
*/
double calc_binominal (int a_kval, int a_nval){

  double denominativ = calc_factorial(a_nval) * calc_factorial(a_kval-a_nval);
  double nominativ = calc_factorial(a_kval); 
  return nominativ / denominativ;
}

/**
* @type: function
* @brief: This function returns Eulerian number of two arguments.
*
* @param [in] a_kval: A first argument.
* @param [in] a_nval: A second argument.
*
* @return: Eulerian number.
*/
double calc_eulerian (int a_kval, int a_nval){

  double y = 0.0;
  for (int m=0; m<=a_nval; m++)
    y +=  pow(-1.0, m) * calc_binominal(a_kval+1, m) * pow(a_nval-m, a_kval);  
  return y;
}

/* ************************************************************************************************************************* */
/**
* @type: function
* @brief: This function returns one coefficient of blackman-Harris window.
*
* @param [in] a_nr: An index of returned coefficient.
* @param [in] a_len: A window length (in coefficients).
* @param [in] a_d_order: An order of derivative.
*
* @return: The coefficient value.
*/
double calc_blackman_harris (int a_nr, int a_len, int a_d_order){

  if(a_d_order<0)
    call_error("not defined order in window calculation");

  switch(a_d_order){
  case 0:
    return
      WIN_BH_0
      + WIN_BH_1 * cos(1.0*TWO_PI*(double)a_nr/a_len)
      + WIN_BH_2 * cos(2.0*TWO_PI*(double)a_nr/a_len)
      + WIN_BH_3 * cos(3.0*TWO_PI*(double)a_nr/a_len)
      + WIN_BH_4 * cos(4.0*TWO_PI*(double)a_nr/a_len);  
    
  case 1:
    return
      - 1.0*TWO_PI * WIN_BH_1 * sin(1.0*TWO_PI * (double)a_nr/a_len)
      - 2.0*TWO_PI * WIN_BH_2 * sin(2.0*TWO_PI * (double)a_nr/a_len)
      - 3.0*TWO_PI * WIN_BH_3 * sin(3.0*TWO_PI * (double)a_nr/a_len)
      - 4.0*TWO_PI * WIN_BH_4 * sin(4.0*TWO_PI * (double)a_nr/a_len); 

  case 2:
    return
      - pow(1.0*TWO_PI, 2.0) * WIN_BH_1 * cos(1.0*TWO_PI * (double)a_nr/a_len)
      - pow(2.0*TWO_PI, 2.0) * WIN_BH_2 * cos(2.0*TWO_PI * (double)a_nr/a_len)
      - pow(3.0*TWO_PI, 2.0) * WIN_BH_3 * cos(3.0*TWO_PI * (double)a_nr/a_len)
      - pow(4.0*TWO_PI, 2.0) * WIN_BH_4 * cos(4.0*TWO_PI * (double)a_nr/a_len);   

  default: /* not tested for a_d_order > 2 */
    return
        pow(1.0*TWO_PI, a_d_order) * WIN_BH_1 * cos(1.0*TWO_PI * (double)a_nr/a_len + 0.5*M_PI*a_d_order)
      + pow(2.0*TWO_PI, a_d_order) * WIN_BH_2 * cos(2.0*TWO_PI * (double)a_nr/a_len + 0.5*M_PI*a_d_order)
      + pow(3.0*TWO_PI, a_d_order) * WIN_BH_3 * cos(3.0*TWO_PI * (double)a_nr/a_len + 0.5*M_PI*a_d_order)
      + pow(4.0*TWO_PI, a_d_order) * WIN_BH_4 * cos(4.0*TWO_PI * (double)a_nr/a_len + 0.5*M_PI*a_d_order);  
  }  
}

/* ************************************************************************************************************************* */
/**
* @type: function
* @brief: This routine is used for sorting and compares two double values.
*
* @param [in] a_arg1: A pointer to double value.
* @param [in] a_arg2: A pointer to double value.
*
* @return: The result of the comarison which is compatible with the 'qsort' function.
*/
int value_comparer (const void * a_arg1, const void * a_arg2){

  double _a = *(double*)a_arg1;
  double _b = *(double*)a_arg2;

  if(_a < _b)
    return -1;
  
  if(_a == _b)
    return 0;
  
  return 1;
}

/* ************************************************************************************************************************* */
/**
* @type: function
* @brief: This function calculates the determinat of 4x4 matrix.
*
* @param [in] a_matrix: a given matrix.
*
* @return: calculated determinant.
*/
double calc_4x4_det (double a_matrix[4][4]){

  double det = 
    + a_matrix[0][3] * a_matrix[1][2] * a_matrix[2][1] * a_matrix[3][0]
    - a_matrix[0][2] * a_matrix[1][3] * a_matrix[2][1] * a_matrix[3][0]
    - a_matrix[0][3] * a_matrix[1][1] * a_matrix[2][2] * a_matrix[3][0]
    + a_matrix[0][1] * a_matrix[1][3] * a_matrix[2][2] * a_matrix[3][0]
    + a_matrix[0][2] * a_matrix[1][1] * a_matrix[2][3] * a_matrix[3][0]
    - a_matrix[0][1] * a_matrix[1][2] * a_matrix[2][3] * a_matrix[3][0]
    - a_matrix[0][3] * a_matrix[1][2] * a_matrix[2][0] * a_matrix[3][1]
    + a_matrix[0][2] * a_matrix[1][3] * a_matrix[2][0] * a_matrix[3][1]
    + a_matrix[0][3] * a_matrix[1][0] * a_matrix[2][2] * a_matrix[3][1]
    - a_matrix[0][0] * a_matrix[1][3] * a_matrix[2][2] * a_matrix[3][1]
    - a_matrix[0][2] * a_matrix[1][0] * a_matrix[2][3] * a_matrix[3][1]
    + a_matrix[0][0] * a_matrix[1][2] * a_matrix[2][3] * a_matrix[3][1]
    + a_matrix[0][3] * a_matrix[1][1] * a_matrix[2][0] * a_matrix[3][2]
    - a_matrix[0][1] * a_matrix[1][3] * a_matrix[2][0] * a_matrix[3][2]
    - a_matrix[0][3] * a_matrix[1][0] * a_matrix[2][1] * a_matrix[3][2]
    + a_matrix[0][0] * a_matrix[1][3] * a_matrix[2][1] * a_matrix[3][2]
    + a_matrix[0][1] * a_matrix[1][0] * a_matrix[2][3] * a_matrix[3][2]
    - a_matrix[0][0] * a_matrix[1][1] * a_matrix[2][3] * a_matrix[3][2]
    - a_matrix[0][2] * a_matrix[1][1] * a_matrix[2][0] * a_matrix[3][3]
    + a_matrix[0][1] * a_matrix[1][2] * a_matrix[2][0] * a_matrix[3][3]
    + a_matrix[0][2] * a_matrix[1][0] * a_matrix[2][1] * a_matrix[3][3]
    - a_matrix[0][0] * a_matrix[1][2] * a_matrix[2][1] * a_matrix[3][3]
    - a_matrix[0][1] * a_matrix[1][0] * a_matrix[2][2] * a_matrix[3][3]
    + a_matrix[0][0] * a_matrix[1][1] * a_matrix[2][2] * a_matrix[3][3];

  return det;
}
