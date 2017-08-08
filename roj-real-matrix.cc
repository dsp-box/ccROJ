/* *************************************************** *
 * This file is a part of ccROJ project (version 0-44) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#include "roj-real-matrix.hh"

/* ************************************************************************************************************************* */
/**
 * @type: constructor
 * @brief: This is a constructor of roj_real_matrix which is dedicated for storing spectrografic data.
 *
 * @param [in] a_conf: a TF configuration.
 */
roj_real_matrix :: roj_real_matrix (roj_image_config a_conf){

  /* copy config data */
  m_config = a_conf;

  /* check args */
  if (!check_config ())
    call_error("matrix configuration is failed");

  /* allocate memory for data */
  m_data = new double*[m_config.x.length];
  int byte_size = m_config.y.length * sizeof(double);
  for(int n=0; n<m_config.x.length; n++){
    m_data[n] = new double[m_config.y.length];    
    memset(m_data[n], 0x0, byte_size);
  }
}

/**
 * @type: constructor
 * @brief: This is a copy constructor of roj_real_matrix which is dedicated for storing spectrografic data. The new object is copy from a pointer to a given argument.
 *
 * @param [in] a_matrix: A pointer to a given matrix.
 */
roj_real_matrix :: roj_real_matrix (roj_real_matrix* a_matrix){

  /* check args */
  if(a_matrix==NULL) call_error("conf is null");

  /* check args */
  if (!a_matrix->check_config ())
    call_error("matrix configuration is failed");

  /* copy config data */
  roj_image_config conf = a_matrix->get_config();
  m_config = conf;
  
  /* allocate memory for data */
  m_data = new double*[m_config.x.length];
  int byte_size = m_config.y.length * sizeof(double);
  for(int n=0; n<m_config.x.length; n++){
    m_data[n] = new double[m_config.y.length];    
    memcpy(m_data[n], a_matrix->m_data[n], byte_size);
  }
}

/**
 * @type: destructor
 * @brief: This is a generator deconstructor. The memory is also released.
 */
