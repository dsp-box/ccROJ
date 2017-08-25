/* *************************************************** *
 * This file is a part of ccROJ project (version 0-46) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#ifndef _roj_median_filter_
#define _roj_median_filter_

/**
* @type: class
* @brief: Definitions of roj_median_filter class.
*/

/* ************************************************************************************************************************* */
/* headers and declarations */

#include "roj-external.hh"
#include "roj-misc.hh"

#include "roj-real-array.hh"
class roj_real_array;

#include "roj-complex-signal.hh"
class roj_complex_signal;

#include "roj-real-matrix.hh"
class roj_real_matrix;

/* ************************************************************************************************************************* */
/* internal struct */
struct roj_median_segment{
  double val;
  bool flag;
  int x,y;
};
  
/* ************************************************************************************************************************* */
/* median filter class definition */

class roj_median_filter{
private:

  /* internal configuration */
  roj_median_segment** m_buffer;
  int m_counter;
  int m_length;
  int m_height;
  int m_width;

  /* internal methods */
  void add_segment(double, int, int);
  void remove_false_segments();
  void order_buffer_segments();
  double get_median_value();
  void clean_buffer_flags();
  void clean_buffer();

public:

  /* construction */
  roj_median_filter(int, int =1);
  ~roj_median_filter();
    
  /* filtering */
  roj_real_array* filtering(roj_real_array*, int =1);
  roj_real_array* smart_filtering(roj_real_array*, int =1);
  roj_real_matrix* smart_filtering(roj_real_matrix*, int =1, int =1);

  /* reset internal buffers */
  void reset ();
};

#endif

