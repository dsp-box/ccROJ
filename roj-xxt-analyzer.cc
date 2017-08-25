/* *************************************************** *
 * This file is a part of ccROJ project (version 0-46) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#include "roj-xxt-analyzer.hh"

/* ************************************************************************************************************************* */
/**
* @type: constructor
* @brief: This is a constructor of roj_xxt_analyzer.
*
* @param [in] a_bank_conf: A bank configuration (frequency axis).
* @param [in] a_window_conf: A pointer to a window generator.
*/
roj_xxt_analyzer :: roj_xxt_analyzer (roj_array_config a_bank_conf, roj_window_generator* a_window_gen){

  if(a_window_gen==NULL){
    call_warning("in roj_xxt_analyzer :: roj_xxt_analyzer");
    call_error("arg is null");
  }

  if(a_bank_conf.min==0 and a_bank_conf.max==0){

    a_bank_conf.max=a_window_gen->get_rate()/2;
    a_bank_conf.min=-a_window_gen->get_rate()/2;
    call_warning("min and max are set automatically");
  }

  if(a_bank_conf.min>=a_bank_conf.max){
    call_warning("in roj_xxt_analyzer :: roj_xxt_analyzer");
    call_error("min >= max");
  }

  if(a_bank_conf.max > a_window_gen->get_rate()/2){
    call_warning("in roj_xxt_analyzer :: roj_xxt_analyzer");
    call_error("max > rate/2");
  }
  if(a_bank_conf.min < -a_window_gen->get_rate()/2){
    call_warning("in roj_xxt_analyzer :: roj_xxt_analyzer");
    call_error("min < -rate/2");
  }

  
  if(a_bank_conf.length<1){
    call_warning("in roj_xxt_analyzer :: roj_xxt_analyzer");
    call_error("length < 1");
  }

  if(a_window_gen->get_length()>a_bank_conf.length){
    call_warning("in roj_xxt_analyzer :: roj_xxt_analyzer");
    call_error("win length > fft length");
  }

  /* main bank config */
  m_bank_config = a_bank_conf;

  /* copy window generator */
  m_window_gen = new roj_window_generator(*a_window_gen);

  /* init pointer to analyzed signal */
  m_input_signal = NULL;

  /* update frequency range */
  double delta = m_window_gen->get_rate() / m_bank_config.length;
  int finish = (m_bank_config.max + m_window_gen->get_rate()/2) / delta;
  int initial = get_initial();
  
  double new_f_min = initial * delta - a_window_gen->get_rate()/2;
  double new_f_max = finish * delta - a_window_gen->get_rate()/2; 

  if(new_f_min!=m_bank_config.min or new_f_max!=m_bank_config.max){ 
    call_warning("frequency is fixed:");
    call_warning("see configuration of returned distributions");
  }
  
  m_bank_config.min = new_f_min;
  m_bank_config.max = new_f_max;
  
  /* allocate slots */
  m_fourier_spectra = new complex double**[6];
  for(int s=0; s<6; s++)
    m_fourier_spectra[s] = NULL;
}

