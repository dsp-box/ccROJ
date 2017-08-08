/* *************************************************** *
 * This file is a part of ccROJ project (version 0-43) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#ifndef _roj_pulse_gener_
#define _roj_pulse_gener_

/**
 * @type: class
 * @brief: Definition of roj_pulse_generator class.
 */

/* ************************************************************************************************************************* */
/* headers and declarations */

#include "roj-external.hh"
#include "roj-misc.hh"

#include "roj-complex-signal.hh"
class roj_complex_signal;


/* ************************************************************************************************************************* */
/**
 * @type: struct
 * @brief: This is a structure for roj_pulse_generator configuration.
 */
struct roj_pulse_config{

  double transition;
  double period;
  double width;

  double amplitude;
  double delay;
  double phase;

  double frequency;
  double bandwidth;  
};


/* ************************************************************************************************************************* */
/* pulse generator definition */

class roj_pulse_generator{
private:
  
  /* pulse methods */
  double get_tukey(double);
  bool check_pulse(double);
  
  /* calc simple sample */
  complex double get_harmonic(double);
  complex double get_lfm_chirp(double);
  complex double get_hfm_chirp(double);
  
public:

  /* construction */
  roj_pulse_generator();
  
  /* add in the whole time */
  double add_harmonic(roj_complex_signal*);
  double add_lfm_chirp(roj_complex_signal*);
  double add_hfm_chirp(roj_complex_signal*);
  double add_random_fsk(roj_complex_signal*);

  /* public pulse parameters */

  /**
   * @type: field
   * @brief: This field allows to set the transition of the Tukey window, which is an envelope of the pulse (0 <= transition <= 1).
   */
  double m_transition;

  /**
   * @type: field
   * @brief: Time in which one pulse occurs.
   */
  double m_period;

  /**
   * @type: field
   * @brief: The width of a single pulse.
   */
  double m_width;
  
  /**
   * @type: field
   * @brief: The amplitude of a single pulse.
   */
  double m_amplitude;

  /**
   * @type: field
   * @brief: A shift in time of pulse series.
   */
  double m_delay;
  
  /**
   * @type: field
   * @brief: An initial phase of a single pulse.
   */
  double m_phase;
  
  /**
   * @type: field
   * @brief: The currier (frequency).
   */
  double m_frequency;

  /**
   * @type: field
   * @brief: The bandwidth of a single pulse.
   */
  double m_bandwidth;  
};
  
#endif

