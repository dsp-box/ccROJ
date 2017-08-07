/* *************************************************** *
 * This file is a part of ccROJ project (version 0-43) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                                Krzysztof Czarnecki  *
 *                       czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#ifndef _roj_reass_
#define _roj_reass_

/**
* @type: module
* @brief: In this module, various independent (separate) and useful functions dedicated to reassignment are introduced.
*/

/* ************************************************************************************************************************* */
/* headers and declarations */

#include "roj-external.hh"
#include "roj-misc.hh"

#include "roj-real-array.hh"
class roj_real_array;

#include "roj-real-matrix.hh"
class roj_real_matrix;

#include "roj-complex-signal.hh"
class roj_complex_signal;

/* ************************************************************************************************************************* */
/* function signatures */

/* TF reassignment */
roj_real_matrix* roj_time_frequency_reassign (roj_real_matrix*, roj_real_matrix*, roj_real_matrix*, roj_real_matrix* =NULL);
roj_real_matrix* roj_frequency_reassign (roj_real_matrix*, roj_real_matrix*, roj_real_matrix* =NULL);

/* profile */
roj_real_array* roj_calculate_profile (roj_real_matrix*, roj_real_matrix*, roj_array_config);
roj_real_array* roj_calculate_profile (roj_real_matrix*, roj_real_matrix*, roj_real_array*);
bool add_sample_to_profile (double, double, roj_real_array*);

/* profile 2D */
roj_real_matrix* roj_calculate_profile_over_time (roj_real_matrix*, roj_real_matrix*, roj_array_config);

/* histogram */
roj_real_array* roj_calculate_histogram (roj_real_matrix*, roj_array_config);
roj_real_array* roj_calculate_histogram (roj_real_matrix*, roj_real_array*);
bool add_sample_to_histogram(double, roj_real_array*);

#endif
