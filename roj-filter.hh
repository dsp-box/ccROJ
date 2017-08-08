/* *************************************************** *
 * This file is a part of ccROJ project (version 0-43) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#ifndef _roj_filter_
#define _roj_filter_

/**
 * @type: class
 * @brief: Definition of roj_filter class. Filters which are used for bank construction have to be an instance of this class.
 */

/* ************************************************************************************************************************* */
/* headers and declarations */

#include "roj-external.hh"
#include "roj-misc.hh"

#include "roj-complex-signal.hh"
class roj_complex_signal;

#include "roj-filter-gener.hh"
class roj_filter_generator;

/* ************************************************************************************************************************* */
/* filter class definition */

class roj_filter{
  friend class roj_filter_generator;
  
private:

  /* configuration */
  unsigned int m_order;
  unsigned int m_index;
  unsigned int m_outlen;

  /* internal buffers */
  complex double *m_output;
  complex double *m_buffer;
  complex double *m_acoeff;
  complex double *m_bcoeff;

public:
  
  /* creation */
  roj_filter(unsigned int, unsigned int =1);
  ~roj_filter();

  /* processing */
  complex double process(complex double);
  complex double get_output(unsigned int =1);

  roj_complex_signal* process(roj_complex_signal*);
};

#endif

