/* *************************************************** *
 * This file is a part of ccROJ project (version 0-46) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#ifndef _roj_window_generator_
#define _roj_window_generator_

/**
* @type: class
* @brief: Definitions of roj_window_generator class.
*/

/* ************************************************************************************************************************* */
/* headers and declarations */

#include "roj-external.hh"
#include "roj-misc.hh"

#include "roj-complex-signal.hh"
class roj_complex_signal;
  
/* ************************************************************************************************************************* */
/* window generator class definition */

class roj_window_generator{
private:

  /* configuration */
  double m_crate;
  double m_rate;
  int m_length;
  char m_type;
  
  /* Blackman-Harris window */
  roj_complex_signal* get_blackman_harris_win ();
  roj_complex_signal* get_blackman_harris_dwin ();
  roj_complex_signal* get_blackman_harris_d2win ();
  
  roj_complex_signal* get_empty_window ();

 public:

  /* construction */
  /* ******************************** */
  roj_window_generator (double);

  /* getter/setter methods */
  /* ******************************** */
  double get_rate();
  void set_chirp_rate(double);
  double get_chirp_rate();    
  void set_length(int);
  int get_length();
  void set_type(char);

  /* window generation */
  /* ******************************** */
  roj_complex_signal* get_window(int =0, int =0);

  /* calc methods */
  /* ******************************** */
  double calc_gain(int=0, int=0); 
  double calc_duration();

};

#endif

