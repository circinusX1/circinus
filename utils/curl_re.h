#ifndef LIBR_RESOLVER_H
#define LIBR_RESOLVER_H
// Copyright Marius C. https://github/comarius (do not remove)
#include <stdio.h>
#include <errno.h>
#include <dlfcn.h>


// -------------- constants ---------------
// review before compile...
#define __CURL_CURL_H
#define CURL_STRICTER
#define WIN32
#define CURL_SOCKET_BAD INVALID_SOCKET
#define CURL_SOCKET_BAD -1
#define curl_socket_typedef
#define CURLSSLBACKEND_LIBRESSL CURLSSLBACKEND_OPENSSL
#define CURLSSLBACKEND_BORINGSSL CURLSSLBACKEND_OPENSSL
#define CURLSSLBACKEND_CYASSL CURLSSLBACKEND_WOLFSSL
#define CURL_HTTPPOST_FILENAME (1<<0)
#define CURL_HTTPPOST_READFILE (1<<1)
#define CURL_HTTPPOST_PTRNAME (1<<2)
#define CURL_HTTPPOST_PTRCONTENTS (1<<3)
#define CURL_HTTPPOST_BUFFER (1<<4)
#define CURL_HTTPPOST_PTRBUFFER (1<<5)
#define CURL_HTTPPOST_CALLBACK (1<<6)
#define CURL_HTTPPOST_LARGE (1<<7)
#define CURL_MAX_READ_SIZE 524288
#define CURL_MAX_WRITE_SIZE 16384
#define CURL_MAX_HTTP_HEADER (100*1024)
#define CURL_WRITEFUNC_PAUSE 0x10000001
#define CURLFINFOFLAG_KNOWN_FILENAME    (1<<0)
#define CURLFINFOFLAG_KNOWN_FILETYPE    (1<<1)
#define CURLFINFOFLAG_KNOWN_TIME        (1<<2)
#define CURLFINFOFLAG_KNOWN_PERM        (1<<3)
#define CURLFINFOFLAG_KNOWN_UID         (1<<4)
#define CURLFINFOFLAG_KNOWN_GID         (1<<5)
#define CURLFINFOFLAG_KNOWN_SIZE        (1<<6)
#define CURLFINFOFLAG_KNOWN_HLINKCOUNT  (1<<7)
#define CURL_CHUNK_BGN_FUNC_OK      0
#define CURL_CHUNK_BGN_FUNC_FAIL    1 /* tell the lib to end the task */
#define CURL_CHUNK_BGN_FUNC_SKIP    2 /* skip this chunk over */
#define CURL_CHUNK_END_FUNC_OK      0
#define CURL_CHUNK_END_FUNC_FAIL    1 /* tell the lib to end the task */
#define CURL_FNMATCHFUNC_MATCH    0 /* string corresponds to the pattern */
#define CURL_FNMATCHFUNC_NOMATCH  1 /* pattern doesn't match the string */
#define CURL_FNMATCHFUNC_FAIL     2 /* an error occurred */
#define CURL_SEEKFUNC_OK       0
#define CURL_SEEKFUNC_FAIL     1 /* fail the entire transfer */
#define CURL_SEEKFUNC_CANTSEEK 2 /* tell libcurl seeking can't be done
 so
#define CURL_READFUNC_ABORT 0x10000000
#define CURL_READFUNC_PAUSE 0x10000001
#define CURL_SOCKOPT_OK 0
#define CURL_SOCKOPT_ERROR 1 /* causes libcurl to abort and return
#define CURL_SOCKOPT_ALREADY_CONNECTED 2
#define CURL_DID_MEMORY_FUNC_TYPEDEFS
#define CURLE_OBSOLETE16 CURLE_HTTP2
#define CURLE_OBSOLETE10 CURLE_FTP_ACCEPT_FAILED
#define CURLE_OBSOLETE12 CURLE_FTP_ACCEPT_TIMEOUT
#define CURLOPT_ENCODING CURLOPT_ACCEPT_ENCODING
#define CURLE_FTP_WEIRD_SERVER_REPLY CURLE_WEIRD_SERVER_REPLY
#define CURLE_UNKNOWN_TELNET_OPTION CURLE_UNKNOWN_OPTION
#define CURLE_SSL_PEER_CERTIFICATE CURLE_PEER_FAILED_VERIFICATION
#define CURLE_OBSOLETE CURLE_OBSOLETE50 /* no one should be using this! */
#define CURLE_BAD_PASSWORD_ENTERED CURLE_OBSOLETE46
#define CURLE_BAD_CALLING_ORDER CURLE_OBSOLETE44
#define CURLE_FTP_USER_PASSWORD_INCORRECT CURLE_OBSOLETE10
#define CURLE_FTP_CANT_RECONNECT CURLE_OBSOLETE16
#define CURLE_FTP_COULDNT_GET_SIZE CURLE_OBSOLETE32
#define CURLE_FTP_COULDNT_SET_ASCII CURLE_OBSOLETE29
#define CURLE_FTP_WEIRD_USER_REPLY CURLE_OBSOLETE12
#define CURLE_FTP_WRITE_ERROR CURLE_OBSOLETE20
#define CURLE_LIBRARY_NOT_FOUND CURLE_OBSOLETE40
#define CURLE_MALFORMAT_USER CURLE_OBSOLETE24
#define CURLE_SHARE_IN_USE CURLE_OBSOLETE57
#define CURLE_URL_MALFORMAT_USER CURLE_NOT_BUILT_IN
#define CURLE_FTP_ACCESS_DENIED CURLE_REMOTE_ACCESS_DENIED
#define CURLE_FTP_COULDNT_SET_BINARY CURLE_FTP_COULDNT_SET_TYPE
#define CURLE_FTP_QUOTE_ERROR CURLE_QUOTE_ERROR
#define CURLE_TFTP_DISKFULL CURLE_REMOTE_DISK_FULL
#define CURLE_TFTP_EXISTS CURLE_REMOTE_FILE_EXISTS
#define CURLE_HTTP_RANGE_ERROR CURLE_RANGE_ERROR
#define CURLE_FTP_SSL_FAILED CURLE_USE_SSL_FAILED
#define CURLE_OPERATION_TIMEOUTED CURLE_OPERATION_TIMEDOUT
#define CURLE_HTTP_NOT_FOUND CURLE_HTTP_RETURNED_ERROR
#define CURLE_HTTP_PORT_FAILED CURLE_INTERFACE_FAILED
#define CURLE_FTP_COULDNT_STOR_FILE CURLE_UPLOAD_FAILED
#define CURLE_FTP_PARTIAL_FILE CURLE_PARTIAL_FILE
#define CURLE_FTP_BAD_DOWNLOAD_RESUME CURLE_BAD_DOWNLOAD_RESUME
   is no longer used by libcurl but is instead #defined here only to not
#define CURLE_ALREADY_COMPLETE 99999
#define CURLOPT_FILE CURLOPT_WRITEDATA /* name changed in 7.9.7 */
#define CURLOPT_INFILE CURLOPT_READDATA /* name changed in 7.9.7 */
#define CURLOPT_WRITEHEADER CURLOPT_HEADERDATA
#define CURLOPT_WRITEINFO CURLOPT_OBSOLETE40
#define CURLOPT_CLOSEPOLICY CURLOPT_OBSOLETE72
#define CURLAUTH_NONE         ((unsigned long)0)
#define CURLAUTH_BASIC        (((unsigned long)1)<<0)
#define CURLAUTH_DIGEST       (((unsigned long)1)<<1)
#define CURLAUTH_NEGOTIATE    (((unsigned long)1)<<2)
#define CURLAUTH_GSSNEGOTIATE CURLAUTH_NEGOTIATE
#define CURLAUTH_GSSAPI CURLAUTH_NEGOTIATE
#define CURLAUTH_NTLM         (((unsigned long)1)<<3)
#define CURLAUTH_DIGEST_IE    (((unsigned long)1)<<4)
#define CURLAUTH_NTLM_WB      (((unsigned long)1)<<5)
#define CURLAUTH_ONLY         (((unsigned long)1)<<31)
#define CURLAUTH_ANY          (~CURLAUTH_DIGEST_IE)
#define CURLAUTH_ANYSAFE      (~(CURLAUTH_BASIC|CURLAUTH_DIGEST_IE))
#define CURLSSH_AUTH_ANY       ~0     /* all types supported by the server */
#define CURLSSH_AUTH_NONE      0      /* none allowed
 silly but complete */
