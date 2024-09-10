#=================================================================
# A large number of source directories. We collect all this here, so we
# have one place to edit this. 
#
#=================================================================

# Stuff for spice
AC_DEFUN([SPICE_SOURCE_DIRECTORY],[
AC_SUBST([srcspicesupport], [spice/cspice/src/csupport])
AC_SUBST([srcspiceinc], [spice/cspice/include])
AC_SUBST([srcspice], [spice/cspice/src/cspice])
AC_SUBST([srcspicebase], [spice/cspice/src])
AC_SUBST([cspiceincdir], [${includedir}])
])

# Stuff for nitro
AC_DEFUN([NITRO_SOURCE_DIRECTORY],[
AC_SUBST([srcnitro], [nitro])
AC_SUBST([srcnitroexcept], [nitro/nitro-2.7/c++/except/source])
AC_SUBST([srcnitroincexcepttop], [nitro/nitro-2.7/c++/except/include])
AC_SUBST([srcnitroincexcept], [nitro/nitro-2.7/c++/except/include/except])
AC_SUBST([srcnitroincexceptimp], [nitro/nitro-2.7/c++/except/include/import])
AC_SUBST([srcnitroio], [nitro/nitro-2.7/c++/io/source])
AC_SUBST([srcnitroinciotop], [nitro/nitro-2.7/c++/io/include])
AC_SUBST([srcnitroincio], [nitro/nitro-2.7/c++/io/include/io])
AC_SUBST([srcnitroincioimp], [nitro/nitro-2.7/c++/io/include/import])
AC_SUBST([srcnitrologging], [nitro/nitro-2.7/c++/logging/source])
AC_SUBST([srcnitroincloggingtop], [nitro/nitro-2.7/c++/logging/include])
AC_SUBST([srcnitroinclogging], [nitro/nitro-2.7/c++/logging/include/logging])
AC_SUBST([srcnitroincloggingimp], [nitro/nitro-2.7/c++/logging/include/import])
AC_SUBST([srcnitroincmemtop], [nitro/nitro-2.7/c++/mem/include])
AC_SUBST([srcnitroincmem], [nitro/nitro-2.7/c++/mem/include/mem])
AC_SUBST([srcnitroincmemimp], [nitro/nitro-2.7/c++/mem/include/import])
AC_SUBST([srcnitromt], [nitro/nitro-2.7/c++/mt/source])
AC_SUBST([srcnitroincmttop], [nitro/nitro-2.7/c++/mt/include])
AC_SUBST([srcnitroincmt], [nitro/nitro-2.7/c++/mt/include/mt])
AC_SUBST([srcnitroincmtimp], [nitro/nitro-2.7/c++/mt/include/import])
AC_SUBST([srcnitronitf], [nitro/nitro-2.7/c++/nitf/source])
AC_SUBST([srcnitronitfapp], [nitro/nitro-2.7/c++/nitf/apps])
AC_SUBST([srcnitroincnitftop], [nitro/nitro-2.7/c++/nitf/include])
AC_SUBST([srcnitroincnitf], [nitro/nitro-2.7/c++/nitf/include/nitf])
AC_SUBST([srcnitroincnitfimp], [nitro/nitro-2.7/c++/nitf/include/import])
AC_SUBST([srcnitrocnitf], [nitro/nitro-2.7/c/nitf/source])
AC_SUBST([srcnitrocnitfshared], [nitro/nitro-2.7/c/nitf/shared])
AC_SUBST([srcnitroinccnitf], [nitro/nitro-2.7/c/nitf/include/nitf])
AC_SUBST([srcnitroinccnitftop], [nitro/nitro-2.7/c/nitf/include])
AC_SUBST([srcnitroinccnitfimp], [nitro/nitro-2.7/c/nitf/include/import])
AC_SUBST([srcnitronrt], [nitro/nitro-2.7/c/nrt/source])
AC_SUBST([srcnitroincnrttop], [nitro/nitro-2.7/c/nrt/include])
AC_SUBST([srcnitroincnrt], [nitro/nitro-2.7/c/nrt/include/nrt])
AC_SUBST([srcnitroincnrtimp], [nitro/nitro-2.7/c/nrt/include/import])
AC_SUBST([srcnitrostr], [nitro/nitro-2.7/c++/str/source])
AC_SUBST([srcnitroincstrtop], [nitro/nitro-2.7/c++/str/include])
AC_SUBST([srcnitroincstr], [nitro/nitro-2.7/c++/str/include/str])
AC_SUBST([srcnitroincstrimp], [nitro/nitro-2.7/c++/str/include/import])
AC_SUBST([srcnitrosys], [nitro/nitro-2.7/c++/sys/source])
AC_SUBST([srcnitroincsystop], [nitro/nitro-2.7/c++/sys/include])
AC_SUBST([srcnitroincsys], [nitro/nitro-2.7/c++/sys/include/sys])
AC_SUBST([srcnitroincsysimp], [nitro/nitro-2.7/c++/sys/include/import])
AC_SUBST([nitroincdir], [${includedir}/nitro])
AC_SUBST([nitroexceptincdir], [${nitroincdir}/except])
AC_SUBST([nitroioincdir], [${nitroincdir}/io])
AC_SUBST([nitrologgingincdir], [${nitroincdir}/logging])
AC_SUBST([nitromemincdir], [${nitroincdir}/mem])
AC_SUBST([nitromtincdir], [${nitroincdir}/mt])
AC_SUBST([nitronitfincdir], [${nitroincdir}/nitf])
AC_SUBST([nitronrtincdir], [${nitroincdir}/nrt])
AC_SUBST([nitrostrincdir], [${nitroincdir}/str])
AC_SUBST([nitrosysincdir], [${nitroincdir}/sys])
AC_SUBST([nitroimportincdir], [${nitroincdir}/import])
])

