
#include "roj-advance.hh"

/* ************************************************************************************************************************* */
/**
* @type: function
* @brief: Calculation of T squeezing distribution. 
*
* @param [in] a_sdelay: Spectral delay.
*
* @return: A pointer to squizeeing matrix.
*/
roj_real_matrix* roj_calculate_time_compacting (roj_real_matrix* a_sdelay){

  /* get config */
  roj_image_config conf = a_sdelay->get_config();
  double t_delta = (conf.time.max - conf.time.min)  / (conf.time.length - 1);
  double t_2delta = 2 * t_delta;
    
  /* allocate output */  
  roj_real_matrix* output = new roj_real_matrix(conf);  

  /* squeezing calculation */
  for(int n=1; n<conf.time.length-1; n++){
    for(int k=0; k<conf.frequency.length; k++){
      
      double dt1 = abs(t_delta + a_sdelay->m_data[n+1][k] - a_sdelay->m_data[n][k]);
      double dt2 = abs(t_delta + a_sdelay->m_data[n][k] - a_sdelay->m_data[n-1][k]);
      output->m_data[n][k] = (dt1 + dt2) / t_2delta;
    }
  }

  /* borders */
  for(int k=0; k<conf.frequency.length; k++){

    double dt = abs(t_delta + a_sdelay->m_data[1][k] - a_sdelay->m_data[0][k]);
    output->m_data[0][k] = dt / t_delta;

    dt = abs(t_delta + a_sdelay->m_data[conf.time.length-1][k] - a_sdelay->m_data[conf.time.length-2][k]);
    output->m_data[conf.time.length-1][k] = dt / t_delta;
  }

  return output;
}

/**
* @type: function
* @brief: Calculation of F squeezing distribution. 
*
* @param [in] a_ifreq: Instantaneous frequency.
*
* @return: A pointer to matrix for which squizeeing is roj_calculated.
*/
roj_real_matrix* roj_calculate_frequency_compacting (roj_real_matrix* a_ifreq){

  /* get config */
  roj_image_config conf = a_ifreq->get_config();
  double f_delta = (conf.frequency.max - conf.frequency.min)  / (conf.frequency.length - 1);
  double f_2delta = 2 * f_delta;
    
  /* allocate output if necessary*/  
  roj_real_matrix* output = new roj_real_matrix(conf);  

  /* squeezing calculation */
  for(int n=0; n<conf.time.length; n++){
    for(int k=1; k<conf.frequency.length-1; k++){
      
      double df1 = abs(a_ifreq->m_data[n][k+1] - a_ifreq->m_data[n][k]);
      double df2 = abs(a_ifreq->m_data[n][k] - a_ifreq->m_data[n][k-1]);
      output->m_data[n][k] = (df1 + df2) / f_2delta;
    }
  }

  /* borders */
  for(int n=0; n<conf.time.length; n++){

    double df = abs(a_ifreq->m_data[n][1] - a_ifreq->m_data[n][0]);
    output->m_data[n][0] = df / f_delta;

    df = abs(a_ifreq->m_data[n][conf.frequency.length-1] - a_ifreq->m_data[n][conf.frequency.length-2]);
    output->m_data[n][conf.frequency.length-1] = df / f_delta;
  }

  return output;
}

/* ************************************************************************************************************************* */
/* internal function */
int roj_combine_matrix (bool (*a_func)(double, double), roj_real_matrix* a_multi_values, roj_real_matrix* a_multi_metrics, roj_real_matrix* a_values, roj_real_matrix* a_metrics){

  /* check arg */
  if(a_func == NULL)
    call_error("arg is null");
  if(a_multi_values == NULL)
    call_error("arg is null");
  if(a_multi_metrics == NULL)
    call_error("arg is null");
  if(a_values == NULL)
    call_error("arg is null");
  if(a_metrics == NULL)
    call_error("arg is null");
  
  roj_image_config config =  a_multi_values->get_config();
  
  if (!a_multi_metrics->compare_config(config))
    call_error("matrixes are not compact (a_multi)");
  if (!a_values->compare_config(config))
    call_error("matrixes are not compact (a_values)");
  if (!a_values->compare_config(config))
    call_error("matrixes are not compact (a_metrics)");
  
  int counter = 0;
  for(int n=0; n<config.time.length; n++)
    for(int k=0; k<config.frequency.length; k++){
      
      if((*a_func)(a_metrics->m_data[n][k], a_multi_metrics->m_data[n][k])){
	
	a_multi_metrics->m_data[n][k] =	a_metrics->m_data[n][k];
	a_multi_values->m_data[n][k] =	a_values->m_data[n][k];

	counter++;
      }
    }
  
  return counter;
}

