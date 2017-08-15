/* *************************************************** *
 * This file is a part of ccROJ project (version 0-45) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#include "roj-fourier-spectr.hh"

/* ************************************************************************************************************************* */
/**
* @type: constructor
* @brief: This is a constructor of roj_fourier_spectrum based on signal object using the rectangular window. 
*
* @param [in] a_signal: A pointer to a signal which is transformed.
*/
roj_fourier_spectrum :: roj_fourier_spectrum (roj_complex_signal* a_signal){
  
  m_config = a_signal->get_config();

  m_spectrum = fftw_alloc_complex(m_config.length);
  int byte_size = m_config.length * sizeof(complex double);
  memset(m_spectrum, 0x0, byte_size);
  
  fftw_plan pl = fftw_plan_dft_1d(m_config.length, a_signal->m_waveform, m_spectrum, FFTW_FORWARD, FFTW_ESTIMATE);

  fftw_execute(pl);  
  fft_shift();

  fftw_destroy_plan(pl);
}

/**
* @type: destructor
* @brief: This is a spectrum deconstructor. It also release memory for lines. 
*/
roj_fourier_spectrum :: ~roj_fourier_spectrum (){

  fftw_free(m_spectrum);
}

/* ************************************************************************************************************************* */
/**
* @type: private
* @brief: This internal routine performes FFT shift. 
*/
void roj_fourier_spectrum :: fft_shift (){

  complex double* tmp_buffer = new complex double[1+m_config.length/2];
  int byte_size = (m_config.length/2) * sizeof(complex double);
  memset(tmp_buffer, 0x0, byte_size + sizeof(complex double));
  
  if(m_config.length%2==0){					
    memcpy(tmp_buffer, m_spectrum, byte_size);
    memcpy(m_spectrum, &m_spectrum[m_config.length/2], byte_size);
    memcpy(&m_spectrum[m_config.length/2], tmp_buffer, byte_size);
  }
  else{
    memcpy(tmp_buffer, m_spectrum, byte_size+sizeof(complex double));
    memcpy(m_spectrum, &m_spectrum[1+m_config.length/2], byte_size);
    memcpy(&m_spectrum[m_config.length/2], tmp_buffer, byte_size+sizeof(complex double));
  }

  delete [] tmp_buffer;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine realizes the inverse Fourier transformation. 
*
* @return: A pointer to an inverted signal.
*/
roj_complex_signal* roj_fourier_spectrum :: get_signal (){

  roj_complex_signal* signal = new roj_complex_signal(m_config);
  complex double* tmp_buffer = fftw_alloc_complex(m_config.length);
  int byte_size = sizeof(complex double) * (m_config.length / 2);

  /* inverse fftshift */
  if(m_config.length%2==0){
    memcpy(tmp_buffer, &m_spectrum[m_config.length/2], byte_size);
    memcpy(&tmp_buffer[m_config.length/2], m_spectrum, byte_size);
  }
  else{
    memcpy(tmp_buffer, &m_spectrum[m_config.length/2], byte_size+sizeof(complex double));
    memcpy(&tmp_buffer[m_config.length/2+1], m_spectrum, byte_size);
  }

  /* inverse transform */
  fftw_plan pl = fftw_plan_dft_1d(m_config.length, tmp_buffer, signal->m_waveform, FFTW_BACKWARD, FFTW_ESTIMATE);
  fftw_execute(pl);  
  fftw_destroy_plan(pl);
  fftw_free(tmp_buffer);

  for (int n=0; n<m_config.length; n++)  
    signal->m_waveform[n] /= m_config.length;

  return signal;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine returns signal parameters as a structure.
*
* @return: A pointer to the structure which contains the configuration.
*/
roj_signal_config* roj_fourier_spectrum :: get_config (){

  roj_signal_config* conf = new roj_signal_config;
  memcpy(conf, &m_config, sizeof(roj_signal_config));
  
  return conf;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine roj_calculates signal energy.
*
* @return: Spectrum energy.
*/
double roj_fourier_spectrum :: calc_energy (){
  
  double energy = 0.0;
  for(int n=0; n<m_config.length; n++){

    double re = creal(m_spectrum[n]);
    double im = cimag(m_spectrum[n]);
    energy += pow(re, 2.0) + pow(im, 2.0);
  }

  return energy;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine saves spectrum to a TXT file in polar form.
*
* @param [in] a_fname: Name of saved file.
*/
void roj_fourier_spectrum :: save (char* a_fname){

  /* open file to write */
  FILE *fds = fopen(a_fname, "w");
  if (fds==NULL)
    call_error("cannot save");

  /* write start and stop */
  fprintf(fds, "#LENGTH=%d\n", m_config.length);
  fprintf(fds, "#START=%e\n", -m_config.rate/2);
  fprintf(fds, "#STOP=%e\n", m_config.rate/2);

  for(int n=0; n<m_config.length; n++){
    
    double freq = -m_config.rate/2 + (double) n*m_config.rate/m_config.length;
    double avalue = cabs(m_spectrum[n]);
    double phase = carg(m_spectrum[n]);
    fprintf(fds, "%e\t%e\t%e\n", freq, avalue, phase);
  }
  
  fclose(fds);  

#ifdef ROJ_DEBUG_ON
  call_info("save file: ", a_fname);
#endif
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine returns complex group delay (complex delay spectrum). Its imaginary part constitutes the group delay. (HAVE TO BE TESTED) 
*
* @return: A pointer to the structure which contains a new spectrum object.
*/
roj_fourier_spectrum* roj_fourier_spectrum :: get_complex_group_delay (){

  roj_complex_signal* signal = get_signal ();
  for(int n=0; n<m_config.length; n++)
    signal->m_waveform[n] *= (double) n / m_config.length;
  
  roj_fourier_spectrum* spectrum = signal->get_spectrum();
  for(int n=0; n<m_config.length; n++)
    spectrum->m_spectrum[n] /= m_spectrum[n];
  
  delete signal;
  return spectrum;
}
