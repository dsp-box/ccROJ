
#include "roj-noise-gener.hh"

/* ************************************************************************************************************************* */
/**
* @type: constructor
* @brief: This is a constructor of roj_noise_generator which is able to white and pink noise.
*/
roj_noise_generator :: roj_noise_generator (){

  srand(time(NULL));
  m_pink_key = 0;

  /* internal buffer */
  m_white_buffer = new unsigned int[PINK_ORDER];  
  for (int i=0; i<PINK_ORDER; i++)
    /* m_white_buffer[i] = (int)rand_gauss(1e6) % ((int)PINK_RANGE/(PINK_ORDER+1)); */
    m_white_buffer[i] = (int)rand() % ((int)PINK_RANGE/(PINK_ORDER+1));
}

/**
* @type: destructor
* @brief: This is a generator deconstructor.
*/
roj_noise_generator :: ~roj_noise_generator (){
  
  delete [] m_white_buffer;
}

/* ************************************************************************************************************************* */
/**
* @type: private
* @brief: This function returns a sample of pink noise according to http://www.firstpr.com.au/dsp/pink-noise .
*
* @return: A sample of pink noise.
*/
double roj_noise_generator :: rand_pink (){
  
  int last_key = m_pink_key;
  m_pink_key++;
  if (m_pink_key>PINK_MAXKEY)
    m_pink_key = 0;
  int diff = last_key ^ m_pink_key;

  /* unsigned int sum = (int)rand_gauss(1e6) % ((int)PINK_RANGE/(PINK_ORDER+1)); */
  unsigned int sum = (int)rand() % ((int)PINK_RANGE/(PINK_ORDER+1));
  for (int i=0; i<PINK_ORDER; i++){
    if (diff & (1<<i)){
   
      /* m_white_buffer[i] = (int)rand_gauss(1e6) % ((int)PINK_RANGE/(PINK_ORDER+1)); */   
      m_white_buffer[i] = (int)rand() % ((int)PINK_RANGE/(PINK_ORDER+1));    
    }
    sum += m_white_buffer[i];
  }

  sum <<= 1;
  /*  return (double)sum/(PINK_RANGE*1e6); */  
  return (double)sum/PINK_RANGE - 1.0;  
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This function adds white noise using standard deviation (sigma) to a given signal.
*
* @param [in,out] a_sig: A given signal.
* @param [in] a_sigma: standard deviation.
*
* @return: Energy of added noise.
*/
double roj_noise_generator :: add_awgn_using_sigma (roj_complex_signal* a_sig, double a_sigma){

  if(a_sig==NULL)
    call_error("signal is null");
  if(a_sigma<=0)
    call_error("sigma is not positive");
  
  roj_signal_config sig_conf = a_sig->get_config();
    
  double awgn_energy = 0.0;
  for(int n=0; n<sig_conf.length; n++){
    
    double re = rand_gauss(a_sigma);
    double im = rand_gauss(a_sigma);
    a_sig->m_waveform[n] += re + 1I*im;
    awgn_energy += pow(re, 2.0) + pow(im, 2.0);
  }

  return awgn_energy;
}

/**
* @type: method
* @brief: This function adds white noise using signal-to-noise ratio (SNR) to a given signal.
*
* @param [in,out] a_sig: A given signal.
* @param [in] a_snr: Signal-to-noise ratio.
*
* @return: Energy of added noise.
*/
double roj_noise_generator :: add_awgn_using_snr (roj_complex_signal* a_sig, double a_snr){

  if(a_sig==NULL)
    call_error("signal is null");
  
  roj_signal_config sig_conf = a_sig->get_config();

  double sig_energy = a_sig->calc_energy();
  double sig_power = sig_energy / (2 * sig_conf.length);
  double awgn_sigma = sqrt(sig_power * pow(10.0, -a_snr/10.0));
  
  if(sig_energy<=0)
    call_error("energy is zero");

  double awgn_energy = add_awgn_using_sigma(a_sig, awgn_sigma);
  
  /* true SNR can be roj_calculated as follows: */
  /* double true_rsnr = 10.0*log10(sig_energy/awgn_energy); */

  return awgn_energy;
}

/**
* @type: method
* @brief: This function adds white noise at exact signal-to-noise ratio (SNR) to a given signal.
*
* @param [in,out] a_sig: A given signal.
* @param [in] a_snr: Signal-to-noise ratio.
*
* @return: Energy of added noise.
*/
double roj_noise_generator :: add_awgn_at_exact_snr (roj_complex_signal* a_sig, double a_snr){

  /* generate noise and add it to a given argument */

  if(a_sig==NULL)
    call_error("signal is null");
  
  roj_signal_config sig_conf = a_sig->get_config();

  double sig_energy = a_sig->calc_energy();
  double sig_power = sig_energy / (2 * sig_conf.length);
  double awgn_sigma = sqrt(sig_power * pow(10.0, -a_snr/10.0));
  
  if(sig_energy<=0)
    call_error("energy is zero");
    
  roj_complex_signal* noise_only = new roj_complex_signal(sig_conf);
    
  double awgn_energy = 0.0;
  for(int n=0; n<sig_conf.length; n++){
    
    double re = rand_gauss(awgn_sigma);
    double im = rand_gauss(awgn_sigma);
    noise_only->m_waveform[n] = re + 1I*im;
  }

  /* SNR correction */
  awgn_energy = noise_only->calc_energy();
  double factor = sqrt(sig_energy/awgn_energy) * sqrt(1.0 * pow(10.0, -a_snr/10.0));
  for(int n=0; n<sig_conf.length; n++)
    noise_only->m_waveform[n] *= factor;

  awgn_energy = noise_only->calc_energy();
  *a_sig += noise_only;

  delete noise_only;
  return awgn_energy;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This function adds pink noise using a range to a given signal.
*
* @param [in,out] a_sig: A given signal.
* @param [in] a_range: standard deviation.
*
* @return: Energy of added noise.
*/
double roj_noise_generator :: add_pink_using_range (roj_complex_signal* a_sig, double a_range){

  if(a_sig==NULL)
    call_error("signal is null");
  if(a_range<=0)
    call_error("range is not positive");
  
  roj_signal_config sig_conf = a_sig->get_config();
    
  double pink_energy = 0.0;
  for(int n=0; n<sig_conf.length; n++){
    
    double re = a_range * rand_pink();
    a_sig->m_waveform[n] += re;
    pink_energy += pow(re, 2.0);
  }

  return pink_energy;
}

/**
* @type: method
* @brief: This function adds white noise at exact signal-to-noise ratio (SNR) to a given signal.
*
* @param [in,out] a_sig: A given signal.
* @param [in] a_snr: Signal-to-noise ratio.
*
* @return: Energy of added noise.
*/
double roj_noise_generator :: add_pink_at_exact_snr (roj_complex_signal* a_sig, double a_snr){

  if(a_sig==NULL)
    call_error("signal is null");
  
  roj_signal_config sig_conf = a_sig->get_config();

  double sig_energy = a_sig->calc_energy();
  if(sig_energy<=0)
    call_error("energy is zero");
    
  roj_complex_signal* noise_only = new roj_complex_signal(sig_conf);
    
  double pink_energy = 0.0;
  for(int n=0; n<sig_conf.length; n++){
    
    double re = rand_pink();
    noise_only->m_waveform[n] = re;
  }

  /* SNR correction */
  pink_energy = noise_only->calc_energy();
  double factor = sqrt(sig_energy/pink_energy) * sqrt(1.0 * pow(10.0, -a_snr/10.0));
  for(int n=0; n<sig_conf.length; n++)
    noise_only->m_waveform[n] *= factor;

  pink_energy = noise_only->calc_energy();
  *a_sig += noise_only;

  delete noise_only;
  return pink_energy;
}