#define CURLSSH_AUTH_PUBLICKEY (1<<0) /* public/private key files */
#define CURLSSH_AUTH_PASSWORD  (1<<1) /* password */
#define CURLSSH_AUTH_HOST      (1<<2) /* host key files */
#define CURLSSH_AUTH_KEYBOARD  (1<<3) /* keyboard interactive */
#define CURLSSH_AUTH_AGENT     (1<<4) /* agent (ssh-agent
 pageant...) */
#define CURLSSH_AUTH_GSSAPI    (1<<5) /* gssapi (kerberos
 ...) */
#define CURLSSH_AUTH_DEFAULT CURLSSH_AUTH_ANY
#define CURLGSSAPI_DELEGATION_NONE        0      /* no delegation (default) */
#define CURLGSSAPI_DELEGATION_POLICY_FLAG (1<<0) /* if permitted by policy */
#define CURLGSSAPI_DELEGATION_FLAG        (1<<1) /* delegate always */
#define CURL_ERROR_SIZE 256
#define CURLSSLOPT_ALLOW_BEAST (1<<0)
#define CURLSSLOPT_NO_REVOKE (1<<1)
#define CURLFTPSSL_NONE CURLUSESSL_NONE
#define CURLFTPSSL_TRY CURLUSESSL_TRY
#define CURLFTPSSL_CONTROL CURLUSESSL_CONTROL
#define CURLFTPSSL_ALL CURLUSESSL_ALL
#define CURLFTPSSL_LAST CURLUSESSL_LAST
#define curl_ftpssl curl_usessl
#define CURLHEADER_UNIFIED  0
#define CURLHEADER_SEPARATE (1<<0)
#define CURLPROTO_HTTP   (1<<0)
#define CURLPROTO_HTTPS  (1<<1)
#define CURLPROTO_FTP    (1<<2)
#define CURLPROTO_FTPS   (1<<3)
#define CURLPROTO_SCP    (1<<4)
#define CURLPROTO_SFTP   (1<<5)
#define CURLPROTO_TELNET (1<<6)
#define CURLPROTO_LDAP   (1<<7)
#define CURLPROTO_LDAPS  (1<<8)
#define CURLPROTO_DICT   (1<<9)
#define CURLPROTO_FILE   (1<<10)
#define CURLPROTO_TFTP   (1<<11)
#define CURLPROTO_IMAP   (1<<12)
#define CURLPROTO_IMAPS  (1<<13)
#define CURLPROTO_POP3   (1<<14)
#define CURLPROTO_POP3S  (1<<15)
#define CURLPROTO_SMTP   (1<<16)
#define CURLPROTO_SMTPS  (1<<17)
#define CURLPROTO_RTSP   (1<<18)
#define CURLPROTO_RTMP   (1<<19)
#define CURLPROTO_RTMPT  (1<<20)
#define CURLPROTO_RTMPE  (1<<21)
#define CURLPROTO_RTMPTE (1<<22)
#define CURLPROTO_RTMPS  (1<<23)
#define CURLPROTO_RTMPTS (1<<24)
#define CURLPROTO_GOPHER (1<<25)
#define CURLPROTO_SMB    (1<<26)
#define CURLPROTO_SMBS   (1<<27)
#define CURLPROTO_ALL    (~0) /* enable everything */
#define CURLOPTTYPE_LONG          0
#define CURLOPTTYPE_OBJECTPOINT   10000
#define CURLOPTTYPE_STRINGPOINT   10000
#define CURLOPTTYPE_FUNCTIONPOINT 20000
#define CURLOPTTYPE_OFF_T         30000
#define CINIT(na
t
nu) CURLOPT_ ## na = CURLOPTTYPE_ ## t + nu
#define LONG          CURLOPTTYPE_LONG
#define OBJECTPOINT   CURLOPTTYPE_OBJECTPOINT
#define STRINGPOINT   CURLOPTTYPE_OBJECTPOINT
#define FUNCTIONPOINT CURLOPTTYPE_FUNCTIONPOINT
#define OFF_T         CURLOPTTYPE_OFF_T
#define CINIT(name
type
number) CURLOPT_/**/name = type + number
#define CURLOPT_XFERINFODATA CURLOPT_PROGRESSDATA
#define CURLOPT_SERVER_RESPONSE_TIMEOUT CURLOPT_FTP_RESPONSE_TIMEOUT
#define CURLOPT_POST301 CURLOPT_POSTREDIR
#define CURLOPT_SSLKEYPASSWD CURLOPT_KEYPASSWD
#define CURLOPT_FTPAPPEND CURLOPT_APPEND
#define CURLOPT_FTPLISTONLY CURLOPT_DIRLISTONLY
#define CURLOPT_FTP_SSL CURLOPT_USE_SSL
#define CURLOPT_SSLCERTPASSWD CURLOPT_KEYPASSWD
#define CURLOPT_KRB4LEVEL CURLOPT_KRBLEVEL
#define CURL_IPRESOLVE_WHATEVER 0 /* default
 resolves addresses to all IP
