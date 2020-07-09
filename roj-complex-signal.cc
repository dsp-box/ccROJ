/* *************************************************** *
 * This file is a part of ccROJ project (version 0-49) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#include "roj-complex-signal.hh"

/* ************************************************************************************************************************* */
/**
 * @type: constructor
 * @brief: This is a constructor of roj_complex_signal based on arrays. A new signal will be filled by samples.  
 *
 * @param [in] a_real: An array which constitutes the real part for the resultant signal.
 * @param [in] a_imag (default NULL): An array which constitutes the imag part for the resultant signal.
 */
roj_complex_signal :: roj_complex_signal (roj_real_array* a_real, roj_real_array* a_imag){

  if(a_real == NULL){
    call_warning("in roj_complex_signal :: roj_complex_signal");
    call_error("arg is null in roj_complex_signal(*arr, *arr)");
  }

  roj_array_config conf = a_real->get_config();
  if(a_imag!=NULL)
    if(!a_imag->compare_config(conf)){
      call_warning("in roj_complex_signal :: roj_complex_signal");
      call_error("confs are diffrent");
    }

  /* check args */
  if (conf.length <= 0){
    call_warning("in roj_complex_signal :: roj_complex_signal");
    call_error("length <= 0");
  }

  if (conf.max<=conf.min){
    call_warning("in roj_complex_signal :: roj_complex_signal");
    call_error("max < min");
  }

  m_config.length = conf.length;
  m_config.rate = (double)(conf.length-1)/(conf.max-conf.min);
  m_config.start = conf.min;
  
  /* allocate memory for samples */
  m_waveform = new complex double [m_config.length];
  if(a_imag==NULL)
    for(int n=0; n<m_config.length; n++)
      m_waveform[n] = a_real->m_data[n] + 0J;
  else
    for(int n=0; n<m_config.length; n++)
      m_waveform[n] = a_real->m_data[n] + 1J * a_imag->m_data[n];

  call_info("copied samples: ", m_config.length);
}

/**
 * @type: constructor
 * @brief: This is a constructor of roj_complex_signal based on configuration. A new signal will be empty, however memory will be allocated. 
 *
 * @param [in] a_conf: Configuration for a new signal object.
 *
 * @return: Resultant signal object.
 */
roj_complex_signal :: roj_complex_signal (roj_signal_config a_conf){

  /* check args */
  if (a_conf.length <= 0){
    call_warning("in roj_complex_signal :: roj_complex_signal");
    call_error("length <= 0");
  }

  if (a_conf.rate<0){
    call_warning("in roj_complex_signal :: roj_complex_signal");
    call_error("rate < 0");
  }

  /* save arguments */
  m_config = a_conf;
  
  /* allocate memory for samples */
  m_waveform = new complex double [m_config.length];
  int byte_size = m_config.length * sizeof(complex double);
  memset(m_waveform, 0x0, byte_size);
}

/**
 * @type: constructor
 * @brief: This is a copy constructor of roj_complex_signal. 
 *
 * @param [in] a_sig: Pointer to a signal object which will be copied.
 *
 * @return: Resultant signal object.
 */
roj_complex_signal :: roj_complex_signal (roj_complex_signal* a_sig){

  /* check args */
  if(a_sig == NULL){
    call_warning("in roj_complex_signal :: roj_complex_signal");
    call_error("arg is null in roj_complex_signal(*sig)");
  }

  /* copy and check config */
  m_config = a_sig->get_config();
  if (m_config.length <= 0){
    call_warning("in roj_complex_signal :: roj_complex_signal");
    call_error("length <= 0");
  }

  if (m_config.rate<0){
    call_warning("in roj_complex_signal :: roj_complex_signal");
    call_error("rate < 0");
  }

  /* allocate memory and copy samples */
  m_waveform = new complex double [m_config.length];
  int byte_size = m_config.length * sizeof(complex double);
  memcpy(m_waveform, a_sig->m_waveform, byte_size);

  call_info("copied samples: ", m_config.length);
}

