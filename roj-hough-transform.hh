/* *************************************************** *
 * This file is a part of ccROJ project (version 0-46) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#ifndef _roj_hough_transform_
#define _roj_hough_transform_

/**
* @type: class
* @brief: Definition of roj_hough_transform class.
*/

/**
* @type: alert
* @brief: This module is not tested!
*/

/* ************************************************************************************************************************* */
/* headers and declarations */

#include "roj-external.hh"
#include "roj-misc.hh"

#include "roj-real-matrix.hh"
class roj_real_matrix;

/* ************************************************************************************************************************* */
/* Hough transtorm class definition */

class roj_hough_transform{
private:

  /* internal data and set */
  //  roj_real_matrix* m_energy;
  roj_array_config m_frequency_conf;
  roj_array_config m_chirprate_conf;
  double m_treshold;
  
  /* private methods */
  roj_real_matrix* reassign_energy(roj_real_matrix*);
  roj_real_matrix* reassign_energy(roj_real_matrix*, roj_real_matrix*, roj_real_matrix*);
  double calc_single_point(double, double, double =0.0);

public:
  
  /* construction */
  roj_hough_transform(roj_real_matrix*, roj_array_config, roj_array_config, double =0.0);
  roj_hough_transform(roj_real_matrix*, roj_real_matrix*, roj_real_matrix*, roj_array_config, roj_array_config, double =0.0);
  ~roj_hough_transform();
  
  /**
   * @type: field
   * @brief: This public field gives access to resultant transform.
   */
  roj_real_matrix* m_transform;
};

#endif