# Stuff for VICAR
AC_DEFUN([VICAR_SOURCE_DIRECTORY],[

AC_SUBST([srcp2prog], [vicar/p2prog/src])

AC_SUBST([srcvrdifont], [vicar/vrdi/fonts])
AC_SUBST([srcvicarlib], [vicar/vicar_lib])
AC_SUBST([srcvidslib], [vicar/vids_lib])
AC_SUBST([srctaeman1], [vicar/taetm/man/man1])
AC_SUBST([srctaeman3], [vicar/taetm/man/man3])
AC_SUBST([srctaeman5], [vicar/taetm/man/man5])
AC_SUBST([srctaepdf], [vicar/taetm/pdf])
AC_SUBST([srctaemenu], [vicar/taetm/menu])
AC_SUBST([srctaemsg], [vicar/taetm/help/msg])
AC_SUBST([srctaetm], [vicar/taetm/help/tm])
AC_SUBST([srctaem], [vicar/taetm/src])
AC_SUBST([srctaebincsh], [vicar/taetm/bin/csh])
AC_SUBST([srctaebinall], [vicar/taetm/bin/all])

AC_SUBST([p2progdir], [\${prefix}/vicar/bin])
AC_SUBST([vicardir], [\${prefix}/vicar])
AC_SUBST([vicarlibdir], [${vicardir}/lib])
AC_SUBST([vidslibdir], [${vicardir}/vids/lib])
AC_SUBST([taebinalldir], [${vicardir}/tae/bin/all])
AC_SUBST([taebincshdir], [${vicardir}/tae/bin/csh])
AC_SUBST([taebindir], [${vicardir}/tae/bin])
AC_SUBST([taeman1dir], [\${prefix}/man/man1])
AC_SUBST([taeman3dir], [\${prefix}/man/man3])
AC_SUBST([taeman5dir], [\${prefix}/man/man5])
AC_SUBST([taemenudir], [${vicardir}/tae/menu])
AC_SUBST([taemsgcpudir], [${vicardir}/tae/help/msg])
AC_SUBST([taemsgdir], [${vicardir}/tae/help/msg])
AC_SUBST([taepdfdir], [${vicardir}/tae/pdf])
AC_SUBST([taetmdir], [${vicardir}/tae/help/tm])
AC_SUBST([vrdifontdir], [\${prefix}/share/fonts/vrdi])
])

# Stuff for Afids python
AC_DEFUN([AFIDS_PYTHON_SOURCE_DIRECTORY],[
AC_SUBST([srcafidspythonlib], [afids_python/lib])
AC_SUBST([afidspkgpythondir], [\${prefix}/\${pythondir}/afids])
AC_SUBST([hidradir], [afids_python/hidra])
AC_SUBST([pommosdir], [afids_python/pommos])
AC_SUBST([srchidrapythonlib], [\${hidradir}/lib])
AC_SUBST([hidrapkgpythondir], [\${prefix}/\${pythondir}/hidra])
])
# Stuff for Carto library
AC_DEFUN([CARTO_SOURCE_DIRECTORY],[
AC_SUBST([srccarto], [carto/src])
AC_SUBST([cartoinc], [carto/inc])

AC_SUBST([cartoincdir], [${includedir}/carto])
])

