/* *************************************************** *
 * This file is a part of ccROJ project (version 0-46) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#ifndef _roj_complex_signal_
#define _roj_complex_signal_

/**
 * @type: class
 * @brief: Definition of roj_complex_signal class.
 */

/* ************************************************************************************************************************* */
/* headers and declarations */

#include "roj-external.hh"
#include "roj-misc.hh"

#include "roj-fourier-spectr.hh"
class roj_fourier_spectrum;

#include "roj-hilbert-equiv.hh"
class roj_hilbert_equiv;

#include "roj-real-array.hh"
class roj_real_arrya;

/* ************************************************************************************************************************* */
/* complex signal class definition */

class roj_complex_signal{
private:

  /* internal configuration */
  roj_signal_config m_config;

  unsigned int append_head(double);
  unsigned int append_tail(double);

 public:

  /* construction */
  roj_complex_signal (roj_real_array*, roj_real_array* =NULL);
  roj_complex_signal (roj_complex_signal*);
  roj_complex_signal (roj_signal_config);
  roj_complex_signal (char*, int=0);
  ~roj_complex_signal ();

  /* config methods */
  bool compare_config (roj_signal_config);
  roj_signal_config get_config();
  double get_last_instant ();

  bool check_real();
  bool check_imag();
  
  roj_real_array* get_real ();
  roj_real_array* get_imag ();

  /* clear */
  void clear();
  double clear_imag();
  double clear_real();

  /* waveform manipulation */
  int copy(roj_complex_signal*, int =0);
  void insert (int, complex double =0);
  void modulate(double);
  void decimate(int);
  void conjugate();
  void reverse();

  /* waveform manipulation with change its length */
  unsigned int append(double, double);
  unsigned int cut(double, double);

  
  /* get signal transforms */
  roj_fourier_spectrum* get_spectrum();
  roj_complex_signal* get_instantaneous_complex_frequency ();
  roj_complex_signal* get_instantaneous_chirp_rate ();
  roj_complex_signal* get_instantaneous_frequency ();

  /* save to file */
  void save(char *);
  void save_wav(char *);

  /* other useful methods */
  complex double remove_const();
  double calc_energy();

  /**
   * @type: field
   * @brief: This public field gives access to signal samples.
   */
  complex double* m_waveform; 

  /* operators */
  void operator += (roj_complex_signal*);
  void operator -= (roj_complex_signal*);
  void operator *= (complex double);
  void operator /= (complex double);
};


#endif