/**
* @type: destructor
* @brief: This is an anayzer deconstructor. This function release also internal buffers.
*/
roj_xxt_analyzer :: ~roj_xxt_analyzer(){

  for(int s=0; s<6; s++){    
    if (m_fourier_spectra[s]!=NULL){

      for(int n=0;n<get_width();n++)
	delete [] m_fourier_spectra[s][n];    
      delete [] m_fourier_spectra[s];      
    }  
  }  
  delete [] m_fourier_spectra;  
  delete m_window_gen;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This function allocates memory for STFT.
*
* @return: A pointer to allocated buffer.
*/
complex double ** roj_xxt_analyzer :: allocate_stft (){

  complex double** stft = new complex double*[get_width()];
  int byte_size = get_height() * sizeof(complex double);

  for(int n=0;n<get_width();n++){
    stft[n] = new complex double[get_height()];
    memset(stft[n], 0x0, byte_size);
  }

  return stft;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This is functiona allows to set a signal for analysis.
*
* @param [in] a_sample: An signal for filtering.
* @param [in] a_hop (default 1): A hopsize.
*/
void roj_xxt_analyzer :: set_signal (roj_complex_signal* a_sig, int a_hop){
  
  roj_analyzer :: set_signal(a_sig, a_hop);
  roj_signal_config conf = m_input_signal->get_config();

  m_width = (conf.length - m_window_gen->get_length() + 1) / m_hop;

  double delta = conf.rate / m_bank_config.length;
  m_height = (m_bank_config.max - m_bank_config.min) / delta;
  
  if(conf.rate!=m_window_gen->get_rate()){
    call_warning("in roj_xxt_analyzer :: set_signal");
    call_error("signal and window sampling rates are different");
  }
  
  if((conf.length-m_hop)<m_window_gen->get_length()){
    call_warning("in roj_xxt_analyzer :: set_signal");
    call_error("signal is too short or window is too long");
  }
}

/* ************************************************************************************************************************* */
/**
* @type: private
* @brief: This function returns spectrogram width (in points).
*
* @return: Spectrogram width.
*/
unsigned int roj_xxt_analyzer :: get_width (){
  
  if(m_input_signal==NULL){
    call_warning("in roj_xxt_analyzer :: get_width");
    call_error("signal is not loaded!");
  }
  
  return m_width;
}

/**
* @type: private
* @brief: This function returns spectrogram height (in points / channels).
*
* @return: Spectrogram width.
*/
unsigned int roj_xxt_analyzer :: get_height (){
  
  if(m_input_signal==NULL){
    call_warning("in roj_xxt_analyzer :: get_height");
    call_error("signal is not loaded!");
  }
  
  return m_height;
}

/**
* @type: private
* @brief: this method returns index of an initial line.
*
* @return: index of an initial line.
*/
unsigned int roj_xxt_analyzer :: get_initial(){

  double delta = m_window_gen->get_rate() / m_bank_config.length;
  unsigned int initial = (m_bank_config.min + m_window_gen->get_rate()/2) / delta;
  return initial;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This function returns empty, however, configured spectrogram.
*
* @return: A pointer to empty distribution.
*/
roj_real_matrix* roj_xxt_analyzer :: create_empty_image (){
  
  roj_image_config img_conf = get_image_config ();
  roj_real_matrix* output = new roj_real_matrix(img_conf);
  return output;
}

/**
* @type: method
* @brief: This function returns image TF configuration (for STFT and generated distributions).
*
* @return: A structure with TF configuration.
*/
roj_image_config roj_xxt_analyzer :: get_image_config (){

  /* if signal is not loaded, 
     time range is unknown */
  if(m_input_signal==NULL){
    call_warning("in roj_xxt_analyzer :: get_image_config");
    call_error("signal is not loaded!");
  }
  
  roj_signal_config sig_conf = m_input_signal->get_config();
  int win_width = m_window_gen->get_length();
  
  roj_image_config img_conf;
  img_conf.y.length = get_height();
  img_conf.x.length = get_width();
  
  img_conf.x.min = sig_conf.start + (double)win_width * 0.5 / sig_conf.rate;  
  img_conf.x.max = img_conf.x.min + (double)((get_width()-1)*m_hop) / sig_conf.rate; 
  img_conf.y.min = m_bank_config.min;
  img_conf.y.max = m_bank_config.max;
  
  return img_conf;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This function returns STFT.
*
* @return: A pointer to STFT transform.
*/
roj_stft_transform* roj_xxt_analyzer :: get_stft_transform (){

  /* check input signal */
  if(m_input_signal==NULL){
    call_warning("in roj_xxt_analyzer :: get_stft_transform");
    call_error("signal is not loaded!");
  }

  roj_image_config config = get_image_config ();
  roj_complex_signal* window = m_window_gen->get_window();
  roj_stft_transform* transform = new roj_stft_transform(config, window);

  /* filtering  (2nd slot) */
  if(m_fourier_spectra[CODE_WIN_ZERO] == NULL)
    m_fourier_spectra[CODE_WIN_ZERO] = transforming(0, 0);

#ifdef ROJ_DEBUG_ON
  double window_gain = m_window_gen->calc_gain();
  call_info("window gain: ", window_gain);
#endif


  /* calc stft */
  int sign = -2 * (get_initial()%2) +1;
  for(int n=0; n<get_width(); n++){
    for(int k=0; k<get_height(); k++)
      transform->m_spectrum[n][k] =  sign * m_fourier_spectra[CODE_WIN_ZERO][n][k] / m_bank_config.length;

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
roj_real_matrix* roj_xxt_analyzer :: get_spectral_energy (){

  roj_stft_transform* c_stft = get_stft_transform ();  
  roj_real_matrix*output = c_stft->get_spectral_energy ();
  
  delete c_stft;
  return output;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This is an estimator of instantaneous frequency.
*
* @return: A pointer to obtained distribution.
*/
roj_real_matrix* roj_xxt_analyzer :: get_instantaneous_frequency_by_1_estimator (){

  /* check input signal */
  if(m_input_signal==NULL){
    call_warning("in roj_xxt_analyzer :: get_instantaneous_frequency_by_1_estimator");
    call_error("signal is not loaded!");
  }
  
  /* transforming (1 and 2 slots) */
  if(m_fourier_spectra[CODE_WIN_D] == NULL)
    m_fourier_spectra[CODE_WIN_D] = transforming(1, 0);
  if(m_fourier_spectra[CODE_WIN_ZERO] == NULL)
    m_fourier_spectra[CODE_WIN_ZERO] = transforming(0, 0);
  
  /* make empty output object */
  roj_real_matrix* output = create_empty_image();

  /* calc instantaneous frequency estimate */
  for(int k=0; k<get_height(); k++){
    double freq = m_bank_config.min + m_window_gen->get_rate() * (double)k/m_bank_config.length;
    
    for(int n=0; n<get_width(); n++){
      
      complex double y = m_fourier_spectra[CODE_WIN_ZERO][n][k]; 
      complex double yD = m_fourier_spectra[CODE_WIN_D][n][k]; 

      if(cabs(y)==0)
	output->m_data[n][k] = 1E300;
      else
	output->m_data[n][k] = freq-cimag(yD/y) / TWO_PI;
    }    

    print_progress(k+1, get_height(), "i-freq");
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
roj_real_matrix* roj_xxt_analyzer :: get_instantaneous_frequency_by_2_estimator (){

  /* roj_calculate if estimate */

  /* check input signal */
  if(m_input_signal==NULL){
    call_warning("in roj_xxt_analyzer :: get_instantaneous_frequency_by_2_estimator");
    call_error("signal is not loaded!");
  }
  
  /* transforming (3 slots) */
  if(m_fourier_spectra[CODE_WIN_T] == NULL)
    m_fourier_spectra[CODE_WIN_T] = transforming(0, 1);  
  if(m_fourier_spectra[CODE_WIN_D] == NULL)
    m_fourier_spectra[CODE_WIN_D] = transforming(1, 0);
  if(m_fourier_spectra[CODE_WIN_ZERO] == NULL)
    m_fourier_spectra[CODE_WIN_ZERO] = transforming(0, 0);


  /* make empty output object */
  roj_real_matrix* output = create_empty_image();

  /* calc instantaneous frequency estimate */
  for(int k=0; k<get_height(); k++){
    double freq = m_bank_config.min + m_window_gen->get_rate() * (double)k/m_bank_config.length;
    
    for(int n=0; n<get_width(); n++){
      
      complex double y = m_fourier_spectra[CODE_WIN_ZERO][n][k]; 
      complex double yD = m_fourier_spectra[CODE_WIN_D][n][k];
      complex double yT = m_fourier_spectra[CODE_WIN_T][n][k]; 

      if(cabs(y)==0)
	output->m_data[n][k] = 1E300;
      else
	output->m_data[n][k] = freq+(creal(yD/yT)/cimag(y/yT)) / TWO_PI;
    }    

    print_progress(k+1, get_height(), "i-freq");
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
roj_real_matrix* roj_xxt_analyzer :: get_spectral_delay (){

  /* check input signal */
  if(m_input_signal==NULL){
    call_warning("in roj_xxt_analyzer :: get_spectral_delay");
    call_error("signal is not loaded!");
  }

  /* transforming  (2 and 3 slots) */
  if(m_fourier_spectra[CODE_WIN_T] == NULL)
    m_fourier_spectra[CODE_WIN_T] = transforming(0, 1);
  if(m_fourier_spectra[CODE_WIN_ZERO] == NULL)
    m_fourier_spectra[CODE_WIN_ZERO] = transforming(0, 0);

  /* make empty output object */
  roj_real_matrix* output = create_empty_image();
  
  /* calc spectral delay estimate */
  for(int k=0; k<get_height(); k++){
    for(int n=0; n<get_width(); n++){

      complex double y = m_fourier_spectra[CODE_WIN_ZERO][n][k]; 
      complex double yT = m_fourier_spectra[CODE_WIN_T][n][k]; 

      if(cabs(y)==0)
	output->m_data[n][k] = 1E300;
      else
	output->m_data[n][k] = creal(yT/y);
    }

    print_progress(k+1, get_height(), "s-delay");
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
roj_real_matrix* roj_xxt_analyzer :: get_chirp_rate_by_k_estimator (){

  /* check input signal */
  if(m_input_signal==NULL){
    call_warning("in roj_xxt_analyzer :: get_chirp_rate_by_k_estimator");
    call_error("signal is not loaded!");
  }
  
    /* transforming  (1, 2, and 3 slots) */
  if(m_fourier_spectra[CODE_WIN_T] == NULL)
    m_fourier_spectra[CODE_WIN_T] = transforming(0, 1);
  if(m_fourier_spectra[CODE_WIN_ZERO] == NULL)
    m_fourier_spectra[CODE_WIN_ZERO] = transforming(0, 0);
  if(m_fourier_spectra[CODE_WIN_D] == NULL)
    m_fourier_spectra[CODE_WIN_D] = transforming(1, 0);
  
  /* make empty output object */
  roj_real_matrix* output = create_empty_image();
  
  /* calc chirp rate estimate */
  for(int k=0; k<get_height(); k++){

      for(int n=0; n<get_width(); n++){

	complex double yD = m_fourier_spectra[CODE_WIN_D][n][k]; 
	complex double y = m_fourier_spectra[CODE_WIN_ZERO][n][k]; 
	complex double yT = m_fourier_spectra[CODE_WIN_T][n][k]; 

	if(cabs(y)==0)
	  output->m_data[n][k] = 1E300;
	else{
	  double nominative = creal(yD/y) / TWO_PI; 
	  double denominative = cimag(yT/y); 
	  output->m_data[n][k] = nominative / denominative;
	}
      }
    
    print_progress(k+1, get_height(), "c-rate");
  }

  print_progress(0, 0, "c-rate");
  return output;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This is an estimator of chirp-rate.
*
* @return: A pointer to obtained distribution.
*/
roj_real_matrix* roj_xxt_analyzer :: get_chirp_rate_by_k1_estimator (){

  /* check input signal */
  if(m_input_signal==NULL){
    call_warning("in roj_xxt_analyzer :: get_chirp_rate_by_k1_estimator");
    call_error("signal is not loaded!");
  }
  
    /* transforming  (1, 2, 3, and 4 slots) */
  if(m_fourier_spectra[CODE_WIN_T] == NULL)
    m_fourier_spectra[CODE_WIN_T] = transforming(0, 1);
  if(m_fourier_spectra[CODE_WIN_T2] == NULL)
    m_fourier_spectra[CODE_WIN_T2] = transforming(0, 2);
  if(m_fourier_spectra[CODE_WIN_D] == NULL)
    m_fourier_spectra[CODE_WIN_D] = transforming(1, 0);
  if(m_fourier_spectra[CODE_WIN_D2] == NULL)
    m_fourier_spectra[CODE_WIN_D2] = transforming(2, 0);
  
  /* make empty output object */
  roj_real_matrix* output = create_empty_image();
  
  /* calc chirp rate estimate */
  for(int k=0; k<get_height(); k++){

      for(int n=0; n<get_width(); n++){

	complex double yD = m_fourier_spectra[CODE_WIN_D][n][k]; 
	complex double yD2 = m_fourier_spectra[CODE_WIN_D2][n][k];
	
	complex double yT = m_fourier_spectra[CODE_WIN_T][n][k]; 
	complex double yT2 = m_fourier_spectra[CODE_WIN_T2][n][k]; 

	if(cabs(yD)==0 or cabs(yT)==0 or cabs(yT2)==0)
	  output->m_data[n][k] = 1E300;
	else{
	  double nominative = creal(yD2/yD) / TWO_PI; 
	  double denominative = cimag(yT2/yT); 
	  output->m_data[n][k] = nominative / denominative;
	}
      }
    
    print_progress(k+1, get_height(), "c-rate");
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
roj_real_matrix* roj_xxt_analyzer :: get_chirp_rate_by_d_estimator (){
  
  /* check input signal */
  if(m_input_signal==NULL){
    call_warning("in roj_xxt_analyzer :: get_chirp_rate_by_d_estimator");
    call_error("signal is not loaded!");
  }
  
  /* transforming  (0, 1, 2, 3, and 5 slots) */
  if(m_fourier_spectra[CODE_WIN_T] == NULL)
    m_fourier_spectra[CODE_WIN_T] = transforming(0, 1);
  if(m_fourier_spectra[CODE_WIN_ZERO] == NULL)
    m_fourier_spectra[CODE_WIN_ZERO] = transforming(0, 0);
  if(m_fourier_spectra[CODE_WIN_D] == NULL)
    m_fourier_spectra[CODE_WIN_D] = transforming(1, 0);
  if(m_fourier_spectra[CODE_WIN_D2] == NULL)
    m_fourier_spectra[CODE_WIN_D2] = transforming(2, 0);
  if(m_fourier_spectra[CODE_WIN_DT] == NULL)
    m_fourier_spectra[CODE_WIN_DT] = transforming(1, 1);
  
  /* make empty output object */
  roj_real_matrix* output = create_empty_image();
  
  /* calc chirp rate estimate */
  for(int k=0; k<get_height(); k++){

      for(int n=0; n<get_width(); n++){

	complex double yD2 = m_fourier_spectra[CODE_WIN_D2][n][k]; 
	complex double yD = m_fourier_spectra[CODE_WIN_D][n][k]; 
	complex double y = m_fourier_spectra[CODE_WIN_ZERO][n][k]; 
	complex double yT = m_fourier_spectra[CODE_WIN_T][n][k]; 
	complex double yDT = m_fourier_spectra[CODE_WIN_DT][n][k]; 

	if(cabs(y)==0)
	  output->m_data[n][k] = 1E300;
	else{
	  double nominative = creal(yD2 / y - yD * yD / (y * y)) / TWO_PI;
	  double denominative = cimag(yDT / y - yD * yT / (y * y));
	  output->m_data[n][k] = nominative / denominative;
	}
      }
    
    print_progress(k+1, get_height(), "c-rate");
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
roj_real_matrix* roj_xxt_analyzer :: get_chirp_rate_by_f_estimator (){

  /* check input signal */
  if(m_input_signal==NULL){
    call_warning("in roj_xxt_analyzer :: get_chirp_rate_by_f_estimator");
    call_error("signal is not loaded!");
  }
  
  /* transforming  (1, 2, 3, 4, and 5 slots) */
  if(m_fourier_spectra[CODE_WIN_T] == NULL)
    m_fourier_spectra[CODE_WIN_T] = transforming(0, 1);
  if(m_fourier_spectra[CODE_WIN_ZERO] == NULL)
    m_fourier_spectra[CODE_WIN_ZERO] = transforming(0, 0);
  if(m_fourier_spectra[CODE_WIN_D] == NULL)
    m_fourier_spectra[CODE_WIN_D] = transforming(1, 0);
  if(m_fourier_spectra[CODE_WIN_T2] == NULL)
    m_fourier_spectra[CODE_WIN_T2] = transforming(0, 2);
  if(m_fourier_spectra[CODE_WIN_DT] == NULL)
    m_fourier_spectra[CODE_WIN_DT] = transforming(1, 1);
  
  /* make empty output object */
  roj_real_matrix* output = create_empty_image();
  
  /* calc chirp rate estimate */
  for(int k=0; k<get_height(); k++){

      for(int n=0; n<get_width(); n++){

	complex double yD = m_fourier_spectra[CODE_WIN_D][n][k]; 
	complex double y = m_fourier_spectra[CODE_WIN_ZERO][n][k]; 
	complex double yT = m_fourier_spectra[CODE_WIN_T][n][k]; 
	complex double yT2 = m_fourier_spectra[CODE_WIN_T2][n][k]; 
	complex double yDT = m_fourier_spectra[CODE_WIN_DT][n][k]; 

	if(cabs(y)==0)
	  output->m_data[n][k] = 1E300;
	else{
	  double nominative = cimag(yDT / y - yD * yT / (y * y))  / TWO_PI;
	  double denominative = creal(yT2 / y - yT * yT / (y * y));
	  output->m_data[n][k] = -nominative / denominative;
	}
      }
    
    print_progress(k+1, get_height(), "c-rate");
  }

  print_progress(0, 0, "c-rate");
  return output;
}



/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This is an estimator of degree of freedom density.
*
* @return: A pointer to obtained distribution.
*/
roj_real_matrix* roj_xxt_analyzer :: get_dof_density (){

  /* check input signal */
  if(m_input_signal==NULL){
    call_warning("in roj_xxt_analyzer :: get_dof_density");
    call_error("signal is not loaded!");
  }
  
    /* transforming  (1, 2, and 3 slots) */
  if(m_fourier_spectra[CODE_WIN_T] == NULL)
    m_fourier_spectra[CODE_WIN_T] = transforming(0, 1);
  if(m_fourier_spectra[CODE_WIN_ZERO] == NULL)
    m_fourier_spectra[CODE_WIN_ZERO] = transforming(0, 0);
  if(m_fourier_spectra[CODE_WIN_D] == NULL)
    m_fourier_spectra[CODE_WIN_D] = transforming(1, 0);
  
  /* make empty output object */
  roj_real_matrix* output = create_empty_image();
  
  /* calc chirp rate estimate */
  for(int k=0; k<get_height(); k++){

      for(int n=0; n<get_width(); n++){

	complex double yD = m_fourier_spectra[CODE_WIN_D][n][k]; 
	complex double y = m_fourier_spectra[CODE_WIN_ZERO][n][k]; 
	complex double yT = m_fourier_spectra[CODE_WIN_T][n][k]; 

	if(cabs(y)==0)
	  output->m_data[n][k] = 1E300;
	else{
	  double nominative = creal(yD/y) / TWO_PI; 
	  double denominative = cimag(yT/y);
	  output->m_data[n][k] = abs(nominative * denominative);
	}
      }
    
    print_progress(k+1, get_height(), "dof");
  }

  print_progress(0, 0, "dof");
  return output;
}
