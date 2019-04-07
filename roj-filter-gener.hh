/* *************************************************** *
 * This file is a part of ccROJ project (version 0-48) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#ifndef _roj_filter_gener_
#define _roj_filter_gener_

/**
* @type: class
* @brief: Definition of roj_ode_filter_generator class, which is an factory for filter producing.
*/

/* ************************************************************************************************************************* */
/* headers and declarations */

#include "roj-external.hh"
#include "roj-misc.hh"

#include "roj-filter.hh"
class roj_filter;

#include "roj-complex-signal.hh"
struct roj_signal_config;
class roj_complex_signal;

/* ************************************************************************************************************************* */
/* filter gener class definition */

class roj_filter_generator{
private:

  int m_type;

  unsigned int m_order;
  unsigned int m_outlen;

  double m_rate;
  double m_spread;
  double m_frequency;
  
public:

  /* construction */
  /* ******************************** */
  roj_filter_generator(double);

  /* setter / getter methods */
  /* ******************************** */
  void set_type(int);
  int get_type();
  void set_spread(double);
  double get_spread();
  double get_frequency();
  void set_frequency(double);
  unsigned int get_delay();
  void set_delay(unsigned int);
  void set_order(unsigned int);
  unsigned int get_order();
  double get_rate();
  void set_rate(double);
  
  /* generate methods */
  /* ******************************** */
  roj_filter* get_ode_filter ();  
  roj_filter* get_filter ();
};

#endif
