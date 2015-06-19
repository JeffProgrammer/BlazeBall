project "jpeg"
	targetname "jpeg"
	language "C"
	kind "StaticLib"
	flags { "StaticRuntime" }
	targetdir (BUILD_LOCATION .. "/bin")

	includedirs {
		"../../thirdparty/jpeg",
	}

	files {
		"../../thirdparty/jpeg/jaricom.c",
		"../../thirdparty/jpeg/jcapimin.c",
		"../../thirdparty/jpeg/jcapistd.c",
		"../../thirdparty/jpeg/jcarith.c",
		"../../thirdparty/jpeg/jccoefct.c",
		"../../thirdparty/jpeg/jccolor.c",
		"../../thirdparty/jpeg/jcdctmgr.c",
		"../../thirdparty/jpeg/jchuff.c",
		"../../thirdparty/jpeg/jcinit.c",
		"../../thirdparty/jpeg/jcmainct.c",
		"../../thirdparty/jpeg/jcmarker.c",
		"../../thirdparty/jpeg/jcmaster.c",
		"../../thirdparty/jpeg/jcomapi.c",
		"../../thirdparty/jpeg/jcparam.c",
		"../../thirdparty/jpeg/jcprepct.c",
		"../../thirdparty/jpeg/jcsample.c",
		"../../thirdparty/jpeg/jctrans.c",
		"../../thirdparty/jpeg/jdapimin.c",
		"../../thirdparty/jpeg/jdapistd.c",
		"../../thirdparty/jpeg/jdarith.c",
		"../../thirdparty/jpeg/jdatadst.c",
		"../../thirdparty/jpeg/jdatasrc.c",
		"../../thirdparty/jpeg/jdcoefct.c",
		"../../thirdparty/jpeg/jdcolor.c",
		"../../thirdparty/jpeg/jddctmgr.c",
		"../../thirdparty/jpeg/jdhuff.c",
		"../../thirdparty/jpeg/jdinput.c",
		"../../thirdparty/jpeg/jdmainct.c",
		"../../thirdparty/jpeg/jdmarker.c",
		"../../thirdparty/jpeg/jdmaster.c",
		"../../thirdparty/jpeg/jdmerge.c",
		"../../thirdparty/jpeg/jdpostct.c",
		"../../thirdparty/jpeg/jdsample.c",
		"../../thirdparty/jpeg/jdtrans.c",
		"../../thirdparty/jpeg/jerror.c",
		"../../thirdparty/jpeg/jfdctflt.c",
		"../../thirdparty/jpeg/jfdctfst.c",
		"../../thirdparty/jpeg/jfdctint.c",
		"../../thirdparty/jpeg/jidctflt.c",
		"../../thirdparty/jpeg/jidctfst.c",
		"../../thirdparty/jpeg/jidctint.c",
		"../../thirdparty/jpeg/jquant1.c",
		"../../thirdparty/jpeg/jquant2.c",
		"../../thirdparty/jpeg/jutils.c",
		"../../thirdparty/jpeg/jmemmgr.c",
	}

	configuration "macosx"
		files {
		   -- Apparently the mac one is for Mac OS, not OSX
		   -- so we use nobs
			"../../thirdparty/jpeg/jmemnobs.c",
		}