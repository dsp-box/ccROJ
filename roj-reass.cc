/* *************************************************** *
 * This file is a part of ccROJ project (version 0-47) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#include "roj-reass.hh"

/* ************************************************************************************************************************* */
/**
* @type: function
* @brief: Reassignment process in time and in frequency. 
*
* @param [in] a_sdelay: Spectral delay.
* @param [in] a_ifreq: Instantaneous frequency.
* @param [in] a_senergy: Spectral energy.
* @param [in,out] a_output: Reassigned spectral energy. If it is NULL, then new output image is allocated and it has configuration of input arguments.
*
* @return: A pointer to reassigned spectral energy.
*/
roj_real_matrix* roj_time_frequency_reassign (roj_real_matrix* a_sdelay, roj_real_matrix* a_ifreq, roj_real_matrix* a_senergy, roj_real_matrix* a_output){

  /* get config */
  roj_image_config conf = a_senergy->get_config();

  /* check args cohesion */
  if (!a_sdelay->compare_config(conf)){
    call_warning("in roj_time_frequency_reassign");
    call_error("images are not compact");
  }

  if (!a_ifreq->compare_config(conf)){
    call_warning("in roj_time_frequency_reassign");
    call_error("images are not compact");
  }

  /* allocate output if necessary*/  
  roj_real_matrix* output = a_output;
  if(a_output==NULL)
    output = new roj_real_matrix(conf);
  
  roj_image_config out_conf = output->get_config();
    
  /* reassignment */
  for(int n=0; n<conf.x.length; n++){
    double time = a_senergy->get_x_by_index(n);
      
    for(int k=0; k<conf.y.length; k++){
      
      double i_freq = a_ifreq->m_data[n][k];
      double c_time = time + a_sdelay->m_data[n][k];

      int t_index = output->get_index_by_x(c_time);
      if (t_index>=out_conf.x.length) continue;
      if (t_index<0) continue;

      int f_index = output->get_index_by_y(i_freq);
      if (f_index>=out_conf.y.length) continue;
      if (f_index<0) continue;

      output->m_data[t_index][f_index] += a_senergy->m_data[n][k];
    }

    print_progress(n+1, conf.x.length, "reass");
  }
  
  print_progress(0, 0, "reass");    
  return output;
}

/* ************************************************************************************************************************* */
/**
* @type: function
* @brief: Reassignment process only in frequency. 
*
* @param [in] a_ifreq: Instantaneous frequency.
* @param [in] a_senergy: Spectral energy.
* @param [in,out] a_output: Reassigned spectral energy. If it is NULL, then new output image is allocated and it has configuration of input arguments.
*
* @return: A pointer to reassigned spectral energy.
*/
roj_real_matrix* roj_frequency_reassign (roj_real_matrix* a_ifreq, roj_real_matrix* a_senergy, roj_real_matrix* a_output){

  /* get config */
  roj_image_config conf = a_senergy->get_config();

  /* check args cohesion */
  if (!a_ifreq->compare_config(conf)){
    call_warning("in roj_frequency_reassign");
    call_error("images are not compact");
  }

  /* allocate output if necessary*/  
  roj_real_matrix* output = a_output;
  if(a_output==NULL)
    output = new roj_real_matrix(conf);
  else
    if (!a_output->compare_x_config(conf)){
      call_warning("in roj_frequency_reassign");
      call_error("images are not compact in time");
    }

  roj_image_config out_conf = output->get_config();

  /* reassignment */
  for(int n=0; n<conf.x.length; n++){
    for(int k=0; k<conf.y.length; k++){
      
      double i_freq = a_ifreq->m_data[n][k];
      int f_index = output->get_index_by_y(i_freq);
      if (f_index>=out_conf.y.length) continue;
      if (f_index<0) continue;

      output->m_data[n][f_index] += a_senergy->m_data[n][k];
    }

    print_progress(n+1, conf.x.length, "reass");
  }

  print_progress(0, 0, "reass");    
  return output;
}

/* ************************************************************************************************************************* */
/**
* @type: function
* @brief: This routine roj_calculates statistical relation (similar to histogram) between input arguments over time. 
*
* @param [in] a_values: Distribution of a selected signal parameter.
* @param [in] a_energy: Spectral energy.
* @param [in] a_arr_conf: A configuration of the output profile.
*
* @return: A pointer to the resultant distribution.
*/
roj_real_matrix* roj_calculate_profile_over_time (roj_real_matrix* a_values, roj_real_matrix* a_energy, roj_array_config a_arr_conf){

  /* get config */
  roj_image_config conf = a_values->get_config();

  /* check args cohesion */
  if (!a_energy->compare_config(conf)){
    call_warning("in roj_calculate_profile_over_time");
    call_error("images are not compact");
  }

  roj_image_config n_conf;
  n_conf.x =  conf.x; 
  n_conf.y.length = a_arr_conf.length;
  n_conf.y.min = a_arr_conf.min;
  n_conf.y.max = a_arr_conf.max;

  roj_real_matrix* output = new roj_real_matrix(n_conf); 
  double delta = (double)(n_conf.y.max - n_conf.y.min) / (n_conf.y.length - 1);

  /* reassignment */
  for(int n=0; n<conf.x.length; n++){
    for(int k=0; k<conf.y.length; k++){

      int index = round((a_values->m_data[n][k] - n_conf.y.min) / delta);
      if (index>=n_conf.y.length) continue;
      if (index<0) continue;

      output->m_data[n][k] += a_energy->m_data[n][k];
    }
  }

  return output;
}

