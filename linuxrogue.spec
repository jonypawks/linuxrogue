Name: linuxrogue
Version: 0.3.6
Release: 1
Summary: Linux port of Rogue game

Group: Amusements/Games
License: BSD
URL: http://nwserver.ath.cx:8080/index.php?cat=rogue
#Source: http://nwserver.webhop.org/linuxrogue-%{version}.tar.gz
Source: %{name}-%{version}.tar.gz
#BuildRoot: %(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot

Requires: ncurses

Prefix: /usr/local/games

#Vendor: Greg Kennedy and Jesse Printz
#Packager: Greg Kennedy <kennedy.greg@gmail.com>
#Prefix: /usr/local
#BuildArch: i386

%description
Rogue is the original predecessor of Nethack, Moria, Angband, et al - hence the genre name "roguelikes". Originally written back in 1980, the source code has been lost and found several times and gone through a number of maintainers. Jesse Printz and Greg Kennedy are the current maintainers of the LinuxRogue project, which aims to bring this historic game to the Linux platform.  Goals of the project include portability, stability, and using modern libraries to update this game to today's coding standards without compromising the core gameplay.

%prep
%setup

%build
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%{prefix}/rogue
%doc

%changelog
* Tue Aug 7 2007 Greg Kennedy <kennedy.greg@gmail.com>
- First draft of the spec file
