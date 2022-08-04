/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois											  */
/* This file is part of cy_lib project						 					  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.				 			  */
/* FILE: cy_memory_unit.c
/*               										   		                  */
/* 																			      */
/* 																			      */
/* DESCRIPTION: Memory unit emulation                                             */
/**********************************************************************************/
#include "../../api/components/cy_memory_unit.h"

#include "cy_errors.h"


static size_t round_memory_alloc(size_t in, size_t align)
{

	size_t mask_align=(1<<align)-1;

	return ((in/align)*(align))+ (in & mask_align) ;

}


cy_error_t cy_mem_init(cy_mem_ctx_t *io_mem, const size_t allocated_t8){
	/*I. Declarations*/
	cy_error_t error=CY_KO;/* return code*/
	size_t cpt_i;
	/*II. Control parameters*/
	if(allocated_t8>_MAX_MEMORY) return CY_MEM_OVERFLOW;

	for(cpt_i=0; cpt_i<allocated_t8; cpt_i++)
	{
		io_mem->Shared_Memory[cpt_i]=_MEM_ERASED;
	}
	io_mem->offset=0;
	io_mem->is_initialized=CY_LIB_INITIALIZED;
	error=CY_OK;

	end:
	return error;
}

cy_error_t cy_mem_malloc(cy_mem_ctx_t *io_mem, size_t allocated_t8)
{
	/*I. Declarations*/
	cy_error_t error=CY_KO;/* return code*/
	size_t cpt_i;
	size_t rounded_allocated =round_memory_alloc(allocated_t8, io_mem->t8_alignement);

	 if(io_mem->is_initialized!=CY_LIB_INITIALIZED){
		 error=CY_ERR_INIT;
		 goto end;
	 }

	/*II. Control parameters*/
	if( ( io_mem->offset+rounded_allocated)> io_mem->allocated_t8 )return CY_MEM_OVERFLOW;


	/*III. Computations*/
	for(cpt_i=0; cpt_i<rounded_allocated; cpt_i++)
	{
		io_mem->Shared_Memory[cpt_i+io_mem->offset]=_MEM_ALLOCATED_UNUSUED;
	}

	io_mem->offset+=allocated_t8;
	error=CY_OK;

		end:
		return error;
}

/* note that in the current implementation, variables shall be freed in reverse order to save Shared RAM space*/
cy_error_t cy_mem_free(cy_mem_ctx_t *io_mem, const uint8_t *index, size_t allocated_t8)
{
	/*I. Declarations*/
	cy_error_t error=CY_KO;/* return code*/
	size_t cpt_i;
	size_t rounded_allocated =round_memory_alloc(allocated_t8, io_mem->t8_alignement);

	/*II. Control parameters*/
	if((int)(index-io_mem->Shared_Memory)>io_mem->offset) return CY_MEM_FREE_INDEX;

	/*III. Computations*/
	for(cpt_i=0; cpt_i<rounded_allocated; cpt_i++)
		{
			io_mem->Shared_Memory[cpt_i+index]=_MEM_ERASED;
		}

	if((io_mem->offset+io_mem->Shared_Memory)== (cpt_i+index)){
		io_mem->offset-=rounded_allocated;
	}

	index=NULL;

	error=CY_OK;

	end:
		return error;

}


/*not implemented for now*/
cy_error_t cy_mem_defrag(cy_mem_ctx_t *io_mem){

	return CY_OK;
}


/* unallocated memory shall have _MEM_ERASED value*/
cy_error_t cy_mem_check(cy_mem_ctx_t *io_mem)
{
	cy_error_t error=CY_OK;/* return code*/
	size_t cpt_i;

	for(cpt_i=io_mem->offset; cpt_i<io_mem->allocated_t8; cpt_i++)
	{
			if(io_mem->Shared_Memory[cpt_i+io_mem->offset]!=_MEM_ERASED) error=CY_KO;
	}

	return error;
}

cy_error_t cy_mem_uninit(cy_mem_ctx_t *io_mem)
{
	cy_error_t error=CY_KO;/* return code*/
	size_t cpt_i;

	Cy_CHECK(cy_memory_check(io_mem));

	for(cpt_i=0; cpt_i<allocated_t8; cpt_i++)
	{
		io_mem->Shared_Memory[cpt_i]=_MEM_ERASED;
	}
	io_mem->is_initialized=CY_LIB_UNITIALIZED;
	error=CY_OK;
	end:
	return error;

}