/* ************************************************************************************************************************* */
/**
* @type: function
* @brief: This routine roj_calculates statistical relation (similar to histogram) between input arguments. 
*
* @param [in] a_values: Distribution of a selected signal parameter.
* @param [in] a_energy: Spectral energy.
* @param [in] a_arr_conf: A configuration of the output profile.
*
* @return: A pointer to the resultant profile.
*/
roj_real_array* roj_calculate_profile (roj_real_matrix* a_values, roj_real_matrix* a_energy, roj_array_config a_arr_conf){

  roj_real_array* arr_ptr = new roj_real_array(a_arr_conf);
  return roj_calculate_profile(a_values, a_energy, arr_ptr);
}

/**
* @type: function
* @brief: This routine roj_calculates statistical relation (similar to histogram) between input arguments. 
*
* @param [in] a_values: Distribution of a selected signal parameter.
* @param [in] a_energy: Spectral energy.
* @param [in,out] a_arr: An array which storing the profile. This profile will be modify.
*
* @return: A pointer to resultant profile.
*/
roj_real_array* roj_calculate_profile (roj_real_matrix* a_values, roj_real_matrix* a_energy, roj_real_array* a_arr){

  /* get config */
  roj_image_config conf = a_energy->get_config();

  /* check args cohesion */
  if(!a_values->compare_config(conf)){
    call_warning("in roj_calculate_profile");
    call_error("images are not compact");
  }

  /* fill profile */
  for(int n=0; n<conf.x.length; n++)
    for(int k=0; k<conf.y.length; k++)
      add_sample_to_profile(a_values->m_data[n][k], a_energy->m_data[n][k], a_arr);

  /* return profile as roj_real_array */
  return a_arr;
}

/* ************************************************************************************************************************* */
/**
* @type: function
* @brief: This routine is can be used for add a single sample to a given profile. 
*
* @param [in] a_value: A value of a selected signal parameter.
* @param [in] a_energy: A value of spectral energy.
* @param [in,out] a_arr: An array which storing the profile. This profile will be modify.
*
* @return: True if the sample is added, false otherwise (e.g. if out of range).
*/
bool add_sample_to_profile (double a_value, double a_energy, roj_real_array* a_arr){
  
  int index = a_arr->get_index_by_arg(a_value);
  if(a_arr->check_in_index(index)){
    a_arr->m_data[index] += a_energy;
    a_arr->increment_counter();
    return true;
  }
  else
    return false;
}

/* ************************************************************************************************************************* */
/**
* @type: function
* @brief: This routine roj_calculates histogram for input first argument. 
*
* @param [in] a_values: Distribution of a selected signal parameter.
* @param [in] a_arr_conf: A configuration of the output histogram.
*
* @return: A pointer to the resultant histogram.
*/
roj_real_array* roj_calculate_histogram (roj_real_matrix* a_values, roj_array_config a_arr_conf){

  roj_real_array* arr_ptr = new roj_real_array(a_arr_conf);
  return roj_calculate_histogram(a_values, arr_ptr);
}

/**
* @type: function
* @brief: This routine roj_calculates histogram for input first argument. 
*
* @param [in] a_values: Distribution of a selected signal parameter.
* @param [in,out] a_arr: An array which storing the histogram. This histogram will be modify.
*
* @return: A pointer to the resultant histogram.
*/
roj_real_array* roj_calculate_histogram (roj_real_matrix* a_values, roj_real_array* a_arr){

  /* config */
  roj_image_config conf = a_values->get_config();
      
  /* fill histogram */
  for(int n=0; n<conf.x.length; n++)
    for(int k=0; k<conf.y.length; k++)      
      add_sample_to_histogram(a_values->m_data[n][k], a_arr);

  /* return histogram as roj_real_array */
  return a_arr;
}

/* ************************************************************************************************************************* */
/**
* @type: function
* @brief: This routine is can be used for add a single sample to a given histogram. 
*
* @param [in] a_value: A value of a selected signal parameter.
* @param [in,out] a_arr: An array which storing the histogram. This histogram will be modify.
*
* @return: True if the sample is added, false otherwise (e.g. if out of range).
*/
bool add_sample_to_histogram (double a_values, roj_real_array* a_arr){
  
  return add_sample_to_profile(a_values, 1, a_arr);
}