# Stuff for GDAL library
AC_DEFUN([GDAL_SOURCE_DIRECTORY],[
AC_SUBST([srcopenjeg], [gdal/external/openjpeg])
AC_SUBST([srcgdalexternal], [gdal/external])
AC_SUBST([srcgdalecw], [gdal/ecw_plugin])
])


# Stuff for hdfeos library
AC_DEFUN([HDFEOS_SOURCE_DIRECTORY],[
AC_SUBST([srchdfeosexternal], [hdfeos/external])
AC_SUBST([srchdfeos], [hdfeos/external/HDF-EOS/v2.19/src])
AC_SUBST([srchdfeos5], [hdfeos/external/HDF-EOS/v5.1.16/src])
AC_SUBST([srcgctp], [hdfeos/external/HDF-EOS/v2.19/gctp/src])
AC_SUBST([hdfeosinc], [hdfeos/external/HDF-EOS/v2.19/include])
AC_SUBST([hdfeos5inc], [hdfeos/external/HDF-EOS/v5.1.16/include])
AC_SUBST([gctpinc], [hdfeos/external/HDF-EOS/v2.19/gctp/include])
AC_SUBST([hdfeosincdir], [${includedir}/hdfeos])
AC_SUBST([hdfeos5incdir], [${includedir}/hdfeos5])
])

# Stuff for ossim library
AC_DEFUN([OSSIM_SOURCE_DIRECTORY],[
AC_SUBST([srcossimexternal], [ossim/external])
])

# Stuff for xvd program
AC_DEFUN([AFIDS_XVD_SOURCE_DIRECTORY], [
AC_SUBST([srcgui], [afids_xvd/gui/src])
AC_SUBST([srcxvd], [afids_xvd/axvd/src])
AC_SUBST([guixresource], [afids_xvd/gui/xresource])
AC_SUBST([guidata], [afids_xvd/gui/data])

AC_SUBST([srcmotifapp], [afids_xvd/motif_app/src])
AC_SUBST([srcvicarmotif], [afids_xvd/vicar_motif/src])

AC_SUBST([xresourcedir], [\${prefix}/share/afids/xresource])
AC_SUBST([vicarguidatadir], [\${prefix}/vicar/data/gui])

AC_SUBST([afidsxvddir], [\${prefix}/afids/vdev])
AC_SUBST([installafidsxvddir], [\${prefix}])
AC_SUBST([etcafidsxvddir], [\${prefix}/etc/afids])
])

# Stuff for afids data
AC_DEFUN([AFIDS_DATA_SOURCE_DIRECTORY],[
AC_SUBST([datavdev], [afids_data/data/vdev])
AC_SUBST([datavextract], [afids_data/data/vextract])
AC_SUBST([datapommosprojdef], [afids_data/data/pommosdata/projdef])
AC_SUBST([dataapi], [afids_data/data/api])
AC_SUBST([datajoe], [afids_data/data/joe])
AC_SUBST([datagenealg], [afids_data/data/genealg])
AC_SUBST([datacspice], [afids_data/data/cspice])
AC_SUBST([datajoesyntax], [afids_data/data/joe/syntax])
AC_SUBST([srcdatascript], [afids_data/script])
AC_SUBST([afidsdatadir], [\${prefix}/data])
AC_SUBST([dataapidir], [\${prefix}/data/api])
AC_SUBST([datagenealgdir], [\${prefix}/data/genealg])
AC_SUBST([datavdevdir], [\${prefix}/data/vdev])
AC_SUBST([datavextractdir], [\${prefix}/data/vextract])
AC_SUBST([datapommosprojdefdir], [\${prefix}/data/pommosdata/projdef])
AC_SUBST([datacspicedir], [\${prefix}/data/cspice])
AC_SUBST([datacspicespkdir], [\${prefix}/data/cspice/spk/planets])
AC_SUBST([datacspicelskdir], [\${prefix}/data/cspice/lsk])
AC_SUBST([datacspicefkdir], [\${prefix}/data/cspice/fk/planets])
AC_SUBST([datacspicepckdir], [\${prefix}/data/cspice/pck])
AC_SUBST([datajoedir], [\${prefix}/share/joe])
AC_SUBST([datajoesyntaxdir], [\${prefix}/share/joe/syntax])

# SPICE has an annoying limitation that if a directory path is > 80
# characters we need to split this between multiple lines in a kernel file.
# This awk expression does this
AC_PROG_AWK
spicedatapath=spicedatapath.txt
if test "x$prefix" = xNONE
then
   echo $ac_default_prefix/data/cspice | $AWK -f $srcdir/config/spice_chop.awk > spicedatapath.txt
else
   echo $prefix/data/cspice | $AWK -f $srcdir/config/spice_chop.awk > spicedatapath.txt
fi
AC_SUBST_FILE([spicedatapath])
])