roj_real_matrix :: ~roj_real_matrix (){  

  for(int n=0;n<m_config.x.length;n++)
    delete [] m_data[n];    
  delete [] m_data;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine cleans data.
 */
void roj_real_matrix :: clear (){
  
  int byte_size = m_config.y.length * sizeof(double);
  for(int n=0; n<m_config.x.length; n++)
    memset(m_data[n], 0x0, byte_size);
}


/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine replaces negative values with its absolute value. 
 */
void roj_real_matrix :: abs (){

  for(int n=0; n<m_config.x.length; n++)
    for(int k=0; k<m_config.y.length; k++)
      
      if(m_data[n][k]<0)
	m_data[n][k] *= -1;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine replaces NaN values with 1E300. 
 */
void roj_real_matrix :: remove_nan (){

  for(int n=0; n<m_config.x.length; n++)
    for(int k=0; k<m_config.y.length; k++){
      
      if(isnan(m_data[n][k]))
	m_data[n][k] = 1E300;
      if(isinf(m_data[n][k]))
	m_data[n][k] = 1E300;
    }
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine calculates the sum of all values stored in the matrix.
 *
 * @return: Cumulated values.
 */
double roj_real_matrix :: get_sum (){
  
  double sum = 0.0;
  for(int n=0; n<m_config.x.length; n++)
    for(int k=0; k<m_config.y.length; k++)
      sum += m_data[n][k];

  return sum;
}

/**
 * @type: method
 * @brief: This routine finds the maximal value from stored in the matrix.
 *
 * @return: Maximal value.
 */
double roj_real_matrix :: get_max (){
  
  double max = m_data[0][0];
  for(int n=0; n<m_config.x.length; n++)
    for(int k=0; k<m_config.y.length; k++)
      if (max<m_data[n][k])
	max = m_data[n][k];

  return max;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine roj_calculates a marginal distribution in the x domain which represents mean value also a wighted mean.
 *
 * @param [in] a_energy (default NULL): a distribution used for weighting
 *
 * @return: a pointer to the array which contains the marginal distribution
 */
roj_real_array* roj_real_matrix :: get_mean_over_x (roj_real_matrix* a_energy){

  /* config returned distribution */
  roj_array_config conf;
  conf.length = m_config.x.length;
  conf.min = m_config.x.min;
  conf.max = m_config.x.max;

  /* allocate returned distribution */
  roj_real_array *array = new roj_real_array(conf);

  /* calc distribution if energy is not given */
  if(a_energy == NULL){
    for(int n=0; n<m_config.x.length; n++){
      for(int k=0; k<m_config.y.length; k++)
	array->m_data[n] += m_data[n][k];
      array->m_data[n] /= m_config.y.length;
      
      print_progress(n+1, m_config.x.length, "marg");
    }
  }

  /* calc distribution if energy is given */
  else{
    
    roj_image_config energy_conf = a_energy->get_config();
    if (!compare_config(energy_conf))
      call_error("matrixes are not compact");
    
    for(int n=0; n<m_config.x.length; n++){
      for(int k=0; k<m_config.y.length; k++)
	array->m_data[n] += m_data[n][k] * a_energy->m_data[n][k];
      
      print_progress(n+1, m_config.x.length, "marg");
    }

    /* div by energy */
    roj_real_array* energy = a_energy->get_mean_over_x();
    for(int n=0; n<m_config.x.length; n++){
      array->m_data[n] /= energy->m_data[n];
      array->m_data[n] /= m_config.y.length;
    }
    delete energy;
  }
  
  print_progress(0, 0, "marg");
  return array;
}

/*
 * @type: method
 * @brief: This routine roj_calculates a marginal distribution in the y domain which represents mean value also a wighted mean.
 *
 * @param [in] a_energy (default NULL): a distribution used for weighting
 *
 * @return: a pointer to the array which contains the marginal distribution
 */
roj_real_array* roj_real_matrix :: get_mean_over_y (roj_real_matrix* a_energy){

  /* config returned distribution */
  roj_array_config conf;
  conf.length = m_config.y.length;
  conf.min = m_config.y.min;
  conf.max = m_config.y.max;

  /* allocate returned distribution */
  roj_real_array *array = new roj_real_array(conf);

  /* calc distribution if energy is not given */
  if(a_energy == NULL){
    for(int k=0; k<m_config.y.length; k++){
      for(int n=0; n<m_config.x.length; n++)
	array->m_data[k] += m_data[n][k];
      array->m_data[k] /= m_config.x.length;

      print_progress(k+1, m_config.y.length, "marg");
    }
  }

  /* calc distribution if energy is given */
  else{
    
    roj_image_config energy_conf = a_energy->get_config();
    if (!compare_config(energy_conf))
      call_error("matrixes are not compact");
    
    for(int k=0; k<m_config.y.length; k++){
      for(int n=0; n<m_config.x.length; n++)
	array->m_data[k] += m_data[n][k] * a_energy->m_data[n][k];
      
      print_progress(k+1, m_config.y.length, "marg");
    }

    /* div by energy */
    roj_real_array* energy = a_energy->get_mean_over_y();
    for(int k=0; k<m_config.y.length; k++){
      array->m_data[k] /= energy->m_data[k];
      array->m_data[k] /= m_config.x.length;
    }
    delete energy;
  }
  
  print_progress(0, 0, "marg");
  return array;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine roj_calculates a marginal distribution in the time domain which represents corresponded arguments of maximal values of a given enregy distribution.
 *
 * @param [in] a_energy: A distribution used for looking for maxima.
 *
 * @return: A pointer to the array which contains the marginal distribution.
 */
roj_real_array* roj_real_matrix :: get_max_over_x (roj_real_matrix* a_energy){

  /* check arg */
  if(a_energy == NULL)
    call_error("arg is null");

  roj_image_config energy_conf = a_energy->get_config();
  if (!compare_config(energy_conf))
    call_error("matrixes are not compact");

  /* config returned distribution */
  roj_array_config conf;
  conf.length = m_config.x.length;
  conf.min = m_config.x.min;
  conf.max = m_config.x.max;

  /* allocate returned distribution */
  roj_real_array *array = new roj_real_array(conf);
  
  for(int n=0; n<m_config.x.length; n++){
    double tmp = 0.0;
    for(int k=0; k<m_config.y.length; k++){
      if(tmp<a_energy->m_data[n][k]){
	tmp=a_energy->m_data[n][k];
	array->m_data[n] = m_data[n][k];
      }
    }
    
    print_progress(n+1, m_config.x.length, "marg");
  }
  
  print_progress(0, 0, "marg");
  return array;
}

/**
 * @type: method
 * @brief: This routine roj_calculates a marginal distribution in the time domain which represents corresponded arguments of maximal values of a given enregy distribution.
 *
 * @param [in] a_energy: A distribution used for looking for maxima.
 *
 * @return: A pointer to the array which contains the marginal distribution.
 */
roj_real_array* roj_real_matrix :: get_max_over_y (roj_real_matrix* a_energy){

  /* check arg */
  if(a_energy == NULL)
    call_error("arg is null");

  roj_image_config energy_conf = a_energy->get_config();
  if (!compare_config(energy_conf))
    call_error("matrixes are not compact");

  /* config returned distribution */
  roj_array_config conf;
  conf.length = m_config.y.length;
  conf.min = m_config.y.min;
  conf.max = m_config.y.max;

  /* allocate returned distribution */
  roj_real_array *array = new roj_real_array(conf);
  
  for(int k=0; k<m_config.y.length; k++){
    double tmp = 0.0;
    for(int n=0; n<m_config.x.length; n++){
      if(tmp<a_energy->m_data[n][k]){
	tmp=a_energy->m_data[n][k];
	array->m_data[k] = m_data[n][k];
      }
    }
    
    print_progress(k+1, m_config.y.length, "marg");
  }
  
  print_progress(0, 0, "marg");
  return array;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine roj_calculates a marginal distribution in the time domain which is a profil of energy.
 *
 * @param [in] a_energy: A corresponded energy.
 * @param [in] a_conf: An range and resolution of obtained profile.
 *
 * @return: A pointer to the array which contains the marginal distribution.
 */
roj_real_array* roj_real_matrix :: get_dominant_over_x (roj_real_matrix* a_energy, roj_array_config a_conf){

  /* check arg */
  if(a_energy == NULL)
    call_error("arg is null");

  if (!a_energy->compare_config(m_config))
    call_error("matrixes are not compact");

  /* config returned distribution */
  roj_array_config out_conf;
  out_conf.length = m_config.x.length;
  out_conf.min = m_config.x.min;
  out_conf.max = m_config.x.max;
  roj_real_array *output = new roj_real_array(out_conf);

  /* tmp buffer */
  roj_real_array *buffer = new roj_real_array(a_conf);
  
  for(int n=0; n<m_config.x.length; n++){
    buffer->clear();
    
    for(int k=0; k<m_config.y.length; k++){

      int index = (m_data[n][k] - a_conf.min) /  buffer->get_delta();
      if(index>=a_conf.length) continue;
      if(index<0) continue;

      buffer->m_data[index] += a_energy->m_data[n][k];
    }
        
    int index = buffer->get_index_of_max();
    output->m_data[n] = buffer->get_arg_by_index(index);

    print_progress(n+1, m_config.x.length, "marg");
  }

  print_progress(0, 0, "marg");
  delete buffer;
  return output;
}

/**
 * @type: method
 * @brief: This routine roj_calculates a marginal distribution in the time domain which is a profil of energy. The algorithm makes correction using a corresponded group delay.
 *
 * @param [in] a_energy: A corresponded energy.
 * @param [in] a_sdelay: A spectral delay.
 * @param [in] a_conf: An range and resolution of obtained profile.
 *
 * @return: A pointer to the array which contains the marginal distribution.
 */
roj_real_array* roj_real_matrix :: get_dominant_over_x (roj_real_matrix* a_energy, roj_real_matrix* a_sdelay, roj_array_config a_conf){

  /* check arg */
  if(a_energy == NULL)
    call_error("arg is null");
  if(a_sdelay == NULL)
    call_error("arg is null");

  if (!a_energy->compare_config(m_config))
    call_error("matrixes are not compact");
  if (!a_sdelay->compare_config(m_config))
    call_error("matrixes are not compact");

  /* config returned distribution */
  roj_array_config out_conf;
  out_conf.length = m_config.x.length;
  out_conf.min = m_config.x.min;
  out_conf.max = m_config.x.max;
  roj_real_array *output = new roj_real_array(out_conf);

  /* tmp buffer */
  roj_real_array **buffer = new roj_real_array*[m_config.x.length]; 
  for(int n=0; n<m_config.x.length; n++)
    buffer[n] = new roj_real_array(a_conf);

  for(int n=0; n<m_config.x.length; n++){
    for(int k=0; k<m_config.y.length; k++){

      double time = get_x_by_index(n);
      time += a_sdelay->m_data[n][k];

      int t_index = get_index_by_x(time);
      if (t_index>=m_config.x.length) continue;
      if (t_index<0) continue;

      int index = (m_data[n][k] - a_conf.min) /  buffer[n]->get_delta();
      if(index>=a_conf.length) continue;
      if(index<0) continue;

      buffer[t_index]->m_data[index] += a_energy->m_data[n][k];
    }
        
    print_progress(n+1, m_config.x.length, "marg");
  }

  for(int n=0; n<m_config.x.length; n++){
    int index = buffer[n]->get_index_of_max();
    output->m_data[n] = buffer[n]->get_arg_by_index(index);
  }
  
  print_progress(0, 0, "marg");
  for(int n=0; n<m_config.x.length; n++)
    delete buffer[n];
  delete buffer;
  return output;
  
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine saves array to a TXT file. The results can be simply drawn using Gnuplot.
 *
 * @param [in] a_fname: Name of saved file.
 */
void roj_real_matrix :: save (char* a_fname){
  
  /* open file to write */
  FILE *fds = fopen(a_fname, "w");
  if (fds==NULL)
    call_error("cannot save");

  /* write start and stop */
  fprintf(fds, "#TIME_MIN=%e\n", m_config.x.min);
  fprintf(fds, "#TIME_MAX=%e\n", m_config.x.max);
  fprintf(fds, "#FREQ_MIN=%e\n", m_config.y.min);
  fprintf(fds, "#FREQ_MAX=%e\n", m_config.y.max);

  /* init variables for finding min and max */
  double min_val = m_data[0][0];
  double max_val = m_data[0][0];

  double hop_time = (m_config.x.max - m_config.x.min) / (m_config.x.length - 1);
  double hop_freq = (m_config.y.max - m_config.y.min) / (m_config.y.length - 1);
  
  /* save data to file */
  for(int n=0; n<m_config.x.length; n++){
    double time = m_config.x.min + n * hop_time;
    for(int k=0; k<m_config.y.length; k++){
      double freq = m_config.y.min + k * hop_freq;

      fprintf(fds, "%e\t", time);
      fprintf(fds, "%e\t", freq);
      fprintf(fds, "%e\n", m_data[n][k]);

      /* finding min and max */
      if (min_val > m_data[n][k])
	min_val = m_data[n][k];
      if (max_val < m_data[n][k]) 
	max_val = m_data[n][k];
    }

    /* add empty line */
    fprintf(fds, "\n");
    print_progress(n+1, m_config.x.length, "save");
  }
  print_progress(0, 0, "save");

  /* save min and max to file */
  fprintf(fds, "#VAL_MIN=%e\n", min_val);
  fprintf(fds, "#VAL_MAX=%e\n", max_val);
  fclose(fds);
  
#ifdef ROJ_DEBUG_ON
  call_info("save file: ", a_fname);
#endif
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine cut a piece from the base matrix.
 *
 * @param [in] a_conf: A pointer to a configuration of a new matrix.
 *
 * @return: The new cropped matrix.
 */
roj_real_matrix* roj_real_matrix :: cropping (roj_image_config a_conf){

  /* check args */
  
  int min_tindex = get_index_by_x(a_conf.x.min);
  if(min_tindex<0 or min_tindex>=m_config.x.length){
    fprintf(stderr, "min t index: %d\n", min_tindex);
    fprintf(stderr, "should be < %d\n", m_config.x.length);
    call_error("min time is wrong!");
  }

  int max_tindex = get_index_by_x(a_conf.x.max);
  if(max_tindex<0 or max_tindex>=m_config.x.length){
    fprintf(stderr, "max t index: %d\n", max_tindex);
    fprintf(stderr, "should be < %d\n", m_config.x.length);
    call_error("max time is wrong!");
  }

  if(min_tindex >= max_tindex){
    fprintf(stderr, "min t index: %d\n", min_tindex);
    fprintf(stderr, "max t index: %d\n", max_tindex);
    call_error("min / max time is wrong!");
  }

  int new_width = max_tindex-min_tindex+1;
  double telta = m_config.x.max - m_config.x.min;
  telta /= m_config.x.length-1;
  
  int min_findex = get_index_by_y(a_conf.y.min);
  if(min_findex<0 or min_findex>=m_config.y.length){
    fprintf(stderr, "min f index: %d\n", min_findex);
    fprintf(stderr, "should be < %d\n", m_config.y.length);
    call_error("min frequency is wrong!");
  }
  
  int max_findex = get_index_by_y(a_conf.y.max);
  if(max_findex<0 or max_findex>=m_config.y.length){
    fprintf(stderr, "max f index: %d\n", max_findex);
    fprintf(stderr, "should be < %d\n", m_config.y.length);
    call_error("max frequency is wrong!");
  }
  
  if(min_findex >= max_findex){
    fprintf(stderr, "min f index: %d\n", min_findex);
    fprintf(stderr, "max f index: %d\n", max_findex);
    call_error("min / max frequency is wrong!");
  }
  
  int new_height = max_findex-min_findex+1;
  double felta = m_config.y.max - m_config.y.min;
  felta /= m_config.y.length-1;

  roj_image_config img_conf;
  img_conf.x.length = new_width;
  img_conf.y.length = new_height;
  img_conf.x.min = m_config.x.min + min_tindex * telta;
  img_conf.x.max = m_config.x.min + max_tindex * telta; 
  img_conf.y.min = m_config.y.min + min_findex * felta;
  img_conf.y.max = m_config.y.min + max_findex * felta;

  roj_real_matrix* out_matrix = new roj_real_matrix(img_conf);
  int byte_size = new_height * sizeof(double); 
  for(int n=0; n<new_width; n++)
    memcpy(out_matrix->m_data[n], &m_data[n+min_tindex][min_findex], byte_size);

  return out_matrix;
}

/* ************************************************************************************************************************* */
/**
 * @type: operator
 * @brief: Overloading of =' operator.
 *
 * @param [in] a_number: A number which is assigned.
 */
void roj_real_matrix :: operator = (double a_number){
  
  for(int n=0; n<m_config.x.length; n++)
    for(int k=0; k<m_config.y.length; k++)
      m_data[n][k] = a_number;    
}

/**
 * @type: operator
 * @brief: Overloading of '*=' operator.
 *
 * @param [in] a_factor: An factor which is used for multiplication.
 */
void roj_real_matrix :: operator *= (double a_factor){
  
  for(int n=0; n<m_config.x.length; n++)
    for(int k=0; k<m_config.y.length; k++)
      m_data[n][k] *= a_factor;    
}

/**
 * @type: operator
 * @brief: Overloading of '/=' operator.
 *
 * @param [in] a_div: A number which is used for division.
 */
void roj_real_matrix :: operator /= (double a_div){
  
  for(int n=0; n<m_config.x.length; n++)
    for(int k=0; k<m_config.y.length; k++)
      m_data[n][k] /= a_div;    
}

/**
 * @type: operator
 * @brief: Overloading of '+=' operator.
 *
 * @param [in] a_component: An component which is used for sum.
 */
void roj_real_matrix :: operator += (double a_component){
  
  for(int n=0; n<m_config.x.length; n++)
    for(int k=0; k<m_config.y.length; k++)
      m_data[n][k] += a_component;    
}

/**
 * @type: operator
 * @brief: Overloading of '-=' operator.
 *
 * @param [in] a_subtrahend: A number which is used for substraction.
 */
void roj_real_matrix :: operator -= (double a_number){
  
  for(int n=0; n<m_config.x.length; n++)
    for(int k=0; k<m_config.y.length; k++)
      m_data[n][k] -= a_number;    
}

/* ************************************************************************************************************************* */
/**
 * @type: operator
 * @brief: Overloading of '*=' operator.
 *
 * @param [in] a_matrix: A pointer to matrix which is used for multiplication (element-by-element).
 */
void roj_real_matrix :: operator *= (roj_real_matrix *a_matrix){
  
  if (!a_matrix->compare_config(m_config))
    call_error("images are not compact");

  for(int n=0; n<m_config.x.length; n++)
    for(int k=0; k<m_config.y.length; k++)
      m_data[n][k] *= a_matrix->m_data[n][k];    
}

/**
 * @type: operator
 * @brief: Overloading of '+=' operator.
 *
 * @param [in] a_matrix: A pointer to matrix which is used for adding.
 */
void roj_real_matrix :: operator += (roj_real_matrix *a_matrix){
  
  if (!a_matrix->compare_config(m_config))
    call_error("images are not compact");

  for(int n=0; n<m_config.x.length; n++)
    for(int k=0; k<m_config.y.length; k++)
      m_data[n][k] += a_matrix->m_data[n][k];    
}
