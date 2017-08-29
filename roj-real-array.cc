/* *************************************************** *
 * This file is a part of ccROJ project (version 0-46) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#include "roj-real-array.hh"

/* ************************************************************************************************************************* */
/**
* @type: constructor
* @brief: This is a constructor of roj_real_array based on a configuration. A new array will be empty, however memory will be allocated. 
*
* @param [in] a_conf: Configuration for a new array object.
*/
roj_real_array :: roj_real_array (roj_array_config a_conf){

  /* check and assign conf */
  verify_config(a_conf);
  m_config = a_conf;
  
  /* allocate memory for samples */
  m_data = new double [m_config.length];
  int byte_size = m_config.length * sizeof(double);
  memset(m_data, 0x0, byte_size);

  /* usable in some cases*/
  m_counter = 0;
}

/*
* @type: constructor
* @brief: This is a copy constructor of roj_real_array. 
*
* @param [in] a_arr: A pointer to object which have to be copied.
*/
roj_real_array :: roj_real_array (roj_real_array* a_arr){

  /* check and assign conf */
  if(a_arr == NULL){
    call_warning("in roj_real_array :: roj_real_array");
    call_error("arg is null");
  }
  m_config = a_arr->get_config();
  
  /* allocate memory for samples */
  m_data = new double [m_config.length];
  int byte_size = m_config.length * sizeof(double);
  memcpy(m_data, a_arr->m_data, byte_size);

  m_counter = a_arr->return_counter();
}

/**
* @type: constructor
* @brief: This is a constructor of roj_real_array based on a a TXT file. A new array will be filled by data from the given file. 
*
* @param [in] a_fname: Filename conained data for the array.
*/
roj_real_array :: roj_real_array (char* a_fname){

  /* check arg */
  if(a_fname == NULL){
    call_warning("in roj_real_array :: roj_real_array");
    call_error("arg is null");
  }

  int len = strlen(a_fname);
  if (len<5){
    call_warning("in roj_real_array :: roj_real_array");
    call_error("not see as txt file");
  }

  if (strcmp(&a_fname[len-4], ".txt") and
      strcmp(&a_fname[len-4], ".TXT")){
    call_warning("in roj_real_array :: roj_real_array");
    call_error("not see as txt file");
  }

  FILE *f_ptr = fopen(a_fname, "r");
  if(f_ptr==NULL){
    call_warning("in roj_real_array :: roj_real_array");
    call_error("cannot read file");
  }

  double start, delta;
  fscanf(f_ptr, " # %lf %lf \n", &start, &delta);

  if(delta<=0){
    call_warning("in roj_real_array :: roj_real_array");
    call_error("delta<=0");
  }

  double tmp;
  int count = 0;
  while(!feof(f_ptr)){

    fscanf(f_ptr, " %lf \n", &tmp);
    count++;
  }
  fclose(f_ptr);
  
  /* save configuration */
  m_config.max= start + (count-1) * delta;
  m_config.length = count;
  m_config.min= start;
  verify_config(m_config);

  /* allocate memory for samples */
  m_data = new double [m_config.length];
  int byte_size = m_config.length * sizeof(double);
  memset(m_data, 0x0, byte_size);

  f_ptr = fopen(a_fname, "r");
  if(f_ptr==NULL){
    call_warning("in roj_real_array :: roj_real_array");
    call_error("cannot read file (2)");
  }

  fscanf(f_ptr, " # %lf %lf \n", &start, &delta);
  
  count=0;
  while(!feof(f_ptr)){

    fscanf(f_ptr, " %lf \n", &tmp);
    m_data[count] = tmp;
    count++;
  }
  
  fclose(f_ptr);
  m_counter = 0;

  call_info("loaded (Sa): ", count);
}

/**
* @type: destructor
* @brief: This is an array deconstructor. It also release memory for samples. 
*/
roj_real_array :: ~roj_real_array (){

  delete [] m_data;
}


/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine increments an internal counter. 
*/
void roj_real_array :: increment_counter (){

  m_counter++;
}

/**
* @type: method
* @brief: This routine returns the state of an internal counter. 
*
* @return: The counter value.
*/
int roj_real_array :: return_counter (){

  return m_counter;
}



