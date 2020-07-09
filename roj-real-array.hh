/* *************************************************** *
 * This file is a part of ccROJ project (version 0-49) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#ifndef _roj_real_array_
#define _roj_real_array_

/**
* @type: class
* @brief: Definition of roj_real_array class.
*/

/* ************************************************************************************************************************* */
/* headers */

#include "roj-external.hh"
#include "roj-misc.hh"

/* ************************************************************************************************************************* */
/* class definition */

class roj_real_array{
private:

  /* configuration */
  roj_array_config m_config;
  int m_counter; 

public:

  /* construction */  
  roj_real_array (roj_array_config);
  roj_real_array (roj_real_array*); 
  roj_real_array (char*); 
  ~roj_real_array (); 

  /* config methods */
  bool compare_config (roj_array_config);
  roj_array_config get_config ();

  /* get selected parameters */
  double get_delta(); 
  double get_mean(); 
  double get_sum(); 
  double get_var(); 

  double get_arg_mean();
  double get_arg_var();
  
  /* clear data */
  void clear();
  roj_array_config cut_head(int);
  roj_array_config cut_tail(int);
 
  
  /* counter manipulation */  
  void increment_counter(); 
  int return_counter(); 

  /* analysis */
  double get_arg_by_index(int);
  int get_index_by_arg(double);
  bool check_in_index(int);
  int get_index_of_max(); 
  double get_max_value();

  /* save to file */
  void save(char*);

  /**
   * @type: field
   * @brief: This public pointer gives access to array elements.
   */
  double* m_data;

  /* operators */
  void operator /= (double);
  void operator *= (double);
  void operator -= (double);
  void operator += (double);
  void operator = (double);
};

#endif
