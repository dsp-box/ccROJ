/* *************************************************** *
 * This file is a part of ccROJ project (version 0-47) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#ifndef _roj_advance_
#define _roj_advance_

/**
* @type: module
* @brief: In this module, various independent (separated), advance, and useful functions dedicated to TF distribution analysis are introduced.
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

/* compacting */
roj_real_matrix* roj_calculate_time_compacting (roj_real_matrix*);
roj_real_matrix* roj_calculate_frequency_compacting (roj_real_matrix*);

/* multi-spectrogram */
int roj_minimize_matrix (roj_real_matrix*, roj_real_matrix*, roj_real_matrix*, roj_real_matrix*);
int roj_maximize_matrix (roj_real_matrix*, roj_real_matrix*, roj_real_matrix*, roj_real_matrix*);
int roj_absminimize_matrix (roj_real_matrix*, roj_real_matrix*, roj_real_matrix*, roj_real_matrix*);

/* save */
void roj_save (char *, roj_real_matrix*, roj_real_matrix*);

/* analysis */
roj_pair roj_calculate_interpolated_argmax (roj_real_matrix*);
roj_pair roj_linear_regression(roj_real_array* , roj_real_array*);
roj_pair roj_linear_regression(roj_real_array*);

#endif