/**
 * @type: constructor
 * @brief: This is a constructor of roj_complex_signal based on WAV file. A new signal will be filled. 
 *
 * @param [in] a_wav_filename: A name of WAV file.
 * @param [in] a_channel_number (default 0): A channel number.
 *
 * @return: Resultant signal object.
 */
roj_complex_signal :: roj_complex_signal (char* a_wav_filename, int a_channel_number){

  /* check file extention */
  int len = strlen(a_wav_filename);
  if (len<5){
    call_warning("in roj_complex_signal :: roj_complex_signal");
    call_error("not see as wav file");
  }

  if (strcmp(&a_wav_filename[len-4], ".wav") and
      strcmp(&a_wav_filename[len-4], ".WAV")){
    call_warning("in roj_complex_signal :: roj_complex_signal");
    call_error("not see as wav file");
  }

  /* open snd file */
  SF_INFO info;
  SNDFILE *sndfile = sf_open(a_wav_filename, SFM_READ, &info);
  if (sndfile == NULL){
    call_warning("in roj_complex_signal :: roj_complex_signal");
    call_error((char *)sf_strerror(sndfile));
  }

  /* check channels number */
  if(a_channel_number>=info.channels or a_channel_number<0){
    call_warning("in roj_complex_signal :: roj_complex_signal");
    call_error("wrong channel number");
  }

  /* load samples */
  double *buffer = new double[info.channels*info.frames];
  int loaded = sf_readf_double(sndfile, buffer, info.frames);
  if (loaded!=info.frames)
    call_warning("cannot read all samples");
  call_info("number of load samples: ", loaded);

  /* configure signal object */
  m_config.start = 0.0;
  m_config.length = info.frames;
  m_config.rate = (double)info.samplerate;
  m_waveform = new complex double [m_config.length];
  int byte_size = m_config.length * sizeof(complex double);
  memset(m_waveform, 0x0, byte_size);
  for (int n=0; n<loaded; n++)
    m_waveform[n] = buffer[n*info.channels+a_channel_number];
  
  /* ending */
  delete [] buffer;
  sf_close(sndfile);
}

/**
 * @type: destructor
 * @brief: This is a signal deconstructor. It also release memory for samples. 
 */
