#include <stdio.h>

#include <mee/pmp.h>

int protected_global;

/* TODO: Create a PMP exception handler */

int main()
{
	int rc;
	struct mee_pmp *pmp;

	printf("PMP Driver Example\n");

	/* Get PMP device handle */
	pmp = mee_pmp_get_device();
	if(!pmp) {
		fprintf(stderr, "Unable to get PMP Device\n");
		return 1;
	}

	/* Initialize PMP */
	mee_pmp_init(pmp);

	/* Perform a write to the memory we're about to protect access to */
	protected_global = 0;

	/* Configure PMPs to protect write access to the memory */

	/* Configure PMP 0 to point at &protected_global */
	rc = mee_pmp_set_address(pmp, 0, (size_t) &protected_global);
	if(rc != 0) {
		fprintf(stderr, "Failed to configure PMP 0: %d\n", rc);
		return 3;
	}

	/* Configure PMP 1 to point at (&protected_global) + 1 and only allow
	 * reads */
	struct mee_pmp_config config = {
		.L = MEE_PMP_UNLOCKED,
		.A = MEE_PMP_TOR,
		.X = 0,
		.W = 0,
		.R = 1,
	};
	rc = mee_pmp_set_region(pmp, 1, config, (size_t) ((&protected_global) + 1));
	if(rc != 0) {
		fprintf(stderr, "Failed to configure PMP 1: %d\n", rc);
		return 4;
	}

	/* Attempt to write protected_global */
	protected_global = 1;

	/* If execution returns to here, print and return the value of protected
	 * global to demonstrate that we can still read the value */
	printf("The value of the protected memory is %d\n", protected_global);

	return protected_global;
}
