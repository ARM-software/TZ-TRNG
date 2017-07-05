CONTENTS

    1. Preface

        a. Proprietary notice
        b. License details
        c. Document confidentiality status
        d. Product status
        e. Web address

    2. Release details

        a. Prerequisites
        b. Build procedure

    3. Validation

        a. Tests run
        b. Known issues
        c. Planned enhancements

~~~~~

1. Preface

    a. Proprietary notice

        Copyright (c) 2017 ARM Limited.

    b. License details

        This product is licensed under the Apache-2.0 license. 
		A copy of the license can be found at https://github.com/ARM-software/TZ-TRNG/blob/master/LICENSE

    c. Document confidentiality status

        Redistribution of source and compiled code is subject to the license
        details above.

    d. Product status
		
		ARM TrustZone TRNG supports 32-bit systems.

        This software product contains the following open source components:

            (*) ARM TrustZone TRNG driver.
            (*) ARM TrustZone TRNG integration test.
            (*) Characterization reference code.

    e. Web address 

        URL:     https://github.com/ARM-software/TZ-TRNG
		

        Containing:

            (*) READM.md (this file)
            (*) Software and test build system:
					build.props
					proj.ext.cfg
					TRNG_test.c
					shared/
					host/  
			(*) Software Integration Manual (trustzone_true_random_number_generator_software_integrators_manual_101049_0000_00_en.pdf)

~~~~~

2. Release details

    This software release supports ARM TrustZone TRNG software.

    a. Prerequisites:

        (*) To be run on an Ubuntu 14.04 LTS system host for building. 

        (*) gcc version 4.7.3 or arm-ds5 compiler are in your PATH

        (*) in case of using linux as a host on the H/W, configure KERNEL_DIR environment variable to point to your
            linux (s)

        (*) the target is running linux kernel 4.4 or FreeRTOS

         Download the Software Bundle from the ARM github.com url mentioned in Preface subsection e.:
            unpack to an empty folder

   b. Build procedure

      Step 1: Open a terminal on the Ubuntu system and 'cd' into the folder
                where you unpacked the software Bundle from the ARM github.com url.

      Step 2: Define environment variables:
				
				export KERNEL_DIR=/path/to/freertos
			or
				export KERNEL_DIR=/path/to/linux
			
			depending on what the hw system target is running.
				
  
	  Step 3: Build all the binaries (common for FreeRTOS and Linux):
				cd /path/to/tztrng
				make -C host/src/tztrng_lib/ clean
				make -C host/src/tztrng_lib/
				make -C host/src/tests/tztrng_test/ clean 
				make -C host/src/tests/tztrng_test/ 
				
				The tztrng library is located in:
	                host/lib/libcc_tztrng.a
	
				The integration test executable is located in the following path:
	                - When compiling with gcc: host/bin/tztrng_test.
	                - When compiling with armcc: host/lib/libtztrng_test.a.


      Step 4: Deploy

                For a target hw system running linux:

                    Copy host/bin/tztrng_test to the target linux file system and exeute:
                        ./tztrng_test

				For a target hw system running FreeRTOS:

					cp host/lib/libcc_tztrng.a /path/to/your/library/folder
					cp host/lib/libtztrng_test.a /path/to/your/library/folder
					cp host/src/tests/tztrng_test/tztrng_test.h /path/to/includes
~~~~~

3. Validation

    a. Tests run

        See 2.b. "Step 4: deploy" above.

    b. Known issues
        
        None

    c. Planned enhancements
        
        None

~~~~~