#define CURL_IPRESOLVE_V4       1 /* resolve to IPv4 addresses */
#define CURL_IPRESOLVE_V6       2 /* resolve to IPv6 addresses */
#define CURLOPT_RTSPHEADER CURLOPT_HTTPHEADER
#define CURL_HTTP_VERSION_2 CURL_HTTP_VERSION_2_0
#define CURL_REDIR_GET_ALL  0
#define CURL_REDIR_POST_301 1
#define CURL_REDIR_POST_302 2
#define CURL_REDIR_POST_303 4
#define CURL_REDIR_POST_ALL \
#define CURL_ZERO_TERMINATED ((size_t) -1)
#define CFINIT(name) CURLFORM_ ## name
#define CFINIT(name) CURLFORM_/**/name
#define CURLINFO_STRING   0x100000
#define CURLINFO_LONG     0x200000
#define CURLINFO_DOUBLE   0x300000
#define CURLINFO_SLIST    0x400000
#define CURLINFO_PTR      0x400000 /* same as SLIST */
#define CURLINFO_SOCKET   0x500000
#define CURLINFO_OFF_T    0x600000
#define CURLINFO_MASK     0x0fffff
#define CURLINFO_TYPEMASK 0xf00000
#define CURLINFO_HTTP_CODE CURLINFO_RESPONSE_CODE
#define CURL_GLOBAL_SSL (1<<0) /* no purpose since since 7.57.0 */
#define CURL_GLOBAL_WIN32 (1<<1)
#define CURL_GLOBAL_ALL (CURL_GLOBAL_SSL|CURL_GLOBAL_WIN32)
#define CURL_GLOBAL_NOTHING 0
#define CURL_GLOBAL_DEFAULT CURL_GLOBAL_ALL
#define CURL_GLOBAL_ACK_EINTR (1<<2)
#define CURLVERSION_NOW CURLVERSION_FIFTH
#define CURL_VERSION_IPV6         (1<<0)  /* IPv6-enabled */
#define CURL_VERSION_KERBEROS4    (1<<1)  /* Kerberos V4 auth is supported
#define CURL_VERSION_SSL          (1<<2)  /* SSL options are present */
#define CURL_VERSION_LIBZ         (1<<3)  /* libz features are present */
#define CURL_VERSION_NTLM         (1<<4)  /* NTLM auth is supported */
#define CURL_VERSION_GSSNEGOTIATE (1<<5)  /* Negotiate auth is supported
#define CURL_VERSION_DEBUG        (1<<6)  /* Built with debug capabilities */
#define CURL_VERSION_ASYNCHDNS    (1<<7)  /* Asynchronous DNS resolves */
#define CURL_VERSION_SPNEGO       (1<<8)  /* SPNEGO auth is supported */
#define CURL_VERSION_LARGEFILE    (1<<9)  /* Supports files larger than 2GB */
#define CURL_VERSION_IDN          (1<<10) /* Internationized Domain Names are
#define CURL_VERSION_SSPI         (1<<11) /* Built against Windows SSPI */
#define CURL_VERSION_CONV         (1<<12) /* Character conversions supported */
#define CURL_VERSION_CURLDEBUG    (1<<13) /* Debug memory tracking supported */
#define CURL_VERSION_TLSAUTH_SRP  (1<<14) /* TLS-SRP auth is supported */
#define CURL_VERSION_NTLM_WB      (1<<15) /* NTLM delegation to winbind helper
#define CURL_VERSION_HTTP2        (1<<16) /* HTTP2 support built-in */
#define CURL_VERSION_GSSAPI       (1<<17) /* Built against a GSS-API library */
#define CURL_VERSION_KERBEROS5    (1<<18) /* Kerberos V5 auth is supported */
#define CURL_VERSION_UNIX_SOCKETS (1<<19) /* Unix domain sockets support */
#define CURL_VERSION_PSL          (1<<20) /* Mozilla's Public Suffix List
 used
#define CURL_VERSION_HTTPS_PROXY  (1<<21) /* HTTPS-proxy support built-in */
#define CURL_VERSION_MULTI_SSL    (1<<22) /* Multiple SSL backends available */
#define CURL_VERSION_BROTLI       (1<<23) /* Brotli features are present. */
#define CURLPAUSE_RECV      (1<<0)
#define CURLPAUSE_RECV_CONT (0)
#define CURLPAUSE_SEND      (1<<2)
#define CURLPAUSE_SEND_CONT (0)
#define CURLPAUSE_ALL       (CURLPAUSE_RECV|CURLPAUSE_SEND)
#define CURLPAUSE_CONT      (CURLPAUSE_RECV_CONT|CURLPAUSE_SEND_CONT)
#define curl_easy_setopt(handle
opt
param) curl_easy_setopt(handle
opt
param)
#define curl_easy_getinfo(handle
info
arg) curl_easy_getinfo(handle
info
arg)
#define curl_share_setopt(share
opt
param) curl_share_setopt(share
opt
param)
#define curl_multi_setopt(handle
opt
param) curl_multi_setopt(handle
opt
param)


