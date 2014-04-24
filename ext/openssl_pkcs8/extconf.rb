require 'mkmf'

extension_name = 'openssl_pkcs8/openssl_pkcs8'

dir_config(extension_name)

find_header('openssl/ssl.h', 'C:\OpenSSL\include') unless have_header("openssl/ssl.h")
%w[crypto libeay32].any? do |lib|
	find_library(lib, 'OpenSSL_add_all_digests', 'C:\OpenSSL\lib') unless have_library(lib, "OpenSSL_add_all_digests")
end 
%w[ssl ssleay32].any? do |lib|
	find_library(lib, 'SSL_library_init', 'C:\OpenSSL\lib') unless have_library(lib, "SSL_library_init")
end 

#find_library('libmsvcrt-ruby191-static', 'ext/openssl/OSSL_Check_Kind', 'C:\RailsInstaller\Ruby1.9.3\lib') unless have_library('libruby', 'ext/openssl/OSSL_Check_Kind')
#find_library('libmsvcrt-ruby191-static', 'ext/openssl/ossl_raise', 'C:\RailsInstaller\Ruby1.9.3\lib') unless have_library('libruby', 'ext/openssl/ossl_raise')

create_makefile(extension_name)
