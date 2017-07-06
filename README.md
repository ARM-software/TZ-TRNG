# ARM TrustZone True Number Generator

ARM TrustZone TRNG supports 32-bit systems.

    This software product contains the following open source components:

    * ARM TrustZone TRNG driver.
    * ARM TrustZone TRNG integration test.
    * Characterization reference code.
	

    Containing:

    * READM.md (this file)
    * Software and test build system:
		build.props
		proj.ext.cfg
		TRNG_test.c
		shared/
		host/  
    * Software Integration Manual   (trustzone_true_random_number_generator_software_integrators_manual_101049_0000_00_en.pdf)


## License 

    This software is provided under the Apache-2.0 license. Contributions to this project are accepted under the same license
		
	
## Release details

This software release supports ARM TrustZone TRNG software.

### Prerequisites:

    * To be run on an Ubuntu 14.04 LTS system host for building. 

    * gcc version 4.7.3 or arm-ds5 compiler are in your PATH

    * in case of using linux as a host on the H/W, configure KERNEL_DIR environment variable to point to your linux (s)

    * the target is running linux kernel 4.4 or FreeRTOS

        Download the Software Bundle from the ARM github.com url mentioned in Preface subsection e.:
        unpack to an empty folder

### Build procedure

    Step 1: Open a terminal on the Ubuntu system and 'cd' into the folder
                where you unpacked the software Bundle from the ARM github.com url.

    Step 2: Define environment variables:
				
    ```bash
    export KERNEL_DIR=/path/to/freertos
    ```
	or
    ```bash
	export KERNEL_DIR=/path/to/linux
	```		
	depending on what the hw system target is running.
				
  
	Step 3: Build all the binaries (common for FreeRTOS and Linux):
    ```bash
	cd /path/to/tztrng
	make -C host/src/tztrng_lib/ clean
	make -C host/src/tztrng_lib/
	make -C host/src/tests/tztrng_test/ clean 
	make -C host/src/tests/tztrng_test/ 
	```			
	The tztrng library is located in:
	```bash
    host/lib/libcc_tztrng.a        
    ```
	
    The integration test executable is located in the following path:
	- When compiling with gcc: host/bin/tztrng_test.
	- When compiling with armcc: host/lib/libtztrng_test.a.


    Step 4: Deploy

    For a target hw system running linux:

    Copy host/bin/tztrng_test to the target linux file system and exeute:
    ```bash
    ./tztrng_test
    ```

	For a target hw system running FreeRTOS:

	```bash
    cp host/lib/libcc_tztrng.a /path/to/your/library/folder
	cp host/lib/libtztrng_test.a /path/to/your/library/folder
	cp host/src/tests/tztrng_test/tztrng_test.h /path/to/includes
    ```
    
## Validation

1. Tests run

    See "Step 4: deploy" above.

1. Known issues
        
    None

1. Planned enhancements
        
    None



