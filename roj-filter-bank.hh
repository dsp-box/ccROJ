/* *************************************************** *
 * This file is a part of ccROJ project (version 0-49) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#ifndef _roj_filter_bank_
#define _roj_filter_bank_

/**
 * @type: class
 * @brief: Definition of roj_filter_bank class. Currently, filter bank works only with ODE filters.
 */

/* ************************************************************************************************************************* */
/* headers and declarations */

#include "roj-misc.hh"
#include "roj-external.hh"
#include "roj-real-array.hh"

#include "roj-complex-signal.hh"
class roj_complex_signal;

#include "roj-filter.hh"
class roj_filter;

#include "roj-filter-gener.hh"
class roj_filter_generator;

/* ************************************************************************************************************************* */
/* filter bank class definition */

class roj_filter_bank{
private:

  /* configuration */
  roj_filter_generator* m_filter_gen;
  roj_array_config m_config;

public:

  /* creation */
  roj_filter_bank(roj_array_config, roj_filter_generator*);
  ~roj_filter_bank();

  /* configuration */
  roj_array_config get_config();
  double get_frequency(int =0);
  
  /* processing */
  roj_complex_signal** filtering(roj_complex_signal*, int =1);
  void filtering(complex double);

  /* components */
  roj_filter* get_filter(int =0);
  roj_filter** m_filter_array;
};

#endif