// -------------- functions ---------------
// review before compile...
//         /* Callback function for closing socket (instead of close(2)). The callback
#define PF_Callback (* (/* (*)(instead))                                           _ptr[0].ptr)
//         /* Callback function for opening socket (instead of socket(2)). Optionally,
#define PF_Callback (* (/* (*)(instead))                                           _ptr[1].ptr)
//         wait till it's ready and try again (Added
#define PF_till (* (wait (*)(Added))                                               _ptr[2].ptr)
//         /* Function that will be called to read the input (instead of fread). The
#define PF_Function (* (/* (*)(instead))                                           _ptr[3].ptr)
//         * parameters will use fread() syntax, make sure to follow them. */
#define PF_parameters (* (* (*)(,make))                                            _ptr[4].ptr)
//         /* Function that will be called to store the output (instead of fwrite). The
#define PF_Function (* (/* (*)(instead))                                           _ptr[5].ptr)
//         * parameters will use fwrite() syntax, make sure to follow them. */
#define PF_parameters (* (* (*)(,make))                                            _ptr[6].ptr)
//         CINIT(APPEND, LONG, 50),       /* Append instead of overwrite on upload! */
#define PF_ (* (CINIT (*)(APPEND,LONG,50,))                                        _ptr[7].ptr)
//         /* Function that will be called to store headers (instead of fwrite). The
#define PF_Function (* (/* (*)(instead))                                           _ptr[8].ptr)
//         * parameters will use fwrite() syntax, make sure to follow them. */
#define PF_parameters (* (* (*)(,make))                                            _ptr[9].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[10].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[11].ptr)
//         CURL_EXTERN time_t curl_getdate(const char *p, const time_t *unused);
#define PF_curl_getdate (* (CURL_EXTERN time_t (*)(const,const))                   _ptr[12].ptr)
//         * NAME curl_mime_name()
#define PF_curl_mime_name (* (* NAME (*)())                                        _ptr[13].ptr)
//         CURL_EXTERN CURLcode curl_mime_name(curl_mimepart *part, const char *name);
#define PF_curl_mime_name (* (CURL_EXTERN CURLcode (*)(curl_mimepart,const))       _ptr[14].ptr)
//         /* Returns code for curl_formadd()
#define PF_Returns (* (/* (*)())                                                   _ptr[15].ptr)
//         * NAME curl_formadd()
#define PF_curl_formadd (* (* NAME (*)())                                          _ptr[16].ptr)
//         CURL_EXTERN CURLFORMcode curl_formadd(struct curl_httppost **httppost,
#define PF_curl_formadd (* (CURL_EXTERN CURLFORMcode (*)(struct,))                 _ptr[17].ptr)
//         * Serialize a curl_httppost struct built with curl_formadd().
#define PF_Serialize (* (* (*)(.))                                                 _ptr[18].ptr)
//         * Free a multipart formpost previously built with curl_formadd().
#define PF_Free (* (* (*)(.))                                                      _ptr[19].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[20].ptr)
//         * NAME curl_formfree()
#define PF_curl_formfree (* (* NAME (*)())                                         _ptr[21].ptr)
//         CURL_EXTERN void curl_formfree(struct curl_httppost *form);
#define PF_curl_formfree (* (CURL_EXTERN void (*)(struct))                         _ptr[22].ptr)
//         * NAME curl_mime_init()
#define PF_curl_mime_init (* (* NAME (*)())                                        _ptr[23].ptr)
//         CURL_EXTERN curl_mime *curl_mime_init(CURL *easy);
#define PF_*curl_mime_init (* (CURL_EXTERN curl_mime (*)(CURL))                    _ptr[24].ptr)
//         * NAME curl_mime_subparts()
#define PF_curl_mime_subparts (* (* NAME (*)())                                    _ptr[25].ptr)
//         CURL_EXTERN CURLcode curl_mime_subparts(curl_mimepart *part,
#define PF_curl_mime_subparts (* (CURL_EXTERN CURLcode (*)(curl_mimepart,))        _ptr[26].ptr)
//         * NAME curl_mime_addpart()
#define PF_curl_mime_addpart (* (* NAME (*)())                                     _ptr[27].ptr)
//         CURL_EXTERN curl_mimepart *curl_mime_addpart(curl_mime *mime);
#define PF_*curl_mime_addpart (* (CURL_EXTERN curl_mimepart (*)(curl_mime))        _ptr[28].ptr)
//         * NAME curl_mime_headers()
#define PF_curl_mime_headers (* (* NAME (*)())                                     _ptr[29].ptr)
//         CURL_EXTERN CURLcode curl_mime_headers(curl_mimepart *part,
#define PF_curl_mime_headers (* (CURL_EXTERN CURLcode (*)(curl_mimepart,))         _ptr[30].ptr)
//         * NAME curl_mime_type()
#define PF_curl_mime_type (* (* NAME (*)())                                        _ptr[31].ptr)
//         CURL_EXTERN CURLcode curl_mime_type(curl_mimepart *part, const char *mimetype);
#define PF_curl_mime_type (* (CURL_EXTERN CURLcode (*)(curl_mimepart,const))       _ptr[32].ptr)
//         /* Function that will be called to read the input (instead of fread). The
#define PF_Function (* (/* (*)(instead))                                           _ptr[33].ptr)
//         * parameters will use fread() syntax, make sure to follow them. */
#define PF_parameters (* (* (*)(,make))                                            _ptr[34].ptr)
//         * NAME curl_mime_data_cb()
#define PF_curl_mime_data_cb (* (* NAME (*)())                                     _ptr[35].ptr)
//         CURL_EXTERN CURLcode curl_mime_data_cb(curl_mimepart *part,
#define PF_curl_mime_data_cb (* (CURL_EXTERN CURLcode (*)(curl_mimepart,))         _ptr[36].ptr)
//         * NAME curl_mime_filename()
#define PF_curl_mime_filename (* (* NAME (*)())                                    _ptr[37].ptr)
//         CURL_EXTERN CURLcode curl_mime_filename(curl_mimepart *part,
#define PF_curl_mime_filename (* (CURL_EXTERN CURLcode (*)(curl_mimepart,))        _ptr[38].ptr)
//         * NAME curl_mime_data()
#define PF_curl_mime_data (* (* NAME (*)())                                        _ptr[39].ptr)
//         CURL_EXTERN CURLcode curl_mime_data(curl_mimepart *part,
#define PF_curl_mime_data (* (CURL_EXTERN CURLcode (*)(curl_mimepart,))            _ptr[40].ptr)
//         * NAME curl_mime_data_cb()
#define PF_curl_mime_data_cb (* (* NAME (*)())                                     _ptr[41].ptr)
//         * NAME curl_mime_filedata()
#define PF_curl_mime_filedata (* (* NAME (*)())                                    _ptr[42].ptr)
//         CURL_EXTERN CURLcode curl_mime_filedata(curl_mimepart *part,
#define PF_curl_mime_filedata (* (CURL_EXTERN CURLcode (*)(curl_mimepart,))        _ptr[43].ptr)
//         * callback function for curl_formget()
#define PF_callback (* (* (*)())                                                   _ptr[44].ptr)
//         *   curl_formget().
#define PF_curl_formget (* (* (*)(.))                                              _ptr[45].ptr)
//         * NAME curl_formget()
#define PF_curl_formget (* (* NAME (*)())                                          _ptr[46].ptr)
//         CURL_EXTERN int curl_formget(struct curl_httppost *form, void *arg,
#define PF_curl_formget (* (CURL_EXTERN int (*)(struct,void,))                     _ptr[47].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[48].ptr)
//         * NAME curl_getdate()
#define PF_curl_getdate (* (* NAME (*)())                                          _ptr[49].ptr)
//         /* curl_strequal() and curl_strnequal() are subject for removal in a future
#define PF_curl_strequal (* (/* (*)())                                             _ptr[50].ptr)
//         CURL_EXTERN int curl_strnequal(const char *s1, const char *s2, size_t n);
#define PF_curl_strnequal (* (CURL_EXTERN int (*)(const,const,size_t))             _ptr[51].ptr)
//         * NAME curl_slist_free_all()
#define PF_curl_slist_free_all (* (* NAME (*)())                                   _ptr[52].ptr)
//         CURL_EXTERN void curl_slist_free_all(struct curl_slist *);
#define PF_curl_slist_free_all (* (CURL_EXTERN void (*)(struct))                   _ptr[53].ptr)
//         operation. Set file name to "-" (dash) to make it go to stdout. */
#define PF_Set (* (operation. (*)(dash))                                           _ptr[54].ptr)
//         /* Function that will be called to store the output (instead of fwrite). The
#define PF_Function (* (/* (*)(instead))                                           _ptr[55].ptr)
//         * parameters will use fwrite() syntax, make sure to follow them. */
#define PF_parameters (* (* (*)(,make))                                            _ptr[56].ptr)
//         /* Function that will be called to store headers (instead of fwrite). The
#define PF_Function (* (/* (*)(instead))                                           _ptr[57].ptr)
//         * parameters will use fwrite() syntax, make sure to follow them. */
#define PF_parameters (* (* (*)(,make))                                            _ptr[58].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[59].ptr)
//         * NAME curl_mime_headers()
#define PF_curl_mime_headers (* (* NAME (*)())                                     _ptr[60].ptr)
//         * NAME curl_getdate()
#define PF_curl_getdate (* (* NAME (*)())                                          _ptr[61].ptr)
//         /* Function that will be called to store the output (instead of fwrite). The
#define PF_Function (* (/* (*)(instead))                                           _ptr[62].ptr)
//         * parameters will use fwrite() syntax, make sure to follow them. */
#define PF_parameters (* (* (*)(,make))                                            _ptr[63].ptr)
//         /* Function that will be called to store headers (instead of fwrite). The
#define PF_Function (* (/* (*)(instead))                                           _ptr[64].ptr)
//         * parameters will use fwrite() syntax, make sure to follow them. */
#define PF_parameters (* (* (*)(,make))                                            _ptr[65].ptr)
//         (unrelated to network send operations). */
#define PF_ (* ( (*)(unrelated))                                                   _ptr[66].ptr)
//         /* Continue to send authentication (user+password) when following locations,
#define PF_Continue (* (/* (*)(user+password,))                                    _ptr[67].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[68].ptr)
//         * NAME curl_easy_strerror()
#define PF_curl_easy_strerror (* (* NAME (*)())                                    _ptr[69].ptr)
//         CURL_EXTERN const char *curl_easy_strerror(CURLcode);
#define PF_const (* (CURL_EXTERN (*)(CURLcode))                                    _ptr[70].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[71].ptr)
//         #define CURL_VERSION_NTLM_WB      (1<<15) /* NTLM delegation to winbind helper
#define PF_CURL_VERSION_NTLM_WB (* (#define (*)(1<<15))                            _ptr[72].ptr)
//         CURLSOCKTYPE_ACCEPT, /* socket created by accept() call */
#define PF_socket (* (CURLSOCKTYPE_ACCEPT,/* (*)())                                _ptr[73].ptr)
//         /* indicates type of proxy. accepted values are CURLPROXY_HTTP (default),
#define PF_indicates (* (/* (*)(default,))                                         _ptr[74].ptr)
//         /* Time-out accept operations (currently for FTP only) after this amount
#define PF_Time-out (* (/* (*)(currently))                                         _ptr[75].ptr)
//         * NAME curl_getdate()
#define PF_curl_getdate (* (* NAME (*)())                                          _ptr[76].ptr)
//         operation. Set file name to "-" (dash) to make it go to stdout. */
#define PF_Set (* (operation. (*)(dash))                                           _ptr[77].ptr)
//         /* Function that will be called to store the output (instead of fwrite). The
#define PF_Function (* (/* (*)(instead))                                           _ptr[78].ptr)
//         * parameters will use fwrite() syntax, make sure to follow them. */
#define PF_parameters (* (* (*)(,make))                                            _ptr[79].ptr)
//         /* Function that will be called to store headers (instead of fwrite). The
#define PF_Function (* (/* (*)(instead))                                           _ptr[80].ptr)
//         * parameters will use fwrite() syntax, make sure to follow them. */
#define PF_parameters (* (* (*)(,make))                                            _ptr[81].ptr)
//         /* Function that will be called to read the input (instead of fread). The
#define PF_Function (* (/* (*)(instead))                                           _ptr[82].ptr)
//         * parameters will use fread() syntax, make sure to follow them. */
#define PF_parameters (* (* (*)(,make))                                            _ptr[83].ptr)
//         * NAME curl_easy_unescape()
#define PF_curl_easy_unescape (* (* NAME (*)())                                    _ptr[84].ptr)
//         CURL_EXTERN char *curl_easy_unescape(CURL *handle,
#define PF_*curl_easy_unescape (* (CURL_EXTERN char (*)(CURL,))                    _ptr[85].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[86].ptr)
//         * NAME curl_getenv()
#define PF_curl_getenv (* (* NAME (*)())                                           _ptr[87].ptr)
//         CURL_EXTERN char *curl_getenv(const char *variable);
#define PF_*curl_getenv (* (CURL_EXTERN char (*)(const))                           _ptr[88].ptr)
//         * NAME curl_easy_strerror()
#define PF_curl_easy_strerror (* (* NAME (*)())                                    _ptr[89].ptr)
//         * NAME curl_share_strerror()
#define PF_curl_share_strerror (* (* NAME (*)())                                   _ptr[90].ptr)
//         CURL_EXTERN const char *curl_share_strerror(CURLSHcode);
#define PF_const (* (CURL_EXTERN (*)(CURLSHcode))                                  _ptr[91].ptr)
//         * NAME curl_version()
#define PF_curl_version (* (* NAME (*)())                                          _ptr[92].ptr)
//         CURL_EXTERN char *curl_version(void);
#define PF_*curl_version (* (CURL_EXTERN char (*)(void))                           _ptr[93].ptr)
//         * NAME curl_version_info()
#define PF_curl_version_info (* (* NAME (*)())                                     _ptr[94].ptr)
//         CURL_EXTERN curl_version_info_data *curl_version_info(CURLversion);
#define PF_*curl_version_info (* (CURL_EXTERN curl_version_info_data (*)(CURLversion))    _ptr[95].ptr)
//         * NAME curl_version_info()
#define PF_curl_version_info (* (* NAME (*)())                                     _ptr[96].ptr)
//         * NAME curl_getenv()
#define PF_curl_getenv (* (* NAME (*)())                                           _ptr[97].ptr)
//         * NAME curl_getenv()
#define PF_curl_getenv (* (* NAME (*)())                                           _ptr[98].ptr)
//         * NAME curl_easy_escape()
#define PF_curl_easy_escape (* (* NAME (*)())                                      _ptr[99].ptr)
//         CURL_EXTERN char *curl_easy_escape(CURL *handle,
#define PF_*curl_easy_escape (* (CURL_EXTERN char (*)(CURL,))                      _ptr[100].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[101].ptr)
//         CURL_EXTERN char *curl_escape(const char *string,
#define PF_*curl_escape (* (CURL_EXTERN char (*)(const,))                          _ptr[102].ptr)
//         * NAME curl_easy_unescape()
#define PF_curl_easy_unescape (* (* NAME (*)())                                    _ptr[103].ptr)
//         CURL_EXTERN char *curl_unescape(const char *string,
#define PF_*curl_unescape (* (CURL_EXTERN char (*)(const,))                        _ptr[104].ptr)
//         * Returns a malloc()'ed string that MUST be curl_free()ed after usage is
#define PF_Returns (* (* (*)('ed))                                                 _ptr[105].ptr)
//         * NAME curl_free()
#define PF_curl_free (* (* NAME (*)())                                             _ptr[106].ptr)
//         CURL_EXTERN void curl_free(void *p);
#define PF_curl_free (* (CURL_EXTERN void (*)(void))                               _ptr[107].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[108].ptr)
//         operation. Set file name to "-" (dash) to make it go to stdout. */
#define PF_Set (* (operation. (*)(dash))                                           _ptr[109].ptr)
//         * NAME curl_slist_free_all()
#define PF_curl_slist_free_all (* (* NAME (*)())                                   _ptr[110].ptr)
//         * NAME curl_slist_append()
#define PF_curl_slist_append (* (* NAME (*)())                                     _ptr[111].ptr)
//         CURL_EXTERN struct curl_slist *curl_slist_append(struct curl_slist *,
#define PF_struct (* (CURL_EXTERN (*)(struct,))                                    _ptr[112].ptr)
//         (unrelated to network send operations). */
#define PF_ (* ( (*)(unrelated))                                                   _ptr[113].ptr)
//         /* Continue to send authentication (user+password) when following locations,
#define PF_Continue (* (/* (*)(user+password,))                                    _ptr[114].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[115].ptr)
//         wait till it's ready and try again (Added
#define PF_till (* (wait (*)(Added))                                               _ptr[116].ptr)
//         /* Function that will be called to read the input (instead of fread). The
#define PF_Function (* (/* (*)(instead))                                           _ptr[117].ptr)
//         * parameters will use fread() syntax, make sure to follow them. */
#define PF_parameters (* (* (*)(,make))                                            _ptr[118].ptr)
//         * NAME curl_getenv()
#define PF_curl_getenv (* (* NAME (*)())                                           _ptr[119].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[120].ptr)
//         /* Function that will be called to read the input (instead of fread). The
#define PF_Function (* (/* (*)(instead))                                           _ptr[121].ptr)
//         * parameters will use fread() syntax, make sure to follow them. */
#define PF_parameters (* (* (*)(,make))                                            _ptr[122].ptr)
//         CURL_EXTERN int curl_strequal(const char *s1, const char *s2);
#define PF_curl_strequal (* (CURL_EXTERN int (*)(const,const))                     _ptr[123].ptr)
//         * Returns a malloc()'ed string that MUST be curl_free()ed after usage is
#define PF_Returns (* (* (*)('ed))                                                 _ptr[124].ptr)
//         * NAME curl_mime_free()
#define PF_curl_mime_free (* (* NAME (*)())                                        _ptr[125].ptr)
//         CURL_EXTERN void curl_mime_free(curl_mime *mime);
#define PF_curl_mime_free (* (CURL_EXTERN void (*)(curl_mime))                     _ptr[126].ptr)
//         * NAME curl_formfree()
#define PF_curl_formfree (* (* NAME (*)())                                         _ptr[127].ptr)
//         * Returns a malloc()'ed string that MUST be curl_free()ed after usage is
#define PF_Returns (* (* (*)('ed))                                                 _ptr[128].ptr)
//         * NAME curl_free()
#define PF_curl_free (* (* NAME (*)())                                             _ptr[129].ptr)
//         * NAME curl_slist_free_all()
#define PF_curl_slist_free_all (* (* NAME (*)())                                   _ptr[130].ptr)
//         * curl_global_init_mem() function to set user defined memory management
#define PF_curl_global_init_mem (* (* (*)())                                       _ptr[131].ptr)
//         * NAME curl_global_init()
#define PF_curl_global_init (* (* NAME (*)())                                      _ptr[132].ptr)
//         * curl_global_init() should be invoked exactly once for each application that
#define PF_curl_global_init (* (* (*)())                                           _ptr[133].ptr)
//         CURL_EXTERN CURLcode curl_global_init(long flags);
#define PF_curl_global_init (* (CURL_EXTERN CURLcode (*)(long))                    _ptr[134].ptr)
//         * NAME curl_global_init_mem()
#define PF_curl_global_init_mem (* (* NAME (*)())                                  _ptr[135].ptr)
//         * curl_global_init() or curl_global_init_mem() should be invoked exactly once
#define PF_curl_global_init (* (* (*)())                                           _ptr[136].ptr)
//         CURL_EXTERN CURLcode curl_global_init_mem(long flags,
#define PF_curl_global_init_mem (* (CURL_EXTERN CURLcode (*)(long,))               _ptr[137].ptr)
//         * *before* curl_global_init().
#define PF_curl_global_init (* (* *before* (*)(.))                                 _ptr[138].ptr)
//         * curl_global_init_mem() function to set user defined memory management
#define PF_curl_global_init_mem (* (* (*)())                                       _ptr[139].ptr)
//         * NAME curl_global_init_mem()
#define PF_curl_global_init_mem (* (* NAME (*)())                                  _ptr[140].ptr)
//         * curl_global_init() or curl_global_init_mem() should be invoked exactly once
#define PF_curl_global_init (* (* (*)())                                           _ptr[141].ptr)
//         * NAME curl_global_cleanup()
#define PF_curl_global_cleanup (* (* NAME (*)())                                   _ptr[142].ptr)
//         * curl_global_cleanup() should be invoked exactly once for each application
#define PF_curl_global_cleanup (* (* (*)())                                        _ptr[143].ptr)
//         CURL_EXTERN void curl_global_cleanup(void);
#define PF_curl_global_cleanup (* (CURL_EXTERN void (*)(void))                     _ptr[144].ptr)
//         #define curl_multi_setopt(handle,opt,param) curl_multi_setopt(handle,opt,param)
#define PF_curl_multi_setopt (* (#define (*)(handle,opt,param))                    _ptr[145].ptr)
//         #define curl_easy_getinfo(handle,info,arg) curl_easy_getinfo(handle,info,arg)
#define PF_curl_easy_getinfo (* (#define (*)(handle,info,arg))                     _ptr[146].ptr)
//         * NAME curl_slist_free_all()
#define PF_curl_slist_free_all (* (* NAME (*)())                                   _ptr[147].ptr)
//         * NAME curl_easy_pause()
#define PF_curl_easy_pause (* (* NAME (*)())                                       _ptr[148].ptr)
//         CURL_EXTERN CURLcode curl_easy_pause(CURL *handle, int bitmask);
#define PF_curl_easy_pause (* (CURL_EXTERN CURLcode (*)(CURL,int))                 _ptr[149].ptr)
//         wait till it's ready and try again (Added
#define PF_till (* (wait (*)(Added))                                               _ptr[150].ptr)
//         /* Function that will be called to read the input (instead of fread). The
#define PF_Function (* (/* (*)(instead))                                           _ptr[151].ptr)
//         * parameters will use fread() syntax, make sure to follow them. */
#define PF_parameters (* (* (*)(,make))                                            _ptr[152].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[153].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[154].ptr)
//         * NAME curl_easy_strerror()
#define PF_curl_easy_strerror (* (* NAME (*)())                                    _ptr[155].ptr)
//         /* Callback function for closing socket (instead of close(2)). The callback
#define PF_Callback (* (/* (*)(instead))                                           _ptr[156].ptr)
//         CURLSOCKTYPE_ACCEPT, /* socket created by accept() call */
#define PF_socket (* (CURLSOCKTYPE_ACCEPT,/* (*)())                                _ptr[157].ptr)
//         /* Callback function for opening socket (instead of socket(2)). Optionally,
#define PF_Callback (* (/* (*)(instead))                                           _ptr[158].ptr)
//         /* Callback function for closing socket (instead of close(2)). The callback
#define PF_Callback (* (/* (*)(instead))                                           _ptr[159].ptr)
//         #define CURL_VERSION_UNIX_SOCKETS (1<<19) /* Unix domain sockets support */
#define PF_CURL_VERSION_UNIX_SOCKETS (* (#define (*)(1<<19))                       _ptr[160].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[161].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[162].ptr)
//         CURL_EXTERN CURLSH *curl_share_init(void);
#define PF_*curl_share_init (* (CURL_EXTERN CURLSH (*)(void))                      _ptr[163].ptr)
//         CURL_EXTERN CURLSHcode curl_share_setopt(CURLSH *, CURLSHoption option, ...);
#define PF_curl_share_setopt (* (CURL_EXTERN CURLSHcode (*)(CURLSH,CURLSHoption,...))    _ptr[164].ptr)
//         #define curl_share_setopt(share,opt,param) curl_share_setopt(share,opt,param)
#define PF_curl_share_setopt (* (#define (*)(share,opt,param))                     _ptr[165].ptr)
//         CURL_EXTERN CURLSHcode curl_share_cleanup(CURLSH *);
#define PF_curl_share_cleanup (* (CURL_EXTERN CURLSHcode (*)(CURLSH))              _ptr[166].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[167].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[168].ptr)
//         * NAME curl_easy_strerror()
#define PF_curl_easy_strerror (* (* NAME (*)())                                    _ptr[169].ptr)
//         * NAME curl_share_strerror()
#define PF_curl_share_strerror (* (* NAME (*)())                                   _ptr[170].ptr)
//         CURLSOCKTYPE_ACCEPT, /* socket created by accept() call */
#define PF_socket (* (CURLSOCKTYPE_ACCEPT,/* (*)())                                _ptr[171].ptr)
//         /* Callback function for opening socket (instead of socket(2)). Optionally,
#define PF_Callback (* (/* (*)(instead))                                           _ptr[172].ptr)
//         /* Callback function for closing socket (instead of close(2)). The callback
#define PF_Callback (* (/* (*)(instead))                                           _ptr[173].ptr)
//         * NAME curl_getdate()
#define PF_curl_getdate (* (* NAME (*)())                                          _ptr[174].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[175].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[176].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[177].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[178].ptr)
//         * NAME curl_slist_append()
#define PF_curl_slist_append (* (* NAME (*)())                                     _ptr[179].ptr)
//         * NAME curl_slist_free_all()
#define PF_curl_slist_free_all (* (* NAME (*)())                                   _ptr[180].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[181].ptr)
//         * NAME curl_mime_headers()
#define PF_curl_mime_headers (* (* NAME (*)())                                     _ptr[182].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[183].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[184].ptr)
//         * NAME curl_mime_headers()
#define PF_curl_mime_headers (* (* NAME (*)())                                     _ptr[185].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[186].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[187].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[188].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[189].ptr)
//         /* Callback function for closing socket (instead of close(2)). The callback
#define PF_Callback (* (/* (*)(instead))                                           _ptr[190].ptr)
//         * NAME curl_getenv()
#define PF_curl_getenv (* (* NAME (*)())                                           _ptr[191].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[192].ptr)
//         (unrelated to network send operations). */
#define PF_ (* ( (*)(unrelated))                                                   _ptr[193].ptr)
//         /* Continue to send authentication (user+password) when following locations,
#define PF_Continue (* (/* (*)(user+password,))                                    _ptr[194].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[195].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[196].ptr)
//         /* Callback function for opening socket (instead of socket(2)). Optionally,
#define PF_Callback (* (/* (*)(instead))                                           _ptr[197].ptr)
//         wait till it's ready and try again (Added
#define PF_till (* (wait (*)(Added))                                               _ptr[198].ptr)
//         /* Function that will be called to read the input (instead of fread). The
#define PF_Function (* (/* (*)(instead))                                           _ptr[199].ptr)
//         * parameters will use fread() syntax, make sure to follow them. */
#define PF_parameters (* (* (*)(,make))                                            _ptr[200].ptr)
//         /* Callback function for closing socket (instead of close(2)). The callback
#define PF_Callback (* (/* (*)(instead))                                           _ptr[201].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[202].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[203].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[204].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[205].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[206].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[207].ptr)
//         /* Function that will be called to read the input (instead of fread). The
#define PF_Function (* (/* (*)(instead))                                           _ptr[208].ptr)
//         * parameters will use fread() syntax, make sure to follow them. */
#define PF_parameters (* (* (*)(,make))                                            _ptr[209].ptr)
//         * NAME curl_mime_init()
#define PF_curl_mime_init (* (* NAME (*)())                                        _ptr[210].ptr)
//         * NAME curl_slist_free_all()
#define PF_curl_slist_free_all (* (* NAME (*)())                                   _ptr[211].ptr)
//         * NAME curl_mime_free()
#define PF_curl_mime_free (* (* NAME (*)())                                        _ptr[212].ptr)
//         * NAME curl_mime_addpart()
#define PF_curl_mime_addpart (* (* NAME (*)())                                     _ptr[213].ptr)
//         * NAME curl_mime_name()
#define PF_curl_mime_name (* (* NAME (*)())                                        _ptr[214].ptr)
//         * NAME curl_mime_filename()
#define PF_curl_mime_filename (* (* NAME (*)())                                    _ptr[215].ptr)
//         * NAME curl_mime_data()
#define PF_curl_mime_data (* (* NAME (*)())                                        _ptr[216].ptr)
//         * NAME curl_mime_data_cb()
#define PF_curl_mime_data_cb (* (* NAME (*)())                                     _ptr[217].ptr)
//         * NAME curl_mime_filedata()
#define PF_curl_mime_filedata (* (* NAME (*)())                                    _ptr[218].ptr)
//         * NAME curl_mime_type()
#define PF_curl_mime_type (* (* NAME (*)())                                        _ptr[219].ptr)
//         * NAME curl_mime_encoder()
#define PF_curl_mime_encoder (* (* NAME (*)())                                     _ptr[220].ptr)
//         CURL_EXTERN CURLcode curl_mime_encoder(curl_mimepart *part,
#define PF_curl_mime_encoder (* (CURL_EXTERN CURLcode (*)(curl_mimepart,))         _ptr[221].ptr)
//         * NAME curl_mime_headers()
#define PF_curl_mime_headers (* (* NAME (*)())                                     _ptr[222].ptr)
//         * NAME curl_mime_data_cb()
#define PF_curl_mime_data_cb (* (* NAME (*)())                                     _ptr[223].ptr)
//         * NAME curl_mime_subparts()
#define PF_curl_mime_subparts (* (* NAME (*)())                                    _ptr[224].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[225].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[226].ptr)
//         /* Function that will be called to store the output (instead of fwrite). The
#define PF_Function (* (/* (*)(instead))                                           _ptr[227].ptr)
//         * parameters will use fwrite() syntax, make sure to follow them. */
#define PF_parameters (* (* (*)(,make))                                            _ptr[228].ptr)
//         /* Function that will be called to store headers (instead of fwrite). The
#define PF_Function (* (/* (*)(instead))                                           _ptr[229].ptr)
//         * parameters will use fwrite() syntax, make sure to follow them. */
#define PF_parameters (* (* (*)(,make))                                            _ptr[230].ptr)
//         /* Function that will be called to read the input (instead of fread). The
#define PF_Function (* (/* (*)(instead))                                           _ptr[231].ptr)
//         * parameters will use fread() syntax, make sure to follow them. */
#define PF_parameters (* (* (*)(,make))                                            _ptr[232].ptr)
//         * NAME curl_slist_append()
#define PF_curl_slist_append (* (* NAME (*)())                                     _ptr[233].ptr)
//         * NAME curl_slist_free_all()
#define PF_curl_slist_free_all (* (* NAME (*)())                                   _ptr[234].ptr)
//         * curl_easy_setopt(). The first argument in the CINIT() macro is the [what]
#define PF_curl_easy_setopt (* (* (*)(.))                                          _ptr[235].ptr)
//         #define curl_easy_setopt(handle,opt,param) curl_easy_setopt(handle,opt,param)
#define PF_curl_easy_setopt (* (#define (*)(handle,opt,param))                     _ptr[236].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[237].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[238].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[239].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[240].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[241].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[242].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[243].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[244].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[245].ptr)
//         * NAME curl_slist_append()
#define PF_curl_slist_append (* (* NAME (*)())                                     _ptr[246].ptr)
//         * NAME curl_slist_free_all()
#define PF_curl_slist_free_all (* (* NAME (*)())                                   _ptr[247].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[248].ptr)
//         * NAME curl_getenv()
#define PF_curl_getenv (* (* NAME (*)())                                           _ptr[249].ptr)
//         * NAME curl_getenv()
#define PF_curl_getenv (* (* NAME (*)())                                           _ptr[250].ptr)
//         * Returns a malloc()'ed string that MUST be curl_free()ed after usage is
#define PF_Returns (* (* (*)('ed))                                                 _ptr[251].ptr)
//         * NAME curl_free()
#define PF_curl_free (* (* NAME (*)())                                             _ptr[252].ptr)
//         * NAME curl_slist_free_all()
#define PF_curl_slist_free_all (* (* NAME (*)())                                   _ptr[253].ptr)
//         /* size of the POST input data, if strlen() is not good to use */
#define PF_size (* (/* (*)())                                                      _ptr[254].ptr)
//         /* Function that will be called to read the input (instead of fread). The
#define PF_Function (* (/* (*)(instead))                                           _ptr[255].ptr)
//         * parameters will use fread() syntax, make sure to follow them. */
#define PF_parameters (* (* (*)(,make))                                            _ptr[256].ptr)
//         * NAME curl_global_sslset()
#define PF_curl_global_sslset (* (* NAME (*)())                                    _ptr[257].ptr)
//         * When built with multiple SSL backends, curl_global_sslset() allows to
#define PF_When (* (* (*)())                                                       _ptr[258].ptr)
//         CURL_EXTERN CURLsslset curl_global_sslset(curl_sslbackend id, const char *name,
#define PF_curl_global_sslset (* (CURL_EXTERN CURLsslset (*)(curl_sslbackend,const,))    _ptr[259].ptr)


