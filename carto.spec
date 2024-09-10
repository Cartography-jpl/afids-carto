Summary: This is the Carto library
Name: carto
Version: 1.05
Release: 2.el%{rhel}
License: Copyright 2016 California Institute of Technology ALL RIGHTS RESERVED
Group: Applications/Engineering
Vendor: California Institute of Technology
URL: http://www-mipl.jpl.nasa.gov/cartlab/cartlab.html
Source0: %{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
BuildRequires: vicar-rtl gsl-devel

%description

This is the Carto library, which has various utilities used by AFIDS.

%prep
%setup -q

%build
./configure --prefix=/opt/afids_support 
make %_smp_mflags 

%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install
rm -f $RPM_BUILD_ROOT/opt/afids_support/lib/libcarto.la

%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%doc
/opt/afids_support/lib/libcarto.a
/opt/afids_support/lib/libcarto.so
/opt/afids_support/lib/libcarto.so.1
/opt/afids_support/lib/libcarto.so.1.0.0
/opt/afids_support/include/carto/*
/opt/afids_support/lib/pkgconfig/carto.pc

%changelog
* Fri Jul 20 2018 Smyth <smyth@macsmyth> - 1.05-2.el%{rhel}
- Rebuild after changes to vicar_rtl

* Tue May 17 2016 Mike M Smyth <smyth@pistol> - 1.05-1.el%{rhel}
- Change all the geotiff label code to not convert everything to upper
  case. This breaks some tools external to afids (e.g., ERDAS image
  which looks at the exact name of the projection). I believe this
  change doesn't change anything in AFIDS.

* Thu Dec 17 2015 Mike M Smyth <smyth@pistol> - 1.04-2
- Rebuild

* Mon Sep 29 2014 Mike M Smyth <smyth@pistol> - 1.04-1
- Clean up some code between carto and afids (some routines were in both)

* Thu Oct 10 2013 Mike M Smyth <smyth@pistol> - 1.03-1
- Changes by Peter to support his afids code

* Fri Aug  9 2013 Mike M Smyth <smyth@pistol> - 1.02-1
- Changes by both Peter and Ray to support their afids code

* Thu Jun  6 2013 Mike M Smyth <smyth@pistol> - 1.01-1
- Add lookup table for reflectance conversion, to speed it up.

* Mon Nov 26 2012 Mike M Smyth <smyth@pistol> - 
- Initial build.

