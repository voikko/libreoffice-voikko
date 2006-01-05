Summary: Suomen kielen oikoluku ja tavutus OpenOffice.orgin versiolle 2.0
Name: oo2-soikko
Version: 0.4
Release: 1
License: See COPYING
Group: Applications/Text
Packager: Harri Pitkänen <hatapitk@cc.jyu.fi>
URL: http://www.lemi.fi/oo2-soikko/

%description
Pasi Ryhäsen Soikkoa käyttävä Suomen kielen oikoluku ja tavutus
OpenOffice.orgin versiolle 2.0.

%prep
# Nothing here

%build
# Nothing here

%install
# Nothing here

%clean
# Nothing here


%post
echo post $1 $2
/opt/oo2-soikko/register-oo2-soikko --register


%preun
echo preun $1 $2
/opt/oo2-soikko/register-oo2-soikko --unregister


%triggerin -- openoffice.org-core openofficeorg-core02
echo triggering $1 $2
/opt/oo2-soikko/register-oo2-soikko --register


%triggerun -- openoffice.org-core
echo triggerun-1 $1 $2
/opt/oo2-soikko/register-oo2-soikko --unregister-pkg openoffice.org-core || true


%triggerun -- openofficeorg-core02
echo triggerun-2 $1 $2
/opt/oo2-soikko/register-oo2-soikko --unregister-pkg openofficeorg-core02 || true


%files
/opt/oo2-soikko
%doc COPYING
%doc COPYING.gpl
%doc COPYING.lgpl
%doc COPYING.soikko

