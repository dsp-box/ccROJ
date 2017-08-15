/* *************************************************** *
 * This file is a part of ccROJ project (version 0-45) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#ifndef _roj_process_
#define _roj_process_

/**
* @type: module
* @brief: In this module, various independent (separated) and useful functions dedicated to signal processing are introduced.
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

/* signal processing */
roj_complex_signal* roj_convolve_signals (roj_complex_signal*, roj_complex_signal*);
roj_complex_signal* roj_correlate_signals (roj_complex_signal*, roj_complex_signal*);

#endif