# Stuff for AFIDS
AC_DEFUN([AFIDS_SOURCE_DIRECTORY],[
AC_SUBST([srcafids], [afids/vdev])
AC_SUBST([srctdps], [afids/tdps])
AC_SUBST([srctma], [afids/tma])
AC_SUBST([srcacp], [afids/acp])
AC_SUBST([srcfilenotify], [afids/filenotify])
AC_SUBST([srcossimtcl], [afids/ossimTcl])
AC_SUBST([srcpdf], [afids/pdf])
AC_SUBST([srcscript], [afids/script])
AC_SUBST([srctcl], [afids/tcl])
AC_SUBST([srcdoc], [afids/doc])
AC_SUBST([srctcldata], [afids/tcl/data])
AC_SUBST([srctifftcl], [afids/tiffTcl])
AC_SUBST([vifinc], [afids/filenotify/vif])
AC_SUBST([srcgeotrans], [afids/external/geotrans/v2.2.3])
AC_SUBST([geotransinc], [afids/external/geotrans/v2.2.3])


AC_SUBST([afidsrootdir], [\${prefix}/afids])
AC_SUBST([afidsdocdir], [\${prefix}/afids/doc])
AC_SUBST([tdpsdocdir], [\${prefix}/share/doc/tdps])
AC_SUBST([tmadocdir], [\${prefix}/share/doc/tma])
AC_SUBST([etcafidsdir], [\${prefix}/etc/afids])
AC_SUBST([installafidsdir], [\${prefix}])
AC_SUBST([afidsdir], [${afidsrootdir}/vdev])
AC_SUBST([afidstcldir], [${afidsrootdir}/tcl])
AC_SUBST([tdpstcldir], [${afidsrootdir}/tdps_tcl])
AC_SUBST([tmatcldir], [${afidsrootdir}/tma_tcl])
AC_SUBST([tmatmatcldir], [${afidsrootdir}/tma_tcl/tma])
AC_SUBST([tmadbtcldir], [${afidsrootdir}/tma_tcl/dbtcl])
AC_SUBST([tmamaptcldir], [${afidsrootdir}/tma_tcl/maptcl])
AC_SUBST([datatmadir], [\${prefix}/data/tma])
AC_SUBST([tdpsdatadir], [\${prefix}/data/tdps])
AC_SUBST([tdpsgraphicsdir], [\${prefix}/data/tdps/graphics])
AC_SUBST([dataguidir], [\${prefix}/data/gui])
])



AC_DEFUN([AFIDS_ALL_SOURCE_DIRECTORY],[
# Top directory for vicar_rtl
vicar_rtl_topdir=vicar_rtl
# Top directory for vicar_gdalplugin
vicar_gdalplugin_topdir=vicar_gdalplugin
# Top directory for geocal
geocal_topdir=geocal

VICAR_RTL_SOURCE_DIRECTORY
VICAR_GDALPLUGIN_SOURCE_DIRECTORY
GEOCAL_SOURCE_DIRECTORY
AFIDS_PYTHON_SOURCE_DIRECTORY
NITRO_SOURCE_DIRECTORY
SPICE_SOURCE_DIRECTORY
VICAR_SOURCE_DIRECTORY
CARTO_SOURCE_DIRECTORY
GDAL_SOURCE_DIRECTORY
HDFEOS_SOURCE_DIRECTORY
OSSIM_SOURCE_DIRECTORY
AFIDS_DATA_SOURCE_DIRECTORY
AFIDS_XVD_SOURCE_DIRECTORY
AFIDS_SOURCE_DIRECTORY

AC_SUBST([srcblas], [external/blas])
AC_SUBST([srclapack], [external/lapack])
AC_SUBST([srcexternal], [external])
AC_SUBST([srcexternalpython], [external/python])
AC_SUBST([srcexternalscript], [external/script])
AC_SUBST([installexternaldir], [\${prefix}])
])
