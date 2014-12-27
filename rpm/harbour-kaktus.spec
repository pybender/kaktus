# 
# Do NOT Edit the Auto-generated Part!
# Generated by: spectacle version 0.27
# 

Name:       harbour-kaktus

# >> macros
# << macros

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}
Summary:    Kaktus
Version:    1.2.3
Release:    0
Group:      Qt/Qt
License:    LICENSE
URL:        https://github.com/mkiol/kaktus
Source0:    %{name}-%{version}.tar.bz2
Source100:  harbour-kaktus.yaml
Requires:   sailfishsilica-qt5 >= 0.10.9
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(sailfishapp) >= 0.0.10
BuildRequires:  desktop-file-utils

%description
An unofficial Netvibes feed reader, specially designed to work offline.


%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
# << build pre

%qtc_qmake5 

%qtc_make %{?_smp_mflags}

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
# << install pre
%qmake5_install

# >> install post
# << install post

desktop-file-install --delete-original       \
  --dir %{buildroot}%{_datadir}/applications             \
   %{buildroot}%{_datadir}/applications/*.desktop

%files
%defattr(-,root,root,-)
%{_bindir}
%{_datadir}/%{name}/qml
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/86x86/apps/%{name}.png
/usr/bin
/usr/share/harbour-kaktus
/usr/share/applications
/usr/share/icons/hicolor/86x86/apps
# >> files
# << files

%changelog

* Sat Dec 27 2014 Michal Kosciesza 1.2.3-0
- Caching improvements
- Detecting and not displaying tiny images in offline mode
- BUG FIX: Handling Netvibes errors: 700 & 702

* Wed Nov 12 2014 Michal Kosciesza 1.2.2-3
- Small UI improvements for 1.1.0.38
- Czech and Dutch translation

* Tue Oct 21 2014 Michal Kosciesza 1.2.2-2
- Small UI improvements
- BUG FIX: Huge cache size take too long to calculate
- QtWebKit cache file removal on 'Delete cache' action
- Persian and Russian translation update

* Tue Oct 21 2014 Michal Kosciesza 1.2.2-1
- Russian language
- Updated Persian translation
- Better increase viewport implementation

* Tue Oct 21 2014 Michal Kosciesza 1.2.2-0
- Option to increase viewport in web viewer

* Sun Oct 12 2014 Michal Kosciesza 1.2.1-2
- BUG FIX: Auto mark as read does not work

* Fri Oct 10 2014 Michal Kosciesza 1.2.1-1
- UI improvements
- Language settings
- Farsi language (thanks to Ali Adineh)

* Sat Oct 03 2014 Michal Kosciesza 1.2.1-0
- UI improvements
- Handling for 'malformed database file' error

* Mon Sep 29 2014 Michal Kosciesza 1.2.0-5
- BUG FIX: Very old saved items not syncec correctly
- Image in offline web view

* Sat Sep 27 2014 Michal Kosciesza 1.2.0-5
- BUG FIX: Read all on Tabs does not work as should
- Image download improvement

* Fri Sep 26 2014 Michal Kosciesza 1.2.0-4
- New Netvibes APi support
- Bottom Bar
- Slow feeds view mode
- User Guide

* Sat Sep 06 2014 Michal Kosciesza 1.2.0-3
- Caching and downloading improvements

* Tue Sep 02 2014 Michal Kosciesza 1.2.0-2
- Netvibes Multi-Feed widget (initial) support
- Improved display of images

* Mon Sep 01 2014 Michal Kosciesza 1.2.0-1
- New "View Mode" options e.g. Option to show all feeds in one list
- Double-click marks article as read/unread
- Changelog page

* Wed Aug 27 2014 Michal Kosciesza 1.2.0-0
- Posibility to browse articles by Tabs (see "View Mode" option)
- Indicator for articles that have been added since last sync
- Possibility to delete cache data (see "Cache size" option)
- Many UI improvements

* Tue Aug 12 2014 Michal Kosciesza 1.1.0-5
- New Offline mode indicator
- Posibility to set Offline/Online mode in Pull-down menu

* Tue Jul 15 2014 Michal Kosciesza 1.1.0-4
- Polkit update

* Fri Jul 11 2014 Michal Kosciesza 1.1.0-3
- Thumbnails in articles list
- 'Mark as read' in Pull-down menu
- Offline mode indicator in Pull-down menu
- BUG FIX: Sync Failed on every first sync attempt

* Sat May 03 2014 Michal Kosciesza 1.0.5-0
- Sigin fix due to change in Netvibes API
- Qt4 support and initial Meego Harmattan support

* Sat May 03 2014 Michal Kosciesza 1.0.4-3
- Workaround for 'High Power Consumption' webkit bug

* Wed Apr 23 2014 Michal Kosciesza 1.0.4-2
- Cover improvements

* Sat Apr 19 2014 Michal Kosciesza 1.0.4-1
- Landscape mode
- 'Last sync' info on Cover
- Possibility to mark all articles in Tab as read/unread

* Mon Apr 14 2014 Michal Kosciesza 1.0.3-4
- Version for Openrepos.org

* Mon Apr 14 2014 Michal Kosciesza 1.0.3-3
- Showing number of unread articles on Cover
- Icon update

* Sun Apr 13 2014 Michal Kosciesza 1.0.3-2
- Showing number of unread articles on Tabs

* Sat Apr 12 2014 Michal Kosciesza 1.0.3-1
- 'Show only unread' option
- Possibility to mark all articles in Feed as read/unread
- Auto cache cleaning algorithm
- Cache size view in Settings
- UI improvements

* Thu Apr 03 2014 Michal Kosciesza 1.0.2-1
- Increased performance
- Better error handling when ne twork connection fails

* Sun Mar 30 2014 Michal Kosciesza 1.0.1-1
- UI improvements
- Extra Tab with Saved articles

* Mon Mar 25 2014 Michal Kosciesza 1.0.0-1
- First release
