Name:		psstop
Version:	1
Release:	1%{?dist}
Summary:	Show processes' memory usage by looking into pss

Group:		Applications/System
License:	GPLv3
URL:		https://github.com/clearlinux/psstop
Source0:	psstop-%{version}.tar.gz


%description
PPSTOP tracks the amount of memory from the process' Pss,
as shown in /proc/PID/smaps

%prep
%setup -q


%build
%configure
make %{?_smp_mflags}


%install
rm -rf %{buildroot}
%make_install

%clean
rm -rf %{buildroot}

%files
%doc
%{_bindir}/*




%changelog
* Thu Jun 01 2017 Ignacio Hernandez <ignacio.hernandez@intel.com> - 1-1
- Initial revision

