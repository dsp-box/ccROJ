/* *************************************************** *
 * This file is a part of ccROJ project (version 0-43) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                                Krzysztof Czarnecki  *
 *                       czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#include "roj-ode-analyzer.hh"

/* ************************************************************************************************************************* */
/**
 * @type: constructor
 * @brief: This is a constructor of roj_ode_analyzer.
 *
 * @param [in] a_bank_conf: A bank configuration (frequency axis).
 * @param [in] a_filter_gen: A pointer to a filter generator.
 */
roj_ode_analyzer :: roj_ode_analyzer(roj_array_config a_bank_conf, roj_filter_generator* a_filter_gen){

  /* check args */
  if(a_filter_gen==NULL)
    call_error("arg is null");

  /* check and set frequencies */
  if(a_bank_conf.min>=a_bank_conf.max and a_bank_conf.length>1)
    call_error("init frequency is grater than final");
  
  /* parse other parameters */
  if(a_bank_conf.length<1)
    call_error("filter number is negative or zero");

  /* copy config data */
  m_bank_config = a_bank_conf;

  /* filter generator */
  m_filter_gen = new roj_filter_generator(*a_filter_gen);
  m_filter_gen->set_type(ROJ_ODE_FILTER);
    
  /* internal filter banks allocate */
  m_bank_array = new roj_filter_bank*[5];
  for(int n=0; n<5; n++){

    m_filter_gen->set_order(a_filter_gen->get_order() - 2 + n);
    m_bank_array[n] = new roj_filter_bank(a_bank_conf, m_filter_gen);
  }

  m_filter_gen->set_order(a_filter_gen->get_order());
  m_input_signal = NULL;

  /* allocate slots */
  m_filtered_signals = new roj_complex_signal**[5];
  for(int n=0; n<5; n++)
    m_filtered_signals[n] = NULL;
}

/**
 * @type: destructor
 * @brief: This is a bank deconstructor. This function release also memory.
 */
