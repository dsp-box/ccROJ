/* *************************************************** *
 * This file is a part of ccROJ project (version 0-44) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#include "roj-pulse-gener.hh"

/* ************************************************************************************************************************* */
/**
* @type: constructor
* @brief: This is a constructor of roj_pulse_generator which produces series of pulses which are tapering by Tukey window.
*/
roj_pulse_generator :: roj_pulse_generator (){

  srand(time(NULL));

  m_transition = 0.5;
  m_frequency = 0.0;
  m_bandwidth = 0.0;  
  m_amplitude = 1.0;

  m_period = 1.0;
  m_width = 1.0;
  m_delay = 0.0;
  m_phase = 0.0;
}

/**
* @type: destructor
* @brief: The deconstructor is default.
*/

/* ************************************************************************************************************************* */
/**
* @type: private
* @brief: This routine roj_calculates Tukey window coefficent.
*
* @param [in] a_base_time: Time of generated value. 
*
* @return: Generated value.
*/
double roj_pulse_generator :: get_tukey (double a_base_time){
  
  if(m_transition<0 or m_transition>1)
    call_error("transition should be in <0, 1> range");

  if (m_transition==0)
    return 1.0;
  
  double time = a_base_time / m_width;
  if(time<-0.5) return 0.0;
  if(time>0.5) return 0.0;
  time += 0.5;
  
  if (time <= m_transition / 2)
    return 0.5 * (1 + cos(M_PI*(2.0 * time / m_transition -1)));
  
  if (time >= 1-m_transition / 2)
    return 0.5 * (1 + cos(M_PI*(2.0 * time / m_transition - 2.0 / m_transition +1)));
  
  return 1.0;  
}

/* ************************************************************************************************************************* */
/**
* @type: private
* @brief: This routine checks the pulse existence in a given instance.
*
* @param [in] a_time: Checked time. 
*
* @return: True if pulse exists, false otherwise.
*/
bool roj_pulse_generator :: check_pulse (double a_time){

  /* check pulse occurs in a given time */
  
  double time = a_time - m_delay;
  
  while(time<-m_period/2)
    time += m_period;
  
  while(time>m_period/2)
    time -= m_period;
  
  if(time<-m_width/2) return false;
  if(time>m_width/2) return false;
  
  return true;
}

/* ************************************************************************************************************************* */
/**
* @type: private
* @brief: This routine roj_calculates a sample of harmonic signal (without frequency modulation).
*
* @param [in] a_time: Time for wichich sample is generated. 
*
* @return: The sample.
*/
complex double roj_pulse_generator :: get_harmonic (double a_time){

  double time = a_time - m_delay;

  while(time<-m_period/2)
    time += m_period;

  while(time>m_period/2)
    time -= m_period;

  if(time<-m_width/2) return 0.0;
  if(time>m_width/2) return 0.0;

  double arg = TWO_PI * time * m_frequency + m_phase;  
  return get_tukey(time) * m_amplitude * cexp(I*arg);
}

/**
* @type: method
* @brief: This routine roj_calculates a signal (series of pulses) and adds it to a given argument.
*
* @param [in,out] a_sig: A signal for modification. 
*
* @return: The added energy.
*/
double roj_pulse_generator :: add_harmonic (roj_complex_signal* a_sig){

  if(a_sig==NULL)
    call_error("signal is null");
  
  roj_signal_config sig_conf = a_sig->get_config();

  /* fullfill waveform */
  double added_energy = 0.0;
  for (int n=0; n<sig_conf.length; n++){

    double time = sig_conf.start + (double)n/sig_conf.rate;
    complex double sample = get_harmonic(time);
    added_energy += pow(cabs(sample), 2.0);
    a_sig->m_waveform[n] += sample;
  }
  
  return added_energy;
}

/* ************************************************************************************************************************* */
/**
* @type: private
* @brief: This routine roj_calculates a sample of linear frequency modulated (LFM) signal.
*
* @param [in] a_time: Time for wichich sample is generated. 
*
* @return: The sample.
*/
complex double roj_pulse_generator :: get_lfm_chirp (double a_time){
  
  double time = a_time - m_delay;

  while(time<-m_period/2)
    time += m_period;

  while(time>m_period/2)
    time -= m_period;

  if(time<-m_width/2) return 0.0;
  if(time>m_width/2) return 0.0;

  double chirprate = m_bandwidth / m_width;
  double arg = TWO_PI * time * m_frequency + m_phase;  
  arg += M_PI * chirprate * pow(time, 2.0);
  
  return get_tukey(time) * m_amplitude * cexp(I*arg);
}

