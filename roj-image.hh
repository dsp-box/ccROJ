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
  bool compare_time_config (roj_image_config);
  roj_image_config get_config ();
  bool check_config ();
  
  /* get time or frequency value */  
  double get_time_by_index(int);
  double get_frequency_by_index(int);

  /* get the nearest indexes */
  int get_index_by_time(double);
  int get_index_by_frequency(double);

  /* checking */
  bool check_in_time_index (int);
  bool check_in_frequency_index (int);
};

#endif