roj_ode_analyzer :: ~roj_ode_analyzer(){
  
  for(int n=0; n<5; n++){  
    if (m_filtered_signals[n]!=NULL){

      for(int k=0; k<m_bank_config.length; k++)
	delete m_filtered_signals[n][k];      
      delete [] m_filtered_signals[n];
    }
  }
  delete [] m_filtered_signals;
  
  for(int n=0; n<5; n++)
    delete m_bank_array[n];
  delete [] m_bank_array;

  delete m_filter_gen;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This is functiona allows to set a signal for analysis.
 *
 * @param [in] a_sample: An signal for filtering.
 * @param [in] a_hop: A hopsize.
 */
void roj_ode_analyzer :: set_signal(roj_complex_signal* a_sig, int a_hop){
  
  roj_analyzer :: set_signal(a_sig, a_hop);
  roj_signal_config conf = m_input_signal->get_config();

  if(conf.rate!=m_filter_gen->get_rate())
    call_error("signal and filter sampling rates are different");
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This function returns frequency of a filter of a given index.
 *
 * @param [in] a_index: A given index (default 0).
 *
 * @return: a currier frequency.
 */
double roj_ode_analyzer :: get_frequency(unsigned int a_index){
  
  if(a_index >= m_bank_config.length )
    call_error("a given index is too large");

  return  m_bank_array[0]->get_frequency(a_index);
}

/* ************************************************************************************************************************* */
/**
 * @type: private
 * @brief: This function returns spectrogram width (in points).
 *
 * @return: Spectrogram width.
 */
unsigned int roj_ode_analyzer :: get_width(){
  
  if(m_input_signal==NULL)
    call_error("signal is not loaded!");

  /* filtering */
  if(m_filtered_signals[2] == NULL)
    m_filtered_signals[2] = m_bank_array[2]->filtering(m_input_signal, m_hop);  

  /* get length from first signal */  
  roj_signal_config conf = m_filtered_signals[2][0]->get_config();  
  return conf.length;
}

/**
 * @type: private
 * @brief: This function returns spectrogram height (in points / channels).
 *
 * @return: Spectrogram width.
 */
unsigned int roj_ode_analyzer :: get_height(){
  
  return m_bank_config.length;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This function returns image TF configuration (for STFT and generated distributions).
*
* @return: A structure with TF configuration.
*/
roj_image_config roj_ode_analyzer :: get_image_config (){

  /* if signal is not loaded, 
     time range is unknown */
  if(m_input_signal==NULL)
    call_error("signal is not loaded!");
  
  /* filtering  (2 bank) */
  if(m_filtered_signals[2] == NULL)
    m_filtered_signals[2] = m_bank_array[2]->filtering(m_input_signal, m_hop);  

  roj_signal_config sig_conf = m_filtered_signals[2][0]->get_config();

  roj_image_config img_conf;
  img_conf.y.length = get_height();
  img_conf.x.length = get_width();

  img_conf.x.min = sig_conf.start;  
  img_conf.x.max = sig_conf.start + (double)(sig_conf.length-1) / sig_conf.rate;  // !!!!!!!!!!
  img_conf.y.min = m_bank_config.min;
  img_conf.y.max = m_bank_config.max;

  return img_conf;
}

/**
 * @type: method
 * @brief: This function returns empty, however, configured spectrogram.
 *
 * @return: A pointer to empty distribution.
 */
roj_real_matrix* roj_ode_analyzer :: create_empty_image(){

  /* if signal is not loaded, 
     we do not know time range */
  if(m_input_signal==NULL)
    call_error("signal is not loaded!");

  roj_image_config img_conf = get_image_config ();  
  roj_real_matrix* output = new roj_real_matrix(img_conf);
  
  return output;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This function returns filtered signals as a recursive STFT.
*
* @return: A pointer to STFT transform.
*/
roj_stft_transform* roj_ode_analyzer :: get_transform_as_stft (){

  /* check input signal */
  if(m_input_signal==NULL)
    call_error("signal is not loaded!");

  /* filtering  (2 bank) */
  if(m_filtered_signals[2] == NULL)
    m_filtered_signals[2] = m_bank_array[2]->filtering(m_input_signal, m_hop);  

  roj_image_config img_conf = get_image_config ();  
  roj_stft_transform* transform = new roj_stft_transform(img_conf, NULL);

  /* calc stft */
  for(int n=0; n<get_width(); n++){
    for(int k=0; k<get_height(); k++)
      transform->m_spectrum[n][k] =  m_filtered_signals[2][k]->m_waveform[n];

    print_progress(n+1, get_width(), "stft");
  }
  print_progress(0, 0, "stft");

  return transform;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This is an estimator of spectral energy.
 *
 * @return: A pointer to obtained distribution.
 */
roj_real_matrix* roj_ode_analyzer :: get_spectral_energy(){

  /* check input signal */
  if(m_input_signal==NULL)
    call_error("signal is not loaded!");
  
  /* filtering  (2 bank) */
  if(m_filtered_signals[2] == NULL)
    m_filtered_signals[2] = m_bank_array[2]->filtering(m_input_signal, m_hop);  
  
  /* make empty output object */
  roj_real_matrix* output = create_empty_image();

  /* calc energy estimate */
  for(int n=0; n<get_width(); n++){
    for(int k=0; k<get_height(); k++)
      output->m_data[n][k] = pow(cabs(m_filtered_signals[2][k]->m_waveform[n]), 2.0);

    print_progress(n+1, get_width(), "energy");
  }

  print_progress(0, 0, "energy");
  return output;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This is an estimator of instantaneous frequency.
 *
 * @return: A pointer to obtained distribution.
 */
roj_real_matrix* roj_ode_analyzer :: get_instantaneous_frequency_by_1_estimator(){

  /* check input signal */
  if(m_input_signal==NULL)
    call_error("signal is not loaded!");

  /* filtering  (1 and 2 banks) */
  for(int n=1; n<3; n++)
    if(m_filtered_signals[n] == NULL)
      m_filtered_signals[n] = m_bank_array[n]->filtering(m_input_signal, m_hop);  
  
  /* make empty output object */
  roj_real_matrix* output = create_empty_image();

  /* calc instantaneous frequency estimate */
  for(int k=0; k<get_height(); k++){

    /* TODO: include to filter bank */
    double frequency = m_bank_array[2]->get_frequency(k);
    complex double pole = I*TWO_PI * frequency -1.0/m_filter_gen->get_spread();

    for(int n=0; n<get_width(); n++){

      /* TODO: include to filter bank */
      complex double y = m_filtered_signals[2][k]->m_waveform[n]; 
      complex double yD = m_filtered_signals[1][k]->m_waveform[n] / m_filter_gen->get_spread();
      yD += m_filtered_signals[2][k]->m_waveform[n] * pole;

      if(cabs(y)==0)
	output->m_data[n][k] = 1E300;
      else
	output->m_data[n][k] = cimag(yD/y) / TWO_PI;
    }    

    print_progress(k+1, m_bank_config.length, "i-freq");
  }

  print_progress(0, 0, "i-freq");    
  return output;
}

/**
 * @type: method
 * @brief: This is an estimator of instantaneous frequency (unbiased). 
 *
 * @return: A pointer to obtained distribution.
 */
roj_real_matrix* roj_ode_analyzer :: get_instantaneous_frequency_by_2_estimator(){

  /* check input signal */
  if(m_input_signal==NULL)
    call_error("signal is not loaded!");

  /* filtering  (1, 2, and 3 banks) */
  for(int n=1; n<4; n++)
    if(m_filtered_signals[n] == NULL)
      m_filtered_signals[n] = m_bank_array[n]->filtering(m_input_signal, m_hop);  
  
  /* make empty output object */
  roj_real_matrix* output = create_empty_image();

  /* calc instantaneous frequency estimate */
  for(int k=0; k<get_height(); k++){

    /* TODO: include to filter bank */
    double frequency = m_bank_array[2]->get_frequency(k);
    complex double pole = I*TWO_PI * frequency -1.0/m_filter_gen->get_spread();

    for(int n=0; n<get_width(); n++){

      /* TODO: include to filter bank */
      complex double y = m_filtered_signals[2][k]->m_waveform[n]; 
      complex double yD = m_filtered_signals[1][k]->m_waveform[n] / m_filter_gen->get_spread();
      yD += m_filtered_signals[2][k]->m_waveform[n] * pole;

      complex double yT = m_filtered_signals[3][k]->m_waveform[n] * m_filter_gen->get_spread() * m_filter_gen->get_order();

      if(cabs(y)==0)
	output->m_data[n][k] = 1E300;
      else
	output->m_data[n][k] = -(creal(yD/yT)/cimag(y/yT)) / TWO_PI;
    }    

    print_progress(k+1, m_bank_config.length, "i-freq");
  }

  print_progress(0, 0, "i-freq");    
  return output;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This is an estimator of spectral (group) delay.
 *
 * @return: A pointer to obtained distribution.
 */
roj_real_matrix* roj_ode_analyzer :: get_spectral_delay(){

  /* check input signal */
  if(m_input_signal==NULL)
    call_error("signal is not loaded!");

  /* filtering  (2 and 3 banks) */
  for(int n=2; n<4; n++)
    if(m_filtered_signals[n] == NULL)
      m_filtered_signals[n] = m_bank_array[n]->filtering(m_input_signal, m_hop);  

  /* make empty output object */
  roj_real_matrix* output = create_empty_image();

  /* calc spectral delay estimate */
  for(int k=0; k<get_height(); k++){
    for(int n=0; n<get_width(); n++){
      
      /* TODO: include to filter bank */
      complex double y = m_filtered_signals[2][k]->m_waveform[n]; 
      complex double yT = m_filtered_signals[3][k]->m_waveform[n] * m_filter_gen->get_spread() * m_filter_gen->get_order();
      
      output->m_data[n][k] = -creal(yT/y); 
    }

    print_progress(k+1, m_bank_config.length, "s-delay");
  }

  print_progress(0, 0, "s-delay");
  return output;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This is an estimator of chirp-rate.
 *
 * @return: A pointer to obtained distribution.
 */
roj_real_matrix* roj_ode_analyzer :: get_chirp_rate_by_k_estimator(){

  /* check input signal */
  if(m_input_signal==NULL)
    call_error("signal is not loaded!");

  /* filtering  (1 and 3 banks) */
  for(int n=1; n<4; n++)
    if(m_filtered_signals[n] == NULL)
      m_filtered_signals[n] = m_bank_array[n]->filtering(m_input_signal, m_hop);  
  
  /* make empty output object */
  roj_real_matrix* output = create_empty_image();

  /* calc chirp rate estimate */
  for(int k=0; k<get_height(); k++){

    /* TODO: include to filter bank */
    double frequency = m_bank_array[2]->get_frequency(k);
    complex double pole = I*TWO_PI * frequency -1.0/m_filter_gen->get_spread();

    for(int n=0; n<get_width(); n++){
      
      /* TODO: include to filter bank */
      complex double y = m_filtered_signals[2][k]->m_waveform[n]; 
      complex double yT = m_filtered_signals[3][k]->m_waveform[n] * m_filter_gen->get_spread() * m_filter_gen->get_order();      
      complex double yD = m_filtered_signals[1][k]->m_waveform[n] / m_filter_gen->get_spread();
      yD += m_filtered_signals[2][k]->m_waveform[n] * pole;

      double nominative = creal(yD/y) / TWO_PI;
      double denominative = cimag(yT/y); 

      if(cabs(y)==0)
	output->m_data[n][k] = 1E300;
      else
	output->m_data[n][k] = nominative / denominative;
    }

    print_progress(k+1, m_bank_config.length, "c-rate");
  }

  print_progress(0, 0, "c-rate");
  return output;
}

/**
 * @type: method
 * @brief: This is an estimator of chirp-rate.
 *
 * @return: A pointer to obtained distribution.
 */
roj_real_matrix* roj_ode_analyzer :: get_chirp_rate_by_d_estimator(){

  /* check input signal */
  if(m_input_signal==NULL)
    call_error("signal is not loaded!");

  /* filtering  (0 and 3 banks) */
  for(int n=0; n<4; n++)
    if(m_filtered_signals[n] == NULL)
      m_filtered_signals[n] = m_bank_array[n]->filtering(m_input_signal, m_hop);  

  /* make empty output object */
  roj_real_matrix* output = create_empty_image();

  /* calc chirp rate estimate */
  for(int k=0; k<get_height(); k++){
    
    /* TODO: include to filter bank */
    double frequency = m_bank_array[2]->get_frequency(k);
    complex double pole = I*TWO_PI * frequency -1.0/m_filter_gen->get_spread();

    for(int n=0; n<get_width(); n++){
      
      /* TODO: include to filter bank */
      complex double y = m_filtered_signals[2][k]->m_waveform[n]; 
      complex double y2 = cpow(m_filtered_signals[2][k]->m_waveform[n], 2.0); 
      complex double yT = m_filtered_signals[3][k]->m_waveform[n] * m_filter_gen->get_spread() * m_filter_gen->get_order();      
      complex double yD = m_filtered_signals[1][k]->m_waveform[n] / m_filter_gen->get_spread();
      yD += m_filtered_signals[2][k]->m_waveform[n] * pole;
      
      complex double yDT = m_filtered_signals[2][k]->m_waveform[n] * m_filter_gen->get_order();
      yDT += m_filtered_signals[3][k]->m_waveform[n] * m_filter_gen->get_order() * m_filter_gen->get_spread() * pole;
      
      complex double yD2 = m_filtered_signals[0][k]->m_waveform[n] / pow(m_filter_gen->get_spread(), 2.0);
      yD2 += 2.0 * pole * m_filtered_signals[1][k]->m_waveform[n] / m_filter_gen->get_spread();
      yD2 += cpow(pole, 2.0) * m_filtered_signals[2][k]->m_waveform[n];
      
      double nominative = creal (yD2/y - yD*yD/y2) / TWO_PI;
      double denominative = cimag (yDT/y - yD*yT/y2);

      if(cabs(y)==0)
	output->m_data[n][k] = 1E300;
      else
	output->m_data[n][k] = nominative / denominative;
    }
    
    print_progress(k+1, m_bank_config.length, "c-rate");
  }
  
  print_progress(0, 0, "c-rate");
  return output;
}

/**
 * @type: method
 * @brief: This is an estimator of chirp-rate.
 *
 * @return: A pointer to obtained distribution.
 */
roj_real_matrix* roj_ode_analyzer :: get_chirp_rate_by_f_estimator(){

  /* roj_calculate cr estimate */
  /* f is from Francois */

  /* check input signal */
  if(m_input_signal==NULL)
    call_error("signal is not loaded!");

  /* filtering  (0 and 3 banks) */
  for(int n=1; n<5; n++)
    if(m_filtered_signals[n] == NULL)
      m_filtered_signals[n] = m_bank_array[n]->filtering(m_input_signal, m_hop);  

  /* make empty output object */
  roj_real_matrix* output = create_empty_image();

  /* calc chirp rate estimate */
  for(int k=0; k<get_height(); k++){
    
    /* TODO: include to filter bank */
    double frequency = m_bank_array[2]->get_frequency(k);
    complex double pole = I*TWO_PI * frequency -1.0/m_filter_gen->get_spread();

    for(int n=0; n<get_width(); n++){
 
    /* TODO: include to filter bank */
      complex double y = m_filtered_signals[2][k]->m_waveform[n]; 
      complex double y2 = cpow(m_filtered_signals[2][k]->m_waveform[n], 2.0); 
      complex double yT = m_filtered_signals[3][k]->m_waveform[n] * m_filter_gen->get_spread() * m_filter_gen->get_order();      
      complex double yD = m_filtered_signals[1][k]->m_waveform[n] / m_filter_gen->get_spread();
      yD += m_filtered_signals[2][k]->m_waveform[n] * pole;
      
      complex double yDT = m_filtered_signals[2][k]->m_waveform[n] * m_filter_gen->get_order();
      yDT += m_filtered_signals[3][k]->m_waveform[n] * m_filter_gen->get_order() * m_filter_gen->get_spread() * pole;
      
      complex double yT2 = m_filter_gen->get_order() * (m_filter_gen->get_order() + 1) * m_filtered_signals[4][k]->m_waveform[n];
      yT2 *= pow(m_filter_gen->get_spread(), 2.0);
      
      double nominative = cimag (yDT/y - yD*yT/y2) / TWO_PI;
      double denominative = creal (yT2/y - yT*yT/y2);

      if(cabs(y)==0)
	output->m_data[n][k] = 1E300;
      else
	output->m_data[n][k] = -nominative / denominative;
    }

    print_progress(k+1, m_bank_config.length, "c-rate");
  }
  
  print_progress(0, 0, "c-rate");
  return output;
}
