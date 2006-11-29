/*
 * Copyright (c) 2005 Sandia Corporation. Under the terms of Contract
 * DE-AC04-94AL85000 with Sandia Corporation, the U.S. Governement
 * retains certain rights in this software.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 * 
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.  
 * 
 *     * Neither the name of Sandia Corporation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
/*****************************************************************************
*
* exgnv - ex_get_nodal_varid_var
*
* author - Sandia National Laboratories
*          Larry A. Schoof - Original
*          James A. Schutt - 8 byte float and standard C definitions
*          Vic Yarberry    - Added headers and error logging
*
*          
* environment - UNIX
*
* entry conditions - 
*   input parameters:
*       int     exoid                   exodus file id
*       int     time_step               whole time step number
*       int     nodal_var_index         index of desired nodal variable
*       int     num_nodes               number of nodal points
*       int     varid                   id of variable on exodus database
*
* exit conditions - 
*       float*  nodal_var_vals          array of nodal variable values
*
* revision history - 
*
*  Id
*
*****************************************************************************/

#include "exodusII.h"
#include "exodusII_int.h"

/*
 * reads the values of a single nodal variable for a single time step from 
 * the database; assume the first time step and nodal variable index is 1
 */

int ex_get_nodal_varid_var(int   exoid,
                           int   time_step,
                           int   nodal_var_index,
                           int   num_nodes, 
                           int   varid,
                           void *nodal_var_vals)
{
  long start[3], count[3];
  char errmsg[MAX_ERR_LENGTH];
  void *array;
  
  exerrval = 0; /* clear error code */

  /* inquire previously defined variable */

  if (ex_large_model(exoid) == 0) {
    start[0] = --time_step;
    start[1] = --nodal_var_index;
    start[2] = 0;

    count[0] = 1;
    count[1] = 1;
    count[2] = num_nodes;
  } else {

    start[0] = --time_step;
    start[1] = 0;

    count[0] = 1;
    count[1] = num_nodes;
  }
  array = ex_conv_array(exoid,RTN_ADDRESS,nodal_var_vals,num_nodes);
  if (ncvarget (exoid, varid, start, count, array) == -1) {
    exerrval = ncerr;
    sprintf(errmsg,
            "Error: failed to get nodal variables in file id %d",
            exoid);
    ex_err("ex_get_nodal_varid_var",errmsg,exerrval);
    return (EX_FATAL);
  }

  if (array != nodal_var_vals)
    ex_conv_array( exoid, READ_CONVERT,nodal_var_vals, num_nodes );

  return (EX_NOERR);
}