/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine sets all samples as zeros.
*/
void roj_real_array :: clear (){

  int byte_size = m_config.length * sizeof(double);
  memset(m_data, 0x0, byte_size);
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine returns array parameters as a structure.
*
* @return: the structure which contains the configuration.
*/
roj_array_config roj_real_array :: get_config (){
    
  return m_config;
}

/**
* @type: method
* @brief: This routine compares the internal configuration to a given configuration.
*
* @param [in] a_conf: the given configuration.
*
* @return: True if configurations are equal, false oterwise.
*/
bool roj_real_array :: compare_config (roj_array_config a_conf){
  
  if(m_config.length!=a_conf.length) 
    return false;
  if(m_config.min!=a_conf.min) 
    return false;  
  if(m_config.max!=a_conf.max) 
    return false;

  return true;
}

/**
 * @type: method
 * @brief: This routine cut a piece from the base array.
 *
 * @param [in] a_head: 
 *
 * @return: new array config.
 */
roj_array_config roj_real_array :: cut_head(int a_head){
  
  if (a_head<0 or a_head>=m_config.length){
    call_warning("in roj_real_array :: cut_head");
    call_error("arg is wrong");
  }

  double d = get_delta();
  m_config.length -= a_head;
  m_config.min += a_head * d;

  double *new_data = new double[m_config.length];
  memcpy(new_data, &m_data[a_head], m_config.length*sizeof(double));
  delete [] m_data;
  m_data = new_data;

  return m_config;
}

/**
 * @type: method
 * @brief: This routine cut a piece from the base array.
 *
 * @param [in] a_tail: 
 *
 * @return: new array config.
 */
roj_array_config roj_real_array :: cut_tail(int a_tail){
    
  if (a_tail<0 or a_tail>=m_config.length){
    call_warning("in roj_real_array :: cut_tail");
    call_error("arg is wrong");
  }

  double d = get_delta();
  m_config.length -= a_tail;
  m_config.max -= a_tail * d;

  double *new_data = new double[m_config.length];
  memcpy(new_data, m_data, m_config.length*sizeof(double));
  delete [] m_data;
  m_data = new_data;

  return m_config;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine returns arguments by its index.
*
* @param [in] a_index: A given index.
*
* @return: The returned argument.
*/
double roj_real_array :: get_arg_by_index (int a_index){

  double hop = get_delta();  
  return m_config.min + a_index * hop;
}

/**
* @type: method
* @brief: This routine returns the nearest index for a given argument.
*
* @param [in] a_arg: A given argument.
*
* @return: The nearest index.
*/
int roj_real_array :: get_index_by_arg (double a_arg){

  double tmp = (a_arg - m_config.min);
  return round(tmp/get_delta());
}

/**
* @type: method
* @brief: Checking index.
*
* @param [in] a_index: A given index.
*
* @return: True if index is valid, false otherwise.
*/
bool roj_real_array :: check_in_index (int a_index){

  if(a_index>=m_config.length) return false;
  if(a_index<0) return false;
  return true;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine roj_calculates and returns space between elements.
*
* @return: The returned delta / hop.
*/
double roj_real_array :: get_delta (){
  
  double hop = (double)(m_config.max - m_config.min) / (m_config.length - 1);
  return hop;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine roj_calculates and returns the sum of all elements.
*
* @return: The sum.
*/
double roj_real_array :: get_sum (){

  double sum = 0.0;
  for(int n=0; n<m_config.length; n++)
    sum += m_data[n];
    
  return sum;
}

/**
* @type: method
* @brief: This routine roj_calculates and returns the mean of all elements.
*
* @return: The mean.
*/
double roj_real_array :: get_mean (){

  double sum = get_sum();
  return sum / m_config.length;
}

/**
* @type: method
* @brief: This routine roj_calculates and returns the variance of all elements.
*
* @return: The variance.
*/
double roj_real_array :: get_var (){

  double sum = 0.0;
  double mean = get_mean();
  for(int n=0; n<m_config.length; n++)
    sum += pow(m_data[n] - mean, 2.0);

  return sum / m_config.length;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine finds the maximal value.
*
* @return: The maximum.
*/
double roj_real_array :: get_max_value (){
  
  double max = m_data[0];
  for(int n=0; n<m_config.length; n++)
    if(max < m_data[n])
      max = m_data[n];
  return max;
}

/**
* @type: method
* @brief: This routine finds an index of the maximal value.
*
* @return: The index.
*/
int roj_real_array :: get_index_of_max (){
  
  double max = m_data[0];
  int index = 0;

  for(int n=0; n<m_config.length; n++)
    if(max < m_data[n]){
      max = m_data[n];
      index = n;
    }
  
  return index;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine saves array to a TXT file.
*
* @param [in] a_fname: Name of saved file.
*/
void roj_real_array :: save (char* a_fname){

  /* save array to a file */
  
  /* open file to write */
  FILE *fds = fopen(a_fname, "w");
  if (fds==NULL){
    call_warning("in roj_real_array :: save");
    call_error("cannot save");
  }

  /* write start and stop */
  fprintf(fds, "#LENGTH=%d\n", m_config.length);
  fprintf(fds, "#START=%e\n", m_config.min);
  fprintf(fds, "#STOP=%e\n", m_config.max);

  for(int n=0; n<m_config.length; n++){
    
    double arg = get_arg_by_index(n);
    fprintf(fds, "%e\t%e\n", arg, m_data[n]);
  }
  
  fclose(fds);  

#ifdef ROJ_DEBUG_ON
    call_info("save file: ", a_fname);
#endif
}

/* ************************************************************************************************************************* */
/**
* @type: operator
* @brief: Overloading of '=' operator.
*
* @param [in] a_number: A number which is used for assignment.
*/
void roj_real_array :: operator = (double a_number){
  
  for(int n=0; n<m_config.length; n++)    
    m_data[n] = a_number;    
}

/**
* @type: operator
* @brief: Overloading of '*=' operator.
*
* @param [in] a_factor: A factor which is used for multiplication.
*/
void roj_real_array :: operator *= (double a_factor){
  
  for(int n=0; n<m_config.length; n++)    
    m_data[n] *= a_factor;    
}

/**
* @type: operator
* @brief: Overloading of '/=' operator.
*
* @param [in] a_div: A number which is used for division.
*/
void roj_real_array :: operator /= (double a_div){
  
  for(int n=0; n<m_config.length; n++)    
    m_data[n] /= a_div;    
}

/**
* @type: operator
* @brief: Overloading of '+=' operator.
*
* @param [in] a_component: A component which is used for adding.
*/
void roj_real_array :: operator += (double a_component){
  
  for(int n=0; n<m_config.length; n++)    
    m_data[n] += a_component;    
}

/**
* @type: operator
* @brief: Overloading of '-=' operator.
*
* @param [in] a_number: A number which is used for subtraction.
*/
void roj_real_array :: operator -= (double a_number){
  
  for(int n=0; n<m_config.length; n++)    
    m_data[n] -= a_number;    
}