/**
* @type: method
* @brief: This routine roj_calculates a signal (series of LFM pulses) and adds it to a given argument.
*
* @param [in,out] a_sig: A signal for modification. 
*
* @return: The added energy.
*/
double roj_pulse_generator :: add_lfm_chirp (roj_complex_signal* a_sig){

  if(a_sig==NULL)
    call_error("signal is null");
  
  roj_signal_config sig_conf = a_sig->get_config();

  /* fullfill waveform */
  double added_energy = 0.0;
  for (int n=0; n<sig_conf.length; n++){

    double time = sig_conf.start + (double)n/sig_conf.rate;
    complex double sample = get_lfm_chirp(time);
    added_energy += pow(cabs(sample), 2.0);
    a_sig->m_waveform[n] += sample;
  }
  
  return added_energy;
}

/* ************************************************************************************************************************* */
/**
* @type: private
* @brief: This routine roj_calculates a sample of hyperbolic frequency modulated (HFM) signal.
*
* @param [in] a_time: Time for wichich sample is generated. 
*
* @return: The sample.
*/
complex double roj_pulse_generator :: get_hfm_chirp (double a_time){

  double time = a_time - m_delay;

  while(time<-m_period/2)
    time += m_period;

  while(time>m_period/2)
    time -= m_period;

  if(time<-m_width/2) return 0.0;
  if(time>m_width/2) return 0.0;

  /* f_stop should be grater than f_start - however it is not tested */
  /* f_start, f_stop should be grater than 0 - however it is not tested */  
  double f_start = m_frequency - m_bandwidth / 2;
  double f_stop = m_frequency + m_bandwidth / 2;
  double c_rate = m_bandwidth / m_width;
  
  double arg = m_phase;
  arg -= TWO_PI*f_start*f_stop * log(f_start+f_stop-2*time*c_rate) / c_rate;

  return get_tukey(time) * m_amplitude * cexp(I*arg);
}

/**
* @type: method
* @brief: This routine roj_calculates a signal (series of HFM pulses) and adds it to a given argument.
*
* @param [in,out] a_sig: a signal for modification. 
*
* @return: The added energy.
*/
double roj_pulse_generator :: add_hfm_chirp (roj_complex_signal* a_sig){

  if(a_sig==NULL)
    call_error("signal is null");
  
  roj_signal_config sig_conf = a_sig->get_config();

  /* fullfill waveform */
  double added_energy = 0.0;
  for (int n=0; n<sig_conf.length; n++){

    double time = sig_conf.start + (double)n/sig_conf.rate;
    complex double sample = get_hfm_chirp(time);
    added_energy += pow(cabs(sample), 2.0);
    a_sig->m_waveform[n] += sample;
  }
  
  return added_energy;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine roj_calculates a random FSK signal and adds it to a given argument.
*
* @param [in,out] a_sig: A signal for modification. 
*
* @return: The added energy.
*/
double roj_pulse_generator :: add_random_fsk (roj_complex_signal* a_sig){

  if(a_sig==NULL)
    call_error("signal is null");
  
  roj_signal_config sig_conf = a_sig->get_config();
  bool pulse_flag = false;

  /* fullfill waveform */
  double added_energy = 0.0;
  double conf_frequency = m_frequency;
  for (int n=0; n<sig_conf.length; n++){

    double time = sig_conf.start + (double)n/sig_conf.rate;

    /* change frequency for each new pulse */
    if (!pulse_flag){
      pulse_flag = check_pulse(time);
      if (pulse_flag){
	double low_frequency = m_bandwidth * ((double)rand() / RAND_MAX);
	m_frequency = low_frequency  + conf_frequency;
      }
    }
    else
      pulse_flag = check_pulse(time);

    complex double sample = get_harmonic(time);
    added_energy += pow(cabs(sample), 2.0);
    a_sig->m_waveform[n] += sample;
  }

  /* original frequency is restored */
  m_frequency = conf_frequency;

  return added_energy;
}
