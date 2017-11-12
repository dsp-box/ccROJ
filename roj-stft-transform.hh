/* *************************************************** *
 * This file is a part of ccROJ project (version 0-47) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#ifndef _roj_stft_transform_
#define _roj_stft_transform_

/**
 * @type: class
 * @brief: Definition of roj_stft_transform class.
 * @herit: roj_stft_transform : roj_image
 */

/* ************************************************************************************************************************* */
/* headers and declarations */

#include "roj-external.hh"
#include "roj-misc.hh"

#include "roj-complex-signal.hh"
class roj_complex_signal;

#include "roj-fourier-spectr.hh"
class roj_fourier_spectrum;

#include "roj-real-matrix.hh"
class roj_real_matrix;

/* ************************************************************************************************************************* */
/* stft transform class definition */

class roj_stft_transform
  : public roj_image{
private:

  /* internal configuration */
  /* ******************************/
  roj_complex_signal *m_window;
    
public:

  /* construction */
  /* ******************************/
  roj_stft_transform(roj_image_config, roj_complex_signal *);
  ~roj_stft_transform();

  /**
   * @type: field
   * @brief: This field gives access to spectral lines.
   */
  complex double** m_spectrum; 

  /* public methods */
  /* ******************************/
  roj_complex_signal* get_signal ();
  roj_real_matrix* get_spectral_energy ();
};

#endif

