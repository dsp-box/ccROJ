
#include "roj-stft-transform.hh"

/* ************************************************************************************************************************* */
/**
* @type: constructor
* @brief: This is a constructor of roj_stft_transform. The constructor only allocates memory and saves TF configuration.
*
* @param [in] a_conf:  TF configuration.
* @param [in] a_window:  pointer to used window.
*/
roj_stft_transform :: roj_stft_transform (roj_image_config a_conf, roj_complex_signal *a_window){

  /* assign window */
  if (a_window==NULL){
    call_warning("window is NULL");
    m_window = NULL;

    /* if window is null signal cannot be recovered */
  }
  else
    m_window = new roj_complex_signal(a_window);

  /* copy config data */
  m_config = a_conf;

  /* check args */
  if (!check_config ())
    call_error("matrix configuration is failed");

  /* allocate memory */
  m_spectrum = new complex double* [m_config.x.length];
  int byte_size = m_config.y.length * sizeof(complex double);
  for(int n=0; n<m_config.x.length; n++){

    m_spectrum[n] = fftw_alloc_complex(m_config.y.length);
    memset(m_spectrum[n], 0x0, byte_size);
  }
}

/**
* @type: destructor
* @brief: This is a STFT destructor. It also release memory for lines. 
*/
roj_stft_transform :: ~roj_stft_transform (){

  for(int n=0; n<m_config.x.length; n++)
     fftw_free(m_spectrum[n]);
  delete [] m_spectrum;

  if(m_window != NULL)
    delete m_window;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine allows to signal recovery.
*
* @return: pointer to reconstructed signal.
*/
roj_complex_signal* roj_stft_transform :: get_signal (){
  
  if (m_window==NULL)
    call_error("window is not set, signal cannot be recovered");

  roj_signal_config sig_config;
  sig_config.rate = (double)(m_config.x.length - 1) / (m_config.x.max - m_config.x.min);
  sig_config.length = m_config.x.length;
  sig_config.start = m_config.x.min;

  roj_complex_signal *signal = new roj_complex_signal(sig_config);
  roj_signal_config win_config = m_window->get_config();

  
  for(int n=0; n<m_config.x.length; n++){
    signal->m_waveform[n] = 0.0;

    for(int k=0; k<m_config.y.length; k++)
      signal->m_waveform[n] += pow(-1, k%2) * m_spectrum[n][k];
    
    signal->m_waveform[n] *= m_window->m_waveform[win_config.length/2];
    print_progress(n+1, m_config.x.length, "rec");
  }
  print_progress(0, 0, "rec");
  
  return signal;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine allows to convert STFT to energy spectrogram.
*
* @return: pointer to obtained energy spectrogram.
*/
roj_real_matrix* roj_stft_transform :: get_spectral_energy (){

  roj_real_matrix* s_energy = new roj_real_matrix (m_config);
  for(int n=0; n<m_config.x.length; n++)
    for(int k=0; k<m_config.y.length; k++)
      s_energy->m_data[n][k] = pow(cabs(m_spectrum[n][k]), 2);
  
  return s_energy;
}
