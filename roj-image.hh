/* *************************************************** *
 * This file is a part of ccROJ project (version 0-44) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#ifndef _roj_image_
#define _roj_image_

/**
 * @type: class
 * @brief: Definition of abstract roj_image class.
 */

/* ************************************************************************************************************************* */
/* headers */

#include "roj-external.hh"
#include "roj-misc.hh"

/* ************************************************************************************************************************* */
/* time-frequency matrix class definition */

class roj_image{
protected:

  /* configuration */
  roj_image_config m_config;
  
public:
  
  /* time-frequency configuration */
  bool compare_config (roj_image_config);
  bool compare_x_config (roj_image_config);
  roj_image_config get_config ();
  bool check_config ();
  
  /* get time or frequency value */  
  double get_x_by_index(int);
  double get_y_by_index(int);

  /* get the nearest indexes */
  int get_index_by_x(double);
  int get_index_by_y(double);

  /* checking */
  bool check_in_x_index (int);
  bool check_in_y_index (int);
};

#endif