/* ************************************************************************************************************************* */
/* internal function */
bool cmp_minimize(double a_x, double a_y){

  return a_x <= a_y ? true : false;
}

/**
* @type: function
* @brief: This routine allows to modify (combine) a roj_real_matrix using other one based on minimalization of an associated metric. 
*
* @param [in,out] a_multi_values: A base distribution to optimize.
* @param [in,out] a_multi_metrics: An associated metric, which is a TF distribution.
* @param [in] a_values: A distribution used to optimize.
* @param [in] a_metrics: An associated metric to a_values.
*
* @return: A number of modified points.
*/
int roj_minimize_matrix (roj_real_matrix* a_multi_values, roj_real_matrix* a_multi_metrics, roj_real_matrix* a_values, roj_real_matrix* a_metrics){

  return roj_combine_matrix(cmp_minimize, a_multi_values, a_multi_metrics, a_values, a_metrics);
}

/* ************************************************************************************************************************* */
/* internal function */
bool cmp_maximize(double a_x, double a_y){

  return a_x >= a_y ? true : false;
}

/**
* @type: function
* @brief: This routine allows to modify (combine) a roj_real_matrix using other one based on maximalization of an associated metric. 
*
* @param [in,out] a_multi_values: A base distribution to optimize.
* @param [in,out] a_multi_metrics: An associated metric, which is a TF distribution.
* @param [in] a_values: A distribution used to optimize.
* @param [in] a_metrics: An associated metric to a_values.
*
* @return: A number of modified points.
*/
int roj_maximize_matrix (roj_real_matrix* a_multi_values, roj_real_matrix* a_multi_metrics, roj_real_matrix* a_values, roj_real_matrix* a_metrics){

  return roj_combine_matrix(cmp_maximize, a_multi_values, a_multi_metrics, a_values, a_metrics);
}

/* ************************************************************************************************************************* */
/**
* @type: function
* @brief: The function saves 2 coherent roj_real_matrix objects to a single file. 
*
* @param [in] a_fname: a file name.
* @param [in] a_matrix_1: a first roj_real_matrix.
* @param [in] a_matrix_2: a second roj_real_matrix.
*/
void roj_save (char *a_fname, roj_real_matrix* a_matrix_1, roj_real_matrix* a_matrix_2){

  roj_image_config config =  a_matrix_1->get_config();
  
  if (!a_matrix_2->compare_config(config))
    call_error("matrixes are not compact");

  /* open file to write */
  FILE *fds = fopen(a_fname, "w");
  if (fds==NULL)
    call_error("cannot save");

  /* write start and stop */
  fprintf(fds, "#TIME_MIN=%e\n", config.time.min);
  fprintf(fds, "#TIME_MAX=%e\n", config.time.max);
  fprintf(fds, "#FREQ_MIN=%e\n", config.frequency.min);
  fprintf(fds, "#FREQ_MAX=%e\n", config.frequency.max);

  /* init variables for finding min and max */
  double min_val = a_matrix_1->m_data[0][0];
  double max_val = a_matrix_1->m_data[0][0];

  double hop_time = (config.time.max - config.time.min) / (config.time.length - 1);
  double hop_freq = (config.frequency.max - config.frequency.min) / (config.frequency.length - 1);
  
  /* save data to file */
  for(int n=0; n<config.time.length; n++){
    double time = config.time.min + n * hop_time;
    for(int k=0; k<config.frequency.length; k++){
      double freq = config.frequency.min + k * hop_freq;

      fprintf(fds, "%e\t", time);
      fprintf(fds, "%e\t", freq);
      fprintf(fds, "%e\t", a_matrix_1->m_data[n][k]);
      fprintf(fds, "%e\n", a_matrix_2->m_data[n][k]);
  
      /* finding min and max */
      if (min_val > a_matrix_1->m_data[n][k])
	min_val = a_matrix_1->m_data[n][k];
      if (max_val < a_matrix_1->m_data[n][k]) 
	max_val = a_matrix_1->m_data[n][k]; 
    }

    /* add empty line */
    fprintf(fds, "\n");
    print_progress(n+1, config.time.length, "save");
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
