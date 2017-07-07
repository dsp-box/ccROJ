#ifndef _roj_real_matrix_
#define _roj_real_matrix_

/**
 * @type: class
 * @brief: Definition of roj_real_matrix class.
 * @herit: roj_real_matrix : roj_image
 */

/* ************************************************************************************************************************* */
/* headers */

#include "roj-misc.hh"
#include "roj-external.hh"
#include "roj-image.hh"

#include "roj-real-array.hh"
class roj_real_array;

/* ************************************************************************************************************************* */
/* time-frequency matrix class definition */

class roj_real_matrix
  : public roj_image{
  
public:
  roj_real_matrix( roj_image_config);
  roj_real_matrix(roj_real_matrix*);
  ~roj_real_matrix();
    
  /* simple manipulation */
  roj_real_matrix* cropping (roj_image_config);
  
  /**
   * @type: field
   * @brief: This public pointer gives access to time-frequency data.
   */
  double **m_data;

  /* save to text file */
  void save(char*);

  /* simple modification */
  void abs ();
  
  /* simple modification */
  void remove_nan ();

  /* clear data */
  void clear();

  /* analysis */
  double get_sum();
  double get_max();
  
  /* get marginal average distributions */ 
  roj_real_array* get_mean_over_time(roj_real_matrix* =NULL);
  roj_real_array* get_mean_over_frequency(roj_real_matrix* =NULL);

  /* get marginal maximum distributions */ 
  roj_real_array* get_max_over_time(roj_real_matrix*);
  roj_real_array* get_max_over_frequency(roj_real_matrix*);

  /* get marginal dominant distributions */ 
  roj_real_array* get_dominant_over_time(roj_real_matrix*, roj_array_config);
  roj_real_array* get_dominant_over_time(roj_real_matrix*, roj_real_matrix*, roj_array_config);

  /* operators */
  void operator /= (double);
  void operator *= (double);
  void operator += (double);
  void operator -= (double);
  void operator = (double);

  void operator *= (roj_real_matrix*);
  void operator += (roj_real_matrix*);
};

#endif