roj_complex_signal :: ~roj_complex_signal (){

  delete [] m_waveform;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine returns signal parameters as a structure.
 *
 * @return: A pointer to the structure which contains the signal configuration.
 */
roj_signal_config roj_complex_signal :: get_config (){

  return m_config;
}

/**
 * @type: method
 * @brief: This routine compares the internal configuration to a given configuration.
 *
 * @param [in] a_conf: A pointer to the given configuration.
 *
 * @return: True if configurations are equal, false oterwise.
 */
bool roj_complex_signal :: compare_config (roj_signal_config a_conf){

  if (a_conf.rate != m_config.rate)
    return false;
  if (a_conf.start != m_config.start)
    return false;
  if (a_conf.length != m_config.length)
    return false;

  return true;
}

/**
 * @type: method
 * @brief: This routine computes an instant of last sample.
 *
 * @return: the instant of last sample.
 */
double roj_complex_signal :: get_last_instant (){

  return m_config.start + (double)(m_config.length-1)/m_config.rate;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine checks the real part.
 *
 * @return: True if real values exists, false oterwise.
 */
bool roj_complex_signal :: check_real (){

  for(int n=0; n<m_config.length; n++)   
    if(creal(m_waveform[n])!=0.0)
      return true;
  
  return false;
}

/**
 * @type: method
 * @brief: This routine checks the imag part.
 *
 * @return: True if imag values exists, false oterwise.
 */
bool roj_complex_signal :: check_imag (){

  for(int n=0; n<m_config.length; n++)   
    if(cimag(m_waveform[n])!=0.0)
      return true;
  
  return false;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine gets the real part.
 *
 * @return: pointer to roj_real_array. [HAS TO BE TESTED]
 */
roj_real_array* roj_complex_signal :: get_real (){

  roj_array_config arr_conf = convert_config (m_config);
  roj_real_array *our_arr = new roj_real_array(arr_conf);
   
  for(int n=0; n<m_config.length; n++)   
    our_arr->m_data[n] = creal(m_waveform[n]);

  return our_arr;
}

/**
 * @type: method
 * @brief: This routine gets the imag part. [HAS TO BE TESTED]
 *
 * @return: pointer to roj_real_array.
 */
roj_real_array* roj_complex_signal :: get_imag (){

  roj_array_config arr_conf = convert_config (m_config);
  roj_real_array *our_arr = new roj_real_array(arr_conf);
   
  for(int n=0; n<m_config.length; n++)   
    our_arr->m_data[n] = cimag(m_waveform[n]);

  return our_arr;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine sets all samples as zeros.
 */
void roj_complex_signal :: clear (){
  
  int byte_size = m_config.length * sizeof(complex double);
  memset(m_waveform, 0x0, byte_size);
}

/**
 * @type: method
 * @brief: This routine removes the imag part.
 *
 * @return: sum of abs of removed part.
 */
double roj_complex_signal :: clear_imag (){

  double imag_sum = 0.0;
  for(int n=0; n<m_config.length; n++){
    imag_sum += cimag(m_waveform[n]) > 0 ? cimag(m_waveform[n]) : -cimag(m_waveform[n]);
    m_waveform[n]=creal(m_waveform[n]);
  }
  
  return imag_sum;
}

/**
 * @type: method
 * @brief: This routine removes the real part.
 *
 * @return: sum of abs of removed part.
 */
double roj_complex_signal :: clear_real (){

  double real_sum = 0.0;
  for(int n=0; n<m_config.length; n++){
    real_sum += creal(m_waveform[n]) > 0 ? creal(m_waveform[n]) : -creal(m_waveform[n]);
    m_waveform[n ]=1I * cimag(m_waveform[n]);
  }
  
  return real_sum;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine copies samples from other signal object.
 *
 * @param [in] a_sig: An object from which samples are copied.
 * @param [in] a_index (default 0): An index in a buffer of other object from samples are copied.
 *
 * @return: A number of copied samples.
 */
int roj_complex_signal :: copy (roj_complex_signal* a_sig, int a_index){

  roj_signal_config conf = a_sig->get_config();
  
  if(a_index<0 or a_index>=conf.length){
    call_warning("in roj_complex_signal :: copy");
    call_error("index has wrong value");
  }

  int min_length = m_config.length;
  if (m_config.length>conf.length-a_index)
    min_length = conf.length-a_index;
  
  int byte_size = min_length * sizeof(complex double);
  memcpy(m_waveform, &a_sig->m_waveform[a_index], byte_size);

  return min_length;
}
 
/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine calculates and substitutes the conjugation of each sample.
 */
void roj_complex_signal :: conjugate (){

  for(int n=0; n<m_config.length; n++)   
    m_waveform[n] = creal(m_waveform[n])  -1I * cimag(m_waveform[n]);
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine performs a simple modulation.
 *
 * @param [in] a_shift: Frequecy shift.
 */
void roj_complex_signal :: modulate (double a_shift){

  for(int n=0; n<m_config.length; n++){
    
    double time = (double)n/m_config.rate;
    double arg = TWO_PI * a_shift * time;
    m_waveform[n] *= cexp(1I*arg);
  }
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine performs reverse of samples.
 */
void roj_complex_signal :: reverse (){

  complex double tmp;
  for(int n=0; n<m_config.length/2; n++){
    
    tmp = m_waveform[n];
    m_waveform[n] = m_waveform[m_config.length-n-1];
    m_waveform[m_config.length-n-1] = tmp;
  }
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine performs decimation.
 *
 * @param [in] a_hop: decimation factor.
 */
void roj_complex_signal :: decimate (int a_hop){

  if(a_hop<=1){
    call_warning("in roj_complex_signal :: decimate");
    call_error("a_hop <= 1");
  }

  if(2*a_hop>=m_config.length){
    call_warning("in roj_complex_signal :: decimate");
    call_error("a_hop too large");
  }

  /* allocate memory for samples */
  int new_length = 1 + m_config.length / a_hop;
  complex double *waveform = new complex double [new_length];
  int byte_size = new_length * sizeof(complex double);
  memset(waveform, 0x0, byte_size);

  /* assign samples */
  for(int n=0; n<m_config.length; n++){
    if (n%a_hop == 0){

      waveform[n/a_hop] = m_waveform[n];
    }
  }

  /* swap */
  delete [] m_waveform;
  m_waveform = waveform;
  m_config.length = new_length;
  m_config.rate /= a_hop;
}


/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine inserts a value between samples. [HAS TO BE TESTED]
 *
 * @param [in] a_number: number of inserted samples per a single signal sample.
 * @param [in] a_value: value of inserted sample (0 is default)
 */
void roj_complex_signal :: insert (int a_number, complex double a_value){

  if(a_number<=0){
    call_warning("in roj_complex_signal :: decimate");
    call_error("a_hop <= 0");
  }

  /* allocate memory for samples */
  int new_length = m_config.length * (a_number+1);
  complex double *waveform = new complex double [new_length];
  int byte_size = new_length * sizeof(complex double);
  memset(waveform, 0x0, byte_size);

  /* insert samples */
  for(int n=0; n<m_config.length; n++){
    waveform[n*(a_number+1)] = m_waveform[n];

    for(int k=0; k<a_number; k++){
    
      waveform[n*(a_number+1) + 1 + k] = a_value;
    }
  }

  /* swap */
  delete [] m_waveform;
  m_waveform = waveform;
  m_config.length = new_length;
  m_config.rate *= a_number + 1;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine cuts waveform.
 *
 * @param [in] a_new_start: new start instant.
 * @param [in] a_new_stop: new stop instant.
 *
 * @return: A new number of preserved samples.
 */
unsigned int roj_complex_signal :: cut (double a_new_start, double a_new_stop){

  /* checking arguments */
  if (a_new_start>=a_new_stop){
    call_warning("in roj_complex_signal :: cut");
    call_error("start should be smalles than stop");
  }

  if (a_new_start<m_config.start){
    call_warning("given start is smaller than current start");
    a_new_start=m_config.start;
  }

  /* checking arguments */
  double max_time = m_config.start+(double)(m_config.length-1)/m_config.rate;
  if (a_new_stop>max_time){
    call_warning("given stop is larger than current stop");
    a_new_stop=max_time;
  }
  
  /* new shorter waveform allocation */  
  int new_length = (a_new_stop-a_new_start) * m_config.rate;
  int new_initial = (a_new_start - m_config.start) * m_config.rate;
  if(new_length+new_initial>m_config.length){
    call_warning("in roj_complex_signal :: cut");
    call_error("new length + new start is too long ");
  }

  if (new_length<3){
    call_warning("in roj_complex_signal :: cut");
    call_error("new length is too short ");
  }
  complex double* new_waveform = new complex double [new_length];
 
  /* copying of samples */
  memcpy(new_waveform, &m_waveform[new_initial], sizeof(complex double) * new_length);
  delete m_waveform;

  /* actualization of configuration */
  m_config.start = (double)new_initial/m_config.rate;
  m_config.length = new_length;
  m_waveform = new_waveform;
  
  return m_config.length;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine attaches silence at the beginning and at the end.
 *
 * @param [in] a_head: duration (in seconds) of attached silence.
 * @param [in] a_tail: duration (in seconds) of attached silence.
 *
 * @return: A number of new waveform samples.
 */
unsigned int roj_complex_signal :: append (double a_head, double a_tail){
  
  unsigned int appended_samples = 0;

  if (a_head>0)
    appended_samples += append_head (a_head);

  if (a_tail>0)
    appended_samples += append_tail (a_tail);

  return appended_samples;
}

/**
 * @type: private
 * @brief: This routine attaches silence at the beginning.
 *
 * @param [in] a_duration: duration (in seconds) of attached silence.
 *
 * @return: A number of new waveform samples.
 */
unsigned int roj_complex_signal :: append_head (double a_duration){

  if(a_duration<=0){
    call_warning("in roj_complex_signal :: append_zero_head");
    call_error("duration <= 0");
  }  
  unsigned int number = a_duration*m_config.rate;
  
  if(number*m_config.rate != a_duration)
    call_warning("duration rate product is not integer");

  complex double *waveform = new complex double [m_config.length + number];
  memcpy(&waveform[number], m_waveform, m_config.length * sizeof(complex double));
  memset(waveform, 0x0, number * sizeof(complex double));

  delete [] m_waveform;

  m_waveform = waveform;
  m_config.length += number;
  m_config.start -= (double)number/m_config.rate;

  return m_config.length;
}

/**
 * @type: private
 * @brief: This routine attaches silence at the end.
 *
 * @param [in] a_duration: duration (in seconds) of attached silence.
 *
 * @return: A number of new waveform samples.
 */
unsigned int roj_complex_signal :: append_tail (double a_duration){

  if(a_duration<=0){
    call_warning("in roj_complex_signal :: append_zero_tail");
    call_error("duration <= 0");
  }
  unsigned int number = a_duration*m_config.rate;

  if(number*m_config.rate != a_duration)
    call_warning("duration rate product is not integer");
  
  complex double *waveform = new complex double [m_config.length + number];
  memcpy(waveform, m_waveform, m_config.length * sizeof(complex double));
  memset(&waveform[m_config.length], 0x0, number * sizeof(complex double));

  delete [] m_waveform;

  m_waveform = waveform;
  m_config.length += number;

  return m_config.length;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine calculates signal energy.
 *
 * @return: Signal energy.
 */
double roj_complex_signal :: calc_energy (){

  /* calculation of signal energy */
  
  double energy = 0.0;
  for(int n=0; n<m_config.length; n++){

    double re = creal(m_waveform[n]);
    double im = cimag(m_waveform[n]);
    energy += pow(re, 2.0) + pow(im, 2.0);
  }

  return energy;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine removes constant value (DC offset).
 *
 * @return: removed constant value;
 */
complex double roj_complex_signal :: remove_const(){

  complex double dc = 0.0;
  for(int n=0; n<m_config.length; n++)
    dc += m_waveform[n];
  dc /= m_config.length;

  for(int n=0; n<m_config.length; n++)
    m_waveform[n] -= dc;

  return dc;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine transforms signal to its Fourier spectrum.
 *
 * @return: A pointer to the object roj_fourier_spectrum.
 */
roj_fourier_spectrum* roj_complex_signal :: get_spectrum (){
  
  return new roj_fourier_spectrum(this);
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine estimates the instantaneous complex frequency (in the frequency domain). The real part is the instantaneous bandwidth and the imaginary part is the instantaneous frequency. (HAVE TO BE TESTED)
 *
 * @return: A pointer to a new signal object which keeps the instantaneous complex frequency.
 */
roj_complex_signal* roj_complex_signal :: get_instantaneous_complex_frequency (){

  roj_fourier_spectrum* spectrum;
  roj_complex_signal* equiv;
  
  if(check_imag())
    spectrum = get_spectrum();  
  else{
    roj_hilbert_equiv *hilbert = new roj_hilbert_equiv(this);
    equiv = hilbert->get_equivalent();
    spectrum = equiv->get_spectrum();
    delete hilbert;
  }

  for(int n=0; n<m_config.length; n++){
    double ramp = (double) n / m_config.length -0.5; // ??
    spectrum->m_spectrum[n] *= ramp * I;
  }

  roj_complex_signal* icf = spectrum->get_signal();
  
  if(check_imag())
    for(int n=0; n<m_config.length; n++)
      icf->m_waveform[n] /= m_waveform[n];  
  else{
    for(int n=0; n<m_config.length; n++)
      icf->m_waveform[n] /= equiv->m_waveform[n];
    delete equiv;
  }

  for(int n=0; n<m_config.length; n++)
    icf->m_waveform[n] *= m_config.rate;

  delete spectrum;
  return icf;
}

/**
 * @type: method
 * @brief: This routine estimates the instantaneous frequency (in the time domain). (HAVE TO BE TESTED)
 *
 * @return: A pointer to a new signal object which keeps the instantaneous frequency as the real part.
 */
roj_complex_signal* roj_complex_signal :: get_instantaneous_frequency (){

  roj_complex_signal* out = new roj_complex_signal(m_config);
  roj_complex_signal* equiv = NULL;
  complex double* waveform;

  if(!check_imag()){
    roj_hilbert_equiv *hilbert = new roj_hilbert_equiv(this);
    equiv = hilbert->get_equivalent();
    waveform = equiv->m_waveform;
    delete hilbert;  
  }
  else
    waveform = m_waveform;
  
  out->m_waveform[0] = m_config.rate * carg(waveform[1]*conj(waveform[0])) / TWO_PI;
  out->m_waveform[m_config.length-1] = m_config.rate * 
    carg(waveform[m_config.length-1]*conj(waveform[m_config.length-2])) / TWO_PI;

  for(int n=1; n<m_config.length-1; n++){
    double d1 = carg(waveform[n+1]*conj(waveform[n]));
    double d2 = carg(waveform[n]*conj(waveform[n-1]));
    out->m_waveform[n] = 0.5 * m_config.rate * (d1+d2) / TWO_PI;
  }
  
  if(equiv)
    delete equiv;
  return out;
}

/**
 * @type: method
 * @brief: This routine estimates the instantaneous bandwidth (in the time domain). (HAVE TO BE TESTED)
 *
 * @return: A pointer to a new signal object which keeps the (signed) instantaneous bandwidth as the real part.
 */
roj_complex_signal* roj_complex_signal :: get_instantaneous_bandwidth (){
  
  roj_complex_signal* out = new roj_complex_signal(m_config);
  roj_complex_signal* equiv = NULL;
  complex double* waveform;

  if(!check_imag()){
    roj_hilbert_equiv *hilbert = new roj_hilbert_equiv(this);
    equiv = hilbert->get_equivalent();
    waveform = equiv->m_waveform;
    delete hilbert;  
  }
  else
    waveform = m_waveform;
  
  out->m_waveform[0] = m_config.rate * log(cabs(waveform[1])/cabs(waveform[0])) / TWO_PI;
  out->m_waveform[m_config.length-1] = m_config.rate * 
    log(cabs(waveform[m_config.length-1])/cabs(waveform[m_config.length-2])) / TWO_PI;

  for(int n=1; n<m_config.length-1; n++){
    double d1 = log(cabs(waveform[n+1])/cabs(waveform[n]));
    double d2 = log(cabs(waveform[n])/cabs(waveform[n-1]));
    out->m_waveform[n] = 0.5 * m_config.rate * (d1+d2) / TWO_PI;
  }
  
  if(equiv)
    delete equiv;
  return out;
}

/**
 * @type: method
 * @brief: This routine estimates the instantaneous chirp rate (in the time domain). (HAVE TO BE TESTED)
 *
 * @return: A pointer to a new signal object which keeps the instantaneous chirp rate as the real part.
 */
roj_complex_signal* roj_complex_signal :: get_instantaneous_chirp_rate (){

  roj_complex_signal* out = new roj_complex_signal(m_config);
  roj_complex_signal* equiv = NULL;
  complex double* waveform;

  if(!check_imag()){
    roj_hilbert_equiv *hilbert = new roj_hilbert_equiv(this);
    equiv = hilbert->get_equivalent();
    waveform = equiv->m_waveform;
    delete hilbert;  
  }
  else
    waveform = m_waveform;
  
  out->m_waveform[0] = 0.0; 
  out->m_waveform[m_config.length-1] = 0.0;

  for(int n=1; n<m_config.length-1; n++){
    
    double a = carg(waveform[n]*waveform[n]*conj(waveform[n+1]*waveform[n-1]));
    out->m_waveform[n] = 0.5 * m_config.rate * a / TWO_PI;
  }
  
  if(equiv)
    delete equiv;
  return out;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine saves signal to a TXT file.
 *
 * @param [in] a_fname: Name of saved file.
 */
void roj_complex_signal :: save (char * a_fname){

  /* open file to write */
  FILE *fds = fopen(a_fname, "w");
  if (fds==NULL){
    call_warning("in roj_complex_signal :: save");
    call_error("cannot save");
  }

  /* write start and stop */
  double stop = m_config.start + (double)(m_config.length-1) / m_config.rate;
  fprintf(fds, "#LENGTH=%d\n", m_config.length);
  fprintf(fds, "#START=%e\n", m_config.start);
  fprintf(fds, "#STOP=%e\n", stop);

  for(int n=0; n<m_config.length; n++){
    
    double time = m_config.start + (double) n/m_config.rate;
    fprintf(fds, "%e\t%e\t%e\n", time, creal(m_waveform[n]), cimag(m_waveform[n]));
  }
  
  fclose(fds);  

#ifdef ROJ_DEBUG_ON
  call_info("save file: ", a_fname);
#endif
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine converts and saves signal to a WAV file.
 *
 * @param [in] a_fname: Name of saved file.
 */
void roj_complex_signal :: save_wav (char * a_fname){

  /* check file extention */
  int len = strlen(a_fname);
  if (len<5){
    call_warning("in roj_complex_signal :: save_wav");
    call_error("not see as wav file");
  }

  if (strcmp(&a_fname[len-4], ".wav") and
      strcmp(&a_fname[len-4], ".WAV")){
    call_warning("in roj_complex_signal :: save_wav");
    call_error("not see as wav extention");
  }

  /* set configuration */
  SF_INFO info;
  info.channels = 1;
  info.samplerate = m_config.rate;
  info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
  
  if (check_imag())
    info.channels = 2;

  /* open snd file */
  SNDFILE *sndfile = sf_open(a_fname, SFM_WRITE, &info);
  if (sndfile == NULL){
    call_warning("in roj_complex_signal :: save_wav");
    call_error((char *)sf_strerror(sndfile));
  }

  /* save samples */
  double *buffer = new double[info.channels*m_config.length];
  if(info.channels==2){
    for(int n=0; n<m_config.length; n++){   
      buffer[2*n+1] = cimag(m_waveform[n]);
      buffer[2*n] = creal(m_waveform[n]);
    }
  }
  else
    for(int n=0; n<m_config.length; n++)
      buffer[n] = creal(m_waveform[n]);

  int saved = sf_writef_double (sndfile, buffer, m_config.length) ;
  if (saved!=m_config.length)
    call_warning("cannot write all samples");

  /* ending */
  sf_write_sync(sndfile);
  delete [] buffer;  
  sf_close(sndfile);

#ifdef ROJ_DEBUG_ON
  call_info("save file: ", a_fname);
#endif
}

/* ************************************************************************************************************************* */
/**
 * @type: operator
 * @brief: Overloading of '+=' operator.
 *
 * @param [in] a_sig: A pointer to a signal which is added.
 */
void roj_complex_signal :: operator += (roj_complex_signal* a_sig){
  
  roj_signal_config conf = a_sig->get_config();  
  if (!compare_config(conf)){
    call_warning("in roj_complex_signal :: operator +=");
    call_error("signals are not compact");
  }

  for(int n=0; n<m_config.length; n++){
    
    m_waveform[n] += a_sig->m_waveform[n];
  }  
}

/**
 * @type: operator
 * @brief: Overloading of '-=' operator.
 *
 * @param [in] a_sig: A pointer to a signal which is subtracted.
 */
void roj_complex_signal :: operator -= (roj_complex_signal* a_sig){
  
  roj_signal_config conf = a_sig->get_config();  
  if (!compare_config(conf)){
    call_warning("in roj_complex_signal :: operator -=");
    call_error("signals are not compact");
  }

  for(int n=0; n<m_config.length; n++){
    
    m_waveform[n] -= a_sig->m_waveform[n];
  }  
}

/* ************************************************************************************************************************* */
/**
 * @type: operator
 * @brief: Overloading of '*=' operator.
 *
 * @param [in] a_number: a number which is used in multiplication.
 */
void roj_complex_signal :: operator *= (complex double a_number){
  
  for(int n=0; n<m_config.length; n++){
    
    m_waveform[n] *= a_number;
  }  
}

/**
 * @type: operator
 * @brief: Overloading of '/=' operator.
 *
 * @param [in] a_number: a number which is used in division.
 */
void roj_complex_signal :: operator /= (complex double a_number){
  
  for(int n=0; n<m_config.length; n++){
    
    m_waveform[n] /= a_number;
  }  
}