// -------------- all funcs array ---------------
struct FUNCS_ {
  const char *name;
  void  (*ptr)(void);
};

inline const FUNCS_* load()
{
    union
    {
    void *p;
    void (*fp)(void);
    } u;

    static struct FUNCS_ _funcs[] ={
        {"Callback", nullptr},
        {"Callback", nullptr},
        {"till", nullptr},
        {"Function", nullptr},
        {"parameters", nullptr},
        {"Function", nullptr},
        {"parameters", nullptr},
        {"Function", nullptr},
        {"parameters", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"curl_getdate", nullptr},
        {"curl_mime_name", nullptr},
        {"curl_mime_name", nullptr},
        {"Returns", nullptr},
        {"curl_formadd", nullptr},
        {"curl_formadd", nullptr},
        {"Serialize", nullptr},
        {"Free", nullptr},
        {"size", nullptr},
        {"curl_formfree", nullptr},
        {"curl_formfree", nullptr},
        {"curl_mime_init", nullptr},
        {"*curl_mime_init", nullptr},
        {"curl_mime_subparts", nullptr},
        {"curl_mime_subparts", nullptr},
        {"curl_mime_addpart", nullptr},
        {"*curl_mime_addpart", nullptr},
        {"curl_mime_headers", nullptr},
        {"curl_mime_headers", nullptr},
        {"curl_mime_type", nullptr},
        {"curl_mime_type", nullptr},
        {"Function", nullptr},
        {"parameters", nullptr},
        {"curl_mime_data_cb", nullptr},
        {"curl_mime_data_cb", nullptr},
        {"curl_mime_filename", nullptr},
        {"curl_mime_filename", nullptr},
        {"curl_mime_data", nullptr},
        {"curl_mime_data", nullptr},
        {"curl_mime_data_cb", nullptr},
        {"curl_mime_filedata", nullptr},
        {"curl_mime_filedata", nullptr},
        {"callback", nullptr},
        {"curl_formget", nullptr},
        {"curl_formget", nullptr},
        {"curl_formget", nullptr},
        {"size", nullptr},
        {"curl_getdate", nullptr},
        {"curl_strequal", nullptr},
        {"curl_strnequal", nullptr},
        {"curl_slist_free_all", nullptr},
        {"curl_slist_free_all", nullptr},
        {"Set", nullptr},
        {"Function", nullptr},
        {"parameters", nullptr},
        {"Function", nullptr},
        {"parameters", nullptr},
        {"size", nullptr},
        {"curl_mime_headers", nullptr},
        {"curl_getdate", nullptr},
        {"Function", nullptr},
        {"parameters", nullptr},
        {"Function", nullptr},
        {"parameters", nullptr},
        {"Continue", nullptr},
        {"size", nullptr},
        {"curl_easy_strerror", nullptr},
        {"const", nullptr},
        {"size", nullptr},
        {"CURL_VERSION_NTLM_WB", nullptr},
        {"socket", nullptr},
        {"indicates", nullptr},
        {"Time-out", nullptr},
        {"curl_getdate", nullptr},
        {"Set", nullptr},
        {"Function", nullptr},
        {"parameters", nullptr},
        {"Function", nullptr},
        {"parameters", nullptr},
        {"Function", nullptr},
        {"parameters", nullptr},
        {"curl_easy_unescape", nullptr},
        {"*curl_easy_unescape", nullptr},
        {"size", nullptr},
        {"curl_getenv", nullptr},
        {"*curl_getenv", nullptr},
        {"curl_easy_strerror", nullptr},
        {"curl_share_strerror", nullptr},
        {"const", nullptr},
        {"curl_version", nullptr},
        {"*curl_version", nullptr},
        {"curl_version_info", nullptr},
        {"*curl_version_info", nullptr},
        {"curl_version_info", nullptr},
        {"curl_getenv", nullptr},
        {"curl_getenv", nullptr},
        {"curl_easy_escape", nullptr},
        {"*curl_easy_escape", nullptr},
        {"size", nullptr},
        {"*curl_escape", nullptr},
        {"curl_easy_unescape", nullptr},
        {"*curl_unescape", nullptr},
        {"Returns", nullptr},
        {"curl_free", nullptr},
        {"curl_free", nullptr},
        {"size", nullptr},
        {"Set", nullptr},
        {"curl_slist_free_all", nullptr},
        {"curl_slist_append", nullptr},
        {"struct", nullptr},
        {"Continue", nullptr},
        {"size", nullptr},
        {"till", nullptr},
        {"Function", nullptr},
        {"parameters", nullptr},
        {"curl_getenv", nullptr},
        {"size", nullptr},
        {"Function", nullptr},
        {"parameters", nullptr},
        {"curl_strequal", nullptr},
        {"Returns", nullptr},
        {"curl_mime_free", nullptr},
        {"curl_mime_free", nullptr},
        {"curl_formfree", nullptr},
        {"Returns", nullptr},
        {"curl_free", nullptr},
        {"curl_slist_free_all", nullptr},
        {"curl_global_init_mem", nullptr},
        {"curl_global_init", nullptr},
        {"curl_global_init", nullptr},
        {"curl_global_init", nullptr},
        {"curl_global_init_mem", nullptr},
        {"curl_global_init", nullptr},
        {"curl_global_init_mem", nullptr},
        {"curl_global_init", nullptr},
        {"curl_global_init_mem", nullptr},
        {"curl_global_init_mem", nullptr},
        {"curl_global_init", nullptr},
        {"curl_global_cleanup", nullptr},
        {"curl_global_cleanup", nullptr},
        {"curl_global_cleanup", nullptr},
        {"curl_multi_setopt", nullptr},
        {"curl_easy_getinfo", nullptr},
        {"curl_slist_free_all", nullptr},
        {"curl_easy_pause", nullptr},
        {"curl_easy_pause", nullptr},
        {"till", nullptr},
        {"Function", nullptr},
        {"parameters", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"curl_easy_strerror", nullptr},
        {"Callback", nullptr},
        {"socket", nullptr},
        {"Callback", nullptr},
        {"Callback", nullptr},
        {"CURL_VERSION_UNIX_SOCKETS", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"*curl_share_init", nullptr},
        {"curl_share_setopt", nullptr},
        {"curl_share_setopt", nullptr},
        {"curl_share_cleanup", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"curl_easy_strerror", nullptr},
        {"curl_share_strerror", nullptr},
        {"socket", nullptr},
        {"Callback", nullptr},
        {"Callback", nullptr},
        {"curl_getdate", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"curl_slist_append", nullptr},
        {"curl_slist_free_all", nullptr},
        {"size", nullptr},
        {"curl_mime_headers", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"curl_mime_headers", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"Callback", nullptr},
        {"curl_getenv", nullptr},
        {"size", nullptr},
        {"Continue", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"Callback", nullptr},
        {"till", nullptr},
        {"Function", nullptr},
        {"parameters", nullptr},
        {"Callback", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"Function", nullptr},
        {"parameters", nullptr},
        {"curl_mime_init", nullptr},
        {"curl_slist_free_all", nullptr},
        {"curl_mime_free", nullptr},
        {"curl_mime_addpart", nullptr},
        {"curl_mime_name", nullptr},
        {"curl_mime_filename", nullptr},
        {"curl_mime_data", nullptr},
        {"curl_mime_data_cb", nullptr},
        {"curl_mime_filedata", nullptr},
        {"curl_mime_type", nullptr},
        {"curl_mime_encoder", nullptr},
        {"curl_mime_encoder", nullptr},
        {"curl_mime_headers", nullptr},
        {"curl_mime_data_cb", nullptr},
        {"curl_mime_subparts", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"Function", nullptr},
        {"parameters", nullptr},
        {"Function", nullptr},
        {"parameters", nullptr},
        {"Function", nullptr},
        {"parameters", nullptr},
        {"curl_slist_append", nullptr},
        {"curl_slist_free_all", nullptr},
        {"curl_easy_setopt", nullptr},
        {"curl_easy_setopt", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"size", nullptr},
        {"curl_slist_append", nullptr},
        {"curl_slist_free_all", nullptr},
        {"size", nullptr},
        {"curl_getenv", nullptr},
        {"curl_getenv", nullptr},
        {"Returns", nullptr},
        {"curl_free", nullptr},
        {"curl_slist_free_all", nullptr},
        {"size", nullptr},
        {"Function", nullptr},
        {"parameters", nullptr},
        {"curl_global_sslset", nullptr},
        {"When", nullptr},
        {"curl_global_sslset", nullptr},
        {nullptr, nullptr}
    };
    void  *dll_handle;
    if ((dll_handle = dlopen("/usr/lib/x86_64-linux-gnu/libcurl.a", RTLD_LAZY)) == 0)
    {
        perror("cannot load:");
        return nullptr;
    }
    struct FUNCS_ *fp = _funcs;
    for (; fp->name != nullptr; fp++)
    {
        u.p = dlsym(dll_handle, fp->name);
        if (u.fp == 0)
        {
            perror("cannot load:");
        }
        else
        {
            fp->ptr = u.fp;
        }
    }
    return _funcs;
}
#endif // LIB_RESOLVER
/*
 add this to cpp 
const FUNCS_* _ptr; // global var
  _ptr = load(); // in main() 
*/
 
